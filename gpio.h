#ifndef GPIO_H
#define GPIO_H

#include "MKL05Z4.h"

// Adresy bazowe GPIO (zgodne z dokumentacja KL05Z)
#define GPIOA ((GPIO_Type *)0x400FF000u) // Baza rejestrów GPIOA
#define GPIOB ((GPIO_Type *)0x400FF040u) // Baza rejestrów GPIOB

// Funkcja inicjalizujaca pin jako wyjscie
void gpio_set_output(PORT_Type *port, GPIO_Type *gpio, uint32_t pin);

// Ustawienie pinu na stan wysoki
void gpio_set_high(GPIO_Type *gpio, uint32_t pin);

// Ustawienie pinu na stan niski
void gpio_set_low(GPIO_Type *gpio, uint32_t pin);

// Zmiana stanu pinu na przeciwny
void gpio_toggle(GPIO_Type *gpio, uint32_t pin);

#endif // GPIO_H