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
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Sep 30 2022
* Author: Quoc Thang Tran
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */


#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

 /*
 ***********************************************************
 * Function name: readerCreate
 * Purpose: Creates the buffer reader according to capacity, increment
	 factor and operational mode ('f', 'a', 'm')
 * Author: Quoc Thang Tran
 * History/Versions: F22
 * Called functions: calloc(), malloc()
 * Parameters:
 *   size = initial capacity
 *   increment = increment factor
 *   mode = operational mode
 * Return value: bPointer (pointer to reader)
 * Algorithm: Allocation of memory according to inicial (default) values.
 * TODO ......................................................
 *	- Adjust datatypes for your LANGUAGE.
 *   - Use defensive programming
 *	- Check boundary conditions
 *	- Check flags.
 *************************************************************
 */

ReaderPointer readerCreate(krait_intg size, krait_intg increment, krait_intg mode) {
	ReaderPointer readerPointer;
	/* TO_DO: Defensive programming */
	/* TO_DO: Adjust the values according to parameters */
	if (!size) {
		size = READER_DEFAULT_SIZE;
		if(!increment)
			mode = MODE_FIXED;
		increment = READER_DEFAULT_INCREMENT;
	}
	if (mode != MODE_ADDIT && mode != MODE_FIXED && mode != MODE_MULTI)
		return NULL;
	readerPointer = (ReaderPointer)calloc(1, sizeof(BufferReader));
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return NULL;
	readerPointer->content = (krait_char*)malloc(size);
	/* TO_DO: Defensive programming */
	if (!(readerPointer->content))
		return NULL;
	/* TO_DO: Initialize the histogram */
	readerPointer->size = size;
	readerPointer->increment = increment;
	readerPointer->mode = mode;
	/* TO_DO: Initialize flags */
	readerPointer->flags = READER_DEFAULT_FLAG;
	/* TO_DO: The created flag must be signalized as EMP */
	readerPointer->flags |= SET_BIT2;
	return readerPointer;
}


/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a char to buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   ch = char to be added
* Return value:
*	readerPointer (pointer to Buffer Reader)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

ReaderPointer readerAddChar(ReaderPointer const readerPointer, krait_char ch) {
	krait_char* tempReader = NULL;
	krait_intg newSize = 0;
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return NULL;
	if (ch < 0 || ch > 127) {
		return NULL;
	}
	/* TO_DO: Reset Realocation */
	readerPointer->flags &= RESET_BIT1;
	/* TO_DO: Test the inclusion of chars */
	if (readerPointer->position.wrte * (krait_intg)sizeof(krait_char) < readerPointer->size) {
		/* TO_DO: This buffer is NOT full */
	}
	else {
		/* TO_DO: Reset Full flag */
		readerPointer->flags &= RESET_BIT3;
		switch (readerPointer->mode) {
		case MODE_FIXED:
			if (ch == '\0') {
				readerPointer->content[--readerPointer->position.wrte] = ch;
				readerPointer->position.wrte++;
				readerPointer->flags |= SET_BIT3;
				return readerPointer;
			}
			return NULL;
		case MODE_ADDIT:
			/* TO_DO: Adjust new size */
			newSize = readerPointer->size + readerPointer->increment;
			/* TO_DO: Defensive programming */
			if (newSize >= READER_MAX_SIZE || newSize < 0) {
				readerPointer->flags |= SET_BIT3;
				return readerPointer;
			}
			break;
		case MODE_MULTI:
			/* TO_DO: Adjust new size */
			newSize = readerPointer->size * readerPointer->increment;
			/* TO_DO: Defensive programming */
			if (newSize >= READER_MAX_SIZE || newSize < 0) {
				readerPointer->flags |= SET_BIT3;
				return readerPointer;
			}
			break;
		default:
			return NULL;
		}
		/* TO_DO: New reader allocation */
		/* TO_DO: Defensive programming */
		tempReader = realloc(readerPointer->content, newSize);
		if (!tempReader)
			return NULL;
		/* TO_DO: Check Relocation */
		if (tempReader != readerPointer->content) {
			readerPointer->flags |= SET_BIT1;
		}
			readerPointer->content = tempReader;
			readerPointer->size = newSize;
	}
	/* TO_DO: Add the char */
	readerPointer->content[readerPointer->position.wrte++] = ch;
	/* TO_DO: Updates histogram */
	readerPointer->histogram[ch] += 1;
	return readerPointer;
}

/*
***********************************************************
* Function name: readerClear
* Purpose: Clears the buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
krait_boln readerClear(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return KRAIT_FALSE;
	/* TO_DO: Adjust flags original */
	readerPointer->flags = READER_DEFAULT_FLAG;
	readerPointer->position.mark, readerPointer->position.read, readerPointer->position.wrte = 0;
	return KRAIT_TRUE;
}

/*
***********************************************************
* Function name: readerFree
* Purpose: Releases the buffer address
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
krait_boln readerFree(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return KRAIT_FALSE;
	/* TO_DO: Free pointers */
	free(readerPointer->content);
	free(readerPointer);
	return KRAIT_TRUE;
}

/*
***********************************************************
* Function name: readerIsFull
* Purpose: Checks if buffer reader is full
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
krait_boln readerIsFull(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return KRAIT_FALSE;
	/* TO_DO: Check flag if buffer is FUL */
	if ((readerPointer->flags & CHECK_BIT3) == CHECK_BIT3)
		return KRAIT_TRUE;
	return KRAIT_FALSE;
}


