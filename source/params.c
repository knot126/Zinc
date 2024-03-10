#include "params.h"

ZNObject zinc__ZNParamHandlerBegin(ZNObject this, ZNObject in) {
	ZNSetVoidp(this, "data", NULL);
	ZNSetUInt64(this, "count", 0);
	
	return NULL;
}

ZNObject zinc__ZNParamHandlerPut(ZNObject this, ZNObject in) {
	void **data = ZNGetVoidp(this, "data");
	uint64_t count = ZNGetUInt64(this, "count");
	
	count++;
	
	data = ZNMemory(data, sizeof *data * count);
	
	data[count - 1] = (void *) in;
	
	ZNSetVoidp(this, "data", data);
	ZNSetUInt64(this, "count", count);
	
	return NULL;
}

ZNObject zinc__ZNParamHandlerGet(ZNObject this, ZNObject in) {
	return NULL;
}

ZNObject zinc__ZNParamHandlerGetPointer(ZNObject this, ZNObject in) {
	return NULL;
}

ZNObject zinc__ZNParamHandlerEnd(ZNObject this, ZNObject in) {
	return NULL;
}

ZNObject zinc__ZNParamHandlerRelease(ZNObject this, ZNObject in) {
	void **data = ZNGetVoidp(this, "data");
	
	ZNMemory(data, 0);
	
	return NULL;
}

ZNObject ZNInitParamHandler(void) {
	ZNObject ph = ZNObjectCreate(NULL);
	
	ZNObjectAddMethod(ph, "begin", zinc__ZNParamHandlerBegin);
	ZNObjectAddMethod(ph, "put:", zinc__ZNParamHandlerPut);
	ZNObjectAddMethod(ph, "get:", zinc__ZNParamHandlerGet);
	ZNObjectAddMethod(ph, "getPointer:", zinc__ZNParamHandlerGetPointer);
	ZNObjectAddMethod(ph, "end", zinc__ZNParamHandlerEnd);
	ZNObjectAddMethod(ph, "release", zinc__ZNParamHandlerRelease);
	
	ZNSetParamObjectPrototype(ph);
	
	return ph;
}
