
#include <stdio.h>

static unsigned char inbuf[BUFSIZ];
static unsigned char outbuf[BUFSIZ];

FILE __iob[FOPEN_MAX] = {
	{
		.fd = 0,
		.buf = inbuf,
		.len = BUFSIZ,
		.flags = _IOREAD
	},
	{
		.fd = 1,
		.buf = outbuf,
		.len = BUFSIZ,
		.flags = _IOWRITE | _IOLBF
	},
	{
		.fd = 2,
		.buf = stderr->unbuf,
		.len = sizeof(stderr->unbuf),
		.flags = _IOWRITE
	},
};
