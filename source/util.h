#pragma once

#include "common.h"

typedef uint32_t ZNResult;
enum {
	ZN_SUCCESS = 0,
	ZN_FAIL = 1,
};

void *ZNMemory(void *block, size_t size);
void ZNClean(void *block, size_t size);
char *ZNStringDuplicate(const char * const string);
bool ZNStringEqual(const char *str1, const char *str2);
size_t ZNStringCountChar(const char *str, char c);
