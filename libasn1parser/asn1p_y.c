/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         asn1p_parse
#define yylex           asn1p_lex
#define yyerror         asn1p_error
#define yydebug         asn1p_debug
#define yynerrs         asn1p_nerrs
#define yylval          asn1p_lval
#define yychar          asn1p_char

/* First part of user prologue.  */
#line 3 "asn1p_y.y"


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <assert.h>

#include "asn1parser.h"

#define YYERROR_VERBOSE
#define YYDEBUG 1
#define YYFPRINTF   prefixed_fprintf

/*
 * Prefix parser debug with "PARSER: " for easier human eye scanning.
 */
static int
__attribute__((format(printf, 2, 3)))
prefixed_fprintf(FILE *f, const char *fmt, ...) {
    static int line_ended = 1;
    va_list ap;
    va_start(ap, fmt);
    if(line_ended) {
        fprintf(f, "PARSER: ");
        line_ended = 0;
    }
    size_t len = strlen(fmt);
    if(len && fmt[len-1] == '\n') {
        line_ended = 1;
    }
    int ret = vfprintf(f, fmt, ap);
    va_end(ap);
    return ret;
}

int yylex(void);
int yyerror(void **param, const char *msg);
#ifdef	YYBYACC
int yyparse(void **param);	/* byacc does not produce a prototype */
#endif
void asn1p_lexer_hack_push_opaque_state(void);
void asn1p_lexer_hack_enable_with_syntax(void);
void asn1p_lexer_hack_push_encoding_control(void);
#define	yylineno	asn1p_lineno
extern int asn1p_lineno;
const char *asn1p_parse_debug_filename;
#define ASN_FILENAME asn1p_parse_debug_filename

/*
 * Process directives as <ASN1C:RepresentAsPointer>
 */
extern int asn1p_as_pointer;

/*
 * This temporary variable is used to solve the shortcomings of 1-lookahead
 * parser.
 */
static struct AssignedIdentifier *saved_aid;

static asn1p_value_t *_convert_bitstring2binary(char *str, int base);
static void _fixup_anonymous_identifier(asn1p_expr_t *expr);

static asn1p_module_t *currentModule;
#define	NEW_EXPR()	(asn1p_expr_new(yylineno, currentModule))

#define	checkmem(ptr)	do {						\
		if(!(ptr))						\
		return yyerror(param, "Memory failure");		\
	} while(0)

#define	CONSTRAINT_INSERT(root, constr_type, arg1, arg2) do {		\
		if(arg1->type != constr_type) {				\
			int __ret;					\
			root = asn1p_constraint_new(yylineno, currentModule);	\
			checkmem(root);					\
			root->type = constr_type;			\
			__ret = asn1p_constraint_insert(root,		\
				arg1);					\
			checkmem(__ret == 0);				\
		} else {						\
			root = arg1;					\
		}							\
		if(arg2) {						\
			int __ret					\
			= asn1p_constraint_insert(root, arg2);		\
			checkmem(__ret == 0);				\
		}							\
	} while(0)

#ifdef	AL_IMPORT
#error	AL_IMPORT DEFINED ELSEWHERE!
#endif
#define AL_IMPORT(to, where, from, field)                                      \
    do {                                                                       \
        if(!(from)) break;                                                     \
        while(TQ_FIRST(&((from)->where))) {                                    \
            TQ_ADD(&((to)->where), TQ_REMOVE(&((from)->where), field), field); \
        }                                                                      \
        assert(TQ_FIRST(&((from)->where)) == 0);                               \
    } while(0)


#line 183 "asn1p_y.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_ASN1P_ASN_P_Y_H_INCLUDED
# define YY_ASN1P_ASN_P_Y_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int asn1p_debug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    TOK_PPEQ = 258,                /* TOK_PPEQ  */
    TOK_VBracketLeft = 259,        /* TOK_VBracketLeft  */
    TOK_VBracketRight = 260,       /* TOK_VBracketRight  */
    TOK_whitespace = 261,          /* TOK_whitespace  */
    TOK_opaque = 262,              /* TOK_opaque  */
    TOK_bstring = 263,             /* TOK_bstring  */
    TOK_cstring = 264,             /* TOK_cstring  */
    TOK_hstring = 265,             /* TOK_hstring  */
    TOK_identifier = 266,          /* "identifier"  */
    TOK_number = 267,              /* "number"  */
    TOK_number_negative = 268,     /* "negative number"  */
    TOK_realnumber = 269,          /* TOK_realnumber  */
    TOK_tuple = 270,               /* TOK_tuple  */
    TOK_quadruple = 271,           /* TOK_quadruple  */
    TOK_typereference = 272,       /* TOK_typereference  */
    TOK_capitalreference = 273,    /* TOK_capitalreference  */
    TOK_typefieldreference = 274,  /* TOK_typefieldreference  */
    TOK_valuefieldreference = 275, /* TOK_valuefieldreference  */
    TOK_Literal = 276,             /* TOK_Literal  */
    TOK_ExtValue_BIT_STRING = 277, /* TOK_ExtValue_BIT_STRING  */
    TOK_ABSENT = 278,              /* TOK_ABSENT  */
    TOK_ABSTRACT_SYNTAX = 279,     /* TOK_ABSTRACT_SYNTAX  */
    TOK_ALL = 280,                 /* TOK_ALL  */
    TOK_ANY = 281,                 /* TOK_ANY  */
    TOK_APPLICATION = 282,         /* TOK_APPLICATION  */
    TOK_AUTOMATIC = 283,           /* TOK_AUTOMATIC  */
    TOK_BEGIN = 284,               /* TOK_BEGIN  */
    TOK_BIT = 285,                 /* TOK_BIT  */
    TOK_BMPString = 286,           /* TOK_BMPString  */
    TOK_BOOLEAN = 287,             /* TOK_BOOLEAN  */
    TOK_BY = 288,                  /* TOK_BY  */
    TOK_CHARACTER = 289,           /* TOK_CHARACTER  */
    TOK_CHOICE = 290,              /* TOK_CHOICE  */
    TOK_CLASS = 291,               /* TOK_CLASS  */
    TOK_COMPONENT = 292,           /* TOK_COMPONENT  */
    TOK_COMPONENTS = 293,          /* TOK_COMPONENTS  */
    TOK_CONSTRAINED = 294,         /* TOK_CONSTRAINED  */
    TOK_CONTAINING = 295,          /* TOK_CONTAINING  */
    TOK_DEFAULT = 296,             /* TOK_DEFAULT  */
    TOK_DEFINITIONS = 297,         /* TOK_DEFINITIONS  */
    TOK_DEFINED = 298,             /* TOK_DEFINED  */
    TOK_EMBEDDED = 299,            /* TOK_EMBEDDED  */
    TOK_ENCODED = 300,             /* TOK_ENCODED  */
    TOK_ENCODING_CONTROL = 301,    /* TOK_ENCODING_CONTROL  */
    TOK_END = 302,                 /* TOK_END  */
    TOK_ENUMERATED = 303,          /* TOK_ENUMERATED  */
    TOK_EXPLICIT = 304,            /* TOK_EXPLICIT  */
    TOK_EXPORTS = 305,             /* TOK_EXPORTS  */
    TOK_EXTENSIBILITY = 306,       /* TOK_EXTENSIBILITY  */
    TOK_FALSE = 307,               /* TOK_FALSE  */
    TOK_FROM = 308,                /* TOK_FROM  */
    TOK_GeneralizedTime = 309,     /* TOK_GeneralizedTime  */
    TOK_GeneralString = 310,       /* TOK_GeneralString  */
    TOK_GraphicString = 311,       /* TOK_GraphicString  */
    TOK_IA5String = 312,           /* TOK_IA5String  */
    TOK_IDENTIFIER = 313,          /* TOK_IDENTIFIER  */
    TOK_IMPLICIT = 314,            /* TOK_IMPLICIT  */
    TOK_IMPLIED = 315,             /* TOK_IMPLIED  */
    TOK_IMPORTS = 316,             /* TOK_IMPORTS  */
    TOK_INCLUDES = 317,            /* TOK_INCLUDES  */
    TOK_INSTANCE = 318,            /* TOK_INSTANCE  */
    TOK_INSTRUCTIONS = 319,        /* TOK_INSTRUCTIONS  */
    TOK_INTEGER = 320,             /* TOK_INTEGER  */
    TOK_ISO646String = 321,        /* TOK_ISO646String  */
    TOK_MAX = 322,                 /* TOK_MAX  */
    TOK_MIN = 323,                 /* TOK_MIN  */
    TOK_MINUS_INFINITY = 324,      /* TOK_MINUS_INFINITY  */
    TOK_NULL = 325,                /* TOK_NULL  */
    TOK_NumericString = 326,       /* TOK_NumericString  */
    TOK_OBJECT = 327,              /* TOK_OBJECT  */
    TOK_ObjectDescriptor = 328,    /* TOK_ObjectDescriptor  */
    TOK_OCTET = 329,               /* TOK_OCTET  */
    TOK_OF = 330,                  /* TOK_OF  */
    TOK_OPTIONAL = 331,            /* TOK_OPTIONAL  */
    TOK_PATTERN = 332,             /* TOK_PATTERN  */
    TOK_PDV = 333,                 /* TOK_PDV  */
    TOK_PLUS_INFINITY = 334,       /* TOK_PLUS_INFINITY  */
    TOK_PRESENT = 335,             /* TOK_PRESENT  */
    TOK_PrintableString = 336,     /* TOK_PrintableString  */
    TOK_PRIVATE = 337,             /* TOK_PRIVATE  */
    TOK_REAL = 338,                /* TOK_REAL  */
    TOK_RELATIVE_OID = 339,        /* TOK_RELATIVE_OID  */
    TOK_SEQUENCE = 340,            /* TOK_SEQUENCE  */
    TOK_SET = 341,                 /* TOK_SET  */
    TOK_SIZE = 342,                /* TOK_SIZE  */
    TOK_STRING = 343,              /* TOK_STRING  */
    TOK_SYNTAX = 344,              /* TOK_SYNTAX  */
    TOK_T61String = 345,           /* TOK_T61String  */
    TOK_TAGS = 346,                /* TOK_TAGS  */
    TOK_TeletexString = 347,       /* TOK_TeletexString  */
    TOK_TRUE = 348,                /* TOK_TRUE  */
    TOK_TYPE_IDENTIFIER = 349,     /* TOK_TYPE_IDENTIFIER  */
    TOK_UNIQUE = 350,              /* TOK_UNIQUE  */
    TOK_UNIVERSAL = 351,           /* TOK_UNIVERSAL  */
    TOK_UniversalString = 352,     /* TOK_UniversalString  */
    TOK_UTCTime = 353,             /* TOK_UTCTime  */
    TOK_UTF8String = 354,          /* TOK_UTF8String  */
    TOK_VideotexString = 355,      /* TOK_VideotexString  */
    TOK_VisibleString = 356,       /* TOK_VisibleString  */
    TOK_WITH = 357,                /* TOK_WITH  */
    UTF8_BOM = 358,                /* "UTF-8 byte order mark"  */
    TOK_EXCEPT = 359,              /* TOK_EXCEPT  */
    TOK_INTERSECTION = 360,        /* TOK_INTERSECTION  */
    TOK_UNION = 361,               /* TOK_UNION  */
    TOK_TwoDots = 362,             /* ".."  */
    TOK_ThreeDots = 363,           /* "..."  */
    TOK_SUCCESSORS = 364,          /* TOK_SUCCESSORS  */
    TOK_DESCENDANTS = 365          /* TOK_DESCENDANTS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define TOK_PPEQ 258
#define TOK_VBracketLeft 259
#define TOK_VBracketRight 260
#define TOK_whitespace 261
#define TOK_opaque 262
#define TOK_bstring 263
#define TOK_cstring 264
#define TOK_hstring 265
#define TOK_identifier 266
#define TOK_number 267
#define TOK_number_negative 268
#define TOK_realnumber 269
#define TOK_tuple 270
#define TOK_quadruple 271
#define TOK_typereference 272
#define TOK_capitalreference 273
#define TOK_typefieldreference 274
#define TOK_valuefieldreference 275
#define TOK_Literal 276
#define TOK_ExtValue_BIT_STRING 277
#define TOK_ABSENT 278
#define TOK_ABSTRACT_SYNTAX 279
#define TOK_ALL 280
#define TOK_ANY 281
#define TOK_APPLICATION 282
#define TOK_AUTOMATIC 283
#define TOK_BEGIN 284
#define TOK_BIT 285
#define TOK_BMPString 286
#define TOK_BOOLEAN 287
#define TOK_BY 288
#define TOK_CHARACTER 289
#define TOK_CHOICE 290
#define TOK_CLASS 291
#define TOK_COMPONENT 292
#define TOK_COMPONENTS 293
#define TOK_CONSTRAINED 294
#define TOK_CONTAINING 295
#define TOK_DEFAULT 296
#define TOK_DEFINITIONS 297
#define TOK_DEFINED 298
#define TOK_EMBEDDED 299
#define TOK_ENCODED 300
#define TOK_ENCODING_CONTROL 301
#define TOK_END 302
#define TOK_ENUMERATED 303
#define TOK_EXPLICIT 304
#define TOK_EXPORTS 305
#define TOK_EXTENSIBILITY 306
#define TOK_FALSE 307
#define TOK_FROM 308
#define TOK_GeneralizedTime 309
#define TOK_GeneralString 310
#define TOK_GraphicString 311
#define TOK_IA5String 312
#define TOK_IDENTIFIER 313
#define TOK_IMPLICIT 314
#define TOK_IMPLIED 315
#define TOK_IMPORTS 316
#define TOK_INCLUDES 317
#define TOK_INSTANCE 318
#define TOK_INSTRUCTIONS 319
#define TOK_INTEGER 320
#define TOK_ISO646String 321
#define TOK_MAX 322
#define TOK_MIN 323
#define TOK_MINUS_INFINITY 324
#define TOK_NULL 325
#define TOK_NumericString 326
#define TOK_OBJECT 327
#define TOK_ObjectDescriptor 328
#define TOK_OCTET 329
#define TOK_OF 330
#define TOK_OPTIONAL 331
#define TOK_PATTERN 332
#define TOK_PDV 333
#define TOK_PLUS_INFINITY 334
#define TOK_PRESENT 335
#define TOK_PrintableString 336
#define TOK_PRIVATE 337
#define TOK_REAL 338
#define TOK_RELATIVE_OID 339
#define TOK_SEQUENCE 340
#define TOK_SET 341
#define TOK_SIZE 342
#define TOK_STRING 343
#define TOK_SYNTAX 344
#define TOK_T61String 345
#define TOK_TAGS 346
#define TOK_TeletexString 347
#define TOK_TRUE 348
#define TOK_TYPE_IDENTIFIER 349
#define TOK_UNIQUE 350
#define TOK_UNIVERSAL 351
#define TOK_UniversalString 352
#define TOK_UTCTime 353
#define TOK_UTF8String 354
#define TOK_VideotexString 355
#define TOK_VisibleString 356
#define TOK_WITH 357
#define UTF8_BOM 358
#define TOK_EXCEPT 359
#define TOK_INTERSECTION 360
#define TOK_UNION 361
#define TOK_TwoDots 362
#define TOK_ThreeDots 363
#define TOK_SUCCESSORS 364
#define TOK_DESCENDANTS 365

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 114 "asn1p_y.y"

	asn1p_t			*a_grammar;
	asn1p_module_flags_e	 a_module_flags;
	asn1p_module_t		*a_module;
	asn1p_expr_type_e	 a_type;	/* ASN.1 Type */
	asn1p_expr_t		*a_expr;	/* Constructed collection */
	asn1p_constraint_t	*a_constr;	/* Constraint */
	enum asn1p_constraint_type_e	a_ctype;/* Constraint type */
	asn1p_xports_t		*a_xports;	/* IMports/EXports */
	struct AssignedIdentifier a_aid;	/* Assigned Identifier */
	asn1p_oid_t		*a_oid;		/* Object Identifier */
	asn1p_oid_arc_t		 a_oid_arc;	/* Single OID's arc */
	struct asn1p_type_tag_s	 a_tag;		/* A tag */
	asn1p_ref_t		*a_ref;		/* Reference to custom type */
	asn1p_wsyntx_t		*a_wsynt;	/* WITH SYNTAX contents */
	asn1p_wsyntx_chunk_t	*a_wchunk;	/* WITH SYNTAX chunk */
	struct asn1p_ref_component_s a_refcomp;	/* Component of a reference */
	asn1p_value_t		*a_value;	/* Number, DefinedValue, etc */
	struct asn1p_param_s	 a_parg;	/* A parameter argument */
	asn1p_paramlist_t	*a_plist;	/* A pargs list */
	struct asn1p_expr_marker_s a_marker;	/* OPTIONAL/DEFAULT */
	enum asn1p_constr_pres_e a_pres;	/* PRESENT/ABSENT/OPTIONAL */
	asn1c_integer_t		 a_int;
	double			 a_dbl;
	char	*tv_str;
	struct {
		char *buf;
		int len;
	}	tv_opaque;
	struct {
		char *name;
		struct asn1p_type_tag_s tag;
	} tv_nametag;

#line 491 "asn1p_y.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE asn1p_lval;


int asn1p_parse (void **param);


