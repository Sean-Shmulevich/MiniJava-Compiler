/* original parser id follows */
/* yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93" */
/* (use YYMAJOR/YYMINOR for ifdefs dependent on parser version) */

#define YYBYACC 1
#define YYMAJOR 2
#define YYMINOR 0
#define YYPATCH 20240109

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)
#define YYENOMEM       (-2)
#define YYEOF          0
#undef YYBTYACC
#define YYBTYACC 0
#define YYDEBUGSTR YYPREFIX "debug"
#define YYPREFIX "yy"

#define YYPURE 0

#line 2 "grammar.y"
/** Code by @author Wonsun Ahn, Fall 2024
 * 
 * The parser implementation file.
 */

#include <stdlib.h>
#include <stdio.h>
#include "proj2.h"

extern int yylex();
static void yyerror(char *str);

tree SyntaxTree;
tree methodDeclType;
tree declType;

#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#line 20 "grammar.y"
typedef union YYSTYPE {
  int intg;
  tree tptr;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 53 "y.tab.c"

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

#if !(defined(yylex) || defined(YYSTATE))
int YYLEX_DECL();
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define IDnum 257
#define DECLARATIONSnum 258
#define ENDDECLARATIONSnum 259
#define EQUALnum 260
#define VOIDnum 261
#define INTnum 262
#define METHODnum 263
#define VALnum 264
#define DOTnum 265
#define ASSGNnum 266
#define RETURNnum 267
#define IFnum 268
#define ELSEnum 269
#define WHILEnum 270
#define LTnum 271
#define LEnum 272
#define EQnum 273
#define NEnum 274
#define GEnum 275
#define GTnum 276
#define PLUSnum 277
#define MINUSnum 278
#define ORnum 279
#define TIMESnum 280
#define DIVIDEnum 281
#define ANDnum 282
#define NOTnum 283
#define ICONSTnum 284
#define SCONSTnum 285
#define LPARENnum 286
#define CLASSnum 287
#define LBRACEnum 288
#define LBRACnum 289
#define COMMAnum 290
#define PROGRAMnum 291
#define SEMInum 292
#define RPARENnum 293
#define RBRACEnum 294
#define RBRACnum 295
#define YYERRCODE 256
typedef int YYINT;
static const YYINT yylhs[] = {                           -1,
   45,    0,   46,   35,   36,   36,   37,   37,   34,   34,
   34,   47,   48,   38,   39,   39,   42,   42,   40,   40,
   40,   44,   44,   43,   43,   41,   41,   41,   24,   24,
   25,   25,   32,   32,   32,   32,   32,   27,   28,   28,
   29,   29,   31,   30,   30,   30,   30,   33,   33,   23,
   23,   49,   18,   22,   22,   20,   20,   19,   19,   21,
   21,   10,   10,   26,   26,   13,   13,   12,   12,   11,
   11,   11,   17,   15,   15,   16,   14,   14,    1,    1,
    1,    1,    1,    1,    1,    2,    2,    2,    2,    9,
    9,    9,    3,    3,    3,    3,    4,    4,    4,    4,
    4,    5,    5,    6,    6,    6,    7,    7,    8,    8,
};
static const YYINT yylen[] = {                            2,
    0,    5,    0,    4,    1,    2,    3,    2,    2,    1,
    1,    0,    0,    9,    1,    1,    1,    2,    2,    4,
    0,    2,    1,    1,    3,    1,    2,    1,    3,    2,
    2,    3,    1,    1,    1,    1,    1,    3,    3,    4,
    1,    2,    3,    3,    5,    4,    6,    4,    5,    3,
    2,    0,    4,    1,    2,    3,    1,    1,    3,    1,
    3,    1,    1,    1,    3,    1,    2,    2,    3,    1,
    1,    1,    2,    3,    4,    3,    1,    3,    1,    3,
    3,    3,    3,    3,    3,    1,    3,    3,    3,    1,
    2,    2,    1,    3,    3,    3,    1,    1,    1,    3,
    2,    1,    1,    1,    2,    3,    3,    4,    1,    3,
};
static const YYINT yydefred[] = {                         0,
    0,    0,    1,    0,    0,    0,    5,    0,    3,    6,
    0,    0,    4,    0,    0,    8,    0,    0,   17,    0,
   63,   51,   62,    0,   54,    0,    0,   60,   16,    0,
   12,    0,    7,   18,    0,    0,    0,   50,   55,    0,
   65,   61,    0,    0,    0,   58,   13,    0,   67,    0,
    0,   53,    0,    0,  104,    0,    0,    0,    0,  102,
  103,    0,    0,   70,    0,   90,    0,   97,    0,   86,
   56,   71,   72,   99,   59,    0,   69,    0,   73,   91,
   92,  101,    0,   77,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  105,   23,    0,    0,    0,    0,  100,    0,   76,    0,
    0,    0,    0,    0,    0,   88,   89,   87,   94,   95,
   96,  106,   39,    0,    0,    0,   22,    0,    0,   24,
    0,    0,   78,    0,   40,  107,    0,    0,    0,   28,
   14,    0,   75,  110,  108,    0,    0,    0,    0,   30,
    0,    0,   33,   37,   34,   35,   36,    0,   27,   25,
   42,    0,    0,    0,   29,    0,   31,    0,   43,   38,
   32,    0,    0,    0,   45,    0,    0,    0,   47,   48,
    0,   49,
};
#if defined(YYDESTRUCT_CALL) || defined(YYSTYPE_TOSTRING)
static const YYINT yystos[] = {                           0,
  291,  297,  257,  342,  292,  287,  332,  333,  257,  332,
  343,  288,  334,  258,  263,  294,  320,  331,  335,  339,
  257,  259,  262,  307,  315,  318,  319,  323,  261,  318,
  336,  339,  294,  335,  265,  289,  346,  259,  315,  344,
  323,  295,  257,  310,  316,  317,  257,  289,  309,  260,
  290,  292,  345,  295,  257,  262,  277,  278,  283,  284,
  285,  286,  288,  298,  299,  300,  301,  302,  303,  306,
  308,  313,  314,  325,  317,  286,  309,  289,  312,  300,
  300,  301,  298,  308,  311,  271,  272,  273,  274,  275,
  276,  277,  278,  279,  280,  281,  282,  265,  286,  289,
  304,  262,  264,  337,  341,  298,  293,  290,  294,  299,
  299,  299,  299,  299,  299,  300,  300,  300,  300,  300,
  300,  257,  293,  298,  305,  298,  262,  292,  293,  257,
  340,  295,  308,  290,  293,  295,  341,  288,  320,  321,
  338,  290,  312,  305,  304,  340,  267,  268,  270,  294,
  303,  322,  324,  325,  326,  327,  328,  329,  321,  257,
  298,  298,  298,  266,  294,  329,  292,  321,  321,  298,
  292,  269,  330,  268,  321,  269,  298,  268,  321,  321,
  298,  321,
};
#endif /* YYDESTRUCT_CALL || YYSTYPE_TOSTRING */
static const YYINT yydgoto[] = {                          2,
   64,   65,   66,   67,   68,   69,  101,  125,   70,   24,
   71,   49,   44,   85,   79,   72,   73,   25,   45,   46,
   26,   27,   17,  140,  152,   28,  153,   74,  155,  156,
  157,  158,  173,   18,    7,    8,   13,   19,   31,  104,
  141,   20,  131,  105,    4,   11,   40,   53,   37,
};
static const YYINT yysindex[] = {                      -286,
 -191,    0,    0, -211, -173, -147,    0, -173,    0,    0,
 -159, -252,    0, -247, -186,    0, -101, -110,    0, -101,
    0,    0,    0,  -77,    0,  -93, -102,    0,    0,  -93,
    0, -101,    0,    0, -180, -109,  -55,    0,    0,  -47,
    0,    0,  -85,  -66, -155,    0,    0,  -83,    0, -189,
  -55,    0,  -68,  -85,    0,  -43, -193, -193, -193,    0,
    0, -177, -189,    0, -104,    0, -149,    0, -135,    0,
    0,    0,    0,    0,    0, -106,    0, -177,    0,    0,
    0,    0,  -73,    0, -216, -177, -177, -177, -177, -177,
 -177, -193, -193, -193, -193, -193, -193,  -31, -223, -177,
    0,    0,  -34, -140,  -12,  -48,    0, -189,    0, -174,
 -174, -174, -174, -174, -174,    0,    0,    0,    0,    0,
    0,    0,    0,  -42,  -41,  -46,    0, -106, -230,    0,
  -40,  -43,    0, -177,    0,    0,  -12, -250,  -37,    0,
    0,   -4,    0,    0,    0,  -40, -177, -177, -177,    0,
 -243, -238,    0,    0,    0,    0,    0,  -38,    0,    0,
    0,  -37,  -37, -177,    0,  -36,    0,  -14,    0,    0,
    0, -264,  -10, -177,    0, -209,  -37, -177,    0,    0,
  -37,    0,
};
static const YYINT yyrindex[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,  257,    0,    0,
    0,    0,    0,    0,    0,    0,  -33,    0,    0,  -32,
    0,    0,    0, -232,    0,    1,    0,    0,    0,    3,
    0,  -30,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, -259, -113,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -225,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -112,    0, -129,    0, -154,    0,
    0,    0,    0,    0,    0,  -53,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -103,
  -95,  -87,  -79,  -71,  -63,    0,    0,    0,    0,    0,
    0,    0,    0,  -28,    0,    0,    0,    0,    0,    0,
  -51, -179,    0,    0,    0,    0,    0,    0, -255,    0,
    0,    0,    0,    0,    0,  -49,  -29,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  -26,    0,    0,
    0,    0,  -25,    0,    0,    0,    0,    0,    0,    0,
    0,    0,
};
#if YYBTYACC
static const YYINT yycindex[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,
};
#endif
static const YYINT yygindex[] = {                         0,
  -62,  147,  -44,  209,    0,  -69,  133,  136,    0,    0,
  -61,  217,    0,    0,  140,    0,    0,  246,    0,  223,
  260,    0,  148, -136,    0,  241,    0,  -54,    0,    0,
    0,  126,    0,    0,  271,    0,    0,  -11,    0,    0,
    0,  263,  144,  154,    0,    0,    0,    0,    0,
};
#define YYTABLESIZE 282
static const YYINT yytable[] = {                         83,
   66,   84,  159,  174,    1,   14,   55,   26,   34,   21,
   15,   22,   80,   81,   23,  106,  147,  148,   55,  149,
   34,   98,  164,  138,   64,  168,  169,   14,  147,  148,
   66,  149,   66,   55,   68,  175,  124,  126,   26,  179,
  180,   16,   99,  150,  182,  100,  133,  116,  117,  118,
  119,  120,  121,   57,   58,  165,   64,  138,  178,   59,
   60,   61,   62,   55,   68,    3,   68,   55,  151,  123,
   21,  124,   56,  108,   29,   23,   21,  109,  138,   55,
    5,   23,  151,  154,  161,  162,  163,   57,   58,   59,
   60,   61,   62,   59,   60,   61,   62,  154,   63,   57,
   58,  170,   92,   93,   94,   59,   60,   61,   62,    9,
   74,  177,   74,    6,   74,  181,   98,   98,   98,   98,
   98,   98,   98,   98,   98,   98,   98,   98,   12,   98,
   95,   96,   97,   98,   51,   98,   52,   98,   98,   98,
   98,   93,   93,   93,   93,   93,   93,   93,   93,   93,
   99,  128,  129,  100,   21,  102,   38,  103,   93,   23,
   93,   15,   93,   93,   93,   93,   86,   87,   88,   89,
   90,   91,   92,   93,   94,   79,   57,   79,   57,   79,
   79,   79,   79,   33,   80,   42,   80,   35,   80,   80,
   80,   80,   81,   50,   81,   36,   81,   81,   81,   81,
   84,   43,   84,   48,   84,   84,   84,   84,   85,   47,
   85,   54,   85,   85,   85,   85,   83,   76,   83,  107,
   83,   83,   83,   83,   82,  122,   82,  127,   82,   82,
   82,   82,  110,  111,  112,  113,  114,  115,   21,   21,
   19,   19,   20,   20,  130,   78,  132,  134,  136,  142,
  138,  135,  160,  167,  172,  171,    2,   52,  176,   15,
   11,   10,   41,    9,  109,   44,   46,   82,  145,  144,
   77,  143,   39,   75,   30,   41,  139,  166,   10,   32,
  146,  137,
};
static const YYINT yycheck[] = {                         62,
  260,   63,  139,  268,  291,  258,  257,  263,   20,  257,
  263,  259,   57,   58,  262,   78,  267,  268,  257,  270,
   32,  265,  266,  288,  257,  162,  163,  258,  267,  268,
  290,  270,  292,  257,  260,  172,   99,  100,  294,  176,
  177,  294,  286,  294,  181,  289,  108,   92,   93,   94,
   95,   96,   97,  277,  278,  294,  289,  288,  268,  283,
  284,  285,  286,  257,  290,  257,  292,  257,  138,  293,
  257,  134,  262,  290,  261,  262,  257,  294,  288,  257,
  292,  262,  152,  138,  147,  148,  149,  277,  278,  283,
  284,  285,  286,  283,  284,  285,  286,  152,  288,  277,
  278,  164,  277,  278,  279,  283,  284,  285,  286,  257,
  290,  174,  292,  287,  294,  178,  271,  272,  273,  274,
  275,  276,  277,  278,  279,  280,  281,  282,  288,  265,
  280,  281,  282,  288,  290,  290,  292,  292,  293,  294,
  295,  271,  272,  273,  274,  275,  276,  277,  278,  279,
  286,  292,  293,  289,  257,  262,  259,  264,  288,  262,
  290,  263,  292,  293,  294,  295,  271,  272,  273,  274,
  275,  276,  277,  278,  279,  288,  290,  290,  292,  292,
  293,  294,  295,  294,  288,  295,  290,  265,  292,  293,
  294,  295,  288,  260,  290,  289,  292,  293,  294,  295,
  288,  257,  290,  289,  292,  293,  294,  295,  288,  257,
  290,  295,  292,  293,  294,  295,  288,  286,  290,  293,
  292,  293,  294,  295,  288,  257,  290,  262,  292,  293,
  294,  295,   86,   87,   88,   89,   90,   91,  292,  293,
  292,  293,  292,  293,  257,  289,  295,  290,  295,  290,
  288,  293,  257,  292,  269,  292,    0,  257,  269,  257,
  294,  294,  292,  294,  293,  292,  292,   59,  136,  134,
   54,  132,   27,   51,   15,   35,  129,  152,    8,   17,
  137,  128,
};
#if YYBTYACC
static const YYINT yyctable[] = {                        -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,
};
#endif
#define YYFINAL 2
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 295
#define YYUNDFTOKEN 347
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? YYUNDFTOKEN : (a))
#if YYDEBUG
static const char *const yyname[] = {

"$end",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"error","IDnum","DECLARATIONSnum",
"ENDDECLARATIONSnum","EQUALnum","VOIDnum","INTnum","METHODnum","VALnum",
"DOTnum","ASSGNnum","RETURNnum","IFnum","ELSEnum","WHILEnum","LTnum","LEnum",
"EQnum","NEnum","GEnum","GTnum","PLUSnum","MINUSnum","ORnum","TIMESnum",
"DIVIDEnum","ANDnum","NOTnum","ICONSTnum","SCONSTnum","LPARENnum","CLASSnum",
"LBRACEnum","LBRACnum","COMMAnum","PROGRAMnum","SEMInum","RPARENnum",
"RBRACEnum","RBRACnum","$accept","Program","Expression","SimpleExpression",
"Term","Factor","UnsignedConstant","Variable","ExprListBrac","ExprList",
"SignedTerm","TypeKind","VariableInitializer","EmptyBracList","VariableDeclId",
"VarInitList","ArrInitList","ArrayInitializer","ArrayCreationExpression",
"FieldDecl","VarList","VarEq","Type","FieldDeclList","Decls","StatementList",
"StatementListHelper","TypeIndexed","AssignmentStatement","MethodCallStatement",
"ReturnStatement","IfStatement","WhileStatement","Statement","ElseIfList",
"Class_op","ClassDecl","ClassDeclList","ClassBody","MethodDecl",
"MethodDeclTypeHelper","FormalParameterList","Block","MethodDeclList","IdList",
"FormalParameterListTypeHelper","$$1","$$2","$$3","$$4","$$5","illegal-symbol",
};
static const char *const yyrule[] = {
"$accept : Program",
"$$1 :",
"Program : PROGRAMnum IDnum $$1 SEMInum ClassDeclList",
"$$2 :",
"ClassDecl : CLASSnum IDnum $$2 ClassBody",
"ClassDeclList : ClassDecl",
"ClassDeclList : ClassDeclList ClassDecl",
"ClassBody : LBRACEnum Class_op RBRACEnum",
"ClassBody : LBRACEnum RBRACEnum",
"Class_op : Decls MethodDeclList",
"Class_op : MethodDeclList",
"Class_op : Decls",
"$$3 :",
"$$4 :",
"MethodDecl : METHODnum MethodDeclTypeHelper $$3 IDnum $$4 LPARENnum FormalParameterList RPARENnum Block",
"MethodDeclTypeHelper : Type",
"MethodDeclTypeHelper : VOIDnum",
"MethodDeclList : MethodDecl",
"MethodDeclList : MethodDeclList MethodDecl",
"FormalParameterList : FormalParameterListTypeHelper IdList",
"FormalParameterList : FormalParameterList SEMInum FormalParameterListTypeHelper IdList",
"FormalParameterList :",
"FormalParameterListTypeHelper : VALnum INTnum",
"FormalParameterListTypeHelper : INTnum",
"IdList : IDnum",
"IdList : IdList COMMAnum IDnum",
"Block : Decls",
"Block : Decls StatementList",
"Block : StatementList",
"StatementList : LBRACEnum StatementListHelper RBRACEnum",
"StatementList : LBRACEnum RBRACEnum",
"StatementListHelper : Statement SEMInum",
"StatementListHelper : StatementListHelper Statement SEMInum",
"Statement : AssignmentStatement",
"Statement : ReturnStatement",
"Statement : IfStatement",
"Statement : WhileStatement",
"Statement : MethodCallStatement",
"AssignmentStatement : Variable ASSGNnum Expression",
"MethodCallStatement : Variable LPARENnum RPARENnum",
"MethodCallStatement : Variable LPARENnum ExprList RPARENnum",
"ReturnStatement : RETURNnum",
"ReturnStatement : RETURNnum Expression",
"WhileStatement : WHILEnum Expression StatementList",
"IfStatement : IFnum Expression StatementList",
"IfStatement : IFnum Expression StatementList ELSEnum StatementList",
"IfStatement : IFnum Expression StatementList ElseIfList",
"IfStatement : IFnum Expression StatementList ElseIfList ELSEnum StatementList",
"ElseIfList : ELSEnum IFnum Expression StatementList",
"ElseIfList : ElseIfList ELSEnum IFnum Expression StatementList",
"Decls : DECLARATIONSnum FieldDeclList ENDDECLARATIONSnum",
"Decls : DECLARATIONSnum ENDDECLARATIONSnum",
"$$5 :",
"FieldDecl : Type $$5 VarList SEMInum",
"FieldDeclList : FieldDecl",
"FieldDeclList : FieldDeclList FieldDecl",
"VarEq : VariableDeclId EQUALnum VariableInitializer",
"VarEq : VariableDeclId",
"VarList : VarEq",
"VarList : VarList COMMAnum VarEq",
"Type : TypeIndexed",
"Type : Type LBRACnum RBRACnum",
"TypeKind : INTnum",
"TypeKind : IDnum",
"TypeIndexed : TypeKind",
"TypeIndexed : TypeKind DOTnum TypeIndexed",
"VariableDeclId : IDnum",
"VariableDeclId : IDnum EmptyBracList",
"EmptyBracList : LBRACnum RBRACnum",
"EmptyBracList : LBRACnum RBRACnum EmptyBracList",
"VariableInitializer : Expression",
"VariableInitializer : ArrayInitializer",
"VariableInitializer : ArrayCreationExpression",
"ArrayCreationExpression : INTnum ArrInitList",
"ArrInitList : LBRACnum Expression RBRACnum",
"ArrInitList : LBRACnum Expression RBRACnum ArrInitList",
"ArrayInitializer : LBRACEnum VarInitList RBRACEnum",
"VarInitList : VariableInitializer",
"VarInitList : VarInitList COMMAnum VariableInitializer",
"Expression : SimpleExpression",
"Expression : SimpleExpression LTnum SimpleExpression",
"Expression : SimpleExpression LEnum SimpleExpression",
"Expression : SimpleExpression GTnum SimpleExpression",
"Expression : SimpleExpression GEnum SimpleExpression",
"Expression : SimpleExpression EQnum SimpleExpression",
"Expression : SimpleExpression NEnum SimpleExpression",
"SimpleExpression : SignedTerm",
"SimpleExpression : SimpleExpression ORnum Term",
"SimpleExpression : SimpleExpression PLUSnum Term",
"SimpleExpression : SimpleExpression MINUSnum Term",
"SignedTerm : Term",
"SignedTerm : PLUSnum Term",
"SignedTerm : MINUSnum Term",
"Term : Factor",
"Term : Factor TIMESnum Term",
"Term : Factor DIVIDEnum Term",
"Term : Factor ANDnum Term",
"Factor : UnsignedConstant",
"Factor : Variable",
"Factor : MethodCallStatement",
"Factor : LPARENnum Expression RPARENnum",
"Factor : NOTnum Factor",
"UnsignedConstant : ICONSTnum",
"UnsignedConstant : SCONSTnum",
"Variable : IDnum",
"Variable : Variable ExprListBrac",
"Variable : Variable DOTnum IDnum",
"ExprListBrac : LBRACnum Expression RBRACnum",
"ExprListBrac : LBRACnum Expression RBRACnum ExprListBrac",
"ExprList : Expression",
"ExprList : Expression COMMAnum ExprList",

};
#endif

