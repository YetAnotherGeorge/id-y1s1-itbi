#include "bin_mat_cerinte.h"

int main(int argc, char *argv[]) {
   if (argc != 2) {
      fprintf(stderr, "Usage: %s <file>\n", argv[0]);
      return -1;
   }
   const char *file = argv[1];
   BinMatrix* bm = BinMat_load_file(file);
   if (bm == NULL) {
      fprintf(stderr, "Eroare incarcare matrice din fisier\n");  
      return -1; 
   }
   printf("Matrix loaded from file '%s':\n", file);
   BinMat_print(bm);
   BinMat_free_ptr(bm);
   return 0;
}