#include<stdio.h>

/*
	���ӳ�������
	���ӵ�һ��ժ�����ɸ����ӣ�����һ�룬�ٶ����һ����
	�ڶ��죬Ҳ����һ�룬�ٶ��һ����
	...
	��ʮ���ʱ����ֻʣ��һ�������ˡ�
	�ʣ����ӵ�һ��ժ�˶��ٸ����ӣ�

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
	printf("��һ�����һ��ժ��%d������\n", total);
	return 0;
}