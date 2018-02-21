#include<stdio.h>   
#include<string.h>
#include<iostream> 
#include<cmath> 
#define MAX_LINE 200
#define MAX_USER 5
#define MAX_COL  4

int current_time = 0;
int user[MAX_USER][MAX_COL];   //�������д������� ��һ��Ϊ����ʱ�� ���ڶ���Ϊ ��ʼ¥�� ��������Ϊ ȥ��¥��  ���һ��Ϊ�жϳ˿͵�ǰ״̬ 1���ڵ�����  0��δ�ϵ���  -1�����µ��� 
    
void Swap(int *a, int *b) //���ڽ���  �ڶ��������� 
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
 // lift ����ģ�� taxi ģʽ 
 void lift(int current_floor, int request_time , int initial_floor , int targeted_floor)//��ǰ¥�㣬����ʱ�䣬�û�����¥�㣬�û�ȥ��Ŀ��¥�� 
{
	if(current_time!=0 && current_time < request_time)   current_time+=request_time-current_time;  //����û������֮ǰ����û�����ˣ�����Ҫ�����Ƕο����ʱ�� 
	current_time += abs(current_floor - initial_floor);
	if(current_floor-initial_floor!=0)      current_time++;  //���ݵ�ǰ����¥�����û����ڲ�һ�� ��������Ҫͣ���ӿ���
	current_time += abs(targeted_floor-initial_floor);    // ����һλ�˿ͺ�ʱ
	current_floor = targeted_floor; 
	current_time++;
 } 

void readFile(const char *srcFile)
{
	int i = 0;
	FILE *fp = NULL;
	char strLine[MAX_LINE];
	memset(strLine, 0, MAX_LINE);
	
	if((fp = fopen(srcFile, "r")) == NULL)      //�ж��ļ��Ƿ���ڼ��ɶ�  
    {   
        printf("Open Falied!");   
        return;   
    }   
    
    while (!feof(fp))                                   //ѭ����ȡÿһ�У�ֱ���ļ�β  
    {   
        fgets(strLine, MAX_LINE, fp);    //��fp��ָ����ļ�һ�����ݶ���strLine������  
		sscanf(strLine, "%d %d %d", &user[i][0], &user[i][1], &user[i][2]);
		i++;
    }
}

void writeFile(const char *fileName,const char *content, int await_time, int current_floor ,int current_time)     //�������ļ�ȫ����д������  
{  
    FILE *fp = NULL;                                   //�����ļ�ָ��  
    if ((fp=fopen(fileName,"a"))==NULL)         //��ָ���ļ�������ļ����������½����ļ�  
    {  
        printf("Open Failed.\n");  
        return;  
    }   
    if(current_time){
    	fprintf(fp,"������%dʱ��ͣ��%d¥",current_time,current_floor);
	}
	if(await_time){
		fprintf(fp,"�˿��ܵȴ�ʱ��Ϊ��%d",await_time);
	}
    fprintf(fp,"%s\n",content);                 //��ʽ��д���ļ���׷�����ļ�ĩβ��  
    fclose(fp);    //�ر��ļ�  
}  

int busMode(const char *outputFile)
{
	bool exit_flag = true;
	int j = 1;  // �����жϵ������� 
	int currentFloor = 1 ,  awaitTime = 0 , currentTime = 0 ;  // ��������¥��  �� �û��ܵȴ�ʱ��  ,��ǰʱ�� 
	int liftPopulation = 0;  //�����ϵ����� 
	writeFile(outputFile, "===== Bus Mode ====",0,0,0);
	
	while(1) {
		 exit_flag = true;
		
		if((j/10)%2==0){      //�������� 
				for(int i=0; i< MAX_USER ; i++){
			if(user[i][0] <= currentTime && user[i][1]==currentFloor && user[i][3]==0){    //�ϵ���
			user[i][3] = 1 ;
			liftPopulation++; 
			currentTime++ ;  //�ϵ��ݼ�����ͣ�� ʱ���һ 
			}
		}
		for(int i=0; i< MAX_USER ; i++){
			if( user[i][3]==1 && currentFloor== user[i][2]){      // �µ���
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
	else{     //�����½� 
			for(int i=0; i< MAX_USER ; i++){
			if(user[i][0] <= currentTime && user[i][1]==currentFloor && user[i][3]==0){    //�ϵ���
			user[i][3] = 1 ;
			liftPopulation++; 
			currentTime++ ;  //�˿��ϵ��ݼ�����ͣ�� ʱ���һ 
			}
		}
		for(int i=0; i< MAX_USER ; i++){
			if( user[i][3]==1 && currentFloor== user[i][2]){      // �µ���
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
			// �˳��ж� 
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
     if(current_time >= user[j][0] && user[j][3] != -1)  user[j][3] = 2;   // 2 ����˿�δ�ϵ��� ���Ѿ��������� 
   }
  
   if(i == 0)  {
   lift(current_floor, user[i][0] , user[i][1] , user[i][2]);
   user[i][3] = -1;    //��ʶ�ó˿����µ���
   awaitTime += (current_time - user[i][0]);
   writeFile(outputFile," ",0,current_floor,current_time);   
   }
   
		  
   if(i != 0){
   
       for(int j= MAX_USER - 1 ; j >= i ;j--){
      		if((abs(current_floor - user[j][1]) > abs(current_floor - user[j-1][1])) && user[j][3]==2) {   //����ǰ���������С�Ĺ˿�������ǰ
      		Swap(&user[j][0] , &user[j+1][0]);
      		Swap(&user[j][1] , &user[j+1][1]);
      		Swap(&user[j][2] , &user[j+1][2]);
		  }
        
	  if(user[i][3] == 2){         // ��ʱ�ȴ��о����������ĳ˿�
	   lift(current_floor , user[i][0] ,user[i][1] , user[i][2]); 
	   user[i][3] = -1;  //�˿��µ���
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
	  for(int i=0 ; i<MAX_USER - 1 ;i++){    // ��������ʱ�̴�С�������� 
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
 
