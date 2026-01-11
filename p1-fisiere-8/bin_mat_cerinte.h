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