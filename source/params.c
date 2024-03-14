#include "params.h"

ZNArray ZNArrayCreate(void) {
	/**
	 * Create a new array
	 */
	
	ZNArray arr = ZNMemory(NULL, sizeof *arr);
	
	if (!arr) {
		return NULL;
	}
	
	ZNClean(arr, sizeof *arr);
	
	return arr;
}

void ZNArrayDestroy(ZNArray this) {
	/**
	 * Destroy an array and assocaited memory
	 */
	
	ZNMemory(this->items, 0);
	ZNMemory(this, 0);
}

static ZNResult ZNArrayExpand(ZNArray this) {
	/**
	 * Expand an array
	 */
	
	this->alloced = 2 + 2 * this->alloced;
	this->items = ZNMemory(this->items, sizeof *this->items * this->alloced);
	
	return (this->items) ? ZN_SUCCESS : ZN_FAIL;
}

static ZNResult ZNArrayPush(ZNArray this, ZNValue value) {
	/**
	 * Push a new value onto the end of the array
	 */
	
	if (this->count <= this->alloced) {
		ZNResult result = ZNArrayExpand(this);
		
		if (result) {
			return result;
		}
	}
	
	this->items[this->count] = value;
	this->count++;
	
	return ZN_SUCCESS;
}

static ZNValue ZNArrayGet(ZNArray this, int64_t index) {
	/**
	 * Get an item from the index
	 */
	
	if (index < 0) {
		index = index + this->count;
	}
	
	if (index < 0 || index >= this->count) {
		return (ZNValue) {ZN_TYPE_NIL, ZNToUInt64(0)};
	}
	
	return this->items[index];
}
