#include<stdio.h>
#include<math.h>
char s[5]={0,1,2,3,4};
int a[6][6],i,j;
int time[120];
int sumtime=0,sfloor=1;
int temp[120][5]; //ȫ���к��˳�� 
int count1=0 , count2=0;
void lift(double current_floor, int request_time , int initial_floor , double targeted_floor)//��ǰ¥�㣬����ʱ�䣬�û�����¥�㣬�û�ȥ��Ŀ��¥�� 
{
	if(sumtime!=0 && sumtime<request_time)   sumtime+=request_time-sumtime;  //����û������ʱ�����û�����ˣ�����Ҫ�����Ƕο����ʱ�� 
	sumtime+=abs(current_floor-initial_floor);
	if(current_floor-initial_floor!=0)    sumtime++;  //���ݵ�ǰ����¥�����û����ڲ�һ�� 
	sumtime+=abs(targeted_floor-initial_floor);
	sfloor=targeted_floor; 
	sumtime++;
 } 
void Swap(char *a, char *b) //���ڽ��� ��ȫ������������ 
{
    char temp = *a;
    *a = *b;
    *b = temp;
}
void fun(char c)  //���ڱ���ȫ���� 
{
	temp[count1][count2]= c;
	count2++;
	if(count2==5)
	{
		count1++;
		count2=0;
	}
}
void Perm(char list[], int k, int m)
{ 
    int i;
    if (k == m) 
	{
        for (i = 0; i <= m; i++)
         fun(list[i]);
    }
    else    // �ݹ�ز�����Щ���з�ʽ
    for (i=k; i <= m; i++) {
        Swap (&list[k], &list[i]);
        Perm (list, k+1, m);
        Swap (&list [k], &list [i]);
    }
}
  
int main()
{
	int list[] = {0,1,2,3,4};
 	for(i=0;i<5;i++)
 	 for(j=0;j<3;j++)
   {
	scanf("%d",&a[i][j]);  //������������ ����0��Ϊ10 
	if(j>0&&(a[i][j]==0))
	{
		a[i][j]=10;
	}
   }
   Perm(s,0,4);
		for(i=0;i<120;i++)
		{
			sumtime=0;
			 for(j=0;j<5;j++)
		 {
		 	lift(sfloor , a[temp[i][j]][0] , a[temp[i][j]][1] ,a[temp[i][j]][2]);
		  } 
		  time[i]=sumtime;
		}
		for(int k=0;k<119;k++)
		{
			i=0;
		if(time[k]<time[k+1])
		{
			time[k+1]=time[k];
			i=k;
		}
		else i=k+1;
		  }  
		  printf("the best method costs %d\n",time[119]);
		  return 0;
 }

