//
//  clientFunctions.c
//  prototyp1
//
//  Created by Christian Ekenstedt on 2015-05-06.
//  Copyright (c) 2015 Grupp 3. All rights reserved.
//

#include "clientFunctions.h"
#include "multiOS.h"

void ClearScreen(){
#ifdef _WIN32
    system("cls");
#else //
    system("clear");
#endif // rest
}

