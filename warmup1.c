#define _GNU_SOURCE
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/stat.h>
#include "cs402.h"
#include "my402list.h"
#define _MY402LIST_H_
#include <time.h>
#define DESCMAX 24
#define DESCWIDTH 24
#define AMOUNTMAX 15



typedef struct Person{
    char *type;
    char *time;
    char *amount;
    char *description;
} Person;

void PrintValues(My402List*);
void CreateTestList(My402List*, Person *);
void BubbleSortForwardList(My402List *);
void BubbleForward(My402List *,My402ListElem **,My402ListElem **);
void HeaderInfo();
void BorderAdding();
void CommaAdding(char amt[],char amount_comma[]);

char c;
char *token;
int my_counter=0;

int main(int argc,char *argv[]){ 
    //time_t current_time =0;
    My402List list;
    struct Person* ptr;
    FILE * fp=NULL;
    char line[1024];
    struct stat dir_check;
    int i=0;

    if(argc != 2 && argc != 3 ){
        fprintf(stderr, "Please enter correct command ./warmup1 sort [tfile]\n");
        exit(1);
    }

    if(strcmp(argv[1],"sort") != 0){
        fprintf(stderr, "Please enter correct command ./warmup1 sort [tfile]\n");
        exit(1);
    }

    if(argc == 2){
        fp = stdin;
    }


   else if(argc == 3){
    fp = fopen(argv[2], "r");
    if(fp == NULL){
            fprintf(stderr, "File \"%s\" not found\n",argv[2]);
            exit(1);
        }

        stat(argv[2], &dir_check);
        
        if(S_ISDIR(dir_check.st_mode)){
           fprintf(stderr, "\"%s\" is a directory\n",argv[2]);
            exit(1);
        }


    }
    //fp = fopen("tfile", "r");
     
        
    

memset(&list, 0, sizeof(My402List));
(void)My402ListInit(&list);


    while (fgets(line,sizeof(line), fp) != NULL) {
        if(strlen(line)<10){
            break;
        }   
    if(strlen(line)>1024){
        fprintf(stderr,"Line exceeds 1024 characters\n");
            exit(1);
    } 
    if(strlen(line)==0){
        fprintf(stderr,"Line is empty. Please check the file.\n");
            exit(1);
    }
/*    char *tab_ptr = strchr(line,'\t');
    int tab_count=0;
    while (tab_ptr != NULL){
            *tab_ptr++ = '\0';
            tab_count++;
            tab_ptr = strchr(line,'\t');
    }
    printf("Tab %d\n",tab_count);*/
/*    if(tab_count != 3){
        fprintf(stderr,"Invalid data!\n");
        exit(1);
        }*/
    ptr = (struct Person*) malloc (sizeof(Person));
    i=0;    
    const char s[4] = "\t\n"; 
    char* tok;
    tok = strtok(line, s); 
    my_counter=my_counter+1;
    //printf("Tab %d\n",my_counter);

    while (tok != 0) { 
        /*printf(" %s\n", tok);*/ 
        if(i==0){
            ptr->type=(char *) malloc (strlen(tok)*sizeof(char));
            if(strcmp(tok,"+") != 0 && strcmp(tok,"-") != 0){
                fprintf(stderr,"Invalid Transaction Type in line %d, missing + or -\n",my_counter);
                exit(0);
            }
            else
                strcpy(ptr->type,tok);
        }

        else if(i==1){
            ptr->time=(char *) malloc (strlen(tok)*sizeof(Person));
            time_t current_time =0;
            double input_time=0;
            input_time = atoi(tok);
            current_time = time(0);
            //printf("Time %ld\n",current_time);
            if (strlen(tok) >= 11){
                fprintf(stderr,"Invalid Timestamp in line %d as the length exceeds 10.\n",my_counter);
                exit(0);
            }else if(input_time<=0 || input_time>=current_time){
                fprintf(stderr,"Invalid Timestamp in line %d as the Timestamp exceeds current time.\n",my_counter);
                exit(0);
            }
            strcpy(ptr->time,tok);
        }
            
        else if(i==2){
            ptr->amount=(char *) malloc (strlen(tok)*sizeof(Person));
            int length_decimal=0;
            char *temp_amount=NULL;
            char *decimal_count=NULL;
            decimal_count = tok;
            double amount_check;
            amount_check = atof(tok);
            //printf("%d\n",amount_check);
            temp_amount = strchr(tok,'.');
            if(temp_amount == NULL){
                fprintf(stderr,"No Decimal digit found in the Amount in line %d\n",my_counter);
                exit(0);
            }
            if(amount_check>10000000){
                fprintf(stderr,"Reached Maximum Transaction Amount in line %d\n",my_counter);
                exit(0);
            }
            while(*decimal_count++ != '.')
                length_decimal++;   
            //printf("len %d\n",length_decimal);
            if(strlen(decimal_count)!=2){
                fprintf(stderr,"Two decimal digits not found in line %d\n",my_counter);
                exit(0);
            }
            if(amount_check<0){
                fprintf(stderr,"Transaction amount should be positive in line %d\n",my_counter);
                exit(0);
            }
            strcpy(ptr->amount,tok);
        }

        else if(i==3){
            ptr->description=(char *) malloc (strlen(tok)*sizeof(Person));
            if(strlen(tok)<=0){
                fprintf(stderr,"Description cant be empty in line %d\n",my_counter);
                exit(0);
            } 
            strcpy(ptr->description,tok);
        }

    tok = strtok(0, s);
        i++;


    } 
//printf("tab %d\n",i);
     if(i<=3){
    fprintf(stderr,"Data absent in line %d\n",my_counter);
    exit(0);
}else if(i>=5){
    fprintf(stderr,"Too many fields in line %d\n",my_counter);
    exit(0);
}
    CreateTestList(&list,ptr);
  }

  if(i<=3){
    fprintf(stderr,"Invalid data in line %d\n",my_counter);
    exit(0);
}else if(i>=5){
    fprintf(stderr,"Too many fields in line %d\n",my_counter);
    exit(0);
}
   fclose(fp);
   //Before
    /*BorderAdding();
    HeaderInfo();
    BorderAdding();
    PrintValues(&list);*/

   //After
   BubbleSortForwardList(&list);
    BorderAdding();
    HeaderInfo();
    BorderAdding();
    PrintValues(&list);
    BorderAdding();
    
   return(0);
}