#endif /* !YY_ASN1P_ASN_P_Y_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TOK_PPEQ = 3,                   /* TOK_PPEQ  */
  YYSYMBOL_TOK_VBracketLeft = 4,           /* TOK_VBracketLeft  */
  YYSYMBOL_TOK_VBracketRight = 5,          /* TOK_VBracketRight  */
  YYSYMBOL_TOK_whitespace = 6,             /* TOK_whitespace  */
  YYSYMBOL_TOK_opaque = 7,                 /* TOK_opaque  */
  YYSYMBOL_TOK_bstring = 8,                /* TOK_bstring  */
  YYSYMBOL_TOK_cstring = 9,                /* TOK_cstring  */
  YYSYMBOL_TOK_hstring = 10,               /* TOK_hstring  */
  YYSYMBOL_TOK_identifier = 11,            /* "identifier"  */
  YYSYMBOL_TOK_number = 12,                /* "number"  */
  YYSYMBOL_TOK_number_negative = 13,       /* "negative number"  */
  YYSYMBOL_TOK_realnumber = 14,            /* TOK_realnumber  */
  YYSYMBOL_TOK_tuple = 15,                 /* TOK_tuple  */
  YYSYMBOL_TOK_quadruple = 16,             /* TOK_quadruple  */
  YYSYMBOL_TOK_typereference = 17,         /* TOK_typereference  */
  YYSYMBOL_TOK_capitalreference = 18,      /* TOK_capitalreference  */
  YYSYMBOL_TOK_typefieldreference = 19,    /* TOK_typefieldreference  */
  YYSYMBOL_TOK_valuefieldreference = 20,   /* TOK_valuefieldreference  */
  YYSYMBOL_TOK_Literal = 21,               /* TOK_Literal  */
  YYSYMBOL_TOK_ExtValue_BIT_STRING = 22,   /* TOK_ExtValue_BIT_STRING  */
  YYSYMBOL_TOK_ABSENT = 23,                /* TOK_ABSENT  */
  YYSYMBOL_TOK_ABSTRACT_SYNTAX = 24,       /* TOK_ABSTRACT_SYNTAX  */
  YYSYMBOL_TOK_ALL = 25,                   /* TOK_ALL  */
  YYSYMBOL_TOK_ANY = 26,                   /* TOK_ANY  */
  YYSYMBOL_TOK_APPLICATION = 27,           /* TOK_APPLICATION  */
  YYSYMBOL_TOK_AUTOMATIC = 28,             /* TOK_AUTOMATIC  */
  YYSYMBOL_TOK_BEGIN = 29,                 /* TOK_BEGIN  */
  YYSYMBOL_TOK_BIT = 30,                   /* TOK_BIT  */
  YYSYMBOL_TOK_BMPString = 31,             /* TOK_BMPString  */
  YYSYMBOL_TOK_BOOLEAN = 32,               /* TOK_BOOLEAN  */
  YYSYMBOL_TOK_BY = 33,                    /* TOK_BY  */
  YYSYMBOL_TOK_CHARACTER = 34,             /* TOK_CHARACTER  */
  YYSYMBOL_TOK_CHOICE = 35,                /* TOK_CHOICE  */
  YYSYMBOL_TOK_CLASS = 36,                 /* TOK_CLASS  */
  YYSYMBOL_TOK_COMPONENT = 37,             /* TOK_COMPONENT  */
  YYSYMBOL_TOK_COMPONENTS = 38,            /* TOK_COMPONENTS  */
  YYSYMBOL_TOK_CONSTRAINED = 39,           /* TOK_CONSTRAINED  */
  YYSYMBOL_TOK_CONTAINING = 40,            /* TOK_CONTAINING  */
  YYSYMBOL_TOK_DEFAULT = 41,               /* TOK_DEFAULT  */
  YYSYMBOL_TOK_DEFINITIONS = 42,           /* TOK_DEFINITIONS  */
  YYSYMBOL_TOK_DEFINED = 43,               /* TOK_DEFINED  */
  YYSYMBOL_TOK_EMBEDDED = 44,              /* TOK_EMBEDDED  */
  YYSYMBOL_TOK_ENCODED = 45,               /* TOK_ENCODED  */
  YYSYMBOL_TOK_ENCODING_CONTROL = 46,      /* TOK_ENCODING_CONTROL  */
  YYSYMBOL_TOK_END = 47,                   /* TOK_END  */
  YYSYMBOL_TOK_ENUMERATED = 48,            /* TOK_ENUMERATED  */
  YYSYMBOL_TOK_EXPLICIT = 49,              /* TOK_EXPLICIT  */
  YYSYMBOL_TOK_EXPORTS = 50,               /* TOK_EXPORTS  */
  YYSYMBOL_TOK_EXTENSIBILITY = 51,         /* TOK_EXTENSIBILITY  */
  YYSYMBOL_TOK_FALSE = 52,                 /* TOK_FALSE  */
  YYSYMBOL_TOK_FROM = 53,                  /* TOK_FROM  */
  YYSYMBOL_TOK_GeneralizedTime = 54,       /* TOK_GeneralizedTime  */
  YYSYMBOL_TOK_GeneralString = 55,         /* TOK_GeneralString  */
  YYSYMBOL_TOK_GraphicString = 56,         /* TOK_GraphicString  */
  YYSYMBOL_TOK_IA5String = 57,             /* TOK_IA5String  */
  YYSYMBOL_TOK_IDENTIFIER = 58,            /* TOK_IDENTIFIER  */
  YYSYMBOL_TOK_IMPLICIT = 59,              /* TOK_IMPLICIT  */
  YYSYMBOL_TOK_IMPLIED = 60,               /* TOK_IMPLIED  */
  YYSYMBOL_TOK_IMPORTS = 61,               /* TOK_IMPORTS  */
  YYSYMBOL_TOK_INCLUDES = 62,              /* TOK_INCLUDES  */
  YYSYMBOL_TOK_INSTANCE = 63,              /* TOK_INSTANCE  */
  YYSYMBOL_TOK_INSTRUCTIONS = 64,          /* TOK_INSTRUCTIONS  */
  YYSYMBOL_TOK_INTEGER = 65,               /* TOK_INTEGER  */
  YYSYMBOL_TOK_ISO646String = 66,          /* TOK_ISO646String  */
  YYSYMBOL_TOK_MAX = 67,                   /* TOK_MAX  */
  YYSYMBOL_TOK_MIN = 68,                   /* TOK_MIN  */
  YYSYMBOL_TOK_MINUS_INFINITY = 69,        /* TOK_MINUS_INFINITY  */
  YYSYMBOL_TOK_NULL = 70,                  /* TOK_NULL  */
  YYSYMBOL_TOK_NumericString = 71,         /* TOK_NumericString  */
  YYSYMBOL_TOK_OBJECT = 72,                /* TOK_OBJECT  */
  YYSYMBOL_TOK_ObjectDescriptor = 73,      /* TOK_ObjectDescriptor  */
  YYSYMBOL_TOK_OCTET = 74,                 /* TOK_OCTET  */
  YYSYMBOL_TOK_OF = 75,                    /* TOK_OF  */
  YYSYMBOL_TOK_OPTIONAL = 76,              /* TOK_OPTIONAL  */
  YYSYMBOL_TOK_PATTERN = 77,               /* TOK_PATTERN  */
  YYSYMBOL_TOK_PDV = 78,                   /* TOK_PDV  */
  YYSYMBOL_TOK_PLUS_INFINITY = 79,         /* TOK_PLUS_INFINITY  */
  YYSYMBOL_TOK_PRESENT = 80,               /* TOK_PRESENT  */
  YYSYMBOL_TOK_PrintableString = 81,       /* TOK_PrintableString  */
  YYSYMBOL_TOK_PRIVATE = 82,               /* TOK_PRIVATE  */
  YYSYMBOL_TOK_REAL = 83,                  /* TOK_REAL  */
  YYSYMBOL_TOK_RELATIVE_OID = 84,          /* TOK_RELATIVE_OID  */
  YYSYMBOL_TOK_SEQUENCE = 85,              /* TOK_SEQUENCE  */
  YYSYMBOL_TOK_SET = 86,                   /* TOK_SET  */
  YYSYMBOL_TOK_SIZE = 87,                  /* TOK_SIZE  */
  YYSYMBOL_TOK_STRING = 88,                /* TOK_STRING  */
  YYSYMBOL_TOK_SYNTAX = 89,                /* TOK_SYNTAX  */
  YYSYMBOL_TOK_T61String = 90,             /* TOK_T61String  */
  YYSYMBOL_TOK_TAGS = 91,                  /* TOK_TAGS  */
  YYSYMBOL_TOK_TeletexString = 92,         /* TOK_TeletexString  */
  YYSYMBOL_TOK_TRUE = 93,                  /* TOK_TRUE  */
  YYSYMBOL_TOK_TYPE_IDENTIFIER = 94,       /* TOK_TYPE_IDENTIFIER  */
  YYSYMBOL_TOK_UNIQUE = 95,                /* TOK_UNIQUE  */
  YYSYMBOL_TOK_UNIVERSAL = 96,             /* TOK_UNIVERSAL  */
  YYSYMBOL_TOK_UniversalString = 97,       /* TOK_UniversalString  */
  YYSYMBOL_TOK_UTCTime = 98,               /* TOK_UTCTime  */
  YYSYMBOL_TOK_UTF8String = 99,            /* TOK_UTF8String  */
  YYSYMBOL_TOK_VideotexString = 100,       /* TOK_VideotexString  */
  YYSYMBOL_TOK_VisibleString = 101,        /* TOK_VisibleString  */
  YYSYMBOL_TOK_WITH = 102,                 /* TOK_WITH  */
  YYSYMBOL_UTF8_BOM = 103,                 /* "UTF-8 byte order mark"  */
  YYSYMBOL_TOK_EXCEPT = 104,               /* TOK_EXCEPT  */
  YYSYMBOL_105_ = 105,                     /* '^'  */
  YYSYMBOL_TOK_INTERSECTION = 106,         /* TOK_INTERSECTION  */
  YYSYMBOL_107_ = 107,                     /* '|'  */
  YYSYMBOL_TOK_UNION = 108,                /* TOK_UNION  */
  YYSYMBOL_TOK_TwoDots = 109,              /* ".."  */
  YYSYMBOL_TOK_ThreeDots = 110,            /* "..."  */
  YYSYMBOL_TOK_SUCCESSORS = 111,           /* TOK_SUCCESSORS  */
  YYSYMBOL_TOK_DESCENDANTS = 112,          /* TOK_DESCENDANTS  */
  YYSYMBOL_113_concrete_TypeDeclaration_ = 113, /* "concrete TypeDeclaration"  */
  YYSYMBOL_114_ = 114,                     /* '{'  */
  YYSYMBOL_115_ = 115,                     /* '}'  */
  YYSYMBOL_116_ = 116,                     /* '('  */
  YYSYMBOL_117_ = 117,                     /* ')'  */
  YYSYMBOL_118_ = 118,                     /* ';'  */
  YYSYMBOL_119_ = 119,                     /* ','  */
  YYSYMBOL_120_ = 120,                     /* ':'  */
  YYSYMBOL_121_ = 121,                     /* '['  */
  YYSYMBOL_122_ = 122,                     /* ']'  */
  YYSYMBOL_123_ = 123,                     /* '!'  */
  YYSYMBOL_124_ = 124,                     /* '.'  */
  YYSYMBOL_125_ = 125,                     /* '<'  */
  YYSYMBOL_126_ = 126,                     /* '@'  */
  YYSYMBOL_YYACCEPT = 127,                 /* $accept  */
  YYSYMBOL_ParsedGrammar = 128,            /* ParsedGrammar  */
  YYSYMBOL_ModuleList = 129,               /* ModuleList  */
  YYSYMBOL_ModuleDefinition = 130,         /* ModuleDefinition  */
  YYSYMBOL_131_1 = 131,                    /* $@1  */
  YYSYMBOL_optObjectIdentifier = 132,      /* optObjectIdentifier  */
  YYSYMBOL_ObjectIdentifier = 133,         /* ObjectIdentifier  */
  YYSYMBOL_ObjectIdentifierBody = 134,     /* ObjectIdentifierBody  */
  YYSYMBOL_ObjectIdentifierElement = 135,  /* ObjectIdentifierElement  */
  YYSYMBOL_optModuleDefinitionFlags = 136, /* optModuleDefinitionFlags  */
  YYSYMBOL_ModuleDefinitionFlags = 137,    /* ModuleDefinitionFlags  */
  YYSYMBOL_ModuleDefinitionFlag = 138,     /* ModuleDefinitionFlag  */
  YYSYMBOL_optModuleBody = 139,            /* optModuleBody  */
  YYSYMBOL_ModuleBody = 140,               /* ModuleBody  */
  YYSYMBOL_AssignmentList = 141,           /* AssignmentList  */
  YYSYMBOL_Assignment = 142,               /* Assignment  */
  YYSYMBOL_143_2 = 143,                    /* $@2  */
  YYSYMBOL_optImports = 144,               /* optImports  */
  YYSYMBOL_ImportsDefinition = 145,        /* ImportsDefinition  */
  YYSYMBOL_optImportsBundleSet = 146,      /* optImportsBundleSet  */
  YYSYMBOL_ImportsBundleSet = 147,         /* ImportsBundleSet  */
  YYSYMBOL_AssignedIdentifier = 148,       /* AssignedIdentifier  */
  YYSYMBOL_ImportsBundle = 149,            /* ImportsBundle  */
  YYSYMBOL_ImportsBundleInt = 150,         /* ImportsBundleInt  */
  YYSYMBOL_ImportsList = 151,              /* ImportsList  */
  YYSYMBOL_ImportsElement = 152,           /* ImportsElement  */
  YYSYMBOL_ImportSelectionOption = 153,    /* ImportSelectionOption  */
  YYSYMBOL_optExports = 154,               /* optExports  */
  YYSYMBOL_ExportsDefinition = 155,        /* ExportsDefinition  */
  YYSYMBOL_ExportsBody = 156,              /* ExportsBody  */
  YYSYMBOL_ExportsElement = 157,           /* ExportsElement  */
  YYSYMBOL_ValueSet = 158,                 /* ValueSet  */
  YYSYMBOL_ValueSetTypeAssignment = 159,   /* ValueSetTypeAssignment  */
  YYSYMBOL_DefinedType = 160,              /* DefinedType  */
  YYSYMBOL_DataTypeReference = 161,        /* DataTypeReference  */
  YYSYMBOL_ParameterArgumentList = 162,    /* ParameterArgumentList  */
  YYSYMBOL_ParameterArgumentName = 163,    /* ParameterArgumentName  */
  YYSYMBOL_ActualParameterList = 164,      /* ActualParameterList  */
  YYSYMBOL_ActualParameter = 165,          /* ActualParameter  */
  YYSYMBOL_optComponentTypeLists = 166,    /* optComponentTypeLists  */
  YYSYMBOL_ComponentTypeLists = 167,       /* ComponentTypeLists  */
  YYSYMBOL_ComponentType = 168,            /* ComponentType  */
  YYSYMBOL_AlternativeTypeLists = 169,     /* AlternativeTypeLists  */
  YYSYMBOL_AlternativeType = 170,          /* AlternativeType  */
  YYSYMBOL_ObjectClass = 171,              /* ObjectClass  */
  YYSYMBOL_optUNIQUE = 172,                /* optUNIQUE  */
  YYSYMBOL_FieldSpec = 173,                /* FieldSpec  */
  YYSYMBOL_ClassField = 174,               /* ClassField  */
  YYSYMBOL_optWithSyntax = 175,            /* optWithSyntax  */
  YYSYMBOL_WithSyntax = 176,               /* WithSyntax  */
  YYSYMBOL_177_3 = 177,                    /* $@3  */
  YYSYMBOL_WithSyntaxList = 178,           /* WithSyntaxList  */
  YYSYMBOL_WithSyntaxToken = 179,          /* WithSyntaxToken  */
  YYSYMBOL_ExtensionAndException = 180,    /* ExtensionAndException  */
  YYSYMBOL_Type = 181,                     /* Type  */
  YYSYMBOL_TaggedType = 182,               /* TaggedType  */
  YYSYMBOL_DefinedUntaggedType = 183,      /* DefinedUntaggedType  */
  YYSYMBOL_UntaggedType = 184,             /* UntaggedType  */
  YYSYMBOL_MaybeIndirectTaggedType = 185,  /* MaybeIndirectTaggedType  */
  YYSYMBOL_NSTD_IndirectMarker = 186,      /* NSTD_IndirectMarker  */
  YYSYMBOL_MaybeIndirectTypeDeclaration = 187, /* MaybeIndirectTypeDeclaration  */
  YYSYMBOL_TypeDeclaration = 188,          /* TypeDeclaration  */
  YYSYMBOL_ConcreteTypeDeclaration = 189,  /* ConcreteTypeDeclaration  */
  YYSYMBOL_ComplexTypeReference = 190,     /* ComplexTypeReference  */
  YYSYMBOL_ComplexTypeReferenceAmpList = 191, /* ComplexTypeReferenceAmpList  */
  YYSYMBOL_ComplexTypeReferenceElement = 192, /* ComplexTypeReferenceElement  */
  YYSYMBOL_PrimitiveFieldReference = 193,  /* PrimitiveFieldReference  */
  YYSYMBOL_FieldName = 194,                /* FieldName  */
  YYSYMBOL_DefinedObjectClass = 195,       /* DefinedObjectClass  */
  YYSYMBOL_ValueAssignment = 196,          /* ValueAssignment  */
  YYSYMBOL_Value = 197,                    /* Value  */
  YYSYMBOL_198_4 = 198,                    /* $@4  */
  YYSYMBOL_SimpleValue = 199,              /* SimpleValue  */
  YYSYMBOL_DefinedValue = 200,             /* DefinedValue  */
  YYSYMBOL_RestrictedCharacterStringValue = 201, /* RestrictedCharacterStringValue  */
  YYSYMBOL_Opaque = 202,                   /* Opaque  */
  YYSYMBOL_OpaqueFirstToken = 203,         /* OpaqueFirstToken  */
  YYSYMBOL_BasicTypeId = 204,              /* BasicTypeId  */
  YYSYMBOL_BasicTypeId_UniverationCompatible = 205, /* BasicTypeId_UniverationCompatible  */
  YYSYMBOL_BuiltinType = 206,              /* BuiltinType  */
  YYSYMBOL_BasicString = 207,              /* BasicString  */
  YYSYMBOL_UnionMark = 208,                /* UnionMark  */
  YYSYMBOL_IntersectionMark = 209,         /* IntersectionMark  */
  YYSYMBOL_optConstraint = 210,            /* optConstraint  */
  YYSYMBOL_optManyConstraints = 211,       /* optManyConstraints  */
  YYSYMBOL_optSizeOrConstraint = 212,      /* optSizeOrConstraint  */
  YYSYMBOL_Constraint = 213,               /* Constraint  */
  YYSYMBOL_ManyConstraints = 214,          /* ManyConstraints  */
  YYSYMBOL_ConstraintSpec = 215,           /* ConstraintSpec  */
  YYSYMBOL_SubtypeConstraint = 216,        /* SubtypeConstraint  */
  YYSYMBOL_ElementSetSpecs = 217,          /* ElementSetSpecs  */
  YYSYMBOL_ElementSetSpec = 218,           /* ElementSetSpec  */
  YYSYMBOL_Unions = 219,                   /* Unions  */
  YYSYMBOL_Intersections = 220,            /* Intersections  */
  YYSYMBOL_IntersectionElements = 221,     /* IntersectionElements  */
  YYSYMBOL_Elements = 222,                 /* Elements  */
  YYSYMBOL_SubtypeElements = 223,          /* SubtypeElements  */
  YYSYMBOL_PermittedAlphabet = 224,        /* PermittedAlphabet  */
  YYSYMBOL_SizeConstraint = 225,           /* SizeConstraint  */
  YYSYMBOL_PatternConstraint = 226,        /* PatternConstraint  */
  YYSYMBOL_ValueRange = 227,               /* ValueRange  */
  YYSYMBOL_LowerEndValue = 228,            /* LowerEndValue  */
  YYSYMBOL_UpperEndValue = 229,            /* UpperEndValue  */
  YYSYMBOL_SingleValue = 230,              /* SingleValue  */
  YYSYMBOL_BitStringValue = 231,           /* BitStringValue  */
  YYSYMBOL_ContainedSubtype = 232,         /* ContainedSubtype  */
  YYSYMBOL_InnerTypeConstraints = 233,     /* InnerTypeConstraints  */
  YYSYMBOL_SingleTypeConstraint = 234,     /* SingleTypeConstraint  */
  YYSYMBOL_MultipleTypeConstraints = 235,  /* MultipleTypeConstraints  */
  YYSYMBOL_FullSpecification = 236,        /* FullSpecification  */
  YYSYMBOL_PartialSpecification = 237,     /* PartialSpecification  */
  YYSYMBOL_TypeConstraints = 238,          /* TypeConstraints  */
  YYSYMBOL_NamedConstraint = 239,          /* NamedConstraint  */
  YYSYMBOL_optPresenceConstraint = 240,    /* optPresenceConstraint  */
  YYSYMBOL_PresenceConstraint = 241,       /* PresenceConstraint  */
  YYSYMBOL_GeneralConstraint = 242,        /* GeneralConstraint  */
  YYSYMBOL_UserDefinedConstraint = 243,    /* UserDefinedConstraint  */
  YYSYMBOL_244_5 = 244,                    /* $@5  */
  YYSYMBOL_ContentsConstraint = 245,       /* ContentsConstraint  */
  YYSYMBOL_ConstraintRangeSpec = 246,      /* ConstraintRangeSpec  */
  YYSYMBOL_TableConstraint = 247,          /* TableConstraint  */
  YYSYMBOL_SimpleTableConstraint = 248,    /* SimpleTableConstraint  */
  YYSYMBOL_ComponentRelationConstraint = 249, /* ComponentRelationConstraint  */
  YYSYMBOL_AtNotationList = 250,           /* AtNotationList  */
  YYSYMBOL_AtNotationElement = 251,        /* AtNotationElement  */
  YYSYMBOL_ComponentIdList = 252,          /* ComponentIdList  */
  YYSYMBOL_optMarker = 253,                /* optMarker  */
  YYSYMBOL_Marker = 254,                   /* Marker  */
  YYSYMBOL_IdentifierList = 255,           /* IdentifierList  */
  YYSYMBOL_IdentifierElement = 256,        /* IdentifierElement  */
  YYSYMBOL_NamedNumberList = 257,          /* NamedNumberList  */
  YYSYMBOL_NamedNumber = 258,              /* NamedNumber  */
  YYSYMBOL_NamedBitList = 259,             /* NamedBitList  */
  YYSYMBOL_NamedBit = 260,                 /* NamedBit  */
  YYSYMBOL_Enumerations = 261,             /* Enumerations  */
  YYSYMBOL_UniverationList = 262,          /* UniverationList  */
  YYSYMBOL_UniverationElement = 263,       /* UniverationElement  */
  YYSYMBOL_SignedNumber = 264,             /* SignedNumber  */
  YYSYMBOL_RealValue = 265,                /* RealValue  */
  YYSYMBOL_optTag = 266,                   /* optTag  */
  YYSYMBOL_Tag = 267,                      /* Tag  */
  YYSYMBOL_TagTypeValue = 268,             /* TagTypeValue  */
  YYSYMBOL_TagClass = 269,                 /* TagClass  */
  YYSYMBOL_TagPlicit = 270,                /* TagPlicit  */
  YYSYMBOL_TypeRefName = 271,              /* TypeRefName  */
  YYSYMBOL_optIdentifier = 272,            /* optIdentifier  */
  YYSYMBOL_Identifier = 273,               /* Identifier  */
  YYSYMBOL_IdentifierAsReference = 274,    /* IdentifierAsReference  */
  YYSYMBOL_IdentifierAsValue = 275         /* IdentifierAsValue  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  9
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   957

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  127
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  149
/* YYNRULES -- Number of rules.  */
#define YYNRULES  346
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  529

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   366


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   123,     2,     2,     2,     2,     2,     2,
     116,   117,     2,     2,   119,     2,   124,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   120,   118,
     125,     2,     2,     2,   126,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   121,     2,   122,   105,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   114,   107,   115,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     106,   108,   109,   110,   111,   112,   113
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   417,   417,   420,   426,   431,   448,   448,   477,   478,
     482,   485,   491,   497,   506,   510,   514,   524,   525,   534,
     537,   546,   549,   552,   555,   559,   580,   581,   590,   603,
     606,   623,   630,   644,   652,   651,   665,   678,   679,   682,
     692,   698,   699,   702,   707,   714,   715,   719,   723,   726,
     738,   743,   750,   756,   762,   771,   774,   780,   781,   793,
     796,   799,   807,   812,   819,   825,   831,   840,   843,   863,
     873,   893,   899,   915,   921,   929,   938,   949,   953,   961,
     969,   977,   988,   993,  1000,  1001,  1009,  1017,  1040,  1041,
    1044,  1049,  1053,  1063,  1070,  1076,  1083,  1089,  1094,  1098,
    1105,  1110,  1113,  1120,  1130,  1131,  1135,  1142,  1152,  1162,
    1173,  1183,  1194,  1204,  1215,  1227,  1228,  1235,  1234,  1243,
    1247,  1254,  1258,  1261,  1265,  1271,  1279,  1288,  1299,  1302,
    1309,  1332,  1355,  1379,  1386,  1405,  1406,  1409,  1410,  1416,
    1422,  1428,  1438,  1448,  1454,  1466,  1481,  1489,  1497,  1508,
    1519,  1541,  1549,  1558,  1562,  1567,  1576,  1581,  1586,  1594,
    1617,  1627,  1628,  1629,  1629,  1637,  1642,  1647,  1652,  1653,
    1654,  1655,  1659,  1660,  1678,  1682,  1687,  1695,  1704,  1719,
    1720,  1726,  1727,  1728,  1729,  1730,  1731,  1733,  1734,  1735,
    1736,  1737,  1738,  1745,  1746,  1747,  1751,  1757,  1762,  1767,
    1772,  1777,  1786,  1787,  1791,  1795,  1796,  1797,  1798,  1799,
    1803,  1804,  1805,  1806,  1810,  1811,  1818,  1818,  1819,  1819,
    1823,  1824,  1828,  1829,  1833,  1834,  1835,  1841,  1847,  1848,
    1857,  1857,  1859,  1862,  1866,  1867,  1873,  1884,  1885,  1891,
    1892,  1898,  1899,  1906,  1907,  1913,  1914,  1925,  1931,  1937,
    1938,  1940,  1941,  1942,  1947,  1952,  1957,  1962,  1974,  1983,
    1984,  1990,  1991,  1996,  1999,  2004,  2012,  2018,  2030,  2033,
    2039,  2040,  2040,  2041,  2043,  2056,  2061,  2067,  2081,  2082,
    2086,  2089,  2092,  2100,  2101,  2102,  2107,  2106,  2118,  2127,
    2128,  2129,  2130,  2133,  2136,  2145,  2161,  2167,  2173,  2187,
    2198,  2214,  2217,  2237,  2241,  2245,  2249,  2256,  2261,  2267,
    2276,  2281,  2288,  2296,  2306,  2311,  2318,  2326,  2336,  2351,
    2356,  2363,  2370,  2378,  2386,  2393,  2404,  2408,  2415,  2446,
    2447,  2451,  2458,  2464,  2465,  2466,  2467,  2471,  2472,  2473,
    2477,  2481,  2489,  2490,  2496,  2503,  2510
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "TOK_PPEQ",
  "TOK_VBracketLeft", "TOK_VBracketRight", "TOK_whitespace", "TOK_opaque",
  "TOK_bstring", "TOK_cstring", "TOK_hstring", "\"identifier\"",
  "\"number\"", "\"negative number\"", "TOK_realnumber", "TOK_tuple",
  "TOK_quadruple", "TOK_typereference", "TOK_capitalreference",
  "TOK_typefieldreference", "TOK_valuefieldreference", "TOK_Literal",
  "TOK_ExtValue_BIT_STRING", "TOK_ABSENT", "TOK_ABSTRACT_SYNTAX",
  "TOK_ALL", "TOK_ANY", "TOK_APPLICATION", "TOK_AUTOMATIC", "TOK_BEGIN",
  "TOK_BIT", "TOK_BMPString", "TOK_BOOLEAN", "TOK_BY", "TOK_CHARACTER",
  "TOK_CHOICE", "TOK_CLASS", "TOK_COMPONENT", "TOK_COMPONENTS",
  "TOK_CONSTRAINED", "TOK_CONTAINING", "TOK_DEFAULT", "TOK_DEFINITIONS",
  "TOK_DEFINED", "TOK_EMBEDDED", "TOK_ENCODED", "TOK_ENCODING_CONTROL",
  "TOK_END", "TOK_ENUMERATED", "TOK_EXPLICIT", "TOK_EXPORTS",
  "TOK_EXTENSIBILITY", "TOK_FALSE", "TOK_FROM", "TOK_GeneralizedTime",
  "TOK_GeneralString", "TOK_GraphicString", "TOK_IA5String",
  "TOK_IDENTIFIER", "TOK_IMPLICIT", "TOK_IMPLIED", "TOK_IMPORTS",
  "TOK_INCLUDES", "TOK_INSTANCE", "TOK_INSTRUCTIONS", "TOK_INTEGER",
  "TOK_ISO646String", "TOK_MAX", "TOK_MIN", "TOK_MINUS_INFINITY",
  "TOK_NULL", "TOK_NumericString", "TOK_OBJECT", "TOK_ObjectDescriptor",
  "TOK_OCTET", "TOK_OF", "TOK_OPTIONAL", "TOK_PATTERN", "TOK_PDV",
  "TOK_PLUS_INFINITY", "TOK_PRESENT", "TOK_PrintableString", "TOK_PRIVATE",
  "TOK_REAL", "TOK_RELATIVE_OID", "TOK_SEQUENCE", "TOK_SET", "TOK_SIZE",
  "TOK_STRING", "TOK_SYNTAX", "TOK_T61String", "TOK_TAGS",
  "TOK_TeletexString", "TOK_TRUE", "TOK_TYPE_IDENTIFIER", "TOK_UNIQUE",
  "TOK_UNIVERSAL", "TOK_UniversalString", "TOK_UTCTime", "TOK_UTF8String",
  "TOK_VideotexString", "TOK_VisibleString", "TOK_WITH",
  "\"UTF-8 byte order mark\"", "TOK_EXCEPT", "'^'", "TOK_INTERSECTION",
  "'|'", "TOK_UNION", "\"..\"", "\"...\"", "TOK_SUCCESSORS",
  "TOK_DESCENDANTS", "\"concrete TypeDeclaration\"", "'{'", "'}'", "'('",
  "')'", "';'", "','", "':'", "'['", "']'", "'!'", "'.'", "'<'", "'@'",
  "$accept", "ParsedGrammar", "ModuleList", "ModuleDefinition", "$@1",
  "optObjectIdentifier", "ObjectIdentifier", "ObjectIdentifierBody",
  "ObjectIdentifierElement", "optModuleDefinitionFlags",
  "ModuleDefinitionFlags", "ModuleDefinitionFlag", "optModuleBody",
  "ModuleBody", "AssignmentList", "Assignment", "$@2", "optImports",
  "ImportsDefinition", "optImportsBundleSet", "ImportsBundleSet",
  "AssignedIdentifier", "ImportsBundle", "ImportsBundleInt", "ImportsList",
  "ImportsElement", "ImportSelectionOption", "optExports",
  "ExportsDefinition", "ExportsBody", "ExportsElement", "ValueSet",
  "ValueSetTypeAssignment", "DefinedType", "DataTypeReference",
  "ParameterArgumentList", "ParameterArgumentName", "ActualParameterList",
  "ActualParameter", "optComponentTypeLists", "ComponentTypeLists",
  "ComponentType", "AlternativeTypeLists", "AlternativeType",
  "ObjectClass", "optUNIQUE", "FieldSpec", "ClassField", "optWithSyntax",
  "WithSyntax", "$@3", "WithSyntaxList", "WithSyntaxToken",
  "ExtensionAndException", "Type", "TaggedType", "DefinedUntaggedType",
  "UntaggedType", "MaybeIndirectTaggedType", "NSTD_IndirectMarker",
  "MaybeIndirectTypeDeclaration", "TypeDeclaration",
  "ConcreteTypeDeclaration", "ComplexTypeReference",
  "ComplexTypeReferenceAmpList", "ComplexTypeReferenceElement",
  "PrimitiveFieldReference", "FieldName", "DefinedObjectClass",
  "ValueAssignment", "Value", "$@4", "SimpleValue", "DefinedValue",
  "RestrictedCharacterStringValue", "Opaque", "OpaqueFirstToken",
  "BasicTypeId", "BasicTypeId_UniverationCompatible", "BuiltinType",
  "BasicString", "UnionMark", "IntersectionMark", "optConstraint",
  "optManyConstraints", "optSizeOrConstraint", "Constraint",
  "ManyConstraints", "ConstraintSpec", "SubtypeConstraint",
  "ElementSetSpecs", "ElementSetSpec", "Unions", "Intersections",
  "IntersectionElements", "Elements", "SubtypeElements",
  "PermittedAlphabet", "SizeConstraint", "PatternConstraint", "ValueRange",
  "LowerEndValue", "UpperEndValue", "SingleValue", "BitStringValue",
  "ContainedSubtype", "InnerTypeConstraints", "SingleTypeConstraint",
  "MultipleTypeConstraints", "FullSpecification", "PartialSpecification",
  "TypeConstraints", "NamedConstraint", "optPresenceConstraint",
  "PresenceConstraint", "GeneralConstraint", "UserDefinedConstraint",
  "$@5", "ContentsConstraint", "ConstraintRangeSpec", "TableConstraint",
  "SimpleTableConstraint", "ComponentRelationConstraint", "AtNotationList",
  "AtNotationElement", "ComponentIdList", "optMarker", "Marker",
  "IdentifierList", "IdentifierElement", "NamedNumberList", "NamedNumber",
  "NamedBitList", "NamedBit", "Enumerations", "UniverationList",
  "UniverationElement", "SignedNumber", "RealValue", "optTag", "Tag",
  "TagTypeValue", "TagClass", "TagPlicit", "TypeRefName", "optIdentifier",
  "Identifier", "IdentifierAsReference", "IdentifierAsValue", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-371)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-304)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      51,  -371,  -371,   175,   129,   175,  -371,  -371,   175,  -371,
    -371,   -74,    70,    60,  -371,  -371,  -371,  -371,    88,  -371,
      35,   204,  -371,  -371,   154,   117,   110,   123,   148,   138,
     231,   204,  -371,   134,  -371,  -371,  -371,  -371,  -371,   230,
    -371,  -371,    69,    66,   234,  -371,   208,  -371,   160,  -371,
     102,  -371,   174,  -371,  -371,    68,   695,  -371,  -371,  -371,
     153,   183,  -371,   203,   153,  -371,   236,     5,  -371,   219,
    -371,  -371,   322,  -371,  -371,  -371,  -371,  -371,  -371,  -371,
    -371,  -371,  -371,  -371,  -371,  -371,   695,  -371,  -371,  -371,
    -371,  -371,     9,   224,  -371,  -371,  -371,  -371,   192,  -371,
     175,   153,   238,  -371,  -371,    -6,   856,    10,   344,  -371,
     780,  -371,   120,   351,  -371,  -371,   -74,  -371,  -371,  -371,
     241,  -371,  -371,   268,  -371,   270,   281,  -371,  -371,  -371,
    -371,   302,   273,  -371,  -371,  -371,    92,  -371,   242,  -371,
    -371,   243,  -371,  -371,  -371,   353,   252,   244,   246,   253,
     328,   285,   260,   261,   313,   275,   101,   112,  -371,  -371,
     274,  -371,   277,  -371,  -371,  -371,  -371,  -371,   687,  -371,
    -371,   217,  -371,  -371,  -371,  -371,  -371,   390,   856,   153,
     153,   286,   369,  -371,   175,   295,    61,   359,   296,    62,
      22,   306,   384,   274,   109,   232,   321,  -371,  -371,   109,
     334,  -371,  -371,   274,   593,  -371,  -371,  -371,  -371,  -371,
    -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,
    -371,  -371,  -371,  -371,   287,  -371,  -371,  -371,   220,    27,
     137,  -371,    -6,  -371,  -371,  -371,  -371,  -371,  -371,   244,
     246,   308,   274,   224,  -371,   125,   290,  -371,   389,   274,
    -371,  -371,   298,   297,   222,   226,  -371,   311,  -371,  -371,
    -371,  -371,  -371,   -29,    50,  -371,  -371,  -371,  -371,  -371,
     293,  -371,  -371,  -371,  -371,   139,  -371,  -371,   384,   384,
     300,   145,  -371,  -371,  -371,  -371,   224,  -371,   303,   301,
    -371,  -371,   309,  -371,   171,  -371,   310,  -371,   349,   304,
     314,  -371,  -371,    52,   224,   394,   224,   175,   312,  -371,
    -371,  -371,  -371,  -371,  -371,   316,  -371,   384,   319,   384,
    -371,   320,  -371,   172,  -371,  -371,  -371,  -371,   282,   384,
    -371,  -371,   687,  -371,    52,    29,    52,  -371,  -371,   340,
      29,    52,   336,   217,  -371,  -371,   498,  -371,  -371,  -371,
    -371,   274,   326,   327,  -371,  -371,   335,  -371,  -371,   498,
    -371,  -371,   498,   498,   325,   338,   520,   317,  -371,   384,
    -371,   180,  -371,   333,   262,  -371,    16,   780,   274,  -371,
    -371,    22,   262,  -371,   384,   262,   224,  -371,    21,  -371,
      52,   339,  -371,   337,  -371,   329,   224,  -371,  -371,   224,
    -371,   593,  -371,   447,  -371,  -371,  -371,  -371,  -371,   324,
    -371,  -371,  -371,    52,  -371,  -371,   371,  -371,  -371,  -371,
    -371,  -371,  -371,    25,  -371,  -371,  -371,  -371,   342,   226,
    -371,  -371,  -371,   343,  -371,  -371,  -371,  -371,  -371,  -371,
     384,   265,  -371,  -371,    62,  -371,  -371,  -371,  -371,   341,
     346,  -371,   347,   348,  -371,    40,  -371,  -371,  -371,  -371,
      15,   182,  -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,
     355,   354,   191,  -371,   274,   389,  -371,  -371,   350,   360,
      19,  -371,  -371,  -371,  -371,    24,   282,   384,   356,  -371,
    -371,   329,  -371,  -371,  -371,   384,  -371,   384,    18,  -371,
    -371,  -371,  -371,  -371,  -371,   447,   356,   384,  -371,    44,
     207,  -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,  -371,
      44,    41,  -371,  -371,  -371,    36,  -371,  -371,  -371
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       0,   340,   341,     0,     0,     3,     4,     6,     2,     1,
       5,     8,     0,     0,     9,   344,    16,    11,     0,    12,
      14,    17,    10,    13,     0,     0,     0,     0,     0,     0,
       0,    18,    19,     0,    25,    23,    21,    24,    22,     0,
      20,    15,    57,     0,     0,    27,    37,    58,     0,    61,
       0,    62,    64,    66,     7,    41,     0,    38,    60,    59,
       0,     0,    40,     0,    42,    43,    48,     0,    50,    52,
      54,   202,     0,   203,   204,   205,   206,   207,   215,   208,
     209,   210,   211,   212,   213,   214,    28,    29,    33,    31,
      32,    36,   329,   329,    63,    65,    39,    44,     0,    47,
       0,     0,     0,    34,    30,   329,     0,   333,     0,   128,
       0,   330,   337,     0,    55,    56,    45,    51,    53,    35,
       0,    72,    71,     0,   181,     0,     0,   194,   190,   193,
     182,     0,     0,   183,   186,   189,     0,    75,     0,   192,
     191,    77,   335,   336,   334,     0,     0,   146,   147,     0,
     143,     0,     0,   194,     0,   193,   224,   224,   136,   129,
     222,   135,    69,   196,   137,   339,   338,   331,     0,    46,
      49,     0,   195,   188,   187,   185,   184,     0,     0,     0,
       0,     0,     0,    68,     0,     0,     0,     0,   195,   329,
       0,     0,     0,     0,   329,     0,     0,   225,   226,   329,
       0,   131,   228,   223,     0,   264,   174,   265,   326,   327,
     328,   175,   176,   166,   165,   167,   163,   160,   161,   162,
     170,   171,   168,   169,     0,   345,   346,   172,   329,   329,
       0,   106,   329,    76,    81,    80,    79,    78,   332,   146,
     147,     0,     0,   329,   260,     0,     0,   233,     0,   222,
     267,   263,     0,   234,   237,   239,   241,   243,   245,   249,
     250,   252,   253,     0,   247,   248,   251,   148,   154,   155,
     150,   151,   153,   149,   201,     0,   307,   309,     0,     0,
     125,     0,    97,   101,   102,   133,   329,   325,     0,   318,
     319,   324,   321,   145,     0,   310,     0,   255,     0,     0,
      89,    90,    96,   303,   329,     0,   329,   163,     0,   230,
     232,   231,   283,   285,   284,   293,   294,   342,     0,   342,
     229,   165,    87,     0,    82,    84,    85,    86,     0,     0,
     159,   156,     0,   305,   303,   303,   303,   108,   304,   104,
     303,   303,   115,     0,    74,    73,     0,   254,   266,   256,
     257,     0,     0,     0,   130,    67,     0,   216,   217,     0,
     218,   219,     0,     0,   289,     0,     0,     0,   200,     0,
     144,     0,   314,     0,     0,   138,   329,     0,   222,   100,
     198,     0,     0,   197,     0,     0,   329,   139,   329,    94,
     303,     0,   288,     0,   227,     0,   329,   343,   140,   329,
      70,     0,   179,   164,   177,   180,   173,   306,   113,     0,
     112,   114,   105,   303,   110,   111,     0,   103,   116,   107,
     238,   270,   268,     0,   269,   271,   272,   246,   235,   240,
     242,   244,   290,   291,   262,   258,   261,   152,   308,   199,
       0,     0,   126,   127,   329,    98,   134,   132,   320,     0,
       0,   311,     0,     0,    95,   329,    91,    93,   286,   295,
       0,     0,   297,   133,   133,    83,   178,   157,   158,   109,
       0,     0,     0,   275,   220,     0,   292,   315,     0,     0,
       0,   323,   322,   313,   312,     0,     0,     0,   299,   301,
     296,     0,   141,   142,   117,     0,   273,     0,   278,   221,
     236,   316,   317,    99,    92,   287,   300,     0,   298,     0,
       0,   276,   281,   282,   280,   277,   279,   302,   121,   122,
       0,     0,   119,   123,   274,     0,   118,   120,   124
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -371,  -371,   467,    82,  -371,  -371,   362,  -371,   454,  -371,
    -371,   450,  -371,  -371,  -371,   398,  -371,  -371,  -371,  -371,
    -371,  -371,   422,  -371,  -371,   386,  -371,  -371,  -371,  -371,
     428,   352,  -371,  -109,  -371,  -371,   315,  -371,    89,   305,
      34,   104,    53,   118,   264,  -371,  -371,   156,  -371,  -371,
    -371,   -20,  -294,  -174,   -83,  -371,  -371,   391,  -182,  -371,
    -114,   140,  -371,   330,  -371,   135,  -167,   289,   291,  -371,
    -159,  -371,  -191,  -176,  -371,    33,  -371,   -68,  -371,  -371,
      47,  -371,  -371,  -371,  -226,   365,  -154,  -371,  -371,  -371,
     331,  -240,  -371,   164,   162,  -245,  -371,  -371,   195,  -371,
    -371,  -371,  -371,   159,  -371,  -371,  -371,  -371,  -371,  -371,
    -371,    32,    42,  -371,  -371,  -371,  -371,  -371,  -371,  -371,
    -371,  -371,  -371,  -371,    49,    54,  -227,  -371,  -371,   173,
    -371,   161,  -371,   103,  -371,  -371,   163,  -169,  -371,   -88,
    -371,  -371,  -371,  -371,    11,   227,   -12,  -371,  -370
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     4,     5,     6,    11,    13,    14,    18,    19,    30,
      31,    32,    44,    45,    86,    87,   119,    56,    57,    63,
      64,   170,    65,    66,    67,    68,    99,    46,    47,    50,
      51,   322,    88,   249,    89,   136,   137,   323,   324,   299,
     300,   301,   281,   282,   121,   413,   230,   231,   417,   418,
     509,   521,   522,   302,   108,   109,   250,   325,   303,   377,
     378,   160,   161,   162,   270,   271,   523,   335,   336,    90,
     251,   328,   218,   219,   220,   403,   404,   163,   139,   164,
     140,   359,   362,   498,   201,   196,   202,   203,   308,   309,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   435,   264,   221,   265,   266,   422,   424,   425,
     426,   472,   473,   515,   516,   311,   312,   486,   313,   366,
     314,   315,   316,   461,   462,   488,   337,   338,   275,   276,
     294,   295,   371,   372,   288,   289,   290,   222,   223,   285,
     111,   112,   145,   167,   224,   396,   225,   226,   227
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      20,   158,   197,   197,   110,   110,    20,   284,   353,   217,
     113,     7,   105,   326,     7,   283,     7,   110,   272,     7,
     444,   291,   122,   354,   503,   455,    15,    15,   327,   504,
     120,    53,    15,    15,   208,   209,    15,   142,   138,   297,
      12,   512,   518,    70,    93,   330,   331,   518,    53,   320,
     518,    15,    70,   474,    52,   268,   269,   519,   100,   298,
     268,   269,   519,   268,   269,   519,    69,    92,     1,     2,
     332,    52,    15,    15,    93,    69,   389,    15,   298,    15,
     364,    15,    16,     1,     2,     1,     2,    10,   347,    70,
      10,    48,   143,   332,   513,   158,   365,    92,   514,    15,
      16,   420,    21,    91,   379,   333,   144,   408,   410,   411,
     138,   116,    69,   414,   415,   107,   -26,   141,   431,    43,
      15,    62,   390,   106,   101,   474,   280,   474,   333,     9,
     107,   280,   287,    91,   349,   471,    15,   107,   376,   487,
     110,   110,   107,   388,   110,   334,   339,   298,   107,   345,
     280,    24,   447,   409,     3,   110,   526,   520,   528,  -259,
     348,   107,   520,   457,    15,   520,    33,   235,   237,   165,
       1,     2,   280,   407,   277,  -259,   274,   286,   292,   166,
     296,    34,   304,   107,    49,    17,   469,   304,   193,   141,
     234,   236,     1,     2,   284,   267,   273,   421,   442,   193,
     272,    35,   283,    22,   454,   443,   449,   177,    37,   452,
     326,   178,   291,   450,    36,   194,   453,   195,   110,   280,
      59,    60,    25,   392,   -88,   327,   199,   527,   195,    38,
     107,   527,    26,   350,    39,   500,   228,   229,   330,   331,
     205,   206,   207,    15,   208,   209,   210,   211,   212,   239,
     240,    41,   342,    27,   368,    28,   343,   241,   369,    42,
     375,   332,   284,    29,   376,   479,   370,   373,   158,    55,
     283,   305,   306,    15,   208,   209,    15,   478,    58,     1,
       2,    54,     1,     2,   213,   242,   383,   400,    61,   402,
     384,   401,   158,    15,   243,   439,   333,   490,    95,   440,
     244,   491,   214,   114,   115,   397,   496,   397,   463,   245,
     497,   464,     1,     2,   268,   269,   405,   406,   393,   193,
     499,    96,   524,   147,   148,   215,   497,   351,   352,   357,
     358,   360,   361,   102,   246,  -303,   268,   269,    98,  -303,
     103,   107,   247,   467,   468,   107,   307,   146,   248,   492,
     493,   198,   198,   118,   168,   171,   172,   277,   173,   174,
     175,   176,   179,   180,   286,   181,   182,   186,   184,   292,
     185,   187,   296,   188,   189,   190,   304,   205,   206,   207,
      15,   208,   209,   210,   211,   212,   239,   240,   191,   192,
     195,   204,   278,   232,   241,    15,   317,   205,   206,   207,
      15,   208,   209,   210,   211,   212,   239,   240,   238,   319,
     279,   329,   346,   355,   241,   363,   356,   367,   380,   387,
     381,   213,   242,   374,   386,   382,   385,   391,   373,   394,
     395,   243,   286,   388,   398,   412,  -182,   244,   416,   214,
     423,   213,   242,   304,   427,   428,   245,   433,   489,   441,
     432,   243,   459,   458,   466,   460,   193,   244,   481,   214,
     470,   475,   215,   482,   483,   484,   245,   501,   476,   494,
       8,   246,    23,   495,   405,   489,   193,   502,   169,   247,
     507,    40,   215,   216,   104,   248,    97,   117,    94,   485,
     465,   246,   456,   233,   445,   517,   344,   480,   183,   419,
     525,   159,   437,   216,   318,   248,   205,   206,   207,    15,
     208,   209,   210,   211,   212,   239,   240,   446,   340,   505,
     341,   293,   200,   429,   430,   436,   310,   510,   205,   206,
     207,    15,   208,   209,   210,   211,   212,     1,     2,   511,
     508,   506,   438,   477,   448,   451,   399,     0,     0,     0,
     213,   242,     0,     0,     0,     0,     0,     0,     0,     0,
     243,     0,     0,     0,     0,     0,   244,     0,   214,     0,
       0,     0,   213,     0,     0,   245,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   193,     0,   434,     0,     0,
     214,   215,     0,     0,     0,     0,     0,     0,     0,     0,
     246,   205,   206,   207,    15,   208,   209,   210,   211,   212,
     239,   240,   216,   215,   248,   149,     0,     0,     0,   150,
       0,     0,     0,   151,    71,   124,     0,   125,   152,     0,
       0,     0,     0,     0,   216,     0,     0,   126,     0,     0,
       0,   153,     0,     0,     0,   213,     0,   128,    73,    74,
      75,     0,     0,     0,     0,     0,   154,     0,   155,    76,
       0,     0,     0,   321,    77,   131,    78,   132,     0,     0,
       0,     0,     0,     0,    79,     0,   133,   134,   156,   157,
       0,     0,     0,    80,     0,    81,   215,     0,     0,     0,
      82,   135,    83,    84,    85,   205,   206,   207,    15,   208,
     209,   210,   211,   212,     1,     2,    15,   182,     0,     0,
       0,     0,     1,     2,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    71,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   213,
       0,    72,     0,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,     0,     0,     0,     0,   214,     0,     0,
       0,    76,     0,     0,     0,     0,    77,     0,    78,     0,
       0,     0,     0,     0,     0,     0,    79,     0,     0,     0,
     215,     0,     0,     0,     0,    80,     0,    81,     0,     0,
       0,     0,    82,     0,    83,    84,    85,   147,   148,     0,
       0,   216,   149,     0,     0,     0,   150,     0,     0,     0,
     151,    71,   124,     0,   125,   152,     0,     0,     0,     0,
       0,     0,     0,     0,   126,     0,     0,     0,   153,     0,
       0,     0,     0,     0,   128,    73,    74,    75,     0,     0,
       0,     0,     0,   154,     0,   155,    76,     0,     0,     0,
     130,    77,   131,    78,   132,     0,     0,     0,     0,     0,
       0,    79,     0,   133,   134,   156,   157,     0,     0,     0,
      80,     0,    81,     1,     2,     0,     0,    82,   135,    83,
      84,    85,     0,     0,     0,     0,   123,    71,   124,     0,
     125,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     126,     0,     0,     0,   127,     0,     0,     0,     0,     0,
     128,    73,    74,    75,     0,     0,     0,     0,     0,     0,
       0,   129,    76,     0,     0,     0,   130,    77,   131,    78,
     132,     0,     0,     0,     0,     0,     0,    79,     0,   133,
     134,     0,     0,     0,     0,     0,    80,     0,    81,     0,
       0,     0,     0,    82,   135,    83,    84,    85
};

