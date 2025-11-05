#ifndef DEBOUNCE_H
#define	DEBOUNCE_H

#include "platformMIS32mk.h"
#include <stdbool.h>
#include <stdint.h>

// Definice struktur

typedef struct {
unsigned int counter;
bool previousValue;
bool currentValue;
} filterTypeBool_t;


// Deklarace funkcí
void initFilterTypeBool (filterTypeBool_t *filter, bool initValue);
bool runFilterTypeBool (filterTypeBool_t *filter, bool value);



#endif