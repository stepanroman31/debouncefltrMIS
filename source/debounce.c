#include "debounce.h"

// Funkce filtru
void initFilterTypeBool (filterTypeBool_t *filter, bool initValue){
    filter->counter = 0;
    filter->previousValue = initValue;
    filter->currentValue = initValue;
}
// Nastavení po?áte?ní hodnot
void initMemoryTypeBool (memoryTypeBool_t *mem, bool initValue){
    mem->previousInputValue = false;
    mem->outputValue = initValue;
}
//Funkce pam?ti
bool runMemoryTypeBool (memoryTypeBool_t *mem, bool value){
    if (value == true && mem->previousInputValue == false){
        mem->outputValue = !mem->outputValue;
    }
    mem->previousInputValue = value;
    return mem->outputValue;
}
bool runFilterTypeBool (filterTypeBool_t *filter, bool value){
    if (value != filter->previousValue){
        filter->counter++;
        if (filter->counter >= 3){
            filter->previousValue = value;
            filter->currentValue = value;
            filter->counter = 0;
        }
    }else {
        filter->counter = 0;
    }
    return filter->currentValue;
}

       

    
    
    
