#include "object.h"

ZNObject zinc__paramPrototype;

ZNResult ZNSetParamObjectPrototype(ZNObject obj) {
	zinc__paramPrototype = obj;
	return ZN_SUCCESS;
}

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
	
	ZNObjectSendMessageWithoutParams(this, "create");
	
	return this;
}

ZNObject ZNObjectRelease(ZNObject this) {
	/**
	 * Destroy an object and free its resources
	 */
	
	ZNObjectSendMessageWithoutParams(this, "release");
	
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
	
	if (!this) {
		return NULL;
	}
	
	ZNMethod method = (ZNMethod) ZNMapGet(&this->methods, which);
	
	if (!method && this->prototype) {
		method = ZNObjectGetMethodPtr(this->prototype, which);
	}
	
	return method;
}

static ZNObject ZNObjectSendMessageUsingParameterObject(ZNObject this, ZNSelector sel, ZNObject params) {
	/**
	 * Send a message to an object, expecting any return type and already having
	 * made a parameter object.
	 */
	
	ZNMethod method = ZNObjectGetMethodPtr(this, sel);
	
	ZNObject result = NULL;
	
	if (method) {
		result = method(this, params);
	}
	
	if (params) {
		ZNObjectRelease(params);
	}
	
	return result;
}

static ZNObject ZNObjectSendMessageWithoutParams(ZNObject this, ZNSelector sel) {
	return ZNObjectSendMessageUsingParameterObject(this, sel, NULL);
}

static ZNObject ZNObjectSendMessageX(ZNObject this, ZNSelector sel, void *param) {
	/**
	 * Send a special type of message where the parameter is voidp instead of
	 * a paramter object.
	 */
	
	ZNMethod method = ZNObjectGetMethodPtr(this, sel);
	
	ZNObject result = NULL;
	
	if (method) {
		result = method(this, (ZNObject) param);
	}
	
	return result;
}

static ZNObject ZNObjectSendMessageUsingPrototypeWithVaradicList(ZNObject this, ZNSelector sel, ZNObject params_type, va_list args) {
	/**
	 * Given the prototype for a parameter object, process the given va_list
	 * into a valid parameter object.
	 */
	
	ZNObject params = ZNObjectCreate(params_type);
	size_t param_count = ZNStringCountChar(sel, ':');
	
	if (ZNObjectSendMessageWithoutParams(params, "begin").asInteger64) {
		return NULL;
	}
	
	for (size_t i = 0; i < param_count; i++) {
		if (ZNObjectSendMessageX(params, "put:", va_arg(args, void *)).asInteger64) {
			return NULL;
		}
	}
	
	if (ZNObjectSendMessageWithoutParams(params, "end").asInteger64) {
		return NULL;
	}
	
	return ZNObjectSendMessageUsingParameterObject(this, sel, params);
}

ZNObject ZNSendMessage(ZNObject this, ZNSelector sel, ...) {
	/**
	 * Send a message named by `sel` to the object `this`, which the parameters
	 * as given in the varadic list. Note that the parameters must be pointers,
	 * so you will probably prefix every parameter with `&`, for example:
	 * 
	 * ZNObject result = ZNSendMessage(myObj, "withA:andB:andC:", &a, &b, &c);
	 * 
	 * Remember that it's perfectly valid to do this with constants, too:
	 * 
	 * ZNObject result = ZNSendMessage(myObj, "initWithBuffer:size:", &buf, &0x400);
	 * 
	 * @warning Remember that you still need to prefix strings with &:
	 * 
	 * ZNObject result = ZNSendMessage(myObj, "withCString:", &"test");
	 * 
	 * @warning Also note that the number of arguments is assumed from the number
	 * of colons in the selector.
	 * 
	 * @param this Target object
	 * @param sel Selector of the message (it's name)
	 * @param ... Pointers to parameters
	 * @return Result
	 */
	
	va_list args;
	va_start(args, ZNStringCountChar(sel, ':'));
	ZNObject ret = ZNObjectSendMessageUsingPrototypeWithVaradicList(this, sel, zinc__paramPrototype, );
	va_end(args);
	
	return ret;
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