static const yytype_int16 yycheck[] =
{
      12,   110,   156,   157,    92,    93,    18,   189,   248,   168,
      93,     0,     3,   204,     3,   189,     5,   105,   185,     8,
       4,   190,   105,   249,     5,     4,    11,    11,   204,     5,
      36,    43,    11,    11,    12,    13,    11,    27,   106,   193,
     114,    23,     6,    55,    56,    18,    19,     6,    60,   203,
       6,    11,    64,   423,    43,    19,    20,    21,    53,    38,
      19,    20,    21,    19,    20,    21,    55,    56,    17,    18,
      41,    60,    11,    11,    86,    64,   303,    11,    38,    11,
     109,    11,    12,    17,    18,    17,    18,     5,   242,   101,
       8,    25,    82,    41,    76,   204,   125,    86,    80,    11,
      12,   346,    42,    56,   286,    76,    96,   334,   335,   336,
     178,   100,   101,   340,   341,   121,    47,   106,   363,    50,
      11,    53,   304,   114,   119,   495,   110,   497,    76,     0,
     121,   110,   110,    86,     9,   110,    11,   121,   119,   124,
     228,   229,   121,   119,   232,   228,   229,    38,   121,   232,
     110,   116,   378,   124,   103,   243,   115,   121,   122,   109,
     243,   121,   121,   390,    11,   121,    12,   179,   180,    49,
      17,    18,   110,   332,   186,   125,   115,   189,   190,    59,
     192,    64,   194,   121,   118,   115,   413,   199,    87,   178,
     179,   180,    17,    18,   376,   184,   185,   351,   374,    87,
     367,    91,   376,   115,   386,   374,   382,   115,    60,   385,
     401,   119,   381,   382,    91,   114,   385,   116,   306,   110,
     118,   119,    18,   306,   115,   401,   114,   521,   116,    91,
     121,   525,    28,   245,     3,   475,    19,    20,    18,    19,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,   117,   115,    49,   115,    51,   119,    25,   119,    29,
     115,    41,   444,    59,   119,   441,   278,   279,   377,    61,
     444,    39,    40,    11,    12,    13,    11,    12,   118,    17,
      18,    47,    17,    18,    52,    53,   115,   115,   114,     7,
     119,   119,   401,    11,    62,   115,    76,   115,   115,   119,
      68,   119,    70,   111,   112,   317,   115,   319,   396,    77,
     119,   399,    17,    18,    19,    20,   328,   329,   307,    87,
     474,   118,   115,    17,    18,    93,   119,    37,    38,   107,
     108,   105,   106,   114,   102,   115,    19,    20,   102,   119,
      18,   121,   110,    19,    20,   121,   114,     3,   116,   463,
     464,   156,   157,   115,     3,   114,    88,   369,    88,    78,
      58,    88,   120,   120,   376,    12,   114,   114,   124,   381,
     124,    43,   384,    88,   114,   114,   388,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    75,   114,
     116,   114,    33,     3,    25,    11,    75,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,   122,    75,
     114,   124,   104,   115,    25,   104,   119,   124,   115,   115,
     119,    52,    53,   123,    75,   116,   116,    33,   440,   117,
     114,    62,   444,   119,   115,    95,   116,    68,   102,    70,
     114,    52,    53,   455,   117,   110,    77,   109,   460,   116,
     125,    62,   115,   114,     7,   126,    87,    68,   117,    70,
      89,   119,    93,   117,   117,   117,    77,   117,   125,   114,
       3,   102,    18,   119,   486,   487,    87,   117,   116,   110,
     124,    31,    93,   114,    86,   116,    64,   101,    60,   455,
     401,   102,   388,   178,   376,   507,   232,   444,   146,   343,
     520,   110,   367,   114,   199,   116,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,   377,   229,   486,
     229,   191,   157,   359,   362,   366,   195,   495,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,   497,
     491,   487,   369,   440,   381,   384,   319,    -1,    -1,    -1,
      52,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    -1,    -1,    -1,    -1,    -1,    68,    -1,    70,    -1,
      -1,    -1,    52,    -1,    -1,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    67,    -1,    -1,
      70,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     102,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,   114,    93,   116,    22,    -1,    -1,    -1,    26,
      -1,    -1,    -1,    30,    31,    32,    -1,    34,    35,    -1,
      -1,    -1,    -1,    -1,   114,    -1,    -1,    44,    -1,    -1,
      -1,    48,    -1,    -1,    -1,    52,    -1,    54,    55,    56,
      57,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,    66,
      -1,    -1,    -1,    70,    71,    72,    73,    74,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    -1,    83,    84,    85,    86,
      -1,    -1,    -1,    90,    -1,    92,    93,    -1,    -1,    -1,
      97,    98,    99,   100,   101,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    11,   114,    -1,    -1,
      -1,    -1,    17,    18,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    56,    57,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    66,    -1,    -1,    -1,    -1,    71,    -1,    73,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    90,    -1,    92,    -1,    -1,
      -1,    -1,    97,    -1,    99,   100,   101,    17,    18,    -1,
      -1,   114,    22,    -1,    -1,    -1,    26,    -1,    -1,    -1,
      30,    31,    32,    -1,    34,    35,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    66,    -1,    -1,    -1,
      70,    71,    72,    73,    74,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    -1,    83,    84,    85,    86,    -1,    -1,    -1,
      90,    -1,    92,    17,    18,    -1,    -1,    97,    98,    99,
     100,   101,    -1,    -1,    -1,    -1,    30,    31,    32,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    66,    -1,    -1,    -1,    70,    71,    72,    73,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,    83,
      84,    -1,    -1,    -1,    -1,    -1,    90,    -1,    92,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,    17,    18,   103,   128,   129,   130,   271,   129,     0,
     130,   131,   114,   132,   133,    11,    12,   115,   134,   135,
     273,    42,   115,   135,   116,    18,    28,    49,    51,    59,
     136,   137,   138,    12,    64,    91,    91,    60,    91,     3,
     138,   117,    29,    50,   139,   140,   154,   155,    25,   118,
     156,   157,   271,   273,    47,    61,   144,   145,   118,   118,
     119,   114,    53,   146,   147,   149,   150,   151,   152,   271,
     273,    31,    46,    55,    56,    57,    66,    71,    73,    81,
      90,    92,    97,    99,   100,   101,   141,   142,   159,   161,
     196,   207,   271,   273,   157,   115,   118,   149,   102,   153,
      53,   119,   114,    18,   142,     3,   114,   121,   181,   182,
     266,   267,   268,   181,   111,   112,   271,   152,   115,   143,
      36,   171,   181,    30,    32,    34,    44,    48,    54,    65,
      70,    72,    74,    83,    84,    98,   162,   163,   204,   205,
     207,   271,    27,    82,    96,   269,     3,    17,    18,    22,
      26,    30,    35,    48,    63,    65,    85,    86,   160,   184,
     188,   189,   190,   204,   206,    49,    59,   270,     3,   133,
     148,   114,    88,    88,    78,    58,    88,   115,   119,   120,
     120,    12,   114,   158,   124,   124,   114,    43,    88,   114,
     114,    75,   114,    87,   114,   116,   212,   213,   225,   114,
     212,   211,   213,   214,   114,     8,     9,    10,    12,    13,
      14,    15,    16,    52,    70,    93,   114,   197,   199,   200,
     201,   231,   264,   265,   271,   273,   274,   275,    19,    20,
     173,   174,     3,   163,   271,   273,   271,   273,   122,    17,
      18,    25,    53,    62,    68,    77,   102,   110,   116,   160,
     183,   197,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   230,   232,   233,   271,    19,    20,
     191,   192,   193,   271,   115,   255,   256,   273,    33,   114,
     110,   169,   170,   180,   185,   266,   273,   110,   261,   262,
     263,   264,   273,   190,   257,   258,   273,   213,    38,   166,
     167,   168,   180,   185,   273,    39,    40,   114,   215,   216,
     217,   242,   243,   245,   247,   248,   249,    75,   166,    75,
     213,    70,   158,   164,   165,   184,   199,   200,   198,   124,
      18,    19,    41,    76,   181,   194,   195,   253,   254,   181,
     194,   195,   115,   119,   171,   181,   104,   213,   181,     9,
     273,    37,    38,   218,   211,   115,   119,   107,   108,   208,
     105,   106,   209,   104,   109,   125,   246,   124,   115,   119,
     273,   259,   260,   273,   123,   115,   119,   186,   187,   185,
     115,   119,   116,   115,   119,   116,    75,   115,   119,   253,
     185,    33,   181,   271,   117,   114,   272,   273,   115,   272,
     115,   119,     7,   202,   203,   273,   273,   197,   253,   124,
     253,   253,    95,   172,   253,   253,   102,   175,   176,   174,
     222,   213,   234,   114,   235,   236,   237,   117,   110,   220,
     221,   222,   125,   109,    67,   229,   230,   192,   256,   115,
     119,   116,   200,   264,     4,   170,   188,   211,   263,   200,
     264,   258,   200,   264,   185,     4,   168,   253,   114,   115,
     126,   250,   251,   266,   266,   165,     7,    19,    20,   253,
      89,   110,   238,   239,   275,   119,   125,   260,    12,   200,
     169,   117,   117,   117,   117,   167,   244,   124,   252,   273,
     115,   119,   187,   187,   114,   119,   115,   119,   210,   213,
     218,   117,   117,     5,     5,   202,   252,   124,   251,   177,
     238,   239,    23,    76,    80,   240,   241,   273,     6,    21,
     121,   178,   179,   193,   115,   178,   115,   179,   122
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   127,   128,   128,   129,   129,   131,   130,   132,   132,
     133,   133,   134,   134,   135,   135,   135,   136,   136,   137,
     137,   138,   138,   138,   138,   138,   139,   139,   140,   141,
     141,   142,   142,   142,   143,   142,   142,   144,   144,   145,
     145,   146,   146,   147,   147,   148,   148,   149,   149,   150,
     151,   151,   152,   152,   152,   153,   153,   154,   154,   155,
     155,   155,   156,   156,   157,   157,   157,   158,   159,   160,
     160,   161,   161,   161,   161,   162,   162,   163,   163,   163,
     163,   163,   164,   164,   165,   165,   165,   165,   166,   166,
     167,   167,   167,   168,   168,   168,   168,   169,   169,   169,
     170,   170,   170,   171,   172,   172,   173,   173,   174,   174,
     174,   174,   174,   174,   174,   175,   175,   177,   176,   178,
     178,   179,   179,   179,   179,   180,   180,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   188,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   190,   190,   190,   190,
     190,   191,   191,   192,   193,   193,   194,   194,   194,   195,
     196,   197,   197,   198,   197,   199,   199,   199,   199,   199,
     199,   199,   200,   200,   201,   201,   201,   202,   202,   203,
     203,   204,   204,   204,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   205,   205,   205,   206,   206,   206,   206,
     206,   206,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   208,   208,   209,   209,
     210,   210,   211,   211,   212,   212,   212,   213,   214,   214,
     215,   215,   216,   217,   217,   217,   217,   218,   218,   219,
     219,   220,   220,   221,   221,   222,   222,   223,   223,   223,
     223,   223,   223,   223,   224,   225,   226,   226,   227,   228,
     228,   229,   229,   230,   231,   231,   232,   232,   233,   233,
     234,   235,   235,   236,   237,   238,   238,   239,   240,   240,
     241,   241,   241,   242,   242,   242,   244,   243,   245,   246,
     246,   246,   246,   247,   247,   248,   249,   250,   250,   251,
     251,   252,   252,   253,   253,   254,   254,   255,   255,   256,
     257,   257,   258,   258,   259,   259,   260,   260,   261,   262,
     262,   263,   263,   263,   263,   263,   264,   264,   265,   266,
     266,   267,   268,   269,   269,   269,   269,   270,   270,   270,
     271,   271,   272,   272,   273,   274,   275
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     2,     0,     9,     0,     1,
       3,     2,     1,     2,     1,     4,     1,     0,     1,     1,
       2,     2,     2,     2,     2,     2,     0,     1,     3,     1,
       2,     1,     1,     1,     0,     3,     1,     0,     1,     3,
       2,     0,     1,     1,     2,     0,     1,     2,     1,     4,
       1,     3,     1,     3,     1,     2,     2,     0,     1,     3,
       3,     2,     1,     3,     1,     3,     1,     3,     4,     1,
       4,     3,     3,     6,     6,     1,     3,     1,     3,     3,
       3,     3,     1,     3,     1,     1,     1,     1,     0,     1,
       1,     3,     5,     3,     2,     3,     1,     1,     3,     5,
       2,     1,     1,     5,     0,     1,     1,     3,     2,     4,
       3,     3,     3,     3,     3,     0,     1,     0,     6,     1,
       2,     1,     1,     1,     3,     1,     3,     3,     1,     2,
       2,     2,     3,     0,     2,     1,     1,     1,     4,     4,
       4,     6,     6,     1,     4,     3,     1,     1,     3,     3,
       3,     1,     3,     1,     1,     1,     1,     3,     3,     1,
       4,     1,     1,     0,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     1,     1,     1,     2,     1,
       1,     1,     1,     1,     2,     2,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     2,     1,     4,     4,     5,
       4,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     0,     1,     0,     1,     1,     3,     1,     2,
       1,     1,     1,     1,     1,     3,     5,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     3,     3,
       1,     1,     1,     3,     5,     1,     3,     3,     0,     1,
       1,     1,     1,     1,     1,     1,     0,     5,     2,     1,
       2,     2,     3,     1,     1,     3,     4,     1,     3,     2,
       3,     1,     3,     0,     1,     1,     2,     1,     3,     1,
       1,     3,     4,     4,     1,     3,     4,     4,     1,     1,
       3,     1,     4,     4,     1,     1,     1,     1,     1,     0,
       1,     2,     4,     0,     1,     1,     1,     0,     1,     1,
       1,     1,     0,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (param, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, param); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, void **param)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (param);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, void **param)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep, param);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule, void **param)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)], param);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, param); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, void **param)
{
  YY_USE (yyvaluep);
  YY_USE (param);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void **param)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* ParsedGrammar: "UTF-8 byte order mark" ModuleList  */
