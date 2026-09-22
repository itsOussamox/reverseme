#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Decoy fail/success functions, disguised as "syscall_malloc".
 * Real level3 uses two names one underscore apart:
 *   ___syscall_malloc  (3 underscores) -> fail
 *   ____syscall_malloc (4 underscores) -> success
 * Renamed here to "no" and "ok" for clarity. */
void no(void)
{
	puts("Nope.");
	exit(1);
}

void ok(void)
{
	puts("Good job.");
}

int main(void)
{
	char input[64];    /* scanf buffer, read with "%23s" */
	char out[9];        /* decoded string, always starts with '*' */
	char tmp[4];          /* holds 3 digits + '\0' for atoi() */
	int k = 2;              /* index into input */
	int j = 1;                /* index into out */

	printf("Please enter key: ");
	scanf("%23s", input);

	if (input[1] != '2')
		no();
	if (input[0] != '4')
		no();

	fflush(stdin);

	memset(out, 0, sizeof(out));
	out[0] = '*';

	while (strlen(out) < 8 && (size_t)k < strlen(input)) {
		tmp[0] = input[k];
		tmp[1] = input[k + 1];
		tmp[2] = input[k + 2];
		tmp[3] = '\0';

		out[j] = (char)atoi(tmp);

		k += 3;
		j++;
	}
	out[j] = '\0';

	if (strcmp(out, "********") == 0)
		ok();
	else
		no();

	return 0;
}
