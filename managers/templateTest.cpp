#include <stdio.h>

template<typename... Args> void test(const char* f, Args... args) {
	print(f, args...);
}

int main()
{
	int a = 2;
	test("%s\n", "test");
	test("%s %d %d %p\n", "second test", 2, a, &a);
}