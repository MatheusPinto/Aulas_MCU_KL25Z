/***************************************************************************************
 * Module      : emb_util.c
 * Revision    : 1.1
 * Date        : 19/08/2015
 * Description : It contains functions useful for embedded applications.
 * Comments    : None.
 * Author(s)   : Matheus Leitzke Pinto
 ***************************************************************************************/

#include "emb_util.h"
#include "string.h"

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
void emb_ReverseStr(char *str, int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        emb_Swap(*(str+start), *(str+end));
        start++;
        end--;
    }
}
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
char* emb_itoa(int num, char* str, uint8_t base)
{
    int i = 0;
    uint8_t isNegative = false;

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }

    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    emb_ReverseStr(str, i);

    return str;
}
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
int32_t emb_Map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/***************************************************************************************
 * END: Module - emb_util.c
 ***************************************************************************************/
