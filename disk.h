//
//  disk.h
//  VM
//
//  Created by Mark Quimpo on 12/9/17.
//  Copyright © 2017 Mark Quimpo. All rights reserved.
//

#ifndef disk_h
#define disk_h
void diskread(int frame, int pid, int page);
void diskwrite(int frame, int pid, int page);

#endif /* disk_h */
