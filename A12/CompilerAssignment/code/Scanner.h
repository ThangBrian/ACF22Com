/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2022
* Author: Svillen Ranev - Paulo Sousa
* Professors: Paulo Sousa
************************************************************
*/

/*
************************************************************
* File name: scanner.h
* Compiler: MS Visual Studio 2022
* Author: Paulo Sousa
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A22, A32.
* Date: Jul 01 2022
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */
#define DBL_LEN	20  /* maximum number of digits for DL*/

#define RTE_CODE 1  /* Value for run-time error */

/* TO_DO: Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	MNID_T,		/*  1: Method name/Variable name identifier token  */
	STR_T,		/*  2: String literal token */
	LPR_T,		/*  3: Left parenthesis token */
	RPR_T,		/*  4: Right parenthesis token */
	KW_T,		/*  5: Keyword token */
	EOS_T,		/*  6: End of statement (\n) */
	SBC_T,		/*  7: Start a block of code*/
	COM_T,		/*  8: Comment token*/
	RTE_T,		/*  9: Run-time error token */
	INL_T,		/* 10: Integer literal token */
	DBL_T,		/* 11: Double literal token*/
	SEOF_T,		/* 12: Source end-of-file token */
	UNS_T,		/* 13: Underscore token */
	CMA_T,		/* 14: Comma token*/
	EXP_T,		/* 15: Exponential token*/
	ART_OP_T,	/* 16: Arithmetic operator token*/
	REL_OP_T,	/* 17: Relational operator token*/
	LOG_OP_T,	/* 18: Logical operator token*/
	ASS_OP_T,	/* 19: Assignment operator token*/
};

