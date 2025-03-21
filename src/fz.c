/*
  FZ: FIZZ FUZZ BOP
  
  Build:
    gcc fz.c -o fz
    
   Usage:
    fz --count 60 --fizz 3 --fuzz 4 --bop 5
    fz -c 60 -i 3 -u 4 -b 5
      => Print from 1 to 60 with the Fizz/Fuzz/Bop keywords.
    fz --count 60 --fizz 3 --fuzz 4 --bop 5 --gameplay
    fz -c 60 -i 3 -u 4 -b 5 -g
      => Print from 1 to 60 as players would play the game.
*/

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>

#define FZ_VERSION "1.1.2.14"
#define FZ_COPYRIGHT "(c) Benedict W. Hazel, 2012"
#define FZ_OPEN_SOURCE "This is free software, released under the GNU Lesser General Public License (LGPL)"

int count = 0, fizz = 0, fuzz = 0, bop = 0;
int isFizz = 0, isFuzz = 0, isBop = 0;

int gameplay = 0;
int countSet = 0, fizzSet = 0, fuzzSet = 0, bopSet = 0;

int c, i, oi;

int main(int argc, char** argv)
{
#ifndef NO_CGI
    printf("Content-Type: text/plain\n\n");
#endif
	struct option options[] =
	{
		{"gameplay", no_argument, 0, 'g'},
		{"count", required_argument, 0, 'c'},
		{"fizz", required_argument, 0, 'i'},
		{"fuzz", required_argument, 0, 'u'},
		{"bop", required_argument, 0, 'b'},
		{"version", no_argument, 0, 'v'},
		{0, 0, 0, 0}
	};

	opterr = 0;
        while (1)
        {
                c = getopt_long(argc, argv, "b:c:gi:u:v", options, &oi);
                if (c == -1) break;

		switch (c)
		{
			case 'b':
				bop = atoi(optarg);
				if (bop != 0) bopSet = 1;
				break;
			case 'c':
				count = atoi(optarg);
				if (count != 0) countSet = 1;
				break;
			case 'u':
				fuzz = atoi(optarg);
				if (fuzz != 0) fuzzSet = 1;
				break;
			case 'i':
				fizz = atoi(optarg);
				if (fizz != 0) fizzSet = 1;
				break;
			case 'v':
				printf("Fizz Fuzz Bop %s\n", FZ_VERSION);
				printf("%s\n", FZ_COPYRIGHT);
				printf("%s\n", FZ_OPEN_SOURCE);
				return 0;
				break;
			case 'g':
				gameplay = 1;
				break;
			case '?':
				fprintf(stderr, "Unrecognised option -%c\n", optopt);
				return 1;
				break;
			default:
				break;
		}
	}

	if (!countSet)
	{
		printf("No count provided.\n");
		return 1;
	}

	for (i = 1; i <= count; i++)
	{
		isFizz = isFuzz = isBop = 0;

		if (fizzSet && i % fizz == 0) isFizz = 1;
		if (fuzzSet && i % fuzz == 0) isFuzz = 1;
		if (bopSet && i % bop == 0) isBop = 1;

		if (!gameplay) printf("%d\t", i);
		else if (!isFizz && !isFuzz && !isBop) printf("%d\t", i);

		if (isFizz) printf("FIZZ\t");
		if (isFuzz) printf("FUZZ\t");
		if (isBop) printf("BOP");
		printf("\n");
	}
	return 0;
}