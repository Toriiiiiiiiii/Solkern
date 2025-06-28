#ifndef PAGEALLOC_H
#define PAGEALLOC_H

void* kalloc_alloc(int n);
void  kalloc_free(void* ptr, int n);

#endif
