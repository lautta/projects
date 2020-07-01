/* keygen.c
 * August Lautt */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
	int i;
	char randChar;

	srand(time(NULL));

	// check number of arguments
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s [keylength]\n", argv[0]);
		exit(1);
	}

	// create key with random characters based on length provided
	for (i = 0; i < atoi(argv[1]); i++)
	{
		// randomize for every letter, spaces are 26
		randChar = rand() % 27;

		// for 26, change it to a space
		if (randChar == 26)
		{
			randChar = ' ';
		}
		// otherwise add the value of A to get the uppercase range of ascii characters
		else
		{
			randChar += 'A';
		}

		// print each indvidual char
		printf("%c", randChar);
	}

	// add the necessary newline for files at the end
	printf("\n");

	return 0;
}
