/*
  BTL: 10 GREEN BOTTLES

  Build:
    gcc btl.c -o btl
  
  Usage:
    btl
      => 10 Green Bottles
    btl -b 20 -c blue
      => 20 Blue Bottles
    btl --bottles 30 --colour red
      => 30 Red Bottles
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#define BTL_VERSION "1.1.0.6"
#define BTL_COPYRIGHT "(c) Benedict W. Hazel, 2012"
#define BTL_OPEN_SOURCE "This is free software, released under the GNU Lesser General Public License (LGPL)"

char* colour;
int bottles;

int bottlesSet = 0, colourSet = 0;

int c, i, oi;

char plural;

int main(int argc, char** argv)
{
#ifndef NO_CGI
    printf("Content-Type: text/plain\n\n");
#endif
	struct option options[] =
	{
		{"bottles", required_argument, 0, 'b'},
		{"colour", required_argument, 0, 'c'},
		{"version", no_argument, 0, 'v'},
		{0, 0, 0, 0}
	};

	opterr = 0;
	while (1)
	{
		c = getopt_long(argc, argv, "b:c:v", options, &oi);
		if (c == -1) break;

		switch (c)
		{
			case 'b':
				bottles = atoi(optarg);
				if (bottles != 0) bottlesSet = 1;
				break;
			case 'c':
				if (strlen(optarg) != 0)
				{
					colour = (char*)malloc(sizeof(char) * strlen(optarg));
					strcpy(colour, optarg);
					colourSet = 1;
				}
				break;
			case 'v':
				printf("Bottles %s\n", BTL_VERSION);
				printf("%s\n", BTL_COPYRIGHT);
				printf("%s\n", BTL_OPEN_SOURCE);
				return 0;
				break;
			case '?':
				fprintf(stderr, "Unrecognised option -%c\n", optopt);
				return 1;
				break;
			default:
				break;
		}
	}

	if (!bottlesSet) bottles = 10;
	if (!colourSet)
	{
		colour = (char*)malloc(sizeof(char) * 5);
		strcpy(colour, "green");
	}

	plural = 's';

	for (i = bottles; i >= 1; i--)
	{
		printf("%d %s bottle%c sitting on the wall,\n", i, colour, plural);
		printf("%d %s bottle%c sitting on the wall,\n", i, colour, plural);
		printf("And if 1 %s bottle were to accidentally fall,\n", colour);
		if (i == 2) plural = 0;
		else if (i == 1) plural = 's';
		printf("There'll be %d %s bottle%c sitting on the wall.\n", i - 1, colour, plural);
		if (i != 1) printf("\n");
	}
	if (colour != NULL) free(colour);
	return 0;
}