#if YYDEBUG
int      yydebug;
#endif

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;
int      yynerrs;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
YYLTYPE  yyloc; /* position returned by actions */
YYLTYPE  yylloc; /* position from the lexer */
#endif

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
#ifndef YYLLOC_DEFAULT
#define YYLLOC_DEFAULT(loc, rhs, n) \
do \
{ \
    if (n == 0) \
    { \
        (loc).first_line   = YYRHSLOC(rhs, 0).last_line; \
        (loc).first_column = YYRHSLOC(rhs, 0).last_column; \
        (loc).last_line    = YYRHSLOC(rhs, 0).last_line; \
        (loc).last_column  = YYRHSLOC(rhs, 0).last_column; \
    } \
    else \
    { \
        (loc).first_line   = YYRHSLOC(rhs, 1).first_line; \
        (loc).first_column = YYRHSLOC(rhs, 1).first_column; \
        (loc).last_line    = YYRHSLOC(rhs, n).last_line; \
        (loc).last_column  = YYRHSLOC(rhs, n).last_column; \
    } \
} while (0)
#endif /* YYLLOC_DEFAULT */
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#if YYBTYACC

#ifndef YYLVQUEUEGROWTH
#define YYLVQUEUEGROWTH 32
#endif
#endif /* YYBTYACC */

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#ifndef YYINITSTACKSIZE
#define YYINITSTACKSIZE 200
#endif

