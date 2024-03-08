#include "util.h"

void *ZNMemory(void *block, size_t size) {
	/**
	 * If `block` == NULL and size > 0, allocate new memory of `size` and return it
	 * If `block` != NULL and size > 0, rellocate block to new size and return it
	 * If `block` != NULL and size == 0, free the block and return NULL
	 * If `block` == NULL and size == 0, nop
	 */
	
	if (block && size) {
		return realloc(block, size);
	}
	else if (block && !size) {
		free(block);
		return NULL;
	}
	else if (!block && size) {
		return malloc(size);
	}
	else {
		return NULL;
	}
}

void ZNClean(void *block, size_t size) {
	/**
	 * Memset(0) on the given block
	 */
	
	memset(block, 0, size);
}

char *ZNStringDuplicate(const char * const string) {
	/**
	 * Duplicate the given string.
	 */
	
	return strdup(string);
}

bool ZNStringEqual(const char *str1, const char *str2) {
	return !strcmp(str1, str2);
}

size_t ZNStringCountChar(const char *str, char c) {
	/**
	 * Count the number of instances of `c` in `str`.
	 */
	
	size_t count = 0;
	
	for (size_t i = 0; str[i]; i++) {
		if (str[i] == c) {
			count++;
		}
	}
	
	return count;
}
