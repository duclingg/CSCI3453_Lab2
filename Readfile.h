#ifndef READFILE_H
#define READFILE_H

#include <vector>
#include <string>
#include "PCB.h"
#include "Queue.h"

Queue<PCB> read_file(const char*);
std::vector<int> parse_line(std::string);

#endif