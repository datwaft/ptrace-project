# S.O.A - Tarea 1
Rastreador de System Calls.

Elaborado por:
- Abreu Cardenas, Miguel Guillermo
- Chaves Salas, Bryan Miguel
- Guevara Sanchez, David Alberto
- Ortiz Ruiz, Victor Adolfo
- Yip Chen, Michael

## Compilar
Compilar código fuente
```sh
$ cd Abreu-Chaves-Guevara-Ortiz-Yip
```
```sh
$ make
```

Limpiar
```sh
$ make clean
```

## Rastreador
Sintaxis
```sh
$ rastreador [opciones rastreador] Prog [opciones de Prog]
```
`[opciones rastreador]`:

- `-v`: desplegará un mensaje cada vez que detecte un System Call de `Prog`.

- `-V`: desplegará un mensaje cada vez que detecte un System Call de `Prog`, pero hará una pausa hasta que el usuario presione cualquier tecla para continuar la ejecución de `Prog`.

## Ejemplos
Se utilizó como ejemplo de `Prog` el programa `free` con su opción `-m`


rastreador
```sh
$ ./rastreador free -m
```
```
               total        used        free      shared  buff/cache   available
Mem:            7853        3346        1263         734        3244        3481
Swap:           2047         395        1652
[RESULT] SYSTEM CALL	NUMBER OF CALLS
[RESULT] -----------	---------------
[RESULT]           0	             17
[RESULT]           1	              3
[RESULT]           3	             19
[RESULT]           8	              1
[RESULT]           9	             46
[RESULT]          10	             14
[RESULT]          11	              1
[RESULT]          12	              3
[RESULT]          17	              4
[RESULT]          21	              1
[RESULT]         157	              6
[RESULT]         158	              2
[RESULT]         218	              1
[RESULT]         231	              1
[RESULT]         257	             30
[RESULT]         262	             17
[RESULT]         273	              1
[RESULT]         302	              1
[RESULT]         318	              1
[RESULT]         334	              1
[RESULT] -----------	---------------
[RESULT]       TOTAL	            170

```


rastreador `-v`
```sh
$ ./rastreador -v free -m
```

```log
[22:27:15.528244] system call 12 called with { %rdi: 0, %rsi: 0x7fffee08ccdc, %rdx: 0, %r10: 0, %r8: 0, %r9: 0x23 }.
[22:27:15.528461] system call 158 called with { %rdi: 0x3001, %rsi: 0x7fffee08cc90, %rdx: 0x7f286f8503e0, %r10: 0x1, %r8: 0x3, %r9: 0x800 }.
[22:27:15.528565] system call 9 called with { %rdi: 0, %rsi: 0x2000, %rdx: 0x3, %r10: 0x22, %r8: 0xffffffff, %r9: 0 }.
[22:27:15.528652] system call 21 called with { %rdi: 0x7f286f860d90, %rsi: 0x4, %rdx: 0x556cb2c87cf0, %r10: 0x22, %r8: 0, %r9: 0 }.
[22:27:15.528739] system call 257 called with { %rdi: 0xffffff9c, %rsi: 0x7f286f85f21b, %rdx: 0x80000, %r10: 0, %r8: 0x80000, %r9: 0x7f286f85f21b }.
[22:27:15.528814] system call 262 called with { %rdi: 0x3, %rsi: 0x7f286f85fee9, %rdx: 0x7fffee08bde0, %r10: 0x1000, %r8: 0x80000, %r9: 0x7f286f85f21b }.

...
...
...


[22:27:15.537166] system call 262 called with { %rdi: 0x1, %rsi: 0x7f286f5d846f, %rdx: 0x7fffee08c530, %r10: 0x1000, %r8: 0xa0100000000, %r9: 0 }.
               total        used        free      shared  buff/cache   available
[22:27:15.537202] system call 1 called with { %rdi: 0x1, %rsi: 0x556cb4b0c4c0, %rdx: 0x51, %r10: 0x1000, %r8: 0x7f286f61ba70, %r9: 0 }.
Mem:            7853        3272        1364         702        3217        3586
[22:27:15.537248] system call 1 called with { %rdi: 0x1, %rsi: 0x556cb4b0c4c0, %rdx: 0x51, %r10: 0, %r8: 0x7f286f61ba70, %r9: 0x7fffffff }.
Swap:           2047         395        1652
[22:27:15.537286] system call 1 called with { %rdi: 0x1, %rsi: 0x556cb4b0c4c0, %rdx: 0x2d, %r10: 0, %r8: 0x7f286f61ba70, %r9: 0x7fffffff }.
[22:27:15.537321] system call 3 called with { %rdi: 0x1, %rsi: 0x556cb4b0c4c0, %rdx: 0x7f286f615a00, %r10: 0, %r8: 0x7f286f61ba70, %r9: 0x7fffffff }.
[22:27:15.537355] system call 3 called with { %rdi: 0x2, %rsi: 0xfbad2006, %rdx: 0x7f286f615a00, %r10: 0, %r8: 0x556cb4b0c4c0, %r9: 0x7fffffff }.
[22:27:15.537617] system call 231 called with { %rdi: 0, %rsi: 0xe7, %rdx: 0x3c, %r10: 0, %r8: 0xffffffffffffff80, %r9: 0x60 }.
[RESULT] SYSTEM CALL	NUMBER OF CALLS
[RESULT] -----------	---------------
[RESULT]           0	             17
[RESULT]           1	              3
[RESULT]           3	             19
[RESULT]           8	              1
[RESULT]           9	             46
[RESULT]          10	             14
[RESULT]          11	              1
[RESULT]          12	              3
[RESULT]          17	              4
[RESULT]          21	              1
[RESULT]         157	              6
[RESULT]         158	              2
[RESULT]         218	              1
[RESULT]         231	              1
[RESULT]         257	             30
[RESULT]         262	             17
[RESULT]         273	              1
[RESULT]         302	              1
[RESULT]         318	              1
[RESULT]         334	              1
[RESULT] -----------	---------------
[RESULT]       TOTAL	            170
```


