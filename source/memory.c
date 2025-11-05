#include "memory.h"

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