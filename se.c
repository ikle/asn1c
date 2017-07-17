/*
 * Parser S-Expressions
 *
 * Copyright (c) 2017 Alexei A. Smekalkine
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdlib.h>
#include <string.h>

#include "se.h"

static void se_base_free (struct se *o)
{
	int i;

	for (i = 0; i < se_count (o->type); ++i)
		se_free (o->item[i]);

	free (o);
}

static struct se_class se_base_class = {
	.free = se_base_free,
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

static void se_term_free (struct se *o)
{
	free (o->item[0]);
	free (o);
}

static struct se_class se_term_class = {
	.free = se_term_free,
};

struct se *se_term (int type, const char *content)
{
	char *value;
	struct se *o;

	if (!se_is_terminal (type) || se_count (type) != 1 ||
	    (value = strdup (content)) == NULL)
		return NULL;

	if ((o = se (type, value)) == NULL)
		goto no_object;

	o->class = &se_term_class;
	return o;
no_object:
	free (value);
	return NULL;
}
