#include "bin_mat_cerinte.h"


int main(int argc, char *argv[]) {
   if (argc != 4) {
      fprintf(stderr, "Usage: %s <file1> <file2> <file_OUT>\n", argv[0]);
      return -1;
   }

   const char *file1 = argv[1];
   const char *file2 = argv[2];
   const char *file_out = argv[3];

   BinMatrix* bm1 = BinMat_load_file(file1);
   if (bm1 == NULL) {
      fprintf(stderr, "Eroare incarcare matrice din fisier\n");  
      return -1;
   }
   printf("Matrix bm1 loaded from file:\n");
   BinMat_print(bm1);

   BinMatrix* bm2 = BinMat_load_file(file2);
   if (bm2 == NULL) {
      fprintf(stderr, "Eroare incarcare matrice din fisier\n");  
      BinMat_free_ptr(bm1);
      return -1;
   }
   printf("Matrix bm2 loaded from file:\n");
   BinMat_print(bm2);

   BinMatrix* bm_sum = BinMat_sum(bm1, bm2);
   if (bm_sum == NULL) {
      fprintf(stderr, "Eroare calcul suma matrici\n");  
      BinMat_free_ptr(bm1);
      BinMat_free_ptr(bm2);
      return -1;
   }
   printf("Sum matrix:\n");
   BinMat_print(bm_sum);
   // save to file
   BinMat_save_file(bm_sum, file_out);

   BinMat_free_ptr(bm1);
   BinMat_free_ptr(bm2);
   BinMat_free_ptr(bm_sum);
   return 0;
}