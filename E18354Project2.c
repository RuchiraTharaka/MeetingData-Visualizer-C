/*This program prints a graph using given data*/

#include <stdio.h>      //Including necessary header files
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct node{      //Creating a structure called List to store filenames
    char filename[30];
    struct node * address;
}List;
List *filelistP,*tailP;   //Creating two pointers of created data type


typedef struct data{      //Creating a structure called meeting to create the final linked list which is needed to the graph
    char name[80];
    int count;
    struct data* next;
}meeting;


typedef struct datas{      //Creating a structure called Meeting to store meeting data taken from the files
    char name[80];
    int count;
    struct datas* nextsameletter;
    struct datas* next;
}Meeting;


int*total;              //Creating an integer variable to keep track of the total count
int*value;
int rows=10;              //Creating an integer variable to store the number of rows in the graph (default is 10)
char*programName = NULL;  //Creating a pointer to point at the program name given as a argument
Meeting*lastMeetingP;     //Creating Meeting pointer to point at the last element of the linkedlist
char*Name,*count,*hours;

void Cannot(void);              //Initializing the Cannot() function
void CannotOpen(void);          //Initializing the CannotOpen() function
void Invalid(char[]);           //Initializing the Invalid() function
void Invalidoption(void);       //Initializing the Invalidoption() function
void Enough(void);              //Initializing the Enough() function
void Only(void);                //Initializing the Only() function
void Noinput(void);             //Initializing the Noinput() function
void Negative(void);            //Initializing the Negative() function
void wrong(void);               //Initializing the wrong() function
void Nodata(void);              //Initializing the Nodata() function
int isNumber(char[]);           //Initializing the isNumber() function
int isCSV(char[]);              //Initializing the isCSV() function
int CSV(int,int);               //Initializing the CSV() function
int Store(char*,int,Meeting*); //Initializing the Store() function
void LoopPrinting(char[],int);   //Initializing the LoopPrinting() function
void Bar(char[],int,int,int);    //Initializing the Bar() function


