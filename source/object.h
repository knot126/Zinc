#pragma once

#include "common.h"

/**
 * Object
 */
typedef struct ZNObjectData {
	ZNClass class;
	void *data;
} ZNObjectData;

typedef struct ZNObjectData *ZNObject;

/**
 * Program wide info
 */
typedef struct {
	ZNClass default_class;
	ZNClass class_ref;
} ZNProgram;
