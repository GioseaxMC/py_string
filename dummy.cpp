#include <windows.h>
#include <iostream>
#include "pystr.h"

#define python(_string) execute_python(str(#_string));

void execute_python(str &code) {
    str command = "python -c ";
    command += str("\"") + code.replace("\n", ";") + str("\"");
    const char* s = command.c_str();
    std::string cmd = s;
    std::cout << "[CMD] " << cmd << std::endl;
    system(s);
}

int main(void) {

python(from random import randint as r)

}