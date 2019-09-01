//
//  disk.c
//  VM
//
//  Created by Mark Quimpo on 12/9/17.
//  Copyright © 2017 Mark Quimpo. All rights reserved.
//

#include <stdio.h>
#include "vm.h"

int numdiskread = 0;
int numdiskwrite = 0;

//Call to read a page from the disk into the main memory
void diskread(int frame, int pid, int page)
{
    numdiskread++;
}

//Write out a dirty page to the disk
void diskwrite(int frame, int pid, int page)
{
    numdiskwrite++;
}
void printdiskstats()
{
    printf("Disk Read: %d\n", numdiskread);
    printf("Disk Write: %d\n", numdiskwrite);
}