#line 417 "asn1p_y.y"
                            {
		*(void **)param = (yyvsp[0].a_grammar);
	}
#line 2293 "asn1p_y.c"
    break;

  case 3: /* ParsedGrammar: ModuleList  */
#line 420 "asn1p_y.y"
                     {
		*(void **)param = (yyvsp[0].a_grammar);
	}
#line 2301 "asn1p_y.c"
    break;

  case 4: /* ModuleList: ModuleDefinition  */
#line 426 "asn1p_y.y"
                         {
		(yyval.a_grammar) = asn1p_new();
		checkmem((yyval.a_grammar));
		TQ_ADD(&((yyval.a_grammar)->modules), (yyvsp[0].a_module), mod_next);
	}
#line 2311 "asn1p_y.c"
    break;

  case 5: /* ModuleList: ModuleList ModuleDefinition  */
#line 431 "asn1p_y.y"
                                      {
		(yyval.a_grammar) = (yyvsp[-1].a_grammar);
		TQ_ADD(&((yyval.a_grammar)->modules), (yyvsp[0].a_module), mod_next);
	}
#line 2320 "asn1p_y.c"
    break;

  case 6: /* $@1: %empty  */
#line 448 "asn1p_y.y"
                    { currentModule = asn1p_module_new(); }
#line 2326 "asn1p_y.c"
    break;

  case 7: /* ModuleDefinition: TypeRefName $@1 optObjectIdentifier TOK_DEFINITIONS optModuleDefinitionFlags TOK_PPEQ TOK_BEGIN optModuleBody TOK_END  */
#line 453 "asn1p_y.y"
                        {

		(yyval.a_module) = currentModule;

		if((yyvsp[-1].a_module)) {
			asn1p_module_t tmp = *((yyval.a_module));
			*((yyval.a_module)) = *((yyvsp[-1].a_module));
			*((yyvsp[-1].a_module)) = tmp;
			asn1p_module_free((yyvsp[-1].a_module));
		} else {
			/* There's a chance that a module is just plain empty */
		}

		(yyval.a_module)->ModuleName = (yyvsp[-8].tv_str);
		(yyval.a_module)->module_oid = (yyvsp[-6].a_oid);
		(yyval.a_module)->module_flags = (yyvsp[-4].a_module_flags);
	}
#line 2348 "asn1p_y.c"
    break;

  case 8: /* optObjectIdentifier: %empty  */
#line 477 "asn1p_y.y"
        { (yyval.a_oid) = 0; }
#line 2354 "asn1p_y.c"
    break;

  case 9: /* optObjectIdentifier: ObjectIdentifier  */
#line 478 "asn1p_y.y"
                           { (yyval.a_oid) = (yyvsp[0].a_oid); }
#line 2360 "asn1p_y.c"
    break;

  case 10: /* ObjectIdentifier: '{' ObjectIdentifierBody '}'  */
#line 482 "asn1p_y.y"
                                     {
		(yyval.a_oid) = (yyvsp[-1].a_oid);
	}
#line 2368 "asn1p_y.c"
    break;

  case 11: /* ObjectIdentifier: '{' '}'  */
#line 485 "asn1p_y.y"
                  {
		(yyval.a_oid) = 0;
	}
#line 2376 "asn1p_y.c"
    break;

  case 12: /* ObjectIdentifierBody: ObjectIdentifierElement  */
#line 491 "asn1p_y.y"
                                {
		(yyval.a_oid) = asn1p_oid_new();
		asn1p_oid_add_arc((yyval.a_oid), &(yyvsp[0].a_oid_arc));
		if((yyvsp[0].a_oid_arc).name)
			free((yyvsp[0].a_oid_arc).name);
	}
#line 2387 "asn1p_y.c"
    break;

  case 13: /* ObjectIdentifierBody: ObjectIdentifierBody ObjectIdentifierElement  */
#line 497 "asn1p_y.y"
                                                       {
		(yyval.a_oid) = (yyvsp[-1].a_oid);
		asn1p_oid_add_arc((yyval.a_oid), &(yyvsp[0].a_oid_arc));
		if((yyvsp[0].a_oid_arc).name)
			free((yyvsp[0].a_oid_arc).name);
	}
#line 2398 "asn1p_y.c"
    break;

  case 14: /* ObjectIdentifierElement: Identifier  */
#line 506 "asn1p_y.y"
                   {					/* iso */
		(yyval.a_oid_arc).name = (yyvsp[0].tv_str);
		(yyval.a_oid_arc).number = -1;
	}
#line 2407 "asn1p_y.c"
    break;

  case 15: /* ObjectIdentifierElement: Identifier '(' "number" ')'  */
#line 510 "asn1p_y.y"
                                        {		/* iso(1) */
		(yyval.a_oid_arc).name = (yyvsp[-3].tv_str);
		(yyval.a_oid_arc).number = (yyvsp[-1].a_int);
	}
#line 2416 "asn1p_y.c"
    break;

  case 16: /* ObjectIdentifierElement: "number"  */
#line 514 "asn1p_y.y"
                     {					/* 1 */
		(yyval.a_oid_arc).name = 0;
		(yyval.a_oid_arc).number = (yyvsp[0].a_int);
	}
#line 2425 "asn1p_y.c"
    break;

  case 17: /* optModuleDefinitionFlags: %empty  */
#line 524 "asn1p_y.y"
        { (yyval.a_module_flags) = MSF_NOFLAGS; }
#line 2431 "asn1p_y.c"
    break;

  case 18: /* optModuleDefinitionFlags: ModuleDefinitionFlags  */
#line 525 "asn1p_y.y"
                                {
		(yyval.a_module_flags) = (yyvsp[0].a_module_flags);
	}
#line 2439 "asn1p_y.c"
    break;

  case 19: /* ModuleDefinitionFlags: ModuleDefinitionFlag  */
#line 534 "asn1p_y.y"
                             {
		(yyval.a_module_flags) = (yyvsp[0].a_module_flags);
	}
#line 2447 "asn1p_y.c"
    break;

  case 20: /* ModuleDefinitionFlags: ModuleDefinitionFlags ModuleDefinitionFlag  */
#line 537 "asn1p_y.y"
                                                     {
		(yyval.a_module_flags) = (yyvsp[-1].a_module_flags) | (yyvsp[0].a_module_flags);
	}
#line 2455 "asn1p_y.c"
    break;

  case 21: /* ModuleDefinitionFlag: TOK_EXPLICIT TOK_TAGS  */
#line 546 "asn1p_y.y"
                              {
		(yyval.a_module_flags) = MSF_EXPLICIT_TAGS;
	}
#line 2463 "asn1p_y.c"
    break;

  case 22: /* ModuleDefinitionFlag: TOK_IMPLICIT TOK_TAGS  */
#line 549 "asn1p_y.y"
                                {
		(yyval.a_module_flags) = MSF_IMPLICIT_TAGS;
	}
#line 2471 "asn1p_y.c"
    break;

  case 23: /* ModuleDefinitionFlag: TOK_AUTOMATIC TOK_TAGS  */
#line 552 "asn1p_y.y"
                                 {
		(yyval.a_module_flags) = MSF_AUTOMATIC_TAGS;
	}
#line 2479 "asn1p_y.c"
    break;

  case 24: /* ModuleDefinitionFlag: TOK_EXTENSIBILITY TOK_IMPLIED  */
#line 555 "asn1p_y.y"
                                        {
		(yyval.a_module_flags) = MSF_EXTENSIBILITY_IMPLIED;
	}
#line 2487 "asn1p_y.c"
    break;

  case 25: /* ModuleDefinitionFlag: TOK_capitalreference TOK_INSTRUCTIONS  */
#line 559 "asn1p_y.y"
                                                {
		/* X.680Amd1 specifies TAG and XER */
		if(strcmp((yyvsp[-1].tv_str), "TAG") == 0) {
		 	(yyval.a_module_flags) = MSF_TAG_INSTRUCTIONS;
		} else if(strcmp((yyvsp[-1].tv_str), "XER") == 0) {
		 	(yyval.a_module_flags) = MSF_XER_INSTRUCTIONS;
		} else {
			fprintf(stderr,
				"WARNING: %s INSTRUCTIONS at %s:%d: "
				"Unrecognized encoding reference\n",
				(yyvsp[-1].tv_str), ASN_FILENAME, yylineno);
		 	(yyval.a_module_flags) = MSF_unk_INSTRUCTIONS;
		}
		free((yyvsp[-1].tv_str));
	}
#line 2507 "asn1p_y.c"
    break;

  case 26: /* optModuleBody: %empty  */
#line 580 "asn1p_y.y"
        { (yyval.a_module) = 0; }
#line 2513 "asn1p_y.c"
    break;

  case 27: /* optModuleBody: ModuleBody  */
#line 581 "asn1p_y.y"
                     {
		(yyval.a_module) = (yyvsp[0].a_module);
	}
#line 2521 "asn1p_y.c"
    break;

  case 28: /* ModuleBody: optExports optImports AssignmentList  */
#line 590 "asn1p_y.y"
                                             {
		(yyval.a_module) = asn1p_module_new();
		AL_IMPORT((yyval.a_module), exports, (yyvsp[-2].a_module), xp_next);
		AL_IMPORT((yyval.a_module), imports, (yyvsp[-1].a_module), xp_next);
		asn1p_module_move_members((yyval.a_module), (yyvsp[0].a_module));

		asn1p_module_free((yyvsp[-2].a_module));
		asn1p_module_free((yyvsp[-1].a_module));
		asn1p_module_free((yyvsp[0].a_module));
	}
#line 2536 "asn1p_y.c"
    break;

  case 29: /* AssignmentList: Assignment  */
#line 603 "asn1p_y.y"
                   {
		(yyval.a_module) = (yyvsp[0].a_module);
	}
#line 2544 "asn1p_y.c"
    break;

  case 30: /* AssignmentList: AssignmentList Assignment  */
#line 606 "asn1p_y.y"
                                    {
		if((yyvsp[-1].a_module)) {
			(yyval.a_module) = (yyvsp[-1].a_module);
		} else {
			(yyval.a_module) = (yyvsp[0].a_module);
			break;
		}
        asn1p_module_move_members((yyval.a_module), (yyvsp[0].a_module));
		asn1p_module_free((yyvsp[0].a_module));
	}
#line 2559 "asn1p_y.c"
    break;

  case 31: /* Assignment: DataTypeReference  */
#line 623 "asn1p_y.y"
                          {
		(yyval.a_module) = asn1p_module_new();
		checkmem((yyval.a_module));
		assert((yyvsp[0].a_expr)->expr_type != A1TC_INVALID);
		assert((yyvsp[0].a_expr)->meta_type != AMT_INVALID);
		asn1p_module_member_add((yyval.a_module), (yyvsp[0].a_expr));
	}
#line 2571 "asn1p_y.c"
    break;

  case 32: /* Assignment: ValueAssignment  */
#line 630 "asn1p_y.y"
                          {
		(yyval.a_module) = asn1p_module_new();
		checkmem((yyval.a_module));
		assert((yyvsp[0].a_expr)->expr_type != A1TC_INVALID);
		assert((yyvsp[0].a_expr)->meta_type != AMT_INVALID);
		asn1p_module_member_add((yyval.a_module), (yyvsp[0].a_expr));
	}
#line 2583 "asn1p_y.c"
    break;

  case 33: /* Assignment: ValueSetTypeAssignment  */
#line 644 "asn1p_y.y"
                                 {
		(yyval.a_module) = asn1p_module_new();
		checkmem((yyval.a_module));
		assert((yyvsp[0].a_expr)->expr_type != A1TC_INVALID);
		assert((yyvsp[0].a_expr)->meta_type != AMT_INVALID);
		asn1p_module_member_add((yyval.a_module), (yyvsp[0].a_expr));
	}
#line 2595 "asn1p_y.c"
    break;

  case 34: /* $@2: %empty  */
#line 652 "asn1p_y.y"
                { asn1p_lexer_hack_push_encoding_control(); }
#line 2601 "asn1p_y.c"
    break;

  case 35: /* Assignment: TOK_ENCODING_CONTROL TOK_capitalreference $@2  */
#line 653 "asn1p_y.y"
                        {
		fprintf(stderr,
			"WARNING: ENCODING-CONTROL %s "
			"specification at %s:%d ignored\n",
			(yyvsp[-1].tv_str), ASN_FILENAME, yylineno);
		free((yyvsp[-1].tv_str));
		(yyval.a_module) = 0;
	}
#line 2614 "asn1p_y.c"
    break;

  case 36: /* Assignment: BasicString  */
#line 665 "asn1p_y.y"
                      {
		return yyerror(param,
			"Attempt to redefine a standard basic string type, "
			"please comment out or remove this type redefinition.");
	}
#line 2624 "asn1p_y.c"
    break;

  case 37: /* optImports: %empty  */
#line 678 "asn1p_y.y"
        { (yyval.a_module) = 0; }
#line 2630 "asn1p_y.c"
    break;

  case 39: /* ImportsDefinition: TOK_IMPORTS optImportsBundleSet ';'  */
#line 682 "asn1p_y.y"
                                            {
		if(!saved_aid && 0)
			return yyerror(param, "Unterminated IMPORTS FROM, "
					"expected semicolon ';'");
		saved_aid = 0;
		(yyval.a_module) = (yyvsp[-1].a_module);
	}
#line 2642 "asn1p_y.c"
    break;

  case 40: /* ImportsDefinition: TOK_IMPORTS TOK_FROM  */
#line 692 "asn1p_y.y"
                                         {
		return yyerror(param, "Empty IMPORTS list");
	}
#line 2650 "asn1p_y.c"
    break;

  case 41: /* optImportsBundleSet: %empty  */
#line 698 "asn1p_y.y"
        { (yyval.a_module) = asn1p_module_new(); }
#line 2656 "asn1p_y.c"
    break;

  case 43: /* ImportsBundleSet: ImportsBundle  */
#line 702 "asn1p_y.y"
                      {
		(yyval.a_module) = asn1p_module_new();
		checkmem((yyval.a_module));
		TQ_ADD(&((yyval.a_module)->imports), (yyvsp[0].a_xports), xp_next);
	}
#line 2666 "asn1p_y.c"
    break;

  case 44: /* ImportsBundleSet: ImportsBundleSet ImportsBundle  */
#line 707 "asn1p_y.y"
                                         {
		(yyval.a_module) = (yyvsp[-1].a_module);
		TQ_ADD(&((yyval.a_module)->imports), (yyvsp[0].a_xports), xp_next);
	}
#line 2675 "asn1p_y.c"
    break;

  case 45: /* AssignedIdentifier: %empty  */
#line 714 "asn1p_y.y"
        { memset(&(yyval.a_aid), 0, sizeof((yyval.a_aid))); }
#line 2681 "asn1p_y.c"
    break;

  case 46: /* AssignedIdentifier: ObjectIdentifier  */
#line 715 "asn1p_y.y"
                           { (yyval.a_aid).oid = (yyvsp[0].a_oid); }
#line 2687 "asn1p_y.c"
    break;

  case 47: /* ImportsBundle: ImportsBundleInt ImportSelectionOption  */
#line 719 "asn1p_y.y"
                                           {
      (yyval.a_xports) = (yyvsp[-1].a_xports);
      (yyval.a_xports)->option = (yyvsp[0].a_int);
    }
#line 2696 "asn1p_y.c"
    break;

  case 49: /* ImportsBundleInt: ImportsList TOK_FROM TypeRefName AssignedIdentifier  */
#line 726 "asn1p_y.y"
                                                            {
		(yyval.a_xports) = (yyvsp[-3].a_xports);
		(yyval.a_xports)->fromModuleName = (yyvsp[-1].tv_str);
		(yyval.a_xports)->identifier = (yyvsp[0].a_aid);
		(yyval.a_xports)->option = 0;
		/* This stupid thing is used for look-back hack. */
		saved_aid = (yyval.a_xports)->identifier.oid ? 0 : &((yyval.a_xports)->identifier);
		checkmem((yyval.a_xports));
	}
#line 2710 "asn1p_y.c"
    break;

  case 50: /* ImportsList: ImportsElement  */
#line 738 "asn1p_y.y"
                       {
		(yyval.a_xports) = asn1p_xports_new();
		checkmem((yyval.a_xports));
		TQ_ADD(&((yyval.a_xports)->xp_members), (yyvsp[0].a_expr), next);
	}
#line 2720 "asn1p_y.c"
    break;

  case 51: /* ImportsList: ImportsList ',' ImportsElement  */
#line 743 "asn1p_y.y"
                                         {
		(yyval.a_xports) = (yyvsp[-2].a_xports);
		TQ_ADD(&((yyval.a_xports)->xp_members), (yyvsp[0].a_expr), next);
	}
#line 2729 "asn1p_y.c"
    break;

  case 52: /* ImportsElement: TypeRefName  */
#line 750 "asn1p_y.y"
                    {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->Identifier = (yyvsp[0].tv_str);
		(yyval.a_expr)->expr_type = A1TC_REFERENCE;
	}
#line 2740 "asn1p_y.c"
    break;

  case 53: /* ImportsElement: TypeRefName '{' '}'  */
#line 756 "asn1p_y.y"
                              {		/* Completely equivalent to above */
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->Identifier = (yyvsp[-2].tv_str);
		(yyval.a_expr)->expr_type = A1TC_REFERENCE;
	}
