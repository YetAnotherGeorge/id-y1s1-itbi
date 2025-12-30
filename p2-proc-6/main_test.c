#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>


int search_substring(const char* str_search, const char* txt) {
   size_t str_search_len = strlen(str_search);
   size_t txt_len = strlen(txt);

   if (str_search_len == 0 || txt_len == 0 || str_search_len > txt_len) {
      printf("search_substring: Warning - input parameters may be invalid: str_search_len=%zu, txt_len=%zu\n", str_search_len, txt_len);
      return 0;
   }

   int matches = 0;
   for (size_t i = 0; i <= (txt_len - str_search_len); i++) {
      printf("Checking position %zu:\n", i); 
      if (strncmp(&txt[i], str_search, str_search_len) == 0) {
         matches++;
      }
   }
   return matches;
}

void test() {
   const char* txt = "lorem ABC ipsum ABC dolor sit amet ABC";
   const char* search_term = "ABC";
   int count = search_substring(search_term, txt);

   printf("The term '%s' was found %d times in the text.\n", search_term, count);

}
