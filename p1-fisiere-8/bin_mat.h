#pragma once

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h> // contine mode_t
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h> // malloc, free
#include <math.h>
// Structura binara fisier matrice:
typedef struct BinMatrix {
   char file_identifier[4]; // "bmat" (4 bytes=1 int)
   int rows;
   int cols;
   float *data; // store data in row-major order (row by row)
} BinMatrix;