/*
***********************************************************
* Function name: readerIsEmpty
* Purpose: Checks if buffer reader is empty.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
krait_boln readerIsEmpty(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return KRAIT_FALSE;
	/* TO_DO: Check flag if buffer is EMP */
	if ((readerPointer->flags & CHECK_BIT2) == CHECK_BIT2)
		return KRAIT_TRUE;
	return KRAIT_FALSE;
}

/*
***********************************************************
* Function name: readerSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
krait_boln readerSetMark(ReaderPointer const readerPointer, krait_intg mark) {
	/* TO_DO: Defensive programming */
	if (!readerPointer || mark<=0 || mark >= readerPointer->position.wrte)
		return KRAIT_FALSE;
	/* TO_DO: Adjust mark */
	readerPointer->position.mark = mark;
	return KRAIT_TRUE;
}


/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
krait_intg readerPrint(ReaderPointer const readerPointer) {
	krait_intg cont = 0;
	krait_char c;
	/* TO_DO: Defensive programming (including invalid chars) */
	c = readerGetChar(readerPointer);
	while (c < 0 || c > 127) {
		c = readerGetChar(readerPointer);
	}
	/* TO_DO: Check flag if buffer EOB has achieved */
	while (cont < readerPointer->position.wrte) {
		cont++;
		printf("%c", c);
		if ((readerPointer->flags & CHECK_BIT0) == CHECK_BIT0)
			break;
		c = readerGetChar(readerPointer);
	}
	return cont;
}

/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
krait_intg readerLoad(ReaderPointer const readerPointer, FILE* const fileDescriptor) {
	krait_intg size = 0;
	krait_char c;
	/* TO_DO: Defensive programming */
	if (!readerPointer || !fileDescriptor)
		return READER_ERROR;
	c = (krait_char)fgetc(fileDescriptor);
	while (!feof(fileDescriptor)) {
		if (!readerAddChar(readerPointer, c)) {
			if (c < 0 || c > 127) {}
			else {
				ungetc(c, fileDescriptor);
				if (readerPointer->position.wrte == readerPointer->size) {
					readerPointer->flags |= SET_BIT3;
					break;
				}
			return READER_ERROR;
			}
		}
		c = (krait_char)fgetc(fileDescriptor);
		size++;
	}
	/* TO_DO: Defensive programming */
	if (size < 0 || size > READER_MAX_SIZE)
		return READER_ERROR;
	if(size!=0)
		readerPointer->flags &= RESET_BIT2;
	return size;
}


/*
***********************************************************
* Function name: readerRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
krait_boln readerRecover(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return KRAIT_FALSE;
	/* TO_DO: Recover positions */
	readerPointer->position.mark, readerPointer->position.read = 0;
	return KRAIT_TRUE;
}


/*
***********************************************************
* Function name: readerRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
krait_boln readerRetract(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer || readerPointer->position.read<=0 )
		return KRAIT_FALSE;
	/* TO_DO: Retract (return 1 pos read) */
	readerPointer->position.read -= 1;
	return KRAIT_TRUE;
}


/*
***********************************************************
* Function name: readerRestore
* Purpose: Resets the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
krait_boln readerRestore(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return KRAIT_FALSE;
	/* TO_DO: Restore positions (read/mark) */
	readerPointer->position.read = readerPointer->position.mark;
	return KRAIT_TRUE;
}



/*
***********************************************************
* Function name: readerGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
krait_char readerGetChar(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer)
		return '\0';
	/* TO_DO: Check condition to read/wrte */
	if (readerPointer->position.read == readerPointer->position.wrte) {
	/* TO_DO: Set EOB flag */
		readerPointer->flags |= SET_BIT0;
		return '\0';
	}
	/* TO_DO: Reset EOB flag */
	readerPointer->flags &= RESET_BIT0;
	return readerPointer->content[readerPointer->position.read++];
}


/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
krait_char* readerGetContent(ReaderPointer const readerPointer, krait_intg pos) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Return content (string) */
	if (readerPointer && pos >= 0 && pos < readerPointer->position.wrte) {
		return(readerPointer->content+pos);
	}
	return NULL;
}



/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the value of getCPosition.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The read position offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
krait_intg readerGetPosRead(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Return read */
	if (readerPointer)
		return(readerPointer->position.read);
	return 0;
}


/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Write position
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
krait_intg readerGetPosWrte(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Return wrte */
	if (readerPointer)
		return(readerPointer->position.wrte);
	return 0;
}


/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Mark position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
krait_intg readerGetPosMark(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Return mark */
	if (readerPointer)
		return(readerPointer->position.mark);
	return 0;
}


/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
krait_intg readerGetSize(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Return size */
	if (readerPointer)
		return(readerPointer->size);
	return 0;
}

/*
***********************************************************
* Function name: readerGetInc
* Purpose: Returns the buffer increment.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The Buffer increment.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
krait_intg readerGetInc(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Return increment */
	if (readerPointer)
		return(readerPointer->increment);
	return 0;
}

/*
***********************************************************
* Function name: readerGetMode
* Purpose: Returns the operational mode
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Operational mode.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
krait_intg readerGetMode(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Return mode */
	if (readerPointer)
		return(readerPointer->mode);
	return 0;
}


/*
***********************************************************
* Function name: readerGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Flags from Buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
krait_byte readerGetFlags(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Return flags */
	if (readerPointer)
		return(readerPointer->flags);
	return 0;
}


/*
***********************************************************
* Function name: readerShowStat
* Purpose: Returns the number of chars used.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
krait_intg readerShowStat(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Updates the histogram */
	if (readerPointer) {
		krait_intg count = 0;
		for (krait_intg i = 0; i < 128; i++) {
			if (readerPointer->histogram[i] != 0)
				count++;
		}
		return count;
	}
	return 0;
}
