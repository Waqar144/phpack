#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/param.h>
#include <string>

#include <stdint.h>

#include "pack.h"

#if HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#include <assert.h>

#define INC_OUTPUTPOS(a,b) \
        if ((a) < 0 || ((INT_MAX - outputpos)/((int)b)) < (a)) { \
                printf( "Type %c: integer overflow in format string", code); \
        } \
        outputpos += (a)*(b);

#define safe_emalloc(len,objsize,setwith)	malloc( len*objsize )

#define ToCString(value)   (*value ? *value : "<string conversion failed>")

#if defined(__x86_64__) || defined(__LP64__) || defined(_LP64) || defined(_WIN64)
# define ENABLE_LONG64 1
#endif

#ifdef ENABLE_LONG64
# define SIZEOF_ZEND_LONG 8
#else
# define SIZEOF_ZEND_LONG 4
#endif

inline double ToDouble(int value)
{
	return static_cast<double>(value);
}


/* Whether machine is little endian */
static char machine_little_endian;

/* Mapping of byte from char (8bit) to long for machine endian */
static int byte_map[1];

/* Mappings of bytes from int (machine dependant) to int for machine endian */
static int int_map[sizeof(int)];

/* Mappings of bytes from shorts (16bit) for all endian environments */
static int machine_endian_short_map[2];
static int big_endian_short_map[2];
static int little_endian_short_map[2];

/* Mappings of bytes from longs (32bit) for all endian environments */
static int machine_endian_long_map[4];
static int big_endian_long_map[4];
static int little_endian_long_map[4];

#if SIZEOF_ZEND_LONG > 4
/* Mappings of bytes from quads (64bit) for all endian environments */
static int machine_endian_longlong_map[8];
static int big_endian_longlong_map[8];
static int little_endian_longlong_map[8];
#endif


/* {{{ NODEJS_pack
 */
static void php_pack(u_int32_t val, int size, int *map, char *output)
{
    int i;
    unsigned char *v;

    //convert_to_long_ex(val);
    v = (unsigned char *) &val;

    for (i = 0; i < size; i++) {
//        if( IsDebug()>0 ) printf("currentValue every Byte value=%d\n", v[map[i]] );
        *output++ = v[map[i]];
    }
}
/* }}} */

static inline uint32_t php_pack_reverse_int32(uint32_t arg)
{
    uint32_t result;
    result = ((arg & 0xFF) << 24) | ((arg & 0xFF00) << 8) | ((arg >> 8) & 0xFF00) | ((arg >> 24) & 0xFF);

    return result;
}

static inline uint64_t php_pack_reverse_int64(uint64_t arg)
{
    union Swap64 {
        uint64_t i;
        uint32_t ul[2];
    } tmp, result;
    tmp.i = arg;
    result.ul[0] = php_pack_reverse_int32(tmp.ul[1]);
    result.ul[1] = php_pack_reverse_int32(tmp.ul[0]);

    return result.i;
}

// We define a max size, since we want to avoid any mallocs() theres are on the stack and faster.
// I think 48 is enough for 99.99% cases. Next release we might have a init option
#define MAX_FORMAT_CODES		(48)

// We also store the output locally in the stack if its under 256 bytes in size, else we malloc it.

/* pack() idea stolen from Perl (implemented formats behave the same as there)
 * Implemented formats are A, a, h, H, c, C, s, S, i, I, l, L, n, N, f, d, x, X, V, v, @.
 */
/* {{{ proto string pack(String format, mixed arg1 [, mixed arg2 [, mixed ...]])
   Takes one or more arguments and packs them into a binary string according to the format argument
 * If the first ARG is a Buffer(), is that as the output instead of creating its own buffer.
 */
std::string pack(char code, int dec)
{
    int outputpos = 0, outputsize = 0;
    int i;
    char *output = NULL;
    int formatcount = 1;

    /* Calculate output length and upper bound while processing*/
    for (i = 0; i < formatcount; i++) {
        switch (code) {
        case 'v':
            INC_OUTPUTPOS(dec, 2)		/* 16 bit per arg */
            break;
        }
        if (outputsize < outputpos) {
            outputsize = outputpos;
        }
    }

    output = (char*)malloc(outputsize);
    outputpos = 0;


    switch (code) {
    case 'v': {
        int *map = machine_endian_short_map;

        if (code == 'n') {
            map = big_endian_short_map;
        } else if (code == 'v') {
            map = little_endian_short_map;
        }

        int val = dec;
        php_pack(val, 2, map, &output[outputpos]);
        outputpos += 2;
        break;
    }
    }
    output[outputpos] = '\0';
    std::string ret{output};
    free(output);
    return ret;
}

static long php_unpack(const char *data, int size, int issigned, int *map)
{
    long result;
    char *cresult = reinterpret_cast<char *>(&result);
    int i;

    result = issigned ? -1 : 0;

    for (i = 0; i < size; i++) {
        cresult[map[i]] = *data++;
    }

    return result;
}

