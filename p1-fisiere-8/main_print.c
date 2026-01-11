#include "./old/bin_mat_mem_utils.h"

int main(int argc, char *argv[]) {
   if (argc != 2) {
      fprintf(stderr, "Usage: %s <file>\n", argv[0]);
      return -1;
   }
   const char *file = argv[1];
   BinMatrix* bm = BinMat_load_file(file);
   if (bm == NULL) {
      fprintf(stderr, "Eroare incarcare '%s' matrice din fisier\n", file);  
      return -1; 
   }
   
   BinMat_print(bm);
   BinMat_free_ptr(bm);
   return 0;
}