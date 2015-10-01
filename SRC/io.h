#line 10 "io.nw"
#ifndef _IO_H_
#define _IO_H_

#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
using namespace std;

#define STDOUT 1
#define STDERR 2
#define SILENT 3
#define EXFILE 4
#define PIPE   5

void initPipe();
void closePipe();
void setSelector(FILE * in);
void setSelector(int x);
int getSelector();

void ioprint(const string & x);
void ioprint(int x);
void ioprint(long long int x);
void ioprint(double x);
void ioprint(char x);
void ioprintln(const string & x);
void ioprintln(int x);
void ioprintln(long long int x);
void ioprintln(double x);
void ioprintln(char x);
void ioprintln();

#endif
