#include <stdio.h>

#include "asn1-parser.h"
#include "asn1-lexer.h"

static struct se *get_ast (FILE *file)
{
	struct se *se;
	yyscan_t scanner;

	if (asn1_lex_init (&scanner) != 0) {
		perror ("E: cannot initialize scanner\n");
		return NULL;
	}

	asn1_set_in (file, scanner);

	if (asn1_parse (&se, scanner) != 0)
		return NULL;

	asn1_lex_destroy (scanner);
	return se;
}

int main (int argc, char *argv[])
{
	struct se *se;

	if ((se = get_ast (stdin)) == NULL)
		return 1;

	se_show (0, se); putchar ('\n');

	se_free (se);
	return 0;
}
