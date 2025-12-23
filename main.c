#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h> // contine mode_t
#include <fcntl.h>
#include <unistd.h>

// Structura binara fisier matrice:
typedef struct BinMatrix {
   char file_identifier[4]; // "bmat" (4 bytes=1 int)
   int rows;
   int cols;
   float *data;
} BinMatrix;


/**
 * Creeaza o matrice m x n de float-uri initiata cu 0.0
 * care este scrisa in f
 */
void new(int f, int m, int n) {
   BinMatrix bm;
   bm.file_identifier[0] = 'b';
   bm.file_identifier[1] = 'm';
   bm.file_identifier[2] = 'a';
   bm.file_identifier[3] = 't';
   bm.rows = m;
   bm.cols = n;
   bm.data = NULL; // scris direct in fisier

   // header
   write(f, &bm.file_identifier, sizeof(bm.file_identifier));
   write(f, &bm.rows, sizeof(bm.rows));
   write(f, &bm.cols, sizeof(bm.cols));
   // date
   float zero = 0.0f;
   for (int i = 0; i < m * n; i++) {
      write(f, &zero, sizeof(float));
   }
}

int main(int argc, char *argv[]) {
   printf("Implementare matrici dreptunghiulare de numere reale\n");

   int mat_new_fd = open("matrix.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);
   if (mat_new_fd == -1) {
      perror("Eroare creare fisier matrix.bin");
      return 1;
   }
   printf("Fisier matrix.bin creat: %d\n", mat_new_fd);
}