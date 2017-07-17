/*
 * ASN.1 Simplified S-Expressions
 *
 * Copyright (c) 2017 Alexei A. Smekalkine
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef ASN1_SE_H
#define ASN1_SE_H  1

#include "se.h"

/* declare terminals */

#define DECL_SE_ZERO(name)  struct se *se_##name (void)

DECL_SE_ZERO (ellipsis);
DECL_SE_ZERO (optional);
DECL_SE_ZERO (true);
DECL_SE_ZERO (false);

#define DECL_SE_ONE(one)  struct se *se_##one (const char *content)

DECL_SE_ONE (name);
DECL_SE_ONE (comment);
DECL_SE_ONE (number);
DECL_SE_ONE (string);
DECL_SE_ONE (range);
DECL_SE_ONE (real);

#undef DECL_SE_ZERO
#undef DECL_SE_ONE

/* declare non-terminals */

#define DECL_SE_ONE(one)  struct se *se_##one (struct se *se)

DECL_SE_ONE (seq_of);
DECL_SE_ONE (set_of);
DECL_SE_ONE (default);
DECL_SE_ONE (size);

#define DECL_SE_LIST(type)  \
	struct se *se_##type (struct se *head, struct se *tail)

DECL_SE_LIST (list);
DECL_SE_LIST (oid);
DECL_SE_LIST (enum);
DECL_SE_LIST (seq);
DECL_SE_LIST (set);
DECL_SE_LIST (choice);

#define DECL_SE_TWO(type)  \
	struct se *se_##type (struct se *a, struct se *b)

DECL_SE_TWO (label);
DECL_SE_TWO (module);
DECL_SE_TWO (type);
DECL_SE_TWO (field);

#define DECL_SE_THREE(type)  \
	struct se *se_##type (struct se *a, struct se *b, struct se *c)

DECL_SE_THREE (typeref);
DECL_SE_THREE (value);

#undef DECL_SE_ONE
#undef DECL_SE_TWO
#undef DECL_SE_LIST
#undef DECL_SE_THREE

#endif  /* ASN1_SE_H */
