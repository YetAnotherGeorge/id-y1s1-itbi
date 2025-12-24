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
   float *data; // store data in row-major order (row by row)
} BinMatrix;

/**
 * Aloca o matrice binara cu dimensiunile date.
 * Nu uita BinMat_free pentru eliberare!
 */
BinMatrix BinMat_create(int rows, int cols) {
   BinMatrix bm;
   bm.file_identifier[0] = 'b';
   bm.file_identifier[1] = 'm';
   bm.file_identifier[2] = 'a';
   bm.file_identifier[3] = 't';
   bm.rows = rows;
   bm.cols = cols;

   void* alloc_data = malloc(rows * cols * sizeof(float));
   bm.data = (float*)alloc_data;

   for (int i = 0; i < rows * cols; i++) {
      bm.data[i] = 0.0f;
   }
}
/**
 * Elibereaza memoria alocata pentru matricea binara
 */
void BinMat_free(BinMatrix bm) {
   free(bm.data);
}
/**
 * Print to std::out
 */
void BinMat_print(BinMatrix bm) {
   printf("BinMat: rows=%d, cols=%d:\n", bm.rows, bm.cols);
   for (int i = 0; i < bm.rows; i++) {
      for (int j = 0; j < bm.cols; j++) {
         printf("%f ", bm.data[i * bm.cols + j]);
      }
      printf("\n");
   }
}
/**
 * Returns 0 for success, -1 for error
 */
int BinMat_set(BinMatrix *bm, int row, int col, float value) {
   if (row < 0 || row >= bm->rows || col < 0 || col >= bm->cols) {
      fprintf(stderr, "Index out of bounds: (%d, %d)\n", row, col);
      return -1;
   }
   int row_idx = row * bm->cols;
   int row_off = col;
   int idx = row_idx + row_off;
   bm->data[idx] = value;
   return 0;
}
/**
 * 
 */
float BinMat_get(BinMatrix* bm, int row, int col) {
   if (row < 0 || row >= bm->rows || col < 0 || col >= bm->cols) {
      fprintf(stderr, "Index out of bounds: (%d, %d)\n", row, col);
      return -1.0f; // valoare de eroare
   }
   int row_idx = row * bm->cols;
   int row_off = col;
   int idx = row_idx + row_off;
   return bm->data[idx];
}

/**
 * Salveaza matricea binara in fisierul deschis cu fd
 * Returns 0 for success, -1 for error
 */
int BinMat_save(BinMatrix* bm, int fd) {
   ssize_t write_code = 0;

   void* bm_file_identifier = &bm->file_identifier;
   write_code = write(fd, bm_file_identifier, sizeof(bm->file_identifier));
   if (write_code == -1) {
      fprintf(stderr, "Eroare scriere identificator fisier");
      return -1;
   }

   void* bm_rows = &bm->rows;
   write_code = write(fd, bm_rows, sizeof(bm->rows));
   if (write_code == -1) {
      fprintf(stderr, "Eroare scriere numar de linii");
      return -1;
   }

   void* bm_cols = &bm->cols;
   write_code = write(fd, bm_cols, sizeof(bm->cols));
   if (write_code == -1) {
      fprintf(stderr, "Eroare scriere numar de coloane");
      return -1;
   }

   void* bm_data = bm->data;
   write_code = write(fd, bm_data, bm->rows * bm->cols * sizeof(float)); 
   if (write_code == -1) {
      fprintf(stderr, "Eroare scriere date matrice");
      return -1;
   }

   printf("Matrice salvata in fisier cu fd=%d\n", fd);
   return 0;
}


BinMatrix* BinMat_load(int fd) {
   BinMatrix* bm = (BinMatrix*)malloc(sizeof(BinMatrix));
   ssize_t read_code = 0;

   // bm.file_identifier
   read_code = read(fd, &bm->file_identifier, sizeof(bm->file_identifier));
   if (read_code == -1) {
      fprintf(stderr, "Eroare citire identificator fisier");
      free(bm);
      return NULL;
   }
   // check id="bmat"
   if (bm->file_identifier[0] != 'b' 
      || bm->file_identifier[1] != 'm'
      || bm->file_identifier[2] != 'a'
      || bm->file_identifier[3] != 't') {
      fprintf(stderr, "Eroare: fisier invalid, id incorect");
      free(bm);
      return NULL;
   }

   // bm.rows
   read_code = read(fd, &bm->rows, sizeof(bm->rows));
   if (read_code == -1) {
      fprintf(stderr, "Eroare citire numar de linii");
      free(bm);
      return NULL;
   }

   // bm.cols
   read_code = read(fd, &bm->cols, sizeof(bm->cols));
   if (read_code == -1) {
      fprintf(stderr, "Eroare citire numar de coloane");
      free(bm);
      return NULL;
   }

   // bm.data
   if (bm->rows <= 0 || bm->cols <= 0) {
      fprintf(stderr, "Eroare: dimensiuni matrice invalide");
      free(bm);
      return NULL;
   }
   size_t alloc_count = bm->rows * bm->cols;
   bm->data = (float*)malloc(alloc_count * sizeof(float));
   read_code = read(fd, bm->data, alloc_count * sizeof(float));
   if (read_code == -1) {
      fprintf(stderr, "Eroare citire date matrice");
      free(bm->data);
      free(bm);
      return NULL;
   }

   printf("Matrice incarcata din fisier fd=%d\n", fd);
   return bm;
}

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