/**
 * @file
 * @author  Ori Livneh <ori@wikimedia.org>
 * @copyright 2012 Wikimedia Foundation
 * @version 0.1
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * MySQL User Defined Function for the 32-bit variant of MurmurHash3, a fast,
 * non-cryptographic hash function created by Austin Appleby.
 *
 */

#include <mysql.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "lib/murmur3/murmur3.h"

#define MUR3_32_SEED ((uint32_t) 0)

/** prototypes */

my_bool mur3_32_init(
        UDF_INIT *initid
,       UDF_ARGS *args
,       char *message
);

long long mur3_32(
        UDF_INIT *initid
,       UDF_ARGS *args
,       char *is_null
,       char *error
);

my_bool mur3_32_digest_init(
        UDF_INIT *initid
,       UDF_ARGS *args
,       char *message
);

char *mur3_32_digest(
        UDF_INIT *initid
,       UDF_ARGS *args
,       char *result
,       unsigned long *length
,       char *is_null
,       char *error
);

/**
 * Check for parameter and inform MySQL about the nullability and length of
 * return value.
 */
my_bool mur3_32_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
    uint i;

    if (args->arg_count < 1) {
	strcpy(message, "mur3_32() requires one or more parameters");
	return 1;
    }
    for (i = 0; i < args->arg_count; i++) {
        args->arg_type[i] = STRING_RESULT;    /** Request cast to string */
        args->maybe_null[i] = 0;              /** Reject NULLs */
    }
    initid->max_length = sizeof(uint32_t);
    initid->maybe_null = 0;
    return 0;
}


/**
 * Hashes value using 32-bit variant of MurmurHash 3, a non-cryptographic
 * hashing function.
 */
long long mur3_32(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error)
{
    uint32_t hash[1];
    uint i;

    hash[0] = MUR3_32_SEED;                   /** set initial seed */
    for (i = 0; i < args->arg_count; i++) {   /** iterate arguments */
        MurmurHash3_x86_32(args->args[i], args->lengths[i], hash[0], hash);
    }
    return hash[0];
}
