#pragma once
#include "common.h"
#include "object.h"

struct ZNObjectData;
typedef struct ZNObjectData *ZNObject;

/**
 * Generic type container
 */
typedef union ZNValueData {
	ZNObject asObject;
	void *asPointer;
	int8_t asInt8;
	uint8_t asUInt8;
	int16_t asInt16;
	uint16_t asUInt16;
	int32_t asInt32;
	uint32_t asUInt32;
	int64_t asInt64;
	uint64_t asUInt64;
	float asFloat32;
	double asFloat64;
	const char *asString;
};

typedef uint32_t ZNValueType;
enum {
	ZN_TYPE_NIL = 0,
	ZN_TYPE_OBJECT = 1,
	ZN_TYPE_POINTER = 2,
	ZN_TYPE_INT8 = 3,
	ZN_TYPE_UINT8 = 4,
	ZN_TYPE_INT16 = 5,
	ZN_TYPE_UINT16 = 6,
	ZN_TYPE_INT32 = 7,
	ZN_TYPE_UINT32 = 8,
	ZN_TYPE_INT64 = 9,
	ZN_TYPE_UINT64 = 10,
	ZN_TYPE_FLOAT32 = 11,
	ZN_TYPE_FLOAT64 = 12,
	ZN_TYPE_STRING = 13,
};

typedef struct {
	ZNValueType type;
	ZNValueData data;
} ZNValue;

#define ZNToPointer(p) ((ZNValueData) {.asObject = p}).asPointer
#define ZNFromPointer(p) ((ZNValueData) {.asPointer = p}).asObject
#define ZNToInt8(p) ((ZNValueData) {.asObject = p}).asInt8
#define ZNFromInt8(p) ((ZNValueData) {.asInt8 = p}).asObject
#define ZNToUInt8(p) ((ZNValueData) {.asObject = p}).asUInt8
#define ZNFromUInt8(p) ((ZNValueData) {.asUInt8 = p}).asObject
#define ZNToInt16(p) ((ZNValueData) {.asObject = p}).asInt16
#define ZNFromInt16(p) ((ZNValueData) {.asInt16 = p}).asObject
#define ZNToUInt16(p) ((ZNValueData) {.asObject = p}).asUInt16
#define ZNFromUInt16(p) ((ZNValueData) {.asUInt16 = p}).asObject
#define ZNToInt32(p) ((ZNValueData) {.asObject = p}).asInt32
#define ZNFromInt32(p) ((ZNValueData) {.asInt32 = p}).asObject
#define ZNToUInt32(p) ((ZNValueData) {.asObject = p}).asUInt32
#define ZNFromUInt32(p) ((ZNValueData) {.asUInt32 = p}).asObject
#define ZNToInt64(p) ((ZNValueData) {.asObject = p}).asInt64
#define ZNFromInt64(p) ((ZNValueData) {.asInt64 = p}).asObject
#define ZNToUInt64(p) ((ZNValueData) {.asObject = p}).asUInt64
#define ZNFromUInt64(p) ((ZNValueData) {.asUInt64 = p}).asObject
#define ZNToFloat32(p) ((ZNValueData) {.asObject = p}).asFloat32
#define ZNFromFloat32(p) ((ZNValueData) {.asFloat32 = p}).asObject
#define ZNToFloat64(p) ((ZNValueData) {.asObject = p}).asFloat64
#define ZNFromFloat64(p) ((ZNValueData) {.asFloat64 = p}).asObject
#define ZNToString(p) ((ZNValueData) {.asObject = p}).asString
#define ZNFromString(p) ((ZNValueData) {.asString = p}).asObject

typedef struct {
	ZNValue *items;
	size_t count;
	size_t alloced;
} ZNArrayData;

typedef ZNArrayData *ZNParams;
