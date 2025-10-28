#include "debounce.h"


void initFilterTypeBool (filterTypeBool_t *filter, initValue){
    filter->counter = 0;
    filter->previousValue = initValue;
    filter->currentValue = initValue;
}

       

    
    
    
