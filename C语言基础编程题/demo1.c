#include<stdio.h>
/*
	쳲��������� - ��������
	�ʼ�� 1 ������
	ÿ�������� 1 ������
	�� n ��󽫻��ж���ֻ���ӣ�
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
	printf("%d��󽫻���%dֻ����\n", n, total * 2);
	return 0;
}