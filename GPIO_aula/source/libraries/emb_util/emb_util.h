/**
 * @file	emb_util.h
 * @author  Matheus Leitzke Pinto <matheus.pinto@ifsc.edu.br>
 * @version 1.1
 * @date    2021
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * It contains prototype functions and defines useful for embedded applications.
 *
 */

#ifndef EMB_UTIL_H_
#define EMB_UTIL_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#ifdef __FREERTOS_H
#include "FreeRTOS.h"
#include "task.h"
#else
#include <stdlib.h>
#endif
#include "string.h"
#include "math.h"

/*!
 * @addtogroup emb_utils
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!< The pi number.*/
#define PI_NUMBER	3.1415926535897932384626433832795
/*!< The pi number times 2.*/
#define TWO_PI_NUMBER 6.283185307179586476925286766559

/*Comment the macros below to not include the function in the binary file*/
#define EMB_STR_COPY_FUNC 	  //EmbUtil_StrCopy
#define EMB_STR_CAT_FUNC 	  //EmbUtil_StrCat
#define EMB_CHAR_CAT_FUNC 	  //EmbUtil_CharCat
#define EMB_REVERSE_FUNC 	  //EmbUtil_Reverse
#define EMB_STR_FIND_FUNC 	  //EmbUtil_StrFind
#define EMB_ITOA_FUNC 		  //EmbUtil_ItoA
#define EMB_ATOI_FUNC 		  //EmbUtil_AtoI
#define EMB_FTOA_FUNC 		  //EmbUtil_FtoA
#define EMB_ATOF_FUNC 		  //EmbUtil_AtoF
#define EMB_ETOA_FUNC 		  //EmbUtil_EtoA
#define EMB_ATOE_FUNC 		  //EmbUtil_AtoE
#define EMB_MAP_FUNC 		  //EmbUtil_Map
#define EMB_FLOOR_SQRT_FUNC   //EmbUtil_FloorSqrt
#define EMB_IPOW_FUNC 		  //EmbUtil_IntPow
#define EMBUTIL_ASSERT        //EmbUtil_Assert
#define EMBUTIL_GETVALUE_16LE //EmbUtil_GetValue16LE
#define EMBUTIL_GETVALUE_24LE //EmbUtil_GetValue24LE
#define EMBUTIL_GETVALUE_32LE //EmbUtil_GetValue32LE
#define EMBUTIL_SETVALUE_16LE //EmbUtil_SetValue16LE
#define EMBUTIL_SETVALUE_24LE //EmbUtil_SetValue24LE
#define EMBUTIL_SETVALUE_32LE //EmbUtil_SetValue32LE


/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @name Mathematics
 * @{
 */

/**
 * @brief Compute the cosine of the argument x.
 *        Angles are specified in radians.
 *
 *
 * @param x - a float value.
 *
 * @return the cosine of x in radians.
 */
static inline float EmbUtil_Cos(float x)
{
	return cosf(x);
}

/**
 * @brief Compute the sine of the argument x.
 *        Angles are specified in radians.
 *
 *
 * @param x - a float value.
 *
 * @return the sine of x in radians.
 */
static inline float EmbUtil_Sin(float x)
{
	return sinf(x);
}

/**
 * @brief Computes the inverse tangent (arc tangent) of the input value.
 *
 * @param x - a float value.
 *
 * @return the arc tangent of x in radians, in the range of -pi/2 to pi/2.
 */
static inline float EmbUtil_Atan(float x)
{
	return atanf(x);
}

/**
 * @brief Computes the inverse tangent (arc tangent) of y/x.
 *
 * @note Produces the correct result even for angles near
 *       pi/2 or -pi/2 (that is, when x is near 0).
 *
 * @param x - a float value.
 *
 * @return the arc tangent of x in radians, in the range of -pi to pi.
 */
static inline float EmbUtil_Atan2(float x, float y)
{
	return atan2f(x, y);
}


#ifdef EMB_MAP_FUNC
/**
 * @brief Re-maps an integer number from one range to another.
 *        That is, a value of fromLow would get mapped to toLow,
 *        a value of fromHigh to toHigh, values in-between to
 *        values in-between, etc.
 *
 * @param x	      - The value that will be mapped.
 * @param in_min  - The min value that x originally assumes.
 * @param in_max  - The max value that x originally assumes.
 * @param out_min - The min value that x will assumes.
 * @param out_min - The max value that x will assumes.
 *
 * @return The x value mapped.
 *
 */
int32_t EmbUtil_Map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);
#endif

#ifdef EMB_FLOOR_SQRT_FUNC
/**
 * @brief Gets the floor square root of an integer.
 *
 * @param x	- The operand value.
 *
 * @return The x floor square root.
 *
 */
int EmbUtil_FloorSqrt(int x);
#endif

