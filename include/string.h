#ifndef STRING_H
#define STRING_H

#include <stdint.h>

void memcpy(void *dest, void *src, unsigned long n);
void memset(char *dest, char val, unsigned long n);

int streq(char *a, char *b);
int strlen(char *s);

char *utos(uint32_t v, uint8_t base, char *buf);

#endif // STRING_H