int main(int argc,char *argv[]){    //Take inputs as arguments
    total = (int*)malloc(sizeof(int));      //Allocate memory
    *total = 0;                             //Set the value to 0
    programName = argv[0];  //Assign programname to the pointer
    enum O{m1, m, p, t, cannot, cannotOpen, invalid, invalidoption, negative, enough, only, zero};  //define different output values
    //(m1-default m-meetings p-participants t-time)
    enum O output = m1;     //Set output value to default (to sort by number of meetings)
    enum S{no, yes};        //define enum S which stores only yes and no (to use in different places in the code as flags)
    enum S scaled = no, takerows = no, handled = no;    //Create S variables (scaled-is graph a scaled one?  takerows-should I take the number of rows?   handled-argument handling is done? )
    int ArgNo = 1;              //Create an integer variable to count arguments and set it to the beginning
    int error_option;           //Create an integer variable to store wrong entries
    FILE * filepointer;         //Create a FILE pointer to handle given files

    filelistP = (List*)malloc(sizeof(List));    //Allocate memory to a List item and assign it to a pointer(this is a fixed head of linked list which contains given filenames)
    tailP = filelistP;                          //Tail is a pointer and it points at the end of the linked-file list
    strcpy(filelistP -> filename,"");           //This first element in the list does not contain a file name. It just behaves like a head.
    filelistP -> address = NULL;

    //Argument handling
    while(ArgNo<argc && (!handled)){                //Till the last argument or till handled=yes (argument handling is done at the middle may be due to an wrong argument)
        if(!strncmp(argv[ArgNo],"-",1)){            //If argument starting with '-'
            if(!strcmp(argv[ArgNo],"-p")){          //If the argument is '-p'
                if(output==t || output==m){         //If there was a argument '-m' or '-t' earlier
                    output = cannot;                //Set output to send a error message "Cannot plot multiple ..."
                }else if(output==m1){               //If there was not any argument earlier mentioning a parameter
                    output=p;                       //Set output to sort users by the number of participants
                }
            }else if(!strcmp(argv[ArgNo],"-t")){    //If the argument is '-t'
                if(output==p || output==m){         //If there was a argument '-p' or '-m' earlier
                    output = cannot;                //Set output to send a error message "Cannot plot multiple ..."
                }else if(output==m1){               //If there was not any argument earlier mentioning a parameter
                    output=t;                       //Set output to sort users by the meeting duration
                }
            }else if(!strcmp(argv[ArgNo],"-m")){    //If the argument is '-m'
                if(output==t || output==p){         //If there was a argument '-t' or '-p' earlier
                    output = cannot;                //Set output to send a error message "Cannot plot multiple ..."
                }else if(output==m1){               //If there was not any argument earlier mentioning a parameter
                    output=m;                       //Set output to sort users by the number of meetings
                }
            }else if(!strcmp(argv[ArgNo],"--scaled")){    //If the argument is '--scaled'
                scaled = yes;                             //Set value of 'scaled' variable to yes(1)
            }else if(!strcmp(argv[ArgNo],"-l")){                //If the argument is '-l'
                if(ArgNo+1!=argc){                              //If there are more arguments
                    if(!isNumber(argv[ArgNo+1])){               //If next arguments is a not a number
                        if(!strncmp(argv[ArgNo+1],"-",1)){      //If next argument starts with '-'
                            if(isNumber(argv[ArgNo+1]+1)){      //If the rest of the next argument is a number (it is checked by using the isNumber() function) (If this is yes, it means next argument is a negative number or -0)
                                if(atoi(argv[ArgNo+1]+1)==0){   //If that number is 0,
                                    output = zero;              //Set output to do nothing but terminate the program
                                    handled = yes;              //Flagging that argument handling is done
                                }else{                          //If the next argument is a negative number
                                    output = negative;          //Set output to send an error message "Invalid option(negative) for ..."
                                    handled = yes;              //Flagging that argument handling is done
                                }
                            }else{                              //If the rest after '-' is not a number (eg:- -abc)
                                output = invalidoption;         //Set output to send an error message "Invalid options for ..."
                                handled=yes;                    //Flagging that argument handling is done
                            }
                        }else{                                  //If next argument does not starts with '-'
                            output = invalidoption;             //Set output to send an error message "Invalid options for ..."
                            handled=yes;                        //Flagging that argument handling is done
                        }
                    }else{                                      //If next argument is a number
                        if(output<4){                           //If output is set to execute the graph (output is not set to send an error message)
                            takerows = yes;                     //Flagging that next argument can be taken as number of rows
                        }
                    }
                }else{                                          //If there are no more arguments (this '-l' is the last argument)
                    output = enough;                            //Set output to send an error message "Not enough options for ..."
                }
            }else{                                      //If the argument is not a specified one with '-'(eg:- -dog, -1234)
                output = invalid;                       //Set output to send an error message "Invalid option [dog] ..."
                error_option = ArgNo;                   //Store the argument in the created variable to send with the error message
                handled = yes;                          //Flagging that argument handling is done
            }
        }else if(isNumber(argv[ArgNo])){            //If the argument is a number
            if(takerows==yes){                      //If flag is flagging to take this as number of rows (if previous argument is '-l')
                if(atoi(argv[ArgNo])==0){           //If the number is 0
                    output = zero;                  //Set output to do nothing but terminate the program
                    handled = yes;                  //Flagging that argument handling is done
                }else{                              //If the argument is a non-zero number
                    rows = atoi(argv[ArgNo]);       //Take the argument as number of rows
                    takerows = no;                  //Reset the flagging variable to not to take rows
                }
            }else if(strcmp(argv[ArgNo-1],"-l")){   //If last argument is not '-l'
                if(output<4){                       //If output is set to execute the graph (output is not set to send an error message)
                    output = only;                  //Set output to send an error message "Only .csv files ..."
                }
            }
        }else if(isCSV(argv[ArgNo])){                                   //If argument is '***.csv'
            if(output<4){                                               //If output is set to execute the graph (output is not set to send an error message
                filepointer = fopen(argv[ArgNo],"rb");                  //Try to open the file
                if(filepointer){                                        //If file opens
                    fclose(filepointer);                                //Close the file
                    tailP -> address = (List*)malloc(sizeof(List));     //Create a new List element and link it to the tail of the Filelist
                    tailP = tailP->address;                             //Update the tail pointer
                    strcpy(tailP -> filename, argv[ArgNo]);             //Store the file name in the element
                    tailP -> address = NULL;                            //Set the pointer in the element to NULL
                }else{                              //If there is no such a file
                    output = cannotOpen;            //Set output to send an error message "Cannot open one or ..."
                }
            }
        }else{
            if(output<4){                   //If the argument is not a specified one (eg:- dog, 1234)
                output = only;              //Set output to send an error message "Only .csv files ..."
            }
        }
        ++ArgNo;        //Jump to the next argument
    }
//Argument handling is done

    if (output<4){                      //If output is set to execute the graph ( m1(by number of meetings as default) / m(by number of meetings) / p(by number of participants) / t(by duration))
        if(filelistP->address==NULL){   //If no **.csv files has given as inputs (if linked list is empty)
            Noinput();                  //Send an error message "No input files ..." using Noinput() function
        }
        else{                           //If **.csv files has given
            CSV(output,scaled);         //Start the graphing process using CSV() function (set output m1,m,p or t and the scaled value (whether graph should scaled or not) is given as arguments)
        }
    }else if(output == cannot){         //If output is set to send an error message "Cannot plot multiple ..."
        Cannot();                       //Send it using Cannot() function
    }else if(output == cannotOpen){     //If output is set to send an error message "Cannot open one or ..."
        CannotOpen();                   //Send it using CannotOpen() function
    }else if(output == invalid){        //If output is set to send an error message "Invalid option ..."
        Invalid(argv[error_option]);    //Send it using CannotOpen() function
    }else if(output == invalidoption){  //If output is set to send an error message "Invalid options for ..."
        Invalidoption();                //Send it using CannotOpen() function
    }else if(output == enough){         //If output is set to send an error message "Not enough options for ..."
        Enough();                       //Send it using CannotOpen() function
    }else if(output == only){           //If output is set to send an error message "Only .csv files ..."
        Only();                         //Send it using CannotOpen() function
    }else if(output == negative){       //If output is set to send an error message "Invalid option(negative) for ..."
        Negative();                     //Send it using CannotOpen() function
    }                                   //If output = zero, No output and terminate

    return 0;             //Terminate the program
}

