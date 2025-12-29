#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h> // contine mode_t
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h> // malloc, free
#include <math.h>
#include "bin_mat.h"

/**
 * creaza in fisierul f o matrice m x n initializata cu 0
 */
void new_matrix(int f, int m, int n) {
  BinMatrix* bm = BinMat_create(m, n);
  BinMat_save(bm, f);
  BinMat_free_ptr(bm);
}
/**
 * Read file, return value at (i, j) or NAN on error
 */
float get(int f, int i, int j) {
   // not the most efficient way, should read from file only up to OFFSET + i * cols + j
   BinMatrix* bm = BinMat_load(f);
   if (bm == NULL) {
      fprintf(stderr, "Eroare incarcare matrice din fisier\n");  
      return NAN;
   }
   float value = BinMat_get(bm, i, j);
   BinMat_free_ptr(bm);
   return value;
}
/**
 * Set value, write to file
 */
void set(int f, int i, int j, float x) {
  BinMatrix* bm = BinMat_load(f);
   if (bm == NULL) {
      fprintf(stderr, "Eroare incarcare matrice din fisier\n");  
      return;
   }
   BinMat_set(bm, i, j, x);
   BinMat_save(bm, f);
   BinMat_free_ptr(bm);  
}

/**
 * calculate sum (if compatible sizes) into file descriptor f
 */
void sum(int f1, int f2, int f) {
   // load matrices
   BinMatrix* bm1 = BinMat_load(f1);
   if (bm1 == NULL) {
      fprintf(stderr, "Eroare incarcare matrice 1 din fisier\n");  
      return;
   }
   BinMatrix* bm2 = BinMat_load(f2);
   if (bm2 == NULL) {
      fprintf(stderr, "Eroare incarcare matrice 2 din fisier\n");  
      BinMat_free_ptr(bm1);
      return;
   }

   BinMatrix* bm_sum = BinMat_sum(bm1, bm2);
   if (bm_sum == NULL) {
      fprintf(stderr, "Eroare calcul suma matrici\n");  
      BinMat_free_ptr(bm1);
      BinMat_free_ptr(bm2);
      return;
   }
   BinMat_save(bm_sum, f);
   BinMat_free_ptr(bm1);
   BinMat_free_ptr(bm2);
   BinMat_free_ptr(bm_sum);
}

/**
 * calculate product (if compatible sizes) into file descriptor f
 */
void pro(int f1, int f2, int f) {
  // load matrices
   BinMatrix* bm1 = BinMat_load(f1);
   if (bm1 == NULL) {
      fprintf(stderr, "Eroare incarcare matrice 1 din fisier\n");  
      return;
   }
   BinMatrix* bm2 = BinMat_load(f2);
   if (bm2 == NULL) {
      fprintf(stderr, "Eroare incarcare matrice 2 din fisier\n");  
      BinMat_free_ptr(bm1);
      return;
   }

   BinMatrix* bm_prod = BinMat_prod(bm1, bm2);
   if (bm_prod == NULL) {
      fprintf(stderr, "Eroare calcul produs matrici\n");  
      BinMat_free_ptr(bm1);
      BinMat_free_ptr(bm2);
      return;
   }
   BinMat_save(bm_prod, f);
   BinMat_free_ptr(bm1);
   BinMat_free_ptr(bm2);
   BinMat_free_ptr(bm_prod);
}
