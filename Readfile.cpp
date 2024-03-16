#include "readfile.h"
#include "PCB.h"
#include "queue.h"
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

Queue<PCB> read_file(const char* fileName) {
    Queue<PCB> result;
    vector<PCB> A;
    ifstream file;
    file.open(fileName);

    if (!file.good()) {
        return result;
    }

    string line;

    while (getline(file, line)) {
        vector<int> p = parse_line(line);
        result.push_back(PCB(p[0], p[1], p[2]));
        line.clear();
    }

    return result;
}

vector<int> parse_line(string line) {
    stringstream stream(line);
    vector<int> result;
    int n;

    while (stream >> n) {
        result.push_back(n);
    }

    return result;
}