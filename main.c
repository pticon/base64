#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <stdint.h>
#include <unistd.h>


#include "base64.h"

#define VERSION		"1.0"

/* Buffer len should be a 3 multiple for encoding to avoid junk characters
 */
#define BUFFER_ENCODE_LEN	99

/* Buffer len should be a 4 multiple for decoding
 */
#define BUFFER_DECODE_LEN	100


static void usage(const char * progname)
{
	fprintf(stderr, "usage: %s [options] <file>\n", progname);
	fprintf(stderr, "options:\n");
	fprintf(stderr, "\t-h: display this and exit\n");
	fprintf(stderr, "\t-d: decode rather than encode\n");
	fprintf(stderr, "\t-v: display version and exit\n");
}

static void display_version(const char * progname)
{
	printf("version: %s %s\n", progname, VERSION);
}

int main(int argc, char * argv[])
{
	const char * progname = argv[0];
	const char * filename;
	FILE * file = NULL;
	size_t nbytes = 0;
	int ch;
	uint8_t buffer[BUFFER_DECODE_LEN];
	uint8_t * base64 = NULL;
	size_t base64_len;
	unsigned buffer_len = BUFFER_ENCODE_LEN;

	/* Default is encode
	 */
	uint8_t * (* fct)(const uint8_t *, size_t, size_t *) = base64_encode;

	/* Parse args
	 */
	while ( (ch = getopt(argc, argv, "hdv")) != -1 )
	{
		switch(ch)
		{
			case 'h':
			usage(progname);
			return 0;

			case 'd':
			fct = base64_decode;
			buffer_len = BUFFER_DECODE_LEN;
			break;

			case 'v':
			display_version(progname);
			return 0;
		}
	}
	argc -= optind;
	argv += optind;

	/* Check args
	 */
	if ( argc < 1 )
	{
		usage(progname);
		return -1;
	}

	/* Assign the filename and open it
	 */
	filename = argv[0];
	if ( (file = fopen(filename, "rb")) == NULL )
	{
		fprintf(stderr, "Cannot open %s: %s\n", filename, strerror(errno));
		return -1;
	}

	/* Parse the file
	 */
	while ( (nbytes = fread(buffer, sizeof(unsigned char), buffer_len, file)) != 0 )
	{
		/* Apply the callback on the buffer
		 */
		if ( (base64 = fct(buffer, nbytes, &base64_len)) == NULL )
		{
			break;
		}

		fprintf(stdout, "%.*s", (int)base64_len, base64);
		free(base64);
	}

	/* Check if we breaked with an error
	 */
	if ( feof(file) == 0 )
	{
		fprintf(stderr, "Did not process the whole file %s\n", filename);
	}

	/* Close the input file
	 */
	if ( file != NULL )
	{
		if ( fclose(file) != 0 )
		{
			fprintf(stderr, "Cannot close %s: %s\n", filename, strerror(errno));
			return -1;
		}
	}

	return 0;
}
