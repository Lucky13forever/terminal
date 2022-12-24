#include <iostream>
#include <cstring>
using namespace std;
int main()
{
    const char * a = "";
    const char * b = "a";
    int rez = strcmp(a, b) == 0 ? 0 : 1;

    cout << rez;
    return 0;
}