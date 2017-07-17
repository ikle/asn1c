/*
 * Parser S-Expressions
 *
 * Copyright (c) 2017 Alexei A. Smekalkine
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef SE_H
#define SE_H  1

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

void se_free (struct se *b);
void se_show (int level, const struct se *o);

#endif  /* SE_H */