int unpack(char format, std::string data)
{
    const char *input = data.c_str();
    int         inputpos = 0, inputlen = 0;
    int size = 0;

    inputlen = strlen(input) + 1;

    switch (format) {
    /* Never use any input */
    case 'v':
        size = 2;
        break;
    default:
        break;
    }

    /* Do actual unpacking */
    if ((inputpos + size) <= inputlen && input != NULL) {
        switch (format) {
        case 'v': {
            long v = 0;
            int issigned = 0;
            int *map = machine_endian_short_map;

            if (format == 's') {
                issigned = input[inputpos + (machine_little_endian ? 1 : 0)] & 0x80;
            } else if (format == 'n') {
                map = big_endian_short_map;
            } else if (format == 'v') {
                map = little_endian_short_map;
            }

            v = php_unpack(&input[inputpos], 2, issigned, map);
            return (int)v;
            break;
        }
        }

        inputpos += size;
        if (inputpos < 0) {
            if (size != -1) { /* only print warning if not working with * */
                printf( "Type %c: outside of string", format);
            }
            inputpos = 0;
        }
    }
}

void Initialize()
{
    int machine_endian_check = 1;
    int i;

    machine_little_endian = ((char *)&machine_endian_check)[0];

    if (machine_little_endian) {
        /* Where to get lo to hi bytes from */
        printf("machine is litte endian\n");
        byte_map[0] = 0;

        for (i = 0; i < (int)sizeof(int); i++) {
            int_map[i] = i;
        }

        machine_endian_short_map[0] = 0;
        machine_endian_short_map[1] = 1;
        big_endian_short_map[0] = 1;
        big_endian_short_map[1] = 0;
        little_endian_short_map[0] = 0;
        little_endian_short_map[1] = 1;

        machine_endian_long_map[0] = 0;
        machine_endian_long_map[1] = 1;
        machine_endian_long_map[2] = 2;
        machine_endian_long_map[3] = 3;
        big_endian_long_map[0] = 3;
        big_endian_long_map[1] = 2;
        big_endian_long_map[2] = 1;
        big_endian_long_map[3] = 0;
        little_endian_long_map[0] = 0;
        little_endian_long_map[1] = 1;
        little_endian_long_map[2] = 2;
        little_endian_long_map[3] = 3;
#if SIZEOF_ZEND_LONG > 4
        machine_endian_longlong_map[0] = 0;
        machine_endian_longlong_map[1] = 1;
        machine_endian_longlong_map[2] = 2;
        machine_endian_longlong_map[3] = 3;
        machine_endian_longlong_map[4] = 4;
        machine_endian_longlong_map[5] = 5;
        machine_endian_longlong_map[6] = 6;
        machine_endian_longlong_map[7] = 7;
        big_endian_longlong_map[0] = 7;
        big_endian_longlong_map[1] = 6;
        big_endian_longlong_map[2] = 5;
        big_endian_longlong_map[3] = 4;
        big_endian_longlong_map[4] = 3;
        big_endian_longlong_map[5] = 2;
        big_endian_longlong_map[6] = 1;
        big_endian_longlong_map[7] = 0;
        little_endian_longlong_map[0] = 0;
        little_endian_longlong_map[1] = 1;
        little_endian_longlong_map[2] = 2;
        little_endian_longlong_map[3] = 3;
        little_endian_longlong_map[4] = 4;
        little_endian_longlong_map[5] = 5;
        little_endian_longlong_map[6] = 6;
        little_endian_longlong_map[7] = 7;
#endif

    }
    else {
        printf("machine is big endian\n");
        int val;
        int size = sizeof((val));

        /* Where to get hi to lo bytes from */
        byte_map[0] = size - 1;

        for (i = 0; i < (int)sizeof(int); i++) {
            int_map[i] = size - (sizeof(int) - i);
        }

        machine_endian_short_map[0] = size - 2;
        machine_endian_short_map[1] = size - 1;
        big_endian_short_map[0] = size - 2;
        big_endian_short_map[1] = size - 1;
        little_endian_short_map[0] = size - 1;
        little_endian_short_map[1] = size - 2;

        machine_endian_long_map[0] = size - 4;
        machine_endian_long_map[1] = size - 3;
        machine_endian_long_map[2] = size - 2;
        machine_endian_long_map[3] = size - 1;
        big_endian_long_map[0] = size - 4;
        big_endian_long_map[1] = size - 3;
        big_endian_long_map[2] = size - 2;
        big_endian_long_map[3] = size - 1;
        little_endian_long_map[0] = size - 1;
        little_endian_long_map[1] = size - 2;
        little_endian_long_map[2] = size - 3;
        little_endian_long_map[3] = size - 4;
#if SIZEOF_ZEND_LONG > 4
        machine_endian_longlong_map[0] = size - 8;
        machine_endian_longlong_map[1] = size - 7;
        machine_endian_longlong_map[2] = size - 6;
        machine_endian_longlong_map[3] = size - 5;
        machine_endian_longlong_map[4] = size - 4;
        machine_endian_longlong_map[5] = size - 3;
        machine_endian_longlong_map[6] = size - 2;
        machine_endian_longlong_map[7] = size - 1;
        big_endian_longlong_map[0] = size - 8;
        big_endian_longlong_map[1] = size - 7;
        big_endian_longlong_map[2] = size - 6;
        big_endian_longlong_map[3] = size - 5;
        big_endian_longlong_map[4] = size - 4;
        big_endian_longlong_map[5] = size - 3;
        big_endian_longlong_map[6] = size - 2;
        big_endian_longlong_map[7] = size - 1;
        little_endian_longlong_map[0] = size - 1;
        little_endian_longlong_map[1] = size - 2;
        little_endian_longlong_map[2] = size - 3;
        little_endian_longlong_map[3] = size - 4;
        little_endian_longlong_map[4] = size - 5;
        little_endian_longlong_map[5] = size - 6;
        little_endian_longlong_map[6] = size - 7;
        little_endian_longlong_map[7] = size - 8;
#endif
    }
}
