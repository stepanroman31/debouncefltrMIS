#include "debounce.h"

// Funkce filtru
void initFilterTypeBool (filterTypeBool_t *filter, bool initValue){
    filter->counter = 0;
    filter->previousValue = initValue;
    filter->currentValue = initValue;
}
// Nastavení pocatecnich hodnot
void initMemoryTypeBool (memoryTypeBool_t *mem, bool initValue){
    mem->previousInputValue = false;
    mem->outputValue = initValue;
}
//Logika pameti
bool runMemoryTypeBool (memoryTypeBool_t *mem, bool value){
    if (value == true && mem->previousInputValue == false){
        mem->outputValue = !mem->outputValue;
    }
    mem->previousInputValue = value;
    return mem->outputValue;
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

       

    
    
    
