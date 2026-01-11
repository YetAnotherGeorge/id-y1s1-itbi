#include "./old/bin_mat_mem_utils.h"

int main(int argc, char *argv[]) {
   if (argc != 2) {
      char msg[100];
      snprintf(msg, sizeof(msg), "Usage: %s <file>", argv[0]);
      perror(msg);
      return -1;
   }
   const char *file = argv[1];
   BinMatrix* bm = BinMat_load_file(file);
   if (bm == NULL) {
      perror("Eroare incarcare matrice din fisier");  
      return -1; 
   }
   
   BinMat_print(bm);
   BinMat_free_ptr(bm);
   return 0;
}