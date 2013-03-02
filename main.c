//
//  main.c
//  hello
//
//  Created by Yigal Alexander on 10/8/12.
//  Copyright (c) 2012 Yigal Alexander. All rights reserved.
//

#include <stdio.h>
#include "util\opcode.h"


int main(int argc, const char * argv[])
{

    // insert code here...
    printf("Hello, World!\n");
    int i;
    for (i=1; i<=250; i++)
        pnum(i*2);
    printf("Hello, this is another test  to pushing changes in GitHub, yet again");
    printf("Remote repository test");
    
    return 0;
}

