#include <stdio.h>      // For printf
#include <stdlib.h>     // For wait(), WIFSTOPPED(), WSTOPSIG(), WIFEXITED(),
                        //   WIFSIGNALED(), SIGTRAP, exit()
#include <sys/ptrace.h> // For ptrace()
#include <unistd.h>     // For fork(), excl()

int main(void) {
  pid_t child = fork();
  if (child == 0) {
    ptrace(PT_TRACE_ME, 0, NULL, 0);
    execl("/bin/ls", "ls", NULL);
  } else {
    int status;
    while (1) {
      wait(&status);
      if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGTRAP) {
        printf("The child has executed a system call.\n");
      }
      if (WIFEXITED(status) || WIFSIGNALED(status)) {
        printf("The child has exited or terminated.\n");
        break;
      }
      ptrace(PT_CONTINUE, child, (caddr_t)1, 0);
    }
  }

  exit(0);
}
