#include <iostream>
#include <fstream>
#include <string>
#include <ctype.h>
#include <vector>
using namespace std;

int main() {
	string line;
	getline(cin, line);
	while (line != "%%end") {
		if (line.size() >= 8 && line.substr(0, 8) == "%%import") {
			string filename = line.substr(9, line.size()-9);
			ifstream in(filename.c_str());
			while (!in.eof()) {
				getline(in, line);
				cout << line << endl;
			}
		} else cout << line << endl;
		getline(cin, line);
	}
}
