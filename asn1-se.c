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

/* define terminals */

static struct se *se_zero (const struct se_class *class)
{
	struct se *o;

	if ((o = malloc (sizeof (*o))) == NULL)
		return NULL;

	o->class = class;
	o->type  = 0;
	return o;
}

static void se_zero_show (int level, const struct se *o)
{
	indent (level);
	printf ("(%s)", o->class->name);
}

#define DECL_SE_ZERO(type)				\
static const struct se_class se_##type##_class = {	\
	.name = #type,					\
	.free = (void (*) ()) free,			\
	.show = se_zero_show,				\
};							\
							\
struct se *se_##type (void)				\
{							\
	return se_zero (&se_##type##_class);		\
}

DECL_SE_ZERO (ellipsis)
DECL_SE_ZERO (optional)
DECL_SE_ZERO (true)
DECL_SE_ZERO (false)

#undef DECL_SE_ZERO

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

/* define non-terminals */

struct se_nt_one {
	struct se base;
	struct se *value;
};

static struct se *se_nt_one (const struct se_class *class, struct se *se)
{
	struct se_nt_one *o;

	if ((o = malloc (sizeof (*o))) == NULL)
		return NULL;

	o->base.class = class;
	o->base.type  = 0;
	o->value      = se;
	return (void *) o;
}

static void se_nt_one_free (struct se *se)
{
	struct se_nt_one *o = (void *) se;

	se_free (o->value);
	free (o);
}

static void se_nt_one_show (int level, const struct se *se)
{
	struct se_nt_one *o = (void *) se;

	indent (level);
	printf ("(%s ", o->base.class->name);
	se_show (0, o->value);
	putchar (')');
}

#define DECL_SE_ONE(type, typename)			\
static const struct se_class se_##type##_class = {	\
	.name = typename,				\
	.free = se_nt_one_free,				\
	.show = se_nt_one_show,				\
};							\
							\
struct se *se_##type (struct se *se)			\
{							\
	return se_nt_one (&se_##type##_class, se);	\
}

DECL_SE_ONE (seq_of,  "seq-of")
DECL_SE_ONE (set_of,  "set-of")
DECL_SE_ONE (default, "default")
DECL_SE_ONE (size,    "size")

#undef DECL_SE_ONE