#ifdef EMB_IPOW_FUNC
/**
 * @brief Calculates the power of a integer number.
 *
 * @param base - The base operand.
 * @param exp  - The exponent operand.
 *
 * @return The base to the power of exp.
 *
 */
int EmbUtil_IntPow(int base, int exp);
#endif

/**
 * @brief Calculates the absolute value of a number.
 *
 * @param x - the value that will be calculated.
 *
 * @return The number absolute value.
 *
 */
#define EmbUtil_Abs( x ) (((x) > 0) ? (x) : (-1*(x)))

/**
 * @brief Returns the minimum value between two numbers.
 *
 * @param a, b - the numbers.
 *
 * @return The minimum value.
 *
 */
#define EmbUtil_Min(a,b) ((a < b) ? a : b)

/*! @}*/

/*!
 * @name Bitwise Operations
 * @{
 */

/**
 * @brief Put the bit 1 (set) in the variable specific bit.
 *
 *
 * @param x - variable.
 * @param i - position of the bit to be set (0 = LSb).
 *
 */
#define EmbUtil_SetBit(x, i) x |= ( (sizeof( x )1 << i )

/**
 * @brief Put the bit 0 (clear) in the variable specific bit.
 *
 *
 * @param x - variable.
 * @param i - position of the bit to be clear (0 = LSb).
 *
 */
#define EmbUtil_ClrBit(x, i) x &= ((typeof( x ))0xFFFFFF ^ ( (sizeof( x )1 << i ) )

/**
 * @brief Return the bit value from the variable.
 *
 * @param x - variable.
 * @param i - position of the bit whose value will be returned (0 = LSb).
 *
 * @return non-zero value (with same size as x) if bit is one;
 *         zero if bit is zero.
 *
 */
#define EmbUtil_GetBit(x, i) (x & (sizeof( x )(1 << i)))

/**
 * @brief It makes the corresponding bit of value 1 in mask, being 1
 * 		  in the variable.
 * *
 * @param x - variable.
 * @param m - mask.
 *
 */
#define EmbUtil_MaskSet(x, m) x |= m

/**
 * @brief It makes the corresponding bit of value 1 in mask, being 0
 * 		  in the variable.
 * *
 * @param x - variable;
 * @param m - mask.
 *
 */
#define EmbUtil_MaskClr(x, m) x &= ~m

/**
 * @brief Returns the value of x variable masked with 1's bits,
 *        where the bits of mask is set as 1.
 *
 * @param x - variable;
 * @param m - mask.
 *
 * @return the value of x masked.
 *
 */
#define EmbUtil_MaskReturnSet(x, m) x | m

/**
 * @brief Returns the value of x variable masked with 0's bits,
 *        where the bits of mask is set as 1.
 *
 * @param x - variable;
 * @param m - mask.
 *
 * @return the value of x masked.
 *
 */
#define EmbUtil_MaskReturnClr(x, m) x & ~m

/**
 * @brief Returns a value that have all bits 0, except the same
 * 		  bits from the variable x that have the same position
 * 		  of the bits with value 1 from the mask.
 *
 * @param x - variable;
 * @param m - mask.
 *
 * @return the value of x masked.
 *
 */
#define EmbUtil_MaskGet(x, m) x & m

#ifdef EMBUTIL_GETVALUE_16LE
/**
 * @brief Returns a 16bit Little Endian value from memory.
 *
 * @param dataP - Pointer to memory
 *
 * @return The Little Endian.
 *
 */
uint16_t EmbUtil_GetValue16LE(uint8_t *dataP);
#endif /* EMBUTIL_GETVALUE_16LE */

#ifdef EMBUTIL_GETVALUE_24LE
 /**
  * @brief Returns a 24bit Little Endian value from memory.
  *
  * @param dataP - Pointer to memory
  *
  * @return The Little Endian.
  *
  */
uint32_t EmbUtil_GetValue24LE(uint8_t *dataP);
#endif /* EMBUTIL_GETVALUE_24LE */

#ifdef EMBUTIL_GETVALUE_32LE
/**
 * @brief Returns a 32bit Little Endian value from memory.
 *
 * @param dataP - Pointer to memory
 *
 * @return The Little Endian.
 *
 */
uint32_t EmbUtil_GetValue32LE(uint8_t *dataP);
#endif /* EMBUTIL_GETVALUE_32LE */

#ifdef EMBUTIL_SETVALUE_16LE
/**
 * @brief Stores a 16bit value in memory as Little Endian.
 *
 * @data  - Value to store
 * @dataP - Pointer to memory
 *
 */
void EmbUtil_SetValue16LE(uint16_t data, uint8_t *dataP);
#endif /* EMBUTIL_SETVALUE_16LE */

