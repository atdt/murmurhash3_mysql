#!/usr/bin/env bash

# This is a kludge and should be re-done as a proper Makefile.

LIBDIR=$(mysql -u root -p <<< "show variables like 'plugin_dir';" | tail -1 | awk '{ print $2 }')
gcc -fPIC -Wall -I/usr/include/mysql -I. -shared murmur3_mysql_udf.c lib/murmur3/murmur3.c -o "${LIBDIR}/murmur3_mysql_udf.so"
chgrp -R mysql "${LIBDIR}/murmur3_mysql_udf.so"
chown -R mysql "${LIBDIR}/murmur3_mysql_udf.so"
