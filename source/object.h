#pragma once

#include "common.h"

/**
 * Generic type container
 */
typedef union ZNStorable {
	void *asPointer;
	ZNObject asObject;
	int64_t asInteger64;
	uint64_t asUnsigned64;
	double asFloat64;
	float asFloat32;
	const char *asString;
};

typedef const char *ZNSelector;
typedef ZNObject (*ZNMethod)(ZNObject this, ZNObject in);

#define ZN_STORABLE_NULL (ZNStorable) {.asPointer = NULL}
#define ZN_AS_STORABLE(p) (ZNStorable) {.asPointer = (void *) p}

/**
 * Object
 */
struct ZNObjectData;
typedef struct ZNObjectData *ZNObject;

typedef struct ZNObjectData {
	ZNMap feilds;
	ZNMap methods;
	ZNObject prototype;
} ZNObjectData;

ZNResult ZNSetParamObjectPrototype(ZNObject obj);

ZNObject ZNObjectCreate(ZNObject prototype);
ZNObject ZNObjectRelease(ZNObject this);