void Cannot(void){          //This function prints an error message when multiple parameters are given (eg:- -t file.csv -p)
    printf("Cannot plot multiple parameters in same graph.\nusage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",programName);
}


void CannotOpen(void){      //This function prints an error message when given files cannot be opened
    printf("Cannot open one or more given files\n");
}


void Invalid(char word[]){  //This function prints an error message when wrong options are given (eg:- -xyz, -124)
    printf("Invalid option [%s]\nusage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",word,programName);
}


void Invalidoption(void){   //This function prints an error message when the argument after the '-l' is not a positive number (eg:- -l ab4)
    printf("Invalid options for [-l]\nusage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",programName);
}


void Enough(void){          //This function prints an error message when no argument is given after '-l'
    printf("Not enough options for [-l]\nusage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",programName);
}


void Only(void){            //This function prints an error message when not specified arguments are given (eg:- abc, 124(without '-l'))
    printf("Only .csv files should be given as inputs.\n");
}


void Negative(void){        //This function prints an error message when negative number is given after '-l'
    printf("Invalid option(negative) for [-l]\nusage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",programName);
}


void Noinput(void){         //This function prints an error message when no .csv files are given
    printf("No input files were given\nusage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",programName);
}


void wrong(void){           //This function prints an error message when given files contains wrong entries
    printf("File/s contain wrong entries.\n");
}


void Nodata(void){          //This function prints an error message when no data is in given files
    printf("No data to process\n");
}


int isNumber(char word[]){      //This function checks whether a given string is a number or not
    int length=strlen(word);    //Take the length of the string
    int i=0;
    while(i<length){            //Checking letter by letter
        if(!isdigit(word[i])){  //If there is a non-numerical letter,
            return 0;           //Returns 0
        }
        ++i;
    }
    return 1;                   //If there are no non-numerical letters found, return 1
}


int isCSV(char word[]){                 //This function checks whether a given string contains .csv extension
    char*csvP = &word[strlen(word)-4];  //Create a pointer to point at the '.' in the string (if there is)
    if(!strcmp(csvP,".csv")){           //Checking whether the rest in the string is .csv or not
        return 1;                       //If it is so, return 1
    }else{
        return 0;                       //Otherwise return 0
    }
}


