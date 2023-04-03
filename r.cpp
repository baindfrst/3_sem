#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

class myclass
{
    public:
    int x = 2;
    int& get()
    {
        return x;
    }
    void ch()
    {
        cout << x << endl;
    }
};

int main () {
    myclass a1;
    a1.get() = 10;
    a1.ch();
    return 0;
}