typedef struct {
    unsigned stacksize;
    YYINT    *s_base;
    YYINT    *s_mark;
    YYINT    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  *p_base;
    YYLTYPE  *p_mark;
#endif
} YYSTACKDATA;
#if YYBTYACC

struct YYParseState_s
{
    struct YYParseState_s *save;    /* Previously saved parser state */
    YYSTACKDATA            yystack; /* saved parser stack */
    int                    state;   /* saved parser state */
    int                    errflag; /* saved error recovery status */
    int                    lexeme;  /* saved index of the conflict lexeme in the lexical queue */
    YYINT                  ctry;    /* saved index in yyctable[] for this conflict */
};
typedef struct YYParseState_s YYParseState;
#endif /* YYBTYACC */
/* variables for the parser stack */
static YYSTACKDATA yystack;
#if YYBTYACC

/* Current parser state */
static YYParseState *yyps = 0;

/* yypath != NULL: do the full parse, starting at *yypath parser state. */
static YYParseState *yypath = 0;

/* Base of the lexical value queue */
static YYSTYPE *yylvals = 0;

/* Current position at lexical value queue */
static YYSTYPE *yylvp = 0;

/* End position of lexical value queue */
static YYSTYPE *yylve = 0;

/* The last allocated position at the lexical value queue */
static YYSTYPE *yylvlim = 0;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
/* Base of the lexical position queue */
static YYLTYPE *yylpsns = 0;

/* Current position at lexical position queue */
static YYLTYPE *yylpp = 0;

/* End position of lexical position queue */
static YYLTYPE *yylpe = 0;

/* The last allocated position at the lexical position queue */
static YYLTYPE *yylplim = 0;
#endif

/* Current position at lexical token queue */
static YYINT  *yylexp = 0;

static YYINT  *yylexemes = 0;
#endif /* YYBTYACC */
#line 512 "grammar.y"

int yycolumn = 1, yyline = 1;
void yyerror(char *str)
{
  printf("yyerror: %s at line %d\n", str, yyline);
  exit(0);
}

#include "lex.yy.c"
#line 662 "y.tab.c"

/* For use in generated program */
#define yydepth (int)(yystack.s_mark - yystack.s_base)
#if YYBTYACC
#define yytrial (yyps->save)
#endif /* YYBTYACC */

#if YYDEBUG
#include <stdio.h>	/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    YYINT *newss;
    YYSTYPE *newvs;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE *newps;
#endif

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return YYENOMEM;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (YYINT *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return YYENOMEM;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return YYENOMEM;

    data->l_base = newvs;
    data->l_mark = newvs + i;

#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    newps = (YYLTYPE *)realloc(data->p_base, newsize * sizeof(*newps));
    if (newps == 0)
        return YYENOMEM;

    data->p_base = newps;
    data->p_mark = newps + i;
#endif

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;

#if YYDEBUG
    if (yydebug)
        fprintf(stderr, "%sdebug: stack size increased to %d\n", YYPREFIX, newsize);
#endif
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    free(data->p_base);
#endif
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif /* YYPURE || defined(YY_NO_LEAKS) */
#if YYBTYACC

static YYParseState *
yyNewState(unsigned size)
{
    YYParseState *p = (YYParseState *) malloc(sizeof(YYParseState));
    if (p == NULL) return NULL;

    p->yystack.stacksize = size;
    if (size == 0)
    {
        p->yystack.s_base = NULL;
        p->yystack.l_base = NULL;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        p->yystack.p_base = NULL;
#endif
        return p;
    }
    p->yystack.s_base    = (YYINT *) malloc(size * sizeof(YYINT));
    if (p->yystack.s_base == NULL) return NULL;
    p->yystack.l_base    = (YYSTYPE *) malloc(size * sizeof(YYSTYPE));
    if (p->yystack.l_base == NULL) return NULL;
    memset(p->yystack.l_base, 0, size * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    p->yystack.p_base    = (YYLTYPE *) malloc(size * sizeof(YYLTYPE));
    if (p->yystack.p_base == NULL) return NULL;
    memset(p->yystack.p_base, 0, size * sizeof(YYLTYPE));
#endif

    return p;
}

static void
yyFreeState(YYParseState *p)
{
    yyfreestack(&p->yystack);
    free(p);
}
#endif /* YYBTYACC */

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab
#if YYBTYACC
#define YYVALID        do { if (yyps->save)            goto yyvalid; } while(0)
#define YYVALID_NESTED do { if (yyps->save && \
                                yyps->save->save == 0) goto yyvalid; } while(0)
#endif /* YYBTYACC */

