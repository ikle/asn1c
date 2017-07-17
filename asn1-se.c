/*
 * ASN.1 Parser node types
 *
 * Copyright (c) 2017 Alexei A. Smekalkine
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asn1-se.h"

static void indent (int level)
{
	printf ("%*s", level * 4, "");
}

struct se_one {
	struct se base;
	char *value;
};

static struct se *se_one (const struct se_class *class, const char *content)
{
	struct se_one *o;

	if ((o = malloc (sizeof (*o))) == NULL)
		goto no_object;

	if ((o->value = strdup (content)) == NULL)
		goto no_value;

	o->base.class = class;
	o->base.type  = 0;
	return (void *) o;
no_value:
	free (o);
no_object:
	return NULL;
}

static void se_one_free (struct se *se)
{
	struct se_one *o = (void *) se;

	free (o->value);
	free (o);
}

static void se_one_show (int level, const struct se *se)
{
	struct se_one *o = (void *) se;

	indent (level);
	printf ("(%s %s)", o->base.class->name, o->value);
}

#define DECL_SE_ONE(one)				\
static const struct se_class se_##one##_class = {	\
	.name = #one,					\
	.free = se_one_free,				\
	.show = se_one_show,				\
};							\
							\
struct se *se_##one (const char *content)		\
{							\
	return se_one (&se_##one##_class, content);	\
}

DECL_SE_ONE (name)
DECL_SE_ONE (comment)
DECL_SE_ONE (number)
DECL_SE_ONE (string)
DECL_SE_ONE (range)
DECL_SE_ONE (real)

#undef DECL_SE_ONE