void PrintValues(My402List *pList)
{
    char bal_comma[20] = {};
    char amount_comma[20] ={};
    float test_amount;
    time_t time_format;
    My402ListElem *elem=NULL;
    Person *ptr;
    int i,j;
    char description_print[DESCWIDTH] = {};
    int desc_len=0;
    double balance=0;
    double temp=0;
    char buffer[50];
    char buffer1[50];
    //int intpart,frac;



    for (elem=My402ListFirst(pList); elem != NULL; elem=My402ListNext(pList, elem)) {
        ptr = elem->obj;

 

        fprintf(stdout,"| ");
        time_format = (time_t)atol((ptr->time));
        char *val=ctime(&(time_format));
        if(val[strlen(val)-1]=='\n')
            val[strlen(val)-1]='\0';
        //strcpy(buf,val);
            for(i =0 ;i<24;i++)
                if(i < 11)
                    val[i] = val[i];
                else
                    val[i] = val[i+9];
        printf("%s",val);
        fprintf(stdout," | ");

        //End of Date






        strncpy(description_print,((ptr->description)),sizeof(description_print));
        desc_len = strlen(description_print);
        //printf("%d\n",desc_len);
        for (j =desc_len; j<=DESCWIDTH-1; j++) {
            description_print[j] = ' ';
        }
        description_print[DESCWIDTH] ='\0';
        printf("%s",description_print);
        fprintf(stdout," | ");

        //End of Description





        test_amount = atof(ptr->amount);
        if(test_amount>9999999){
            fprintf(stdout," ");
            strncpy(amount_comma,"?,???,???.??",sizeof(amount_comma));
            amount_comma[12]= '\0';
            fprintf(stdout,"%s",amount_comma);
            fprintf(stdout," ");
            fprintf(stdout," | ");
        }else{
            CommaAdding(ptr->amount,amount_comma);
        if(strcmp(ptr->type,"-") == 0)
            fprintf(stdout,"(");
        else
            fprintf(stdout," ");
        fprintf(stdout,"%s",amount_comma);
        if(strcmp(ptr->type,"-") == 0)
            fprintf(stdout,")");
        else
            fprintf(stdout," ");
        fprintf(stdout," | ");

        }


        //End of Amount
        




        if(strcmp(ptr->type,"+") == 0){
             balance = balance+atof(ptr->amount);
             sprintf(buffer,"%.2f",balance);
             //printf("%s\n",buffer);
        }
        else{
            balance = balance-atof(ptr->amount);
            sprintf(buffer,"%.2f",balance);
            //printf("%s\n",buffer);
        }



        if(balance > 9999999)
        {
            fprintf(stdout," ");
            strncpy(bal_comma,"?,???,???.??",sizeof(bal_comma));
            bal_comma[12]= '\0';
            fprintf(stdout,"%s",bal_comma);
            fprintf(stdout," ");
            fprintf(stdout," |\n");
        }else{
        if(balance<0)
            fprintf(stdout,"(");
        else
            fprintf(stdout," ");

        temp = atof(buffer);
        if(temp<0){
            sprintf(buffer1,"%.2f",(-1)*(temp));
        }else{
            sprintf(buffer1,"%.2f",temp);
        }
        

        CommaAdding(buffer1,bal_comma);
        fprintf(stdout,"%s",bal_comma);
        if(balance < 0)
            fprintf(stdout,")");
        else
            fprintf(stdout," ");
        
        fprintf(stdout," |\n");

        ///End of Balance
        }
        
       }
    }

