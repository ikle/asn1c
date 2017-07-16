%{
/*
 * ASN.1 Simplified Lexical Analyzer
 *
 * Copyright (c) 2017 Alexei A. Smekalkine
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <string.h>

#include "asn1-parser.h"

#define YY_USER_ACTION  (yylloc->first_line = yylineno);

#define RET(name)  do {					\
	return TOKEN_ ## name;				\
} while (0)

#define TAG(number)  do {				\
	*yylval = se (ASN1_SE_TAG (0, number));		\
	RET (TYPE);					\
} while (0)

#define SET(name)  do {					\
	*yylval = se (SE_ ## name, strdup (yytext));	\
} while (0)

%}

%option reentrant prefix="asn1_"
%option yylineno never-interactive
%option nodefault noyywrap

%option nounistd
%option bison-bridge

%x COMMENT

WS		[\t\n\v\f\r ]+
NL		[\n\v\f\r]
SIMPLE		[{}<>,./()[\]\-:="';@|!^]
NAME_TAIL	(-?[0-9A-Za-z])*
NUMBER		[0-9]+

%%
	BEGIN (INITIAL);

<INITIAL>{
	/* keywords */

	DEFINITIONS		{ RET (DEFINITIONS);		}
	BEGIN			{ RET (BEGIN);			}
	END			{ RET (END);			}

	::=			{ RET (ASSIGN);			}
	[.]{3}			{ RET (ELLIPSIS);		}

	OF			{ RET (OF);			}

	ENUMERATED		{ RET (ENUMERATED);		}
	SEQUENCE		{ RET (SEQUENCE);		}
	SET			{ RET (SET);			}
	CHOICE			{ RET (CHOICE);			}

	SIZE			{ RET (SIZE);			}

	/* universal types */

	BOOLEAN			{ TAG (1);			}
	INTEGER			{ TAG (2);			}
	BIT{WS}STRING		{ TAG (3);			}
	OCTET{WS}STRING		{ TAG (4);			}
	NULL			{ TAG (5);			}
	OBJECT{WS}IDENTIFIER	{ TAG (6);			}
	REAL			{ TAG (9);			}
	UTF8String		{ TAG (12);			}
	NumericString		{ TAG (18);			}
	PrintableString		{ TAG (19);			}
	IA5String		{ TAG (22);			}
	UTCTime			{ TAG (23);			}
	GeneralizedTime		{ TAG (24);			}

	/* compound */

	[A-Z]{NAME_TAIL}	{ SET (NAME);	RET (TYPE);	}
	[a-z]{NAME_TAIL}	{ SET (NAME);	RET (ID);	}
	{NUMBER}[.]{2}{NUMBER}	{ SET (RANGE);	RET (RANGE);	}
	{NUMBER}		{ SET (NUMBER);	RET (NUMBER);	}

	/* space, comment and simple */

	{WS}
	--			{ BEGIN (COMMENT);		}

	{SIMPLE}		{ return yytext[0];		}

	/* undefined */
	.			{ RET (ERROR);			}
}

<COMMENT>{
	--			{ BEGIN (INITIAL);		}
	{NL}			{ BEGIN (INITIAL);		}
	.
}
%%
