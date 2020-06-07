#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

int main(int argc, char *argv[]) {

	if (argc == 4) {
		FILE *input = fopen(argv[2], "r");
		FILE *output = fopen(argv[3], "w");

		if (strcmp(argv[1], "-c1") == 0)
			c1(input, output);

		if (strcmp(argv[1], "-c2") == 0)
			c2(input, output);

		fclose(input);
		fclose(output);
	}
	return 0;
}