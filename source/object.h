#pragma once

#include "common.h"

struct ZNClassData;
struct ZNObjectData;

typedef struct ZNClassData *ZNClass;
typedef struct ZNObjectData *ZNObject;

typedef const char *ZNSelector;
typedef ZNObject (*ZNMethodFunction)(ZNObject this, ZNClass class, ZNObject in);

enum {
	ZN_ALIGN = 0x10,
};

/**
 * Information about a method on an object
 */
typedef uint32_t ZNMethodFlags;
enum {
	ZN_METHOD_CLASS = (1 << 0),
};

typedef struct {
	ZNMethodFunction function;
	ZNMethodFlags flags;
} ZNMethodData;

/**
 * Information about a feild on an object
 */
typedef uint32_t ZNFeildFlags;

typedef struct {
	size_t size;
	size_t offset;
	ZNFeildFlags flags;
} ZNFeildData;

/**
 * Information about a class
 */
typedef struct ZNClassData {
	//ZNFeildData *feilds;
	//size_t feild_count;
	ZNMap feilds;
	size_t total_size;
	size_t local_size;
	
	//ZNMethodData *methods;
	//size_t method_count;
	ZNMap methods;
	
	ZNClass parent;
} ZNClassData;

/**
 * Object
 */
typedef struct ZNObjectData {
	ZNClass class;
	void *data;
} ZNObjectData;

/**
 * Program context
 */
// typedef struct {
// 	ZNMap classes;
// } ZNProgram;

/**
 * Class creation info
 */
typedef struct {
	ZNSelector sel; // Name of the method
	ZNMethodFunction func; // which function to call
	bool for_class; // if this is a class method or not
} ZNClassMethod;

typedef struct {
	ZNSelector sel; // name of the feild
	size_t size; // size of the feild
	// offset is computed automatically
} ZNClassFeild;

/**
 * Program wide info
 */
typedef struct {
	ZNClass default_class;
	ZNClass class_ref;
} ZNProgram;
