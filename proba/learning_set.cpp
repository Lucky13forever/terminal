#include <set>
#include <iostream>
using namespace std;


int main()
{
    set<int> mySet;

    mySet.insert(-3);
    mySet.insert(-2);
    mySet.insert(-1);

    for (int x : mySet){
        cout << -x <<"\n";
    }
    return 0;
}