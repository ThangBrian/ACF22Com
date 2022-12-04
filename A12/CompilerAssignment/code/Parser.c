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
* File name: mainBuffer.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A32.
* Date: Sep 01 2022
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef PARSER_H_
#include "Parser.h"
#endif

/*
************************************************************
 * Process Parser
 ***********************************************************
 */
/* TO_DO: This is the function to start the parser - check your program definition */

krait_void startParser() {
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*
 ************************************************************
 * Match Token
 ***********************************************************
 */
/* TO_DO: This is the main code for match - check your definition */
krait_void matchToken(krait_intg tokenCode, krait_intg tokenAttribute) {
	krait_intg matchFlag = 1;
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType != tokenAttribute)
			matchFlag = 0;
	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
	}
	if (matchFlag && lookahead.code == SEOF_T)
		return;
	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		syncErrorHandler(tokenCode);
}

/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */
/* TO_DO: This is the function to handler error - adjust basically datatypes */
krait_void syncErrorHandler(krait_intg syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*
 ************************************************************
 * Print Error
 ***********************************************************
 */
/* TO_DO: This is the function to error printing - adjust basically datatypes */
krait_void printError() {
	Token t = lookahead;
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d \nAttribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("*ERROR*: %s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MNID_T:
		printf("MNID_T:\t\t%s\t\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("STR_T: %s\n", readerGetContent(stringLiteralTable, t.attribute.contentString));
		break;
	case KW_T:
		printf("KW_T: %s\n", keywordTable[t.attribute.codeType]);
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case EOS_T:
		printf("NA\n");
		break;
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
	}
}

/*
 ************************************************************
 * Program statement
 * BNF: <program> -> main& { <opt_statements> }
 * FIRST(<program>)= {MNID_T (main&)}.
 ***********************************************************
 */
krait_void program() {
	while (lookahead.code == COM_T) {
		matchToken(COM_T, NO_ATTR);
	}
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType == KW_def) {
			matchToken(KW_T, KW_def);
			matchToken(MNID_T, NO_ATTR);
			matchToken(LPR_T, NO_ATTR);
			dataSession();
			//matchToken(RPR_T, NO_ATTR);
			matchToken(SBC_T, NO_ATTR);
			matchToken(EOS_T, NO_ATTR);
			codeSession();
		}
		else
			printError();
	case SEOF_T:
		; // Empty
		break;
	default:
		//printError();
		codeSession();
	}
	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}

/*
 ************************************************************
 * dataSession
 * BNF: <dataSession> -> data { <opt_varlist_declarations> }
 * FIRST(<program>)= {KW_T (KW_data)}.
 ***********************************************************
 */
krait_void dataSession() {
	optVarListDeclarations();
	printf("%s%s\n", STR_LANGNAME, ": Data Session parsed");
}

/*
 ************************************************************
 * Optional Var List Declarations
 * BNF: <opt_varlist_declarations> -> <varlist_declarations> | e
 * FIRST(<opt_varlist_declarations>) = { e, MNID_T}.
 ***********************************************************
 */
