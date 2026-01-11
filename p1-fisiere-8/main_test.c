#include "bin_mat_cerinte.h"

int test_1() {
   int fd = open("./files/test_mat_5x6.bmat", O_TRUNC | O_CREAT | O_RDWR, 0644);
   if (fd == -1) {
      perror("Eroare creare/deschidere fisier pentru test");
      return -1;
   }

   // create and fill out with sequential values
   new(fd, 5, 6);

   int v = 0;
   for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 6; j++) {
         float val = (float)v++;
         set(fd, i, j, val);
      }
   }
   // Check
   v = 0;
   for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 6; j++) {
         float expected = (float)v++;
         float got = get(fd, i, j);
         if (got != expected) {
            char msg[200];
            snprintf(msg, sizeof(msg), "Test failed at (%d,%d): expected %.2f, got %.2f", i, j, expected, got);
            perror(msg);
            close(fd);
            return -1;
         }
      }
   }
   printf("Test 1 passed: all values match expected results.\n");

   if (close(fd) == -1) {
      perror("Eroare inchidere fisier dupa creare");
      return -1;
   }
   return 0;
}

int test_2() {
   int fd1 = open("./files/matrix_3x3_1.bmat", O_RDONLY);
   if (fd1 == -1) {
      perror("Eroare deschidere fisier matrix_3x3_1.bmat");
      return -1;
   }
   int fd2 = open("./files/matrix_3x3_2.bmat", O_RDONLY);
   if (fd2 == -1) {
      perror("Eroare deschidere fisier matrix_3x3_2.bmat");
      close(fd1);
      return -1;
   }

   int fd_out = open("./files/matrix_3x3_sum.bmat", O_TRUNC | O_CREAT | O_RDWR, 0644);
   if (fd_out == -1) {
      perror("Eroare creare/deschidere fisier matrix_3x3_sum.bmat");
      close(fd1); close(fd2);
      return -1;
   }

   sum(fd1, fd2, fd_out);

   close(fd1); close(fd2); close(fd_out);
   printf("Test 2 passed: sum completed without errors.\n");
   return 0;
}

int main(int argc, char *argv[]) {
   return test_2();
}