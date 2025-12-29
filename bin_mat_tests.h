#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h> // contine mode_t
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h> // malloc, free
#include <math.h>
#include "bin_mat_cerinte.h"

void BinMat_create_write_test() {
   printf("Implementare matrici dreptunghiulare de numere reale\n");

   int mat_1_fd = open("./files/mat_1.bmat", O_WRONLY | O_CREAT | O_TRUNC, 0644);
   BinMatrix* bmat = BinMat_create(3, 4);
   BinMat_print(bmat);
   BinMat_fill_sequential(bmat);
   BinMat_print(bmat);
   BinMat_save(bmat, mat_1_fd);
   BinMat_free_ptr(bmat);
   close(mat_1_fd);

   printf("Saved mat_1.bmat\n");
}
void BinMat_load_test() {
   int mat_1_fd = open("./files/mat_1.bmat", O_RDONLY);
   BinMatrix* bmat_loaded = BinMat_load(mat_1_fd);
   if (bmat_loaded != NULL) {
      BinMat_print(bmat_loaded);
      BinMat_free_ptr(bmat_loaded);
   }
   close(mat_1_fd);
}
void BinMat_test2() {
   BinMatrix* bm = BinMat_create(3, 3);
   BinMat_set_diagonal(bm, 1.0f);
   BinMat_print(bm);
}
void BinMat_test_sum() {
   BinMatrix* bm1 = BinMat_create(2,2);
   BinMat_fill_sequential(bm1);
   printf("Matrix 1:\n");
   BinMat_print(bm1);
   
   BinMatrix* bm2 = BinMat_create(2,2);
   BinMat_set_diagonal(bm2, 1.0f);
   printf("Matrix 2:\n");
   BinMat_print(bm2);

   BinMatrix* bm_sum = BinMat_sum(bm1, bm2);
   if (bm_sum != NULL) {
      printf("Sum Matrix:\n");
      BinMat_print(bm_sum);
      BinMat_free_ptr(bm_sum);
   } else {
      printf("Error: incompatible sizes for sum\n"); 
   }
   BinMat_free_ptr(bm1);
   BinMat_free_ptr(bm2);
}
void BinMat_test_product() {
   BinMatrix* bm1 = BinMat_create(3,4);
   BinMat_fill_random(bm1, 0.0f, 9.0f);
   printf("Matrix 1:\n");
   BinMat_print(bm1);
   
   BinMatrix* bm2 = BinMat_create(4,5);
   BinMat_fill_random(bm2, 0.0f, 4.0f);
   printf("Matrix 2:\n");
   BinMat_print(bm2);

   BinMatrix* bm_pro = BinMat_prod(bm1, bm2);
   if (bm_pro != NULL) {
      printf("Product Matrix:\n");
      BinMat_print(bm_pro);
      BinMat_free_ptr(bm_pro);
   } else {
      printf("Error: incompatible sizes for product\n"); 
   }
   BinMat_free_ptr(bm1);
   BinMat_free_ptr(bm2);
}
void BinMat_test_save(int rows, int cols, const char* filename) {
   BinMatrix* bm = BinMat_create(rows, cols);
   BinMat_fill_random(bm, 0.0f, 9.0f);
   printf("Matrix %dx%d:\n", rows, cols);
   BinMat_print(bm);

   // save to file
   int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
   BinMat_save(bm, fd);
   close(fd);
   BinMat_free_ptr(bm);
   printf("Saved %s\n", filename);
}
