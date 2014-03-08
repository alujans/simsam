#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "common/parser.h"
#define BLANK ' '
#define MARK '$'
#define COME ','
#define PARENTHESIS '('

int get_type(char * line);
int ins_arith_params(char * line, struct instruction * instruction);
int ins_memacc_params(char * line, struct instruction * instruction);
void insert_nop(struct instruction * instruction);

int parse_instruction(char * line, struct instruction * instruction){

    if( !line ) return -3;
    char * spointer = line;
    int type;
    int result;

    //Skip initial blanks
    while( *spointer == BLANK ) spointer++;

    //Check instruction type
    type = get_type(spointer);
    instruction->codop = type;
    switch(type){
        case 0:case 1:case 2:
            result = ins_arith_params(line, instruction);
            break;
        case 3:case 4:
            result = ins_memacc_params(line, instruction);
            break;
        case 5:case 6:
            instruction->args[0]=0;
            instruction->args[1]=0;
            instruction->args[2]=0;
            result=0;
            break;
        default:
            //Wrong syntax, nop operation instead
            insert_nop(instruction);
            result=type;
            break;
    }
    return result;
}

int get_type(char * line){ 

    char * tmp_line = malloc(strlen(line)+1);
    strcpy(tmp_line, line);
    char * spointer = tmp_line;

    if((spointer = strchr(spointer, BLANK))){
        *spointer++='\0';

        if(strcmp(tmp_line, "add")==0){ free(tmp_line);return 0;}
        if(strcmp(tmp_line, "sub")==0){ free(tmp_line);return 1;}
        if(strcmp(tmp_line, "mul")==0){ free(tmp_line);return 2;}
        if(strcmp(tmp_line, "lw")==0){ free(tmp_line);return 3;}
        if(strcmp(tmp_line, "sw")==0){ free(tmp_line);return 4;}

        free(tmp_line); 
        return -1; 
    }
    
    spointer = tmp_line;
    spointer +=strlen(tmp_line)-1; 
    *spointer='\0';

    if(strcmp(tmp_line, "nop")==0){ free(tmp_line);return 5;}
    if(strcmp(tmp_line, "trap")==0){ free(tmp_line);return 6;}

    free(tmp_line);
    return -3;
}

int ins_arith_params(char * line, struct instruction * instruction){

    int count;
    char * spointer;

    spointer=line;
    count=0;
    while((spointer = strchr(spointer, MARK)) && count < 3){
        spointer+=2;
        line=spointer++;
        *spointer++='\0'; 
        instruction->args[count]=atoi(line);
        count++;
    }
    //Wrong syntax -> NOP
    if(count < 2){
        insert_nop(instruction);
        return -1;
    }
    return 0;
}

int ins_memacc_params(char * line, struct instruction * instruction){
    

    char * spointer;

    if(!(spointer=strchr(line, MARK))){
        insert_nop(instruction);
        return -1;
    } 

    spointer+=2;
    line=spointer;
    spointer++;
    *spointer++='\0';
    instruction->args[0]=atoi(line);

    line = spointer;
    if(!(spointer = strchr(spointer, PARENTHESIS))){
        insert_nop(instruction);
        return -1;
    }

    *spointer++='\0';
    instruction->args[2]=atoi(line);

    if(!(spointer=strchr(spointer, MARK))){
        insert_nop(instruction);
        return -1;
    } 

    spointer+=2;
    line=spointer;
    spointer++;
    *spointer='\0';
    instruction->args[1]=atoi(line);

    return 0;

}

void insert_nop(struct instruction * instruction){
    instruction->codop=5;
    instruction->args[0]=0;
    instruction->args[1]=0;
    instruction->args[2]=0;
} 
