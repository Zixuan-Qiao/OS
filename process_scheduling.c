#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int MAXJOB=50;

typedef struct node
{
	int number;		        
	int reach_time;	
	int need_time;	
	int privilege;	
	float excellent;
	int start_time;	
	int wait_time;	
	int visited;	
	bool isreached;	
}job;
job jobs[MAXJOB];	
int quantity;		

void initial_jobs()
{
	int i;
	for(i=0;i<MAXJOB;i++)
	{
		jobs[i].number=0;
		jobs[i].reach_time=0;
		jobs[i].privilege=0;
		jobs[i].excellent=0;
		jobs[i].start_time=0;
		jobs[i].wait_time=0;
		jobs[i].visited=0;
		jobs[i].isreached=false;
	}
	quantity=0;
}

void reset_jinfo() 
{ 
	int i; 
	for(i=0;i<MAXJOB;i++)
	{ 
		jobs[i].start_time=0; 
		jobs[i].wait_time=0; 
		jobs[i].visited=0; 
		jobs[i].isreached=0;
	}
}

int findminjob(job jobs[],int count)
{
	int minjob=-1;//=jobs[0].need_time;
	int minloc=-1;
	for(int i=0;i<count;i++)
	{
		if(minloc==-1){
			if(	 jobs[i].isreached==true && jobs[i].visited==0){
			minjob=jobs[i].need_time;
			minloc=i;
			}
		}
		else if(minjob>jobs[i].need_time&&jobs[i].visited==0&&jobs[i].isreached==true)
		{
			minjob=jobs[i].need_time;
			minloc=i;
		}
	}
	return minloc;
}

int findrearlyjob(job jobs[],int count)
{
	int rearlyloc=-1;
	int rearlyjob=-1;
	for(int i=0;i<count;i++)
	{
		if(rearlyloc==-1){
			if(jobs[i].visited==0){
			rearlyloc=i;
			rearlyjob=jobs[i].reach_time;
			}
		}
		else if(rearlyjob>jobs[i].reach_time&&jobs[i].visited==0)
		{
			rearlyjob=jobs[i].reach_time;
			rearlyloc=i;
		}
	}
	return rearlyloc;
}

int findhighestjob(int ctime)
{
	int re=-1;
	float p=0;
	float lastp=0;
	for(int i=0;i<quantity;i++)
	{
		if(jobs[i].reach_time<=ctime&&jobs[i].visited==0)
		{
				//p=(jobs[i].need_time+ctime-jobs[i].reach_time)/jobs[i].need_time;
				p=(float(jobs[i].need_time)+float(ctime)-float(jobs[i].reach_time))/float(jobs[i].need_time);
				if(p>lastp) {lastp=p; re=i;}
		}
	}
	return re;
}

int findHPjob(int ctime)
{
	int re=-1;
	int p;
	int lastp=-1;
	p=0;
	lastp=-1;
	for(int i=0;i<quantity;i++)
	{
		if(jobs[i].reach_time<=ctime&&jobs[i].visited==0)
		{
				p=jobs[i].privilege;
				if(p>lastp) {lastp=p; re=i;}
				else continue;
		}
	}
	return re;
}


void readJobdata()
{
	FILE *fp;
	char fname[20];
	int i;

	if((fp=fopen("test.txt","r"))==NULL)
	{
		printf("error, open file failed, please check filename:\n");
	}
	else
	{

		while(!feof(fp))
		{
			if(fscanf(fp,"%d %d %d %d",&jobs[quantity].number,&jobs[quantity].reach_time,&jobs[quantity].need_time,&jobs[quantity].privilege)==4)
			quantity++;
		}

		printf("output the origin job data\n");
		printf("---------------------------------------------------------------------\n");
		printf("\tjobID\treachtime\tneedtime\tprivilege\n");
		for(i=0;i<quantity;i++)
		{
	printf("\t%-8d\t%-8d\t%-8d\t%-8d\n",jobs[i].number,jobs[i].reach_time,jobs[i].need_time,jobs[i].privilege);
		}
	}
}
//First Come First Service algorithm
void FCFS() 
{ 
	int i; 
	int current_time=0;
	int loc;
	int total_waitime=0;
	int total_roundtime=0;

	loc=findrearlyjob(jobs,quantity);

	printf("\n\nFCFS算法作业流\n");
	printf("------------------------------------------------------------------------\n"); 
	printf("\tjobID\treachtime\tstarttime\twaittime\troundtime\n");
	current_time=jobs[loc].reach_time; 

	for(i=0;i<quantity;i++)
	{ 
		if(jobs[loc].reach_time>current_time)
		{
			jobs[loc].start_time=jobs[loc].reach_time;
			current_time=jobs[loc].reach_time;
		}
		else
		{
			jobs[loc].start_time=current_time;
		}
		jobs[loc].wait_time=current_time-jobs[loc].reach_time; 
	printf("\t%-8d\t%-8d\t%-8d\t%-8d\t%-8d\n",loc+1,jobs[loc].reach_time,jobs[loc].start_time,jobs[loc].wait_time,
			jobs[loc].wait_time+jobs[loc].need_time);
		jobs[loc].visited=1;
		current_time+=jobs[loc].need_time;
		total_waitime+=jobs[loc].wait_time; 
		total_roundtime=total_roundtime+jobs[loc].wait_time+jobs[loc].need_time;

		loc=findrearlyjob(jobs,quantity);
	} 
	printf("总等待时间:%-8d 总周转时间:%-8d\n",total_waitime,total_roundtime); 
	printf("平均等待时间: %4.2f 平均周转时间: %4.2f\n",(float)total_waitime/(quantity),(float)total_roundtime/(quantity)); 
}

