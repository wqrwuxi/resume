///////////////////////////////////////////////////////////////////////////////
//头文件区域，所需要的头文件，添加到这里。

#include <stdio.h>
#include <string.h>
#include <stdlib.h> // 提供 exit()的原型  mallic
#include <errno.h>
#include <sys/stat.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

///////////////////////////////////////////////////////////////////////////////
//宏定义


///////////////////////////////////////////////////////////////////////////////
//结构体
typedef struct
{
    int count;
    int* data;
} datas;
///////////////////////////////////////////////////////////////////////////////
//变量声明
int segments;
datas datas0;
datas datas_final;
datas* datas_all=NULL;
///////////////////////////////////////////////////////////////////////////////
//函数声明
void input_data(char* filename)
{
    FILE* fp;
    struct stat	statbuf;
    int size=0;
    int value=0;
    
    fp=fopen(filename,"r");

    if (fp==NULL)
    {
       fprintf(stderr,"Open fail:%s\n",(strerror(errno)));
       exit(EXIT_FAILURE);
    }

    if (lstat(filename, &statbuf) < 0)	/* stat error */
	{
        fprintf(stderr,"function'lstat' fail:%s\n",(strerror(errno)));
        exit(EXIT_FAILURE);
    }
    
    size=statbuf.st_size/3;
    datas0.count=0;
    datas0.data=(int*)malloc(sizeof(int)*size);
    datas_final.count=0;
    datas_final.data=(int*)malloc(sizeof(int)*size);
    
    if(datas0.data==NULL)
    {
        fprintf(stderr, "Error in malloc\n",(strerror(errno)));
    }

    if(datas_final.data==NULL)
    {
        fprintf(stderr, "Error in malloc\n",(strerror(errno)));
    }

    while(fscanf(fp,"%d,",&value)!=EOF)//注意逗号，C语言书P218
    {
        datas0.data[datas0.count]=value;
        datas0.count++;
        
       if(datas0.count==size)
       {
           size=size*2;
           datas0.data=realloc(datas0.data, sizeof(int)*size);

           if(datas0.data==NULL)
           {
               fprintf(stderr, "Error in realloc\n",(strerror(errno)));
           }
           //void* realloc (void* ptr, size_t size);  成功分配内存后 ptr 将被系统回收，一定不可再对 ptr 指针做任何操作，包括 free()

           datas_final.data=realloc(datas_final.data, sizeof(int)*size);

           if(datas_final.data==NULL)
           {
               fprintf(stderr, "Error in realloc\n",(strerror(errno)));
           }
       }
    
    }

   datas_final.count=datas0.count;
   datas_all=(datas*)malloc(sizeof(datas)*segments);

   if(datas_all==NULL)
   {
        fprintf(stderr, "Error in malloc datas_all\n",(strerror(errno)));
   }

   for(int i=0;i<segments-1;i++)
   {
       datas_all[i].count=(int)(datas0.count/segments);
   }

   datas_all[segments-1].count=datas0.count-datas_all[0].count*(segments-1);

   for(int i=0;i<segments;i++)
   {
      datas_all[i].data=(int*)malloc(sizeof(int)*datas_all[i].count);
    
      if(datas_all[i].data==NULL)
      {
        printf("Error in malloc datas_all[%d].data: %s\n",i,(strerror(errno)));//可以么？
      } 
      
      for(int j=0;j<datas_all[i].count;j++)
      {
        datas_all[i].data[j]=datas0.data[i*datas_all[0].count+j];
      }

   }
}

void *sorter(void *args)
{
   int k=0; 
   int temp=0;
   datas local_datas;

   //没这句就有问题，但之前没注意这句。并且这是数值版，不是指针版。见12章timeout.c
   local_datas=(datas)(*(( datas *)args));

   for(int i=0;i<local_datas.count-1;i++)//选择排序法
   {
      k=i;

      for(int j=i+1;j<local_datas.count;j++)
      {
          
          if(local_datas.data[j]<local_datas.data[i])
          {
             temp=local_datas.data[i];
             local_datas.data[i]=local_datas.data[j];
             local_datas.data[j]=temp;
          }

      }

   }

}

