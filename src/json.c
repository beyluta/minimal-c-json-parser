#include "json.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

static StatusJSON TrimEnds(StringJSON *src) {
  if (src->length <= 2)
    return MEMORY_FAILURE;
  memcpy(src->str, &src->str[1], src->length - 2);
  src->length = src->length - 2;
  return FUNC_SUCCESS;
}

static StatusJSON GetWordBetweenIndexes(const StringJSON src,
                                        const size_t start, const size_t end,
                                        StringJSON *dest, bool trimEnds) {
  const ssize_t length = end - start + 1;
  if (length < 1)
    return MEMORY_FAILURE;

  memcpy(dest->str, &src.str[start], length);
  dest->length = length;

  StatusJSON status;
  if (trimEnds && (status = TrimEnds(dest) != FUNC_SUCCESS)) {
    return status;
  }

  return FUNC_SUCCESS;
}

StatusJSON JSONToStr(StringJSON src, char *dest) {
  if (src.length >= JSONBUFFSIZE)
    return MEMORY_FAILURE;

  memcpy(dest, src.str, src.length);
  dest[src.length] = '\0';
  return FUNC_SUCCESS;
}

StatusJSON StrToJSON(const char *src, StringJSON *dest) {
  const size_t size = strlen(src);
  if (size >= JSONBUFFSIZE)
    return MEMORY_FAILURE;
  for (size_t i = 0; i < size; i++) {
    dest->str[i] = src[i];
  }
  dest->length = size;
  return FUNC_SUCCESS;
}

static TypeJSON GetJSONType(const char c) {
  switch ((int)c) {
  default:
    return NUMBER;
  case DOUBLE_QUOTES:
    return STRING;
  case SQUARE_OPEN:
    return ARRAY;
  case CURLY_OPEN:
    return OBJECT;
  case 'n':
    return JNULL;
  case 'f':
  case 't':
    return BOOLEAN;
  }
}

static bool TypeRequiresDelimiter(TypeJSON type) {
  switch (type) {
  case NUMBER:
  case BOOLEAN:
  case JNULL:
    return false;
  default:
    return true;
  }
}

