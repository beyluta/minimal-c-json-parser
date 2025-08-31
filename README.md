# Minimal JSON Parser in C (C23)

This project provides a **lightweight JSON parser written in C23**, designed for small-scale projects where full-fledged JSON libraries are too heavy or unnecessary.

It supports extracting values from JSON strings, handling:

- **Strings**
- **Numbers**
- **Arrays**
- **Objects** (including nested properties)

Not yet supported but planned for the future:

- **NULL**
- **Booleans**

The parser works with a custom `StringJSON` structure and avoids dynamic memory allocation by using fixed-size buffers.

## Features

- Convert between C strings and `StringJSON` structures
- Retrieve properties by name (`GetProperty`)
- Parse nested objects and arrays
- Support for both primitive and structured JSON types
- Simple error handling via `StatusJSON`

## Usage

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

  StringJSON json;
  StrToJSON(jsonFileContent, &json);

  GetProperty(json, &json, "progName");

  char progName[512];
  JSONToStr(json, progName);
  printf("%s\n", progName); // Expected result: "library"
```

## Limitations

- Very small buffer size of `USHRT_MAX` supported. Bigger JSON files will fail to parse.
- Does not validate JSON data. Make sure yours is compliant.
- No support for `boolean` values (`true` or `false` will fail).
- Numbers are parsed as raw strings (do the convertion yourself with `strtol` or `atoi`).
- Not a fully compliant JSON parser. Designed for lightweight extraction only.
