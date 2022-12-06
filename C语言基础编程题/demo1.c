#include<stdio.h>
/*
	斐波那契数列 - 兔子问题
	最开始有 1 对兔子
	每三个月生 1 对兔子
	求 n 年后将会有多少只兔子？
	1  1
	2  1
	3  2
	4  3
	5  5
*/
#define MM 12
int rabbit(int num)
{
	if ((num == 1) || (num == 2))
		return 1;
	return rabbit(num - 1) + rabbit(num - 2);
}
int main()
{
	int n = 3 * MM, total;
	total = rabbit(n);
	printf("%d年后将会有%d只兔子\n", n, total * 2);
	return 0;
}