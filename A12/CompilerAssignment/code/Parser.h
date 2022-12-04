/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2022
* Author: Svillen Ranev - Paulo Sousa
* Professors: Paulo Sousa
************************************************************
 _________________________________
|                                 |
| ........ KRAIT LANGUAGE ......... |
|     __    __    __    __        |
|    /  \  /  \  /  \  /  \       |
| __/  __\/  __\/  __\/  __\__    |
| _/  /__/  /__/  /__/  /_____|   |
|  \_/ \   / \   / \   / \  \___  |
|       \_/   \_/   \_/   \___o_> |
|                                 |
| .. ALGONQUIN COLLEGE - 2022F .. |
|_________________________________|

*/

/*
************************************************************
* File name: parser.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A32.
* Date: Sep 01 2022
* Professor: Paulo Sousa
* Purpose: This file is the main header for Parser (.h)
*************************************************************/

#ifndef PARSER_H_
#define PARSER_H_
#endif
/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef READER_H_
#include "Reader.h"
#endif
#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global vars */
static Token lookahead;
extern BufferReader* stringLiteralTable;
extern krait_intg line;
extern Token tokenizer();
extern krait_char* keywordTable[];
extern LanguageAttributes lang1;
krait_intg syntaxErrorNumber = 0;

#define STR_LANGNAME	"Krait"
#define LANG_WRTE		"print"
#define LANG_READ		"input"
//#define LANG_MAIN		"main&"

/* TO_DO: Create ALL constants for keywords (sequence given in table.h) */

/* Constants */
enum KEYWORDS {
	NO_ATTR = -1,
	KW_if,
	KW_else,
	KW_while,
	KW_do,
	KW_false,
	KW_true,
	KW_and,
	KW_or,
	KW_not,
	KW_return,
	KW_in,
	KW_def,
};

/* Function definitions */
krait_void startParser();
krait_void matchToken(krait_intg, krait_intg);
krait_void syncErrorHandler(krait_intg);
krait_void printError();

/* TO_DO: Place ALL non-terminal function declarations */
krait_void program();
krait_void dataSession();
krait_void optVarListDeclarations();
krait_void varListDeclarations();
krait_void varListDeclaration();
krait_void varListDeclarationPrime();
krait_void codeSession();
krait_void optionalStatements();
krait_void statements();
krait_void statement();
krait_void assignmentStatement();
krait_void assignmentExpression();
krait_void arithmeticExpression();
krait_void additiveArithmeticExpression();
krait_void multiplicativeArithmeticExpression();
krait_void primaryArithmeticExpression();
krait_void primaryArithmeticExpressionPrime();
krait_void variableIdentifier();
krait_void functionIdentifier();
krait_void whileStatement();
krait_void conditionStatement();
krait_void conditionExpresion();
krait_void unaryArithmeticExpression();
krait_void statementsPrime();
krait_void outputStatement();
krait_void outputVariableLists();
krait_void outputVariableList();
krait_void outputVariableListPrime();
krait_void inputStatement();
krait_void inputVariableList();