krait_void optVarListDeclarations() {
	switch (lookahead.code) {
	case RPR_T:
		matchToken(RPR_T, NO_ATTR);
		break;
	case MNID_T:
		varListDeclarations();
		break;
	default:
		matchToken(RPR_T, NO_ATTR);
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional Variable List Declarations parsed");
}

/*
* BNF: <varlist_declarations> -> <varlist_declaration> <varlist_declarationsPrime>
	   <varlist_declarationsPrime> -> <varlist_declaration> <varlist_declarationsPrime> | ϵ
* FIRST
*/
krait_void varListDeclarations() {
	varListDeclaration();
	varListDeclarationPrime();
	printf("%s%s\n", STR_LANGNAME, ": Variable List Declarations parsed");
}

krait_void varListDeclaration() {
	matchToken(MNID_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Variable Identifier parsed");
	printf("%s%s\n", STR_LANGNAME, ": Variable List Declaration parsed");
}

krait_void varListDeclarationPrime() {
	switch (lookahead.code)
	{
	case CMA_T:
		matchToken(CMA_T, NO_ATTR);
		if(lookahead.code == MNID_T)
			varListDeclarations();
		else {
			if(lookahead.code == RPR_T)
				printError();
			while (lookahead.code != RPR_T && lookahead.code != MNID_T) {
				printError();
				matchToken(lookahead.code, NO_ATTR);
			}
		}
		break;
	case RPR_T:
		matchToken(RPR_T, NO_ATTR);
		break;
	default:
		printError();
		matchToken(lookahead.code, NO_ATTR);
		break;
	}
}

/*
 ************************************************************
 * codeSession statement
 * BNF: <codeSession> -> code { <opt_statements> }
 * FIRST(<codeSession>)= {KW_T (KW_code)}.
 ***********************************************************
 */
krait_void codeSession() {
	optionalStatements();
	printf("%s%s\n", STR_LANGNAME, ": Code Session parsed");
}

/* TO_DO: Continue the development (all non-terminal functions) */

/*
 ************************************************************
 * Optional statement
 * BNF: <opt_statements> -> <statements> | ϵ
 * FIRST(<opt_statements>) = { ϵ , IVID_T, FVID_T, SVID_T, KW_T(KW_if),
 *				KW_T(KW_while), MNID_T(print&), MNID_T(input&) }
 ***********************************************************
 */
krait_void optionalStatements() {
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.keywordIndex != KW_if && lookahead.attribute.keywordIndex != KW_while)
			break;
	case MNID_T:
		//if ((strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 5) == 0) ||
		//	(strncmp(lookahead.attribute.idLexeme, LANG_READ, 5) == 0)) {
 			statements();
			break;
		//}
	case COM_T:
		while (lookahead.code == COM_T) {
			matchToken(COM_T, NO_ATTR);
		}
		statements();
		break;
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}

/*
 ************************************************************
 * Statements
 * BNF: <statements> -> <statement><statementsPrime>
 * FIRST(<statements>) = { MNID_T, KW_T(KW_if),
 *		KW_T(KW_while), MNID_T(input), MNID_T(print) }
 ***********************************************************
 */
krait_void statements() {
	statement();
	statementsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}

/*
 ************************************************************
 * Statements Prime
 * BNF: <statementsPrime> -> <statement><statementsPrime> | ϵ
 * FIRST(<statementsPrime>) = { ϵ , IVID_T, FVID_T, SVID_T, 
 *		KW_T(KW_if), KW_T(KW_while), MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
krait_void statementsPrime() {
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.keywordIndex != KW_if && lookahead.attribute.keywordIndex != KW_while)
			break;
	case MNID_T:
			statements();
			break;
	default:
		; //empty string
	}
}

/*
 ************************************************************
 * Single statement
 * BNF: <statement> -> <assignment statement> | <selection statement> |
 *	<iteration statement> | <input statement> | <output statement>
 * FIRST(<statement>) = { MNID_T, KW_T(KW_if), KW_T(KW_while),
 *			MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
krait_void statement() {
	switch (lookahead.code) {
	case KW_T:
		switch (lookahead.attribute.codeType) {
		case KW_while:
			whileStatement();
			break;
		case KW_if:
			break;
		default:
			printError();
		}
		break;
	case MNID_T:
		if (strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 5) == 0) {
			outputStatement();
		}
		else if (strncmp(lookahead.attribute.idLexeme, LANG_READ, 5) == 0) {
			inputStatement();
		}
		else {
			matchToken(MNID_T, NO_ATTR);
			switch (lookahead.code)
			{
			case ASS_OP_T:
				variableIdentifier();
				assignmentStatement();
				break;
			case LPR_T:
				matchToken(LPR_T, NO_ATTR);
				while (lookahead.code == MNID_T || lookahead.code == INL_T || lookahead.code == DBL_T || lookahead.code == STR_T || lookahead.code == CMA_T) {
					matchToken(lookahead.code, NO_ATTR);
				}
				matchToken(RPR_T, NO_ATTR);
				functionIdentifier();
				break;
			case SEOF_T:
				break;
			default:
				if(lookahead.code == EOS_T)
					printError();
				matchToken(EOS_T, NO_ATTR);
				break;
			}
		}
		break;
	default:
		printError();
	}
	if(lookahead.code == EOS_T)
		matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

krait_void whileStatement() {
	matchToken(KW_T, KW_while);
	krait_intg whileIndentation = lang1.indentationCurrentPos;
	conditionStatement();
	while (lang1.indentationCurrentPos != whileIndentation) {
		statement();
	}
	printf("%s%s\n", STR_LANGNAME, ": While loop parsed");
}

krait_void conditionStatement() {
	conditionExpresion();
	matchToken(SBC_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Condition statement parsed");
}

krait_void conditionExpresion() {
	while (lookahead.code != SBC_T) {
		switch (lookahead.code)
		{
		case INL_T:
		case DBL_T:
		case STR_T:
		case MNID_T:
			primaryArithmeticExpression();
			if (lookahead.code != REL_OP_T) {
				printError();
			}
			else {
				matchToken(REL_OP_T, NO_ATTR);
			}
			if(lookahead.code == INL_T || lookahead.code == DBL_T || lookahead.code ==  STR_T || lookahead.code == MNID_T)
				primaryArithmeticExpression();
			break;
		default:
			printError();
			break;
		}
	}
}

krait_void assignmentStatement() {
	assignmentExpression();
	printf("%s%s\n", STR_LANGNAME, ": Assignment statement parsed");
}


krait_void assignmentExpression() {
	matchToken(ASS_OP_T, NO_ATTR);
	arithmeticExpression();
	while (lookahead.code != EOS_T) {
		arithmeticExpression();
	}
	//while (lookahead.code != EOS_T) {
	//	switch (lookahead.code)
	//	{
	//	case INL_T:
	//	case DBL_T:
	//	case STR_T:
	//	case MNID_T:
	//		if (strncmp(lookahead.attribute.idLexeme, LANG_READ, 5) == 0) {
	//			inputStatement();
	//			break;
	//		}
	//	case ART_OP_T:
	//		arithmeticExpression();
	//		break;
	//	case LPR_T:
	//		matchToken(LPR_T, NO_ATTR);
	//		break;
	//	case RPR_T:
	//		matchToken(RPR_T, NO_ATTR);
	//		break;
	//	default:
	//		printError();
	//		exit(0);
	//	}
	//}
	printf("%s%s\n", STR_LANGNAME, ": Assignment expression parsed");
}

krait_void arithmeticExpression() {
	if (lookahead.code == LPR_T)
		matchToken(LPR_T, NO_ATTR);
	additiveArithmeticExpression();
	/*while (lookahead.code != EOS_T) {
		switch (lookahead.code)
		{
		case INL_T:
		case DBL_T:
		case STR_T:
		case ART_OP_T:
		case MNID_T:
			additiveArithmeticExpression();
			break;
		case EOS_T:
			break;
		case LPR_T:
			matchToken(LPR_T, NO_ATTR);
			break;
		case RPR_T:
			matchToken(RPR_T, NO_ATTR);
			break;
		default:
			printError();
			exit(0);
		}
	}*/
	printf("%s%s\n", STR_LANGNAME, ": Arithmetic expression parsed");
}

