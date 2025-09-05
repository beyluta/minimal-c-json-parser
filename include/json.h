#ifndef JSON_H
#define JSON_H

#include <limits.h>
#include <stddef.h>

#define GETPROP2(a, b) GetJsonProperty2(a, b)
#define GETPROP3(a, b, c) GetJsonProperty3(a, b, c)
#define EXPAND(a) a
#define GET_PROP_MACRO(_1, _2, _3, name, ...) name
#define GetProperty(...)                                                       \
  EXPAND(GET_PROP_MACRO(__VA_ARGS__, GETPROP3, GETPROP2)(__VA_ARGS__))

constexpr unsigned short JSONBUFFSIZE = USHRT_MAX;
typedef enum : short {
  MEMORY_FAILURE = -1,
  FUNC_SUCCESS = 0,
  UNSUPPORTED_OPERATION = 1
} status_json_t;
typedef enum : char {
  UNDEFINED = -1,
  NUMBER = 0,
  OBJECT = 1,
  STRING = 2,
  ARRAY = 3,
  BOOLEAN = 4,
  JNULL = 5
} type_json_t;
typedef enum : char {
  JSON_DOUBLE,
  JSON_INT,
  JSON_LONG,
  JSON_CHAR_ARR,
  JSON_DOUBLE_ARR,
  JSON_LONG_ARR,
  JSON_INT_ARR,
  JSON_BOOLEAN
} native_json_type_t;
typedef enum : unsigned char {
  SQUARE_OPEN = '[',
  SQUARE_CLOSE = ']',
  CURLY_OPEN = '{',
  CURLY_CLOSE = '}',
  DOUBLE_QUOTES = '\"',
  COLON = ':',
  COMMA = ',',
  PERIOD = '.',
  BACKSLASH = '\\',
  SPACE = ' ',
} token_json_t;

typedef struct {
  size_t length;
  char str[JSONBUFFSIZE];
  type_json_t type;
} string_json_t;

typedef union {
  double d[JSONBUFFSIZE];
  long l[JSONBUFFSIZE];
  int i[JSONBUFFSIZE];
} array_data_t;

typedef struct {
  array_data_t data;
  int length;
} array_json_t;

/**
 * @brief Converts a json string to a standard c-string
 * @param src string in json format
 * @param dest destination array of chars to store the result
 * @returns the status of the operation
 */
status_json_t ConvertJsonToString(string_json_t src, char *dest);

/**
 * @brief Converts a c-string to a JSON String
 * @param src string in json format
 * @param dest destination array of chars to store the result
 * @returns the status of the operation
 */
status_json_t ConvertStringToJson(const char *src, string_json_t *dest);

/**
 * @brief Gets a property from a JSON object by the field name
 * @param src JSON object containing the key-value we want to get
 * @param dest Destination JSON string with the result of the operation
 * @param target Name of the field we want to get the value of
 * @returns The status of the operation
 */
status_json_t GetJsonProperty3(string_json_t src, string_json_t *dest,
                               const char *target);

/**
 * @brief Gets a property from a JSON object by the field name
 * @param srcDest JSON object containing the key-value we want to get; The
 * result of the query will be saved inside the same object
 * @param target Name of the field we want to get the value of
 * @returns The status of the operation
 */
status_json_t GetJsonProperty2(string_json_t *srcDest, const char *target);

/**
 * @brief Gets a description of the error message thrown
 * @param status Status of the message to get the description from
 * @param dest String to save the message to
 */
void GetStatusErrorMessage(status_json_t status, char *dest);

/**
 * @brief Converts a JSON Struct to a primitive value passed by pointer
 * @param json JSON object containing the value to be converted
 * @param type Type to be converted
 * @param dest Destination void pointer to save the result to
 */
status_json_t ConvertJsonToStandardType(string_json_t json,
                                        native_json_type_t type, void *dest);

#endif
