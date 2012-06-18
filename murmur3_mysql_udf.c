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
#include "lib/murmur3.h"

#define MUR3_32_SEED ((uint32_t) 0)

/* Prototypes */

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
    if (args->arg_count != 1) {
	strcpy(message, "mur3_32 requires exactly one parameter");
	return 1;
    }
    args->arg_type[0] = STRING_RESULT;     /** coerces first parameter to string */
    initid->max_length = sizeof(uint32_t);
    initid->maybe_null = 1;                /** a null key hashes to null */
    return 0;
}


/**
 * Hashes value using 32-bit variant of MurmurHash 3, a non-cryptographic
 * hashing function.
 */
long long mur3_32(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error)
{
    const char *key = args->args[0];
    int len = args->lengths[0];
    uint32_t hash[1];

    if (args->args[0] == NULL) {
	*is_null = 1;
	return (long long) NULL;
    }
    MurmurHash3_x86_32(key, len, MUR3_32_SEED, hash);
    return hash[0];
}
