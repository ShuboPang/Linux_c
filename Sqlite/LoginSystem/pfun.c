#include <stdio.h>

void number()
{
	printf("1234567890\n");
}
void character()
{
	printf("abcdefghijk\n");
}
void sign()
{
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

void main()
{
	void (*p)();

	number();
	character();
	sign();

	printf("point function:\n");
	p=number;
	p();
	p=character;
	p();
	p=sign;
	p();
	printf("hello world,i can conquer you!\n");
	printf("this is amazing!\n");
}