krait_void additiveArithmeticExpression() {
	multiplicativeArithmeticExpression();
	//while (lookahead.code != RPR_T && lookahead.code != EOS_T) {
	//	switch (lookahead.code)
	//	{
	//	case INL_T:
	//	case DBL_T:
	//	case STR_T:
	//	case MNID_T:
	//	case ART_OP_T:
	//		multiplicativeArithmeticExpression();
	//		break;
	//	default:
	//		printError();
	//		exit(0);
	//	}
	//}
	printf("%s%s\n", STR_LANGNAME, ": Additive arithmetic expression parsed");
}

krait_void multiplicativeArithmeticExpression() {
	switch (lookahead.code)
	{
	case INL_T:
	case DBL_T:
	case STR_T:
	case MNID_T:
		primaryArithmeticExpression();
		break;
	//case ART_OP_T:
	//	matchToken(ART_OP_T, NO_ATTR);
	//	break;
	//case LPR_T:
	//	matchToken(LPR_T, NO_ATTR);
	//	break;
	default:
		printError();
		exit(0);
	}
	printf("%s%s\n", STR_LANGNAME, ": Multiplicative expression parsed");
}

krait_void primaryArithmeticExpression() {
	switch (lookahead.code) {
	case INL_T:
	case DBL_T:
	case STR_T:
		matchToken(lookahead.code, NO_ATTR);
		printf("%s%s\n", STR_LANGNAME, ": Primary arithmetic expression parsed");
		primaryArithmeticExpressionPrime();
		break;
	case MNID_T:
		if (strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 5) == 0) {
			outputStatement();
			break;
		}
		else if (strncmp(lookahead.attribute.idLexeme, LANG_READ, 5) == 0) {
			inputStatement();
			break;
		}
		matchToken(MNID_T, NO_ATTR);
		if (lookahead.code == LPR_T) {
			functionIdentifier();
			matchToken(LPR_T, NO_ATTR);
			while (lookahead.code == INL_T || lookahead.code == DBL_T || lookahead.code == STR_T || lookahead.code == MNID_T) {
				matchToken(lookahead.code, NO_ATTR);
			}
			matchToken(RPR_T, NO_ATTR);
			break;
		}
		else {
			variableIdentifier();
		}
		primaryArithmeticExpressionPrime();
		break;
	default:
		matchToken(EOS_T, NO_ATTR);
		break;
	}
}

