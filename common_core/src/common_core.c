// common_core.c - Implementations of commonly-used functions

#include "stdafx.h"
#include "common_core.h"

///////////////////////////////////////////////////////////////////////////////
// Internal-use-only functions

///////////////////////////////////////////////////////////////////////////////
// Publicly-exposed functions

///////////////////////////////////////////////////////////////////////////////
// Contains function - Checks whether one string contains another (case-
// sensitive).
//

BOOL Contains(const char* pszString, const char* pszSubstring) {
  if (IsNullOrWhiteSpace(pszString)) {
    return FALSE;
  }

  if (IsNullOrWhiteSpace(pszSubstring)) {
    return FALSE;
  }

  return strstr(pszString, pszSubstring) != NULL;
}

///////////////////////////////////////////////////////////////////////////////
// ContainsNoCase function - Does the same thing as Contains but ignores case.
//

BOOL ContainsNoCase(const char* pszString, const char* pszSubstring) {
  if (IsNullOrWhiteSpace(pszString)) {
    return FALSE;
  }

  if (IsNullOrWhiteSpace(pszSubstring)) {
    return FALSE;
  }

  return strcasestr(pszString, pszSubstring) != NULL;
}

///////////////////////////////////////////////////////////////////////////////
// ClearString function

void ClearString(char* pszBuffer, int nSize) {
  if (IsNullOrWhiteSpace(pszBuffer)) {
    // Nothing to do, string is already blank
    return;
  }

  if (nSize <= 0) {
    ThrowArgumentOutOfRangeException("nSize");
  }

  memset(pszBuffer, 0, nSize);
}

///////////////////////////////////////////////////////////////////////////////
// Equals function - Are strings equal to each other?

BOOL Equals(const char* pszDest, const char* pszSrc) {
  return strcmp(pszDest, pszSrc) == 0;
}

///////////////////////////////////////////////////////////////////////////////
// EqualsNoCase function - Are strings equal to each other? (case-insensitive)

BOOL EqualsNoCase(const char* pszDest, const char* pszSrc) {
  return strcasecmp(pszDest, pszSrc) == 0;
}

///////////////////////////////////////////////////////////////////////////////
// FormatDate function - Formats the current system date/time into a string.
//

void FormatDate(char* pszBuffer, int nSize, const char* pszFormat) {
  if (pszBuffer == NULL || nSize <= 0) {
    fprintf(stderr, "FormatDate: Invalid buffer and/or size passed.\n");
    exit(ERROR);
  }

  if (IsNullOrWhiteSpace(pszFormat)) {
    fprintf(stderr, "FormatDate: Format string is missing.\n");
    exit(ERROR);
  }

  time_t now;

  struct tm* tm_info;

  time(&now);
  tm_info = localtime(&now);

  strftime(pszBuffer, nSize, pszFormat, tm_info);
}

///////////////////////////////////////////////////////////////////////////////
// FreeBuffer function - Frees the memory at the specified address.
//

/**
 * @brief Frees the memory at the address specified.
 * @param ppBuffer Address of a pointer which points to memory
 * allocated with the '*alloc' functions (malloc, calloc, realloc).
 * @remarks Remember to cast the address of the pointer being passed
 * to this function to void**
 */
void FreeBuffer(void **ppBuffer) {
  if (ppBuffer == NULL || *ppBuffer == NULL) {
    return;     // Nothing to do since there is no address referenced
  }

  free(*ppBuffer);
  *ppBuffer = NULL;
}

///////////////////////////////////////////////////////////////////////////////
// FreeStringArray function

void FreeStringArray(char*** pppszStringArray, int nElementCount) {
  if (pppszStringArray == NULL
      || *pppszStringArray == NULL) {
    return;
  }

  if (nElementCount <= 0) {
    return; /* a count must be a positive integer */
  }

  for (int i = 0; i < nElementCount; i++) {
    if (IsNullOrWhiteSpace((*pppszStringArray)[i])) {
      continue;
    }
    free((*pppszStringArray)[i]);
    (*pppszStringArray)[i] = NULL;
  }

  free(*pppszStringArray);
  *pppszStringArray = NULL;
}

///////////////////////////////////////////////////////////////////////////////
// GetSubstringOccurrenceCount function

