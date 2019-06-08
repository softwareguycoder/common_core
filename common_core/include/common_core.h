////////////////////////////////////////////////////////////////////////////////////////////////////
// common_core.h - Include file for data types, constants, and other things that are used throughout
// our stack of libraries and applications

#ifndef __COMMON_CORE_H__
#define __COMMON_CORE_H__

#include "stdafx.h"

/**
 * @brief Symbol indicating the value to be returned on successful execution.
 */
#ifndef OK
#define OK			0
#endif

/**
 * @brief Symbol idnicating the value to be returned if an error occurs.
 */
#ifndef ERROR
#define ERROR		-1
#endif

/**
 * @brief Checks whether the specified substring is contained within the
 * specified string.
 * @param pszString	String value to check.
 * @param pszSubstring The substring to look for.
 * @returns TRUE if the pszString contains the pszSubstring; FALSE otherwise.
 * @remarks This function performs a case-sensitive comparison.
 */
BOOL Contains(const char* pszString, const char* pszSubstring);

/**
 * @brief Checks whether the specified substring is contained within the
 * specified string, without regards to case.
 * @param pszString	String value to check.
 * @param pszSubstring The substring to look for.
 * @returns TRUE if the pszString contains the pszSubstring; FALSE otherwise.
 * @remarks This function performs a case-insensitive comparison.
 */
BOOL ContainsNoCase(const char* pszString, const char* pszSubstring);

/**
 * @brief Clears a char array to be filled with null terminator characters.
 * @param pszBuffer Pointer to the array to be filled with zero.
 * @param nSize Length of the buffer, in bytes.
 * @remarks This is basically just a wrapper for memset.
 */
void ClearString(char* pszBuffer, int nSize);

/**
 * @brief Compares two strings to each other to see if they match (case-
 * sensitive).
 * @param pszDest One of the strings to compare against.
 * @param pszSrc The other string to compare.
 * @returns TRUE if the strings match, as a case-sensitive comparison; FALSE
 * otherwise.
 */
BOOL Equals(const char* pszDest, const char* pszSrc);

/**
 * @brief Compares two strings to each other to see if they match (case-
 * insensitive).
 * @param pszDest One of the strings to compare against.
 * @param pszSrc The other string to compare.
 * @returns TRUE if the strings match, as a case-insensitive comparison; FALSE
 * otherwise.
 */
BOOL EqualsNoCase(const char* pszDest, const char* pszSrc);
/**
 * @brief Formats the current date and time according to the format string.
 * @param pszBuffer Address of the storage where the result is to be placed.
 * @param nSize Length of the buffer, in bytes.
 * @param pszFormat Format string to be passed to strftime.
 */
void FormatDate(char* pszBuffer, int nSize, const char* pszFormat);

/**
 * @brief Frees the memory at the address specified.
 * @param ppBuffer Address of a pointer which points to memory
 * allocated with the '*alloc' functions (malloc, calloc, realloc).
 * @remarks Remember to cast the address of the pointer being passed
 * to this function to void**
 */
void FreeBuffer(void **ppBuffer);

/**
 * @brief Frees the string array located at the address specified.
 * @param pppszStringArray Address of a char** array of strings, each
 * element of which has been previously allocated with malloc.
 * @param nElementCount Count of elements that are present in the string
 * array.  Must be a number greater than zero.
 * @remarks Iterates through the provided string array and frees each element.
 * Then the entire block of memory containing the string array is freed,
 * and the pppszStringArray pointer's value is set to NULL.
 */
void FreeStringArray(char*** pppszStringArray, int nElementCount);

/**
 * @name GetSubstringOccurrenceCount
 * @brief Determines the number of times a substring appears in a string.
 * @param pszSrc String to examine.
 * @param pszFindWhat Substring to check for.
 * @return Number of occurrences of pszFindWhat in pszSrc.
 */
int GetSubstringOccurrenceCount(const char* pszSrc, const char* pszFindWhat);

/**
 * @brief Executes the shell command and returns an array of the lines of
 * output returned by it.
 * @param pszCommand The shell command to execute with Bash.
 * @param pppszOutputLines Location of storage that receives the address of
 * an array of strings that contains one element per line of output returned.
 * @param pnOutputLineCount Address of an integer variable that receives the
 * count of lines returned.  Required.
 * @remarks Shout out to user14038 on Stack Overflow for the inspiration.
 */
void GetSystemCommandOutput(const char* pszCommand,
    char*** pppszOutputLines, int *pnOutputLineCount);

