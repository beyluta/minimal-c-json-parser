# Minimal JSON Parser in C (C23)

This project provides a **lightweight JSON parser written in C23**, designed for small-scale projects where full-fledged JSON libraries are too heavy or unnecessary.

It supports extracting values from JSON strings, handling:

- **Strings**
- **Numbers**
- **Booleans**
- **Arrays**
- **Objects**
- **NULL**

The parser works with a custom `StringJSON` structure and avoids dynamic memory allocation by using fixed-size buffers.

## Features

- Convert between C strings and `StringJSON` structures
- Retrieve properties by name (`GetProperty`)
- Parse nested objects and arrays
- Support for both primitive and structured JSON types
- Simple error handling via `StatusJSON`

## Examples

These are a few examples of how to extract JSON data and convert them to native C types.

### Extracting

Given the following JSON data:

```json
{
  "progName": "library",
  "version": 1.0,
  "tags": ["C", "C++"],
  "metadata": { "origin": "unknown", "device": { "pc": "Desktop" } }
}
```

We want to extract the `progName` field into a readable C-Style-String (aka char array):

```c
  char *jsonFileContent = ReadFile(...); // Load the json file here as a string

  string_json_t json;
  ConvertStringToJson(jsonFileContent, &json);

  GetProperty(&json, "progName");
  // Or GetProperty(src, dest, "progName")

  char progName[512];
  ConvertJsonToString(json, progName);
  printf("%s\n", progName); // Expected result: "library"
```

### Converting

Currently the following types can be converted to native C types from a `StringJSON` struct:

| Type       | Identifier      |
| ---------- | --------------- |
| int        | JSON_INT        |
| double     | JSON_DOUBLE     |
| long       | JSON_LONG       |
| boolean    | JSON_BOOLEAN    |
| int [ ]    | JSON_INT_ARR    |
| double [ ] | JSON_DOUBLE_ARR |
| long [ ]   | JSON_LONG_ARR   |
| char [ ]   | JSON_CHAR_ARR   |

To convert them you must use the function `JSONToPrimitive(StringJSON, PrimitiveJSON, void*)` as such:

```c
string_json_t json; // = 99.95

double num; // double, int, long, or char[] supported
ConvertJsonToStandardType(json, JSON_DOUBLE, &num);
// Or                 JSON_INT
// Or                 JSON_LONG
// Or                 JSON_BOOLEAN
// Or                 JSON_CHAR_ARR

printf("Result is %f\n", num);
```

And for various types of number arrays:

```c
json_string_t json; // = [ 10.5, 20, 30.0 ]

array_json_t list;
ConvertJsonToStandardType(json, JSON_DOUBLE_ARR, &list);

for (int i = 0; i < list.length; i++) {
  printf("Result is: %f\n", list.data.d[i]);
  // Or                              .i for int
  // Or                              .l for long
}
```

## Error-handling

Every function returns a `status_json_t` type. This contains an error code with the status of the function.

```c
string_json_t json; // { "username": "myUser", password: "hash_0495399..." }
ConvertStringToJson(jsonFileContent, &json);

status_json_t errorCode = GetProperty(&json, "createdOn");
if (errorCode == UNDEFINED_KEY) {
  // Key 'createdOn' does not exist in the JSON file
  exit(1);
}
```

Not all errors are failures, here is a table of codes and their meaning:

| Error                 | Code | Purpose                 |
| --------------------- | ---- | ----------------------- |
| MEMORY_FAILURE        | -1   | Internal library error  |
| FUNC_SUCCESS          | 0    | Success message         |
| UNSUPPORTED_OPERATION | 1    | User-error              |
| UNDEFINED_KEY         | 2    | JSON Key does not exist |

## Limitations

- Very small buffer size of `USHRT_MAX` supported. Bigger JSON files will fail to parse.
- Does not validate JSON data. Make sure yours is compliant.
- Not a fully compliant JSON parser. Designed for lightweight extraction only.
