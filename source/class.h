#pragma once

struct ZNClassData;
typedef struct ZNClassData *ZNClass;

typedef const char *ZNSelector;
typedef ZNObject (*ZNMethodFunction)(ZNObject this, ZNClass class, ZNObject in);

/**
 * Information about a method on a class
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
 * Information about a feild on a class
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
	ZNMap feilds;
	size_t total_size;
	size_t local_size;
	
	ZNMap methods;
	
	ZNClass parent;
} ZNClassData;

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

ZNClass ZNCreateEmptyClass(void);
ZNResult ZNClassAddMethod(ZNClass this, ZNSelector selector, ZNMethodFunction func, ZNMethodFlags flags);
ZNMethodFunction ZNClassGetMethodFunction(ZNClass this, ZNSelector selector, ZNClass *assoc);
ZNResult ZNClassAddFeild(ZNClass this, ZNSelector selector, size_t size, ZNFeildFlags flags);
int64_t ZNClassGetFeildOffset(ZNClass this, ZNSelector selector);
void ZNClassSetParent(ZNClass this, ZNClass new_parent);
ZNResult ZNClassRecalculate(ZNClass this);
