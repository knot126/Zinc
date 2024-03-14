#pragma once

#include "common.h"
#include "params.h"

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
