#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here


int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    int user_str_len = 0;  
    int prevSpace = 1;

    while (*user_str != '\0') {
        if (user_str_len >= BUFFER_SZ) {
            return -1;
        }
        if (*user_str == ' ' || *user_str == '\t') {
            if (prevSpace != 0) {
                *buff = ' ';
                buff+=1;
                user_str_len+=1;
                prevSpace = 0;
            }
        } else {
            *buff = *user_str;
            buff += 1;
            user_str_len +=1;
            prevSpace = 1;
        }

        user_str+=1;
    }
    while (user_str_len < BUFFER_SZ) {
        *buff = '.';
        buff+=1;
        user_str_len+=1;
    }
    return user_str_len; 
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    int rc = 0;        // Word count
    int in_word = 0;   // Flag to track if we're inside a word

    while (*buff != '.' && *buff != '\0') {
        if (*buff != ' ' && !in_word) {
            rc+=1;           
            in_word = 1;    
        }
        else if (*buff != ' ') {
            in_word = 0;
        }
        buff+=1;  
    }

    return rc;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    /*
        If argv[1] doesn’t exist, then argc would be less than 1. So, this takes that into account 
    */
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    /*
        If less then 3 arguemnts have been provided, automatically remind the user of the input format 
    */ 

    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = (char *)malloc(BUFFER_SZ * sizeof(char));

    if (buff == NULL) {
        // Handle the error
        char errorMessage[] = "Memory allocation failed";
        printf("%s\n", errorMessage);
        exit(99); // Exit the program with a failure code
    }



    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0) {
        if (user_str_len == -1) {
            printf("Error: The user-supplied string is too large.\n");
        } else if (user_str_len == -2) {
            printf("Error: Invalid arguments supplied to setup_buff.\n");
        }
        exit(2); 
    }

    

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'r': 
            int start = 0;
            int end = user_str_len - 1;
            while (start < end) {
                char temp = buff[start];
                buff[start] = buff[end];
                buff[end] = temp;

                start+=1;
                end-=1;
            }
            printf("Reversed: %.*s\n", BUFFER_SZ, buff);
            break;
        

        case 'w':
            char line1[] = "Word Print";
            char line2[] = "----------";
            printf("%s\n", line1);
            printf("%s\n", line2);
            int index = 1;
            int start2 = 0;
            int in_word = 1;
            int length = 0;
            for (int i = 0; i < user_str_len; i++) {
                if (buff[i] == ' ' || buff[i] == '\t' || buff[i] == '.') {
                    if (in_word == 0) {
                        printf("%d. ", index);
                        for (int j = start2; j < i; j++) {
                            printf("%c", buff[j]);
                        }
                        printf(" (%d)\n", length);
                        index++;
                        in_word = 1;
                    }
                } else {
                    if (in_word == 1) {
                        start2 = i;
                        length = 1;
                        in_word = 0;
                    } else {
                        length+=1;
                    }
                }
            }

        if (in_word) {
            printf("%d. ", index);
            for (int j = start2; j < user_str_len; j++) {
                printf("%c", buff[j]);
            }
            printf(" (%d)\n", length);
        }
    
        break;


        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    memset(buff, 0, BUFFER_SZ);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
/*
We base both the buffer and its length to prevent the functions from
being tied to one length variable ( makeing them more resuable and readable),
to double check the size even if we know it'll be 50, to avoid any mistakes when working
 with memory by over or undershotting the size of the buffer.
*/