int GetSubstringOccurrenceCount(const char* pszSrc,
    const char* pszFindWhat) {
  int nResult = 0;
  if (pszSrc == NULL || pszSrc[0] == '\0') {
    // NOTE: do not use IsNullOrWhiteSpace here to check pszSrc
    return nResult; // Required parameter
  }

  if (pszFindWhat == NULL || pszFindWhat[0] == '\0') {
    // NOTE: do not use IsNullOrWhiteSpace here to check pszFindWhat
    return nResult; // Required parameter
  }

  const int FIND_WHAT_LEN = strlen(pszFindWhat);

  for (int i = 0; i < strlen(pszSrc)
      && pszSrc[i] != '\0'; i++) {
    if (strstr(&pszSrc[i], pszFindWhat) != &pszSrc[i]) {
      continue;
    }
    nResult++;
    i += FIND_WHAT_LEN;
  }

  return nResult;
}

///////////////////////////////////////////////////////////////////////////////
// GetSystemCommandOutput function

#define SYSTEM_COMMAND_OUTPUT_LINE_LENGTH 80

void GetSystemCommandOutput(const char* pszCommand,
    char*** pppszOutputLines, int *pnOutputLineCount) {

  FILE *fp = NULL;

  char curline[SYSTEM_COMMAND_OUTPUT_LINE_LENGTH];
  memset(curline, 0, SYSTEM_COMMAND_OUTPUT_LINE_LENGTH);

  if (IsNullOrWhiteSpace(pszCommand)) {
    return;
  }

  if (pppszOutputLines == NULL) {
    return;
  }

  if (pnOutputLineCount == NULL) {
    return;
  }

  *pnOutputLineCount = 0; /* number of lines processed */
  *pppszOutputLines = NULL;

  fp = popen(pszCommand, "r");

  if (fp == NULL)
  {
    fprintf(stderr, "ERROR: Failed to run command\n");
    exit(EXIT_FAILURE);
  }

  /* Read the output a line at a time - output it. */
  while (fgets(curline, SYSTEM_COMMAND_OUTPUT_LINE_LENGTH, fp) != NULL)
  {
    if (IsNullOrWhiteSpace(curline)) {
      /* skip over blank lines */
      continue;
    }

    const int ARRAY_SIZE = (*pnOutputLineCount + 1);
    /* Add another element to the string array of lines */
    *pppszOutputLines = (char**) realloc(*pppszOutputLines,
        ARRAY_SIZE * sizeof(char*));
    const int LINE_SIZE = strlen(curline) + 1;
    (*pppszOutputLines)[*pnOutputLineCount] =
        (char*) malloc(LINE_SIZE * sizeof(char));

    strcpy((*pppszOutputLines)[*pnOutputLineCount], curline);
    (*pnOutputLineCount)++;

    /* blank out the curline again so that it can receive
     * new data */
    memset(curline, 0, SYSTEM_COMMAND_OUTPUT_LINE_LENGTH);
  }

  pclose(fp);
  fp = NULL;

  /* Now, pppszOutputLines contains the address of an array of strings
   * that contains the lines outputted by the system command.  A free() call
   * should be done on the array pointed to by pppszOutputLines before
   * the program exits. */
}

///////////////////////////////////////////////////////////////////////////////
// HandleError function

void HandleError(const char* pszErrorMessage) {
  if (IsNullOrWhiteSpace(pszErrorMessage)) {
    return;
  }

  fprintf(stderr, "%s\n", pszErrorMessage);

  perror(NULL);
  exit(ERROR);
}

///////////////////////////////////////////////////////////////////////////////
// IsAlphaNumeric function - Examines the string in pszTest for whether it
// only contains letters and/or numbers.  No spaces or special characters are
// allowed.  Returns FALSE if the pszTest variable is NULL or it contains only
// whitespace.
//

