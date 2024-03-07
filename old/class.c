#include "class.h"
#include "zn_config.h"

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

ZNMethodFunction ZNClassGetMethodFunction(ZNClass this, ZNSelector selector, ZNClass *assoc) {
	/**
	 * Get a reference to the internal function that implements the given
	 * method.
	 * 
	 * @param this Class to find method on
	 * @param selector Selector of the method
	 * @param assoc Optional return value for the class that's assocaited with
	 * the method when found. This may not be the same as the passed class when
	 * using a function from a parent class.
	 * @return Pointer to the function
	 */
	
	ZNMethodFunction out = NULL;
	
	ZNMethodData *md = ZNMapGet(&this->methods, selector);
	
	if (md) {
		out = md->function;
		
		if (assoc) {
			*assoc = this;
		}
	}
	else if (this->parent) {
		out = ZNClassGetMethodFunction(this->parent, selector, assoc);
	}
	
	return out;
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

int64_t ZNClassGetFeildOffset(ZNClass this, ZNSelector selector) {
	/**
	 * Get the offset to a feild `selector` in an object of class `this`.
	 * Returns -1 on failure.
	 */
	
	ZNFeildData *fd = ZNMapGet(&this->feilds, selector);
	int64_t out = -1;
	
	if (fd) {
		out = fd->offset;
	}
	else if (this->parent) {
		out = ZNClassGetFeildOffset(this->parent, selector);
	}
	
	return out;
}

void ZNClassSetParent(ZNClass this, ZNClass new_parent) {
	/**
	 * Set the parent class for this class to the given class.
	 */
	
	this->parent = new_parent;
}

static ZNResult ZNClassRecalculateSizes(ZNClass this) {
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

ZNResult ZNClassRecalculate(ZNClass this) {
	return ZNClassRecalculateSizes(this);
}

ZNClass ZNCreateClass(ZNClass parent, ZNClassFeild *feilds[], ZNClassMethod *methods[]) {
	/**
	 * 
	 */
	
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
