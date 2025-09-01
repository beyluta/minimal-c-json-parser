#ifndef JSON_H
#define JSON_H

#include <limits.h>
#include <stddef.h>

constexpr unsigned short JSONBUFFSIZE = USHRT_MAX;
typedef enum : short { MEMORY_FAILURE = -1, FUNC_SUCCESS = 0 } StatusJSON;
typedef enum : char {
  UNDEFINED = -1,
  NUMBER = 0,
  OBJECT = 1,
  STRING = 2,
  ARRAY = 3,
  BOOLEAN = 4,
  JNULL = 5
} TypeJSON;
typedef enum : unsigned char {
  SQUARE_OPEN = '[',
  SQUARE_CLOSE = ']',
  CURLY_OPEN = '{',
  CURLY_CLOSE = '}',
  DOUBLE_QUOTES = '\"',
  COLON = ':',
  COMMA = ',',
  BACKSLASH = '\\',
  SPACE = ' ',
} TokenJSON;

typedef struct {
  size_t length;
  char str[JSONBUFFSIZE];
} StringJSON;

/**
 * @brief Converts a json string to a standard c-string
 * @param src string in json format
 * @param dest destination array of chars to store the result
 * @returns the status of the operation
 */
StatusJSON JSONToStr(StringJSON src, char *dest);

/**
 * @brief Converts a c-string to a JSON String
 * @param src string in json format
 * @param dest destination array of chars to store the result
 * @returns the status of the operation
 */
StatusJSON StrToJSON(const char *src, StringJSON *dest);

/**
 * @brief Gets a property from a JSON object by the field name
 * @param src JSON object containing the key-value we want to get
 * @param dest Destination JSON string with the result of the operation
 * @param target Name of the field we want to get the value of
 * @returns The status of the operation
 */
StatusJSON GetProperty(StringJSON src, StringJSON *dest, const char *target);

/**
 * @brief Gets a description of the error message thrown
 * @param status Status of the message to get the description from
 * @param dest String to save the message to
 */
void GetStatusErrorMessage(StatusJSON status, char *dest);

#endif
