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

// CREATE / LOAD / SAVE / FREE / PRINT

/**
 * Aloca o matrice binara cu dimensiunile date.
 * Nu uita BinMat_free_ptr pentru eliberare!
 */
BinMatrix* BinMat_create(int rows, int cols) {
   BinMatrix* bm = (BinMatrix*)malloc(sizeof(BinMatrix));
   bm->file_identifier[0] = 'b';
   bm->file_identifier[1] = 'm';
   bm->file_identifier[2] = 'a';
   bm->file_identifier[3] = 't';
   bm->rows = rows;
   bm->cols = cols;

   void* alloc_data = malloc(rows * cols * sizeof(float));
   bm->data = (float*)alloc_data;

   for (int i = 0; i < rows * cols; i++) {
      bm->data[i] = 0.0f;
   }
   return bm;
}

/**
 * Salveaza matricea binara in fisierul deschis cu fd.
 * Seeks 
 * Returns 0 for success, -1 for error
 */
int BinMat_save(BinMatrix* bm, int fd) {
   ssize_t write_code = 0;

   // 1. bm.file_identifier
   void* bm_file_identifier = &bm->file_identifier;
   write_code = write(fd, bm_file_identifier, sizeof(bm->file_identifier));
   if (write_code == -1) {
      fprintf(stderr, "Eroare scriere identificator fisier");
      return -1;
   }

   // 2. bm.rows
   void* bm_rows = &bm->rows;
   write_code = write(fd, bm_rows, sizeof(bm->rows));
   if (write_code == -1) {
      fprintf(stderr, "Eroare scriere numar de linii");
      return -1;
   }

   // 3. bm.cols
   void* bm_cols = &bm->cols;
   write_code = write(fd, bm_cols, sizeof(bm->cols));
   if (write_code == -1) {
      fprintf(stderr, "Eroare scriere numar de coloane");
      return -1;
   }

   // 4. bm.data
   void* bm_data = bm->data;
   write_code = write(fd, bm_data, bm->rows * bm->cols * sizeof(float)); 
   if (write_code == -1) {
      fprintf(stderr, "Eroare scriere date matrice");
      return -1;
   }

   printf("Matrice salvata in fisier cu fd=%d\n", fd);
   return 0;
}
/**
 * Salveaza matricea binara in fisierul filename.
 * Returns 0 for success, -1 for error
 */
int BinMat_save_file(BinMatrix* bm, const char* filename) {
   int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
   if (fd == -1) {
      fprintf(stderr, "Eroare deschidere fisier %s\n", filename);
      return -1;
   }
   int save_code = BinMat_save(bm, fd);
   close(fd);
   return save_code;
}

/**
 * Loads a BinMatrix from an open file descriptor
 * Returns NULL on error
 */
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

   printf("BinMat_load(int fd): loaded fd=%d\n", fd);
   return bm;
}
/**
 * Loads a BinMatrix from an open file descriptor
 * Returns NULL on error
 */
BinMatrix* BinMat_load_file(const char* filename) {
   int fd = open(filename, O_RDONLY);
   if (fd == -1) {
      fprintf(stderr, "Eroare deschidere fisier %s\n", filename);
      return NULL;
   }
   BinMatrix* bm = BinMat_load(fd);
   printf("Matrice incarcata din fisier \"%s\" fd=%d\n", filename, fd);
   close(fd);
   return bm;
}

/**
 * Elibereaza memoria alocata pentru matricea binara (pointer version)
 */
void BinMat_free_ptr(BinMatrix* bm) {
   free(bm->data);
   free(bm);
}

/**
 * Print to std::out
 */
void BinMat_print(BinMatrix* bm) {
   printf("BinMat: rows=%d, cols=%d:\n", bm->rows, bm->cols);
   for (int i = 0; i < bm->rows; i++) {
      for (int j = 0; j < bm->cols; j++) {
         printf("   %.2f ", bm->data[i * bm->cols + j]);
      }
      printf("\n");
   }
   printf("In Wolfram Alpha Layout: ");
   printf("{");
   for (int i = 0; i < bm->rows; i++) {
      printf("{");
      for (int j = 0; j < bm->cols; j++) {
         printf("%.2f", bm->data[i * bm->cols + j]);
         if (j < bm->cols - 1)
            printf(", ");
      }
      printf("}");
      if (i < bm->rows - 1)
         printf(", ");
   }
   printf("}\n");
   printf("\n");
}
// SET / GET

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

// UTILS
/**
 * Fill matrix with sequential values starting from 0
 */
void BinMat_fill_sequential(BinMatrix* bm) {
   printf("Filling matrix with sequential values\n");
   for (int i = 0, counter=0; i < bm->rows; i++) {
      for (int j = 0; j < bm->cols; j++) {
         float value = (float)counter++;
         BinMat_set(bm, i, j, value);
      }
   }
}
/**
 * Fill matrix with constant value v
 */
void BinMat_fill_constant(BinMatrix* bm, float v) {
   size_t elem_count = bm->rows * bm->cols;
   for (size_t i = 0; i < elem_count; i++)
      bm->data[i] = v;
}
/**
 * Fill matrix with random values in [min, max]
 */
void BinMat_fill_random(BinMatrix* bm, float min, float max) {
   size_t elem_count = bm->rows * bm->cols;
   for (size_t i = 0; i < elem_count; i++) {
      float scale = rand() / (float)RAND_MAX; // [0, 1.0]
      bm->data[i] = min + scale * (max - min);
   }
}
/**
 * Set all diagonal elements to value, the rest remain unchanged
 */
void BinMat_set_diagonal(BinMatrix* bm, float value) {
   for (int i = 0; i < bm->rows && i < bm->cols; i++) {
      bm->data[i * bm->cols + i] = value;
   }
}

/**
 * Returns a new matrix which is the sum of a and b
 * Returns NULL on error
 */
BinMatrix* BinMat_sum(BinMatrix* a, BinMatrix* b) {
   if (a->rows != b->rows || a->cols != b->cols) {
      fprintf(stderr, "Dimensiuni matrice incompatibile pentru suma\n");
      return NULL;
   }
   BinMatrix* result = BinMat_create(a->rows, a->cols);
   size_t elem_count = a->rows * a->cols;
   for (size_t i = 0; i < elem_count; i++) {
      result->data[i] = a->data[i] + b->data[i];
   }
   return result;
}
/**
 * Returns a new matrix which is the product of a and b
 * Returns NULL on error
 */
BinMatrix* BinMat_prod(BinMatrix* a, BinMatrix* b) {
   // m * n  x  n * p  =  m * p
   if (a->cols != b->rows) {
      fprintf(stderr, "Dimensiuni matrice incompatibile pentru produs\n");
      return NULL;
   }
   printf("Calcul produs matrice %dx%d x %dx%d\n", a->rows, a->cols, b->rows, b->cols);
   BinMatrix* p = BinMat_create(a->rows, b->cols);
   for (int i = 0; i < p->rows; i++) {
      for (int j = 0; j < p->cols; j++) {

         float sum = 0.0f;
         for (int k = 0; k < a->cols; k++) {
            float a_val = BinMat_get(a, i, k);
            float b_val = BinMat_get(b, k, j);
            sum += a_val * b_val;
         }
         BinMat_set(p, i, j, sum);
      }
   }
  
   return p;
}