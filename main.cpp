#include <iostream>

#include "lib/pystr.h"

using namespace std;

int main(void) {
    string s1 = "Hello World!";

    string s2 = $str(s1, upper,);

    cout << s2 << endl;
}