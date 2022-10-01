/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2022
* Author: Quoc Thang Tran
* Professors: Paulo Sousa
************************************************************
 _________________________________
|                                 |
| ....... KRAIT LANGUAGE ........ |
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
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Sep 30 2022
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#define READER_H_

 /* TIP: Do not change pragmas, unless necessary .......................................*/
 /*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */
 /*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

 /* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Modes (used to create buffer reader) */
enum READER_MODE {
	MODE_FIXED = 'f', /* Fixed mode (constant size) */
	MODE_ADDIT = 'a', /* Additive mode (constant increment to be added) */
	MODE_MULTI = 'm'  /* Multiplicative mode (constant increment to be multiplied) */
};

/* Constants about controls (not need to change) */
#define READER_ERROR (-1)						/* General error message */
#define READER_TERMINATOR '\0'							/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* You should add your own constant definitions here */
#define READER_MAX_SIZE	INT_MAX-1	/* maximum capacity */ 

#define READER_DEFAULT_SIZE			250		/* default initial buffer reader capacity */
#define READER_DEFAULT_INCREMENT	10		/* default increment factor */

/* Add your bit-masks constant definitions here - Defined for KRAIT */
/*#define BITS                                (7654.3210) */
#define READER_DEFAULT_FLAG 0x00 	/* (0000.0000)_2 = (000)_10 */
#define SET_BIT3	0x08
#define SET_BIT2	0x04
#define SET_BIT1	0x02
#define SET_BIT0	0x01
#define RESET_BIT3	0xF7	
#define RESET_BIT2	0xFB	
#define RESET_BIT1	0xFD	
#define RESET_BIT0	0xFE	
#define CHECK_BIT0	0x01
#define CHECK_BIT2	0x04
#define CHECK_BIT3	0x08
/* TO_DO: BIT 3: FUL = Full */
/* TO_DO: BIT 2: EMP: Empty */
/* TO_DO: BIT 1: REL = Relocation */
/* TO_DO: BIT 0: END = EndOfBuffer */

#define NCHAR				128			/* Chars from 0 to 127 */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* TODO: Adjust datatypes */

/* Offset declaration */
typedef struct position {
	krait_intg mark;			/* the offset to the mark position (in chars) */
	krait_intg read;			/* the offset to the get a char position (in chars) */
	krait_intg wrte;			/* the offset to the add chars (in chars) */
} Position;

/* Buffer structure */
typedef struct bufferReader {
	krait_char* content;			/* pointer to the beginning of character array (character buffer) */
	krait_intg	size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	krait_intg	increment;			/* character array increment factor */
	krait_intg	mode;				/* operational mode indicator */
	krait_byte	flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position	position;			/* Offset / position field */
	krait_intg	histogram[NCHAR];	/* Statistics of chars */
} BufferReader, * ReaderPointer;

/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
ReaderPointer	readerCreate(krait_intg, krait_intg, krait_intg);
ReaderPointer	readerAddChar(ReaderPointer const, krait_char);
krait_boln		readerClear(ReaderPointer const);
krait_boln		readerFree(ReaderPointer const);
krait_boln		readerIsFull(ReaderPointer const);
krait_boln		readerIsEmpty(ReaderPointer const);
krait_boln		readerSetMark(ReaderPointer const, krait_intg);
krait_intg		readerPrint(ReaderPointer const);
krait_intg		readerLoad(ReaderPointer const, FILE* const);
krait_boln		readerRecover(ReaderPointer const);
krait_boln		readerRetract(ReaderPointer const);
krait_boln		readerRestore(ReaderPointer const);
/* Getters */
krait_char		readerGetChar(ReaderPointer const);
krait_char*		readerGetContent(ReaderPointer const, krait_intg);
krait_intg		readerGetPosRead(ReaderPointer const);
krait_intg		readerGetPosWrte(ReaderPointer const);
krait_intg		readerGetPosMark(ReaderPointer const);
krait_intg		readerGetSize(ReaderPointer const);
krait_intg		readerGetInc(ReaderPointer const);
krait_intg		readerGetMode(ReaderPointer const);
krait_byte		readerGetFlags(ReaderPointer const);
krait_intg		readerShowStat(ReaderPointer const);

#endif
