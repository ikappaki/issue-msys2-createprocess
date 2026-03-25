# MSYS2 `{}` argument bug reproducer

Shows that `{}` are stripped from arguments when the target program is built with the MSYS2 POSIX runtime.

## Files

- `wrapper.c` - runs a program via `CreateProcess`, forwarding args
- `echo2.c` - prints its arguments

## Setup

Install `gcc` in the MSYS2 POSIX environment and `mingw-w64-x86_64-gcc` in the MSYS2/MINGW64 environment.

## Build

```sh
# MSYS/MINGW64 shell
gcc -o wrapper.x86_64.exe wrapper.c
gcc -o echo2.x86_64.exe echo2.c

# MSYS2 (POSIX) shell
gcc -o echo2.msys2.exe echo2.c
````

## Reproduce

Running the programs directly preserves `{}`:

```sh
$ ./echo2.x86_64.exe 1 {2} 3
1 {2} 3
$ ./echo2.msys2.exe 1 {2} 3
1 {2} 3
```

Launching via `wrapper` triggers the POSIX runtime issue:

```sh
# OK (MINGW64 target)
./wrapper.x86_64.exe ./echo2.x86_64.exe 1 {2} 3
# Executing: "./echo2.x86_64.exe" 1 {2} 3
# 1 {2} 3

# ISSUE (MSYS2 POSIX target)
./wrapper.x86_64.exe ./echo2.msys2.exe 1 {2} 3
# Executing: "./echo2.msys2.exe" 1 {2} 3
# 1 2 3
```

The second argument loses the {} when using the MSYS2 POSIX runtime.

Same behaviour occurs with /usr/bin/echo
```sh
$ ./wrapper.x86_64.exe /usr/bin/echo 1 {2} 3
# Executing: "D:/msys64/usr/bin/echo" 1 {2} 3
# 1 2 3

```

## Summary

Expected:

```
1 {2} 3
```

Actual (MSYS2 POSIX):

```
1 2 3
```

## Notes

* Only affects MSYS2 POSIX executables
* MINGW64 executables behave correctly


