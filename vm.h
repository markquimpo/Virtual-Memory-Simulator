//
//  vm.h
//  VM
//
//  Created by Mark Quimpo on 12/9/17.
//  Copyright © 2017 Mark Quimpo. All rights reserved.
//

#ifndef vm_h
#define vm_h

#include <stdbool.h>

#define debug(M, ...) fprintf(stderr, "DEBUG(%s:%d) " M, __FILE__, __LINE__, ##__VA_ARGS__)
#endif /* vm_h */

#define RANDOM 0

#define MAX_PAGE 8
#define MAX_FRAME 3
#define MAX_PID 1

extern int replacementpolicy;
int pagereplacement();
int memorymanagementunit(int pid, int addr, char type, bool *hit);
void printpagetablestats();
void pagetableinit();
void printdiskstats();
