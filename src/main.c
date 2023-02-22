#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define RESET "\x1b[0m"
#define BOLD "\x1b[1m"
#define DIM "\x1b[2m"
#define GREEN "\x1b[32m"
#define BLUE "\x1b[34m"

struct opts {
  bool verbose;
  bool pause;
};

// This snippet was inspired by https://stackoverflow.com/a/24830768/10702981
char getchar_without_echo(void) {
  struct termios old;
  tcgetattr(STDIN_FILENO, &old);
  struct termios new = old;
  new.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &new);
  char result = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &old);
  return result;
}

void print_system_call(struct user_regs_struct regs) {
  // Obtain timestamp
  struct timeval tv;
  gettimeofday(&tv, NULL);
  struct tm *tm = localtime(&tv.tv_sec);
  char timestamp[64];
  char milliseconds[8];
  strftime(timestamp, sizeof(timestamp), "%H:%M:%S", tm);
  sprintf(milliseconds, ".%06ld", tv.tv_usec);
  strcat(timestamp, milliseconds);
  // Print information regarding the system call
  fprintf(stderr,
          DIM "[%s]" RESET " "
              "system call " BLUE "%lld" RESET " called with { "
              "%%rdi: " GREEN "%#llx" RESET ", "
              "%%rsi: " GREEN "%#llx" RESET ", "
              "%%rdx: " GREEN "%#llx" RESET ", "
              "%%r10: " GREEN "%#llx" RESET ", "
              "%%r8: " GREEN "%#llx" RESET ", "
              "%%r9: " GREEN "%#llx" RESET " }.\n",
          timestamp, regs.orig_rax, regs.rdi, regs.rsi, regs.rdx, regs.r10,
          regs.r8, regs.r9);
}

int do_child(int argc, char *argv[]) {
  ptrace(PTRACE_TRACEME);
  return execvp(argv[0], argv);
}

int do_trace(pid_t child, struct opts options) {
  int counter = 0;
  int table[512] = {0};

  int status;
  waitpid(child, &status, 0);
  while (WIFSTOPPED(status) && WSTOPSIG(status) == SIGTRAP) {
    ptrace(PTRACE_SYSCALL, child, NULL, 0);
    waitpid(child, &status, 0);
    // While the system call is executing
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, child, NULL, &regs);
    // ---

    ptrace(PTRACE_SYSCALL, child, NULL, 0);
    waitpid(child, &status, 0);
    // After the system call finished executing
    if (options.verbose) {
      print_system_call(regs);
    }
    if (options.pause) {
      getchar_without_echo();
    }
    counter += 1;
    table[regs.orig_rax] += 1;
    // ---
  }

  for (size_t system_call = 0; system_call < 512; system_call++) {
    if (table[system_call] != 0) {
      fprintf(stderr,
              DIM "[RESULT]" RESET " system call " BLUE "%zu" RESET
                  " was called " BOLD "%d" RESET " times.\n",
              system_call, table[system_call]);
    }
  }
  fprintf(stderr,
          DIM "[RESULT]" RESET " The total number of system calls was " BOLD
              "%d" RESET ".\n",
          counter);

  return 0;
}

int main(int argc, char *argv[]) {
  struct opts options = {
      .verbose = true,
      .pause = false,
  };

  pid_t pid = fork();

  switch (pid) {
  // On error
  case -1:
    perror("fork");
    return 1;
  // On child process
  case 0:
    return do_child(argc - 1, argv + 1);
  // On parent process
  default:
    return do_trace(pid, options);
  }
}