int CSV(int option,int Scaled){
    //Reading given files and create a linked list contains the information of all the users
    value = (int*)malloc(sizeof(int));                      //Create an integer pointer to take the count of each entry
    Name = (char*)malloc(80);                         //Create a char pointer to store the address of the name taken out from the entry
    count = (char*)malloc(20);                        //Create a char pointer to store the address of the count taken out from the entry
    hours = (char*)malloc(20);                        //Create a char pointer to store the address of the hours taken out from the entry
    Meeting* meetingP = (Meeting*)malloc(sizeof(Meeting));  //Creating two Meeting elements and they behaves as the tail of the list
    strcpy(meetingP->name,"");                              //Give some null values to the variables
    meetingP->count = 0;
    meetingP->nextsameletter = NULL;
    meetingP->next = (Meeting*)malloc(sizeof(Meeting));     //Link them
    strcpy(meetingP->next->name,"");
    meetingP->next->count = 0;
    meetingP->next->nextsameletter = NULL;
    meetingP->next->next = NULL;                            //Set the 'next' of the second elements as NULL
    lastMeetingP = meetingP->next;                          //Assign the address of the tail of the list to the pointer
    char* buffer=(char*)malloc(150);                                       //Create a buffer to store the line
    FILE * filepointer;                                     //Create a FILE pointer
    List*deleteitem, *pointer=filelistP->address;           //Create two List pointers to delete elements and access elements
    while(pointer!=NULL){                                   //Until the last element
        filepointer = fopen(pointer->filename,"r");         //Open the file load the content to the memory
        deleteitem = pointer;                               //Assign the elements to the pointer 'deleteitem'
        pointer=pointer->address;                           //Jump to the next element
        free(deleteitem);                                   //Free the last element
        while(fgets(buffer,150,filepointer) != NULL){       //Read line by line
            if(Store(buffer,option,meetingP)){              //Store the data inside the line by Store() function (If it contains wrong entries, function returns 1)
                fclose(filepointer);                        //If function returned 1, close the opened file
                wrong();                                    //Send an error message "file/s contain wrong ..." using wrong() function
                return 0;                                   //Return 0 and exit the function
            }
        }
        fclose(filepointer);                                //If the file content is ok, close the file
    }
    free(filelistP);                                        //Free the head of the list (So now, the whole memory in the linked-list 'filelist' is freed)
    free(value);                                            //Free unwanted memory
    free(Name);
    free(count);
    free(hours);

    //Create a linked-list of given number of elements(rows)
    if(*total!=0){                                   //If the count total is not zero, execute the graph
        meeting*filtered_list = (meeting*)malloc(sizeof(meeting));  //Create a meeting and a pointer to behave as the tail of the list
        filtered_list->count=0;                                     //Give some NULL values to the fields
        strcpy(filtered_list->name,"");
        filtered_list->next=NULL;
        meeting*Pointer = filtered_list, *lastelement;              //Create two pointers to access elements and to keep track of the second last element
        for(int i=0;i<rows;i++){
            Pointer->next=(meeting*)malloc(sizeof(meeting));        //Create the linked-list
            lastelement = Pointer;
            Pointer=Pointer->next;
            Pointer->count=0;
            strcpy(Pointer->name,"");
            Pointer->next=NULL;
        }
        //Add data to the list
        Meeting*element_to_free=meetingP->next;     //Create an meeting pointer
        for(Meeting*element=meetingP->next->next;element!=NULL;element=element->next){  //Thrugh the data list we are having
            free(element_to_free);                                                      //Free the previous element
            element_to_free=element;                                                    //Assign this element to free in next cycle
            if((element->count)>(lastelement->next->count)){                            //If the count of the element is larger the the count of the last element
                for(meeting*listelement=filtered_list;listelement->next!=NULL;listelement=listelement->next){   //through the linked-list of given size
                    if((listelement->next->count)< (element->count)){                       //If the next count of next element is less than list element
                        lastelement->next->count = element->count;                          //Element should be added here. So the last element is gonna drop. Instead of dropping, change its field as this and add it here
                        strcpy(lastelement->next->name,element->name);
                        if(listelement!=lastelement){                                       //If the next element is the last element no need to drop and add again but let it be there. But if it is not so,
                            lastelement->next->next=listelement->next;                      //Remove the last element from the end and add it here
                            listelement->next=lastelement->next;
                            lastelement->next=NULL;
                            Pointer=listelement;
                            while(Pointer->next!=NULL){                                     //So, the second last element is now changed. So it should be found by this
                                lastelement=Pointer;
                                Pointer=Pointer->next;
                            }
                        }
                    break;                                                                  //If the process is done, break the cycle
                    }
                }
            }
        }
        free(element_to_free);                                   //Free the last element
        free(meetingP);                                          //Free the tail of the data list (So now, whole memory allocated for the data list is freed)

        //Now, the sorted list is set and have to execute the graph
        int Max=filtered_list->next->count;  //Largest count is taken by the first element of the linked-list
        int MaxName=0;                       //create an integer variable to store the length of the longest name

        for(meeting*p=filtered_list->next;p!=NULL;p=p->next){   //Longest name is found by this loop
            MaxName = (strlen(p->name)>MaxName?strlen(p->name):MaxName);
        }

        char Hfrequency[20];                //Create a char array to store the largest count as an string
        sprintf(Hfrequency,"%d",Max);       //This function converts the integer to a string and store
        int MaxLength = strlen(Hfrequency); //Find the length of the largest count

        MaxName = MaxName+2;                        //From left side of the graph, there should be a margin of length of longest name + 2
        int graphsize = 79 - MaxName - MaxLength;   //So, the space dedicated for the bar is 80-1-(length of longest name)-(length of largest count)
        Max = (Scaled?Max:*total);                   //Now on, Max is used to store the value which is considered as the '100%'. So, if it is scaled '100%' is the largest count. Otherwise it is the total count
        free(total);
        printf("\n");                                                                   //Go to a new line
        for(meeting*p=filtered_list->next;p!=NULL;p=p->next){                           //Print bar by bar
            if(strcmp("",p->name)){                                                     //If it is null value containing element, ignore them
                Bar(p->name,MaxName,(int)((float)(p->count)/Max*graphsize),p->count);   //Bar is printed using Bar() function
            }                                                                           //The name of the bar, Left margin, Length of the bar and the count of the bar is given as arguments
        }                                                                               //Length of the bar is calculated by dividing the count by '100%' and multiply by the graph size

        LoopPrinting(" ",MaxName);          //Print the Left side margin
        printf("\u2514");                   //Print the horizontal axis
        LoopPrinting("\u2500",79-MaxName);
        printf("\n");                       //Print the new line


    }else{                                  //If the total count is zero,
        Nodata();                           //Send an error message "No data to ..." using Nodata() funtion
    }

    return 0;                               //Return 0 and exit the function
}


