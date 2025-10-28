#include "debounce.h"

// Funkce filtru
void initFilterTypeBool (filterTypeBool_t *filter, bool initValue){
    filter->counter = 0;
    filter->previousValue = initValue;
    filter->currentValue = initValue;
}
// Nastavení po?áte?ních hodnot
void initMemoryTypeBool (memoryTypeBool_t *mem, bool initValue){
    mem->previousInputValue = false;
    mem->outputValue = initValue;
}
//Logika pam?ti
bool runMemoryTypeBool (memoryTypeBool_t *mem, bool value){
    if (value == true && mem->previousInputValue == false){
        mem->outputValue = !mem->outputValue;
    }
    mem->previousInputValue = value;
    return mem->outputValue;
}
// Detekce zm?ny
bool runFilterTypeBool (filterTypeBool_t *filter, bool value){
    if (value != filter->previousValue){
        filter->counter++;
        // Po?et cykl? pot?ebných pro zm?nu stavu LED
        if (filter->counter >= 3){
            filter->previousValue = value;
            filter->currentValue = value;
            filter->counter = 0;
        }
    }else {
        filter->counter = 0;    // Reset ?íta?e
    }
    return filter->currentValue;
}

       

    
    
    
