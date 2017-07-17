%{
/*
 * ASN.1 Simplified Lexical Analyzer
 *
 * Copyright (c) 2017 Alexei A. Smekalkine
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "asn1-parser.h"

#define YY_USER_ACTION  (yylloc->first_line = yylineno);

#define RET(name)  do {					\
	return TOKEN_ ## name;				\
} while (0)

#define SET(name)  do {					\
	*yylval = se_##name (yytext);			\
} while (0)

#define RET_UT(name, value)  do {			\
	*yylval = se_name (value);			\
	RET (name);					\
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
NUMBER		0|[1-9][0-9]*
REAL		{NUMBER}[.]([0-9]*)?([eE]-?{NUMBER})?

%%
	BEGIN (INITIAL);

<INITIAL>{
	/* keywords */

	DEFINITIONS		{ RET (DEFINITIONS);		}
	BEGIN			{ RET (BEGIN);			}
	END			{ RET (END);			}

	IMPLICIT		{ RET (IMPLICIT);		}
	EXPLICIT		{ RET (EXPLICIT);		}
	AUTOMATIC		{ RET (AUTOMATIC);		}
	TAGS			{ RET (TAGS);			}

	::=			{ RET (ASSIGN);			}
	[.]{3}			{ RET (ELLIPSIS);		}

	OF			{ RET (OF);			}

	ENUMERATED		{ RET (ENUMERATED);		}
	SEQUENCE		{ RET (SEQUENCE);		}
	SET			{ RET (SET);			}
	CHOICE			{ RET (CHOICE);			}

	SIZE			{ RET (SIZE);			}
	OPTIONAL		{ RET (OPTIONAL);		}
	DEFAULT			{ RET (DEFAULT);		}

	/* strange universal types with spaces in name */

	BIT{WS}STRING		{ RET_UT (TYPE, "BIT STRING");	}
	OCTET{WS}STRING		{ RET_UT (TYPE, "OCTET STRING"); }
	OBJECT{WS}IDENTIFIER	{ RET_UT (TYPE, "OBJECT IDENTIFIER"); }

	/* constants */

	TRUE			{ RET (TRUE);			}
	FALSE			{ RET (FALSE);			}

	/* compound */

	[A-Z]{NAME_TAIL}	{ SET (name);	RET (TYPE);	}
	[a-z]{NAME_TAIL}	{ SET (name);	RET (ID);	}
	{NUMBER}[.]{2}{NUMBER}	{ SET (range);	RET (RANGE);	}
	{REAL}			{ SET (real);	RET (REAL);	}
	{NUMBER}		{ SET (number);	RET (NUMBER);	}
	\"(([^"]|\n|\"\")*)\"	{ SET (string);	RET (STRING);	}

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
