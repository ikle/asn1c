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

static const char *se_name (const struct se *o)
{
	switch (o->type) {
	case SE_NAME:		return "name";
	case SE_COMMENT:	return "comment";
	case SE_NUMBER:		return "num";
	case SE_RANGE:		return "range";

	case SE_CONST:		return "const";
	case SE_LIST:		return "list";
	case SE_MODULE:		return "module";
	case SE_TYPE:		return "type";
	case SE_TYPEREF:	return "ref";
	case SE_FIELD:		return "field";

	case SE_ENUM:		return "enum";
	case SE_SEQ_OF:		return "seq-of";
	case SE_SET_OF:		return "set-of";
	case SE_SEQ:		return "seq";
	case SE_SET:		return "set";
	case SE_CHOICE:		return "choice";
	}

	return NULL;
}

static void show (const char *prefix, const struct se *o)
{
	int i;
	const char *name;;

	if (o == NULL) {
		printf ("(nil)");
		return;
	}

	printf ("%s(", prefix);

	if ((name = se_name (o)) != NULL)
		printf ("%s", name);
	else
		printf ("tag %d", o->type >> 12);

	for (i = 0; i < se_count (o->type); ++i)
		if (se_is_terminal (o->type))
			printf (" %s", o->item[i]);
		else {
			putchar (' ');
			show ("", o->item[i]);
		}

	putchar (')');

	if (prefix[0] != '\0')
		putchar ('\n');
}

int main (int argc, char *argv[])
{
	struct se *se;

	if ((se = get_ast (stdin)) == NULL)
		return 1;

	show ("ASN.1 = ", se);

	se_free (se);
	return 0;
}