#ifdef EMBUTIL_SETVALUE_24LE
/**
 * @brief Stores a 24bit value in memory as Little Endian.
 *
 * @data  - Value to store
 * @dataP - Pointer to memory
 *
 */
void EmbUtil_SetValue24LE(uint32_t data, uint8_t *dataP);
#endif /* EMBUTIL_SETVALUE_24LE */

#ifdef EMBUTIL_SETVALUE_32LE
/**
 * @brief Stores a 32bit value in memory as Little Endian.
 *
 * @data  - Value to store
 * @dataP - Pointer to memory
 *
 */
void EmbUtil_SetValue32LE(uint32_t data, uint8_t *dataP);
#endif /* EMBUTIL_SETVALUE_32LE */


/*! @}*/

/*!
 * @name Miscellaneous
 * @{
 */

/**
 * @brief The function tests the condition parameter.
 *
 *        If it is false, it will trap the application in a
 *        infinite loop. Otherwise the program will follow
 *        its usual flow.
 *
 * @note The loop operation can be modified with another code
 *       based on the user needs.
 *
 * @param expr - The expression being availed.
 *
 */
#ifdef EMBUTIL_ASSERT
	#ifdef __FREERTOS_H
		#define EmbUtil_Assert(expr) configASSERT(expr)
	#else
		#define EmbUtil_Assert(expr) {if(expr) (void)0; else for(;;);}
	#endif
#else
	#define EmbUtil_Assert(expr) (void)0
#endif

/**
 * @brief Swap the value of two variables.
 *
 * @param x - swap variable.
 * @param y - swap variable.
 *
 */
#define EmbUtil_Swap(x,y) do \
   { unsigned char swap_temp[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1]; \
     memcpy(swap_temp,&y,sizeof(x)); \
     memcpy(&y,&x,       sizeof(x)); \
     memcpy(&x,swap_temp,sizeof(x)); \
    } while(0)

/**
 * @brief Gets the time difference in timer implementation specific ticks.
 *
 *        The function was think for timer maximum count below 65536,
 *        because this timers usually overflowed in application. Thus,
 *        it is necessary to take to account when overflow happens.
 *
 * @param before - the time value in ticks obtained in a previous measure.
 * @param now    - the time value in ticks currently obtained.
 * @param max    - the maximum timer value in ticks.
 *
 * @return The difference in time between before and now.
 *
 * @note The function will not calculate correctly if "now" was obtained after
 *       two or more counting overflows.
 */
inline uint16_t EmbUtil_GetTimeDiff(uint16_t before, uint16_t now, uint16_t max)
{
	uint16_t diff;

	if(now < before)
	{
		diff = (max - before) + now + 1;
	}
	else
	{
		diff = now - before;
	}

	return diff;
}

/**
 * @brief Allocates memory dynamically.
 *
 * @param x - the memory size in bytes.
 *
 * @return The allocated memory block first position, or;
 *         NULL if there is no memory space.
 *
 */
#ifdef __FREERTOS_H
#define EmbUtil_Malloc(x) pvPortMalloc(x)
#else
#define EmbUtil_Malloc(x) malloc(x)
#endif

/**
 * @brief Free a memory block allocated dynamically.
 *
 * @param x - the memory block first position.
 *
 */
#ifdef __FREERTOS_H
#define EmbUtil_Free(x) vPortFree(x)
#else
#define EmbUtil_Free(x) free(x)
#endif


/**
 * @brief Returns a byte from the program memory.
 *
 * @note This function is useful in some hardware
 *       architectures, where constants and variables
 *       are located in different memories that have
 *       the same memory map.
 *
 * @param addr - the address from the constant byte.
 *
 * @return The byte.
 *
 */
#define EmbUtil_ReadConstByte(addr) (*(const uint8_t *)(addr))

/*! @}*/

/*!
 * @name String Manipulation
 * @{
 */


 /**
 * @brief Returns the string length.
 *
 * @Note: The null terminated string is not counted.
 * @Precondition: Null terminated string.
 *
 * @param str - The null terminated string.
 *
 * @return The string length.
 *
 */
#define EmbUtil_StrLen(str) strlen(str)


 /**
 * @brief Compares two strings.
 * *
 * @param str1, str2 - The strings to compare.
 *
 * @return - If str1 sorts lexicographically after
 *           str2, returns a number greater than zero.
 *         - If the two strings match, returns zero.
 *         - If str1 sorts lexicographically before str2,
 *           returns a number less than zero.
 *
 */
#define EmbUtil_StrCmp(str1, str2) strcmp(str1, str2)


 /**
 * @brief Compares up to length characters from the
 *        string at str1 to the string at str2.
 * *
 * @param str1, str2 - The strings to compare.
 *
 * @return - If str1 sorts lexicographically after
 *           str2, returns a number greater than zero.
 *         - If the two strings match, returns zero.
 *         - If str1 sorts lexicographically before str2,
 *           returns a number less than zero.
 *
 */
