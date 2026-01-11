#include "bin_mat.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
 * @brief Writes data to a file descriptor, with error checking.
 * @param fd The file descriptor to write to.
 * @param buf The buffer containing the data to write.
 * @param count The number of bytes to write.
 * @param msg The error message to display with perror on failure.
 */
void checked_write(int fd, const void *buf, size_t count, const char *msg) {
   if (write(fd, buf, count) != count) {
      perror(msg);
      exit(EXIT_FAILURE);
   }
}
/**
 * @brief Reads data from a file descriptor, with error checking.
 * @param fd The file descriptor to read from.
 * @param buf The buffer to store the read data into.
 * @param count The number of bytes to read.
 * @param msg The error message to display with perror on failure.
 */
void checked_read(int fd, void *buf, size_t count, const char *msg) {
   if (read(fd, buf, count) != count) {
      perror(msg);
      exit(EXIT_FAILURE);
   }
}


/// @brief creaza in fisierul f o matrice m x n initializata cu 0. Apelantul trebuie sa inchida fisierul
/// @param f file descriptor
/// @param m rows
/// @param n cols
void new(int f, int m, int n) {
   if (lseek(f, 0, SEEK_SET) == -1) {
      perror("lseek failed");
      exit(EXIT_FAILURE);
   }

   char id[4] = {'b', 'm', 'a', 't'};
   checked_write(f, id, sizeof(id), "write failed for id");
   checked_write(f, &m, sizeof(m), "write failed for m");
   checked_write(f, &n, sizeof(n), "write failed for n");
   
   float zero = 0.0f;
   for (int i = 0; i < m * n; i++) {
      checked_write(f, &zero, sizeof(float), "write failed for zero");
   }
}

/// @brief Muta 'cursorul' in fisier la pozitia (i, j) si returneaza valoarea. 
/// @param f File descriptor
/// @param i row_idx
/// @param j col_idx
/// @return Returneaza NAN in caz de erorare
float get(int f, int i, int j) {
   if (lseek(f, 0, SEEK_SET) == -1) {
      perror("lseek failed");
      exit(EXIT_FAILURE);
   }
   // Citeste header
   char id[4];
   checked_read(f, id, sizeof(id), "read failed for id");
   if (id[0] != 'b' || id[1] != 'm' || id[2] != 'a' || id[3] != 't') {
      perror("Eroare: fisier invalid, id incorect");
      return NAN;
   }

   // Citeste dimensiuni
   int rows, cols;
   checked_read(f, &rows, sizeof(rows), "read failed for rows");
   checked_read(f, &cols, sizeof(cols), "read failed for cols");
   if (i < 0 || i >= rows || j < 0 || j >= cols) {
      char msg[100];
      snprintf(msg, sizeof(msg), "Eroare: index (%d,%d) out of bounds (%d,%d)", i, j, rows, cols);
      perror(msg);
      return NAN;
   }
   
   size_t offset = (i * cols + j) * sizeof(float);
   if (lseek(f, offset, SEEK_CUR) == -1) {
      perror("lseek failed");
      exit(EXIT_FAILURE);
   }
   
   float value;
   checked_read(f, &value, sizeof(value), "read failed for value");
   return value;
}

/// @brief 
/// @param f 
/// @param i 
/// @param j 
/// @param x 
void set(int f, int i, int j, float x) {
   if (lseek(f, 0, SEEK_SET) == -1) {
      perror("lseek failed");
      exit(EXIT_FAILURE);
   }
   // Citeste header
   char id[4];
   checked_read(f, id, sizeof(id), "read failed for id");
   if (id[0] != 'b' || id[1] != 'm' || id[2] != 'a' || id[3] != 't') {
      perror("Eroare: fisier invalid, id incorect");
      return;
   }

   // Citeste dimensiuni
   int rows, cols;
   checked_read(f, &rows, sizeof(rows), "read failed for rows");
   checked_read(f, &cols, sizeof(cols), "read failed for cols");
   if (i < 0 || i >= rows || j < 0 || j >= cols) {
      char msg[100];
      snprintf(msg, sizeof(msg), "Eroare: index (%d,%d) out of bounds (%d,%d)", i, j, rows, cols);
      perror(msg);
      return;
   }
   
   // Muta la pozitia (i, j)
   size_t offset = (i * cols + j) * sizeof(float);
   if (lseek(f, offset, SEEK_CUR) == -1) {
      perror("lseek failed");
      exit(EXIT_FAILURE);
   }
   checked_write(f, &x, sizeof(x), "write failed for x");
}

