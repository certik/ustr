#include <stdlib.h>
#include <stdio.h>

int main(void)
{ /* output a "1" is it's a 64 bit platform. Major hack. */
	size_t val = -1;

	puts((val == 0xFFFFFFFF) ? "0" : "1");

	return 0;
}