/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_EXP } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT, OP_GE, OP_LE } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT} LogOperator;
typedef enum AssignmentOperator{ OP_ASS} AssOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* TO_DO: Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	krait_intg codeType;      /* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetisc operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	AssOperator assignmentOperator;		/* assignment operator attribute code*/
	EofOperator seofType;				/* source-end-of-file attribute code */
	krait_intg intValue;						/* integer literal attribute (value) */
	krait_intg keywordIndex;					/* keyword index in the keyword table */
	krait_intg contentString;				/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	krait_float floatValue;					/* floating-point literal attribute (value) */
	krait_char idLexeme[VID_LEN + 1];		/* variable identifier token attribute */
	krait_char errLexeme[ERR_LEN + 1];		/* error token attribite */
} TokenAttribute;

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	krait_byte flags;			/* Flags information */
	union {
		krait_intg intValue;				/* Integer value */
		krait_float floatValue;			/* Float value */
		krait_char* stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	krait_intg code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

/*  Special case tokens processed separately one by one in the token-driven part of the scanner
 *  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' , ';', 
 *  white space, #comment\n , ',' , ';' , '-' , '+' , '*' , '/', # , 
 *  .&., .|. , .!. , SEOF.
 */

/* TO_DO: Define lexeme FIXED classes */
/* These constants will be used on nextClass */
#define CHRCOL2 '_'
#define CHRCOL3 '('
#define CHRCOL4 '"'
#define CHRCOL5 '#'
#define CHRCOL6 '\n'
#define CHRCOL7 '.'

/* These constants will be used on VID / MID function */
//#define MNIDPREFIX '&'

/* TO_DO: Error states and illegal state */
#define FS		100		/* Final State */
#define ESWR	14		/* Error state with retract */
#define ESNR	13		/* Error state with no retract */

 /* TO_DO: State transition table definition */
#define TABLE_COLUMNS 9

/* TO_DO: Transition table - type of states defined in separate table */
static krait_intg transitionTable[][TABLE_COLUMNS] = {
/*   [A-z] , [0-9],    _,    (,    ", #,     \n,    .,     
	   L(0),  D(1), U(2), M(3), Q(4), H(5),  E(6),  P(7),  O(8)*/
	{     1,	 8,	   1, ESNR,    4,    6,	 ESNR,  ESNR,  ESNR},  // S0: NOFS
	{     1,	 1,    1,    2,	   2,    2,     2,	   2,     2},  // S1: NOFS
	{    FS,    FS,   FS,   FS,   FS,   FS,    FS,	  FS,    FS}, // S2: FSWR (MVID)
	{    FS,    FS,   FS,   FS,   FS,   FS,    FS,    FS,    FS}, // S3: FSWR (KEY)
	{     4,     4,    4,    4,    5,    4,     4,     4,     4}, // S4: NOFS
	{    FS,    FS,   FS,   FS,   FS,   FS,    FS,    FS,    FS}, // S5: FSNR (SL)
	{     6,     6,    6,    6,    6,    6,     7,     6,     6}, // S6: NOFS
	{    FS,    FS,   FS,   FS,   FS,   FS,    FS,    FS,    FS},  // S7: FSNR (Comment)
	{	 12,     8,   12,   12,   12,   12,    12,     9,    12}, // S8: NOFS
	{  ESNR,    10, ESNR, ESNR, ESNR, ESNR,  ESNR,  ESNR,  ESNR}, // S9: NOFS
	{    11,    10,   11,   11,   11,   11,    11,    11,    11}, // S10: NOFS
	{    FS,    FS,	  FS,   FS,   FS,   FS,    FS,    FS,    FS}, // S11: FSWR (DL)
	{    FS,    FS,	  FS,   FS,   FS,   FS,    FS,    FS,    FS},  // S12: FSWR (IL)
	{    FS,    FS,	  FS,   FS,   FS,   FS,    FS,    FS,    FS}, // S13: FSNR (ES)
	{    FS,    FS,	  FS,   FS,   FS,   FS,    FS,    FS,    FS}, // S14: FSWR (ER)
};

/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

/* TO_DO: Define list of acceptable states */
static krait_intg stateType[] = {
	NOFS, /* 00 */
	NOFS, /* 01 */
	FSWR, /* 02 (MID) - Methods */
	FSWR, /* 03 (KEY) */
	NOFS, /* 04 */
	FSNR, /* 05 (SL) */
	NOFS, /* 06 */
	FSNR, /* 07 (Comment)*/
	NOFS, /* 08 */
	NOFS, /* 09 */
	NOFS, /* 10 */
	FSWR, /* 11 (DL) */
	FSWR, /* 12 (IL) */
	FSNR, /* 13 (Err1 - no retract*/
	FSWR, /* 14 (Err2 - retract*/
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
krait_intg startScanner(ReaderPointer psc_buf);
static krait_intg nextClass(krait_char c);			/* character class function */
static krait_intg nextState(boa_intg, boa_char);		/* state machine function */

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(krait_char* lexeme);

/* Declare accepting states functions */
Token funcSL	 (krait_char lexeme[]);
Token funcIL	 (krait_char lexeme[]);
Token funcDL	 (krait_char lexeme[]);
Token funcID	 (krait_char lexeme[]);
Token funcKEY	 (krait_char lexeme[]);
Token funcErr	(krait_char lexeme[]);

/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

/* TO_DO: Define final state table */
static PTR_ACCFUN finalStateTable[] = {
	NULL,		/* -    [00] */
	NULL,		/* -    [01] */
	funcID,		/* MNID	[02] */
	funcKEY,	/* KEY  [03] */
	NULL,		/* -    [04] */
	funcSL,		/* SL   [05] */
	NULL,		/*		[06] */
	NULL,		/*		[07] */
	NULL,		/* -    [08] */
	NULL,		/* -    [09] */
	NULL,		/* -    [10] */
	funcDL,		/* DL   [11] */
	funcIL,		/* IL   [12] */
	funcErr,	/* ERR1 [13] */
	funcErr		/* ERR2 [14] */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */
#define KWT_SIZE 13

/* TO_DO: Define the list of keywords */
static krait_char* keywordTable[KWT_SIZE] = {
	"if",
	"then",
	"else",
	"while",
	"do",
	"False",
	"True",
	"and",
	"or",
	"not",
	"return",
	"in",
	"def",
};

/* NEW SECTION: About indentation */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT '\t'  /* Tabulation */

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct languageAttributes {
	krait_char indentationCharType;
	krait_intg indentationCurrentPos;
	/* TO_DO: Include any extra attribute to be used in your scanner (OPTIONAL and FREE) */
} LanguageAttributes;

#endif
