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

#define ASN1_SE_TAG(class, number)  (SE ((number) << 4 | class, 1, 0))

enum se_type
{
	SE_NAME		= SE ( 0, 1, 1),
	SE_COMMENT	= SE ( 1, 1, 1),
	SE_NUMBER	= SE ( 2, 1, 1),
	SE_STRING	= SE ( 3, 1, 1),
	SE_RANGE	= SE ( 4, 1, 1),
	SE_REAL		= SE ( 5, 1, 1),

	SE_CONST	= SE ( 0, 0, 2),
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

	SE_ELLIPSIS	= SE ( 0, 1, 0),
	SE_OPTIONAL	= SE ( 1, 1, 0),
	SE_TRUE		= SE ( 2, 1, 0),
	SE_FALSE	= SE ( 3, 1, 0),
};

#endif  /* ASN1_SE_H */
