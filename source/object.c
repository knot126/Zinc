#include "object.h"

ZNProgram zinc;

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
