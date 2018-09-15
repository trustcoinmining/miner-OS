#ifndef SYSMON_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define SYSMON_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <string>
#include <cstdio>


using namespace std;

void cpu_usage(long double *cpu_usage_percent);
string pci_stats(void);

#endif