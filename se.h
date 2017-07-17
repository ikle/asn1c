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
	int type;
	void *item[];
};

#include <stdarg.h>

struct se *se (int type, ...);
void se_free (struct se *b);

struct se *se_term (int type, const char *content);

#endif  /* SE_H */
