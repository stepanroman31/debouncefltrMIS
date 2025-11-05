

#ifndef MEMORY_H
#define	MEMORY_H

#include "platformMIS32mk.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct{
    bool previousInputValue;
    bool outputValue;
}memoryTypeBool_t;

void initMemoryTypeBool (memoryTypeBool_t *mem, bool initValue);
bool runMemoryTypeBool (memoryTypeBool_t *mem, bool value);


#endif	/* MEMORY_H */

