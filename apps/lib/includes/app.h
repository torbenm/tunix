#pragma once

__attribute__((noreturn)) void exit(int status);
void putchar(char ch);
int syscall(int sysno, int arg0, int arg1, int arg2);
int readfile(const char *filename, char *buf, int len);
int writefile(const char *filename, const char *buf, int len);
