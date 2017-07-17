/*
 * ASN.1 Simplified S-Expressions
 *
 * Copyright (c) 2017 Alexei A. Smekalkine
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef ASN1_SE_H
#define ASN1_SE_H  1

#include <stddef.h>

#include "se.h"

enum se_type
{
	SE_LABEL	= SE ( 0, 0, 2),
	SE_LIST		= SE ( 1, 0, 2),
	SE_MODULE	= SE ( 2, 0, 2),
	SE_TYPE		= SE ( 3, 0, 2),
	SE_FIELD	= SE ( 5, 0, 2),
	SE_ENUM		= SE ( 6, 0, 2),
	SE_SEQ		= SE ( 7, 0, 2),
	SE_SET		= SE ( 8, 0, 2),
	SE_CHOICE	= SE ( 9, 0, 2),
	SE_OID		= SE (10, 0, 2),

	SE_VALUE	= SE ( 0, 0, 3),
	SE_TYPEREF	= SE ( 1, 0, 3),

	SE_SEQ_OF	= SE ( 1, 0, 1),
	SE_SET_OF	= SE ( 2, 0, 1),
	SE_DEFAULT	= SE ( 6, 0, 1),
	SE_SIZE		= SE ( 7, 0, 1),
};

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

#endif  /* ASN1_SE_H */