#line 2751 "asn1p_y.c"
    break;

  case 54: /* ImportsElement: Identifier  */
#line 762 "asn1p_y.y"
                     {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->Identifier = (yyvsp[0].tv_str);
		(yyval.a_expr)->expr_type = A1TC_REFERENCE;
	}
#line 2762 "asn1p_y.c"
    break;

  case 55: /* ImportSelectionOption: TOK_WITH TOK_SUCCESSORS  */
#line 771 "asn1p_y.y"
                                {
		(yyval.a_int) = XPT_WITH_SUCCESSORS;
	}
#line 2770 "asn1p_y.c"
    break;

  case 56: /* ImportSelectionOption: TOK_WITH TOK_DESCENDANTS  */
#line 774 "asn1p_y.y"
                                   {
		(yyval.a_int) = XPT_WITH_DESCENDANTS;
	}
#line 2778 "asn1p_y.c"
    break;

  case 57: /* optExports: %empty  */
#line 780 "asn1p_y.y"
        { (yyval.a_module) = 0; }
#line 2784 "asn1p_y.c"
    break;

  case 58: /* optExports: ExportsDefinition  */
#line 781 "asn1p_y.y"
                            {
		(yyval.a_module) = asn1p_module_new();
		checkmem((yyval.a_module));
		if((yyvsp[0].a_xports)) {
			TQ_ADD(&((yyval.a_module)->exports), (yyvsp[0].a_xports), xp_next);
		} else {
			/* "EXPORTS ALL;" */
		}
	}
#line 2798 "asn1p_y.c"
    break;

  case 59: /* ExportsDefinition: TOK_EXPORTS ExportsBody ';'  */
#line 793 "asn1p_y.y"
                                    {
		(yyval.a_xports) = (yyvsp[-1].a_xports);
	}
#line 2806 "asn1p_y.c"
    break;

  case 60: /* ExportsDefinition: TOK_EXPORTS TOK_ALL ';'  */
#line 796 "asn1p_y.y"
                                  {
		(yyval.a_xports) = 0;
	}
#line 2814 "asn1p_y.c"
    break;

  case 61: /* ExportsDefinition: TOK_EXPORTS ';'  */
#line 799 "asn1p_y.y"
                          {
		/* Empty EXPORTS clause effectively prohibits export. */
		(yyval.a_xports) = asn1p_xports_new();
		checkmem((yyval.a_xports));
	}
#line 2824 "asn1p_y.c"
    break;

  case 62: /* ExportsBody: ExportsElement  */
#line 807 "asn1p_y.y"
                       {
		(yyval.a_xports) = asn1p_xports_new();
		assert((yyval.a_xports));
		TQ_ADD(&((yyval.a_xports)->xp_members), (yyvsp[0].a_expr), next);
	}
#line 2834 "asn1p_y.c"
    break;

  case 63: /* ExportsBody: ExportsBody ',' ExportsElement  */
#line 812 "asn1p_y.y"
                                         {
		(yyval.a_xports) = (yyvsp[-2].a_xports);
		TQ_ADD(&((yyval.a_xports)->xp_members), (yyvsp[0].a_expr), next);
	}
#line 2843 "asn1p_y.c"
    break;

  case 64: /* ExportsElement: TypeRefName  */
#line 819 "asn1p_y.y"
                    {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->Identifier = (yyvsp[0].tv_str);
		(yyval.a_expr)->expr_type = A1TC_EXPORTVAR;
	}
#line 2854 "asn1p_y.c"
    break;

  case 65: /* ExportsElement: TypeRefName '{' '}'  */
#line 825 "asn1p_y.y"
                              {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->Identifier = (yyvsp[-2].tv_str);
		(yyval.a_expr)->expr_type = A1TC_EXPORTVAR;
	}
#line 2865 "asn1p_y.c"
    break;

  case 66: /* ExportsElement: Identifier  */
#line 831 "asn1p_y.y"
                     {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->Identifier = (yyvsp[0].tv_str);
		(yyval.a_expr)->expr_type = A1TC_EXPORTVAR;
	}
#line 2876 "asn1p_y.c"
    break;

  case 67: /* ValueSet: '{' ElementSetSpecs '}'  */
#line 840 "asn1p_y.y"
                                  { (yyval.a_constr) = (yyvsp[-1].a_constr); }
#line 2882 "asn1p_y.c"
    break;

  case 68: /* ValueSetTypeAssignment: TypeRefName Type TOK_PPEQ ValueSet  */
#line 843 "asn1p_y.y"
                                           {
		(yyval.a_expr) = (yyvsp[-2].a_expr);
		assert((yyval.a_expr)->Identifier == 0);
		(yyval.a_expr)->Identifier = (yyvsp[-3].tv_str);
		(yyval.a_expr)->meta_type = AMT_VALUESET;
		(yyval.a_expr)->constraints = (yyvsp[0].a_constr);
	}
#line 2894 "asn1p_y.c"
    break;

  case 69: /* DefinedType: ComplexTypeReference  */
#line 863 "asn1p_y.y"
                             {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->reference = (yyvsp[0].a_ref);
		(yyval.a_expr)->expr_type = A1TC_REFERENCE;
		(yyval.a_expr)->meta_type = AMT_TYPEREF;
	}
#line 2906 "asn1p_y.c"
    break;

  case 70: /* DefinedType: ComplexTypeReference '{' ActualParameterList '}'  */
#line 873 "asn1p_y.y"
                                                           {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->reference = (yyvsp[-3].a_ref);
		(yyval.a_expr)->rhs_pspecs = (yyvsp[-1].a_expr);
		(yyval.a_expr)->expr_type = A1TC_REFERENCE;
		(yyval.a_expr)->meta_type = AMT_TYPEREF;
	}
#line 2919 "asn1p_y.c"
    break;

  case 71: /* DataTypeReference: TypeRefName TOK_PPEQ Type  */
#line 893 "asn1p_y.y"
                                  {
		(yyval.a_expr) = (yyvsp[0].a_expr);
		(yyval.a_expr)->Identifier = (yyvsp[-2].tv_str);
		assert((yyval.a_expr)->expr_type);
		assert((yyval.a_expr)->meta_type);
	}
#line 2930 "asn1p_y.c"
    break;

  case 72: /* DataTypeReference: TypeRefName TOK_PPEQ ObjectClass  */
#line 899 "asn1p_y.y"
                                           {
		(yyval.a_expr) = (yyvsp[0].a_expr);
		(yyval.a_expr)->Identifier = (yyvsp[-2].tv_str);
		assert((yyval.a_expr)->expr_type == A1TC_CLASSDEF);
		assert((yyval.a_expr)->meta_type == AMT_OBJECTCLASS);
	}
#line 2941 "asn1p_y.c"
    break;

  case 73: /* DataTypeReference: TypeRefName '{' ParameterArgumentList '}' TOK_PPEQ Type  */
#line 915 "asn1p_y.y"
                                                                  {
		(yyval.a_expr) = (yyvsp[0].a_expr);
		(yyval.a_expr)->Identifier = (yyvsp[-5].tv_str);
		(yyval.a_expr)->lhs_params = (yyvsp[-3].a_plist);
	}
#line 2951 "asn1p_y.c"
    break;

  case 74: /* DataTypeReference: TypeRefName '{' ParameterArgumentList '}' TOK_PPEQ ObjectClass  */
#line 921 "asn1p_y.y"
                                                                         {
		(yyval.a_expr) = (yyvsp[0].a_expr);
		(yyval.a_expr)->Identifier = (yyvsp[-5].tv_str);
		(yyval.a_expr)->lhs_params = (yyvsp[-3].a_plist);
	}
#line 2961 "asn1p_y.c"
    break;

  case 75: /* ParameterArgumentList: ParameterArgumentName  */
#line 929 "asn1p_y.y"
                              {
		int ret;
		(yyval.a_plist) = asn1p_paramlist_new(yylineno);
		checkmem((yyval.a_plist));
		ret = asn1p_paramlist_add_param((yyval.a_plist), (yyvsp[0].a_parg).governor, (yyvsp[0].a_parg).argument);
		checkmem(ret == 0);
		asn1p_ref_free((yyvsp[0].a_parg).governor);
		free((yyvsp[0].a_parg).argument);
	}
#line 2975 "asn1p_y.c"
    break;

  case 76: /* ParameterArgumentList: ParameterArgumentList ',' ParameterArgumentName  */
#line 938 "asn1p_y.y"
                                                          {
		int ret;
		(yyval.a_plist) = (yyvsp[-2].a_plist);
		ret = asn1p_paramlist_add_param((yyval.a_plist), (yyvsp[0].a_parg).governor, (yyvsp[0].a_parg).argument);
		checkmem(ret == 0);
		asn1p_ref_free((yyvsp[0].a_parg).governor);
		free((yyvsp[0].a_parg).argument);
	}
#line 2988 "asn1p_y.c"
    break;

  case 77: /* ParameterArgumentName: TypeRefName  */
#line 949 "asn1p_y.y"
                    {
		(yyval.a_parg).governor = NULL;
		(yyval.a_parg).argument = (yyvsp[0].tv_str);
	}
#line 2997 "asn1p_y.c"
    break;

  case 78: /* ParameterArgumentName: TypeRefName ':' Identifier  */
#line 953 "asn1p_y.y"
                                     {
		int ret;
		(yyval.a_parg).governor = asn1p_ref_new(yylineno, currentModule);
		ret = asn1p_ref_add_component((yyval.a_parg).governor, (yyvsp[-2].tv_str), 0);
		checkmem(ret == 0);
		(yyval.a_parg).argument = (yyvsp[0].tv_str);
		free((yyvsp[-2].tv_str));
	}
#line 3010 "asn1p_y.c"
    break;

  case 79: /* ParameterArgumentName: TypeRefName ':' TypeRefName  */
#line 961 "asn1p_y.y"
                                      {
		int ret;
		(yyval.a_parg).governor = asn1p_ref_new(yylineno, currentModule);
		ret = asn1p_ref_add_component((yyval.a_parg).governor, (yyvsp[-2].tv_str), 0);
		checkmem(ret == 0);
		(yyval.a_parg).argument = (yyvsp[0].tv_str);
		free((yyvsp[-2].tv_str));
	}
#line 3023 "asn1p_y.c"
    break;

  case 80: /* ParameterArgumentName: BasicTypeId ':' Identifier  */
#line 969 "asn1p_y.y"
                                     {
		int ret;
		(yyval.a_parg).governor = asn1p_ref_new(yylineno, currentModule);
		ret = asn1p_ref_add_component((yyval.a_parg).governor,
			ASN_EXPR_TYPE2STR((yyvsp[-2].a_type)), 1);
		checkmem(ret == 0);
		(yyval.a_parg).argument = (yyvsp[0].tv_str);
	}
#line 3036 "asn1p_y.c"
    break;

  case 81: /* ParameterArgumentName: BasicTypeId ':' TypeRefName  */
#line 977 "asn1p_y.y"
                                      {
		int ret;
		(yyval.a_parg).governor = asn1p_ref_new(yylineno, currentModule);
		ret = asn1p_ref_add_component((yyval.a_parg).governor,
			ASN_EXPR_TYPE2STR((yyvsp[-2].a_type)), 1);
		checkmem(ret == 0);
		(yyval.a_parg).argument = (yyvsp[0].tv_str);
	}
#line 3049 "asn1p_y.c"
    break;

  case 82: /* ActualParameterList: ActualParameter  */
#line 988 "asn1p_y.y"
                        {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		asn1p_expr_add((yyval.a_expr), (yyvsp[0].a_expr));
	}
#line 3059 "asn1p_y.c"
    break;

  case 83: /* ActualParameterList: ActualParameterList ',' ActualParameter  */
#line 993 "asn1p_y.y"
                                                  {
		(yyval.a_expr) = (yyvsp[-2].a_expr);
		asn1p_expr_add((yyval.a_expr), (yyvsp[0].a_expr));
	}
#line 3068 "asn1p_y.c"
    break;

  case 85: /* ActualParameter: SimpleValue  */
#line 1001 "asn1p_y.y"
                      {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->Identifier = strdup("?");
		(yyval.a_expr)->expr_type = A1TC_REFERENCE;
		(yyval.a_expr)->meta_type = AMT_VALUE;
		(yyval.a_expr)->value = (yyvsp[0].a_value);
	}
#line 3081 "asn1p_y.c"
    break;

  case 86: /* ActualParameter: DefinedValue  */
#line 1009 "asn1p_y.y"
                       {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->Identifier = strdup("?");
		(yyval.a_expr)->expr_type = A1TC_REFERENCE;
		(yyval.a_expr)->meta_type = AMT_VALUE;
		(yyval.a_expr)->value = (yyvsp[0].a_value);
	}
#line 3094 "asn1p_y.c"
    break;

  case 87: /* ActualParameter: ValueSet  */
#line 1017 "asn1p_y.y"
                   {
		(yyval.a_expr) = NEW_EXPR();
		(yyval.a_expr)->expr_type = A1TC_VALUESET;
		(yyval.a_expr)->meta_type = AMT_VALUESET;
		(yyval.a_expr)->constraints = (yyvsp[0].a_constr);
	}
#line 3105 "asn1p_y.c"
    break;

  case 88: /* optComponentTypeLists: %empty  */
#line 1040 "asn1p_y.y"
        { (yyval.a_expr) = NEW_EXPR(); }
#line 3111 "asn1p_y.c"
    break;

  case 89: /* optComponentTypeLists: ComponentTypeLists  */
#line 1041 "asn1p_y.y"
                             { (yyval.a_expr) = (yyvsp[0].a_expr); }
#line 3117 "asn1p_y.c"
    break;

  case 90: /* ComponentTypeLists: ComponentType  */
#line 1044 "asn1p_y.y"
                      {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		asn1p_expr_add((yyval.a_expr), (yyvsp[0].a_expr));
	}
#line 3127 "asn1p_y.c"
    break;

  case 91: /* ComponentTypeLists: ComponentTypeLists ',' ComponentType  */
#line 1049 "asn1p_y.y"
                                               {
		(yyval.a_expr) = (yyvsp[-2].a_expr);
		asn1p_expr_add((yyval.a_expr), (yyvsp[0].a_expr));
	}
#line 3136 "asn1p_y.c"
    break;

  case 92: /* ComponentTypeLists: ComponentTypeLists ',' TOK_VBracketLeft ComponentTypeLists TOK_VBracketRight  */
#line 1053 "asn1p_y.y"
                                                                                       {
		(yyval.a_expr) = (yyvsp[-4].a_expr);
		(yyvsp[-1].a_expr)->meta_type = AMT_TYPE;
		(yyvsp[-1].a_expr)->expr_type = ASN_CONSTR_SEQUENCE;
		(yyvsp[-1].a_expr)->marker.flags |= EM_OPTIONAL;
		asn1p_expr_add((yyval.a_expr), (yyvsp[-1].a_expr));
	}
#line 3148 "asn1p_y.c"
    break;

  case 93: /* ComponentType: Identifier MaybeIndirectTaggedType optMarker  */
#line 1063 "asn1p_y.y"
                                                     {
		(yyval.a_expr) = (yyvsp[-1].a_expr);
		assert((yyval.a_expr)->Identifier == 0);
		(yyval.a_expr)->Identifier = (yyvsp[-2].tv_str);
		(yyvsp[0].a_marker).flags |= (yyval.a_expr)->marker.flags;
		(yyval.a_expr)->marker = (yyvsp[0].a_marker);
	}
#line 3160 "asn1p_y.c"
    break;

  case 94: /* ComponentType: MaybeIndirectTaggedType optMarker  */
#line 1070 "asn1p_y.y"
                                            {
		(yyval.a_expr) = (yyvsp[-1].a_expr);
		(yyvsp[0].a_marker).flags |= (yyval.a_expr)->marker.flags;
		(yyval.a_expr)->marker = (yyvsp[0].a_marker);
		_fixup_anonymous_identifier((yyval.a_expr));
	}
#line 3171 "asn1p_y.c"
    break;

  case 95: /* ComponentType: TOK_COMPONENTS TOK_OF MaybeIndirectTaggedType  */
#line 1076 "asn1p_y.y"
                                                        {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->meta_type = (yyvsp[0].a_expr)->meta_type;
		(yyval.a_expr)->expr_type = A1TC_COMPONENTS_OF;
		asn1p_expr_add((yyval.a_expr), (yyvsp[0].a_expr));
	}
#line 3183 "asn1p_y.c"
    break;

  case 96: /* ComponentType: ExtensionAndException  */
#line 1083 "asn1p_y.y"
                                {
		(yyval.a_expr) = (yyvsp[0].a_expr);
	}
#line 3191 "asn1p_y.c"
    break;

  case 97: /* AlternativeTypeLists: AlternativeType  */
#line 1089 "asn1p_y.y"
                        {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		asn1p_expr_add((yyval.a_expr), (yyvsp[0].a_expr));
	}
#line 3201 "asn1p_y.c"
    break;

  case 98: /* AlternativeTypeLists: AlternativeTypeLists ',' AlternativeType  */
#line 1094 "asn1p_y.y"
                                                   {
		(yyval.a_expr) = (yyvsp[-2].a_expr);
		asn1p_expr_add((yyval.a_expr), (yyvsp[0].a_expr));
	}
#line 3210 "asn1p_y.c"
    break;

  case 99: /* AlternativeTypeLists: AlternativeTypeLists ',' TOK_VBracketLeft AlternativeTypeLists TOK_VBracketRight  */
#line 1098 "asn1p_y.y"
                                                                                           {
		(yyval.a_expr) = (yyvsp[-4].a_expr);
		asn1p_expr_add_many((yyval.a_expr), (yyvsp[-1].a_expr));
	}
#line 3219 "asn1p_y.c"
    break;

  case 100: /* AlternativeType: Identifier MaybeIndirectTaggedType  */
#line 1105 "asn1p_y.y"
                                           {
		(yyval.a_expr) = (yyvsp[0].a_expr);
		assert((yyval.a_expr)->Identifier == 0);
		(yyval.a_expr)->Identifier = (yyvsp[-1].tv_str);
	}
#line 3229 "asn1p_y.c"
    break;

  case 101: /* AlternativeType: ExtensionAndException  */
#line 1110 "asn1p_y.y"
                                {
		(yyval.a_expr) = (yyvsp[0].a_expr);
	}
#line 3237 "asn1p_y.c"
    break;

  case 102: /* AlternativeType: MaybeIndirectTaggedType  */
#line 1113 "asn1p_y.y"
                                  {
		(yyval.a_expr) = (yyvsp[0].a_expr);
		_fixup_anonymous_identifier((yyval.a_expr));
	}
#line 3246 "asn1p_y.c"
    break;

  case 103: /* ObjectClass: TOK_CLASS '{' FieldSpec '}' optWithSyntax  */
#line 1120 "asn1p_y.y"
                                                  {
		(yyval.a_expr) = (yyvsp[-2].a_expr);
		checkmem((yyval.a_expr));
		(yyval.a_expr)->with_syntax = (yyvsp[0].a_wsynt);
		assert((yyval.a_expr)->expr_type == A1TC_CLASSDEF);
		assert((yyval.a_expr)->meta_type == AMT_OBJECTCLASS);
	}
#line 3258 "asn1p_y.c"
    break;

  case 104: /* optUNIQUE: %empty  */
#line 1130 "asn1p_y.y"
        { (yyval.a_int) = 0; }
#line 3264 "asn1p_y.c"
    break;

  case 105: /* optUNIQUE: TOK_UNIQUE  */
#line 1131 "asn1p_y.y"
                     { (yyval.a_int) = 1; }
#line 3270 "asn1p_y.c"
    break;

  case 106: /* FieldSpec: ClassField  */
#line 1135 "asn1p_y.y"
                   {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->expr_type = A1TC_CLASSDEF;
		(yyval.a_expr)->meta_type = AMT_OBJECTCLASS;
		asn1p_expr_add((yyval.a_expr), (yyvsp[0].a_expr));
	}
#line 3282 "asn1p_y.c"
    break;

  case 107: /* FieldSpec: FieldSpec ',' ClassField  */
#line 1142 "asn1p_y.y"
                                   {
		(yyval.a_expr) = (yyvsp[-2].a_expr);
		asn1p_expr_add((yyval.a_expr), (yyvsp[0].a_expr));
	}
#line 3291 "asn1p_y.c"
    break;

  case 108: /* ClassField: TOK_typefieldreference optMarker  */
#line 1152 "asn1p_y.y"
                                         {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->Identifier = (yyvsp[-1].tv_str);
		(yyval.a_expr)->meta_type = AMT_OBJECTFIELD;
		(yyval.a_expr)->expr_type = A1TC_CLASSFIELD_TFS;	/* TypeFieldSpec */
		(yyval.a_expr)->marker = (yyvsp[0].a_marker);
	}
#line 3304 "asn1p_y.c"
    break;

  case 109: /* ClassField: TOK_valuefieldreference Type optUNIQUE optMarker  */
#line 1162 "asn1p_y.y"
                                                           {
		(yyval.a_expr) = NEW_EXPR();
		(yyval.a_expr)->Identifier = (yyvsp[-3].tv_str);
		(yyval.a_expr)->meta_type = AMT_OBJECTFIELD;
		(yyval.a_expr)->expr_type = A1TC_CLASSFIELD_FTVFS;	/* FixedTypeValueFieldSpec */
		(yyval.a_expr)->unique = (yyvsp[-1].a_int);
		(yyval.a_expr)->marker = (yyvsp[0].a_marker);
		asn1p_expr_add((yyval.a_expr), (yyvsp[-2].a_expr));
	}
#line 3318 "asn1p_y.c"
    break;

  case 110: /* ClassField: TOK_valuefieldreference FieldName optMarker  */
#line 1173 "asn1p_y.y"
                                                      {
		(yyval.a_expr) = NEW_EXPR();
		(yyval.a_expr)->Identifier = (yyvsp[-2].tv_str);
		(yyval.a_expr)->meta_type = AMT_OBJECTFIELD;
		(yyval.a_expr)->expr_type = A1TC_CLASSFIELD_VTVFS;
		(yyval.a_expr)->reference = (yyvsp[-1].a_ref);
		(yyval.a_expr)->marker = (yyvsp[0].a_marker);
	}
#line 3331 "asn1p_y.c"
    break;

  case 111: /* ClassField: TOK_valuefieldreference DefinedObjectClass optMarker  */
#line 1183 "asn1p_y.y"
                                                               {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->Identifier = (yyvsp[-2].tv_str);
		(yyval.a_expr)->reference = (yyvsp[-1].a_ref);
		(yyval.a_expr)->meta_type = AMT_OBJECTFIELD;
		(yyval.a_expr)->expr_type = A1TC_CLASSFIELD_OFS;
		(yyval.a_expr)->marker = (yyvsp[0].a_marker);
	}
#line 3345 "asn1p_y.c"
    break;

  case 112: /* ClassField: TOK_typefieldreference FieldName optMarker  */
#line 1194 "asn1p_y.y"
                                                     {
		(yyval.a_expr) = NEW_EXPR();
		(yyval.a_expr)->Identifier = (yyvsp[-2].tv_str);
		(yyval.a_expr)->meta_type = AMT_OBJECTFIELD;
		(yyval.a_expr)->expr_type = A1TC_CLASSFIELD_VTVSFS;
		(yyval.a_expr)->reference = (yyvsp[-1].a_ref);
		(yyval.a_expr)->marker = (yyvsp[0].a_marker);
	}
#line 3358 "asn1p_y.c"
    break;

  case 113: /* ClassField: TOK_typefieldreference Type optMarker  */
#line 1204 "asn1p_y.y"
                                                {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->Identifier = (yyvsp[-2].tv_str);
		(yyval.a_expr)->meta_type = AMT_OBJECTFIELD;
		(yyval.a_expr)->expr_type = A1TC_CLASSFIELD_FTVSFS;
		asn1p_expr_add((yyval.a_expr), (yyvsp[-1].a_expr));
		(yyval.a_expr)->marker = (yyvsp[0].a_marker);
	}
#line 3372 "asn1p_y.c"
    break;

  case 114: /* ClassField: TOK_typefieldreference DefinedObjectClass optMarker  */
#line 1215 "asn1p_y.y"
                                                              {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->Identifier = (yyvsp[-2].tv_str);
		(yyval.a_expr)->reference = (yyvsp[-1].a_ref);
		(yyval.a_expr)->meta_type = AMT_OBJECTFIELD;
		(yyval.a_expr)->expr_type = A1TC_CLASSFIELD_OSFS;
		(yyval.a_expr)->marker = (yyvsp[0].a_marker);
	}
#line 3386 "asn1p_y.c"
    break;

  case 115: /* optWithSyntax: %empty  */
#line 1227 "asn1p_y.y"
        { (yyval.a_wsynt) = 0; }
#line 3392 "asn1p_y.c"
    break;

  case 116: /* optWithSyntax: WithSyntax  */
#line 1228 "asn1p_y.y"
                     {
		(yyval.a_wsynt) = (yyvsp[0].a_wsynt);
	}
#line 3400 "asn1p_y.c"
    break;

  case 117: /* $@3: %empty  */
