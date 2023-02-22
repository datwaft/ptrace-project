#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

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
    while (1) {
      wait(&status);
      if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGTRAP) {
        // Obtain the registers
        struct user_regs_struct regs;
        ptrace(PTRACE_GETREGS, pid, NULL, &regs);
        if (verbose) {
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
                 "\x1b[1;32m" // Green bold
                 "%llu"
                 "\x1b[0m" // Reset
                 " called with { "
                 "%%rdi: %llu, "
                 "%%rsi: %llu, "
                 "%%rdx: %llu, "
                 "%%r10: %llu, "
                 "%%r8: %llu, "
                 "%%r9: %llu "
                 "}.\n",
                 timestamp, regs.orig_rax, regs.rdi, regs.rsi, regs.rdx,
                 regs.r10, regs.r8, regs.r9);
        }
        // Continue with the child process
        if (pause) {
          getchar();
        }
        ptrace(PTRACE_CONT, pid, NULL, 0);
      }
      if (WIFEXITED(status) || WIFSIGNALED(status)) {
        exit(0);
      }
    }
  }
  }
}
