#include <stdio.h>

void do_stuff(int arg)
{
	int local = arg + 2;
	int i;

	for (i = 0; i < local; i++)
		printf("i = %d\n", i);
}

int main()
{
	do_stuff(2);
	return 0;
}
