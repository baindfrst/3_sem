#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

class myclass
{
    public:
    static int x;
    int get()
    {
        int y = x;
        x += 10;
        return y;
    }
};
int myclass::x = 10;
int main () {
    myclass a1;
    myclass a2 = a1;
    cout << a1.get() <<  ' ' << a2.get() << ' '  << endl;
    return 0;
}
