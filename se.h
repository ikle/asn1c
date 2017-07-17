/*
 * Parser S-Expressions
 *
 * Copyright (c) 2017 Alexei A. Smekalkine
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef SE_H
#define SE_H  1

/* se_type encodes number of arguments in lower nibble and flags in next one */
#define SE(i, f, n)  ((i) << 8 | (f) << 4 | (n))

static int se_is_terminal (int type)
{
	return (type & 0x10) != 0;
}

static int se_count (int type)
{
	return type & 0xf;
}

struct se
{
	const struct se_class *class;
	int type;
	void *item[];
};

struct se_class {
	const char *name;
	void (*free) (struct se *o);
	void (*show) (int level, const struct se *o);
};

#include <stdarg.h>

struct se *se (int type, ...);
void se_free (struct se *b);
void se_show (int level, const struct se *o);

#endif  /* SE_H */
