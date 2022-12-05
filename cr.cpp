#include <iostream>

class A
{
public:
	int a;
	int b;
	A(int f = 1, int c = 1)
	{
		a = f; b = c;
	}
};

int main()
{
	A a;
	A b(1);
	A c(1, 2);
	A d('1', 1);
	std::cout << a.a << ' ' << a.b << ' ' << b.a << ' ' << b.b << ' ' << c.a << ' ' << c.b << ' ' << d.a << ' ' << d.b << std::endl;
	return 0; 
}