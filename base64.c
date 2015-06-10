#include <sys/types.h>
#include <stdint.h>
#include <stdlib.h>

#include "base64.h"



/* Base 64 (rfc3548)
 */
static const char base64_table [] =
{
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
};


/* Encode 3 8-bit binary byte as 4 6-bit characters
 */
static inline void __base64_encode_block(const uint8_t * in, uint8_t * out, int len)
{
	out [0] = base64_table[ in[0] >> 2 ];
	out [1] = base64_table[ ((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4) ];
	if (len > 1)
	{
		out [2] = base64_table[ ((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6) ];
		if (len > 2)
			out [3] = base64_table[ in[2] & 0x3f ];
		else
			out [3] = '=';
	}
	else
	{
		out [2] = '=';
		out [3] = '=';
	}
}


/* Encode a string helping with base64
 */
uint8_t * base64_encode(const uint8_t * in, size_t input_len, size_t *output_len)
{
	uint8_t * data;
	int i,j, len = input_len;

	switch ( input_len % 3 )
	{
		case 1: len++;
		case 2: len++;
		default: break;
	}
	len = (size_t) (4 * (len / 3));

	if ( (data = malloc(len)) == NULL )
		return NULL;

	for (i=0,j=0; j<len; i+=3,j+=4)
	{
		__base64_encode_block(in + i, data + j, input_len);
		input_len-=3;
	}

	if ( output_len )
		*output_len = len;

	return data;
}


/* Check if the buffer is base64 encoded
 */
int base64_decode_check(const uint8_t * in, size_t input_len)
{
	unsigned int i;

	if ( (input_len % 4) != 0 )
		return 0;

	for ( i=0; i<input_len; i++)
	{
		if ( ! IS_BASE64( in[i] ) )
			return 0;
	}

	return 1;
}

/* Decode 4 6-bit characters as 3 8-bit characters
 */
static inline uint8_t __base64_value_char(const char a)
{
	if ( IS_UPPER(a) ) return (a - 'A');
	if ( IS_LOWER(a) ) return (a + 26 - 'a');
	if ( IS_DIGIT(a) ) return (a + 52 - '0');
	if ( a == '+' ) return 62;
	return 63; // eg '/'
}

static inline void __base64_decode_block(const uint8_t * in, uint8_t * out, int max)
{
	if ( max )
		out[0] = (__base64_value_char(in[0]) << 2) | (__base64_value_char(in[1]) >> 4);
	if ( max > 1 )
		out[1] = (__base64_value_char(in[1]) << 4) | (__base64_value_char(in[2]) >> 2);
	if ( max > 2 )
		out[2] = ((__base64_value_char(in[2]) << 6) & 0xc0) | __base64_value_char(in[3]);
}

/* Decode characters helping with the base64
 */
uint8_t * base64_decode(const uint8_t * in, size_t input_len, size_t * output_len)
{
	uint8_t * out;
	unsigned int i,j,len;

	if ( base64_decode_check(in, input_len) != 1 )
		return NULL;

	len = (input_len * 3) / 4;
	if ( in[input_len - 1] == '=' ) len--;
	if ( in[input_len - 2] == '=' ) len--;

	if ( (out = malloc(len)) == NULL )
		return NULL;

	for (i=0,j=0; i<input_len; i+=4,j+=3)
		__base64_decode_block(in + i, out + j, len - j);

	if ( output_len )
		*output_len = len;

	return out;
}
