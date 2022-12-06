#include<stdio.h>

/*
	猴子吃桃问题
	猴子第一天摘了若干个桃子，吃了一半，再多吃了一个。
	第二天，也吃了一半，再多吃一个。
	...
	第十天的时候发现只剩下一个桃子了。
	问：猴子第一天摘了多少个桃子？

	1  n
	2  n/2-1
	3  n/2-1
	...
	10  n/2-1=1   n9 = (n10 + 1) * 2
*/

int monkey(int day)
{
	if (day == 10)
		return 1;
	return (monkey(day + 1) + 1) * 2;
}
int main()
{
	int day = 1;
	int total = monkey(day);
	printf("第一天猴子一共摘了%d个桃子\n", total);
	return 0;
}