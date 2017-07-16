/*
 * Parser S-Expressions
 *
 * Copyright (c) 2017 Alexei A. Smekalkine
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdlib.h>

#include "se.h"

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

	return o;
}

void se_free (struct se *o)
{
	void (*fn) ();
	int i;

	if (o == NULL)
		return;

	fn = se_is_terminal (o->type) ?	free : se_free;

	for (i = 0; i < se_count (o->type); ++i)
		fn (o->item[i]);

	free (o);
}