int
YYPARSE_DECL()
{
    int yym, yyn, yystate, yyresult;
#if YYBTYACC
    int yynewerrflag;
    YYParseState *yyerrctx = NULL;
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    YYLTYPE  yyerror_loc_range[3]; /* position of error start/end (0 unused) */
#endif
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
    if (yydebug)
        fprintf(stderr, "%sdebug[<# of symbols on state stack>]\n", YYPREFIX);
#endif
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    memset(yyerror_loc_range, 0, sizeof(yyerror_loc_range));
#endif

#if YYBTYACC
    yyps = yyNewState(0); if (yyps == 0) goto yyenomem;
    yyps->save = 0;
#endif /* YYBTYACC */
    yym = 0;
    /* yyn is set below */
    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark = yystack.p_base;
#endif
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
#if YYBTYACC
        do {
        if (yylvp < yylve)
        {
            /* we're currently re-reading tokens */
            yylval = *yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylloc = *yylpp++;
#endif
            yychar = *yylexp++;
            break;
        }
        if (yyps->save)
        {
            /* in trial mode; save scanner results for future parse attempts */
            if (yylvp == yylvlim)
            {   /* Enlarge lexical value queue */
                size_t p = (size_t) (yylvp - yylvals);
                size_t s = (size_t) (yylvlim - yylvals);

                s += YYLVQUEUEGROWTH;
                if ((yylexemes = (YYINT *)realloc(yylexemes, s * sizeof(YYINT))) == NULL) goto yyenomem;
                if ((yylvals   = (YYSTYPE *)realloc(yylvals, s * sizeof(YYSTYPE))) == NULL) goto yyenomem;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                if ((yylpsns   = (YYLTYPE *)realloc(yylpsns, s * sizeof(YYLTYPE))) == NULL) goto yyenomem;
#endif
                yylvp   = yylve = yylvals + p;
                yylvlim = yylvals + s;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp   = yylpe = yylpsns + p;
                yylplim = yylpsns + s;
#endif
                yylexp  = yylexemes + p;
            }
            *yylexp = (YYINT) YYLEX;
            *yylvp++ = yylval;
            yylve++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            *yylpp++ = yylloc;
            yylpe++;
#endif
            yychar = *yylexp++;
            break;
        }
        /* normal operation, no conflict encountered */
#endif /* YYBTYACC */
        yychar = YYLEX;
#if YYBTYACC
        } while (0);
#endif /* YYBTYACC */
        if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            fprintf(stderr, "%s[%d]: state %d, reading token %d (%s)",
                            YYDEBUGSTR, yydepth, yystate, yychar, yys);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
            if (!yytrial)
#endif /* YYBTYACC */
                fprintf(stderr, " <%s>", YYSTYPE_TOSTRING(yychar, yylval));
#endif
            fputc('\n', stderr);
        }
#endif
    }
