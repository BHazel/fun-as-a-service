/*
  PT: POTATOES!
  
  Build:
    gcc pt.c -o pt
    
   Usage:
    pt --count 12
    pt --count 12 --potatoes
    pt -c 12 -p
      => Count 12 potatoes.
    pt --count 16 --tomates
    pt -c 16 -t
      => Count 16 tomatoes.
    pt --count 20 --boozy
    pt -c 20 -b
      => Count 20 tequilas!
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#define PT_VERSION "1.1.1.10"
#define PT_COPYRIGHT "(c) Benedict W. Hazel, 2012"
#define PT_OPEN_SOURCE "This is free software, released under the GNU Lesser General Public License (LGPL)"

int countSet = 0, potatoesSet = 0, tomatoesSet = 0, boozySet = 0;
int count = 0;
char* food;
char* final;
int i, oi;
char c;

int main(int argc, char** argv)
{
#ifndef NO_CGI
    printf("Content-Type: text/plain\n\n");
#endif
	struct option options[] =
	{
		{"count", required_argument, 0, 'c'},
		{"potatoes", no_argument, 0, 'p'},
		{"tomatoes", no_argument, 0, 't'},
		{"boozy", no_argument, 0, 'b'},
		{"version", no_argument, 0, 'v'},
		{0, 0, 0, 0}
	};

	opterr = 0;
	while (1)
	{
		c = getopt_long(argc, argv, "bc:ptv", options, &oi);
		if (c == -1) break;

		switch (c)
		{
			case 'c':
				count = atoi(optarg);
				if (count != 0) countSet = 1;
				if (count % 4 != 0)
				{
					fprintf(stderr, "Count not divisible by 4\n");
					return 1;
				}
				break;
			case 'p':
				potatoesSet = 1;
				break;
			case 't':
				tomatoesSet = 1;
				break;
			case 'b':
				boozySet = 1;
				break;
			case 'v':
				printf("Potatoes %s\n", PT_VERSION);
				printf("%s\n", PT_COPYRIGHT);
				printf("%s\n", PT_OPEN_SOURCE);
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
	if (!countSet) count = 8;
	if (!potatoesSet && !tomatoesSet && !boozySet) potatoesSet = 1;
	if (potatoesSet)
	{
		food = (char*)malloc(sizeof(char) * 6);
		final = (char*)malloc(sizeof(char) * 4);
		strcpy(food, "Potato");
		strcpy(final, "More");
	}
	else if (tomatoesSet)
	{
		food = (char*)malloc(sizeof(char) * 6);
		final = (char*)malloc(sizeof(char) * 4);
		strcpy(food, "Tomato");
		strcpy(final, "More");
	}
	else if (boozySet)
	{
		food = (char*)malloc(sizeof(char) * 7);
		final = (char*)malloc(sizeof(char) * 5);
		strcpy(food, "Tequila");
		strcpy(final, "Floor");
	}

	for (i = 1; i <= count; i++)
	{
		if (i == count) printf("%s!\n", final);
		else if (i % 4 == 0) printf("%d!\n", i);
		else printf("%d %s,\n", i, food);

		if (i % 4 == 0 && i != count) printf("\n");
	}

	if (food != NULL) free(food);
	if (final != NULL) free(final);

	return 0;
}