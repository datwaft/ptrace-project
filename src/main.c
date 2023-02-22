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
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  char timestamp[64];
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%S", tm);
  // Print information regarding the system call
  printf(DIM "[%s]" RESET " system call " BLUE "%lld" RESET " called with { "
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
  bool in_call = false;
  int counter = 0;

  int status;
  waitpid(child, &status, 0);
  while (WIFSTOPPED(status) && WSTOPSIG(status) == SIGTRAP) {
    if (!in_call) { // While the system call is executing
      struct user_regs_struct regs;
      ptrace(PTRACE_GETREGS, child, NULL, &regs);
      if (options.verbose) {
        print_system_call(regs);
      }

      in_call = true;
      counter += 1;

      if (options.pause) {
        getchar_without_echo();
      }
    } else { // After the system call has finished
      in_call = false;
    }
    ptrace(PTRACE_SYSCALL, child, NULL, 0);
    waitpid(child, &status, 0);
  }
  printf(DIM "[RESULT]" RESET " The total number of system calls was " BOLD
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
