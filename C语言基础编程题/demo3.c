#pragma warning(default:4996)
#include<stdio.h>
/*
	随机输入三个数字，比较大小。
*/
int main()
{
	int a, b, c, temp;
	printf("请输入三个整数:");
	scanf("%d %d %d", &a, &b, &c);
	if (a > b)
		temp = a, a = b, b = temp;
	if (a > c)
		temp = a, a = c, c = temp;
	if (b > c)
		temp = b, b = c, c = temp;
	printf("%d<%d<%d\n", a, b, c);
	return 0;
}