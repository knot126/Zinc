#pragma once

#include "common.h"

typedef const char *ZNMapKey;
typedef void *ZNMapValue;

typedef struct {
	ZNMapKey *keys;
	ZNMapValue *values;
	size_t count;
	size_t prealloc;
} ZNMap;

ZNResult ZNMapInit(ZNMap *this);
void ZNMapRelease(ZNMap *this);

ZNResult ZNMapAppend(ZNMap *this, ZNMapKey key, ZNMapValue value);
ZNResult ZNMapSet(ZNMap *this, ZNMapKey key, ZNMapValue value);
ZNMapValue ZNMapGet(ZNMap *this, ZNMapKey key);
bool ZNMapHas(ZNMap *this, ZNMapKey key);
size_t ZNMapCount(ZNMap *this);

ZNMap *ZNMapFreeValues(ZNMap *this);