static StatusJSON GetValue(StringJSON src, const ssize_t iStartAt,
                           StringJSON *dest) {
  if (iStartAt < 0 || src.length == 0)
    return MEMORY_FAILURE;

  ssize_t iStartWord = -1, iEndWord = -1;
  size_t fieldNestingLevel = 0;
  bool isCurrentWordValue = false;
  bool isCurrentIndexInsideDoubleQuotes = false;
  TypeJSON type = UNDEFINED;
  for (size_t i = iStartAt + 1; i < src.length; i++) {
    // Ignoring spaces unless the type requires delimiters
    if (src.str[i] == SPACE && TypeRequiresDelimiter(type))
      continue;

    // This is needed so that we know that we can start reading the value
    if (src.str[i] == COLON) {
      isCurrentWordValue = true;
      continue;
    }

    // Skip this iteration if the still haven't found a value to read
    if (!isCurrentWordValue) {
      continue;
    }

    // Make sure to assign a type to the current json being read
    if (type == UNDEFINED) {
      type = GetJSONType(src.str[i]);
      iStartWord = i;

      if (type == OBJECT || type == ARRAY) {
        fieldNestingLevel++;
      }

      if (type != NUMBER) {
        continue;
      }
    }

    // Strings are the most basic type to parse because we just need to return
    // the indexes of the start and end of the double quotes
    if (src.str[i] == DOUBLE_QUOTES && src.str[i - 1] != BACKSLASH &&
        type == STRING) {
      iEndWord = i;
      break;
    }

    // Booleans and nulls are basically the same. We just need to read until the
    // end of the stream and return whatever has been read
    if (type == BOOLEAN || type == JNULL) {
      if (src.str[i] == SPACE || src.str[i] == COMMA) {
        iEndWord = i - 1;
        break;
      }

      if (i >= src.length - 1) {
        iEndWord = i;
        break;
      }
    }

    // Numbers are a little trickier, we need to make sure to return the number
    // as soon as there aren't any digits left in the current readable stream.
    // This excludes some annoyances like the period to declare decimal values
    if (type == NUMBER) {
      if (src.str[i] == '.') {
        continue;
      }

      if (!isdigit(src.str[i]) || src.str[i] == SPACE) {
        iEndWord = i - 1;
        break;
      }

      if (i >= src.length - 1) {
        iEndWord = i;
        break;
      }
    }

    // Arrays are sort of basic too though it needs a little more work. We need
    // to make sure to disregard any values inside double quotes since these are
    // just user strings. As well as keep track of the amount of square brackets
    // for nested arrays
    if (type == ARRAY) {
      if (src.str[i] == DOUBLE_QUOTES && i - 1 > 0 &&
          src.str[i - 1] != BACKSLASH) {
        isCurrentIndexInsideDoubleQuotes = !isCurrentIndexInsideDoubleQuotes;
      }

      if (isCurrentIndexInsideDoubleQuotes)
        continue;

      if (src.str[i] == SQUARE_OPEN) {
        fieldNestingLevel++;
      }

      if (src.str[i] == SQUARE_CLOSE) {
        fieldNestingLevel--;
        if (fieldNestingLevel == 0) {
          iEndWord = i;
        }
      }
    }

    // Objects are basically the same as arrays but to make the code more
    // readable I separated them, but they just do the same thing essentially
    if (type == OBJECT) {
      if (src.str[i] == DOUBLE_QUOTES && i - 1 > 0 &&
          src.str[i - 1] != BACKSLASH) {
        isCurrentIndexInsideDoubleQuotes = !isCurrentIndexInsideDoubleQuotes;
      }

      if (isCurrentIndexInsideDoubleQuotes)
        continue;

      if (src.str[i] == CURLY_OPEN)
        fieldNestingLevel++;

      if (src.str[i] == CURLY_CLOSE) {
        fieldNestingLevel--;
        if (fieldNestingLevel == 0) {
          iEndWord = i;
        }
      }
    }
  }

  StatusJSON status;
  if ((status = GetWordBetweenIndexes(src, iStartWord, iEndWord, dest,
                                      type == STRING) != FUNC_SUCCESS)) {
    return status;
  }

  return FUNC_SUCCESS;
}

StatusJSON GetProperty(StringJSON src, StringJSON *dest, const char *target) {
  if (src.length <= 0 || strlen(target) >= JSONBUFFSIZE) {
    return MEMORY_FAILURE;
  }

  StatusJSON status;
  if ((status = TrimEnds(&src) != FUNC_SUCCESS)) {
    return status;
  }

  bool isCurrentWordKey = true;
  ssize_t iStartWord = -1, iEndWord = -1;
  for (size_t i = 0; i < src.length; i++) {
    if (src.str[i] == COLON) {
      isCurrentWordKey = !isCurrentWordKey;
      continue;
    }

    if (src.str[i] == COMMA && !isCurrentWordKey) {
      isCurrentWordKey = true;
      continue;
    }

    if (src.str[i] == DOUBLE_QUOTES && isCurrentWordKey) {
      if (iStartWord < 0) {
        iStartWord = i;
        continue;
      } else if (iEndWord < 0) {
        iEndWord = i;

        StringJSON value;
        StatusJSON status;
        if ((status = GetWordBetweenIndexes(src, iStartWord, iEndWord, &value,
                                            true) != FUNC_SUCCESS)) {
          return MEMORY_FAILURE;
        }

        char cStr[JSONBUFFSIZE];
        if ((status = JSONToStr(value, cStr) != FUNC_SUCCESS)) {
          return MEMORY_FAILURE;
        }

        if (strcmp(cStr, target) == 0) {
          GetValue(src, i, dest);
          break;
        }

        iStartWord = -1;
        iEndWord = -1;
      }
    }
  }
  return FUNC_SUCCESS;
}

void GetStatusErrorMessage(StatusJSON status, char *dest) {
  switch (status) {
  default:
  case FUNC_SUCCESS:
    snprintf(dest, BUFSIZ, "Function exited with success code %d", status);
    break;
  case MEMORY_FAILURE:
    snprintf(dest, BUFSIZ, "Function exited with failure code %d", status);
    break;
  }
}
