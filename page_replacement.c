#include<stdio.h>
#include<stdlib.h>

#define VM_PAGE 7      
#define PM_PAGE 4         
#define TOTAL_INSERT 18
typedef struct
{
	int vmn;
	int pmn;
	int exist;
	int time;
	int found;
	int clock;
}vpage_item;
vpage_item page_table[VM_PAGE];	

vpage_item* ppage_bitmap[PM_PAGE];	

int vpage_arr[TOTAL_INSERT] = { 1,2,3,4,2,6,2,1,2,3,7,6,3,2,1,2,3,6 };	//access sequence


void init_data() 
{
	for (int i = 0; i<VM_PAGE; i++)
	{
		page_table[i].vmn = i + 1;  
		page_table[i].pmn = -1;    
		page_table[i].exist = 0;
		page_table[i].time = -1;
		page_table[i].found=0;
		page_table[i].clock=-1;
	}
	for (i = 0; i<PM_PAGE; i++) 
	{
		ppage_bitmap[i] = NULL;
	}
}

void FIFO()/*First in First out algorithm*/
{
	int k = 0;
	int i;
	int sum = 0;
	int missing_page_count = 0;
	int current_time = 0;
	bool isleft = true;   // check if there is any physical page left
	while (sum < TOTAL_INSERT)
	{
		if (page_table[vpage_arr[sum] - 1].exist == 0)
		{
			missing_page_count++;
			if (k < 4)
			{
				if (ppage_bitmap[k] == NULL) //find an empty physical page
				{
					ppage_bitmap[k] = &page_table[vpage_arr[sum] - 1];
					ppage_bitmap[k]->exist = 1;
					ppage_bitmap[k]->pmn = k;
					ppage_bitmap[k]->time = current_time;
					k++;
				}
			}
			else
			{
				int temp = ppage_bitmap[0]->time;
				int j = 0;                    
				for (i = 0; i < PM_PAGE; i++)
				{
					if (ppage_bitmap[i]->time < temp)
					{
						temp = ppage_bitmap[i]->time;
						j = i;
					}
				}
				ppage_bitmap[j]->exist = 0;
				ppage_bitmap[j] = &page_table[vpage_arr[sum] - 1];     
				ppage_bitmap[j]->exist = 1;
				ppage_bitmap[j]->pmn = j;
				ppage_bitmap[j]->time = current_time;
			}
		}
		current_time++;
		sum++;
	}
	printf("FIFO算法缺页次数为:%d\t缺页率为:%f\t置换次数为:%d\t置换率为:%f", missing_page_count, missing_page_count / (float)TOTAL_INSERT, missing_page_count - 4, (missing_page_count - 4) / (float)TOTAL_INSERT);
}

void LRU() // Least Recently Used algorithm
{
    int k = 0;
    int i;
    int sum = 0;
    int missing_page_count = 0;
    int current_time = 0;
    bool isleft = true;   
    while (sum < TOTAL_INSERT)
    {
        if (page_table[vpage_arr[sum] - 1].exist == 0)
        {
            missing_page_count++;
            if (k < 4)
            {
                if (ppage_bitmap[k] == NULL) 
                {
                    ppage_bitmap[k] = &page_table[vpage_arr[sum] - 1];
                    ppage_bitmap[k]->exist = 1;
                    ppage_bitmap[k]->pmn = k;
                    ppage_bitmap[k]->time = current_time;
                    ppage_bitmap[k]->clock = current_time;
                    k++;
                }
            }
            else    
            {
                int temp;
                int lt;
				lt = -1;
                int re;
                for (int m = 0; m < 4; m++)
                {
                    temp = current_time - ppage_bitmap[m]->clock ;
                    if (temp > lt) re = m;
                }
                ppage_bitmap[re]->exist = 0;
                ppage_bitmap[re]->clock = -1;
                ppage_bitmap[re] = &page_table[vpage_arr[sum] - 1];      
                ppage_bitmap[re]->exist = 1;
                ppage_bitmap[re]->pmn = re;
                ppage_bitmap[re]->time = current_time;
                ppage_bitmap[re]->clock = current_time+1;
            }
        }
		else page_table[vpage_arr[sum] - 1].clock = current_time+1;
        current_time++;
        sum++;
    }
    printf("LRU算法缺页次数为:%d\t缺页率为:%f\t置换次数为:%d\t置换率为:%f", missing_page_count, missing_page_count / (float)TOTAL_INSERT, missing_page_count - 4, (missing_page_count - 4) / (float)TOTAL_INSERT);
}

void OPT() // Optimal case scenario, used as comparison
{
	int k = 0;
	int i;
	int sum = 0;
	int missing_page_count = 0;
	int current_time = 0;
	bool isleft = true;   
	while (sum < TOTAL_INSERT)
	{
		if (page_table[vpage_arr[sum] - 1].exist == 0)
		{
			missing_page_count++;
			if (k < 4)
			{
				if (ppage_bitmap[k] == NULL) 
				{
					ppage_bitmap[k] = &page_table[vpage_arr[sum] - 1];
					ppage_bitmap[k]->exist = 1;
					ppage_bitmap[k]->pmn = k;
					ppage_bitmap[k]->time = current_time;
					k++;
				}
			}
			else	
			{
				int m;
				for(m=0;m<4;m++)
				{
					ppage_bitmap[m]->found=0;
				}
				int found=0;
				found=0;
				for(int j=sum;j<TOTAL_INSERT;j++)
				{
					for(m=0;m<4;m++)
					{
						if(vpage_arr[j]==ppage_bitmap[m]->pmn){
							found++;
							ppage_bitmap[m]->found=1;
						}
					}
					if(found==3) break;
				} 
				for(m=0;m<4;m++)
				{
					if(ppage_bitmap[m]->found==0) break;
				}
				ppage_bitmap[m]->exist = 0;
				ppage_bitmap[m]->found=0;
				ppage_bitmap[m] = &page_table[vpage_arr[sum] - 1];      
				ppage_bitmap[m]->exist = 1;
				ppage_bitmap[m]->pmn = m;
				ppage_bitmap[m]->time = current_time;
			}
		}
		current_time++;
		sum++;
	}
	printf("OPT算法缺页次数为:%d\t缺页率为:%f\t置换次数为:%d\t置换率为:%f", missing_page_count, missing_page_count / (float)TOTAL_INSERT, missing_page_count - 4, (missing_page_count - 4) / (float)TOTAL_INSERT);
}

int main()
{
	int a;
	printf("请输入需要选择的页面置换算法：1.FIFO\t2.LRU\t3.OPT\t输入0结束\n");
	do
	{
		scanf("%d", &a);
		switch (a)
		{
		case 1:
			init_data();
			FIFO();
			break;
		case 2:
			init_data();
			LRU();
			break;
		case 3:
			init_data();
			OPT();
			break;
		}
	} while (a != 0);
	return 0;
}
