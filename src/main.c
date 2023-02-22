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
  printf("\x1b[2m" // Dim
         "[%s]"
         "\x1b[0m" // Reset
         " system call "
         "\x1b[34m" // Blue bold
         "%lld"
         "\x1b[0m" // Reset
         " called with { "
         "%%rdi: "
         "\x1b[32m" // Green
         "%#llx"
         "\x1b[0m" // Reset
         ", "
         "%%rsi: "
         "\x1b[32m" // Green
         "%#llx"
         "\x1b[0m" // Reset
         ", "
         "%%rdx: "
         "\x1b[32m" // Green
         "%#llx"
         "\x1b[0m" // Reset
         ", "
         "%%r10: "
         "\x1b[32m" // Green
         "%#llx"
         "\x1b[0m" // Reset
         ", "
         "%%r8: "
         "\x1b[32m" // Green
         "%#llx"
         "\x1b[0m" // Reset
         ", "
         "%%r9: "
         "\x1b[32m" // Green
         "%#llx"
         "\x1b[0m" // Reset
         " }.\n",
         timestamp, regs.orig_rax, regs.rdi, regs.rsi, regs.rdx, regs.r10,
         regs.r8, regs.r9);
}

int main(int argc, char *argv[]) {
  bool verbose = true;
  bool pause = false;

  pid_t pid = fork();

  switch (pid) {
  // On error
  case -1: {
    perror("fork");
    exit(1);
  }
  // On child process
  case 0: {
    ptrace(PTRACE_TRACEME);
    execvp(argv[1], argv + 1);
  }
  // On parent process
  default: {
    int status;
    bool in_call = false;
    int counter = 0;
    while (true) {
      wait(&status);
      while (WIFSTOPPED(status) && WSTOPSIG(status) == SIGTRAP) {
        struct user_regs_struct regs;
        ptrace(PTRACE_GETREGS, pid, NULL, &regs);
        if (!in_call) {
          if (verbose) {
            print_system_call(regs);
          }
          in_call = true;
          counter += 1;
        } else {
          in_call = false;
        }
        if (pause) {
          getchar_without_echo();
        }
        ptrace(PTRACE_SYSCALL, pid, NULL, 0);
        wait(&status);
      }
      if (WIFEXITED(status) || WIFSIGNALED(status)) {
        printf("\x1b[2m" // Dim
               "[RESULT]"
               "\x1b[0m" // Reset
               " The total number of system calls was "
               "\x1b[1m" // Bold
               "%d"
               "\x1b[0m" // Reset
               ".\n",
               counter);
        exit(0);
      }
    }
  }
  }
}