#if YYBTYACC

    /* Do we have a conflict? */
    if (((yyn = yycindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
        yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        YYINT ctry;

        if (yypath)
        {
            YYParseState *save;
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%s[%d]: CONFLICT in state %d: following successful trial parse\n",
                                YYDEBUGSTR, yydepth, yystate);
#endif
            /* Switch to the next conflict context */
            save = yypath;
            yypath = save->save;
            save->save = NULL;
            ctry = save->ctry;
            if (save->state != yystate) YYABORT;
            yyFreeState(save);

        }
        else
        {

            /* Unresolved conflict - start/continue trial parse */
            YYParseState *save;
#if YYDEBUG
            if (yydebug)
            {
                fprintf(stderr, "%s[%d]: CONFLICT in state %d. ", YYDEBUGSTR, yydepth, yystate);
                if (yyps->save)
                    fputs("ALREADY in conflict, continuing trial parse.\n", stderr);
                else
                    fputs("Starting trial parse.\n", stderr);
            }
#endif
            save                  = yyNewState((unsigned)(yystack.s_mark - yystack.s_base + 1));
            if (save == NULL) goto yyenomem;
            save->save            = yyps->save;
            save->state           = yystate;
            save->errflag         = yyerrflag;
            save->yystack.s_mark  = save->yystack.s_base + (yystack.s_mark - yystack.s_base);
            memcpy (save->yystack.s_base, yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            save->yystack.l_mark  = save->yystack.l_base + (yystack.l_mark - yystack.l_base);
            memcpy (save->yystack.l_base, yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            save->yystack.p_mark  = save->yystack.p_base + (yystack.p_mark - yystack.p_base);
            memcpy (save->yystack.p_base, yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            ctry                  = yytable[yyn];
            if (yyctable[ctry] == -1)
            {
#if YYDEBUG
                if (yydebug && yychar >= YYEOF)
                    fprintf(stderr, "%s[%d]: backtracking 1 token\n", YYDEBUGSTR, yydepth);
#endif
                ctry++;
            }
            save->ctry = ctry;
            if (yyps->save == NULL)
            {
                /* If this is a first conflict in the stack, start saving lexemes */
                if (!yylexemes)
                {
                    yylexemes = (YYINT *) malloc((YYLVQUEUEGROWTH) * sizeof(YYINT));
                    if (yylexemes == NULL) goto yyenomem;
                    yylvals   = (YYSTYPE *) malloc((YYLVQUEUEGROWTH) * sizeof(YYSTYPE));
                    if (yylvals == NULL) goto yyenomem;
                    yylvlim   = yylvals + YYLVQUEUEGROWTH;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpsns   = (YYLTYPE *) malloc((YYLVQUEUEGROWTH) * sizeof(YYLTYPE));
                    if (yylpsns == NULL) goto yyenomem;
                    yylplim   = yylpsns + YYLVQUEUEGROWTH;
#endif
                }
                if (yylvp == yylve)
                {
                    yylvp  = yylve = yylvals;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpp  = yylpe = yylpsns;
#endif
                    yylexp = yylexemes;
                    if (yychar >= YYEOF)
                    {
                        *yylve++ = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                        *yylpe++ = yylloc;
#endif
                        *yylexp  = (YYINT) yychar;
                        yychar   = YYEMPTY;
                    }
                }
            }
            if (yychar >= YYEOF)
            {
                yylvp--;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp--;
#endif
                yylexp--;
                yychar = YYEMPTY;
            }
            save->lexeme = (int) (yylvp - yylvals);
            yyps->save   = save;
        }
        if (yytable[yyn] == ctry)
        {
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%s[%d]: state %d, shifting to state %d\n",
                                YYDEBUGSTR, yydepth, yystate, yyctable[ctry]);
#endif
            if (yychar < 0)
            {
                yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylpp++;
#endif
                yylexp++;
            }
            if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM)
                goto yyoverflow;
            yystate = yyctable[ctry];
            *++yystack.s_mark = (YYINT) yystate;
            *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            *++yystack.p_mark = yylloc;
#endif
            yychar  = YYEMPTY;
            if (yyerrflag > 0) --yyerrflag;
            goto yyloop;
        }
        else
        {
            yyn = yyctable[ctry];
            goto yyreduce;
        }
    } /* End of code dealing with conflicts */
#endif /* YYBTYACC */
    if (((yyn = yysindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
#if YYDEBUG
        if (yydebug)
            fprintf(stderr, "%s[%d]: state %d, shifting to state %d\n",
                            YYDEBUGSTR, yydepth, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        *++yystack.p_mark = yylloc;
#endif
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if (((yyn = yyrindex[yystate]) != 0) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag != 0) goto yyinrecovery;
#if YYBTYACC

    yynewerrflag = 1;
    goto yyerrhandler;
    goto yyerrlab; /* redundant goto avoids 'unused label' warning */

yyerrlab:
    /* explicit YYERROR from an action -- pop the rhs of the rule reduced
     * before looking for error recovery */
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark -= yym;
#endif

    yynewerrflag = 0;
yyerrhandler:
    while (yyps->save)
    {
        int ctry;
        YYParseState *save = yyps->save;
#if YYDEBUG
        if (yydebug)
            fprintf(stderr, "%s[%d]: ERROR in state %d, CONFLICT BACKTRACKING to state %d, %d tokens\n",
                            YYDEBUGSTR, yydepth, yystate, yyps->save->state,
                    (int)(yylvp - yylvals - yyps->save->lexeme));
#endif
        /* Memorize most forward-looking error state in case it's really an error. */
        if (yyerrctx == NULL || yyerrctx->lexeme < yylvp - yylvals)
        {
            /* Free old saved error context state */
            if (yyerrctx) yyFreeState(yyerrctx);
            /* Create and fill out new saved error context state */
            yyerrctx                 = yyNewState((unsigned)(yystack.s_mark - yystack.s_base + 1));
            if (yyerrctx == NULL) goto yyenomem;
            yyerrctx->save           = yyps->save;
            yyerrctx->state          = yystate;
            yyerrctx->errflag        = yyerrflag;
            yyerrctx->yystack.s_mark = yyerrctx->yystack.s_base + (yystack.s_mark - yystack.s_base);
            memcpy (yyerrctx->yystack.s_base, yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            yyerrctx->yystack.l_mark = yyerrctx->yystack.l_base + (yystack.l_mark - yystack.l_base);
            memcpy (yyerrctx->yystack.l_base, yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yyerrctx->yystack.p_mark = yyerrctx->yystack.p_base + (yystack.p_mark - yystack.p_base);
            memcpy (yyerrctx->yystack.p_base, yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            yyerrctx->lexeme         = (int) (yylvp - yylvals);
        }
        yylvp          = yylvals   + save->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        yylpp          = yylpsns   + save->lexeme;
#endif
        yylexp         = yylexemes + save->lexeme;
        yychar         = YYEMPTY;
        yystack.s_mark = yystack.s_base + (save->yystack.s_mark - save->yystack.s_base);
        memcpy (yystack.s_base, save->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
        yystack.l_mark = yystack.l_base + (save->yystack.l_mark - save->yystack.l_base);
        memcpy (yystack.l_base, save->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        yystack.p_mark = yystack.p_base + (save->yystack.p_mark - save->yystack.p_base);
        memcpy (yystack.p_base, save->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
        ctry           = ++save->ctry;
        yystate        = save->state;
        /* We tried shift, try reduce now */
        if ((yyn = yyctable[ctry]) >= 0) goto yyreduce;
        yyps->save     = save->save;
        save->save     = NULL;
        yyFreeState(save);

        /* Nothing left on the stack -- error */
        if (!yyps->save)
        {
#if YYDEBUG
            if (yydebug)
                fprintf(stderr, "%sdebug[%d,trial]: trial parse FAILED, entering ERROR mode\n",
                                YYPREFIX, yydepth);
#endif
            /* Restore state as it was in the most forward-advanced error */
            yylvp          = yylvals   + yyerrctx->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylpp          = yylpsns   + yyerrctx->lexeme;
#endif
            yylexp         = yylexemes + yyerrctx->lexeme;
            yychar         = yylexp[-1];
            yylval         = yylvp[-1];
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yylloc         = yylpp[-1];
#endif
            yystack.s_mark = yystack.s_base + (yyerrctx->yystack.s_mark - yyerrctx->yystack.s_base);
            memcpy (yystack.s_base, yyerrctx->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
            yystack.l_mark = yystack.l_base + (yyerrctx->yystack.l_mark - yyerrctx->yystack.l_base);
            memcpy (yystack.l_base, yyerrctx->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            yystack.p_mark = yystack.p_base + (yyerrctx->yystack.p_mark - yyerrctx->yystack.p_base);
            memcpy (yystack.p_base, yyerrctx->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
            yystate        = yyerrctx->state;
            yyFreeState(yyerrctx);
            yyerrctx       = NULL;
        }
        yynewerrflag = 1;
    }
    if (yynewerrflag == 0) goto yyinrecovery;
#endif /* YYBTYACC */

    YYERROR_CALL("syntax error");
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yyerror_loc_range[1] = yylloc; /* lookahead position is error start position */
#endif

#if !YYBTYACC
    goto yyerrlab; /* redundant goto avoids 'unused label' warning */
yyerrlab:
#endif
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if (((yyn = yysindex[*yystack.s_mark]) != 0) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    fprintf(stderr, "%s[%d]: state %d, error recovery shifting to state %d\n",
                                    YYDEBUGSTR, yydepth, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                /* lookahead position is error end position */
                yyerror_loc_range[2] = yylloc;
                YYLLOC_DEFAULT(yyloc, yyerror_loc_range, 2); /* position of error span */
                *++yystack.p_mark = yyloc;
#endif
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    fprintf(stderr, "%s[%d]: error recovery discarding state %d\n",
                                    YYDEBUGSTR, yydepth, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                /* the current TOS position is the error start position */
                yyerror_loc_range[1] = *yystack.p_mark;
#endif
#if defined(YYDESTRUCT_CALL)
#if YYBTYACC
                if (!yytrial)
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    YYDESTRUCT_CALL("error: discarding state",
                                    yystos[*yystack.s_mark], yystack.l_mark, yystack.p_mark);
#else
                    YYDESTRUCT_CALL("error: discarding state",
                                    yystos[*yystack.s_mark], yystack.l_mark);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#endif /* defined(YYDESTRUCT_CALL) */
                --yystack.s_mark;
                --yystack.l_mark;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                --yystack.p_mark;
#endif
            }
        }
    }
    else
    {
        if (yychar == YYEOF) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
            fprintf(stderr, "%s[%d]: state %d, error recovery discarding token %d (%s)\n",
                            YYDEBUGSTR, yydepth, yystate, yychar, yys);
        }
#endif
#if defined(YYDESTRUCT_CALL)
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
            YYDESTRUCT_CALL("error: discarding token", yychar, &yylval, &yylloc);
#else
            YYDESTRUCT_CALL("error: discarding token", yychar, &yylval);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
#endif /* defined(YYDESTRUCT_CALL) */
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
    yym = yylen[yyn];
#if YYDEBUG
    if (yydebug)
    {
        fprintf(stderr, "%s[%d]: state %d, reducing by rule %d (%s)",
                        YYDEBUGSTR, yydepth, yystate, yyn, yyrule[yyn]);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
            if (yym > 0)
            {
                int i;
                fputc('<', stderr);
                for (i = yym; i > 0; i--)
                {
                    if (i != yym) fputs(", ", stderr);
                    fputs(YYSTYPE_TOSTRING(yystos[yystack.s_mark[1-i]],
                                           yystack.l_mark[1-i]), stderr);
                }
                fputc('>', stderr);
            }
#endif
        fputc('\n', stderr);
    }
#endif
    if (yym > 0)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)

    /* Perform position reduction */
    memset(&yyloc, 0, sizeof(yyloc));
#if YYBTYACC
    if (!yytrial)
#endif /* YYBTYACC */
    {
        YYLLOC_DEFAULT(yyloc, &yystack.p_mark[-yym], yym);
        /* just in case YYERROR is invoked within the action, save
           the start of the rhs as the error start position */
        yyerror_loc_range[1] = yystack.p_mark[1-yym];
    }
#endif

    switch (yyn)
    {
case 1:
#line 59 "grammar.y"
	{ yystack.l_mark[-3].tptr = MakeLeaf(IDNode, yystack.l_mark[0].intg); }
#line 1335 "y.tab.c"
break;
case 2:
#line 59 "grammar.y"
	{yyval.tptr = MakeTree(ProgramOp, yystack.l_mark[0].tptr, yystack.l_mark[-6].tptr); SyntaxTree = yyval.tptr; }
#line 1340 "y.tab.c"
break;
case 3:
#line 63 "grammar.y"
	{ yystack.l_mark[-4].tptr = MakeLeaf(IDNode, yystack.l_mark[0].intg); }
#line 1345 "y.tab.c"
break;
case 4:
#line 63 "grammar.y"
	{yyval.tptr = MakeTree(ClassDefOp, yystack.l_mark[0].tptr, yystack.l_mark[-6].tptr);}
#line 1350 "y.tab.c"
break;
case 5:
#line 66 "grammar.y"
	{yyval.tptr = MakeTree(ClassOp, NullExp(), yystack.l_mark[0].tptr);}
#line 1355 "y.tab.c"
break;
case 6:
#line 67 "grammar.y"
	{yyval.tptr = MakeTree(ClassOp, yystack.l_mark[-1].tptr, yystack.l_mark[0].tptr);}
#line 1360 "y.tab.c"
break;
case 7:
#line 70 "grammar.y"
	{yyval.tptr = yystack.l_mark[-1].tptr;}
#line 1365 "y.tab.c"
break;
case 8:
#line 71 "grammar.y"
	{yyval.tptr = NullExp();}
#line 1370 "y.tab.c"
break;
case 9:
#line 74 "grammar.y"
	{
      /* both of the right subtrees are meant for */
      yyval.tptr = MkLeftC(yystack.l_mark[-1].tptr, yystack.l_mark[0].tptr); /* make decls, the end of the methodDeclList.*/
  }
#line 1378 "y.tab.c"
break;
case 10:
#line 78 "grammar.y"
	{
      /* decls is at the end, add body op to end of other body op's, decls is the last one.*/
      yyval.tptr = yystack.l_mark[0].tptr;
  }
#line 1386 "y.tab.c"
break;
case 11:
#line 82 "grammar.y"
	{
      yyval.tptr = yystack.l_mark[0].tptr;
  }
#line 1393 "y.tab.c"
break;
case 12:
#line 87 "grammar.y"
	{methodDeclType = yystack.l_mark[0].tptr;}
#line 1398 "y.tab.c"
break;
case 13:
#line 87 "grammar.y"
	{yystack.l_mark[-7].tptr = MakeLeaf(IDNode, yystack.l_mark[0].intg);}
#line 1403 "y.tab.c"
break;
case 14:
#line 87 "grammar.y"
	{
    yyval.tptr = MakeTree(MethodOp, MakeTree(HeadOp, yystack.l_mark[-12].tptr, yystack.l_mark[-2].tptr), yystack.l_mark[0].tptr);
  }
#line 1410 "y.tab.c"
break;
case 15:
#line 96 "grammar.y"
	{yyval.tptr = yystack.l_mark[0].tptr;}
#line 1415 "y.tab.c"
break;
case 16:
#line 97 "grammar.y"
	{yyval.tptr = NullExp();}
#line 1420 "y.tab.c"
break;
case 17:
#line 101 "grammar.y"
	{ yyval.tptr = MakeTree(BodyOp, NullExp(), yystack.l_mark[0].tptr); }
#line 1425 "y.tab.c"
break;
case 18:
#line 102 "grammar.y"
	{ yyval.tptr = MakeTree(BodyOp, yystack.l_mark[-1].tptr, yystack.l_mark[0].tptr); }
#line 1430 "y.tab.c"
break;
case 19:
#line 106 "grammar.y"
	{yyval.tptr = MakeTree(SpecOp, yystack.l_mark[0].tptr, methodDeclType);}
#line 1435 "y.tab.c"
break;
case 20:
#line 107 "grammar.y"
	{ MkRightC(yystack.l_mark[0].tptr, LeftChild(yystack.l_mark[-3].tptr)); yyval.tptr = yystack.l_mark[-3].tptr;}
#line 1440 "y.tab.c"
break;
case 21:
#line 108 "grammar.y"
	{yyval.tptr = MakeTree(SpecOp, NullExp(), methodDeclType);}
#line 1445 "y.tab.c"
break;
case 22:
#line 116 "grammar.y"
	{yyval.tptr = MakeTree(VArgTypeOp, MakeTree(CommaOp, NullExp(), MakeLeaf(INTEGERTNode, yystack.l_mark[0].intg)), NullExp()); yyval.tptr = yyval.tptr;}
#line 1450 "y.tab.c"
break;
case 23:
#line 117 "grammar.y"
	{yyval.tptr = MakeTree(RArgTypeOp, MakeTree(CommaOp, NullExp(), MakeLeaf(INTEGERTNode, yystack.l_mark[0].intg)), NullExp()); yyval.tptr = yyval.tptr;}
#line 1455 "y.tab.c"
break;
case 24:
#line 124 "grammar.y"
	{
      tree nameAndType = LeftChild(yystack.l_mark[-1].tptr);
      MkLeftC(MakeLeaf(IDNode, yystack.l_mark[0].intg), nameAndType);
      yyval.tptr = yystack.l_mark[-1].tptr;
    }
#line 1464 "y.tab.c"
break;
case 25:
#line 129 "grammar.y"
	{
      tree prevType = RightChild(LeftChild(yystack.l_mark[-2].tptr));
      int opType = NodeOp(yystack.l_mark[-2].tptr);
      tree nextId = MakeTree(CommaOp, MakeLeaf(IDNode, yystack.l_mark[0].intg), prevType);
      /* $$ = MakeTree(opType, nextId, $1); // this will go the opposite way.*/
      tree nextOp = MakeTree(opType, nextId, NullExp());
      yyval.tptr = MkRightC(nextOp, yystack.l_mark[-2].tptr);
  }
#line 1476 "y.tab.c"
break;
case 26:
#line 139 "grammar.y"
	{ yyval.tptr = MakeTree(BodyOp, yystack.l_mark[0].tptr, NullExp()); }
#line 1481 "y.tab.c"
break;
case 27:
#line 140 "grammar.y"
	{ yyval.tptr = MakeTree(BodyOp, yystack.l_mark[-1].tptr, yystack.l_mark[0].tptr); }
#line 1486 "y.tab.c"
break;
case 28:
#line 141 "grammar.y"
	{ yyval.tptr = MakeTree(BodyOp, NullExp(), yystack.l_mark[0].tptr); }
#line 1491 "y.tab.c"
break;
case 29:
#line 145 "grammar.y"
	{ yyval.tptr = yystack.l_mark[-1].tptr; }
#line 1496 "y.tab.c"
break;
case 30:
#line 146 "grammar.y"
	{ yyval.tptr = MakeTree(StmtOp, NullExp(), NullExp());}
#line 1501 "y.tab.c"
break;
case 31:
#line 149 "grammar.y"
	{ yyval.tptr = MakeTree(StmtOp, NullExp(), yystack.l_mark[-1].tptr); }
#line 1506 "y.tab.c"
break;
case 32:
#line 151 "grammar.y"
	{yyval.tptr = MakeTree(StmtOp, yystack.l_mark[-2].tptr, yystack.l_mark[-1].tptr) ;}
#line 1511 "y.tab.c"
break;
case 33:
#line 154 "grammar.y"
	{yyval.tptr = yystack.l_mark[0].tptr;}
#line 1516 "y.tab.c"
break;
case 34:
#line 155 "grammar.y"
	{yyval.tptr = yystack.l_mark[0].tptr;}
#line 1521 "y.tab.c"
break;
case 35:
#line 156 "grammar.y"
	{yyval.tptr = yystack.l_mark[0].tptr;}
#line 1526 "y.tab.c"
break;
case 36:
#line 157 "grammar.y"
	{yyval.tptr = yystack.l_mark[0].tptr;}
#line 1531 "y.tab.c"
break;
case 37:
#line 158 "grammar.y"
	{yyval.tptr = yystack.l_mark[0].tptr;}
#line 1536 "y.tab.c"
break;
case 38:
#line 161 "grammar.y"
	{yyval.tptr = MakeTree(AssignOp, MakeTree(AssignOp, NullExp(), yystack.l_mark[-2].tptr), yystack.l_mark[0].tptr);}
#line 1541 "y.tab.c"
break;
case 39:
#line 171 "grammar.y"
	{yyval.tptr = MakeTree(RoutineCallOp, yystack.l_mark[-2].tptr, NullExp()); }
#line 1546 "y.tab.c"
break;
case 40:
#line 179 "grammar.y"
	{
    yyval.tptr = MakeTree(RoutineCallOp, yystack.l_mark[-3].tptr, yystack.l_mark[-1].tptr);
  }
#line 1553 "y.tab.c"
break;
case 41:
#line 184 "grammar.y"
	{yyval.tptr = MakeTree(ReturnOp, NullExp(), NullExp());}
#line 1558 "y.tab.c"
break;
case 42:
#line 185 "grammar.y"
	{yyval.tptr = MakeTree(ReturnOp, yystack.l_mark[0].tptr, NullExp());}
#line 1563 "y.tab.c"
break;
case 43:
#line 188 "grammar.y"
	{yyval.tptr = MakeTree(LoopOp, yystack.l_mark[-1].tptr, yystack.l_mark[0].tptr);}
#line 1568 "y.tab.c"
break;
case 44:
#line 193 "grammar.y"
	{
      tree rightSub = MakeTree(CommaOp, yystack.l_mark[-1].tptr, yystack.l_mark[0].tptr);
      tree fullIfTree = MakeTree(IfElseOp, NullExp(), rightSub);
      /* $$ = MkLeftC(fullIfTree, $3);*/
      /* $$ = MakeTree(IfElseOp, fullIfTree, $3);*/
      yyval.tptr = fullIfTree;
    }
#line 1579 "y.tab.c"
break;
case 45:
#line 201 "grammar.y"
	{
      tree rightSub = MakeTree(CommaOp, yystack.l_mark[-3].tptr, yystack.l_mark[-2].tptr);
      tree subIf = MakeTree(IfElseOp, NullExp(), rightSub);
      tree rootIf = MakeTree(IfElseOp, subIf, yystack.l_mark[0].tptr);

      yyval.tptr = rootIf;
  }
#line 1590 "y.tab.c"
break;
case 46:
#line 209 "grammar.y"
	{
      /* this one is root*/
      /* issue right now, the first if else should */
      /* the subtree for elseif, needs to be built the other way*/
      tree ifSub = MakeTree(CommaOp, yystack.l_mark[-2].tptr, yystack.l_mark[-1].tptr);
      tree elseIfList = yystack.l_mark[0].tptr;
      tree ifTree = MakeTree(IfElseOp, elseIfList, ifSub);

      yyval.tptr = ifTree;
  }
#line 1604 "y.tab.c"
break;
case 47:
#line 221 "grammar.y"
	{
    tree elseStatementList = yystack.l_mark[0].tptr;
    tree elseIfList = yystack.l_mark[-2].tptr;
    /* */
    tree rightSubIf = MakeTree(CommaOp, yystack.l_mark[-4].tptr, yystack.l_mark[-3].tptr);
    /* link to the rest of the elseif statements.*/
    /* flip this around somehow?*/
    /* make root var, set new to end.*/
    tree mainIf = MakeTree(IfElseOp, NullExp(), rightSubIf);
    tree all = MkLeftC(mainIf, elseIfList);

    tree rootIf = MakeTree(IfElseOp, all, elseStatementList);

    /* $$ = MkLeftC(newElseIf, $1);*/
    yyval.tptr = rootIf;
  }
#line 1624 "y.tab.c"
break;
case 48:
#line 239 "grammar.y"
	{
      tree rightSub = MakeTree(CommaOp, yystack.l_mark[-1].tptr, yystack.l_mark[0].tptr);
      yyval.tptr = MakeTree(IfElseOp, NullExp(), rightSub);
  }
#line 1632 "y.tab.c"
break;
case 49:
#line 243 "grammar.y"
	{
      /* else if list is gonna be a if else op that contains the tree so far.*/
      /* set the leftmost of the ElseIfList to be The new if else op.*/
      tree rightSub = MakeTree(CommaOp, yystack.l_mark[-1].tptr, yystack.l_mark[0].tptr);
      /* create new if else tree*/
      /* 2 ways to go*/
      /* make old ElseIfList (which is an IfElseOp) the root and set the new one to its leftmostchild.*/
      /* make the new else if statement the root, set its left child to the old root (ElseIfList ($1)).*/
      tree newElseIf = MakeTree(IfElseOp, yystack.l_mark[-4].tptr, rightSub);
      yyval.tptr = newElseIf;
      /* $$ = MkLeftC(newElseIf, $1);*/
  }
#line 1648 "y.tab.c"
break;
case 50:
#line 258 "grammar.y"
	{ yyval.tptr = yystack.l_mark[-1].tptr; }
#line 1653 "y.tab.c"
break;
case 51:
#line 259 "grammar.y"
	{yyval.tptr = MakeTree(BodyOp, NullExp(), NullExp()); }
#line 1658 "y.tab.c"
break;
case 52:
#line 283 "grammar.y"
	{ yyval.tptr = yystack.l_mark[0].tptr; declType = yystack.l_mark[0].tptr; }
#line 1663 "y.tab.c"
break;
case 53:
#line 283 "grammar.y"
	{ yyval.tptr = yystack.l_mark[-1].tptr; }
#line 1668 "y.tab.c"
break;
case 54:
#line 287 "grammar.y"
	{yyval.tptr =  MakeTree(BodyOp, NullExp(), yystack.l_mark[0].tptr);}
#line 1673 "y.tab.c"
break;
case 55:
#line 290 "grammar.y"
	{ yyval.tptr = MkLeftC(yystack.l_mark[-1].tptr, MakeTree(BodyOp, NullExp(), yystack.l_mark[0].tptr)); }
#line 1678 "y.tab.c"
break;
case 56:
#line 299 "grammar.y"
	{
      tree entireVarTree = MakeTree(CommaOp, yystack.l_mark[-2].tptr, MakeTree(CommaOp, NullExp(), yystack.l_mark[0].tptr));
      /* tree entireVarTree = MakeTree(CommaOp, $1, MakeTree(CommaOp, NullExp(), $3));*/
      yyval.tptr = entireVarTree;
  }
#line 1687 "y.tab.c"
break;
case 57:
#line 304 "grammar.y"
	{
      tree varRightSub = MakeTree(CommaOp, NullExp(), NullExp());
      /* tree varRightSub = MakeTree(CommaOp, currentType, NullExp());*/
      tree entireVarTree = MakeTree(CommaOp, yystack.l_mark[0].tptr, varRightSub);
      yyval.tptr = entireVarTree;
  }
#line 1697 "y.tab.c"
break;
case 58:
#line 314 "grammar.y"
	{
      tree varRightSub = RightChild(yystack.l_mark[0].tptr);
      tree varWithType = MkLeftC(yystack.l_mark[-1].tptr, varRightSub);
      yyval.tptr = MakeTree(DeclOp, NullExp(), yystack.l_mark[0].tptr);
    }
#line 1706 "y.tab.c"
break;
case 59:
#line 319 "grammar.y"
	{
      tree varRightSub = RightChild(yystack.l_mark[0].tptr);
      tree varWithType = MkLeftC(yystack.l_mark[-3].tptr, varRightSub);
      yyval.tptr = MakeTree(DeclOp, yystack.l_mark[-2].tptr, yystack.l_mark[0].tptr);
  }
#line 1715 "y.tab.c"
break;
case 60:
#line 336 "grammar.y"
	{ yyval.tptr = yystack.l_mark[0].tptr;}
#line 1720 "y.tab.c"
break;
case 61:
#line 337 "grammar.y"
	{

    /* tree indexing = MakeTree(IndexOp, NullExp(), NullExp());*/
    /* tree indexing = MakeTree(IndexOp, NullExp(), NullExp());*/
    /* $$ = MkRightC(indexing, $1);*/
    yyval.tptr = MkRightC(MakeTree(IndexOp, NullExp(), NullExp()), yystack.l_mark[-2].tptr);
  }
#line 1731 "y.tab.c"
break;
case 62:
#line 346 "grammar.y"
	{yyval.tptr = MakeLeaf(INTEGERTNode, yystack.l_mark[0].intg);}
#line 1736 "y.tab.c"
break;
case 63:
#line 347 "grammar.y"
	{yyval.tptr = MakeLeaf(IDNode, yystack.l_mark[0].intg);}
#line 1741 "y.tab.c"
break;
case 64:
#line 350 "grammar.y"
	{ yyval.tptr = MakeTree(TypeIdOp, yystack.l_mark[0].tptr, NullExp()); }
#line 1746 "y.tab.c"
break;
case 65:
#line 351 "grammar.y"
	{
    tree field = MakeTree(FieldOp, yystack.l_mark[-2].tptr, NullExp());
    yyval.tptr = MkRightC(field, yystack.l_mark[0].tptr);
  }
#line 1754 "y.tab.c"
break;
case 66:
#line 364 "grammar.y"
	{yyval.tptr = MakeLeaf(IDNode, yystack.l_mark[0].intg);}
#line 1759 "y.tab.c"
break;
case 67:
#line 365 "grammar.y"
	{yyval.tptr = MakeLeaf(IDNode, yystack.l_mark[-1].intg);}
#line 1764 "y.tab.c"
break;
case 68:
#line 369 "grammar.y"
	{}
#line 1769 "y.tab.c"
break;
case 69:
#line 370 "grammar.y"
	{}
#line 1774 "y.tab.c"
break;
case 70:
#line 375 "grammar.y"
	{yyval.tptr = yystack.l_mark[0].tptr;}
#line 1779 "y.tab.c"
break;
case 71:
#line 376 "grammar.y"
	{yyval.tptr = yystack.l_mark[0].tptr;}
#line 1784 "y.tab.c"
break;
case 72:
#line 377 "grammar.y"
	{yyval.tptr = yystack.l_mark[0].tptr;}
#line 1789 "y.tab.c"
break;
case 73:
#line 385 "grammar.y"
	{yyval.tptr = yystack.l_mark[0].tptr;}
#line 1794 "y.tab.c"
break;
case 74:
#line 395 "grammar.y"
	{yyval.tptr = MakeTree(ArrayTypeOp, MakeTree(BoundOp, NullExp(), yystack.l_mark[-1].tptr), MakeLeaf(INTEGERTNode, INTnum));}
#line 1799 "y.tab.c"
break;
case 75:
#line 396 "grammar.y"
	{
      tree leftSub = MakeTree(BoundOp, NullExp(), yystack.l_mark[-2].tptr);
      /* tree root = MakeTree(ArrayTypeOp, NullExp(), $2);*/
      yyval.tptr = MkLeftC(leftSub, yystack.l_mark[0].tptr);
    }
#line 1808 "y.tab.c"
break;
case 76:
#line 404 "grammar.y"
	{yyval.tptr = MakeTree(ArrayTypeOp, yystack.l_mark[-1].tptr, declType);}
#line 1813 "y.tab.c"
break;
case 77:
#line 410 "grammar.y"
	{yyval.tptr = MakeTree(CommaOp, NullExp(), yystack.l_mark[0].tptr);}
#line 1818 "y.tab.c"
break;
case 78:
#line 411 "grammar.y"
	{yyval.tptr = MakeTree(CommaOp, yystack.l_mark[-2].tptr, yystack.l_mark[0].tptr);}
#line 1823 "y.tab.c"
break;
case 79:
#line 417 "grammar.y"
	{yyval.tptr = yystack.l_mark[0].tptr; }
#line 1828 "y.tab.c"
break;
case 80:
#line 421 "grammar.y"
	{ yyval.tptr = MakeTree(LTOp, yystack.l_mark[-2].tptr, yystack.l_mark[0].tptr); }
#line 1833 "y.tab.c"
break;
case 81:
#line 422 "grammar.y"
	{ yyval.tptr = MakeTree(LEOp, yystack.l_mark[-2].tptr, yystack.l_mark[0].tptr); }
#line 1838 "y.tab.c"
break;
case 82:
#line 423 "grammar.y"
	{ yyval.tptr = MakeTree(GTOp, yystack.l_mark[-2].tptr, yystack.l_mark[0].tptr); }
#line 1843 "y.tab.c"
break;
case 83:
#line 424 "grammar.y"
	{ yyval.tptr = MakeTree(GEOp, yystack.l_mark[-2].tptr, yystack.l_mark[0].tptr); }
#line 1848 "y.tab.c"
break;
case 84:
#line 425 "grammar.y"
	{ yyval.tptr = MakeTree(EQOp, yystack.l_mark[-2].tptr, yystack.l_mark[0].tptr); }
#line 1853 "y.tab.c"
break;
case 85:
#line 426 "grammar.y"
	{ yyval.tptr = MakeTree(NEOp, yystack.l_mark[-2].tptr, yystack.l_mark[0].tptr); }
#line 1858 "y.tab.c"
break;
case 86:
#line 429 "grammar.y"
	{ yyval.tptr = yystack.l_mark[0].tptr; }
#line 1863 "y.tab.c"
break;
case 87:
#line 430 "grammar.y"
	{ yyval.tptr = MakeTree(OrOp, yystack.l_mark[-2].tptr, yystack.l_mark[0].tptr); }
#line 1868 "y.tab.c"
break;
case 88:
#line 431 "grammar.y"
	{ yyval.tptr = MakeTree(AddOp, yystack.l_mark[-2].tptr, yystack.l_mark[0].tptr); }
#line 1873 "y.tab.c"
break;
case 89:
#line 432 "grammar.y"
	{ yyval.tptr = MakeTree(SubOp, yystack.l_mark[-2].tptr, yystack.l_mark[0].tptr); }
#line 1878 "y.tab.c"
break;
case 90:
#line 435 "grammar.y"
	{ yyval.tptr = yystack.l_mark[0].tptr; }
#line 1883 "y.tab.c"
break;
case 91:
#line 436 "grammar.y"
	{ yyval.tptr = MakeTree(AddOp, yystack.l_mark[0].tptr, NullExp()); }
#line 1888 "y.tab.c"
break;
case 92:
#line 437 "grammar.y"
	{ yyval.tptr = MakeTree(UnaryNegOp, yystack.l_mark[0].tptr, NullExp()); }
#line 1893 "y.tab.c"
break;
case 93:
#line 442 "grammar.y"
	{ yyval.tptr = yystack.l_mark[0].tptr; }
#line 1898 "y.tab.c"
break;
case 94:
#line 443 "grammar.y"
	{ yyval.tptr = MakeTree(MultOp, yystack.l_mark[-2].tptr, yystack.l_mark[0].tptr); }
#line 1903 "y.tab.c"
break;
case 95:
#line 444 "grammar.y"
	{ yyval.tptr = MakeTree(DivOp, yystack.l_mark[-2].tptr, yystack.l_mark[0].tptr); }
#line 1908 "y.tab.c"
break;
case 96:
#line 445 "grammar.y"
	{ yyval.tptr = MakeTree(AndOp, yystack.l_mark[-2].tptr, yystack.l_mark[0].tptr); }
#line 1913 "y.tab.c"
break;
case 97:
#line 449 "grammar.y"
	{ yyval.tptr = yystack.l_mark[0].tptr; }
#line 1918 "y.tab.c"
break;
case 98:
#line 450 "grammar.y"
	{ yyval.tptr = yystack.l_mark[0].tptr; }
#line 1923 "y.tab.c"
break;
case 99:
#line 451 "grammar.y"
	{ yyval.tptr = yystack.l_mark[0].tptr; }
#line 1928 "y.tab.c"
break;
case 100:
#line 453 "grammar.y"
	{ yyval.tptr = yystack.l_mark[-1].tptr; }
#line 1933 "y.tab.c"
break;
case 101:
#line 454 "grammar.y"
	{ yyval.tptr = MakeTree(NotOp, yystack.l_mark[0].tptr, NullExp()); }
#line 1938 "y.tab.c"
break;
case 102:
#line 458 "grammar.y"
	{yyval.tptr = MakeLeaf(NUMNode, yystack.l_mark[0].intg);}
#line 1943 "y.tab.c"
break;
case 103:
#line 459 "grammar.y"
	{yyval.tptr = MakeLeaf(STRINGNode, yystack.l_mark[0].intg);}
#line 1948 "y.tab.c"
break;
case 104:
#line 470 "grammar.y"
	{ yyval.tptr = MakeTree(VarOp, MakeLeaf(IDNode, yystack.l_mark[0].intg), NullExp());}
#line 1953 "y.tab.c"
break;
case 105:
#line 472 "grammar.y"
	{
      tree select = MakeTree(SelectOp, yystack.l_mark[0].tptr, NullExp());
      yyval.tptr = MkRightC(select, yystack.l_mark[-1].tptr);
    }
#line 1961 "y.tab.c"
break;
case 106:
#line 477 "grammar.y"
	{
      /* the first one is always var and stays var.*/
      /* eveything after is select / field*/
      /* just add a new select to the rightmost leaf.*/
      tree fieldOpNode = MakeTree(FieldOp, MakeLeaf(IDNode, yystack.l_mark[0].intg), NullExp()); 
      tree select = MakeTree(SelectOp, fieldOpNode, NullExp());
      MkRightC(select, yystack.l_mark[-2].tptr);
      yyval.tptr = yystack.l_mark[-2].tptr;
    }
#line 1974 "y.tab.c"
break;
case 107:
#line 492 "grammar.y"
	{yyval.tptr = MakeTree(IndexOp, yystack.l_mark[-1].tptr, NullExp());}
#line 1979 "y.tab.c"
break;
case 108:
#line 493 "grammar.y"
	{ yyval.tptr = MakeTree(IndexOp, yystack.l_mark[-2].tptr, yystack.l_mark[0].tptr); }
#line 1984 "y.tab.c"
break;
case 109:
#line 503 "grammar.y"
	{yyval.tptr = MakeTree(CommaOp, yystack.l_mark[0].tptr, NullExp());}
#line 1989 "y.tab.c"
break;
case 110:
#line 504 "grammar.y"
	{yyval.tptr = MakeTree(CommaOp, yystack.l_mark[-2].tptr, yystack.l_mark[0].tptr);}
#line 1994 "y.tab.c"
break;
#line 1996 "y.tab.c"
    default:
        break;
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark -= yym;
#endif
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
        {
            fprintf(stderr, "%s[%d]: after reduction, ", YYDEBUGSTR, yydepth);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
            if (!yytrial)
#endif /* YYBTYACC */
                fprintf(stderr, "result is <%s>, ", YYSTYPE_TOSTRING(yystos[YYFINAL], yyval));
#endif
            fprintf(stderr, "shifting from state 0 to final state %d\n", YYFINAL);
        }
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        *++yystack.p_mark = yyloc;
#endif
        if (yychar < 0)
        {
#if YYBTYACC
            do {
            if (yylvp < yylve)
            {
                /* we're currently re-reading tokens */
                yylval = *yylvp++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                yylloc = *yylpp++;
#endif
                yychar = *yylexp++;
                break;
            }
            if (yyps->save)
            {
                /* in trial mode; save scanner results for future parse attempts */
                if (yylvp == yylvlim)
                {   /* Enlarge lexical value queue */
                    size_t p = (size_t) (yylvp - yylvals);
                    size_t s = (size_t) (yylvlim - yylvals);

                    s += YYLVQUEUEGROWTH;
                    if ((yylexemes = (YYINT *)realloc(yylexemes, s * sizeof(YYINT))) == NULL)
                        goto yyenomem;
                    if ((yylvals   = (YYSTYPE *)realloc(yylvals, s * sizeof(YYSTYPE))) == NULL)
                        goto yyenomem;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    if ((yylpsns   = (YYLTYPE *)realloc(yylpsns, s * sizeof(YYLTYPE))) == NULL)
                        goto yyenomem;
#endif
                    yylvp   = yylve = yylvals + p;
                    yylvlim = yylvals + s;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                    yylpp   = yylpe = yylpsns + p;
                    yylplim = yylpsns + s;
#endif
                    yylexp  = yylexemes + p;
                }
                *yylexp = (YYINT) YYLEX;
                *yylvp++ = yylval;
                yylve++;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
                *yylpp++ = yylloc;
                yylpe++;
#endif
                yychar = *yylexp++;
                break;
            }
            /* normal operation, no conflict encountered */
#endif /* YYBTYACC */
            yychar = YYLEX;
#if YYBTYACC
            } while (0);
#endif /* YYBTYACC */
            if (yychar < 0) yychar = YYEOF;
#if YYDEBUG
            if (yydebug)
            {
                if ((yys = yyname[YYTRANSLATE(yychar)]) == NULL) yys = yyname[YYUNDFTOKEN];
                fprintf(stderr, "%s[%d]: state %d, reading token %d (%s)\n",
                                YYDEBUGSTR, yydepth, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == YYEOF) goto yyaccept;
        goto yyloop;
    }
    if (((yyn = yygindex[yym]) != 0) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == (YYINT) yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
    {
        fprintf(stderr, "%s[%d]: after reduction, ", YYDEBUGSTR, yydepth);
#ifdef YYSTYPE_TOSTRING
#if YYBTYACC
        if (!yytrial)
#endif /* YYBTYACC */
            fprintf(stderr, "result is <%s>, ", YYSTYPE_TOSTRING(yystos[yystate], yyval));
#endif
        fprintf(stderr, "shifting from state %d to state %d\n", *yystack.s_mark, yystate);
    }
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    *++yystack.s_mark = (YYINT) yystate;
    *++yystack.l_mark = yyval;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    *++yystack.p_mark = yyloc;
#endif
    goto yyloop;
#if YYBTYACC

    /* Reduction declares that this path is valid. Set yypath and do a full parse */
yyvalid:
    if (yypath) YYABORT;
    while (yyps->save)
    {
        YYParseState *save = yyps->save;
        yyps->save = save->save;
        save->save = yypath;
        yypath = save;
    }
#if YYDEBUG
    if (yydebug)
        fprintf(stderr, "%s[%d]: state %d, CONFLICT trial successful, backtracking to state %d, %d tokens\n",
                        YYDEBUGSTR, yydepth, yystate, yypath->state, (int)(yylvp - yylvals - yypath->lexeme));
#endif
    if (yyerrctx)
    {
        yyFreeState(yyerrctx);
        yyerrctx = NULL;
    }
    yylvp          = yylvals + yypath->lexeme;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yylpp          = yylpsns + yypath->lexeme;
#endif
    yylexp         = yylexemes + yypath->lexeme;
    yychar         = YYEMPTY;
    yystack.s_mark = yystack.s_base + (yypath->yystack.s_mark - yypath->yystack.s_base);
    memcpy (yystack.s_base, yypath->yystack.s_base, (size_t) (yystack.s_mark - yystack.s_base + 1) * sizeof(YYINT));
    yystack.l_mark = yystack.l_base + (yypath->yystack.l_mark - yypath->yystack.l_base);
    memcpy (yystack.l_base, yypath->yystack.l_base, (size_t) (yystack.l_mark - yystack.l_base + 1) * sizeof(YYSTYPE));
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
    yystack.p_mark = yystack.p_base + (yypath->yystack.p_mark - yypath->yystack.p_base);
    memcpy (yystack.p_base, yypath->yystack.p_base, (size_t) (yystack.p_mark - yystack.p_base + 1) * sizeof(YYLTYPE));
#endif
    yystate        = yypath->state;
    goto yyloop;
#endif /* YYBTYACC */

yyoverflow:
    YYERROR_CALL("yacc stack overflow");
#if YYBTYACC
    goto yyabort_nomem;
yyenomem:
    YYERROR_CALL("memory exhausted");
yyabort_nomem:
#endif /* YYBTYACC */
    yyresult = 2;
    goto yyreturn;

yyabort:
    yyresult = 1;
    goto yyreturn;

yyaccept:
#if YYBTYACC
    if (yyps->save) goto yyvalid;
#endif /* YYBTYACC */
    yyresult = 0;

yyreturn:
#if defined(YYDESTRUCT_CALL)
    if (yychar != YYEOF && yychar != YYEMPTY)
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        YYDESTRUCT_CALL("cleanup: discarding token", yychar, &yylval, &yylloc);
#else
        YYDESTRUCT_CALL("cleanup: discarding token", yychar, &yylval);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */

    {
        YYSTYPE *pv;
#if defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED)
        YYLTYPE *pp;

        for (pv = yystack.l_base, pp = yystack.p_base; pv <= yystack.l_mark; ++pv, ++pp)
             YYDESTRUCT_CALL("cleanup: discarding state",
                             yystos[*(yystack.s_base + (pv - yystack.l_base))], pv, pp);
#else
        for (pv = yystack.l_base; pv <= yystack.l_mark; ++pv)
             YYDESTRUCT_CALL("cleanup: discarding state",
                             yystos[*(yystack.s_base + (pv - yystack.l_base))], pv);
#endif /* defined(YYLTYPE) || defined(YYLTYPE_IS_DECLARED) */
    }
#endif /* defined(YYDESTRUCT_CALL) */

#if YYBTYACC
    if (yyerrctx)
    {
        yyFreeState(yyerrctx);
        yyerrctx = NULL;
    }
    while (yyps)
    {
        YYParseState *save = yyps;
        yyps = save->save;
        save->save = NULL;
        yyFreeState(save);
    }
    while (yypath)
    {
        YYParseState *save = yypath;
        yypath = save->save;
        save->save = NULL;
        yyFreeState(save);
    }
#endif /* YYBTYACC */
    yyfreestack(&yystack);
    return (yyresult);
}