//Shortest Job First algorithm
void SFJschdulejob(job jobs[],int count)
{
	int i; 
	int current_time=0;
	int loc;
	int total_waitime=0;
	int total_roundtime=0;
	for(i=0;i<quantity;i++)
	{
		if(current_time>=jobs[i].reach_time) jobs[i].isreached=1;
	}
	printf("\n\nSFJschdule算法作业流\n");
	printf("------------------------------------------------------------------------\n"); 
	printf("\tjobID\treachtime\tstarttime\twaittime\troundtime\n");
	loc=findrearlyjob(jobs,quantity);	
	current_time=jobs[loc].reach_time; 	
	for(i=0;i<quantity;i++)
	{
		if(current_time>=jobs[i].reach_time) jobs[i].isreached=true;
	}
	loc=findminjob(jobs,quantity);
	for(i=0;i<quantity;i++)
	{ 
		if(loc==-1)
		{
			current_time+=5;
			for(int j=0;j<quantity;j++)
			{
				if(current_time>=jobs[j].reach_time) jobs[j].isreached=true;
			}
			loc=findminjob(jobs,quantity);
			i--;
			continue;
		}
		else jobs[loc].start_time=current_time;

		jobs[loc].wait_time=current_time-jobs[loc].reach_time; 
	printf("\t%-8d\t%-8d\t%-8d\t%-8d\t%-8d\n",loc+1,jobs[loc].reach_time,jobs[loc].start_time,jobs[loc].wait_time,
			jobs[loc].wait_time+jobs[loc].need_time);
		jobs[loc].visited=1;
		current_time+=jobs[loc].need_time;
		for(int j=0;j<quantity;j++)
		{
			if(current_time>=jobs[j].reach_time) jobs[j].isreached=true;
		}
		total_waitime+=jobs[loc].wait_time; 
		total_roundtime=total_roundtime+jobs[loc].wait_time+jobs[loc].need_time;

		loc=findminjob(jobs,quantity);
	} 
	printf("总等待时间:%-8d 总周转时间:%-8d\n",total_waitime,total_roundtime); 
	printf("平均等待时间: %4.2f 平均周转时间: %4.2f\n",(float)total_waitime/(quantity),(float)total_roundtime/(quantity)); 
}

// Highest Response Ratio First
void HRRFschdulejob() 
{
	int i; 
	int current_time=0;
	int loc;
	int total_waitime=0;
	int total_roundtime=0;
	printf("\n\n高响应比算法作业流\n");
	printf("------------------------------------------------------------------------\n"); 
	printf("\tjobID\treachtime\tstarttime\twaittime\troundtime\n");
	loc=findrearlyjob(jobs,quantity);	
	current_time=jobs[loc].reach_time; 	
	loc=findhighestjob(current_time);
	for(i=0;i<quantity;i++)
	{ 
		if(loc==-1)
		{
			current_time+=5;
			loc=findhighestjob(current_time);
			i--;
			continue;
		}
		else jobs[loc].start_time=current_time;

		jobs[loc].wait_time=current_time-jobs[loc].reach_time; 
	printf("\t%-8d\t%-8d\t%-8d\t%-8d\t%-8d\n",loc+1,jobs[loc].reach_time,jobs[loc].start_time,jobs[loc].wait_time,
			jobs[loc].wait_time+jobs[loc].need_time);
		jobs[loc].visited=1;
		current_time+=jobs[loc].need_time;
		total_waitime+=jobs[loc].wait_time; 
		total_roundtime=total_roundtime+jobs[loc].wait_time+jobs[loc].need_time;

		loc=findhighestjob(current_time);
	} 
	printf("总等待时间:%-8d 总周转时间:%-8d\n",total_waitime,total_roundtime); 
	printf("平均等待时间: %4.2f 平均周转时间: %4.2f\n",(float)total_waitime/(quantity),(float)total_roundtime/(quantity)); 
}

// Highest Priority First algorithm
void HPF()
{
	int i; 
	int current_time=0;
	int loc;
	int total_waitime=0;
	int total_roundtime=0;
	printf("\n\n优先权高者优先算法作业流\n");
	printf("------------------------------------------------------------------------\n"); 
	printf("\tjobID\treachtime\tstarttime\twaittime\troundtime\n");
	loc=findrearlyjob(jobs,quantity);	
	current_time=jobs[loc].reach_time; 	
	loc=findHPjob(current_time);
	for(i=0;i<quantity;i++)
	{ 
		if(loc==-1)
		{
			current_time+=5;
			loc=findHPjob(current_time);
			i--;
			continue;
		}
		else jobs[loc].start_time=current_time;

		jobs[loc].wait_time=current_time-jobs[loc].reach_time; 
	printf("\t%-8d\t%-8d\t%-8d\t%-8d\t%-8d\n",loc+1,jobs[loc].reach_time,jobs[loc].start_time,jobs[loc].wait_time,
			jobs[loc].wait_time+jobs[loc].need_time);
		jobs[loc].visited=1;
		current_time+=jobs[loc].need_time;
		total_waitime+=jobs[loc].wait_time; 
		total_roundtime=total_roundtime+jobs[loc].wait_time+jobs[loc].need_time;

		loc=findHPjob(current_time);
	} 
	printf("总等待时间:%-8d 总周转时间:%-8d\n",total_waitime,total_roundtime); 
	printf("平均等待时间: %4.2f 平均周转时间: %4.2f\n",(float)total_waitime/(quantity),(float)total_roundtime/(quantity)); 
}

int main() 
{  
	initial_jobs(); 
	readJobdata(); 
	FCFS();
	reset_jinfo();
	SFJschdulejob(jobs,quantity);
	reset_jinfo();
	HRRFschdulejob();
	reset_jinfo();
	HPF();
	system("pause");
	return 0;
}
