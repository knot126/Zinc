#include "object.h"

ZNClass base_class;

static ZNResult ZNClassInit(ZNClass this) {
	ZNClean(this, sizeof *this);
	ZNMapInit(&this->feilds);
	ZNMapInit(&this->methods);
	return ZN_SUCCESS;
}

static void ZNClassRelease(ZNClass this) {
	ZNMapRelease(ZNMapFreeValues(&this->feilds));
	ZNMapRelease(ZNMapFreeValues(&this->methods));
}

ZNClass ZNCreateClass(void) {
	/**
	 * Create a new class.
	 */
	
	ZNClass this = ZNMemory(NULL, sizeof *this);
	
	if (!this) {
		return NULL;
	}
	
	ZNResult r = ZNClassInit(this);
	
	if (r) {
		return NULL;
	}
	
	return this;
}

ZNResult ZNClassAddMethod(ZNClass this, ZNSelector selector, ZNMethodFunction func, ZNMethodFlags flags) {
	/**
	 * Add a method to an existing class.
	 */
	
	ZNMethodData *md = ZNMemory(NULL, sizeof *md);
	
	if (!md) {
		return ZN_FAIL;
	}
	
	ZNResult r = ZNMapAppend(&this->methods, selector, md);
	
	if (r) {
		return r;
	}
	
	md->function = func;
	md->flags = flags;
	
	return ZN_SUCCESS;
}

ZNResult ZNClassAddFeild(ZNClass this, ZNSelector selector, size_t size, ZNFeildFlags flags) {
	/**
	 * Add a feild to an existing class.
	 */
	
	ZNFeildData *fd = ZNMemory(NULL, sizeof *fd);
	
	if (!fd) {
		return ZN_FAIL;
	}
	
	ZNResult r = ZNMapAppend(&this->feilds, selector, fd);
	
	if (r) {
		return r;
	}
	
	md->size = size;
	md->flags = flags;
	
	return ZN_SUCCESS;
}

void ZNClassSetParent(ZNClass this, ZNClass new_parent) {
	this->parent = new_parent;
}

ZNResult ZNClassRecalculateSizes(ZNClass this) {
	size_t local = 0, offset = 0;
	ZNResult r;
	
	// If we have a parent, update for the parent first
	if (this->parent) {
		r = ZNClassRecalculateSizes(this->parent);
		
		if (r) {
			return r;
		}
		
		offset = this->parent->total_size;
	}
	
	size_t feild_count = ZNMapCount(&this->feilds);
	
	for (size_t i = 0; i < feild_count; i++) {
		ZNFeildData *fd = ZNMapAt(&this->feilds, i);
		ZNFeildData *fd_next = ZNMapAt(&this->feilds, i + 1);
	}
}
