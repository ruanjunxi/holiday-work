#include<stdio.h>
#include<math.h>
char s[5]={0,1,2,3,4};
int a[6][6],i,j;
int time[120];
int sumtime=0,sfloor=1;
int temp[120][5]; //全排列后的顺序 
int count1=0 , count2=0;
void lift(double current_floor, int request_time , int initial_floor , double targeted_floor)//当前楼层，请求时间，用户所在楼层，用户去往目标楼层 
{
	if(sumtime!=0 && sumtime<request_time)   sumtime+=request_time-sumtime;  //如果用户请求的时候电梯没搭载人，则需要加上那段空余的时间 
	sumtime+=abs(current_floor-initial_floor);
	if(current_floor-initial_floor!=0)    sumtime++;  //电梯当前所在楼层与用户所在不一致 
	sumtime+=abs(targeted_floor-initial_floor);
	sfloor=targeted_floor; 
	sumtime++;
 } 
void Swap(char *a, char *b) //用于交换 在全排列中起作用 
{
    char temp = *a;
    *a = *b;
    *b = temp;
}
void fun(char c)  //用于保存全排列 
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
    else    // 递归地产生这些排列方式
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
	scanf("%d",&a[i][j]);  //输入五组数据 并将0改为10 
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