rastreador `-V`
```sh
$ ./rastreador -V free -m
```
```log
[22:27:15.528244] system call 12 called with { %rdi: 0, %rsi: 0x7fffee08ccdc, %rdx: 0, %r10: 0, %r8: 0, %r9: 0x23 }.
[22:27:15.528461] system call 158 called with { %rdi: 0x3001, %rsi: 0x7fffee08cc90, %rdx: 0x7f286f8503e0, %r10: 0x1, %r8: 0x3, %r9: 0x800 }.
[22:27:15.528565] system call 9 called with { %rdi: 0, %rsi: 0x2000, %rdx: 0x3, %r10: 0x22, %r8: 0xffffffff, %r9: 0 }.
[22:27:15.528652] system call 21 called with { %rdi: 0x7f286f860d90, %rsi: 0x4, %rdx: 0x556cb2c87cf0, %r10: 0x22, %r8: 0, %r9: 0 }.
[22:27:15.528739] system call 257 called with { %rdi: 0xffffff9c, %rsi: 0x7f286f85f21b, %rdx: 0x80000, %r10: 0, %r8: 0x80000, %r9: 0x7f286f85f21b }.
[22:27:15.528814] system call 262 called with { %rdi: 0x3, %rsi: 0x7f286f85fee9, %rdx: 0x7fffee08bde0, %r10: 0x1000, %r8: 0x80000, %r9: 0x7f286f85f21b }.

...
...
...


[22:27:15.537166] system call 262 called with { %rdi: 0x1, %rsi: 0x7f286f5d846f, %rdx: 0x7fffee08c530, %r10: 0x1000, %r8: 0xa0100000000, %r9: 0 }.
               total        used        free      shared  buff/cache   available
[22:27:15.537202] system call 1 called with { %rdi: 0x1, %rsi: 0x556cb4b0c4c0, %rdx: 0x51, %r10: 0x1000, %r8: 0x7f286f61ba70, %r9: 0 }.
Mem:            7853        3272        1364         702        3217        3586
[22:27:15.537248] system call 1 called with { %rdi: 0x1, %rsi: 0x556cb4b0c4c0, %rdx: 0x51, %r10: 0, %r8: 0x7f286f61ba70, %r9: 0x7fffffff }.
Swap:           2047         395        1652
[22:27:15.537286] system call 1 called with { %rdi: 0x1, %rsi: 0x556cb4b0c4c0, %rdx: 0x2d, %r10: 0, %r8: 0x7f286f61ba70, %r9: 0x7fffffff }.
[22:27:15.537321] system call 3 called with { %rdi: 0x1, %rsi: 0x556cb4b0c4c0, %rdx: 0x7f286f615a00, %r10: 0, %r8: 0x7f286f61ba70, %r9: 0x7fffffff }.
[22:27:15.537355] system call 3 called with { %rdi: 0x2, %rsi: 0xfbad2006, %rdx: 0x7f286f615a00, %r10: 0, %r8: 0x556cb4b0c4c0, %r9: 0x7fffffff }.
[22:27:15.537617] system call 231 called with { %rdi: 0, %rsi: 0xe7, %rdx: 0x3c, %r10: 0, %r8: 0xffffffffffffff80, %r9: 0x60 }.
[RESULT] SYSTEM CALL	NUMBER OF CALLS
[RESULT] -----------	---------------
[RESULT]           0	             17
[RESULT]           1	              3
[RESULT]           3	             19
[RESULT]           8	              1
[RESULT]           9	             46
[RESULT]          10	             14
[RESULT]          11	              1
[RESULT]          12	              3
[RESULT]          17	              4
[RESULT]          21	              1
[RESULT]         157	              6
[RESULT]         158	              2
[RESULT]         218	              1
[RESULT]         231	              1
[RESULT]         257	             30
[RESULT]         262	             17
[RESULT]         273	              1
[RESULT]         302	              1
[RESULT]         318	              1
[RESULT]         334	              1
[RESULT] -----------	---------------
[RESULT]       TOTAL	            170
```