int Store(char*line,int option,Meeting*Address){          //This function takes information from given data line and stores them in linked list element
    //Extract name and the count from entries
    strncpy(Name,line,strcspn(line,","));                   //Copy the name from the line and paste it in the char array using strncpy function (strcspn() function is used to find where the ',' is)
    *(Name+strcspn(line,","))=0;
    if((!strcmp(Name,"")) || (!strcmp(Name,line))){         //If the name is empty or there is no more data in the line other than the name
        return 1;                                           //It is a wrong entry. So, return 1 and exit the function
    }
    strcpy(line,line+strlen(Name)+1);                       //Assign the rest of the line to the same array (without name)
    strncpy(count,line,strcspn(line,","));                  //Copy the participant count from the line and paste it in the char array using strncpy function (strcspn() function is used to find where the ',' is)
    *(count+strcspn(line,","))=0;
    if((!strcmp(count,"")) || (!strcmp(count,line))){       //If the count is empty or there is no more data in the line other than the name and count
        return 1;                                           //It is a wrong entry. So, return 1 and exit the function
    }

    if(option<2){                                 //If the data is sorting by the number of meetings
        *value = 1;                                    //Increment is one
        ++(*total);                                      //Increase the total count by 1
    }else if(option==2){                          //If the data is sorting by the number of participants
        *value = atoi(count);                          //Increment is the participant count taken from the line (atoi() function converts a string to a integer)
        *total = (*total) + (*value);                        //Increase the total count by the the count
    }else if(option==3){                          //If the data is sorting by the duration(time)
        strcpy(line,line+strlen(count)+1);            //Refresh the line (removing the name and the participant count)
        strncpy(hours,line,strcspn(line,":"));        //Copy the number of hours and paste it in the array
        *(hours+strcspn(line,":"))=0;
        strcpy(line,line+strlen(hours)+1);            //Refresh the array (removing the number of hours)
        *value = (atoi(hours)*60)+atoi(line);          //Increment is the duration (number of seconds is ignored)
        *total = (*total) + (*value);                        //Increase the total count by the duration
    }

    int flag = 1;                   //Creating an integer variable to flagging
    //Add count under the relevant name or create new elements
    Meeting*sameletterpointer=NULL,*pointer=Address;                            //Create two Meeting pointers and point one of them to the Address(head of the linked data list)
    do{
        pointer = pointer-> next;
        if(!strcmp(pointer->name,Name)){                //If there is en element with the same name,
            pointer->count=(pointer->count)+(*value);      //Increase the relevant count by this value
            return 0;                                   //Return 0 (flagging that storing is successful) and exit the function
        }else if(!strncmp(pointer->name,Name,1)){       //If the first letter of the Name and the name of the element are same,
            sameletterpointer = pointer;                //Assign the address of the current element to the sameletterpointer
            if((sameletterpointer->nextsameletter)!=NULL){                              //If the there are more elements with the same first letter
                do{
                    sameletterpointer = sameletterpointer-> nextsameletter;
                    if(!strcmp(sameletterpointer->name,Name)){                          //If there is en element with the same name,
                        sameletterpointer->count=(sameletterpointer->count)+(*value);      //Increase the relevant count by this value
                        return 0;                                                       //Return 0 (flagging that storing is successful) and exit the function
                    }
                }while((sameletterpointer->nextsameletter) != NULL);                    //Stop if there are no more elements with the same first letter
                flag=0;                                                                 //Flagging that there are no more elements (But there were)
            }
        }
    }while((pointer->next != NULL)&&flag);              //Find through the list
                                                        //If there is no element with the same name,
    lastMeetingP->next = (Meeting*)malloc(sizeof(Meeting));  //Create a new element and link it to the list
    lastMeetingP = lastMeetingP -> next;                     //Add data to the element
    lastMeetingP -> next = NULL;
    if(sameletterpointer!=NULL){                                      //If there were elements with the same first letter, link this to the last element with the same first letter
        sameletterpointer->nextsameletter = lastMeetingP;
    }
    lastMeetingP ->nextsameletter = NULL;
    strcpy(lastMeetingP->name,Name);
    lastMeetingP->count = *value;
    return 0;                                           //Return 0 (flagging that storing is successful) and exit the function
}