#line 1235 "asn1p_y.y"
                { asn1p_lexer_hack_enable_with_syntax(); }
#line 3406 "asn1p_y.c"
    break;

  case 118: /* WithSyntax: TOK_WITH TOK_SYNTAX '{' $@3 WithSyntaxList '}'  */
#line 1237 "asn1p_y.y"
                    {
		(yyval.a_wsynt) = (yyvsp[-1].a_wsynt);
	}
#line 3414 "asn1p_y.c"
    break;

  case 119: /* WithSyntaxList: WithSyntaxToken  */
#line 1243 "asn1p_y.y"
                        {
		(yyval.a_wsynt) = asn1p_wsyntx_new();
		TQ_ADD(&((yyval.a_wsynt)->chunks), (yyvsp[0].a_wchunk), next);
	}
#line 3423 "asn1p_y.c"
    break;

  case 120: /* WithSyntaxList: WithSyntaxList WithSyntaxToken  */
#line 1247 "asn1p_y.y"
                                         {
		(yyval.a_wsynt) = (yyvsp[-1].a_wsynt);
		TQ_ADD(&((yyval.a_wsynt)->chunks), (yyvsp[0].a_wchunk), next);
	}
#line 3432 "asn1p_y.c"
    break;

  case 121: /* WithSyntaxToken: TOK_whitespace  */
#line 1254 "asn1p_y.y"
                       {
		(yyval.a_wchunk) = asn1p_wsyntx_chunk_fromstring((yyvsp[0].tv_opaque).buf, 0);
		(yyval.a_wchunk)->type = WC_WHITESPACE;
	}
#line 3441 "asn1p_y.c"
    break;

  case 122: /* WithSyntaxToken: TOK_Literal  */
#line 1258 "asn1p_y.y"
                      {
		(yyval.a_wchunk) = asn1p_wsyntx_chunk_fromstring((yyvsp[0].tv_str), 0);
	}
#line 3449 "asn1p_y.c"
    break;

  case 123: /* WithSyntaxToken: PrimitiveFieldReference  */
#line 1261 "asn1p_y.y"
                                  {
		(yyval.a_wchunk) = asn1p_wsyntx_chunk_fromstring((yyvsp[0].a_refcomp).name, 0);
		(yyval.a_wchunk)->type = WC_FIELD;
	}
#line 3458 "asn1p_y.c"
    break;

  case 124: /* WithSyntaxToken: '[' WithSyntaxList ']'  */
#line 1265 "asn1p_y.y"
                                 {
		(yyval.a_wchunk) = asn1p_wsyntx_chunk_fromsyntax((yyvsp[-1].a_wsynt));
	}
#line 3466 "asn1p_y.c"
    break;

  case 125: /* ExtensionAndException: "..."  */
#line 1271 "asn1p_y.y"
                      {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->Identifier = strdup("...");
		checkmem((yyval.a_expr)->Identifier);
		(yyval.a_expr)->expr_type = A1TC_EXTENSIBLE;
		(yyval.a_expr)->meta_type = AMT_TYPE;
	}
#line 3479 "asn1p_y.c"
    break;

  case 126: /* ExtensionAndException: "..." '!' DefinedValue  */
#line 1279 "asn1p_y.y"
                                         {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->Identifier = strdup("...");
		checkmem((yyval.a_expr)->Identifier);
		(yyval.a_expr)->value = (yyvsp[0].a_value);
		(yyval.a_expr)->expr_type = A1TC_EXTENSIBLE;
		(yyval.a_expr)->meta_type = AMT_TYPE;
	}
#line 3493 "asn1p_y.c"
    break;

  case 127: /* ExtensionAndException: "..." '!' SignedNumber  */
#line 1288 "asn1p_y.y"
                                         {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->Identifier = strdup("...");
		(yyval.a_expr)->value = (yyvsp[0].a_value);
		checkmem((yyval.a_expr)->Identifier);
		(yyval.a_expr)->expr_type = A1TC_EXTENSIBLE;
		(yyval.a_expr)->meta_type = AMT_TYPE;
	}
#line 3507 "asn1p_y.c"
    break;

  case 129: /* TaggedType: optTag UntaggedType  */
#line 1302 "asn1p_y.y"
                        {
        (yyval.a_expr) = (yyvsp[0].a_expr);
        (yyval.a_expr)->tag = (yyvsp[-1].a_tag);
    }
#line 3516 "asn1p_y.c"
    break;

  case 130: /* DefinedUntaggedType: DefinedType optManyConstraints  */
#line 1309 "asn1p_y.y"
                                       {
		(yyval.a_expr) = (yyvsp[-1].a_expr);
		/*
		 * Outer constraint for SEQUENCE OF and SET OF applies
		 * to the inner type.
		 */
		if((yyval.a_expr)->expr_type == ASN_CONSTR_SEQUENCE_OF
		|| (yyval.a_expr)->expr_type == ASN_CONSTR_SET_OF) {
			assert(!TQ_FIRST(&((yyval.a_expr)->members))->constraints);
			TQ_FIRST(&((yyval.a_expr)->members))->constraints = (yyvsp[0].a_constr);
		} else {
			if((yyval.a_expr)->constraints) {
				assert(!(yyvsp[0].a_constr));
				/* Check this : optManyConstraints is not used ?! */
				asn1p_constraint_free((yyvsp[0].a_constr));
			} else {
				(yyval.a_expr)->constraints = (yyvsp[0].a_constr);
			}
		}
	}
#line 3541 "asn1p_y.c"
    break;

  case 131: /* UntaggedType: TypeDeclaration optManyConstraints  */
#line 1332 "asn1p_y.y"
                                           {
		(yyval.a_expr) = (yyvsp[-1].a_expr);
		/*
		 * Outer constraint for SEQUENCE OF and SET OF applies
		 * to the inner type.
		 */
		if((yyval.a_expr)->expr_type == ASN_CONSTR_SEQUENCE_OF
		|| (yyval.a_expr)->expr_type == ASN_CONSTR_SET_OF) {
			assert(!TQ_FIRST(&((yyval.a_expr)->members))->constraints);
			TQ_FIRST(&((yyval.a_expr)->members))->constraints = (yyvsp[0].a_constr);
		} else {
			if((yyval.a_expr)->constraints) {
				assert(!(yyvsp[0].a_constr));
				/* Check this : optManyConstraints is not used ?! */
				asn1p_constraint_free((yyvsp[0].a_constr));
			} else {
				(yyval.a_expr)->constraints = (yyvsp[0].a_constr);
			}
		}
	}
#line 3566 "asn1p_y.c"
    break;

  case 132: /* MaybeIndirectTaggedType: optTag MaybeIndirectTypeDeclaration optManyConstraints  */
#line 1355 "asn1p_y.y"
                                                           {
		(yyval.a_expr) = (yyvsp[-1].a_expr);
		(yyval.a_expr)->tag = (yyvsp[-2].a_tag);
		/*
		 * Outer constraint for SEQUENCE OF and SET OF applies
		 * to the inner type.
		 */
		if((yyval.a_expr)->expr_type == ASN_CONSTR_SEQUENCE_OF
		|| (yyval.a_expr)->expr_type == ASN_CONSTR_SET_OF) {
			assert(!TQ_FIRST(&((yyval.a_expr)->members))->constraints);
			TQ_FIRST(&((yyval.a_expr)->members))->constraints = (yyvsp[0].a_constr);
		} else {
			if((yyval.a_expr)->constraints) {
				assert(!(yyvsp[-1].a_expr));
				/* Check this : optManyConstraints is not used ?! */
				asn1p_constraint_free((yyvsp[0].a_constr));
			} else {
				(yyval.a_expr)->constraints = (yyvsp[0].a_constr);
			}
		}
	}
#line 3592 "asn1p_y.c"
    break;

  case 133: /* NSTD_IndirectMarker: %empty  */
#line 1379 "asn1p_y.y"
        {
		(yyval.a_int) = asn1p_as_pointer ? EM_INDIRECT : 0;
		asn1p_as_pointer = 0;
	}
#line 3601 "asn1p_y.c"
    break;

  case 134: /* MaybeIndirectTypeDeclaration: NSTD_IndirectMarker TypeDeclaration  */
#line 1386 "asn1p_y.y"
                                        {
        (yyval.a_expr) = (yyvsp[0].a_expr);
		(yyval.a_expr)->marker.flags |= (yyvsp[-1].a_int);

		if(((yyval.a_expr)->marker.flags & EM_INDIRECT)
		&& ((yyval.a_expr)->marker.flags & EM_OPTIONAL) != EM_OPTIONAL) {
			fprintf(stderr,
				"INFO: Directive <ASN1C:RepresentAsPointer> "
				"applied to %s at %s:%d\n",
				ASN_EXPR_TYPE2STR((yyval.a_expr)->expr_type)
					?  ASN_EXPR_TYPE2STR((yyval.a_expr)->expr_type)
					: "member",
				ASN_FILENAME, (yyval.a_expr)->_lineno
			);
		}
    }
#line 3622 "asn1p_y.c"
    break;

  case 138: /* ConcreteTypeDeclaration: TOK_CHOICE '{' AlternativeTypeLists '}'  */
#line 1410 "asn1p_y.y"
                                                  {
		(yyval.a_expr) = (yyvsp[-1].a_expr);
		assert((yyval.a_expr)->expr_type == A1TC_INVALID);
		(yyval.a_expr)->expr_type = ASN_CONSTR_CHOICE;
		(yyval.a_expr)->meta_type = AMT_TYPE;
	}
#line 3633 "asn1p_y.c"
    break;

  case 139: /* ConcreteTypeDeclaration: TOK_SEQUENCE '{' optComponentTypeLists '}'  */
#line 1416 "asn1p_y.y"
                                                     {
		(yyval.a_expr) = (yyvsp[-1].a_expr);
		assert((yyval.a_expr)->expr_type == A1TC_INVALID);
		(yyval.a_expr)->expr_type = ASN_CONSTR_SEQUENCE;
		(yyval.a_expr)->meta_type = AMT_TYPE;
	}
#line 3644 "asn1p_y.c"
    break;

  case 140: /* ConcreteTypeDeclaration: TOK_SET '{' optComponentTypeLists '}'  */
#line 1422 "asn1p_y.y"
                                                {
		(yyval.a_expr) = (yyvsp[-1].a_expr);
		assert((yyval.a_expr)->expr_type == A1TC_INVALID);
		(yyval.a_expr)->expr_type = ASN_CONSTR_SET;
		(yyval.a_expr)->meta_type = AMT_TYPE;
	}
#line 3655 "asn1p_y.c"
    break;

  case 141: /* ConcreteTypeDeclaration: TOK_SEQUENCE optSizeOrConstraint TOK_OF optIdentifier optTag MaybeIndirectTypeDeclaration  */
#line 1428 "asn1p_y.y"
                                                                                                    {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->constraints = (yyvsp[-4].a_constr);
		(yyval.a_expr)->expr_type = ASN_CONSTR_SEQUENCE_OF;
		(yyval.a_expr)->meta_type = AMT_TYPE;
		(yyvsp[0].a_expr)->Identifier = (yyvsp[-2].tv_str);
		(yyvsp[0].a_expr)->tag = (yyvsp[-1].a_tag);
		asn1p_expr_add((yyval.a_expr), (yyvsp[0].a_expr));
	}
#line 3670 "asn1p_y.c"
    break;

  case 142: /* ConcreteTypeDeclaration: TOK_SET optSizeOrConstraint TOK_OF optIdentifier optTag MaybeIndirectTypeDeclaration  */
#line 1438 "asn1p_y.y"
                                                                                               {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->constraints = (yyvsp[-4].a_constr);
		(yyval.a_expr)->expr_type = ASN_CONSTR_SET_OF;
		(yyval.a_expr)->meta_type = AMT_TYPE;
		(yyvsp[0].a_expr)->Identifier = (yyvsp[-2].tv_str);
		(yyvsp[0].a_expr)->tag = (yyvsp[-1].a_tag);
		asn1p_expr_add((yyval.a_expr), (yyvsp[0].a_expr));
	}
#line 3685 "asn1p_y.c"
    break;

  case 143: /* ConcreteTypeDeclaration: TOK_ANY  */
#line 1448 "asn1p_y.y"
                                                        {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->expr_type = ASN_TYPE_ANY;
		(yyval.a_expr)->meta_type = AMT_TYPE;
	}
#line 3696 "asn1p_y.c"
    break;

  case 144: /* ConcreteTypeDeclaration: TOK_ANY TOK_DEFINED TOK_BY Identifier  */
#line 1454 "asn1p_y.y"
                                                        {
		int ret;
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->reference = asn1p_ref_new(yylineno, currentModule);
		ret = asn1p_ref_add_component((yyval.a_expr)->reference,
			(yyvsp[0].tv_str), RLT_lowercase);
		checkmem(ret == 0);
		(yyval.a_expr)->expr_type = ASN_TYPE_ANY;
		(yyval.a_expr)->meta_type = AMT_TYPE;
		free((yyvsp[0].tv_str));
	}
#line 3713 "asn1p_y.c"
    break;

  case 145: /* ConcreteTypeDeclaration: TOK_INSTANCE TOK_OF ComplexTypeReference  */
#line 1466 "asn1p_y.y"
                                                   {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->reference = (yyvsp[0].a_ref);
		(yyval.a_expr)->expr_type = A1TC_INSTANCE;
		(yyval.a_expr)->meta_type = AMT_TYPE;
	}
#line 3725 "asn1p_y.c"
    break;

  case 146: /* ComplexTypeReference: TOK_typereference  */
#line 1481 "asn1p_y.y"
                          {
		int ret;
		(yyval.a_ref) = asn1p_ref_new(yylineno, currentModule);
		checkmem((yyval.a_ref));
		ret = asn1p_ref_add_component((yyval.a_ref), (yyvsp[0].tv_str), RLT_UNKNOWN);
		checkmem(ret == 0);
		free((yyvsp[0].tv_str));
	}
#line 3738 "asn1p_y.c"
    break;

  case 147: /* ComplexTypeReference: TOK_capitalreference  */
#line 1489 "asn1p_y.y"
                               {
		int ret;
		(yyval.a_ref) = asn1p_ref_new(yylineno, currentModule);
		checkmem((yyval.a_ref));
		ret = asn1p_ref_add_component((yyval.a_ref), (yyvsp[0].tv_str), RLT_CAPITALS);
		free((yyvsp[0].tv_str));
		checkmem(ret == 0);
	}
#line 3751 "asn1p_y.c"
    break;

  case 148: /* ComplexTypeReference: TOK_typereference '.' TypeRefName  */
#line 1497 "asn1p_y.y"
                                            {
		int ret;
		(yyval.a_ref) = asn1p_ref_new(yylineno, currentModule);
		checkmem((yyval.a_ref));
		ret = asn1p_ref_add_component((yyval.a_ref), (yyvsp[-2].tv_str), RLT_UNKNOWN);
		checkmem(ret == 0);
		ret = asn1p_ref_add_component((yyval.a_ref), (yyvsp[0].tv_str), RLT_UNKNOWN);
		checkmem(ret == 0);
		free((yyvsp[-2].tv_str));
		free((yyvsp[0].tv_str));
	}
#line 3767 "asn1p_y.c"
    break;

  case 149: /* ComplexTypeReference: TOK_capitalreference '.' TypeRefName  */
#line 1508 "asn1p_y.y"
                                               {
		int ret;
		(yyval.a_ref) = asn1p_ref_new(yylineno, currentModule);
		checkmem((yyval.a_ref));
		ret = asn1p_ref_add_component((yyval.a_ref), (yyvsp[-2].tv_str), RLT_UNKNOWN);
		checkmem(ret == 0);
		ret = asn1p_ref_add_component((yyval.a_ref), (yyvsp[0].tv_str), RLT_UNKNOWN);
		checkmem(ret == 0);
		free((yyvsp[-2].tv_str));
		free((yyvsp[0].tv_str));
	}
#line 3783 "asn1p_y.c"
    break;

  case 150: /* ComplexTypeReference: TOK_capitalreference '.' ComplexTypeReferenceAmpList  */
#line 1519 "asn1p_y.y"
                                                               {
		int ret;
		(yyval.a_ref) = (yyvsp[0].a_ref);
		ret = asn1p_ref_add_component((yyval.a_ref), (yyvsp[-2].tv_str), RLT_CAPITALS);
		free((yyvsp[-2].tv_str));
		checkmem(ret == 0);
		/*
		 * Move the last element infront.
		 */
		{
			struct asn1p_ref_component_s tmp_comp;
			tmp_comp = (yyval.a_ref)->components[(yyval.a_ref)->comp_count-1];
			memmove(&(yyval.a_ref)->components[1],
				&(yyval.a_ref)->components[0],
				sizeof((yyval.a_ref)->components[0])
				* ((yyval.a_ref)->comp_count - 1));
			(yyval.a_ref)->components[0] = tmp_comp;
		}
	}
#line 3807 "asn1p_y.c"
    break;

  case 151: /* ComplexTypeReferenceAmpList: ComplexTypeReferenceElement  */
#line 1541 "asn1p_y.y"
                                    {
		int ret;
		(yyval.a_ref) = asn1p_ref_new(yylineno, currentModule);
		checkmem((yyval.a_ref));
		ret = asn1p_ref_add_component((yyval.a_ref), (yyvsp[0].a_refcomp).name, (yyvsp[0].a_refcomp).lex_type);
		free((yyvsp[0].a_refcomp).name);
		checkmem(ret == 0);
	}
#line 3820 "asn1p_y.c"
    break;

  case 152: /* ComplexTypeReferenceAmpList: ComplexTypeReferenceAmpList '.' ComplexTypeReferenceElement  */
#line 1549 "asn1p_y.y"
                                                                      {
		int ret;
		(yyval.a_ref) = (yyvsp[-2].a_ref);
		ret = asn1p_ref_add_component((yyval.a_ref), (yyvsp[0].a_refcomp).name, (yyvsp[0].a_refcomp).lex_type);
		free((yyvsp[0].a_refcomp).name);
		checkmem(ret == 0);
	}
#line 3832 "asn1p_y.c"
    break;

  case 154: /* PrimitiveFieldReference: TOK_typefieldreference  */
#line 1562 "asn1p_y.y"
                               {
		(yyval.a_refcomp).lex_type = RLT_AmpUppercase;
		(yyval.a_refcomp).name = (yyvsp[0].tv_str);
	}
#line 3841 "asn1p_y.c"
    break;

  case 155: /* PrimitiveFieldReference: TOK_valuefieldreference  */
#line 1567 "asn1p_y.y"
                                  {
		(yyval.a_refcomp).lex_type = RLT_Amplowercase;
		(yyval.a_refcomp).name = (yyvsp[0].tv_str);
	}
#line 3850 "asn1p_y.c"
    break;

  case 156: /* FieldName: TOK_typefieldreference  */
#line 1576 "asn1p_y.y"
                               {
		(yyval.a_ref) = asn1p_ref_new(yylineno, currentModule);
		asn1p_ref_add_component((yyval.a_ref), (yyvsp[0].tv_str), RLT_AmpUppercase);
		free((yyvsp[0].tv_str));
	}
#line 3860 "asn1p_y.c"
    break;

  case 157: /* FieldName: FieldName '.' TOK_typefieldreference  */
#line 1581 "asn1p_y.y"
                                               {
		(yyval.a_ref) = (yyval.a_ref);
		asn1p_ref_add_component((yyval.a_ref), (yyvsp[0].tv_str), RLT_AmpUppercase);
		free((yyvsp[0].tv_str));
	}
#line 3870 "asn1p_y.c"
    break;

  case 158: /* FieldName: FieldName '.' TOK_valuefieldreference  */
#line 1586 "asn1p_y.y"
                                                {
		(yyval.a_ref) = (yyval.a_ref);
		asn1p_ref_add_component((yyval.a_ref), (yyvsp[0].tv_str), RLT_Amplowercase);
		free((yyvsp[0].tv_str));
	}
#line 3880 "asn1p_y.c"
    break;

  case 159: /* DefinedObjectClass: TOK_capitalreference  */
#line 1594 "asn1p_y.y"
                             {
		(yyval.a_ref) = asn1p_ref_new(yylineno, currentModule);
		asn1p_ref_add_component((yyval.a_ref), (yyvsp[0].tv_str), RLT_CAPITALS);
		free((yyvsp[0].tv_str));
	}
#line 3890 "asn1p_y.c"
    break;

  case 160: /* ValueAssignment: Identifier Type TOK_PPEQ Value  */
#line 1617 "asn1p_y.y"
                                       {
		(yyval.a_expr) = (yyvsp[-2].a_expr);
		assert((yyval.a_expr)->Identifier == NULL);
		(yyval.a_expr)->Identifier = (yyvsp[-3].tv_str);
		(yyval.a_expr)->meta_type = AMT_VALUE;
		(yyval.a_expr)->value = (yyvsp[0].a_value);
	}
#line 3902 "asn1p_y.c"
    break;

  case 163: /* $@4: %empty  */
#line 1629 "asn1p_y.y"
              { asn1p_lexer_hack_push_opaque_state(); }
#line 3908 "asn1p_y.c"
    break;

  case 164: /* Value: '{' $@4 Opaque  */
#line 1629 "asn1p_y.y"
                                                               {
		(yyval.a_value) = asn1p_value_frombuf((yyvsp[0].tv_opaque).buf, (yyvsp[0].tv_opaque).len, 0);
		checkmem((yyval.a_value));
		(yyval.a_value)->type = ATV_UNPARSED;
	}
#line 3918 "asn1p_y.c"
    break;

  case 165: /* SimpleValue: TOK_NULL  */
#line 1637 "asn1p_y.y"
                 {
		(yyval.a_value) = asn1p_value_fromint(0);
		checkmem((yyval.a_value));
		(yyval.a_value)->type = ATV_NULL;
	}
#line 3928 "asn1p_y.c"
    break;

  case 166: /* SimpleValue: TOK_FALSE  */
#line 1642 "asn1p_y.y"
                    {
		(yyval.a_value) = asn1p_value_fromint(0);
		checkmem((yyval.a_value));
		(yyval.a_value)->type = ATV_FALSE;
	}
#line 3938 "asn1p_y.c"
    break;

  case 167: /* SimpleValue: TOK_TRUE  */
#line 1647 "asn1p_y.y"
                   {
		(yyval.a_value) = asn1p_value_fromint(1);
		checkmem((yyval.a_value));
		(yyval.a_value)->type = ATV_TRUE;
	}
#line 3948 "asn1p_y.c"
    break;

  case 173: /* DefinedValue: TypeRefName '.' Identifier  */
#line 1660 "asn1p_y.y"
                                     {
		asn1p_ref_t *ref;
		int ret;
		ref = asn1p_ref_new(yylineno, currentModule);
		checkmem(ref);
		ret = asn1p_ref_add_component(ref, (yyvsp[-2].tv_str), RLT_UNKNOWN);
		checkmem(ret == 0);
		ret = asn1p_ref_add_component(ref, (yyvsp[0].tv_str), RLT_lowercase);
		checkmem(ret == 0);
		(yyval.a_value) = asn1p_value_fromref(ref, 0);
		checkmem((yyval.a_value));
		free((yyvsp[-2].tv_str));
		free((yyvsp[0].tv_str));
	}
#line 3967 "asn1p_y.c"
    break;

  case 174: /* RestrictedCharacterStringValue: TOK_cstring  */
#line 1678 "asn1p_y.y"
                    {
		(yyval.a_value) = asn1p_value_frombuf((yyvsp[0].tv_opaque).buf, (yyvsp[0].tv_opaque).len, 0);
		checkmem((yyval.a_value));
	}
#line 3976 "asn1p_y.c"
    break;

  case 175: /* RestrictedCharacterStringValue: TOK_tuple  */
#line 1682 "asn1p_y.y"
                    {
		(yyval.a_value) = asn1p_value_fromint((yyvsp[0].a_int));
		checkmem((yyval.a_value));
		(yyval.a_value)->type = ATV_TUPLE;
	}
#line 3986 "asn1p_y.c"
    break;

  case 176: /* RestrictedCharacterStringValue: TOK_quadruple  */
#line 1687 "asn1p_y.y"
                        {
		(yyval.a_value) = asn1p_value_fromint((yyvsp[0].a_int));
		checkmem((yyval.a_value));
		(yyval.a_value)->type = ATV_QUADRUPLE;
	}
#line 3996 "asn1p_y.c"
    break;

  case 177: /* Opaque: OpaqueFirstToken  */
#line 1695 "asn1p_y.y"
                     {
		(yyval.tv_opaque).len = (yyvsp[0].tv_opaque).len + 1;
		(yyval.tv_opaque).buf = malloc(1 + (yyval.tv_opaque).len + 1);
		checkmem((yyval.tv_opaque).buf);
		(yyval.tv_opaque).buf[0] = '{';
		memcpy((yyval.tv_opaque).buf + 1, (yyvsp[0].tv_opaque).buf, (yyvsp[0].tv_opaque).len);
		(yyval.tv_opaque).buf[(yyval.tv_opaque).len] = '\0';
		free((yyvsp[0].tv_opaque).buf);
    }
#line 4010 "asn1p_y.c"
    break;

  case 178: /* Opaque: Opaque TOK_opaque  */
