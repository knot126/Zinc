#include "object.h"

ZNProgram zinc;

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

ZNClass ZNCreateEmptyClass(void) {
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
		
		// Set new offset, since we have that
		fd->offset = offset;
		
		// Find padding (could be improved)
		size_t padding = ZN_ALIGN - (fd->size % ZN_ALIGN);
		
		// Calculate offset and new local size
		offset += fd->size + padding;
		local += fd->size + padding;
	}
	
	this->total_size = offset;
	this->local_size = local;
	
	return ZN_SUCCESS;
}

ZNClass ZNCreateClass(ZNClass parent, ZNClassFeild *feilds[], ZNClassMethod *methods[]) {
	ZNResult r;
	ZNClass cls = ZNCreateEmptyClass();
	
	if (!cls) {
		return NULL;
	}
	
	if (parent) {
		ZNClassSetParent(cls, parent);
	}
	else {
		ZNClassSetParent(cls, zinc.default_class);
	}
	
	for (size_t i = 0; feilds[i] != NULL; i++) {
		r = ZNClassAddFeild(cls, feilds[i]->sel, feilds[i]->size, 0);
		
		if (r) {
			ZNClassRelease(cls);
			return NULL;
		}
	}
	
	for (size_t i = 0; methods[i] != NULL; i++) {
		r = ZNClassAddMethod(cls, methods[i]->sel, methods[i]->func, methods[i]->for_class ? ZN_METHOD_CLASS : 0);
		
		if (r) {
			ZNClassRelease(cls);
			return NULL;
		}
	}
	
	r = ZNClassRecalculateSizes(cls);
	
	if (r) {
		ZNClassRelease(cls);
		return NULL;
	}
	
	return cls;
}

ZNObject ZNCreateObject(ZNClass class) {
	ZNObject this = ZNMemory(NULL, sizeof *this);
	
	if (!this) {
		return NULL;
	}
	
	ZNClean(this, sizeof *this);
	
	this->class = class;
	this->data = this->class ? ZNMemory(NULL, this->class->total_size) : NULL;
	
	return this;
}

ZNObject ZNObject_getClass(ZNObject this, ZNClass cls, ZNObject in) {
	return (ZNObject) cls;
}

ZNResult ZNInit(void) {
	zinc.default_class = ZNCreateClass(
		NULL,
		(ZNClassFeild *[]) {
			NULL,
		},
		(ZNClassMethod *[]) {
			&(ZNClassMethod) {"getClass", &ZNObject_getClass, false},
			NULL,
		}
	);
}
