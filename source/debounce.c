#include "debounce.h"

// Funkce filtru
void initFilterTypeBool (filterTypeBool_t *filter, bool initValue){
    filter->counter = 0;
    filter->previousValue = initValue;
    filter->currentValue = initValue;
}
// Detekce zmeny
bool runFilterTypeBool (filterTypeBool_t *filter, bool value){
    if (value != filter->previousValue){
        filter->counter++;
        // Pocet cyklu potrebných pro zmenu stavu LED
        if (filter->counter >= 3){
            filter->previousValue = value;
            filter->currentValue = value;
            filter->counter = 0;
        }
    }else {
        filter->counter = 0;    // Reset cítace
    }
    return filter->currentValue;
}