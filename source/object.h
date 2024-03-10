#pragma once

#include "common.h"

/**
 * Generic type container
 */
typedef union ZNStorable {
	void *asPointer;
	ZNObject asObject;
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

#define ZNToPointer(p) ((ZNStorable) {.asObject = p}).asPointer
#define ZNFromPointer(p) ((ZNStorable) {.asPointer = p}).asObject
#define ZNToInt8(p) ((ZNStorable) {.asObject = p}).asInt8
#define ZNFromInt8(p) ((ZNStorable) {.asInt8 = p}).asObject
#define ZNToUInt8(p) ((ZNStorable) {.asObject = p}).asUInt8
#define ZNFromUInt8(p) ((ZNStorable) {.asUInt8 = p}).asObject
#define ZNToInt16(p) ((ZNStorable) {.asObject = p}).asInt16
#define ZNFromInt16(p) ((ZNStorable) {.asInt16 = p}).asObject
#define ZNToUInt16(p) ((ZNStorable) {.asObject = p}).asUInt16
#define ZNFromUInt16(p) ((ZNStorable) {.asUInt16 = p}).asObject
#define ZNToInt32(p) ((ZNStorable) {.asObject = p}).asInt32
#define ZNFromInt32(p) ((ZNStorable) {.asInt32 = p}).asObject
#define ZNToUInt32(p) ((ZNStorable) {.asObject = p}).asUInt32
#define ZNFromUInt32(p) ((ZNStorable) {.asUInt32 = p}).asObject
#define ZNToInt64(p) ((ZNStorable) {.asObject = p}).asInt64
#define ZNFromInt64(p) ((ZNStorable) {.asInt64 = p}).asObject
#define ZNToUInt64(p) ((ZNStorable) {.asObject = p}).asUInt64
#define ZNFromUInt64(p) ((ZNStorable) {.asUInt64 = p}).asObject
#define ZNToFloat32(p) ((ZNStorable) {.asObject = p}).asFloat32
#define ZNFromFloat32(p) ((ZNStorable) {.asFloat32 = p}).asObject
#define ZNToFloat64(p) ((ZNStorable) {.asObject = p}).asFloat64
#define ZNFromFloat64(p) ((ZNStorable) {.asFloat64 = p}).asObject
#define ZNToString(p) ((ZNStorable) {.asObject = p}).asString
#define ZNFromString(p) ((ZNStorable) {.asString = p}).asObject

typedef const char *ZNSelector;
typedef ZNObject (*ZNMethod)(ZNObject this, ZNObject in);

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

ZNResult ZNObjectAddMethod(ZNObject this, ZNSelector named, ZNMethod method);
ZNObject ZNSendMessage(ZNObject this, ZNSelector sel, ...);

void *ZNGetVoidp(ZNObject this, ZNSelector which);
uint64_t ZNGetUInt64(ZNObject this, ZNSelector which);

ZNResult ZNSetVoidp(ZNObject this, ZNSelector which, void *what);
ZNResult ZNSetUInt64(ZNObject this, ZNSelector which, uint64_t what);
