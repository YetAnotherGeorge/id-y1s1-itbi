#define _POSIX_C_SOURCE 200809L // scapa de erorile pentru sigaction, siginfo_t
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h> // pentru bool

pid_t SELF_PID = 0;
bool IS_MASTER = true;
char* LOG_PREFIX = NULL;
/**
 * @returns preffix of the form "[PID: xxxx] [MASTER/worker]"
 */
const char* get_log_prefix() {
   if (LOG_PREFIX != NULL) {
      return LOG_PREFIX;
   }

   static char prefix[50];
   pid_t pid_self = getpid();
   if (IS_MASTER) {
      snprintf(prefix, sizeof(prefix), "[PID: %d] [MASTER]", pid_self);
   } else {
      snprintf(prefix, sizeof(prefix), "[PID: %d] [worker]", pid_self);
   }
   LOG_PREFIX = prefix;
   return LOG_PREFIX;
}

/**
 * Cauta string incepand de la pozitia specificata (inclusiv)
 */
bool check_substring_at_pos(const char* str_search, const char* txt, size_t check_pos) {
   size_t str_search_len = strlen(str_search);
   size_t txt_len = strlen(txt);

   if (str_search_len == 0 || txt_len == 0 || str_search_len > txt_len || check_pos >= txt_len) {
      printf("%s check_substring_at_pos: Warning - input parameters may be invalid: str_search_len=%zu, txt_len=%zu, start_pos=%zu\n", get_log_prefix(), str_search_len, txt_len, check_pos);
      return 0;
   }

   if (strncmp(&txt[check_pos], str_search, str_search_len) == 0) {
      return true;
   } else {
      return false;
   }
}

// Array global pentru rezultate: [worker_id] = match_result (0/1)
int* WORKER_RESULTS = NULL;
/**
 * Doar procesul master va procesa semnale
 */
void master_sigusr1_handler(int signum, siginfo_t* info, void* context) {
   if (signum != SIGUSR1) {
      printf("%s master_sigusr1_handler: Warning - received unexpected signal: %d\n", get_log_prefix(), signum);
      return;
   }
   if (!IS_MASTER)
      return;
   int received_value = info->si_value.sival_int;
   //printf("%s received SIGUSR1 with value: %d from \n", get_log_prefix(), received_value);
   
   int id = received_value >> 16; // keep higher 16 bits as worker id
   int pos = received_value & 0xFFFF; // keep lower 16 bits as match result (0 = no match, 1 = match)
   if (WORKER_RESULTS == NULL) {
      fprintf(stderr, "%s WORKER_RESULTS is NULL in signal handler\n", get_log_prefix());
      return;
   }
   WORKER_RESULTS[id] = pos;
}
/**
 * 
 */
void main_worker(const char* arg_search, const char* arg_txt, size_t start_pos, pid_t master_pid) { 
   IS_MASTER = false;
   SELF_PID = getpid();
   LOG_PREFIX = NULL; // reset log prefix for worker
   printf("%s Worker started: idx: %zu ch: '%c'\n", get_log_prefix(), start_pos, arg_txt[start_pos]);

   size_t arg_search_len = strlen(arg_search);
   size_t arg_txt_len = strlen(arg_txt);

   bool found = check_substring_at_pos(arg_search, arg_txt, start_pos);
   int signal_value = (start_pos << 16) | (found ? 1 : 0); // encode start_pos and found result into one int
   if (found) {
      printf("%s found 1 match at position %zu\n", get_log_prefix(), start_pos);
   } 

   // Trimite semnal catre master
   union sigval value;
   value.sival_int = signal_value; // trimite pozitia gasita
   if (sigqueue(master_pid, SIGUSR1, value) == -1) {
      fprintf(stderr, "%s could not send SIGUSR1 to master\n", get_log_prefix());
   } else {
      //printf("%s sent SIGUSR1 to master with value %d\n", get_log_prefix(), value.sival_int);
   }
   // exit(EXIT_SUCCESS);
   exit(EXIT_SUCCESS); 
}  


