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
	case SE_STRING:		return "str";
	case SE_RANGE:		return "range";

	case SE_CONST:		return "const";
	case SE_LIST:		return "list";
	case SE_MODULE:		return "module";
	case SE_TYPE:		return "type";
	case SE_TYPEREF:	return "type-ref";
	case SE_FIELD:		return "field";

	case SE_VALUE:		return "value";

	case SE_ENUM:		return "enum";
	case SE_SEQ_OF:		return "seq-of";
	case SE_SET_OF:		return "set-of";
	case SE_SEQ:		return "seq";
	case SE_SET:		return "set";
	case SE_CHOICE:		return "choice";
	case SE_DEFAULT:	return "default";

	case SE_ELLIPSIS:	return "ellipsis";
	case SE_OPTIONAL:	return "optional";

	case 0x02010:		return "INTEGER";
	case 0x05010:		return "NULL";
	case 0x12010:		return "NumericString";
	case 0x13010:		return "PrintableString";
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
		else {
			putchar ('\n');
			show (level + 1, o->item[i]);
		}
}

static void show_list (int level, const struct se *o)
{
	const struct se *head, *tail;

	for (; o != NULL; o = tail) {
		putchar ('\n');

		if (o->type != SE_LIST) {  /* non-list leaf node */
			show (level + 1, o);
			break;
		}

		head = o->item[0], tail = o->item[1];

		show (level + 1, head);
	}
}

static void show (int level, const struct se *o)
{
	int i;
	const char *name;;

	indent (level); putchar ('(');

	if (o == NULL) {
		printf ("nil)");
		return;
	}

	if ((name = se_name (o)) != NULL)
		printf ("%s", name);
	else
		printf ("tag %d", o->type >> 12);

	if (o->type == SE_LIST)
		show_list (level, o);
	else
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
