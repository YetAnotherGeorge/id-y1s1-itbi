#include "bin_mat_cerinte.h"

int main(int argc, char *argv[]) {
   if (argc != 4) {
      char msg[100];
      snprintf(msg, sizeof(msg), "Usage: %s <file1> <file2> <file_OUT>", argv[0]);
      perror(msg);
      return -1;
   }

   const char *file1 = argv[1];
   const char *file2 = argv[2];
   const char *file_out = argv[3];

   int f1 = open(file1, O_RDONLY);
   if (f1 < 0) {
      char msg[100];
      snprintf(msg, sizeof(msg), "Eroare deschidere fisier '%s' pentru citire", file1);
      perror(msg);
      return -1;
   }

   int f2 = open(file2, O_RDONLY);
   if (f2 < 0) {
      char msg[100];
      snprintf(msg, sizeof(msg), "Eroare deschidere fisier '%s' pentru citire", file2);
      perror(msg);
      close(f1);
      return -1;
   }

   int f_out = open(file_out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
   if (f_out < 0) {
      char msg[100];
      snprintf(msg, sizeof(msg), "Eroare deschidere fisier '%s' pentru scriere", file_out);
      perror(msg);
      close(f1);
      close(f2);
      return -1;
   }

   printf("Calculare suma matrici din '%s' si '%s' in '%s'\n", file1, file2, file_out);
   sum(f1, f2, f_out);

   close(f1);
   close(f2);
   close(f_out);

   return 0;
}