BOOL IsAlphaNumeric(const char* pszTest) {
  if (IsNullOrWhiteSpace(pszTest)) {
    return FALSE;	// Surely, a blank string cannot be alphanumeric!
  }

  // The string pszTest is alphanumeric if and only if
  // every character is either a letter or a number.
  // Anything else anywhere else in the string, and too bad.
  while (*pszTest != 0) {
    if (!isalnum(*pszTest)) {
      return FALSE;
    }
    pszTest++;
  }
  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// IsNullOrWhiteSpace function

BOOL IsNullOrWhiteSpace(const char* pszTest) {
  if (pszTest == NULL || strlen(pszTest) == 0) {
    return TRUE;
  }

  char szTrimResult[strlen(pszTest) + 1];
  Trim(szTrimResult, strlen(pszTest) + 1, pszTest);
  if (strlen(szTrimResult) == 0) {
    return TRUE;
  }

  return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// IsNumeric function

BOOL IsNumeric(const char* pszTest) {
  if (IsNullOrWhiteSpace(pszTest)) {
    return FALSE;
  }

  // The string pszTest is alphanumeric if and only if
  // every character is either a letter or a number.
  // Anything else anywhere else in the string, and too bad.
  while (*pszTest != 0) {
    if (!isdigit(*pszTest)) {
      return FALSE;
    }
    pszTest++;
  }

  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// IsOneOf function

BOOL IsOneOf(char chTest, const char* pszPossibilities, int nPossibilities) {
  // TODO: Add implementation code here
  return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// IsUppercase function

BOOL IsUppercase(const char* pszTest) {
  if (IsNullOrWhiteSpace(pszTest)) {
    return FALSE;
  }

  const int TEST_SIZE = strlen(pszTest) + 1;

  char* pszTrimResult = (char*) malloc(TEST_SIZE * sizeof(char));

  Trim(pszTrimResult, TEST_SIZE, pszTest);

  // The string pszTest is alphanumeric if and only if
  // every character is either a letter or a number.
  // Anything else anywhere else in the string, and too bad.
  for (int i = 0; i < TEST_SIZE; i++) {
    if (pszTrimResult[i] == '\0') {
      continue;
    }
    if (!isupper(pszTrimResult[i])) {
      free(pszTrimResult);
      pszTrimResult = NULL;

      return FALSE;
    }
  }

  free(pszTrimResult);
  pszTrimResult = NULL;

  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// JoinStrings function

void JoinStrings(char* ppszSourceStringArray[],
    int nSourceStringArrayLength, char** ppszOutput,
    int *pnOutputLength) {
  if (ppszSourceStringArray == NULL) {
    return;
  }

  if (nSourceStringArrayLength <= 0) {
    return;
  }

  if (ppszOutput == NULL) {
    return;
  }

  if (pnOutputLength == NULL) {
    return;
  }

  int nTotalBytes = 0;
  for (int i = 0; i < nSourceStringArrayLength; i++) {
    const int CURRENT_ENTRY_SIZE
    = strlen(ppszSourceStringArray[i]) + 1;
    nTotalBytes += CURRENT_ENTRY_SIZE;
    *ppszOutput = (char*) realloc(*ppszOutput,
        (nTotalBytes) * sizeof(char));
    if (i == 0) {
      memset(*ppszOutput, 0, nTotalBytes);
    }
    strcat(*ppszOutput, ppszSourceStringArray[i]);
  }
  const int FINISHED_STRING_SIZE = strlen(*ppszOutput) + 1;
  *ppszOutput = (char*) realloc(*ppszOutput,
      (FINISHED_STRING_SIZE) * sizeof(char));
  (*ppszOutput)[FINISHED_STRING_SIZE - 1] = '\0';
  *pnOutputLength = FINISHED_STRING_SIZE;
}

///////////////////////////////////////////////////////////////////////////////
// MinimumOf function

int MinimumOf(int a, int b) {
  if (a <= b)
    return a;

  return b;
}

///////////////////////////////////////////////////////////////////////////////
// PrependTo function

void PrependTo(char** ppszDest, const char* pszPrefix, const char* pszSrc) {
  // Double-check that we have valid pointers and non-blank
  // prefix and source strings in the input
  if (ppszDest == NULL) {
    return;
  }

  // The prefix is required and must not be blank
  if (pszPrefix == NULL || pszPrefix[0] == '\0') {
    return;
  }

  // The source string is required and must not be blank
  if (pszSrc == NULL || pszSrc[0] == '\0') {
    return;
  }

  // Figure out the total length of the two strings
  // (prefix and source) and add one more for the
  // null-terminator
  const int TOTAL_SIZE = strlen(pszPrefix) + strlen(pszSrc) + 1;

  // Allocate a block of memory that is TOTAL_SIZE characters
  // in length to prepare for gluing the prefix and source
  // strings together
  char* pszResult = (char*) malloc(TOTAL_SIZE * sizeof(char));
  if (pszResult == NULL) {
    return;			// Failed to allocate memory
  }

  // Copy the prefix into the buffer pointed to by pszResult
  // and then glue the source on the end of that.
  strcpy(pszResult, pszPrefix);
  strcat(pszResult, pszSrc);

  // Place the address of the first character of the result
  // into the location pointed to by ppszDest
  *ppszDest = pszResult;
}

///////////////////////////////////////////////////////////////////////////////
// Split function

void Split(char* pszStringToSplit, const char* pszDelimiters,
    char*** pppszStrings, int* pnResultCount) {
  if (IsNullOrWhiteSpace(pszStringToSplit)) {
    return;
  }

  if (pppszStrings == NULL) {
    return;
  }

  if (pszDelimiters == NULL || pszDelimiters[0] == '\0') {
    return;
  }

  if (pnResultCount == NULL) {
    return;
  }

  /* This function tokenizes the string pszStringToSplit on the
   character(s) in pszDelimiters using strtok() over and over
   again, copying each result into a dynamically-allocated
   array of strings, and then returns the address of the array
   of strings.  Each member of the array of strings is itself
   dynamically-allocated.  Each time strtok() returns a non-NULL
   result, the string returned is copied using strcpy() into the
   current dynamically-allocated string, the address of which
   is then stored in the dynamically-allocated array of strings.
   Then, the array of strings is grown by one, and the tokenization
   is attempted again. */

  *pnResultCount = 0; /* initialize result count to zero */
  *pppszStrings = NULL; /* initialize result array to NULL */

  /* Attempt to extract the first token from the string
   ( a token is defined as the "stuff between the delimiters" ) */
  char *pszCurrentResult = strtok(pszStringToSplit, pszDelimiters);
  if (pszCurrentResult == NULL) {
    return; /* nothing came of splitting the string */
  }

  /* Measure the amount of storage that needs to be allocated
   for the first element of the output.  Be sure to include
   space for the null terminator. Base the amount of storage
   needed off of the length of the token string just extracted.*/
  const int FIRST_RESULT_SIZE = strlen(pszCurrentResult) + 1;

  /* Allocate storage for the first token's text to be copied
   into and initialize it to all zeroes (a best practice).*/
  char* pszFirstElement = (char*) malloc(
      FIRST_RESULT_SIZE * sizeof(char));
  memset(pszFirstElement, 0,
      FIRST_RESULT_SIZE * sizeof(char));

  /* Copy the text pointed to by pszCurrent Result into the
   storage referenced by pszFirstElement. */
  strcpy(pszFirstElement, pszCurrentResult);

  /* Allocate storage for an array of char* pointers that is
   currently needing to be just one element in size (since
   as of right now, we just have one thing to put into it) */
  char** ppszResultArray = (char**) malloc(1 * sizeof(char*));
  memset(ppszResultArray, 0, 1 * sizeof(char*));

  /* Initialize the first element of the array of strings
   to contain the address of the first token */
  ppszResultArray[0] = pszFirstElement;

  int nCurArrayElement = 1; /* new current array element */
  int nReturnedElementCount = 1; /* how many tokens thus far */

  while (pszCurrentResult != NULL) {
    /* Try again to tokenize the string.  If we get NULL
     as the answer, then there's no more tokens (parts of
     the input string that are between the delimiters are known
     as tokens) to process, so we stop */
    pszCurrentResult = strtok(NULL, pszDelimiters);
    if (pszCurrentResult == NULL) {
      break; /* done splitting */
    }

    /* Using the length of the current result string (plus one,
     for the null terminator) as the size, allocate a fresh
     block of storage to hold the text referenced by
     pszCurrentResult. */
    int CURRENT_RESULT_SIZE = strlen(pszCurrentResult) + 1;
    char* pszNextResult = (char*) malloc(
        CURRENT_RESULT_SIZE * sizeof(char));
    memset(pszNextResult, 0,
        CURRENT_RESULT_SIZE * sizeof(char));

    /* Copy the text in pszCurrentResult into the newly-allocated
     storage at the address specified by pszNextResult */
    strcpy(pszNextResult, pszCurrentResult);

    /* grow the array of strings by one and then initialize
     the element on the end with the address of
     pszNextElement. */
    ppszResultArray = (char**) realloc(ppszResultArray,
        (nReturnedElementCount + 1) * sizeof(char*));
    ppszResultArray[nCurArrayElement] = pszNextResult;

    /* Increment our counters, and then wash, rinse, repeat. */
    nCurArrayElement += 1;
    nReturnedElementCount += 1;
  }

  /* Save the count of returned elements in the
   variable at the address given by pnResultCount */
  *pnResultCount = nReturnedElementCount;

  /* Tell the caller where in the machine's memory
   the first element of our array of token strings
   is stored. */
  *pppszStrings = ppszResultArray;

  /* Done */
}

///////////////////////////////////////////////////////////////////////////////
// StartsWith function

BOOL StartsWith(const char *str, const char *startsWith) {
  size_t prefixLength = strlen(startsWith);
  size_t stringLength = strlen(str);
  return
  stringLength < prefixLength ?
                                FALSE :
                                strncmp(startsWith, str, prefixLength) == 0;
}

///////////////////////////////////////////////////////////////////////////////
// StringReplace function

void StringReplace(const char* pszSrc,
    const char* pszFindWhat, const char* pszReplaceWith,
    char** ppszResult) {
  if (pszSrc == NULL || pszSrc[0] == '\0') {  // do not use IsNullOrWhiteSpace
    return; // Required parameter
  }

  if (pszFindWhat == NULL || pszFindWhat[0] == '\0') {
    return; // Required parameter
  }

  if (ppszResult == NULL) {
    return; // Required parameter
  }

  int nSrcLen = strlen(pszSrc);
  int nFindWhatLen = strlen(pszFindWhat);
  int nReplaceWithLen = strlen(pszReplaceWith);
  const int DELTA = nReplaceWithLen - nFindWhatLen;

  /* In order to determine the proper size for the result
   buffer, count the occurrences of findWhat in pszSrc.
   But, there is no need to bother if the DELTA is zero,
   since this means the src and dest strings will be of
   identical length. In this case, set nOccurrences to a
   default value of zero. */
  int nOccurrences = DELTA == 0
      ? 0
        :
        GetSubstringOccurrenceCount(pszSrc, pszFindWhat);
  if (nOccurrences < 0) {
    return; // unknown error
  }

  /* Determine the appropriate size for the block of memory
   * in which to store the result. */
  const int BUFSIZE = DELTA == 0
      ? nSrcLen
        :
        nSrcLen + nOccurrences * DELTA;

  *ppszResult = (char*) malloc((BUFSIZE + 1) * sizeof(char));
  memset(*ppszResult, 0, BUFSIZE + 1);

  int i = 0;
  while (*pszSrc) {
    // compare the substring with the result
    if (strstr(pszSrc, pszFindWhat) == pszSrc) {
      strcpy(&((*ppszResult)[i]), pszReplaceWith);
      i += nReplaceWithLen;
      pszSrc += nFindWhatLen;
      //*ppszResult += nReplaceWithLen;
    } else {
      (*ppszResult)[i] = *pszSrc;
      pszSrc++;
      i++;
    }
  }

  (*ppszResult)[i] = '\0';
}

///////////////////////////////////////////////////////////////////////////////
// Trim function

// Stores the trimmed input string into the given output buffer,
// which must be large enough to store the result.  If it is too small,
// the output is truncated.  Shout out to Stack Overflow
void Trim(char *out, size_t len, const char *str) {
  if (len == 0)
    return;

  memset(out, 0, len);

  const char *end;

  int leading_space_trimmed = 0;

  end = str + strlen(str) - 1;
  if (!isspace((unsigned char )*str)) {
    *out = *str;
    out++;
  }

  while (*str++ != 0 && str <= end && strlen(out) <= len) {
    if (isspace((unsigned char )*end)) {
      end--;
    }

    if (!leading_space_trimmed && isspace((unsigned char )*str)) {
      continue;
    }

    if (str == end && isspace((unsigned char )*str)
        && isspace((unsigned char )*end)) {
      break;
    }

    leading_space_trimmed = 1;
    *out = *str;
    out++;
  }
}