void LoopPrinting(char symbol[],int num){        //This function prints a particular symbol given number of times. Symbol and the number of times is given as arguments
    for(;num>0;num--){
        printf("%s",symbol);
    }
}


void Bar(char name[], int NameWidth, int BarSize,  int Frequency){   //This function  prints the bar including the name, the count and the relevant vertical axis
    //Name of the bar, Left side margin, the bar size should be printed and the count should be printed is given as arguments
    LoopPrinting(" ",NameWidth);        //Print the left margin of the first line of the bar
    printf("\u2502");                   //Print the vertical axis of the first line of the bar
    LoopPrinting("\u2591",BarSize);     //Print the first line of the bar
    printf("\n");                       //Print the new line

    printf(" %s",name);                         //Prints the name
    LoopPrinting(" ",NameWidth-strlen(name)-1); //Prints the left margin of the second line of the bar
    printf("\u2502");                           //Print the vertical axis of the first line of the bar
    LoopPrinting("\u2591",BarSize);             //Print the first line of the bar
    printf("%d\n",Frequency);                   //Print the count and the new line of the bar

    LoopPrinting(" ",NameWidth);        //Print the left margin of the second line of the bar
    printf("\u2502");                   //Print the vertical axis of the second line of the bar
    LoopPrinting("\u2591",BarSize);     //Print the second line of the bar
    printf("\n");                       //Print the new line

    LoopPrinting(" ",NameWidth);        //Print the Left side margin
    printf("\u2502");                   //Print the vertical axis
    printf("\n");                       //Print new line

}