#line 1704 "asn1p_y.y"
                            {
		int newsize = (yyvsp[-1].tv_opaque).len + (yyvsp[0].tv_opaque).len;
		char *p = malloc(newsize + 1);
		checkmem(p);
		memcpy(p         , (yyvsp[-1].tv_opaque).buf, (yyvsp[-1].tv_opaque).len);
		memcpy(p + (yyvsp[-1].tv_opaque).len, (yyvsp[0].tv_opaque).buf, (yyvsp[0].tv_opaque).len);
		p[newsize] = '\0';
		free((yyvsp[-1].tv_opaque).buf);
		free((yyvsp[0].tv_opaque).buf);
		(yyval.tv_opaque).buf = p;
		(yyval.tv_opaque).len = newsize;
	}
#line 4027 "asn1p_y.c"
    break;

  case 180: /* OpaqueFirstToken: Identifier  */
#line 1720 "asn1p_y.y"
                 {
        (yyval.tv_opaque).len = strlen((yyvsp[0].tv_str));
        (yyval.tv_opaque).buf = (yyvsp[0].tv_str);
    }
#line 4036 "asn1p_y.c"
    break;

  case 181: /* BasicTypeId: TOK_BOOLEAN  */
#line 1726 "asn1p_y.y"
                    { (yyval.a_type) = ASN_BASIC_BOOLEAN; }
#line 4042 "asn1p_y.c"
    break;

  case 182: /* BasicTypeId: TOK_NULL  */
#line 1727 "asn1p_y.y"
                   { (yyval.a_type) = ASN_BASIC_NULL; }
#line 4048 "asn1p_y.c"
    break;

  case 183: /* BasicTypeId: TOK_REAL  */
#line 1728 "asn1p_y.y"
                   { (yyval.a_type) = ASN_BASIC_REAL; }
#line 4054 "asn1p_y.c"
    break;

  case 184: /* BasicTypeId: TOK_OCTET TOK_STRING  */
#line 1729 "asn1p_y.y"
                               { (yyval.a_type) = ASN_BASIC_OCTET_STRING; }
#line 4060 "asn1p_y.c"
    break;

  case 185: /* BasicTypeId: TOK_OBJECT TOK_IDENTIFIER  */
#line 1730 "asn1p_y.y"
                                    { (yyval.a_type) = ASN_BASIC_OBJECT_IDENTIFIER; }
#line 4066 "asn1p_y.c"
    break;

  case 186: /* BasicTypeId: TOK_RELATIVE_OID  */
#line 1731 "asn1p_y.y"
                           { (yyval.a_type) = ASN_BASIC_RELATIVE_OID; }
#line 4072 "asn1p_y.c"
    break;

  case 187: /* BasicTypeId: TOK_EMBEDDED TOK_PDV  */
#line 1733 "asn1p_y.y"
                               { (yyval.a_type) = ASN_BASIC_EMBEDDED_PDV; }
#line 4078 "asn1p_y.c"
    break;

  case 188: /* BasicTypeId: TOK_CHARACTER TOK_STRING  */
#line 1734 "asn1p_y.y"
                                   { (yyval.a_type) = ASN_BASIC_CHARACTER_STRING; }
#line 4084 "asn1p_y.c"
    break;

  case 189: /* BasicTypeId: TOK_UTCTime  */
#line 1735 "asn1p_y.y"
                      { (yyval.a_type) = ASN_BASIC_UTCTime; }
#line 4090 "asn1p_y.c"
    break;

  case 190: /* BasicTypeId: TOK_GeneralizedTime  */
#line 1736 "asn1p_y.y"
                              { (yyval.a_type) = ASN_BASIC_GeneralizedTime; }
#line 4096 "asn1p_y.c"
    break;

  case 193: /* BasicTypeId_UniverationCompatible: TOK_INTEGER  */
#line 1745 "asn1p_y.y"
                    { (yyval.a_type) = ASN_BASIC_INTEGER; }
#line 4102 "asn1p_y.c"
    break;

  case 194: /* BasicTypeId_UniverationCompatible: TOK_ENUMERATED  */
#line 1746 "asn1p_y.y"
                         { (yyval.a_type) = ASN_BASIC_ENUMERATED; }
#line 4108 "asn1p_y.c"
    break;

  case 195: /* BasicTypeId_UniverationCompatible: TOK_BIT TOK_STRING  */
#line 1747 "asn1p_y.y"
                             { (yyval.a_type) = ASN_BASIC_BIT_STRING; }
#line 4114 "asn1p_y.c"
    break;

  case 196: /* BuiltinType: BasicTypeId  */
#line 1751 "asn1p_y.y"
                    {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->expr_type = (yyvsp[0].a_type);
		(yyval.a_expr)->meta_type = AMT_TYPE;
	}
#line 4125 "asn1p_y.c"
    break;

  case 197: /* BuiltinType: TOK_INTEGER '{' NamedNumberList '}'  */
#line 1757 "asn1p_y.y"
                                          {
        (yyval.a_expr) = (yyvsp[-1].a_expr);
        (yyval.a_expr)->expr_type = ASN_BASIC_INTEGER;
        (yyval.a_expr)->meta_type = AMT_TYPE;
    }
#line 4135 "asn1p_y.c"
    break;

  case 198: /* BuiltinType: TOK_ENUMERATED '{' Enumerations '}'  */
#line 1762 "asn1p_y.y"
                                          {
        (yyval.a_expr) = (yyvsp[-1].a_expr);
        (yyval.a_expr)->expr_type = ASN_BASIC_ENUMERATED;
        (yyval.a_expr)->meta_type = AMT_TYPE;
    }
#line 4145 "asn1p_y.c"
    break;

  case 199: /* BuiltinType: TOK_BIT TOK_STRING '{' NamedBitList '}'  */
#line 1767 "asn1p_y.y"
                                              {
        (yyval.a_expr) = (yyvsp[-1].a_expr);
        (yyval.a_expr)->expr_type = ASN_BASIC_BIT_STRING;
        (yyval.a_expr)->meta_type = AMT_TYPE;
    }
#line 4155 "asn1p_y.c"
    break;

  case 200: /* BuiltinType: TOK_ExtValue_BIT_STRING '{' IdentifierList '}'  */
#line 1772 "asn1p_y.y"
                                                     {
        (yyval.a_expr) = (yyvsp[-1].a_expr);
        (yyval.a_expr)->expr_type = ASN_BASIC_BIT_STRING;
        (yyval.a_expr)->meta_type = AMT_TYPE;
    }
#line 4165 "asn1p_y.c"
    break;

  case 201: /* BuiltinType: TOK_ExtValue_BIT_STRING '{' '}'  */
#line 1777 "asn1p_y.y"
                                      {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
        (yyval.a_expr)->expr_type = ASN_BASIC_BIT_STRING;
        (yyval.a_expr)->meta_type = AMT_TYPE;
    }
#line 4176 "asn1p_y.c"
    break;

  case 202: /* BasicString: TOK_BMPString  */
#line 1786 "asn1p_y.y"
                      { (yyval.a_type) = ASN_STRING_BMPString; }
#line 4182 "asn1p_y.c"
    break;

  case 203: /* BasicString: TOK_GeneralString  */
#line 1787 "asn1p_y.y"
                            {
		(yyval.a_type) = ASN_STRING_GeneralString;
		fprintf(stderr, "WARNING: GeneralString is not fully supported\n");
	}
#line 4191 "asn1p_y.c"
    break;

  case 204: /* BasicString: TOK_GraphicString  */
#line 1791 "asn1p_y.y"
                            {
		(yyval.a_type) = ASN_STRING_GraphicString;
		fprintf(stderr, "WARNING: GraphicString is not fully supported\n");
	}
#line 4200 "asn1p_y.c"
    break;

  case 205: /* BasicString: TOK_IA5String  */
#line 1795 "asn1p_y.y"
                        { (yyval.a_type) = ASN_STRING_IA5String; }
#line 4206 "asn1p_y.c"
    break;

  case 206: /* BasicString: TOK_ISO646String  */
#line 1796 "asn1p_y.y"
                           { (yyval.a_type) = ASN_STRING_ISO646String; }
#line 4212 "asn1p_y.c"
    break;

  case 207: /* BasicString: TOK_NumericString  */
#line 1797 "asn1p_y.y"
                            { (yyval.a_type) = ASN_STRING_NumericString; }
#line 4218 "asn1p_y.c"
    break;

  case 208: /* BasicString: TOK_PrintableString  */
#line 1798 "asn1p_y.y"
                              { (yyval.a_type) = ASN_STRING_PrintableString; }
#line 4224 "asn1p_y.c"
    break;

  case 209: /* BasicString: TOK_T61String  */
#line 1799 "asn1p_y.y"
                        {
		(yyval.a_type) = ASN_STRING_T61String;
		fprintf(stderr, "WARNING: T61String is not fully supported\n");
	}
#line 4233 "asn1p_y.c"
    break;

  case 210: /* BasicString: TOK_TeletexString  */
#line 1803 "asn1p_y.y"
                            { (yyval.a_type) = ASN_STRING_TeletexString; }
#line 4239 "asn1p_y.c"
    break;

  case 211: /* BasicString: TOK_UniversalString  */
#line 1804 "asn1p_y.y"
                              { (yyval.a_type) = ASN_STRING_UniversalString; }
#line 4245 "asn1p_y.c"
    break;

  case 212: /* BasicString: TOK_UTF8String  */
#line 1805 "asn1p_y.y"
                         { (yyval.a_type) = ASN_STRING_UTF8String; }
#line 4251 "asn1p_y.c"
    break;

  case 213: /* BasicString: TOK_VideotexString  */
#line 1806 "asn1p_y.y"
                             {
		(yyval.a_type) = ASN_STRING_VideotexString;
		fprintf(stderr, "WARNING: VideotexString is not fully supported\n");
	}
#line 4260 "asn1p_y.c"
    break;

  case 214: /* BasicString: TOK_VisibleString  */
#line 1810 "asn1p_y.y"
                            { (yyval.a_type) = ASN_STRING_VisibleString; }
#line 4266 "asn1p_y.c"
    break;

  case 215: /* BasicString: TOK_ObjectDescriptor  */
#line 1811 "asn1p_y.y"
                               { (yyval.a_type) = ASN_STRING_ObjectDescriptor; }
#line 4272 "asn1p_y.c"
    break;

  case 220: /* optConstraint: %empty  */
#line 1823 "asn1p_y.y"
        { (yyval.a_constr) = 0; }
#line 4278 "asn1p_y.c"
    break;

  case 222: /* optManyConstraints: %empty  */
#line 1828 "asn1p_y.y"
        { (yyval.a_constr) = 0; }
#line 4284 "asn1p_y.c"
    break;

  case 224: /* optSizeOrConstraint: %empty  */
#line 1833 "asn1p_y.y"
        { (yyval.a_constr) = 0; }
#line 4290 "asn1p_y.c"
    break;

  case 226: /* optSizeOrConstraint: SizeConstraint  */
#line 1835 "asn1p_y.y"
                         {
		CONSTRAINT_INSERT((yyval.a_constr), ACT_CA_SET, (yyvsp[0].a_constr), 0);
    }
#line 4298 "asn1p_y.c"
    break;

  case 227: /* Constraint: '(' ConstraintSpec ')'  */
#line 1841 "asn1p_y.y"
                           {
		CONSTRAINT_INSERT((yyval.a_constr), ACT_CA_SET, (yyvsp[-1].a_constr), 0);
    }
#line 4306 "asn1p_y.c"
    break;

  case 229: /* ManyConstraints: ManyConstraints Constraint  */
#line 1848 "asn1p_y.y"
                                     {
        if((yyvsp[0].a_constr)->type == ACT_CA_SET && (yyvsp[0].a_constr)->el_count == 1) {
            CONSTRAINT_INSERT((yyval.a_constr), ACT_CA_SET, (yyvsp[-1].a_constr), (yyvsp[0].a_constr)->elements[0]);
        } else {
            CONSTRAINT_INSERT((yyval.a_constr), ACT_CA_SET, (yyvsp[-1].a_constr), (yyvsp[0].a_constr));
        }
	}
#line 4318 "asn1p_y.c"
    break;

  case 233: /* ElementSetSpecs: "..."  */
#line 1862 "asn1p_y.y"
                       {
		(yyval.a_constr) = asn1p_constraint_new(yylineno, currentModule);
		(yyval.a_constr)->type = ACT_EL_EXT;
	}
#line 4327 "asn1p_y.c"
    break;

  case 235: /* ElementSetSpecs: ElementSetSpec ',' "..."  */
#line 1867 "asn1p_y.y"
                                      {
       asn1p_constraint_t *ct;
       ct = asn1p_constraint_new(yylineno, currentModule);
       ct->type = ACT_EL_EXT;
       CONSTRAINT_INSERT((yyval.a_constr), ACT_CA_CSV, (yyvsp[-2].a_constr), ct);
   }
#line 4338 "asn1p_y.c"
    break;

  case 236: /* ElementSetSpecs: ElementSetSpec ',' "..." ',' ElementSetSpec  */
#line 1873 "asn1p_y.y"
                                                         {
       asn1p_constraint_t *ct;
       ct = asn1p_constraint_new(yylineno, currentModule);
       ct->type = ACT_EL_EXT;
       CONSTRAINT_INSERT((yyval.a_constr), ACT_CA_CSV, (yyvsp[-4].a_constr), ct);
       ct = (yyval.a_constr);
       CONSTRAINT_INSERT((yyval.a_constr), ACT_CA_CSV, ct, (yyvsp[0].a_constr));
   }
#line 4351 "asn1p_y.c"
    break;

  case 238: /* ElementSetSpec: TOK_ALL TOK_EXCEPT Elements  */
#line 1885 "asn1p_y.y"
                                      {
		CONSTRAINT_INSERT((yyval.a_constr), ACT_CA_AEX, (yyvsp[0].a_constr), 0);
	}
#line 4359 "asn1p_y.c"
    break;

  case 240: /* Unions: Unions UnionMark Intersections  */
#line 1892 "asn1p_y.y"
                                         {
		CONSTRAINT_INSERT((yyval.a_constr), ACT_CA_UNI, (yyvsp[-2].a_constr), (yyvsp[0].a_constr));
	}
#line 4367 "asn1p_y.c"
    break;

  case 242: /* Intersections: Intersections IntersectionMark IntersectionElements  */
#line 1899 "asn1p_y.y"
                                                               {
		CONSTRAINT_INSERT((yyval.a_constr), ACT_CA_INT, (yyvsp[-2].a_constr), (yyvsp[0].a_constr));
	}
#line 4375 "asn1p_y.c"
    break;

  case 244: /* IntersectionElements: Elements TOK_EXCEPT Elements  */
#line 1907 "asn1p_y.y"
                                       {
		CONSTRAINT_INSERT((yyval.a_constr), ACT_CA_EXC, (yyvsp[-2].a_constr), (yyvsp[0].a_constr));
	}
#line 4383 "asn1p_y.c"
    break;

  case 246: /* Elements: '(' ElementSetSpec ')'  */
#line 1914 "asn1p_y.y"
                             {
        int ret;
        (yyval.a_constr) = asn1p_constraint_new(yylineno, currentModule);
        checkmem((yyval.a_constr));
        (yyval.a_constr)->type = ACT_CA_SET;
        ret = asn1p_constraint_insert((yyval.a_constr), (yyvsp[-1].a_constr));
        checkmem(ret == 0);
    }
#line 4396 "asn1p_y.c"
    break;

  case 247: /* SubtypeElements: SingleValue  */
#line 1925 "asn1p_y.y"
                    {
		(yyval.a_constr) = asn1p_constraint_new(yylineno, currentModule);
		checkmem((yyval.a_constr));
		(yyval.a_constr)->type = ACT_EL_VALUE;
		(yyval.a_constr)->value = (yyvsp[0].a_value);
	}
#line 4407 "asn1p_y.c"
    break;

  case 248: /* SubtypeElements: ContainedSubtype  */
#line 1931 "asn1p_y.y"
                           {
		(yyval.a_constr) = asn1p_constraint_new(yylineno, currentModule);
		checkmem((yyval.a_constr));
		(yyval.a_constr)->type = ACT_EL_TYPE;
		(yyval.a_constr)->containedSubtype = (yyvsp[0].a_value);
	}
#line 4418 "asn1p_y.c"
    break;

  case 254: /* PermittedAlphabet: TOK_FROM Constraint  */
#line 1947 "asn1p_y.y"
                            {
		CONSTRAINT_INSERT((yyval.a_constr), ACT_CT_FROM, (yyvsp[0].a_constr), 0);
	}
#line 4426 "asn1p_y.c"
    break;

  case 255: /* SizeConstraint: TOK_SIZE Constraint  */
#line 1952 "asn1p_y.y"
                            {
		CONSTRAINT_INSERT((yyval.a_constr), ACT_CT_SIZE, (yyvsp[0].a_constr), 0);
	}
#line 4434 "asn1p_y.c"
    break;

  case 256: /* PatternConstraint: TOK_PATTERN TOK_cstring  */
#line 1957 "asn1p_y.y"
                                {
		(yyval.a_constr) = asn1p_constraint_new(yylineno, currentModule);
		(yyval.a_constr)->type = ACT_CT_PATTERN;
		(yyval.a_constr)->value = asn1p_value_frombuf((yyvsp[0].tv_opaque).buf, (yyvsp[0].tv_opaque).len, 0);
	}
#line 4444 "asn1p_y.c"
    break;

  case 257: /* PatternConstraint: TOK_PATTERN Identifier  */
#line 1962 "asn1p_y.y"
                                 {
		asn1p_ref_t *ref;
		(yyval.a_constr) = asn1p_constraint_new(yylineno, currentModule);
		(yyval.a_constr)->type = ACT_CT_PATTERN;
		ref = asn1p_ref_new(yylineno, currentModule);
		asn1p_ref_add_component(ref, (yyvsp[0].tv_str), RLT_lowercase);
		(yyval.a_constr)->value = asn1p_value_fromref(ref, 0);
		free((yyvsp[0].tv_str));
	}
#line 4458 "asn1p_y.c"
    break;

  case 258: /* ValueRange: LowerEndValue ConstraintRangeSpec UpperEndValue  */
#line 1974 "asn1p_y.y"
                                                    {
		(yyval.a_constr) = asn1p_constraint_new(yylineno, currentModule);
		checkmem((yyval.a_constr));
		(yyval.a_constr)->type = (yyvsp[-1].a_ctype);
		(yyval.a_constr)->range_start = (yyvsp[-2].a_value);
		(yyval.a_constr)->range_stop = (yyvsp[0].a_value);
    }
#line 4470 "asn1p_y.c"
    break;

  case 260: /* LowerEndValue: TOK_MIN  */
#line 1984 "asn1p_y.y"
              {
		(yyval.a_value) = asn1p_value_fromint(-123);
		(yyval.a_value)->type = ATV_MIN;
    }
#line 4479 "asn1p_y.c"
    break;

  case 262: /* UpperEndValue: TOK_MAX  */
#line 1991 "asn1p_y.y"
              {
		(yyval.a_value) = asn1p_value_fromint(321);
		(yyval.a_value)->type = ATV_MAX;
    }
#line 4488 "asn1p_y.c"
    break;

  case 264: /* BitStringValue: TOK_bstring  */
#line 1999 "asn1p_y.y"
                    {
		(yyval.a_value) = _convert_bitstring2binary((yyvsp[0].tv_str), 'B');
		checkmem((yyval.a_value));
		free((yyvsp[0].tv_str));
	}
#line 4498 "asn1p_y.c"
    break;

  case 265: /* BitStringValue: TOK_hstring  */
#line 2004 "asn1p_y.y"
                      {
		(yyval.a_value) = _convert_bitstring2binary((yyvsp[0].tv_str), 'H');
		checkmem((yyval.a_value));
		free((yyvsp[0].tv_str));
	}
#line 4508 "asn1p_y.c"
    break;

  case 266: /* ContainedSubtype: TOK_INCLUDES Type  */
#line 2012 "asn1p_y.y"
                      {
		(yyval.a_value) = asn1p_value_fromtype((yyvsp[0].a_expr));
		checkmem((yyval.a_value));
		asn1p_expr_free((yyvsp[0].a_expr));
    }
#line 4518 "asn1p_y.c"
    break;

  case 267: /* ContainedSubtype: DefinedUntaggedType  */
#line 2018 "asn1p_y.y"
                          {
		(yyval.a_value) = asn1p_value_fromtype((yyvsp[0].a_expr));
		checkmem((yyval.a_value));
		asn1p_expr_free((yyvsp[0].a_expr));
    }
#line 4528 "asn1p_y.c"
    break;

  case 268: /* InnerTypeConstraints: TOK_WITH TOK_COMPONENT SingleTypeConstraint  */
#line 2030 "asn1p_y.y"
                                                    {
		CONSTRAINT_INSERT((yyval.a_constr), ACT_CT_WCOMP, (yyvsp[0].a_constr), 0);
	}
#line 4536 "asn1p_y.c"
    break;

  case 269: /* InnerTypeConstraints: TOK_WITH TOK_COMPONENTS MultipleTypeConstraints  */
#line 2033 "asn1p_y.y"
                                                          {
        assert((yyvsp[0].a_constr)->type == ACT_CA_CSV);
        (yyvsp[0].a_constr)->type = ACT_CT_WCOMPS;
        (yyval.a_constr) = (yyvsp[0].a_constr);
	}
#line 4546 "asn1p_y.c"
    break;

  case 273: /* FullSpecification: '{' TypeConstraints '}'  */
#line 2041 "asn1p_y.y"
                                           { (yyval.a_constr) = (yyvsp[-1].a_constr); }
#line 4552 "asn1p_y.c"
    break;

  case 274: /* PartialSpecification: '{' "..." ',' TypeConstraints '}'  */
#line 2043 "asn1p_y.y"
                                              {
        assert((yyvsp[-1].a_constr)->type == ACT_CA_CSV);
		(yyval.a_constr) = asn1p_constraint_new(yylineno, currentModule);
        (yyval.a_constr)->type = ACT_CA_CSV;
		asn1p_constraint_t *ct = asn1p_constraint_new(yylineno, currentModule);
		checkmem((yyval.a_constr));
		ct->type = ACT_EL_EXT;
        asn1p_constraint_insert((yyval.a_constr), ct);
        for(unsigned i = 0; i < (yyvsp[-1].a_constr)->el_count; i++) {
            asn1p_constraint_insert((yyval.a_constr), (yyvsp[-1].a_constr)->elements[i]);
        }
    }
#line 4569 "asn1p_y.c"
    break;

  case 275: /* TypeConstraints: NamedConstraint  */
#line 2056 "asn1p_y.y"
                    {
        (yyval.a_constr) = asn1p_constraint_new(yylineno, currentModule);
        (yyval.a_constr)->type = ACT_CA_CSV;
        asn1p_constraint_insert((yyval.a_constr), (yyvsp[0].a_constr));
    }
#line 4579 "asn1p_y.c"
    break;

  case 276: /* TypeConstraints: TypeConstraints ',' NamedConstraint  */
#line 2061 "asn1p_y.y"
                                          {
        (yyval.a_constr) = (yyvsp[-2].a_constr);
        asn1p_constraint_insert((yyval.a_constr), (yyvsp[0].a_constr));
	}
#line 4588 "asn1p_y.c"
    break;

  case 277: /* NamedConstraint: IdentifierAsValue optConstraint optPresenceConstraint  */
#line 2067 "asn1p_y.y"
                                                              {
        (yyval.a_constr) = asn1p_constraint_new(yylineno, currentModule);
        checkmem((yyval.a_constr));
        (yyval.a_constr)->type = ACT_EL_VALUE;
        (yyval.a_constr)->value = (yyvsp[-2].a_value);
        if((yyvsp[-1].a_constr)) asn1p_constraint_insert((yyval.a_constr), (yyvsp[-1].a_constr));
        (yyval.a_constr)->presence = (yyvsp[0].a_pres);
    }
#line 4601 "asn1p_y.c"
    break;

  case 278: /* optPresenceConstraint: %empty  */
#line 2081 "asn1p_y.y"
        { (yyval.a_pres) = ACPRES_DEFAULT; }
#line 4607 "asn1p_y.c"
    break;

  case 279: /* optPresenceConstraint: PresenceConstraint  */
#line 2082 "asn1p_y.y"
                             { (yyval.a_pres) = (yyvsp[0].a_pres); }
#line 4613 "asn1p_y.c"
    break;

  case 280: /* PresenceConstraint: TOK_PRESENT  */
#line 2086 "asn1p_y.y"
                    {
		(yyval.a_pres) = ACPRES_PRESENT;
	}
#line 4621 "asn1p_y.c"
    break;

  case 281: /* PresenceConstraint: TOK_ABSENT  */
#line 2089 "asn1p_y.y"
                     {
		(yyval.a_pres) = ACPRES_ABSENT;
	}
#line 4629 "asn1p_y.c"
    break;

  case 282: /* PresenceConstraint: TOK_OPTIONAL  */
#line 2092 "asn1p_y.y"
                       {
		(yyval.a_pres) = ACPRES_OPTIONAL;
	}
#line 4637 "asn1p_y.c"
    break;

  case 286: /* $@5: %empty  */
