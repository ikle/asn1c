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

static const char *se_get_name (const struct se *o)
{
	switch (o->type) {
	case SE_TYPEREF:	return "type-ref";
	case SE_VALUE:		return "value";
	}

	return NULL;
}

static void indent (int level)
{
	printf ("%*s", level * 4, "");
}

static void show (int level, const struct se *o);

static void show_args (int level, const struct se *o)
{
	int i;

	for (i = 0; i < se_count (o->type); ++i)
		if (se_is_terminal (o->type))
			printf (" %s", o->item[i]);
		else if (se_count (o->type) == 1) {
			putchar (' ');
			show (0, o->item[i]);
		}
		else {
			putchar ('\n');
			show (level + 1, o->item[i]);
		}
}

static void show_name (const struct se *o)
{
	const char *name;

	if ((name = se_get_name (o)) != NULL)
		printf ("%s", name);
	else
		printf ("%05x", o->type);
}

static void show (int level, const struct se *o)
{
	int i;

	if (o == NULL || o->type == 0) {
		se_show (level, o);
		return;
	}

	indent (level); putchar ('(');

	show_name (o);
	show_args (level, o);

	putchar (')');
}

int main (int argc, char *argv[])
{
	struct se *se;

	if ((se = get_ast (stdin)) == NULL)
		return 1;

	show (0, se); putchar ('\n');

	se_free (se);
	return 0;
}
