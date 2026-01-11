#include "bin_mat.h"

/// @brief creaza in fisierul f o matrice m x n initializata cu 0. Apelantul trebuie sa inchida fisierul
/// @param f file descriptor
/// @param m rows
/// @param n cols
void new(int f, int m, int n) {
   fseek(f, 0, SEEK_SET); // incepem de la inceputul fisierului

   char id[4] = {'b', 'm', 'a', 't'};
   write(f, id, sizeof(id));
   write(f, &m, sizeof(m));
   write(f, &n, sizeof(n));
   
   float zero = 0.0f;
   for (int i = 0; i < m * n; i++) {
      write(f, &zero, sizeof(float));
   }
}

/// @brief Muta 'cursorul' in fisier la pozitia (i, j) si returneaza valoarea. 
/// @param f File descriptor
/// @param i row_idx
/// @param j col_idx
/// @return Returneaza NAN in caz de erorare
float get(int f, int i, int j) {
   fseek(f, 0, SEEK_SET); // incepem de la inceputul fisierului
   // Citeste header
   char id[4];
   read(f, id, sizeof(id));
   if (id[0] != 'b' || id[1] != 'm' || id[2] != 'a' || id[3] != 't') {
      fprintf(stderr, "Eroare: fisier invalid, id incorect\n");
      return NAN;
   }

   // Citeste dimensiuni
   int rows, cols;
   read(f, &rows, sizeof(rows));
   read(f, &cols, sizeof(cols));
   if (i < 0 || i >= rows || j < 0 || j >= cols) {
      fprintf(stderr, "Eroare: index (%d,%d) out of bounds (%d,%d)\n", i, j, rows, cols);
      return NAN;
   }
   
   size_t offset = (i * cols + j) * sizeof(float);
   fseek(f, offset, SEEK_CUR);
   
   float value;
   read(f, &value, sizeof(value));
   return value;
}

/// @brief 
/// @param f 
/// @param i 
/// @param j 
/// @param x 
void set(int f, int i, int j, float x) {
   fseek(f, 0, SEEK_SET); // incepem de la inceputul fisierului
   // Citeste header
   char id[4];
   read(f, id, sizeof(id));
   if (id[0] != 'b' || id[1] != 'm' || id[2] != 'a' || id[3] != 't') {
      fprintf(stderr, "Eroare: fisier invalid, id incorect\n");
   }

   // Citeste dimensiuni
   int rows, cols;
   read(f, &rows, sizeof(rows));
   read(f, &cols, sizeof(cols));
   if (i < 0 || i >= rows || j < 0 || j >= cols) {
      fprintf(stderr, "Eroare: index (%d,%d) out of bounds (%d,%d)\n", i, j, rows, cols);
      return NAN;
   }
   
   // Muta la pozitia (i, j)
   size_t offset = (i * cols + j) * sizeof(float);
   fseek(f, offset, SEEK_CUR);
   write(f, &x, sizeof(x));
}