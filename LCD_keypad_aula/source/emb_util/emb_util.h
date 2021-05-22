/***************************************************************************************
 * Module      : emb_util.h
 * Revision    : 1.1
 * Date        : 19/08/2015
 * Description : It contains prototype functions and defines useful for embedded applications.
 * Comments    : None.
 * Author(s)   : Matheus Leitzke Pinto
 ***************************************************************************************/

#ifndef EMB_UTIL_H_
#define EMB_UTIL_H_

#include "string.h"
#include "stdint.h"
#include "stdbool.h"

/*Comment the macros below to not include the correspond function in binary file*/
#define embREVERSE_FUNC //emb_Reverse
#define embITOA //emb_Itoa

/**********************************************************************
 * Function		:	emb_Swap
 *
 * Description	:   A utility function to swap the value of two variables.
 *
 * Inputs		:   x, y: the variables that will be swapped.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
#define emb_Swap(x,y) do \
   { unsigned char swap_temp[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1]; \
     memcpy(swap_temp,&y,sizeof(x)); \
     memcpy(&y,&x,       sizeof(x)); \
     memcpy(&x,swap_temp,sizeof(x)); \
    } while(0)


/**********************************************************************
 * Function		:	emb_Abs
 *
 * Description	:   Calculate the absolute value of a number.
 *
 * Inputs		:   x: the value that will be calculated.
 *
 * Outputs 		:   The absolute value from the number.
 *
 * Comments 	: 	None.
 * ********************************************************************/
#define emb_Abs( x ) (((x) > 0) ? x : -(x))

/**********************************************************************
 * Function		:	emb_Min
 *
 * Description	:   Returns the minimum value between two numbers.
 *
 * Inputs		:   a, b: the numbers.
 *
 * Outputs 		:   The minimum value.
 *
 * Comments 	: 	None.
 * ********************************************************************/
#define emb_Min(a,b) ((a < b) ? a : b)

/**********************************************************************
 * Function		:	emb_ReadConstByte
 *
 * Description	:   Return a byte from the memory program.
 *
 * Inputs		:   addr: the address from memory program.
 *
 * Outputs 		:   the byte.
 *
 * Comments 	: 	None.
 * ********************************************************************/
#define emb_ReadConstByte(addr) (*(const uint8_t *)(addr))

/**********************************************************************
 * Function		:	emb_SetBit
 *
 * Description	:   Set a bit to 1 from the variable.
 *
 * Inputs		:   x: variable.
 * 					i: position of the bit to be set (0 = LSb).
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
#define emb_SetBit(x, i) x |= ( (sizeof( x )1 << i )

/**********************************************************************
 * Function		:	emb_ClrBit
 *
 * Description	:   Clear a bit to 0 from the variable.
 *
 * Inputs		:   x: variable.
 * 					i: position of the bit to be clear (0 = LSb).
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
#define emb_ClrBit(x, i) x &= ((typeof( x ))0xFFFFFF ^ ( (sizeof( x )1 << i ) )

/**********************************************************************
 * Function		:	emb_GetBit
 *
 * Description	:   Return the bit value from the variable.
 *
 * Inputs		:   x: variable.
 * 					i: position of the bit whose value will be returned (0 = LSb).
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
#define emb_GetBit(x, i) x & (1 << i)

/**********************************************************************
 * Function		:	emb_MaskSet
 *
 * Description	:   It makes the corresponding bit of value 1 in mask, being 1
 * 					in the variable.
 *
 * Inputs		:   x: variable.
 * 					m: mask.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
#define emb_MaskSet(x, m) x |= m

/**********************************************************************
 * Function		:	emb_MaskClr
 *
 * Description	:   It makes the corresponding bit of value 1 in mask, being 0
 * 					in the variable.
 *
 * Inputs		:   x: variable.
 * 					m: mask.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
#define emb_MaskClr(x, m) x &= ~m

/**********************************************************************
 * Function		:	emb_MaskReturnSet
 *
 * Description	:   Returns the value of variable masked with 1's bits,
 * 					where the bits from mask is 1.
 *
 * Inputs		:   x: variable.
 * 					m: mask.
 *
 * Outputs 		:   the value of x masked.
 *
 * Comments 	: 	None.
 * ********************************************************************/
#define emb_MaskReturnSet(x, m) x | m

/**********************************************************************
 * Function		:	emb_MaskReturnClr
 *
 * Description	:   Returns the value of variable masked with 0's bits,
 * 					where the bits from mask is 1.
 *
 * Inputs		:   x: variable.
 * 					m: mask.
 *
 * Outputs 		:   the value of x masked.
 *
 * Comments 	: 	None.
 * ********************************************************************/
#define emb_MaskReturnClr(x, m) x & ~m

/**********************************************************************
 * Function		:	emb_MaskReturnGet
 *
 * Description	:   Returns a value that have all bits 0, except the same
 * 					bits from the variable x that have the same position
 * 					of the bits with value 1 from the mask.
 *
 * Inputs		:   x: variable.
 * 					m: mask.
 *
 * Outputs 		:   the value of x masked.
 *
 * Comments 	: 	None.
 * ********************************************************************/
#define emb_MaskGet(x, m) x & m

#ifdef embREVERSE_FUNC
/**********************************************************************
 * Function		:	emb_ReverseStr
 *
 * Description	:   A utility function to reverse a string.
 *
 * Inputs		:   str	  : the string that will be reverted.
 * 					lenght: the lenght of the string.
 *
 * Outputs 		:   None.
 *
 * Comments 	: 	None.
 * ********************************************************************/
void emb_ReverseStr(char *str, int length);
#endif

#ifdef embITOA
 /**********************************************************************
  * Function	:	emb_itoa
  *
  * Description	:   Converts integer into null-terminated string.
  *					It can convert negative numbers too.
  *
  * Inputs		:   str: the string that will represent num.
  *					base: the numerical system that the num will
  * 	 				  be showed:
  * 					  2  - binary;
  * 					  10 - decimal;
  * 					  16 - hex.
  *
  * Outputs 	:   pdTRUE: OK
  * 				pdFALSE.
  *
  * Comments 	: 	None.
  * ********************************************************************/
char* emb_itoa(int num, char* str, uint8_t base);
#endif

/**********************************************************************
 * Function	:	emb_Map
 *
 * Description	:  Re-maps a number from one range to another.
 * 				   That is, a value of fromLow would get mapped to toLow,
 * 				   a value of fromHigh to toHigh, values in-between to
 * 				   values in-between, etc.
 *
 * Inputs		:   x	   : The value that will be mapped.
 * 					in_min : The min value that x originally assumes.
 * 					in_max : The max value that x originally assumes.
 * 					out_min: The min value that x will assumes.
 * 					out_min: The max value that x will assumes.
 *
 * Outputs 		:   The x value mapped.
 *
 * Comments 	: 	None.
 * ********************************************************************/
int32_t emb_Map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);

#endif /* EMB_UTIL_H_ */

/***************************************************************************************
 * END: Module - emb_util.h
 ***************************************************************************************/
