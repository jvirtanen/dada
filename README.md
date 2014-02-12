Dada
====

Dada is a simple program for generating tabular text data.


Requirements
------------

Dada requires a C99 compiler and GNU Make.


Installation
------------

To build, run:

    make

To install, run:

    make install

> The default installation location is `$HOME`. Define the environment variable
> `$PREFIX` to specify an alternative installation location. For example:
>
>     PREFIX="/usr/local" make install


Usage
-----

To generate one kilobyte of [tab-separated values][TSV], run:

    dada

You can specify the size of the data with the `-s` option. To generate one
gigabyte of data, run:

    dada -s1g

If you want to generate, for example, [comma-separated values][CSV] instead of
TSV, you have to set the field delimiter with the `-d` option. To generate CSV,
run:

    dada -d,

By default, the number of columns is randomly chosen. However, you can set it
with the `-c` option. To generate data with eight columns, run:

    dada -c8

  [CSV]: http://tools.ietf.org/html/rfc4180
  [TSV]: http://www.iana.org/assignments/media-types/text/tab-separated-values


License
-------

Dada is released under the MIT License. See `LICENSE` for details.
