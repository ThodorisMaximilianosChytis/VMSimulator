#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vmemfuncts.h"



int main(int argc, char *argv[]){

  int f_num,q,max,window;
  char algorithm[4];
  if (argc!=6 && argc!=5){    
    printf("Wrong Input\n");
    exit(1);
  }
  else{
    printf("THE PAGE REPLACEMENT ALGOIRTHM IS %s\n",argv[1]);
    strcpy(algorithm,argv[1]);
    if (strcmp(algorithm,"WS")!=0 && strcmp(algorithm,"LRU")!=0){
      printf("wrong algorithm input\n");
      exit(1);
    }
    f_num=atoi(argv[2]);
    q=atoi(argv[3]);
    max=atoi(argv[4]);
      if(strcmp(algorithm,"WS")==0 && argc==5){
        printf("error,not enough arguments\n");
        exit(1);
      }
      if(strcmp(algorithm,"WS")==0 && argc==6){
        window=atoi(argv[5]);
        if (window>=f_num){
          printf("error,WS window has to be smaller than frame number for IPT\n");
          exit(1);
        }
      }else
        window=0;
  }



  FILE *fp1, *fp2 , *entries ;

  if ( (fp1=fopen("bzip.trace","r") ) == NULL){
    printf("Error opening file bzip\n");
    exit(1);
  }


  if ( (entries=fopen("entries.txt","w+") ) == NULL){
    printf("Error opening entry file\n");
    exit(1);
  }

  
  if ( (fp2=fopen("gcc.trace","r") ) == NULL){
     printf("Error opening file gcc\n");
     exit(1);
  }

  char file1[15];
  char file2[15];


  Pageptr* IPT;


  IPT=(Pageptr*)malloc(f_num*sizeof(Pageptr));    //Allocate IPT
    if (IPT==NULL){
        printf("Error allocating IPT\n");
        return -1;
    }
    int i;
    for(i=0; i<f_num; i++){
      IPT[i]=(Pageptr)malloc(sizeof(Page));     //Initialize IPT
      if (IPT==NULL){
        printf("Error allocating IPT[i]\n");
        return -1;
      } 
      IPT[i]->Rbit=0;
      IPT[i]->pid=-1;
      IPT[i]->page=(char*)malloc(6*sizeof(char));
      strcpy(IPT[i]->page,"fffff");
      IPT[i]->timestamp=0;

      // printf("Printing the frames\n");
      // printf("%d ",IPT[i]->Rbit);
      // printf("%d ",IPT[i]->pid);
      // printf("%d ",IPT[i]->timestamp);
      // printf("%s\n",IPT[i]->page);

    }




  int rcount=0;
  int wcount=0;
  
  int count=0;
  int j=0;
  int status1, status2;
  int Process=1;
  int PAGEFAULTS=0;

  do{
    i=0;
    while(status1=(fgets(file1,sizeof(file1),fp1)!=NULL) && count<max ){  //Process 1 reading from bzip for q times
      count++; 
      fprintf(entries,"Process 1 examines : %s",file1);
      IPTManager(IPT,file1,f_num,&rcount,&wcount,1,count,&PAGEFAULTS,algorithm,window);
      
      i++;
      if(i==q)
        break;
    }
    
    i=0;
    Process=2;
    while(status2=(fgets(file2,sizeof(file2),fp2)!=NULL) && count<max){  //Process 2 reading from gcc for q times
      count++;
      fprintf(entries,"Process 2 examines : %s",file2);
      IPTManager(IPT,file2,f_num,&rcount,&wcount,2,count,&PAGEFAULTS,algorithm,window);

      i++;
      if(i==q)
        break;      
    }    


}while(count<max && (status2!=0 && status1!=0));    //repeat while max number of Traces to bre read are done
                                                    //or until both files reach EOF


printf("Number of reads is %d\n",rcount);
printf("Number of writes is %d\n",wcount);
printf("Number of PAGEFAULTS is %d\n",PAGEFAULTS);
printf("Frame Number used is %d\n",f_num);
printf("In the end the IPT looks like this:\n");
printf("Refbit|pid|timestamp|page |\n");
printf("---------------------------\n");
for(i=0;i<f_num;i++){
    printf("|  %d  |",IPT[i]->Rbit);
    printf(" %d |",IPT[i]->pid);
    printf("    %d    |",IPT[i]->timestamp);
    printf("%s| \n",IPT[i]->page);
}


for(i=0;i<f_num;i++){     //Deallocate IPT
  free(IPT[i]->page);
  free(IPT[i]);

}
free(IPT);





fclose(fp1);
fclose(fp2);
fclose(entries);
}