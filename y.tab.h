#ifndef _yy_defines_h_
#define _yy_defines_h_

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
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union YYSTYPE {
  int intg;
  tree tptr;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
extern YYSTYPE yylval;

#endif /* _yy_defines_h_ */
