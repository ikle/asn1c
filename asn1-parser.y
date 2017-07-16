%{
/*
 * ASN.1 Simplified Parser
 *
 * Copyright (c) 2017 Alexei A. Smekalkine
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "se.h"

#define yylex    asn1_lex
#define yyparse  asn1_parse	/* define api.prefix for bison >= 2.6 */

static void yyerror (void *loc, struct se **o, void *scanner, const char *msg);
%}

%define api.pure

%lex-param   { void *scanner }
%parse-param { struct se **o }
%parse-param { void *scanner }

%locations

%code requires {
#define YYSTYPE  struct se *
}

%code provides {
#include "asn1-se.h"

#define YY_DECL  \
	int asn1_lex (YYSTYPE *yylval_param, YYLTYPE *yylloc, void *yyscanner)

int asn1_lex (YYSTYPE *ret, YYLTYPE *loc, void *scanner);
int asn1_parse (struct se **o, void *scanner);
}

%token TOKEN_ERROR

%token TOKEN_DEFINITIONS
%token TOKEN_BEGIN TOKEN_END
%token TOKEN_IMPLICIT TOKEN_EXPLICIT TOKEN_AUTOMATIC TOKEN_TAGS

%token TOKEN_ASSIGN "::=" TOKEN_ELLIPSIS "..." TOKEN_OF "OF"

%token TOKEN_ENUMERATED TOKEN_SEQUENCE TOKEN_SET TOKEN_CHOICE

%token TOKEN_TYPE TOKEN_ID
%token TOKEN_NUMBER
%destructor { se_free ($$); }
	TOKEN_TYPE TOKEN_ID TOKEN_NUMBER

%token TOKEN_SIZE TOKEN_RANGE
%token TOKEN_OPTIONAL TOKEN_DEFAULT

%token '{' '}' '(' ')'
%left ','

%%

input	: modules[L]	{ *o = $L; }
	;

modules : module[H] modules[T]			{ $$ = se (SE_LIST, $H, $T);	}
	| /* empty */				{ $$ = NULL;			}
	;

module	: TOKEN_TYPE[M] TOKEN_DEFINITIONS def_tags "::="
	  TOKEN_BEGIN
		assignments[L]
	  TOKEN_END				{ $$ = se (SE_MODULE, $M, $L);	}
	;

def_tags
	: tag_type TOKEN_TAGS
	| /* empty */
	;

tag_type
	: TOKEN_IMPLICIT
	| TOKEN_EXPLICIT
	| TOKEN_AUTOMATIC
	;

assignments
	: assignment[H] assignments[T]		{ $$ = se (SE_LIST, $H, $T);	}
	| /* empty */				{ $$ = NULL;			}
	;

assignment
	: TOKEN_TYPE[T] "::=" type[E]		{ $$ = se (SE_TYPE, $T, $E);	}
	;

type	: TOKEN_TYPE[T]	constrains[C] defs	{ $$ = se (SE_TYPEREF, $T, $C); }
	| TOKEN_ENUMERATED '{' consts[L] '}'	{ $$ = se (SE_ENUM, $L);	}
	| TOKEN_SEQUENCE "OF" TOKEN_TYPE[T]	{ $$ = se (SE_SEQ_OF, $T);	}
	| TOKEN_SET "OF"      TOKEN_TYPE[T]	{ $$ = se (SE_SET_OF, $T);	}
	| TOKEN_SEQUENCE fields[L]		{ $$ = se (SE_SEQ, $L);		}
	| TOKEN_SET      fields[L]		{ $$ = se (SE_SET, $L);		}
	| TOKEN_CHOICE   fields[L]		{ $$ = se (SE_CHOICE, $L);	}
	;

constrains
	: '(' constrain[C] ')'			{ $$ = $C;			}
	| /* empty */				{ $$ = NULL;			}
	;

constrain
	: TOKEN_SIZE '(' size_constrain[C] ')'	{ $$ = $C;			}
	;

size_constrain
	: TOKEN_NUMBER[N]			{ $$ = $N;			}
	| TOKEN_RANGE[R]			{ $$ = $R;			}
	;

defs	: TOKEN_OPTIONAL
	| TOKEN_DEFAULT constant
	|
	;

constant
	: TOKEN_ID				{ $$ = $1;			}
	| TOKEN_NUMBER				{ $$ = $1;			}
	;

consts	: consts[H] ',' consts[T]		{ $$ = se (SE_LIST, $H, $T);	}
	| TOKEN_ID[I] '(' TOKEN_NUMBER[N] ')'	{ $$ = se (SE_CONST, $I,   $N);	}
	| TOKEN_NUMBER[N]			{ $$ = se (SE_CONST, NULL, $N);	}
	;

fields	: '{' field_list[L] '}'			{ $$ = $L;			}
	| '{' field_list[L] ',' "..." '}'	{ $$ = $L;			}
	;

field_list
	: field_list[H] ',' field_list[T]	{ $$ = se (SE_LIST, $H, $T);	}
	| TOKEN_ID[I] type[T]			{ $$ = se (SE_FIELD, $I, $T);	}
	;

%%

#include <stdio.h>

static void yyerror (void *loc, struct se **o, void *scanner, const char *msg)
{
	struct YYLTYPE *l = loc;

	fprintf (stderr, "E:%d: %s\n", l->first_line, msg);
}
