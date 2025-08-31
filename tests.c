#include <assert.h>
#include <json.h>
#include <stdio.h>
#include <string.h>

constexpr char CHECKMARK[] = "\xE2\x9C\x93\n";
constexpr unsigned char COUNT_CASES = 5;
static unsigned char tests = 0;

void tryAssert(char *case1, char *case2, const char *message) {
  assert(strcmp(case1, case2) == 0);
  char completeMessage[BUFSIZ];
  snprintf(completeMessage, BUFSIZ, "%s case passed %d/%d %s", message, ++tests,
           COUNT_CASES, CHECKMARK);
  printf("%s", completeMessage);
}

int main() {
  char cJsonStr[] = "{ \"progName\": \"library\", \"version\": 1.0, \"tags\": "
                    "[\"C\", \"C++\"], \"metadata\": { \"origin\": "
                    "\"unknown\", \"device\": { \"pc\": \"Desktop\" } } }";

  StringJSON jsonStr;
  StatusJSON status;
  if ((status = StrToJSON(cJsonStr, &jsonStr) != FUNC_SUCCESS)) {
    return status;
  }

  // Testing for Strings
  StringJSON result;
  if ((status = GetProperty(jsonStr, &result, "progName") != FUNC_SUCCESS)) {
    return status;
  }

  char cResult[512];
  if ((status = JSONToStr(result, cResult) != FUNC_SUCCESS)) {
    return status;
  }

  tryAssert(cResult, "library", "String");

  // Testing for Numbers
  if ((status = GetProperty(jsonStr, &result, "version") != FUNC_SUCCESS)) {
    return status;
  }

  if ((status = JSONToStr(result, cResult) != FUNC_SUCCESS)) {
    return status;
  }

  tryAssert(cResult, "1.0", "Number");

  // Testing for Arrays
  if ((status = GetProperty(jsonStr, &result, "tags") != FUNC_SUCCESS)) {
    return status;
  }

  if ((status = JSONToStr(result, cResult) != FUNC_SUCCESS)) {
    return status;
  }

  tryAssert(cResult, "[\"C\", \"C++\"]", "Array");

  // Testing for an Object
  if ((status = GetProperty(jsonStr, &result, "metadata") != FUNC_SUCCESS)) {
    return status;
  }

  if ((status = JSONToStr(result, cResult) != FUNC_SUCCESS)) {
    return status;
  }

  tryAssert(cResult,
            "{ \"origin\": \"unknown\", \"device\": { \"pc\": \"Desktop\" } }",
            "Object");

  // Testing the access of nested objects
  if ((status = GetProperty(result, &result, "device") != FUNC_SUCCESS)) {
    return status;
  }

  if ((status = JSONToStr(result, cResult) != FUNC_SUCCESS)) {
    return status;
  }

  tryAssert(cResult, "{ \"pc\": \"Desktop\" }", "Object nested");

  return 0;
}
