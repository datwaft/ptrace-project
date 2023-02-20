FROM ubuntu:latest

RUN apt-get update -y && apt-get install -y clang make

COPY . /ptrace-project
WORKDIR /ptrace-project
RUN make clean
RUN make
ENTRYPOINT [ "./bin/main" ]
