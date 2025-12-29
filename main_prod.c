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
   BinMat_print(bm1);

   BinMatrix* bm2 = BinMat_load_file(file2);
   if (bm2 == NULL) {
      fprintf(stderr, "Eroare incarcare matrice din fisier\n");  
      BinMat_free_ptr(bm1);
      return -1;
   }
   BinMat_print(bm2);

   BinMatrix* bm_prod = BinMat_prod(bm1, bm2);
   if (bm_prod == NULL) {
      fprintf(stderr, "Eroare calcul produs matrici\n");  
      BinMat_free_ptr(bm1);
      BinMat_free_ptr(bm2);
      return -1;
   }
   printf("Product matrix:\n");
   BinMat_print(bm_prod);
   // save to file
   BinMat_save_file(bm_prod, file_out);

   BinMat_free_ptr(bm1);
   BinMat_free_ptr(bm2);
   BinMat_free_ptr(bm_prod);
   return 0;
}
