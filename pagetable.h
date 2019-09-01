//
//  pagetable.h
//  VM
//
//  Created by Mark Quimpo on 12/9/17.
//  Copyright © 2017 Mark Quimpo. All rights reserved.
//

#ifndef pagetable_h
#define pagetable_h

//Invert to get victim frame
typedef struct
{
    int pid;
    int page;
} Invert;

//PageTableEntry
typedef struct
{
    int framenum;   //Physical page Number
    bool valid;     //Valid or Invalid Bit
    bool dirty;     //Dirty Bit
} PTE;

typedef struct
{
    int hitcount;
    int misscount;
} STATS;

//PageTable
typedef struct
{
    PTE entry[MAX_PID][MAX_PAGE];
    STATS stats;
} PT;

int pagefaulthandler(int pid, int pagenum, char type, bool *hit);
extern int frameindex;
extern int framereference;


#endif /* pagetable_h */