krait_void primaryArithmeticExpressionPrime() {
	switch (lookahead.code)
	{
	case ART_OP_T:
		matchToken(ART_OP_T, NO_ATTR);
		if(lookahead.code != LPR_T)
			primaryArithmeticExpression();
		break;
	case RPR_T:
		matchToken(RPR_T, NO_ATTR);
		if (lookahead.code != ART_OP_T && lookahead.code != EOS_T)
			matchToken(EOS_T, NO_ATTR);
		else {
			if (lookahead.code == ART_OP_T) {
				matchToken(ART_OP_T, NO_ATTR);
			}
		}
	default:
		break;
	}
}

/*
 ************************************************************
 * Output Statement
 * BNF: <output statement> -> print& (<output statementPrime>);
 * FIRST(<output statement>) = { MNID_T(print&) }
 ***********************************************************
 */
krait_void outputStatement() {
	matchToken(MNID_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	outputVariableLists();
	//matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}

krait_void outputVariableLists() {
	outputVariableList();
	outputVariableListPrime();
	printf("%s%s\n", STR_LANGNAME, ": Output variable lists parsed");
}

/*
 ************************************************************
 * Output Variable List
 * BNF: <opt_variable list> -> <variable list> | ϵ
 * FIRST(<opt_variable_list>) = { IVID_T, FVID_T, SVID_T, ϵ }
 ***********************************************************
 */
krait_void outputVariableList() {

	if(lookahead.code == STR_T)
		matchToken(STR_T, NO_ATTR);
	else
		primaryArithmeticExpression();
	printf("%s%s\n", STR_LANGNAME, ": Output variable list parsed");
}

krait_void outputVariableListPrime(){
	switch (lookahead.code)
	{
	case CMA_T:
		matchToken(CMA_T, NO_ATTR);
		switch (lookahead.code)
		{
		case STR_T:
		case MNID_T:
			outputVariableLists();
			break;
		default:
			matchToken(EOS_T, NO_ATTR);
			break;
		}
		break;
	case RPR_T:
		matchToken(RPR_T, NO_ATTR);
		break;
	default:
		break;
	}
}
krait_void inputStatement() {
	matchToken(MNID_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	inputVariableList();
	matchToken(RPR_T, NO_ATTR);
	//matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Input statement parsed");
}

krait_void inputVariableList() {
	switch (lookahead.code) {
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		break;
	default:
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": Input variable list parsed");
}

krait_void variableIdentifier() {
	printf("%s%s\n", STR_LANGNAME, ": Variable identifier parsed");
}
krait_void functionIdentifier() {
	printf("%s%s\n", STR_LANGNAME, ": Function identifier parsed");
}