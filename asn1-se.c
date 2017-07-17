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

struct se_nt_list {
	struct se base;
	struct se *head, *tail;
};

static struct se *se_nt_list (const struct se_class *class,
			      struct se *head, struct se *tail)
{
	struct se_nt_list *o;

	if ((o = malloc (sizeof (*o))) == NULL)
		return NULL;

	o->base.class = class;
	o->base.type  = 0;
	o->head       = head;
	o->tail       = tail;
	return (void *) o;
}

static void se_nt_list_free (struct se *se)
{
	struct se_nt_list *o = (void *) se;

	se_free (o->head);
	se_free (o->tail);
	free (o);
}

static void se_nt_list_show (int level, const struct se *se)
{
	struct se_nt_list *o = (void *) se;

	indent (level);
	printf ("(%s", o->base.class->name);

	for (; o != NULL; o = (void *) o->tail) {
		putchar ('\n');
		se_show (level + 1, o->head);
	}

	putchar (')');
}

#define DECL_SE_LIST(type)					\
static const struct se_class se_##type##_class = {		\
	.name = #type,						\
	.free = se_nt_list_free,				\
	.show = se_nt_list_show,				\
};								\
								\
struct se *se_##type (struct se *head, struct se *tail)		\
{								\
	return se_nt_list (&se_##type##_class, head, tail);	\
}

DECL_SE_LIST (list)
DECL_SE_LIST (oid)
DECL_SE_LIST (enum)
DECL_SE_LIST (seq)
DECL_SE_LIST (set)
DECL_SE_LIST (choice)

#undef DECL_SE_LIST

static void se_nt_two_show (int level, const struct se *se)
{
	struct se_nt_list *o = (void *) se;

	indent (level);
	printf ("(%s", o->base.class->name);

	putchar ('\n'); se_show (level + 1, o->head);
	putchar ('\n'); se_show (level + 1, o->tail);

	putchar (')');
}

#define DECL_SE_TWO(type)					\
static const struct se_class se_##type##_class = {		\
	.name = #type,						\
	.free = se_nt_list_free,				\
	.show = se_nt_two_show,					\
};								\
								\
struct se *se_##type (struct se *head, struct se *tail)		\
{								\
	return se_nt_list (&se_##type##_class, head, tail);	\
}

DECL_SE_TWO (label)
DECL_SE_TWO (module)
DECL_SE_TWO (type)
DECL_SE_TWO (field)

#undef DECL_SE_TWO

struct se_nt_three {
	struct se base;
	struct se *a, *b, *c;
};

static struct se *se_nt_three (const struct se_class *class,
			       struct se *a, struct se *b, struct se *c)
{
	struct se_nt_three *o;

	if ((o = malloc (sizeof (*o))) == NULL)
		return NULL;

	o->base.class = class;
	o->base.type  = 0;
	o->a = a;
	o->b = b;
	o->c = c;
	return (void *) o;
}

static void se_nt_three_free (struct se *se)
{
	struct se_nt_three *o = (void *) se;

	se_free (o->a);
	se_free (o->b);
	se_free (o->c);
	free (o);
}

static void se_nt_three_show (int level, const struct se *se)
{
	struct se_nt_three *o = (void *) se;

	indent (level);
	printf ("(%s", o->base.class->name);

	putchar ('\n'); se_show (level + 1, o->a);
	putchar ('\n'); se_show (level + 1, o->b);
	putchar ('\n'); se_show (level + 1, o->c);

	putchar (')');
}

#define DECL_SE_THREE(type, typename)					\
static const struct se_class se_##type##_class = {			\
	.name = typename,						\
	.free = se_nt_three_free,					\
	.show = se_nt_three_show,					\
};									\
									\
struct se *se_##type (struct se *a, struct se *b, struct se *c)		\
{									\
	return se_nt_three (&se_##type##_class, a, b, c);		\
}

DECL_SE_THREE (typeref, "type-ref")
DECL_SE_THREE (value,   "value")

#undef DECL_SE_THREE
