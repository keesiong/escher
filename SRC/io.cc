#line 47 "io.nw"
#include "io.h"

ofstream bcpipe; 
FILE * iofile;
static int selector;
ofstream logfile("log.complete");

void initPipe() { bcpipe.open("pipe1"); bcpipe.setf(ios::fixed); }
void closePipe() { bcpipe.close(); }
void setSelector(FILE * in) { iofile = in; selector = EXFILE; }
void setSelector(int x) { selector = x; }
int getSelector() { return selector; }

void ioprint(const string & x) { 
	
#line 97 "io.nw"
// #define DEBUG
#ifdef DEBUG
logfile << x;
#endif
if (selector == SILENT) return;
if (selector == STDOUT) cout << x;
if (selector == STDERR) cerr << x;
else if (selector == PIPE) bcpipe << x;
#line 61 "io.nw"
                              
#line 118 "io.nw"
if (selector == EXFILE) fprintf(iofile, x.c_str());
#line 61 "io.nw"
                                           }
void ioprint(int x) { 
	
#line 97 "io.nw"
// #define DEBUG
#ifdef DEBUG
logfile << x;
#endif
if (selector == SILENT) return;
if (selector == STDOUT) cout << x;
if (selector == STDERR) cerr << x;
else if (selector == PIPE) bcpipe << x;
#line 63 "io.nw"
                              
#line 122 "io.nw"
if (selector == EXFILE) fprintf(iofile, "%d", x);
#line 63 "io.nw"
                                             }
void ioprint(long long int x) { 
	
#line 97 "io.nw"
// #define DEBUG
#ifdef DEBUG
logfile << x;
#endif
if (selector == SILENT) return;
if (selector == STDOUT) cout << x;
if (selector == STDERR) cerr << x;
else if (selector == PIPE) bcpipe << x;
#line 65 "io.nw"
                              
#line 126 "io.nw"
if (selector == EXFILE) fprintf(iofile, "%lld", x);
#line 65 "io.nw"
                                              }
void ioprint(double x) { 
	// cout.setf(ios::fixed, ios::floatfield); 
	cout.setf(ios::showpoint); 
	
#line 97 "io.nw"
// #define DEBUG
#ifdef DEBUG
logfile << x;
#endif
if (selector == SILENT) return;
if (selector == STDOUT) cout << x;
if (selector == STDERR) cerr << x;
else if (selector == PIPE) bcpipe << x;
#line 69 "io.nw"
                              
#line 130 "io.nw"
if (selector == EXFILE) fprintf(iofile, "%f", x);
#line 69 "io.nw"
                                             }
void ioprint(char x) { 
	
#line 97 "io.nw"
// #define DEBUG
#ifdef DEBUG
logfile << x;
#endif
if (selector == SILENT) return;
if (selector == STDOUT) cout << x;
if (selector == STDERR) cerr << x;
else if (selector == PIPE) bcpipe << x;
#line 71 "io.nw"
                              
#line 134 "io.nw"
if (selector == EXFILE) fprintf(iofile, "%c", x);
#line 71 "io.nw"
                                             }
void ioprintln(const string & x) { 
	
#line 108 "io.nw"
#ifdef DEBUG
logfile << x << endl;
#endif
if (selector == SILENT) return;
if (selector == STDOUT) cout << x << endl;
if (selector == STDERR) cerr << x << endl;
else if (selector == PIPE) bcpipe << x << endl;
#line 73 "io.nw"
                                 
#line 118 "io.nw"
if (selector == EXFILE) fprintf(iofile, x.c_str());
#line 73 "io.nw"
                                              }
void ioprintln(int x) { 
	
#line 108 "io.nw"
#ifdef DEBUG
logfile << x << endl;
#endif
if (selector == SILENT) return;
if (selector == STDOUT) cout << x << endl;
if (selector == STDERR) cerr << x << endl;
else if (selector == PIPE) bcpipe << x << endl;
#line 75 "io.nw"
                                 
#line 122 "io.nw"
if (selector == EXFILE) fprintf(iofile, "%d", x);
#line 75 "io.nw"
                                                }
void ioprintln(long long int x) { 
	
#line 108 "io.nw"
#ifdef DEBUG
logfile << x << endl;
#endif
if (selector == SILENT) return;
if (selector == STDOUT) cout << x << endl;
if (selector == STDERR) cerr << x << endl;
else if (selector == PIPE) bcpipe << x << endl;
#line 77 "io.nw"
                                 
#line 126 "io.nw"
if (selector == EXFILE) fprintf(iofile, "%lld", x);
#line 77 "io.nw"
                                                 }
void ioprintln(double x) { 
	//cout.setf(ios::fixed, ios::floatfield); 
	cout.setf(ios::showpoint); 
	
#line 108 "io.nw"
#ifdef DEBUG
logfile << x << endl;
#endif
if (selector == SILENT) return;
if (selector == STDOUT) cout << x << endl;
if (selector == STDERR) cerr << x << endl;
else if (selector == PIPE) bcpipe << x << endl;
#line 81 "io.nw"
                                 
#line 130 "io.nw"
if (selector == EXFILE) fprintf(iofile, "%f", x);
#line 81 "io.nw"
                                                }
void ioprintln(char x) { 
	
#line 108 "io.nw"
#ifdef DEBUG
logfile << x << endl;
#endif
if (selector == SILENT) return;
if (selector == STDOUT) cout << x << endl;
if (selector == STDERR) cerr << x << endl;
else if (selector == PIPE) bcpipe << x << endl;
#line 83 "io.nw"
                                 
#line 134 "io.nw"
if (selector == EXFILE) fprintf(iofile, "%c", x);
#line 83 "io.nw"
                                                }
void ioprintln() {
#ifdef DEBUG
        logfile << endl;
#endif
        if (selector == SILENT) return;
        else if (selector == STDOUT) cout << endl;
	else if (selector == EXFILE) fprintf(iofile, "\n");
	else if (selector == PIPE) bcpipe << endl;
        else cerr << endl;
}