void *merger(void *args)
{
   
   int num=segments;
   //一开始middle和final用的malloc，第二轮对middle赋值就赋值一个，长度少一个,why？
   int middle[datas0.count];
   int final[datas0.count];
   //warning: excess elements in array initializer
   //int final[datas0.count]={0};
   for(int i=0;i<datas0.count;i++)
   {
      final[i]=datas_final.data[i];
      middle[i]=0;
   }
   
   int group=datas_all[0].count;
   int mid=datas_all[0].count;
   int end=0;
   int start1=0;
   int start2=datas_all[0].count;
   int item=0;

   while(num>1)
   {

       for(int i=0;i<num/2;i++)
       {
          if(datas_all[segments-1].count-datas_all[0].count>0 && i==(num/2-1))
          {
             end=datas0.count;
          }
          else
          {
             end=(mid+group)<datas0.count?(mid+group):datas0.count;
          }
          
          while(start1<mid && start2<end)
          {
             middle[item++]=final[start1]<final[start2]?final[start1++]:final[start2++];
          }

          while(start1<mid)
          {
              middle[item++]=final[start1++];
          }

          while(start2<mid+group && start2<datas0.count)
          {
              middle[item++]=final[start2++];
          }
          
          start1=mid+group;
          start2=mid+2*group;
          mid=mid+2*group;
       }

       for(int i=item;i<datas0.count;i++)
        {
          middle[i]=final[i];
        }

       if(segments==2 && 2*datas_all[0].count<datas0.count)
       {
          middle[2*datas_all[0].count]=final[2*datas_all[0].count];
          num=num/2;
       }
       else if(num%2==1)
       {
          num=num/2+1;
       }
       else
       {
          num=num/2;
       }

       group=group*2;
       start1=0;
       start2=group;
       mid=group;
       item=0;
    
       for(int i=0;i<datas0.count;i++)
       {
         final[i]=middle[i];
         middle[i]=0;
       }
   
   }
   
   for(int i=0;i<datas0.count;i++)
   {
     datas_final.data[i]=final[i];
   }
}

void handle_sorting()
{
   int				err;
   pthread_t tid[segments];

    for(int i=0;i<segments;i++)
    {
       //最后一个参数传地址
       err=pthread_create(&tid[i],0,sorter,&datas_all[i]);

       if (err != 0)
       {
		  printf("pthread_create%d fail:%s\n",i,(strerror(errno)));
       }
       
    }

    for(int i=0;i<segments;i++)
    {
        //https://blog.csdn.net/wushuomin/article/details/80051295
       //不建议分离属性
        err=pthread_join(tid[i],NULL);

        if (err != 0)
        {
		  printf("can't join with thread%d:%s\n",i,(strerror(errno)));
        }
    }

    sleep(2);

    for(int i=0;i<segments;i++)
    {
       for(int j=0;j<datas_all[i].count;j++)
       {
           datas_final.data[datas_all[0].count*i+j]=datas_all[i].data[j];
       }
    }

}

void handle_merge()
{
   int		 err;
   pthread_t tid;

   err=pthread_create(&tid,0,merger,NULL);

   if (err != 0)
   {
		printf("pthread_create fail:%s\n",(strerror(errno)));
   }

   sleep(1);
}

int is_sorted()
{
   for(int i=0;i<datas0.count-1;i++)
   {
       if(datas_final.data[i]>datas_final.data[i+1])
       {
           return false;
       }
   }

   return true;

}

void print_data(int judge)
{
    if(judge==1)
    {
        printf("Data sorted succeed!\n");
    }
    else
    {
        printf("Data sorted failed!\n");
    }
    
    printf("original data:\n");

    for(int i=0;i<datas0.count;i++)
    {
        printf("%d ",datas0.data[i]);
    }

    printf("\n");

    printf("Sorted data:\n");

    for(int i=0;i<datas_final.count;i++)
    {
        printf("%d ",datas_final.data[i]);
    }

    printf("\n");
}

void output_data(char* filename)
{
    FILE* fp;

    fp=fopen(filename,"a+");
    fprintf(fp,"\n");

    for (int i = 0; i < datas_final.count-1; i++) 
    {
       fprintf(fp,"%d,",datas_final.data[i]);
    }

    fprintf(fp,"%d,",datas_final.data[datas_final.count-1]);
    fprintf(fp,"\n");
}

void cleanup()
{
   free(datas0.data);
   free(datas_final.data);

   for(int i=0;i<segments;i++)
   {
       free(datas_all[i].data);
   }

   free(datas_all);
}

void main()
{
    char filename[10];
    printf("input segments:");
    scanf("%d",&segments);
    getchar();
    printf("input data name:");
    fgets(filename,10,stdin);
    int len=strlen(filename);
    filename[len-1]='\0';
    input_data(filename);
    handle_sorting();
    handle_merge();
    int judge=is_sorted();
    print_data(judge);
    output_data(filename);
    cleanup();

    //gcc -lm -lpthread -o handle_sorting handle_sorting.c

}