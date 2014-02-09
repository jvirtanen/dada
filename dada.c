/*
 * Copyright (c) 2012 Jussi Virtanen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <ctype.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * Dada
 * ====
 *
 * Dada generates tabular text data.
 *
 *     https://github.com/jvirtanen/dada
 */

#define VERSION "0.1.0"

struct settings {
    size_t              number_of_columns;
    char                field_delimiter;
    unsigned long long  size;
};

#define DEFAULT_FIELD_DELIMITER '\t'
#define DEFAULT_SIZE            1024

#define RECORD_SEPARATOR '\n'

#define COLUMNS_MIN 2
#define COLUMNS_MAX 16

#define NUMBER_MIN 0
#define NUMBER_MAX 1000000

#define TEXT_LENGTH_MIN 4
#define TEXT_LENGTH_MAX 8

enum column_type {
    COLUMN_TYPE_NUMBER,
    COLUMN_TYPE_TEXT
};

static int write_row(FILE *, const enum column_type *,
    const struct settings *);
static int write_field(FILE *, enum column_type);
static int write_number(FILE *);
static int write_text(FILE *);
static enum column_type generate_column_type(void);
static int xrand(int, int);

static void
usage(void)
{
    fprintf(stderr, "Usage: dada [-c columns] [-d delimiter] [-s size] [-v]\n");
    exit(EXIT_FAILURE);
}

static void
version(void)
{
    printf("%s\n", VERSION);
    exit(EXIT_SUCCESS);
}

static size_t
parse_number_of_columns(const char *p)
{
    size_t number_of_columns;
    char *q;

    number_of_columns = strtol(p, &q, 10);

    if (*q != '\0')
        number_of_columns = 0;

    return number_of_columns;
}

static unsigned long long
parse_size(const char *p)
{
    unsigned long long size;
    char *q;

    size = strtoll(p, &q, 10);

    switch (*q) {
    case 'K':
    case 'k':
        size *= 1024;
        q++;
        break;
    case 'M':
    case 'm':
        size *= 1024 * 1024;
        q++;
        break;
    case 'G':
    case 'g':
        size *= 1024 * 1024 * 1024;
        q++;
        break;
    default:
        break;
    }

    if (*q != '\0')
        size = 0;

    return size;
}

static void
init(void)
{
    srandom(time(NULL));
}

static size_t
generate_number_of_columns(void)
{
    return (size_t)xrand(COLUMNS_MIN, COLUMNS_MAX);
}

static enum column_type *
generate_column_types(size_t number_of_columns)
{
    enum column_type *column_types;
    size_t i;

    column_types = calloc(number_of_columns, sizeof(enum column_type));
    if (column_types == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < number_of_columns; i++)
        column_types[i] = generate_column_type();

    return column_types;
}

static void
write_rows(FILE *file, const enum column_type *column_types,
    const struct settings *settings)
{
    unsigned long long target;
    unsigned long long size;

    target = settings->size;
    size = 0;

    while (size < target)
        size += write_row(file, column_types, settings);
}

int
main(int argc, char *argv[])
{
    enum column_type *column_types;
    int ch;

    init();

    struct settings settings = {
        .number_of_columns  = generate_number_of_columns(),
        .field_delimiter    = DEFAULT_FIELD_DELIMITER,
        .size               = DEFAULT_SIZE
    };

    opterr = 0;

    while ((ch = getopt(argc, argv, "c:d:s:v")) != -1) {
        switch (ch) {
        case 'c':
            settings.number_of_columns = parse_number_of_columns(optarg);
            break;
        case 'd':
            settings.field_delimiter = optarg[0];
            break;
        case 's':
            settings.size = parse_size(optarg);
            break;
        case 'v':
            version();
        default:
            usage();
        }
    }

    if (settings.number_of_columns == 0)
        usage();

    if (settings.size == 0)
        usage();

    argc -= optind;
    argv += optind;

    if (argc != 0)
        usage();

    column_types = generate_column_types(settings.number_of_columns);

    write_rows(stdout, column_types, &settings);

    free(column_types);

    return 0;
}

static int
write_row(FILE *file, const enum column_type *column_types,
    const struct settings *settings)
{
    int size;
    size_t i;

    size = 0;

    for (i = 0; i < settings->number_of_columns - 1; i++) {
        size += write_field(file, column_types[i]) + 1;
        fputc(settings->field_delimiter, file);
    }
    size += write_field(file, column_types[i]) + 1;
    fputc(RECORD_SEPARATOR, file);

    return size;
}

static int
write_field(FILE *file, enum column_type column_type)
{
    int size;

    switch (column_type) {
    case COLUMN_TYPE_NUMBER:
        size = write_number(file);
        break;
    case COLUMN_TYPE_TEXT:
        size = write_text(file);
        break;
    }

    return size;
}

static int
write_number(FILE *file)
{
    return fprintf(file, "%d", xrand(NUMBER_MIN, NUMBER_MAX));
}

static int
write_text(FILE *file)
{
    int size;
    int i;

    size = xrand(TEXT_LENGTH_MIN, TEXT_LENGTH_MAX);

    for (i = 0; i < size; i++) {
        fputc(xrand('a', 'z'), file);
    }

    return size;
}

static enum column_type
generate_column_type(void)
{
    return (enum column_type)xrand(COLUMN_TYPE_NUMBER, COLUMN_TYPE_TEXT);
}

static int
xrand(int min, int max)
{
    return (int)(random() * (max - min + 0.5) / INT32_MAX + min);
}
