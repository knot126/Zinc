#include "object.h"

ZNObject ZNObjectCreate(ZNObject prototype) {
	/**
	 * Create a new object
	 */
	
	ZNObject this = ZNMemory(NULL, sizeof *this);
	
	if (!this) {
		return NULL;
	}
	
	ZNClean(this, sizeof *this);
	
	ZNMapInit(&this->feilds);
	ZNMapInit(&this->methods);
	
	this->prototype = prototype;
	
	ZNObjectSendMessage(this, "create");
	
	return this;
}

ZNObject ZNObjectRelease(ZNObject this) {
	/**
	 * Destroy an object and free its resources
	 */
	
	ZNObjectSendMessage(this, "release");
	
	ZNMapRelease(&this->methods);
	ZNMapRelease(&this->feilds);
	
	ZNMemory(this, 0);
	
	return NULL;
}

ZNResult ZNObjectAddMethod(ZNObject this, ZNSelector named, ZNMethod method) {
	/**
	 * Add a method to the given object.
	 */
	
	return ZNMapAppend(&this->methods, named, method);
}

static ZNMethod ZNObjectGetMethodPtr(ZNObject this, ZNSelector which) {
	/**
	 * Get a pointer to the function that implements the method.
	 */
	
	ZNMethod method = (ZNMethod) ZNMapGet(&this->methods, which);
	
	if (!method && this->prototype) {
		method = ZNObjectGetMethodPtr(this->prototype, which);
	}
	
	return method;
}

static ZNStorable ZNObjectSendMessageUsingParameterObjectWithAnyReturnType(ZNObject this, ZNSelector sel, ZNObject params) {
	/**
	 * Send a message to an object, expecting any return type and already having
	 * made a parameter object.
	 */
	
	ZNMethod method = ZNObjectGetMethodPtr(this, sel);
	
	ZNStorable result = {.asPointer = NULL};
	
	if (method) {
		result = method(this, params);
	}
	
	if (params) {
		ZNObjectRelease(params);
	}
	
	return result;
}

static ZNObject ZNObjectSendMessageUsingParameterObject(ZNObject this, ZNSelector sel, ZNObject params) {
	return ZNObjectSendMessageUsingParameterObjectWithAnyReturnType(this, sel, params).asObject;
}

ZNStorable ZNObjectGetFeild(ZNObject this, ZNSelector which) {
	/**
	 * Get the value of the given feild.
	 */
	
	return (ZNStorable) {.asPointer = ZNMapGet(&this->feilds, which)};
}

ZNResult ZNObjectSetFeild(ZNObject this, ZNSelector which, ZNStorable what) {
	/**
	 * Set the value of the given feild.
	 */
	
	return ZNMapSet(&this->feilds, which, what.asPointer);
}

bool ZNObjectHasFeild(ZNObject this, ZNSelector which) {
	/**
	 * Check if the object has the feild of a given name.
	 */
	
	return ZNMapHas(&this->feilds, which);
}