int main(int argc, char *argv[]) {
   if (argc != 3) {
      printf("Usage: %s ARG_SEARCH ARG_TXT\n", argv[0]);
      return 1;
   }

   const char* arg_search = argv[1];
   const char* arg_txt = argv[2];

   SELF_PID = getpid();

   printf("%s start searching for '%s' in '%s'\n", get_log_prefix(), arg_search, arg_txt);

   // Inregistrare handler pentru SIGUSR1
   struct sigaction sa;
   sa.sa_flags = SA_SIGINFO;
   sa.sa_sigaction = master_sigusr1_handler;
   sigemptyset(&sa.sa_mask);

   // Atasare handler
   if (sigaction(SIGUSR1, &sa, NULL) == -1) {
      fprintf(stderr, "%s could not register SIGUSR1 handler\n", get_log_prefix());
      exit(EXIT_FAILURE);
   } else {
      printf("%s Master process registered SIGUSR1 handler successfully.\n", get_log_prefix());
   }
   
   // Start workers
   size_t arg_search_len = strlen(arg_search);
   size_t arg_txt_len = strlen(arg_txt);
   if (arg_search_len == 0 || arg_txt_len == 0 || arg_search_len > arg_txt_len) {
      printf("%s main_master: Warning - input parameters may be invalid: arg_search_len=%zu, arg_txt_len=%zu\n", get_log_prefix(), arg_search_len, arg_txt_len);
      return 1;
   }

   size_t str_search_iter = arg_txt_len - arg_search_len + 1;

   // reincearca search-ul pana cand acest array este complet populat; init to -1
   WORKER_RESULTS = (int*)malloc(str_search_iter * sizeof(int));
   for (size_t i = 0; i < str_search_iter; i++) {
      WORKER_RESULTS[i] = -1;
   }

   pid_t* worker_pids = malloc(str_search_iter * sizeof(pid_t));
   bool all_workers_done = false;
   // Reincearca pana cand toti workerii au terminat si au raportat rezultatele (signal loss protection)
   while (!all_workers_done) {
      // set workers to NULL
      for (size_t i = 0; i < str_search_iter; i++)
         worker_pids[i] = -1;

      // launch all that do not have a WORKER_RESULT yet
      for (size_t i = 0; i < str_search_iter; i++) {
         if (WORKER_RESULTS[i] != -1) 
            continue; // skip

         pid_t pid_worker = fork();
         if (pid_worker < 0) { // could not create worker
            fprintf(stderr, "%s Fork failed for worker at index %zu\n", get_log_prefix(), i);
            exit(EXIT_FAILURE);
         } else if (pid_worker == 0) { // worker created, inside worker process
            main_worker(arg_search, arg_txt, i, SELF_PID);
            return 0;
         }
   
         fprintf(stdout, "%s Created worker for start index %zu at ch '%c', PID: %d\n", get_log_prefix(), i, arg_txt[i], pid_worker);
         worker_pids[i] = pid_worker;
      }
      
      // Wait for all started workers to finish
      printf("%s Waiting for workers to finish...\n", get_log_prefix());
      for (size_t i = 0; i < str_search_iter; i++) {
         if (worker_pids[i] == -1)
            continue;
         
         int status;
         if (waitpid(worker_pids[i], &status, 0) == -1) {
            fprintf(stderr, "%s Error waiting for worker PID %d\n", get_log_prefix(), worker_pids[i]);
         } else {
            printf("%s Worker PID %d finished with status %d\n", get_log_prefix(), worker_pids[i], status);
         }
      }
      
      // Check if all workers have reported results
      int done_count = 0;
      for (size_t i = 0; i < str_search_iter; i++) {
         if (WORKER_RESULTS[i] != -1) {
            done_count++;
         } else {
            printf("%s >>>>>>>>>>>>>> Worker for index %zu has not reported result yet.\n", get_log_prefix(), i);
         }
      }
      if (done_count == str_search_iter) {
         all_workers_done = true;
      } else {
         printf("%s Not all workers done yet (%d/%zu). Retrying...\n", get_log_prefix(), done_count, str_search_iter);
      }
   }

   int total_matches = 0;
   for (size_t i = 0; i < str_search_iter; i++) {
      if (WORKER_RESULTS[i] == 1) {
         total_matches++;
      }
   }
   free(worker_pids);
   free(WORKER_RESULTS);
   printf("%s All workers have finished. Substring matches found: %d\n", get_log_prefix(), total_matches);
   return 0;
}

