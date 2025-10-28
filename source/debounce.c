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

       

    
    
    
