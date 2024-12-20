#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <string>
#include <unordered_map>
#include <algorithm>
#include "proj2.h"
#include "y.tab.h"

extern "C" int yyleng;

#define STRTBL_LEN 3001 /* String table length */
#define STR_SPRTR 0     /* String seperator in string table */

static std::unordered_map<std::string, int> hashTable; /* Hash table mapping strings to indices in the string table */
static char stringTable[STRTBL_LEN] = {STR_SPRTR};
static int stringTableEndIndex = 0; /* End of the string table */

/* TODO: Add additional variable definitions as needed. */

extern "C" void printStringTable()
{
  int i;
  for (i = 0; i < stringTableEndIndex; i++)
  {
    if (stringTable[i] == STR_SPRTR)
      printf(" ");
    else
      printf("%c", stringTable[i]);
  }
  printf("\n");
}

/** If text is found in string table, assign the index of the string to the lexeme (yylval).
 *
 * If text is not found in string table, add text to end of string table.
 * Also, add the STR_SPRTR character to separate the string from the next string.
 * Lastly, assign the index where text was added to the lexeme (yylval).
 *
 * @param text - String to add to string table.
 * @param tokenType - The type of token (either SCONSTnum or IDnum).
 */

extern "C" void addStringTable(char *text, int tokenType)
{
  std::string str(text);

  // Check if the string is already in the hash table
  auto it = hashTable.find(str);
  if (it != hashTable.end())
  {
    yylval.intg = it->second;
    return;
  }

  // String not found, need to add it to the string table
  int newIndex = stringTableEndIndex;

  // Check if there's enough space in the string table
  if (newIndex + strlen(text) + 1 >= STRTBL_LEN)
  {
    fprintf(stderr, "Error: String table overflow\n");
    exit(1);
  }

  // Copy the string to the string table
  char *dest = stringTable + newIndex;
  char *src = text;

  while (*src)
  {
    if (tokenType == SCONSTnum && *src == '\\')
    {
      src++;
      switch (*src)
      {
      case 'n':
        *dest++ = '\n';
        break;
      case 't':
        *dest++ = '\t';
        break;
      case '\\':
        *dest++ = '\\';
        break;
      case '\'':
        *dest++ = '\'';
        break;
      default:
        *dest++ = *src;
      }
    }
    else
    {
      *dest++ = *src;
    }
    src++;
  }

  stringTableEndIndex = dest - stringTable;

  // Add the string separator
  stringTable[stringTableEndIndex++] = STR_SPRTR;

  // Add the new string to the hash table
  hashTable[str] = newIndex;

  // Assign the index to yylval
  yylval.intg = newIndex;
}

/** Returns a pointer to the string that is at the given index of the string table.
 *
 * If index is out of bounds of the string table, returns null.
 *
 * @param index - Index of string in the string table.
 *
 * @return Pointer to the string in the index.
 */

const char *getString(int index)
{
  // Check if index is out of bounds
  if (index < 0 || index >= stringTableEndIndex)
  {
    return NULL;
  }
  // Return pointer to the string at the given index
  return &stringTable[index];
}

/** Returns the index in the string table where the given text is.
 *
 * If the text is not found in the string table, returns -1.
 *
 * @param text - The string to search for in the string table.
 *
 * @return Index into the string table where the string was found.  -1 if not found.
 */

int getIndex(const char *text)
{
  if (!text)
  {
    return -1;
  }

  // Use the existing hashTable to find the string
  // auto it = hashTable.find(text);

  std::unordered_map<std::string, int>::iterator it = hashTable.find(text);

  // If found, return the index
  if (it != hashTable.end())
  {
    return it->second;
  }

  // String not found
  return -1;
}