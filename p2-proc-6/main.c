#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <stdbool.h> // pentru bool

/**
 * Cauta string incepand de la pozitia specificata (inclusiv)
 */
bool check_substring_at_pos(const char* str_search, const char* txt, size_t check_pos) {
   size_t str_search_len = strlen(str_search);
   size_t txt_len = strlen(txt);

   if (str_search_len == 0 || txt_len == 0 || str_search_len > txt_len || check_pos >= txt_len) {
      printf("check_substring_at_pos: Warning - input parameters may be invalid: str_search_len=%zu, txt_len=%zu, start_pos=%zu\n", str_search_len, txt_len, check_pos);
      return 0;
   }

   if (strncmp(&txt[check_pos], str_search, str_search_len) == 0) {
      return true;
   } else {
      return false;
   }
}

void main_master(const char* self_name, const char* arg_search, const char* arg_txt) { 
   size_t arg_search_len = strlen(arg_search);
   size_t arg_txt_len = strlen(arg_txt);

}

int main(int argc, char *argv[]) {
   if (argc != 3) {
      printf("Usage: %s ARG_SEARCH ARG_TXT\n", argv[0]);
      return 1;
   }

   const char* arg_search = argv[1];
   const char* arg_txt = argv[2];

   bool master_mode; // master mode check     
   int search_idx;   // master mode check  
   if (argc == 4) {
      master_mode = false;
      search_idx = atoi(argv[3]);
   } else {
      master_mode = true;
      search_idx = 0;
   }

   pid_t pid_self = getpid();
   if (master_mode) {
      printf("Self Process ID: %d [MASTER] ['%s']\n", pid_self, argv[0]);
   } else {
      printf("Self Process ID: %d [WORKER for index %d]\n", pid_self, search_idx);
   }
   
   if (master_mode) {
      main_master(argv[0], arg_search, arg_txt);
   } else {
      
   }

   return 0;
}