/// @brief 
/// @param f1 fisier matrice 1
/// @param f2 fisier matrice 2
/// @param f fisier matrice suma
void sum(int f1, int f2, int f) {
   if (lseek(f1, 0, SEEK_SET) == -1 || lseek(f2, 0, SEEK_SET) == -1) {
      perror("lseek failed");
      exit(EXIT_FAILURE);
   }
   // Citeste tip fisere
   char id[4];
   checked_read(f1, id, sizeof(id), "read failed for id f1");
   if (id[0] != 'b' || id[1] != 'm' || id[2] != 'a' || id[3] != 't') {
      perror("Eroare: fisier f1 invalid, id incorect");
      exit(EXIT_FAILURE);
   }
   checked_read(f2, id, sizeof(id), "read failed for id f2");
   if (id[0] != 'b' || id[1] != 'm' || id[2] != 'a' || id[3] != 't') {
      perror("Eroare: fisier f2 invalid, id incorect");
      exit(EXIT_FAILURE);
   }
   // Citeste header matrice 1
   int rows1, cols1;
   checked_read(f1, &rows1, sizeof(rows1), "read failed for rows1");
   checked_read(f1, &cols1, sizeof(cols1), "read failed for cols1");
   // Citeste header matrice 2
   int rows2, cols2;
   checked_read(f2, &rows2, sizeof(rows2), "read failed for rows2");
   checked_read(f2, &cols2, sizeof(cols2), "read failed for cols2");

   if (rows1 != rows2 || cols1 != cols2) {
      perror("Eroare: matrici incompatibile pentru suma");
      exit(EXIT_FAILURE);
   }

   // Calculeaza suma - pregateste fisierul de output
   new(f, rows1, cols1);
   lseek(f, sizeof(char) * 4 + sizeof(int) * 2, SEEK_SET); // sari peste header 
   // f1 si f2 sunt deja pozitionate corect la inceputul array-ului

   for (int i = 0; i < rows1; i++) {
      for (int j = 0; j < cols1; j++) {
         float val1, val2;
         checked_read(f1, &val1, sizeof(val1), "read failed for val1");
         checked_read(f2, &val2, sizeof(val2), "read failed for val2");
         float sum = val1 + val2;
         checked_write(f, &sum, sizeof(sum), "write failed for sum");
      }
   }
}

/// @brief 
/// @param f1 fisier matrice 1
/// @param f2 fisier matrice 2
/// @param f fisier matrice produs
void pro(int f1, int f2, int f) {
   if (lseek(f1, 0, SEEK_SET) == -1 || lseek(f2, 0, SEEK_SET) == -1) {
      perror("lseek failed");
      exit(EXIT_FAILURE);
   }
   // Citeste tip fisere
   char id[4];
   checked_read(f1, id, sizeof(id), "read failed for id f1");
   if (id[0] != 'b' || id[1] != 'm' || id[2] != 'a' || id[3] != 't') {
      perror("Eroare: fisier f1 invalid, id incorect");
      exit(EXIT_FAILURE);
   }
   checked_read(f2, id, sizeof(id), "read failed for id f2");
   if (id[0] != 'b' || id[1] != 'm' || id[2] != 'a' || id[3] != 't') {
      perror("Eroare: fisier f2 invalid, id incorect");
      exit(EXIT_FAILURE);
   }
   // Citeste header matrice 1
   int rows1, cols1;
   checked_read(f1, &rows1, sizeof(rows1), "read failed for rows1");
   checked_read(f1, &cols1, sizeof(cols1), "read failed for cols1");
   // Citeste header matrice 2
   int rows2, cols2;
   checked_read(f2, &rows2, sizeof(rows2), "read failed for rows2");
   checked_read(f2, &cols2, sizeof(cols2), "read failed for cols2");

   if (cols1 != rows2) {
      perror("Eroare: matrici incompatibile pentru suma");
      exit(EXIT_FAILURE);
   }

   // Calculeaza produs - pregateste fisierul de output
   new(f, rows1, cols2);
   lseek(f, sizeof(char) * 4 + sizeof(int) * 2, SEEK_SET); // sari peste header 
  
   
   for (int i = 0; i < rows1; i++) {
      for (int j = 0; j < cols2; j++) {
         
         float sum = 0;
         for (int k = 0; k < cols1; k++) {
            float v1, v2;
            // mat1[i][k] x mat2[k][j]
            size_t offset1 = sizeof(char) * 4 + sizeof(int) * 2 + (i * cols1 + k) * sizeof(float);
            if (lseek(f1, offset1, SEEK_SET) == -1) {
               perror("lseek failed");
               exit(EXIT_FAILURE);
            }
            checked_read(f1, &v1, sizeof(v1), "read failed for v1");

            size_t offset2 = sizeof(char) * 4 + sizeof(int) * 2 + (k * cols2 + j) * sizeof(float);
            if (lseek(f2, offset2, SEEK_SET) == -1) {
               perror("lseek failed");
               exit(EXIT_FAILURE);
            }
            checked_read(f2, &v2, sizeof(v2), "read failed for v2");

            sum += v1 * v2;
         }

         checked_write(f, &sum, sizeof(sum), "write failed for sum");
      }
   }
}