#include <assert.h>
#include <json.h>
#include <stdio.h>
#include <string.h>

constexpr char CHECKMARK[] = "\xE2\x9C\x93\n";
constexpr unsigned char COUNT_CASES = 11;
static unsigned char tests = 0;

static void tryAssert(char *case1, char *case2, const char *message) {
  assert(strcmp(case1, case2) == 0);
  char completeMessage[BUFSIZ];
  snprintf(completeMessage, BUFSIZ, "%s case passed %d/%d %s", message, ++tests,
           COUNT_CASES, CHECKMARK);
  printf("%s", completeMessage);
}

static StatusJSON Test_String(StringJSON json) {
  StringJSON result;
  StatusJSON status;
  if ((status = GetProperty(json, &result, "progName") != FUNC_SUCCESS)) {
    return status;
  }

  char cResult[512];
  if ((status = JSONToStr(result, cResult) != FUNC_SUCCESS)) {
    return status;
  }

  tryAssert(cResult, "library", "String");

  return FUNC_SUCCESS;
}

static StatusJSON Test_Empty_String(StringJSON json) {
  StringJSON result;
  StatusJSON status;
  if ((status = GetProperty(json, &result, "description") != FUNC_SUCCESS)) {
    return status;
  }

  char cResult[512];
  if ((status = JSONToStr(result, cResult) != FUNC_SUCCESS)) {
    printf("Issue occurred!\n");
    return status;
  }

  tryAssert(cResult, "", "Empty String");

  return FUNC_SUCCESS;
}

static StatusJSON Test_Primitive_Empty_String(StringJSON json) {
  StringJSON result;
  StatusJSON status;
  if ((status = GetProperty(json, &result, "description") != FUNC_SUCCESS)) {
    return status;
  }

  char array[512];
  if ((status =
           JSONToPrimitive(result, JSON_CHAR_ARR, &array) != FUNC_SUCCESS)) {
    return status;
  }

  tryAssert(array, "", "Primitive Empty String");

  return FUNC_SUCCESS;
}

static StatusJSON Test_Boolean(StringJSON json) {
  StringJSON result;
  StatusJSON status;
  if ((status = GetProperty(json, &result, "isCompliant") != FUNC_SUCCESS)) {
    return status;
  }

  char cResult[512];
  if ((status = JSONToStr(result, cResult) != FUNC_SUCCESS)) {
    return status;
  }

  tryAssert(cResult, "false", "Boolean");

  return FUNC_SUCCESS;
}

static StatusJSON Test_Null(StringJSON json) {
  StringJSON result;
  StatusJSON status;
  if ((status = GetProperty(json, &result, "lastUpdated") != FUNC_SUCCESS)) {
    return status;
  }

  char cResult[512];
  if ((status = JSONToStr(result, cResult) != FUNC_SUCCESS)) {
    return status;
  }

  tryAssert(cResult, "null", "Null");
  return FUNC_SUCCESS;
}

static StatusJSON Test_Number(StringJSON json) {
  StringJSON result;
  StatusJSON status;
  if ((status = GetProperty(json, &result, "version") != FUNC_SUCCESS)) {
    return status;
  }

  char cResult[512];
  if ((status = JSONToStr(result, cResult) != FUNC_SUCCESS)) {
    return status;
  }

  tryAssert(cResult, "1.0", "Number");
  return FUNC_SUCCESS;
}

static StatusJSON Test_Array(StringJSON json) {
  StringJSON result;
  StatusJSON status;
  if ((status = GetProperty(json, &result, "tags") != FUNC_SUCCESS)) {
    return status;
  }

  char cResult[512];
  if ((status = JSONToStr(result, cResult) != FUNC_SUCCESS)) {
    return status;
  }

  tryAssert(cResult, "[\"C\", \"C++\"]", "Array");

  return FUNC_SUCCESS;
}

static StatusJSON Test_Empty_Array(StringJSON json) {
  StringJSON result;
  StatusJSON status;
  if ((status = GetProperty(json, &result, "devs") != FUNC_SUCCESS)) {
    return status;
  }

  char cResult[512];
  if ((status = JSONToStr(result, cResult) != FUNC_SUCCESS)) {
    return status;
  }

  // printf("Result is: %s\n", cResult);

  tryAssert(cResult, "[]", "Empty Array");

  return FUNC_SUCCESS;
}

static StatusJSON Test_Object(StringJSON json) {
  StringJSON result;
  StatusJSON status;
  if ((status = GetProperty(json, &result, "metadata") != FUNC_SUCCESS)) {
    return status;
  }

  char cResult[512];
  if ((status = JSONToStr(result, cResult) != FUNC_SUCCESS)) {
    return status;
  }

  tryAssert(cResult,
            "{ \"origin\": \"unknown\", \"device\": { \"pc\": \"Desktop\" } }",
            "Object");

  return FUNC_SUCCESS;
}

static StatusJSON Test_Empty_Object(StringJSON json) {
  StringJSON result;
  StatusJSON status;
  if ((status = GetProperty(json, &result, "other") != FUNC_SUCCESS)) {
    return status;
  }

  char cResult[512];
  if ((status = JSONToStr(result, cResult) != FUNC_SUCCESS)) {
    return status;
  }

  tryAssert(cResult, "{}", "Empty Object");

  return FUNC_SUCCESS;
}

static StatusJSON Test_Nested_Object(StringJSON json) {
  StringJSON result;
  StatusJSON status;
  if ((status = GetProperty(json, &result, "device") != FUNC_SUCCESS)) {
    return status;
  }

  char cResult[512];
  if ((status = JSONToStr(result, cResult) != FUNC_SUCCESS)) {
    return status;
  }

  tryAssert(cResult, "{ \"pc\": \"Desktop\" }", "Object nested");

  return FUNC_SUCCESS;
}

int main() {
  char cJsonStr[] = "{ \"progName\": \"library\", \"description\": \"\","
                    "\"version\": 1.0, \"tags\": "
                    "[\"C\", \"C++\"], \"metadata\": { \"origin\": "
                    "\"unknown\", \"device\": { \"pc\": \"Desktop\" } }, "
                    "\"isCompliant\": false, \"lastUpdated\": null, \"devs\": "
                    "[], \"other\": {} }";

  StringJSON jsonStr;
  StatusJSON status;
  if ((status = StrToJSON(cJsonStr, &jsonStr) != FUNC_SUCCESS)) {
    return status;
  }

  Test_String(jsonStr);
  Test_Empty_String(jsonStr);
  Test_Primitive_Empty_String(jsonStr);
  Test_Boolean(jsonStr);
  Test_Null(jsonStr);
  Test_Number(jsonStr);
  Test_Array(jsonStr);
  Test_Empty_Array(jsonStr);
  Test_Object(jsonStr);
  Test_Empty_Object(jsonStr);
  Test_Nested_Object(jsonStr);

  return 0;
}