/**
 *  @brief Reports the error message specified as well as the error from
 *  the system. Exits the program with the ERROR exit code.
 *  @param pszErrorMessage Additional error text to be echoed to the console.
 **/
void HandleError(const char* pszErrorMessage);

/**
 * @brief Tells whether a string buffer contains only letters and numbers.
 * @param pszTest Pointer to the string to check.
 * @returns TRUE if the string contains only letters and numbers; FALSE
 * otherwise, i.e., if the string contains spaces or special characters.
 */
BOOL IsAlphaNumeric(const char* test);

/**
 * @brief Tells if a string pointer is NULL or is only whitespace.
 * @param pszTest Pointer to the string to check.
 * @returns TRUE if the string is NULL or whitespace; FALSE otherwise.
 */
BOOL IsNullOrWhiteSpace(const char* pszTest);

/**
 * @brief Returns a value indicating whether the string specified represents a
 * numeric value.
 * @param pszTest Pointer to the string to check.
 * @returns TRUE if the string represents a numeric value; FALSE otherwise.
 */
BOOL IsNumeric(const char* pszTest);

/**
 * @brief Returns a value indicating whether the string specified represents a
 * all-uppercase value.
 * @param pszTest Pointer to the string to check.
 * @returns TRUE if the string represents a numeric value; FALSE otherwise.
 */
BOOL IsUppercase(const char* pszTest);
/**
 * @brief Tells which of the two integer values passed is the smaller of the two.
 * @param a The first integer value to be checked.
 * @param b The second integer value to be checked.
 * @returns If a < b, then a is returned. If a = b, a is returned.  If b < a, then b is returned.
 * @remarks This function compares two values and returns the value which is the smaller
 * of the two.  If they are equal, then both are returned.
 */
int MinimumOf(int a, int b);

/**
 * @brief Prepends a string to another string.
 * @param ppszDest Memory location that receives the address of the resultant string.
 * @param pszPrefix Prefix to be prepended to the source string.
 * @param pszSrc The string you want the prefix prepended to.
 * @remarks Concatinates the pszPrefix and pszSrc strings and places the address
 * of the first string of the resultant into the location referenced by
 * ppszDest.  All parameters are required.  The string referenced by ppszDest
 * must be freed after use.
 */
void PrependTo(char** ppszDest, const char* pszPrefix, const char* pszSrc);

/**
 * @brief Splits a specified string into tokens based on given delimiters.
 * @param pszStringToSplit String to be tokenized.
 * @param pszDelimiters String is split on the delimiters found in this
 * string.
 * @param pppszStrings Memory location to be filled with the address of an
 * array of character strings containing the tokens.
 * @param pnResultCount Count of tokens that were found.
 * @remarks This function is basically a nice wrapper for strtok(3). Be sure
 * to call free() on each element of the array of strings returned, as well
 * as the array itself, when you're done using the data.  The pszStringToSplit
 * and pszDelimiters are not allowed to be NULL or whitespace characters only;
 * if this is the case for either one of them or both, the Split function
 * does nothing.
 */
void Split(char* pszStringToSplit, const char* pszDelimiters,
  char*** pppszStrings, int* pnResultCount);

/**
 * @brief Checks to see whether one string begins with another.
 * @param str String to be examined.
 * @param startsWith The prefix to be checked.
 * @returns TRUE if the string in str begins with the string in startsWith.
 */
BOOL StartsWith(const char *str, const char *startsWith);

/**
 * @name StringReplace
 * @brief Replaces all occurrences of a substring with another string in a
 * given larger character string.
 * @param pszSrc Address of the character string in which to do
 * the replacement.
 * @param pszFindWhat Substring to locate and replace occurrences of.
 * @param pszReplaceWith Address of a sequence of characters to replace the
 * substring identiied by pszFindWhat with.
 * @param ppszResult Address of a pointer to be filled with the location of
 * the resultant string.
 * @remarks This function gives up if passed a blank string for either of
 * pszSrc or pszFindWhat. In such case, the value of the pointer referred
 * to by ppszResult will remain unchanged. */
void StringReplace(const char* pszSrc,
    const char* pszFindWhat, const char* pszReplaceWith,
    char** ppszResult);


/**
 * @brief Returns a new string in which all leading and trailing occurrences
 * of a set of specified characters from the current String object are removed.
 * @param out Pointer to a buffer that will receive the trimmed output.
 * @param len Size of the output buffer.  This must be at least as big as the
 * strlen of the str buffer.
 * @param str Pointer to a memory location containing the string to be trimmed.
 */
void Trim(char *out, size_t len, const char *str);

#endif /* __COMMON_CORE_H__ */
