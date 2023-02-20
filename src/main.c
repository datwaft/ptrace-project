#include <stdio.h>      // For printf
#include <stdlib.h>     // For wait(), WIFSTOPPED(), WSTOPSIG(), WIFEXITED(),
                        //   WIFSIGNALED(), SIGTRAP, exit()
#include <mach/mach.h>  // For task_for_pid() and mach_task_self()
#include <sys/ptrace.h> // For ptrace()
#include <sys/types.h>  // For ptrace()
#include <sys/wait.h>   // For wait() in Linux
#include <unistd.h>     // For fork(), excl(), execvp()

int main(int argc, char *argv[]) {
  pid_t pid = fork();

  switch (pid) {
  // On error
  case -1: {
    perror("fork");
    exit(1);
  }
  // On child process
  case 0: {
    ptrace(PT_TRACE_ME, 0, NULL, 0);
    execvp(argv[1], argv + 1);
  }
  // On parent process
  default: {
    int status;
    while (1) {
      wait(&status);
      if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGTRAP) {
        printf("The child has executed a system call.\n");
        ptrace(PT_CONTINUE, pid, (caddr_t)1, 0);
      }
      if (WIFEXITED(status) || WIFSIGNALED(status)) {
        exit(0);
      }
    }
  }
  }
}