#line 2107 "asn1p_y.y"
                { asn1p_lexer_hack_push_opaque_state(); }
#line 4643 "asn1p_y.c"
    break;

  case 287: /* UserDefinedConstraint: TOK_CONSTRAINED TOK_BY '{' $@5 Opaque  */
#line 2107 "asn1p_y.y"
                                                                           {
		(yyval.a_constr) = asn1p_constraint_new(yylineno, currentModule);
		checkmem((yyval.a_constr));
		(yyval.a_constr)->type = ACT_CT_CTDBY;
		(yyval.a_constr)->value = asn1p_value_frombuf((yyvsp[0].tv_opaque).buf, (yyvsp[0].tv_opaque).len, 0);
		checkmem((yyval.a_constr)->value);
		(yyval.a_constr)->value->type = ATV_UNPARSED;
	}
#line 4656 "asn1p_y.c"
    break;

  case 288: /* ContentsConstraint: TOK_CONTAINING Type  */
#line 2118 "asn1p_y.y"
                            {
		(yyval.a_constr) = asn1p_constraint_new(yylineno, currentModule);
		(yyval.a_constr)->type = ACT_CT_CTNG;
		(yyval.a_constr)->value = asn1p_value_fromtype((yyvsp[0].a_expr));
		asn1p_expr_free((yyvsp[0].a_expr));
	}
#line 4667 "asn1p_y.c"
    break;

  case 289: /* ConstraintRangeSpec: ".."  */
#line 2127 "asn1p_y.y"
                                { (yyval.a_ctype) = ACT_EL_RANGE; }
#line 4673 "asn1p_y.c"
    break;

  case 290: /* ConstraintRangeSpec: ".." '<'  */
#line 2128 "asn1p_y.y"
                                { (yyval.a_ctype) = ACT_EL_RLRANGE; }
#line 4679 "asn1p_y.c"
    break;

  case 291: /* ConstraintRangeSpec: '<' ".."  */
#line 2129 "asn1p_y.y"
                                { (yyval.a_ctype) = ACT_EL_LLRANGE; }
#line 4685 "asn1p_y.c"
    break;

  case 292: /* ConstraintRangeSpec: '<' ".." '<'  */
#line 2130 "asn1p_y.y"
                                { (yyval.a_ctype) = ACT_EL_ULRANGE; }
#line 4691 "asn1p_y.c"
    break;

  case 293: /* TableConstraint: SimpleTableConstraint  */
#line 2133 "asn1p_y.y"
                              {
		(yyval.a_constr) = (yyvsp[0].a_constr);
	}
#line 4699 "asn1p_y.c"
    break;

  case 294: /* TableConstraint: ComponentRelationConstraint  */
#line 2136 "asn1p_y.y"
                                      {
		(yyval.a_constr) = (yyvsp[0].a_constr);
	}
#line 4707 "asn1p_y.c"
    break;

  case 295: /* SimpleTableConstraint: '{' TypeRefName '}'  */
#line 2145 "asn1p_y.y"
                            {
		asn1p_ref_t *ref = asn1p_ref_new(yylineno, currentModule);
		asn1p_constraint_t *ct;
		int ret;
		ret = asn1p_ref_add_component(ref, (yyvsp[-1].tv_str), 0);
		checkmem(ret == 0);
		ct = asn1p_constraint_new(yylineno, currentModule);
		checkmem((yyval.a_constr));
		ct->type = ACT_EL_VALUE;
		ct->value = asn1p_value_fromref(ref, 0);
		CONSTRAINT_INSERT((yyval.a_constr), ACT_CA_CRC, ct, 0);
		free((yyvsp[-1].tv_str));
	}
#line 4725 "asn1p_y.c"
    break;

  case 296: /* ComponentRelationConstraint: SimpleTableConstraint '{' AtNotationList '}'  */
#line 2161 "asn1p_y.y"
                                                     {
		CONSTRAINT_INSERT((yyval.a_constr), ACT_CA_CRC, (yyvsp[-3].a_constr), (yyvsp[-1].a_constr));
	}
#line 4733 "asn1p_y.c"
    break;

  case 297: /* AtNotationList: AtNotationElement  */
#line 2167 "asn1p_y.y"
                          {
		(yyval.a_constr) = asn1p_constraint_new(yylineno, currentModule);
		checkmem((yyval.a_constr));
		(yyval.a_constr)->type = ACT_EL_VALUE;
		(yyval.a_constr)->value = asn1p_value_fromref((yyvsp[0].a_ref), 0);
	}
#line 4744 "asn1p_y.c"
    break;

  case 298: /* AtNotationList: AtNotationList ',' AtNotationElement  */
#line 2173 "asn1p_y.y"
                                               {
		asn1p_constraint_t *ct;
		ct = asn1p_constraint_new(yylineno, currentModule);
		checkmem(ct);
		ct->type = ACT_EL_VALUE;
		ct->value = asn1p_value_fromref((yyvsp[0].a_ref), 0);
		CONSTRAINT_INSERT((yyval.a_constr), ACT_CA_CSV, (yyvsp[-2].a_constr), ct);
	}
#line 4757 "asn1p_y.c"
    break;

  case 299: /* AtNotationElement: '@' ComponentIdList  */
#line 2187 "asn1p_y.y"
                            {
		char *p = malloc(strlen((yyvsp[0].tv_str)) + 2);
		int ret;
		*p = '@';
		strcpy(p + 1, (yyvsp[0].tv_str));
		(yyval.a_ref) = asn1p_ref_new(yylineno, currentModule);
		ret = asn1p_ref_add_component((yyval.a_ref), p, 0);
		checkmem(ret == 0);
		free(p);
		free((yyvsp[0].tv_str));
	}
#line 4773 "asn1p_y.c"
    break;

  case 300: /* AtNotationElement: '@' '.' ComponentIdList  */
#line 2198 "asn1p_y.y"
                                  {
		char *p = malloc(strlen((yyvsp[0].tv_str)) + 3);
		int ret;
		p[0] = '@';
		p[1] = '.';
		strcpy(p + 2, (yyvsp[0].tv_str));
		(yyval.a_ref) = asn1p_ref_new(yylineno, currentModule);
		ret = asn1p_ref_add_component((yyval.a_ref), p, 0);
		checkmem(ret == 0);
		free(p);
		free((yyvsp[0].tv_str));
	}
#line 4790 "asn1p_y.c"
    break;

  case 301: /* ComponentIdList: Identifier  */
#line 2214 "asn1p_y.y"
                   {
		(yyval.tv_str) = (yyvsp[0].tv_str);
	}
#line 4798 "asn1p_y.c"
    break;

  case 302: /* ComponentIdList: ComponentIdList '.' Identifier  */
#line 2217 "asn1p_y.y"
                                         {
		int l1 = strlen((yyvsp[-2].tv_str));
		int l3 = strlen((yyvsp[0].tv_str));
		(yyval.tv_str) = malloc(l1 + 1 + l3 + 1);
		memcpy((yyval.tv_str), (yyvsp[-2].tv_str), l1);
		(yyval.tv_str)[l1] = '.';
		memcpy((yyval.tv_str) + l1 + 1, (yyvsp[0].tv_str), l3);
		(yyval.tv_str)[l1 + 1 + l3] = '\0';
		free((yyvsp[-2].tv_str));
		free((yyvsp[0].tv_str));
	}
#line 4814 "asn1p_y.c"
    break;

  case 303: /* optMarker: %empty  */
#line 2237 "asn1p_y.y"
        {
		(yyval.a_marker).flags = EM_NOMARK;
		(yyval.a_marker).default_value = 0;
	}
#line 4823 "asn1p_y.c"
    break;

  case 304: /* optMarker: Marker  */
#line 2241 "asn1p_y.y"
                 { (yyval.a_marker) = (yyvsp[0].a_marker); }
#line 4829 "asn1p_y.c"
    break;

  case 305: /* Marker: TOK_OPTIONAL  */
#line 2245 "asn1p_y.y"
                     {
		(yyval.a_marker).flags = EM_OPTIONAL | EM_INDIRECT;
		(yyval.a_marker).default_value = 0;
	}
#line 4838 "asn1p_y.c"
    break;

  case 306: /* Marker: TOK_DEFAULT Value  */
#line 2249 "asn1p_y.y"
                            {
		(yyval.a_marker).flags = EM_DEFAULT;
		(yyval.a_marker).default_value = (yyvsp[0].a_value);
	}
#line 4847 "asn1p_y.c"
    break;

  case 307: /* IdentifierList: IdentifierElement  */
#line 2256 "asn1p_y.y"
                      {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		asn1p_expr_add((yyval.a_expr), (yyvsp[0].a_expr));
    }
#line 4857 "asn1p_y.c"
    break;

  case 308: /* IdentifierList: IdentifierList ',' IdentifierElement  */
#line 2261 "asn1p_y.y"
                                           {
		(yyval.a_expr) = (yyvsp[-2].a_expr);
		asn1p_expr_add((yyval.a_expr), (yyvsp[0].a_expr));
    }
#line 4866 "asn1p_y.c"
    break;

  case 309: /* IdentifierElement: Identifier  */
#line 2267 "asn1p_y.y"
               {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->expr_type = A1TC_UNIVERVAL;
		(yyval.a_expr)->meta_type = AMT_VALUE;
		(yyval.a_expr)->Identifier = (yyvsp[0].tv_str);
    }
#line 4878 "asn1p_y.c"
    break;

  case 310: /* NamedNumberList: NamedNumber  */
#line 2276 "asn1p_y.y"
                    {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		asn1p_expr_add((yyval.a_expr), (yyvsp[0].a_expr));
	}
#line 4888 "asn1p_y.c"
    break;

  case 311: /* NamedNumberList: NamedNumberList ',' NamedNumber  */
#line 2281 "asn1p_y.y"
                                          {
		(yyval.a_expr) = (yyvsp[-2].a_expr);
		asn1p_expr_add((yyval.a_expr), (yyvsp[0].a_expr));
	}
#line 4897 "asn1p_y.c"
    break;

  case 312: /* NamedNumber: Identifier '(' SignedNumber ')'  */
#line 2288 "asn1p_y.y"
                                        {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->expr_type = A1TC_UNIVERVAL;
		(yyval.a_expr)->meta_type = AMT_VALUE;
		(yyval.a_expr)->Identifier = (yyvsp[-3].tv_str);
		(yyval.a_expr)->value = (yyvsp[-1].a_value);
	}
#line 4910 "asn1p_y.c"
    break;

  case 313: /* NamedNumber: Identifier '(' DefinedValue ')'  */
#line 2296 "asn1p_y.y"
                                          {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->expr_type = A1TC_UNIVERVAL;
		(yyval.a_expr)->meta_type = AMT_VALUE;
		(yyval.a_expr)->Identifier = (yyvsp[-3].tv_str);
		(yyval.a_expr)->value = (yyvsp[-1].a_value);
	}
#line 4923 "asn1p_y.c"
    break;

  case 314: /* NamedBitList: NamedBit  */
#line 2306 "asn1p_y.y"
                 {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		asn1p_expr_add((yyval.a_expr), (yyvsp[0].a_expr));
	}
#line 4933 "asn1p_y.c"
    break;

  case 315: /* NamedBitList: NamedBitList ',' NamedBit  */
#line 2311 "asn1p_y.y"
                                    {
		(yyval.a_expr) = (yyvsp[-2].a_expr);
		asn1p_expr_add((yyval.a_expr), (yyvsp[0].a_expr));
	}
#line 4942 "asn1p_y.c"
    break;

  case 316: /* NamedBit: Identifier '(' "number" ')'  */
#line 2318 "asn1p_y.y"
                                      {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->expr_type = A1TC_UNIVERVAL;
		(yyval.a_expr)->meta_type = AMT_VALUE;
		(yyval.a_expr)->Identifier = (yyvsp[-3].tv_str);
		(yyval.a_expr)->value = asn1p_value_fromint((yyvsp[-1].a_int));
	}
#line 4955 "asn1p_y.c"
    break;

  case 317: /* NamedBit: Identifier '(' DefinedValue ')'  */
#line 2326 "asn1p_y.y"
                                          {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->expr_type = A1TC_UNIVERVAL;
		(yyval.a_expr)->meta_type = AMT_VALUE;
		(yyval.a_expr)->Identifier = (yyvsp[-3].tv_str);
		(yyval.a_expr)->value = (yyvsp[-1].a_value);
	}
#line 4968 "asn1p_y.c"
    break;

  case 318: /* Enumerations: UniverationList  */
#line 2336 "asn1p_y.y"
                    {
		(yyval.a_expr) = (yyvsp[0].a_expr);
        asn1p_expr_t *first_memb = TQ_FIRST(&((yyval.a_expr)->members));
        if(first_memb) {
            if(first_memb->expr_type == A1TC_EXTENSIBLE) {
                return yyerror(param,
                    "The ENUMERATION cannot start with extension (...).");
            }
        } else {
            return yyerror(param,
                "The ENUMERATION list cannot be empty.");
        }
    }
#line 4986 "asn1p_y.c"
    break;

  case 319: /* UniverationList: UniverationElement  */
#line 2351 "asn1p_y.y"
                           {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		asn1p_expr_add((yyval.a_expr), (yyvsp[0].a_expr));
	}
#line 4996 "asn1p_y.c"
    break;

  case 320: /* UniverationList: UniverationList ',' UniverationElement  */
#line 2356 "asn1p_y.y"
                                                 {
		(yyval.a_expr) = (yyvsp[-2].a_expr);
		asn1p_expr_add((yyval.a_expr), (yyvsp[0].a_expr));
	}
#line 5005 "asn1p_y.c"
    break;

  case 321: /* UniverationElement: Identifier  */
#line 2363 "asn1p_y.y"
                   {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->expr_type = A1TC_UNIVERVAL;
		(yyval.a_expr)->meta_type = AMT_VALUE;
		(yyval.a_expr)->Identifier = (yyvsp[0].tv_str);
	}
#line 5017 "asn1p_y.c"
    break;

  case 322: /* UniverationElement: Identifier '(' SignedNumber ')'  */
#line 2370 "asn1p_y.y"
                                          {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->expr_type = A1TC_UNIVERVAL;
		(yyval.a_expr)->meta_type = AMT_VALUE;
		(yyval.a_expr)->Identifier = (yyvsp[-3].tv_str);
		(yyval.a_expr)->value = (yyvsp[-1].a_value);
	}
#line 5030 "asn1p_y.c"
    break;

  case 323: /* UniverationElement: Identifier '(' DefinedValue ')'  */
#line 2378 "asn1p_y.y"
                                          {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->expr_type = A1TC_UNIVERVAL;
		(yyval.a_expr)->meta_type = AMT_VALUE;
		(yyval.a_expr)->Identifier = (yyvsp[-3].tv_str);
		(yyval.a_expr)->value = (yyvsp[-1].a_value);
	}
#line 5043 "asn1p_y.c"
    break;

  case 324: /* UniverationElement: SignedNumber  */
#line 2386 "asn1p_y.y"
                       {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->expr_type = A1TC_UNIVERVAL;
		(yyval.a_expr)->meta_type = AMT_VALUE;
		(yyval.a_expr)->value = (yyvsp[0].a_value);
	}
#line 5055 "asn1p_y.c"
    break;

  case 325: /* UniverationElement: "..."  */
#line 2393 "asn1p_y.y"
                        {
		(yyval.a_expr) = NEW_EXPR();
		checkmem((yyval.a_expr));
		(yyval.a_expr)->Identifier = strdup("...");
		checkmem((yyval.a_expr)->Identifier);
		(yyval.a_expr)->expr_type = A1TC_EXTENSIBLE;
		(yyval.a_expr)->meta_type = AMT_VALUE;
	}
#line 5068 "asn1p_y.c"
    break;

  case 326: /* SignedNumber: "number"  */
#line 2404 "asn1p_y.y"
                   {
		(yyval.a_value) = asn1p_value_fromint((yyvsp[0].a_int));
		checkmem((yyval.a_value));
	}
#line 5077 "asn1p_y.c"
    break;

  case 327: /* SignedNumber: "negative number"  */
#line 2408 "asn1p_y.y"
                              {
		(yyval.a_value) = asn1p_value_fromint((yyvsp[0].a_int));
		checkmem((yyval.a_value));
	}
#line 5086 "asn1p_y.c"
    break;

  case 328: /* RealValue: TOK_realnumber  */
#line 2415 "asn1p_y.y"
                       {
		(yyval.a_value) = asn1p_value_fromdouble((yyvsp[0].a_dbl));
		checkmem((yyval.a_value));
	}
#line 5095 "asn1p_y.c"
    break;

  case 329: /* optTag: %empty  */
#line 2446 "asn1p_y.y"
        { memset(&(yyval.a_tag), 0, sizeof((yyval.a_tag))); }
#line 5101 "asn1p_y.c"
    break;

  case 330: /* optTag: Tag  */
#line 2447 "asn1p_y.y"
              { (yyval.a_tag) = (yyvsp[0].a_tag); }
#line 5107 "asn1p_y.c"
    break;

  case 331: /* Tag: TagTypeValue TagPlicit  */
#line 2451 "asn1p_y.y"
                               {
		(yyval.a_tag) = (yyvsp[-1].a_tag);
		(yyval.a_tag).tag_mode = (yyvsp[0].a_tag).tag_mode;
	}
#line 5116 "asn1p_y.c"
    break;

  case 332: /* TagTypeValue: '[' TagClass "number" ']'  */
#line 2458 "asn1p_y.y"
                                    {
		(yyval.a_tag) = (yyvsp[-2].a_tag);
		(yyval.a_tag).tag_value = (yyvsp[-1].a_int);
	}
#line 5125 "asn1p_y.c"
    break;

  case 333: /* TagClass: %empty  */
#line 2464 "asn1p_y.y"
        { (yyval.a_tag).tag_class = TC_CONTEXT_SPECIFIC; }
#line 5131 "asn1p_y.c"
    break;

  case 334: /* TagClass: TOK_UNIVERSAL  */
#line 2465 "asn1p_y.y"
                        { (yyval.a_tag).tag_class = TC_UNIVERSAL; }
#line 5137 "asn1p_y.c"
    break;

  case 335: /* TagClass: TOK_APPLICATION  */
#line 2466 "asn1p_y.y"
                          { (yyval.a_tag).tag_class = TC_APPLICATION; }
#line 5143 "asn1p_y.c"
    break;

  case 336: /* TagClass: TOK_PRIVATE  */
#line 2467 "asn1p_y.y"
                      { (yyval.a_tag).tag_class = TC_PRIVATE; }
#line 5149 "asn1p_y.c"
    break;

  case 337: /* TagPlicit: %empty  */
#line 2471 "asn1p_y.y"
        { (yyval.a_tag).tag_mode = TM_DEFAULT; }
#line 5155 "asn1p_y.c"
    break;

  case 338: /* TagPlicit: TOK_IMPLICIT  */
#line 2472 "asn1p_y.y"
                       { (yyval.a_tag).tag_mode = TM_IMPLICIT; }
#line 5161 "asn1p_y.c"
    break;

  case 339: /* TagPlicit: TOK_EXPLICIT  */
#line 2473 "asn1p_y.y"
                       { (yyval.a_tag).tag_mode = TM_EXPLICIT; }
#line 5167 "asn1p_y.c"
    break;

  case 340: /* TypeRefName: TOK_typereference  */
#line 2477 "asn1p_y.y"
                          {
		checkmem((yyvsp[0].tv_str));
		(yyval.tv_str) = (yyvsp[0].tv_str);
	}
#line 5176 "asn1p_y.c"
    break;

  case 341: /* TypeRefName: TOK_capitalreference  */
#line 2481 "asn1p_y.y"
                               {
		checkmem((yyvsp[0].tv_str));
		(yyval.tv_str) = (yyvsp[0].tv_str);
	}
#line 5185 "asn1p_y.c"
    break;

  case 342: /* optIdentifier: %empty  */
#line 2489 "asn1p_y.y"
        { (yyval.tv_str) = 0; }
#line 5191 "asn1p_y.c"
    break;

  case 343: /* optIdentifier: Identifier  */
#line 2490 "asn1p_y.y"
                     {
		(yyval.tv_str) = (yyvsp[0].tv_str);
	}
#line 5199 "asn1p_y.c"
    break;

  case 344: /* Identifier: "identifier"  */
#line 2496 "asn1p_y.y"
                       {
		checkmem((yyvsp[0].tv_str));
		(yyval.tv_str) = (yyvsp[0].tv_str);
	}
#line 5208 "asn1p_y.c"
    break;

  case 345: /* IdentifierAsReference: Identifier  */
#line 2503 "asn1p_y.y"
               {
		(yyval.a_ref) = asn1p_ref_new(yylineno, currentModule);
		asn1p_ref_add_component((yyval.a_ref), (yyvsp[0].tv_str), RLT_lowercase);
		free((yyvsp[0].tv_str));
    }
#line 5218 "asn1p_y.c"
    break;

  case 346: /* IdentifierAsValue: IdentifierAsReference  */
#line 2510 "asn1p_y.y"
                          {
		(yyval.a_value) = asn1p_value_fromref((yyvsp[0].a_ref), 0);
    }
#line 5226 "asn1p_y.c"
    break;


#line 5230 "asn1p_y.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (param, YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, param);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, param);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (param, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, param);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, param);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 2514 "asn1p_y.y"



/*
 * Convert Xstring ('0101'B or '5'H) to the binary vector.
 */
static asn1p_value_t *
_convert_bitstring2binary(char *str, int base) {
	asn1p_value_t *val;
	int slen;
	int memlen;
	int baselen;
	int bits;
	uint8_t *binary_vector;
	uint8_t *bv_ptr;
	uint8_t cur_val;

	assert(str);
	assert(str[0] == '\'');

	switch(base) {
	case 'B':
		baselen = 1;
		break;
	case 'H':
		baselen = 4;
		break;
	default:
		assert(base == 'B' || base == 'H');
		errno = EINVAL;
		return NULL;
	}

	slen = strlen(str);
	assert(str[slen - 1] == base);
	assert(str[slen - 2] == '\'');

	memlen = slen / (8 / baselen);	/* Conservative estimate */

	bv_ptr = binary_vector = malloc(memlen + 1);
	if(bv_ptr == NULL) {
		/* ENOMEM */
		return NULL;
	}

	cur_val = 0;
	bits = 0;
	while(*(++str) != '\'') {
		switch(baselen) {
		case 1:
			switch(*str) {
			case '1':
				cur_val |= 1 << (7 - (bits % 8));
			case '0':
				break;
			default:
				assert(!"_y UNREACH1");
			case ' ': case '\r': case '\n':
				continue;
			}
			break;
		case 4:
			switch(*str) {
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				cur_val |= (*str - '0') << (4 - (bits % 8));
				break;
			case 'A': case 'B': case 'C':
			case 'D': case 'E': case 'F':
				cur_val |= ((*str - 'A') + 10)
					<< (4 - (bits % 8));
				break;
			default:
				assert(!"_y UNREACH2");
			case ' ': case '\r': case '\n':
				continue;
			}
			break;
		}

		bits += baselen;
		if((bits % 8) == 0) {
			*bv_ptr++ = cur_val;
			cur_val = 0;
		}
	}

	*bv_ptr = cur_val;
	assert((bv_ptr - binary_vector) <= memlen);

	val = asn1p_value_frombits(binary_vector, bits, 0);
	if(val == NULL) {
		free(binary_vector);
	}

	return val;
}

/*
 * For unnamed types (used in old X.208 compliant modules)
 * generate some sort of interim names, to not to force human being to fix
 * the specification's compliance to modern ASN.1 standards.
 */
static void
_fixup_anonymous_identifier(asn1p_expr_t *expr) {
	char *p;
	assert(expr->Identifier == 0);

	/*
	 * Try to figure out the type name
	 * without going too much into details
	 */
	expr->Identifier = ASN_EXPR_TYPE2STR(expr->expr_type);
	if(expr->reference && expr->reference->comp_count > 0)
		expr->Identifier = expr->reference->components[0].name;

	fprintf(stderr,
		"WARNING: Line %d: expected lower-case member identifier, "
		"found an unnamed %s.\n"
		"WARNING: Obsolete X.208 syntax detected, "
		"please give the member a name.\n",
		yylineno, expr->Identifier ? expr->Identifier : "type");

	if(!expr->Identifier)
		expr->Identifier = "unnamed";
	expr->Identifier = strdup(expr->Identifier);
	assert(expr->Identifier);
	/* Make a lowercase identifier from the type name */
	for(p = expr->Identifier; *p; p++) {
		switch(*p) {
		case 'A' ... 'Z': *p += 32; break;
		case ' ': *p = '_'; break;
		case '-': *p = '_'; break;
		}
	}
	fprintf(stderr, "NOTE: Assigning temporary identifier \"%s\". "
			"Name clash may occur later.\n",
		expr->Identifier);
}

int
yyerror(void **param, const char *msg) {
	(void)param;
	extern char *asn1p_text;
	fprintf(stderr,
		"ASN.1 grammar parse error "
		"near %s:%d (token \"%s\"): %s\n",
		ASN_FILENAME, yylineno, asn1p_text, msg);
	return -1;
}

