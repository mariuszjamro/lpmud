#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    void lower(char *);
    char buffer[BUFSIZ], *p;
    FILE *in, *out;
    int  first=0x7fffffff, last=0, n;

    if (argc != 3) {
	(void)fprintf(stderr, "Usage: make_table input.h output.h\n");
	exit(1);
	/* NOTREACHED */
    }
    if ((in = fopen(argv[1], "r")) == NULL) {
	perror("fopen");
	exit(1);
	/* NOTREACHED */
    }
    if ((out = fopen(argv[2], "w")) == NULL) {
	perror("fopen");
	exit(1);
	/* NOTREACHED */
    }
    (void)fprintf(out, "/*\n");
    (void)fprintf(out, " * This file is automatically generated; do not edit.\n");
    (void)fprintf(out, " */\n\n");
    (void)fprintf(out, "#ifdef EFUN_TABLE\n");
    (void)fprintf(out, "static void (*efun_table[])(int) = {\n");
    while (fgets(buffer, sizeof(buffer), in)) {
	if (buffer[0] != '#')
	    continue;
	p = strtok(buffer+1, " \t\n");
	if (strcmp(p, "define"))
	    continue;
	if ((p = strtok(NULL, " \t\n")) == NULL)
	    continue;
	if (strcmp(p, "YYEMPTY") == 0)
	    continue;
	lower(p);
	(void)fprintf(out, "\t%s,\n", p);
	if ((p = strtok(NULL, " \t\n")) == NULL)
	    continue;
	n = atoi(p);
	if (n < first)
	    first = n;
	if (n > last)
	    last = n;
    }
    (void)fprintf(out, "};\n");
    (void)fprintf(out, "#else /* EFUN_TABLE */\n");
    (void)fprintf(out, "#define EFUN_FIRST %4d\n", first);
    (void)fprintf(out, "#define EFUN_LAST  %4d\n", last);
    (void)fprintf(out, "#endif /* EFUN_TABLE */\n");
    (void)fclose(out);
    (void)fclose(in);
    return 0;
}

void
lower(char *string)
{
    while (*string) {
        if (isupper(*string))
	    *string = tolower(*string);
	string++;
    }
}
