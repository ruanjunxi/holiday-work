#include<stdio.h>   
#include<string.h>
#include<iostream> 
#include<cmath> 
#define MAX_LINE 200
#define MAX_USER 5
#define MAX_COL  4

int current_time = 0;
int user[MAX_USER][MAX_COL];   //数组五行代表五人 第一列为请求时间 ，第二列为 起始楼层 ，第三列为 去往楼层  最后一列为判断乘客当前状态 1：在电梯上  0：未上电梯  -1：已下电梯 
    
void Swap(int *a, int *b) //用于交换  在对请求排列 
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
 // lift 函数模拟 taxi 模式 
 void lift(int current_floor, int request_time , int initial_floor , int targeted_floor)//当前楼层，请求时间，用户所在楼层，用户去往目标楼层 
{
	if(current_time!=0 && current_time < request_time)   current_time+=request_time-current_time;  //如果用户请求的之前电梯没搭载人，则需要加上那段空余的时间 
	current_time += abs(current_floor - initial_floor);
	if(current_floor-initial_floor!=0)      current_time++;  //电梯当前所在楼层与用户所在不一致 即电梯需要停靠接客人
	current_time += abs(targeted_floor-initial_floor);    // 运送一位乘客耗时
	current_floor = targeted_floor; 
	current_time++;
 } 

void readFile(const char *srcFile)
{
	int i = 0;
	FILE *fp = NULL;
	char strLine[MAX_LINE];
	memset(strLine, 0, MAX_LINE);
	
	if((fp = fopen(srcFile, "r")) == NULL)      //判断文件是否存在及可读  
    {   
        printf("Open Falied!");   
        return;   
    }   
    
    while (!feof(fp))                                   //循环读取每一行，直到文件尾  
    {   
        fgets(strLine, MAX_LINE, fp);    //将fp所指向的文件一行内容读到strLine缓冲区  
		sscanf(strLine, "%d %d %d", &user[i][0], &user[i][1], &user[i][2]);
		i++;
    }
}

void writeFile(const char *fileName,const char *content, int await_time, int current_floor ,int current_time)     //参数：文件全名，写入内容  
{  
    FILE *fp = NULL;                                   //定义文件指针  
    if ((fp=fopen(fileName,"a"))==NULL)         //打开指定文件，如果文件不存在则新建该文件  
    {  
        printf("Open Failed.\n");  
        return;  
    }   
    if(current_time){
    	fprintf(fp,"电梯在%d时刻停在%d楼",current_time,current_floor);
	}
	if(await_time){
		fprintf(fp,"乘客总等待时间为：%d",await_time);
	}
    fprintf(fp,"%s\n",content);                 //格式化写入文件（追加至文件末尾）  
    fclose(fp);    //关闭文件  
}  

