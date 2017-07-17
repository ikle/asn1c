/*
 * Parser S-Expressions
 *
 * Copyright (c) 2017 Alexei A. Smekalkine
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdio.h>
#include <stdlib.h>

#include "se.h"

static void se_base_free (struct se *o)
{
	int i;

	for (i = 0; i < se_count (o->type); ++i)
		se_free (o->item[i]);

	free (o);
}

static void indent (int level)
{
	printf ("%*s", level * 4, "");
}

static void se_base_show (int level, const struct se *o)
{
	int i;

	indent (level);

	if (o->class->name != NULL)
		printf ("(%s", o->class->name);
	else
		printf ("(%05x", o->type);

	if (se_count (o->type) == 1) {
		putchar (' ');
		se_show (0, o->item[i]);
	}
	else for (i = 0; i < se_count (o->type); ++i) {
		putchar ('\n');
		se_show (level + 1, o->item[i]);
	}

	putchar (')');
}

static const struct se_class se_base_class = {
	.free = se_base_free,
	.show = se_base_show,
};

struct se *se (int type, ...)
{
	struct se *o;
	size_t size = sizeof (*o) + sizeof (o->item[0]) * se_count (type);
	va_list ap;
	int i;

	if ((o = malloc (size)) == NULL)
		return NULL;

	o->type = type;

	va_start (ap, type);

	for (i = 0; i < se_count (type); ++i)
		o->item[i] = va_arg (ap, void *);

	va_end (ap);

	o->class = &se_base_class;
	return o;
}

void se_free (struct se *o)
{
	if (o == NULL)
		return;

	o->class->free (o);
}

void se_show (int level, const struct se *o)
{
	if (o == NULL) {
		indent (level);
		printf ("(empty)");
	}
	else
		o->class->show (level, o);
}
