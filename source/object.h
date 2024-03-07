#pragma once

#include "common.h"

/**
 * Generic type container
 */
typedef union ZNStorable {
	void *asPointer;
	ZNObject asObject;
	int64_t asInteger;
	uint64_t asUnsigned;
	double asFloat64;
	float asFloat32;
	const char *asString;
};

typedef const char *ZNSelector;
typedef ZNStorable (*ZNMethod)(ZNObject this, ZNObject in);

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

ZNObject ZNObjectCreate(ZNObject prototype);
ZNObject ZNObjectRelease(ZNObject this);
