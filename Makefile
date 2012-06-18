CC = gcc
MYSQLD = /usr/local/bin/mysqld
MYSQL_CFLAGS = $(shell mysql_config --cflags)

all:
	$(CC) -Wall -bundle -bundle_loader $(MYSQLD) -o murmur3_mysql_udf.so $(MYSQL_CFLAGS) murmur3_mysql_udf.c lib/murmur3/murmur3.c

clean:
	rm -rf murmur3_mysql_udf.so.dSYM murmur3_mysql_udf.so
