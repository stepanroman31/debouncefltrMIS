#ifndef DEK_DECODER_H
#define DEK_DECODER_H

#include <stdint.h>
#include <stdbool.h>

// Deklarace globálního ?íta?e
extern uint8_t s9_counter;

// Funkce pro inicializaci a b?h
void initDecoder(void);
void runDecoder(bool A_filtered, bool B_filtered);

#endif
