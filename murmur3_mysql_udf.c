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
 * MySQL User Defined Function for the 32-bit variant of MurmurHash3, a fast
 * non-cryptographic hash.
 *
 * To install, run `make`, copy to MySQL plugins folder, and run the following
 * queries:
 *
 * DROP FUNCTION IF EXISTS mur3_32;
 * CREATE FUNCTION mur3_32 RETURNS INTEGER SONAME "murmur3_mysql_udf.so";
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
    long long seed;

    if (args->arg_count < 1 || args->arg_count > 2) {
	strcpy(message, "mur3_32 accepts at least one and at most two params");
	return 1;
    }

    args->maybe_null[0] = 0;              /** Reject NULL key values */

    args->arg_type[0] = STRING_RESULT;    /** Coerce key parameter to string */

    if (args->arg_count == 2) {           /** Ensure seed is castable as uint32_t */
	args->maybe_null[1] = 0;          /** Reject NULL seed values */
	args->arg_type[1] = INT_RESULT;   /** Coerce seed parameter to integer */
	seed = *((long long*) args->args[1]);
	if (seed < 0 || seed > UINT32_MAX) {
	    snprintf(message, MYSQL_ERRMSG_SIZE,
		    "seed must be between 0 and %hu", UINT32_MAX);
	}
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
    long long seed;
    
    if (args->arg_count == 2) {
	seed = *((long long*) args->args[1]);
    } else {  /** no seed param, so use default */
	seed = MUR3_32_SEED;
    }

    const char *key = args->args[0];
    int len = args->lengths[0];
    uint32_t hash[1];

    MurmurHash3_x86_32(key, len, (uint32_t) seed, hash);
    return hash[0];
}