#define EmbUtil_StrNCmp(str1, str2, len) strncmp(str1, str2, len)


#ifdef EMB_STR_COPY_FUNC
/**
* @brief Copy the string src into dst. It performs the same task as strncpy,
*        except:
*           - always terminates the result string.
*           - does not zero out the remaining part in dst.
*
* @Note: dstSize is the size of dst INCLUDING zero byte.
* @Precondition: src, dst != NULL
*
* @param dst     - Start of string buffer, where to append the number string.
* @param dstSize - The size of the buffer, including the zero byte.
* @param src - The source string to copy.
*
*/
void EmbUtil_StrCopy(uint8_t *dst, size_t dstSize, const char *src);
#endif /* EMB_STR_COPY_FUNC */


#ifdef EMB_STR_CAT_FUNC
/**
 * @brief Concat the string src into dst. Always terminates the result string.
 *
 * @Note: count is the size of dst INCLUDING zero byte.
 * @Precondition: src, dst != NULL
 *
 * @param dst     - Start of string buffer, where to append the number string.
 * @param dstSize - The size of the buffer, including the zero byte.
 * @param src - The source string to add.
 *
 */
void EmbUtil_StrCat(uint8_t *dst, size_t dstSize, const unsigned char *src);
#endif /* EMB_STR_CAT_FUNC */


#ifdef EMB_CHAR_CAT_FUNC
/**
 * @brief Adds a single character to a zero byte terminated string
 *         buffer. It cares about buffer overflow.
 *
 * @Note: count is the size of dst INCLUDING zero byte.
 * @Precondition: src, dst != NULL
 *
 * @param dst     - Start of string buffer, where to append the character.
 * @param dstSize - The size of the buffer, including the zero byte.
 * @param src - The character to append.
 *
 */
void EmbUtil_CharCat(uint8_t *dst, size_t dstSize, uint8_t ch);
#endif /* EMB_CHAR_CAT_FUNC */

#ifdef EMB_REVERSE_FUNC
/**
 * @brief A function to reverse a string.
 *
 * @param str - the string that will be reverted.
 * @param length - the string length.
 *
 */
void EmbUtil_ReverseStr(uint8_t *str, size_t length);
#endif


#ifdef EMB_STR_FIND_FUNC
/**
 * @brief Searches a substring inside a string and returns the
 *        position.
 *
 * @param str  - the string that will be searched.
 * @param subStr - substring to search inside str.
 *
 * @return -1 if not found,
 *          otherwise the character index.
 *
 */
int16_t EmbUtil_StrFind(uint8_t *str, uint8_t *subStr);
#endif /* EMB_STR_FIND_FUNC */


#ifdef EMB_ITOA_FUNC
 /**
  * @brief Converts integer into null-terminated string.
  *		   It can convert negative numbers too.
  *
  * @param num  - the integer number.
  * @param str  - the string that will represent the number.
  * @param base - the numerical system that the num will be showed:
  * 		      \a 2 - binary;
  * 			  \a 10 - decimal;
  * 			  \a 16 - hex.
  *
  * @return The string length.
  *
  */
int EmbUtil_ItoA(int32_t num, uint8_t* str, uint8_t base);
#endif

#ifdef EMB_ATOI_FUNC
 /**
  * @brief Converts a null-terminated string representing a number
  *        into an integer value.
  *		   It can convert negative numbers too.
  *
  * @param str  - Pointer to string to scan. Returns until
  *               where it has scanned.
  * @param res  - Pointer to where to store the result
  *
  * @return \a true  - if conversion was ok;
  *         \a false - otherwise.
  *
  */
bool EmbUtil_AtoI(const unsigned char** str, int32_t* res);
#endif

#if defined(EMB_ITOA_FUNC) && defined(EMB_FTOA_FUNC)
/**
 * @brief Converts floating-point number into null-terminated string.
 *
 * @param n - input number.
 * @param res - array where output string to be stored.
 * @param afterPoint - number of digits to be considered after the point.
 *
 * @return The string length.
 *
 */
int embUtil_FtoA(float n, uint8_t *res, int afterPoint);
#endif

#ifdef EMB_ATOF_FUNC
/**
 * @brief Converts a null-terminated string representing a number
 *        into an float value.
 *		   It can convert negative numbers too.
 *
 * @param str  - Pointer to string to scan. Returns until
 *               where it has scanned.
 * @param res  - Pointer to where to store the result
 *
 * @return \a true  - if conversion was ok;
 *         \a false - otherwise.
 *
 */
bool EmbUtil_AtoF(const unsigned char** str, float* res);
#endif

/*! @}*/


/*! @}*/

#endif /* EMB_UTIL_H_ */