int busMode(const char *outputFile)
{
	bool exit_flag = true;
	int j = 1;  // 用于判断电梯升降 
	int currentFloor = 1 ,  awaitTime = 0 , currentTime = 0 ;  // 电梯所在楼层  ， 用户总等待时间  ,当前时刻 
	int liftPopulation = 0;  //电梯上的人数 
	writeFile(outputFile, "===== Bus Mode ====",0,0,0);
	
	while(1) {
		 exit_flag = true;
		
		if((j/10)%2==0){      //电梯上升 
				for(int i=0; i< MAX_USER ; i++){
			if(user[i][0] <= currentTime && user[i][1]==currentFloor && user[i][3]==0){    //上电梯
			user[i][3] = 1 ;
			liftPopulation++; 
			currentTime++ ;  //上电梯即电梯停下 时间加一 
			}
		}
		for(int i=0; i< MAX_USER ; i++){
			if( user[i][3]==1 && currentFloor== user[i][2]){      // 下电梯
			user[i][3] = -1;
			currentTime++ ;
			awaitTime += currentTime - user[i][0];
			liftPopulation--;
			writeFile(outputFile," ",0,currentFloor,currentTime);
			}
		}
		currentFloor ++; 
		currentTime ++;
		}
	else{     //电梯下降 
			for(int i=0; i< MAX_USER ; i++){
			if(user[i][0] <= currentTime && user[i][1]==currentFloor && user[i][3]==0){    //上电梯
			user[i][3] = 1 ;
			liftPopulation++; 
			currentTime++ ;  //乘客上电梯即电梯停下 时间加一 
			}
		}
		for(int i=0; i< MAX_USER ; i++){
			if( user[i][3]==1 && currentFloor== user[i][2]){      // 下电梯
			user[i][3] = -1;
			currentTime++ ;
			awaitTime += currentTime - user[i][0];
			liftPopulation--;
			writeFile(outputFile," ",0,currentFloor,currentTime);
			}
		}
		currentFloor --; 
		currentTime ++;
	} 
		
		j++;
			// 退出判断 
		for(int i=0;i<MAX_USER;i++) {
		    if(user[i][3] != -1) {
		    	exit_flag = false;
		    	break;
			}
		}
		
		if(exit_flag == 1) {
		 writeFile(outputFile," ",awaitTime,0,0);
			break;
		}
		
	}
	writeFile(outputFile, "\n\n",0,0,0)	;
	}
	
	

int taxiMode(const char *outputFile)
{
	writeFile(outputFile, "===== Taxi Mode ====",0,0,0);
	writeFile(outputFile, "\n",0,0,0);
	for(int j =0 ; j < MAX_USER ; j++){
		user[j][3] = 0 ;
	}
	int  awaitTime =0 , i = 0 ,current_floor = 1;
  while( i < MAX_USER)
 {
  for(int j = 0 ;j < MAX_USER ;j++){
     if(current_time >= user[j][0] && user[j][3] != -1)  user[j][3] = 2;   // 2 代表乘客未上电梯 但已经发出请求 
   }
  
   if(i == 0)  {
   lift(current_floor, user[i][0] , user[i][1] , user[i][2]);
   user[i][3] = -1;    //标识该乘客已下电梯
   awaitTime += (current_time - user[i][0]);
   writeFile(outputFile," ",0,current_floor,current_time);   
   }
   
		  
   if(i != 0){
   
       for(int j= MAX_USER - 1 ; j >= i ;j--){
      		if((abs(current_floor - user[j][1]) > abs(current_floor - user[j-1][1])) && user[j][3]==2) {   //将当前距离电梯最小的顾客排在最前
      		Swap(&user[j][0] , &user[j+1][0]);
      		Swap(&user[j][1] , &user[j+1][1]);
      		Swap(&user[j][2] , &user[j+1][2]);
		  }
        
	  if(user[i][3] == 2){         // 此时等待中距离电梯最近的乘客
	   lift(current_floor , user[i][0] ,user[i][1] , user[i][2]); 
	   user[i][3] = -1;  //乘客下电梯
	   awaitTime += (current_time - user[i][0]);
	   writeFile(outputFile," ",0,current_floor,current_time);
	  }
       
   }
	 
 }
	i++;	
	
}
  writeFile(outputFile," ",awaitTime,0,0);
  writeFile(outputFile, "\n",0,0,0);
}
 
int main()   
{   
    for(int i=0; i<MAX_USER; i++) {
    	memset(user[i], 0, MAX_COL);
	}
	  for(int i=0 ; i<MAX_USER - 1 ;i++){    // 按照请求时刻从小到大排序 
      	for(int j=i ; j<MAX_USER - 1 ;j++){
      		if(user[j][0] > user[j+1][0]) {
      		Swap(&user[j][0] , &user[j+1][0]);
      		Swap(&user[j][1] , &user[j+1][1]);
      		Swap(&user[j][2] , &user[j+1][2]);
		  }
		  }
	  }
	  
    readFile("./input.txt");
	busMode("./output.txt");
	taxiMode("./output.txt");
    // bestMode("./output.txt", a);
    return 0;
}  
 
