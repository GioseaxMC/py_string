#include <stdio.h>
#include <iostream>
#include <windows.h>

#include "pystr.h"

using namespace std;

int main(void) {
    str s1 = "Hello World!";
    cout << s1.c_str() << endl;
}