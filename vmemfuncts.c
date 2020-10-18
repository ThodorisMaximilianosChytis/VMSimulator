#include <stdio.h>
#include "vmemfuncts.h"


int LRU(int f_num,Pageptr* IPT,int pid,int timestamp,char * newpage){
    
    int min=IPT[0]->timestamp;
    int minindex=0;

    for(int i=1; i<f_num; i++){         //find min timestamp
        if ( (IPT[i]->timestamp) < min ){

            min=IPT[i]->timestamp;
            minindex=i;
        }
    }
    IPT[minindex]->pid=pid;             //insert newpage into IPT
    IPT[minindex]->Rbit=1;
    IPT[minindex]->timestamp=timestamp;
    strcpy(IPT[minindex]->page,newpage);

    return minindex;
}

void WSUpdate(int f_num,Pageptr *IPT,int pid,int count,char *newpage,int window){
    int j;
    int free=-1;
    for (j=0;j<f_num;j++){
        if(IPT[j]->pid==pid){ 
            IPT[j]->Rbit=0;      ////Simulation
            if (IPT[j]->Rbit==0 && count-IPT[j]->timestamp>window){
                IPT[j]->pid=-1;
                strcpy(IPT[j]->page,"fffff");
                IPT[j]->timestamp=0;
                free=j;
            }
        }
    }
    if (free!=-1){
        IPT[free]->pid=pid;
        IPT[free]->Rbit=1;
        IPT[free]->timestamp=count;
        strcpy(IPT[free]->page,newpage);
    }else{
        LRU(f_num,IPT,pid,count,newpage);
    }
}



void IPTManager(Pageptr* IPT,char* file,int f_num,int* rcount,int* wcount,int Process,int count,int* PAGEFAULTS,char * algorithm,int window){
    

    char c;
    char linetemp[15];
    strncpy(linetemp,file,5);       //p#
    linetemp[6]='\0';
    c=file[9];
    if (c=='R')                         
        (*rcount)++;                //Number of reads
    else
        (*wcount)++;                //Number of writes

    int j=0;
    int found=-1;               
    int foundfree=-1;
    for(j=0;j<f_num;j++){
        
        if (IPT[j]->pid==-1){           //found free IPT section
            foundfree=j;
        }
        else if(strcmp(IPT[j]->page,linetemp)==0 && IPT[j]->pid==Process){
            found=j;
            IPT[j]->timestamp=count;       //found thee p# in the table
            IPT[j]->Rbit=1;  
        }
      }

    if (foundfree!=-1 && found==-1){    //there is an emptysection in IPT and IPT and didnt fint the page in table
        (*PAGEFAULTS)++;
        strcpy(IPT[foundfree]->page,linetemp);  //Insert into table
        IPT[foundfree]->Rbit=1;
        IPT[foundfree]->timestamp=count;
        IPT[foundfree]->pid=Process;
    }else if(found==-1){    //full IPT and no match
            (*PAGEFAULTS)++;
        if (strcmp(algorithm,"LRU")==0){

            LRU(f_num,IPT,Process,count,linetemp); 

        }else{

            WSUpdate(f_num,IPT,Process,count,linetemp,window);  

        }
    }

}
