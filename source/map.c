#include "map.h"

ZNResult ZNMapInit(ZNMap *this) {
	/**
	 * Initialise memory for the map
	 */
	
	ZNClean(this, sizeof *this);
}

void ZNMapRelease(ZNMap *this) {
	/**
	 * Release the map's memory
	 */
	
	for (size_t i = 0; i < this->count; i++) {
		ZNMemory(this->keys[i], 0);
	}
	
	ZNMemory(this->keys, 0);
	ZNMemory(this->values, 0);
}

static int64_t ZNMapFind(ZNMap *this, ZNMapKey key) {
	if (!this->keys || !this->values) {
		return -1;
	}
	
	for (size_t i = 0; i < this->count; i++) {
		if (ZNStringEqual(this->keys[i], key)) {
			return i;
		}
	}
	
	return -1;
}

static ZNReuslt ZNMapExtend(ZNMap *this) {
	this->prealloc = 2 + 2 * this->prealloc;
	
	this->keys = ZNMemory(this->keys, this->prealloc);
	this->values = ZNMemory(this->values, this->prealloc);
	
	if (!this->keys || !this->values) {
		return ZN_FAIL;
	}
	
	return ZN_SUCCESS;
}

ZNResult ZNMapAppend(ZNMap *this, ZNMapKey key, ZNMapValue value) {
	if (this->prealloc >= this->count) {
		ZNResult e = ZNMapExtend(this);
		
		if (e) {
			return e;
		}
	}
	
	this->keys[this->count] = ZNStringDuplicate(key);
	
	if (!this->keys[this->count]) {
		return ZN_FAIL;
	}
	
	this->values[this->count] = value;
	
	this->count++;
	
	return ZN_SUCCESS;
}

ZNResult ZNMapSet(ZNMap *this, ZNMapKey key, ZNMapValue value) {
	int64_t index = ZNMapFind(this, key);
	
	if (index < 0) {
		return ZNMapAppend(this, key, value);
	}
	else {
		this->values[index] = value;
		return ZN_SUCCESS;
	}
}

static ZNMapValue ZNMapAt(ZNMap *this, size_t index) {
	if (index < this->count) {
		return this->values[index];
	}
	
	return NULL;
}

ZNMapValue ZNMapGet(ZNMap *this, ZNMapKey key) {
	int64_t index = ZNMapFind(this, key);
	
	if (index < 0) {
		return NULL;
	}
	
	return ZNMapAt(this, index);
}

bool ZNMapHas(ZNMap *this, ZNMapKey key) {
	return ZNMapFind(this, key) >= 0;
}

size_t ZNMapCount(ZNMap *this) {
	return this->count;
}

ZNMap *ZNMapFreeValues(ZNMap *this) {
	for (size_t i = 0; i < ZNMapCount(this); i++) {
		ZNMemory(ZNMapAt(this, i), 0);
	}
	
	return this;
}