void CreateTestList(My402List *pList,Person* ptr)
{
        My402ListElem *elem=NULL;
        Person *temp;
        for (elem=My402ListFirst(pList); elem != NULL; elem=My402ListNext(pList, elem)) {
        temp = elem->obj;
        if(strcmp(temp->time,ptr->time)==0){
            fprintf(stderr,"Duplicate Timestamp value found \n");
            exit(0);
        }
    }
        (void)My402ListAppend(pList, ptr);

    
}
    


void BubbleForward(My402List *pList, My402ListElem **pp_elem1, My402ListElem **pp_elem2)
    /* (*pp_elem1) must be closer to First() than (*pp_elem2) */
{
    My402ListElem *elem1=(*pp_elem1), *elem2=(*pp_elem2);
    void *obj1=elem1->obj, *obj2=elem2->obj;
    My402ListElem *elem1prev=My402ListPrev(pList, elem1);
/*  My402ListElem *elem1next=My402ListNext(pList, elem1); */
/*  My402ListElem *elem2prev=My402ListPrev(pList, elem2); */
    My402ListElem *elem2next=My402ListNext(pList, elem2);

    My402ListUnlink(pList, elem1);
    My402ListUnlink(pList, elem2);
    if (elem1prev == NULL) {
        (void)My402ListPrepend(pList, obj2);
        *pp_elem1 = My402ListFirst(pList);
    } else {
        (void)My402ListInsertAfter(pList, obj2, elem1prev);
        *pp_elem1 = My402ListNext(pList, elem1prev);
    }
    if (elem2next == NULL) {
        (void)My402ListAppend(pList, obj1);
        *pp_elem2 = My402ListLast(pList);
    } else {
        (void)My402ListInsertBefore(pList, obj1, elem2next);
        *pp_elem2 = My402ListPrev(pList, elem2next);
    }
}


void BubbleSortForwardList(My402List *pList)
{
    My402ListElem *elem=NULL;
    int i=0;
    Person *ptr,*ptr1;

    for (i=0; i < pList->num_members; i++) {
        int j=0, something_swapped=FALSE;
        My402ListElem *next_elem=NULL;

        for (elem=My402ListFirst(pList), j=0; j < pList->num_members-i-1; elem=next_elem, j++) {
            ptr=elem->obj;
            int cur_val=(atoi)(ptr->time), next_val=0;

            next_elem=My402ListNext(pList, elem);
            ptr1=next_elem->obj;
            next_val = (atoi)(ptr1->time);

            if (cur_val > next_val) {
                BubbleForward(pList, &elem, &next_elem);
                something_swapped = TRUE;
            }
        }
        if (!something_swapped) break;
    }
}

void BorderAdding(){
    int j =0;
    
    for (j=1; j<=80; j++) {
        if(j == 1 || j == 19 || j == 46 || j == 63){
            fprintf(stdout,"+");
        }
        else if(j == 80){
            fprintf(stdout,"+\n");
        }
        else{
            fprintf(stdout,"-");
        }
        
    }
    
}

void HeaderInfo() {
    
    fprintf(stdout,"|");
    fprintf(stdout,"       Date      |");
    fprintf(stdout," Description              |");
    fprintf(stdout,"         Amount |");
    fprintf(stdout,"        Balance |\n");
    
}


void CommaAdding(char amt[],char amount_comma[]){
    int i =11,length=0,count =0;
    
    length = strlen(amt)-1;
    while(i>=0){
        if (length >= 0) {
            amount_comma[i] = amt[length--];
            count++;
        }else{
            amount_comma[i] = ' ';
            
        }
        if(((count-3) % 3)==0 && count-3 > 0 && length+1 >0)
            amount_comma[--i] = ',';
        i--;
    }
    amount_comma[12]= '\0';
}
