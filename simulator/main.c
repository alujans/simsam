#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include "common/values.h"
#include "common/types.h"
#include "common/func_units.h"
#include "common/parser.h"


struct instruction ins_memory[IMEMB_SIZE]; 
int register_bank[REGB_SIZE];
int data_memory[DMEMB_SIZE];

struct if_to_id_reg if_to_id_reg={5,0,0,0};
struct id_to_exe_reg id_to_exe_reg={5,0,0,0};
struct exe_to_wb_reg exe_to_wb_reg={0,0,0};


void init_insmem(void);
void init_datamem(void);
void init_register_bank(void);
void print_processor_state(void);
void print_reg_state(void);
void print_mem_state(void);

int main (int argc, char * argv[]){

    init_insmem();
   
    init_datamem();
    init_register_bank();

    int i = 0;
    int control = 0;

    setbuf(stdout,NULL);
    printf("\n\n");
    printf("\t\tSIMSAM Simulator, PIPELINED PROCESSOR\n");
    printf("\t\t=====================================\n\n");

    printf("\n\n\t\tINITIAL PROCESSOR STATE\n\n\n");
    print_processor_state();
    print_mem_state();
    sleep(3);
    
    printf("\n\n\t\tBEGINS SIMULATION\n\n\n");


    while( control!=-1 ){
        
        printf("\t\t   Cycle: %-4d   \n", i+1);
        printf("\t\t-----------------\n\n");

        wb_stage(register_bank, &exe_to_wb_reg);
        
        exe_stage(&id_to_exe_reg, &exe_to_wb_reg, data_memory);

        control = id_stage(&if_to_id_reg, &id_to_exe_reg, register_bank);

        if( control != 10 && control != -1 ){
            if_stage(ins_memory,&if_to_id_reg,i);
            i++;
        }

        print_reg_state();
        //print_processor_state();
        //print_mem_state();
        sleep(3);
    }
   
    //Complete last cyle - remains one stage to be executed when trap is detected
    wb_stage(register_bank, &exe_to_wb_reg);
    printf("\t\t   Cycle: %-4d   \n", i+1);
    printf("\t\t-----------------\n\n");
    print_reg_state();
    print_processor_state();
    print_mem_state();
    sleep(3);

        
    printf("\n\t\t===============================\n");
    printf("\t\tTRAP REACHED, END OF SIMULATION\n");
    printf("\t\t===============================\n\n");
        
    //printf("%d\n",EOF);
    print_processor_state();
    print_mem_state();

    exit(0);
    
}


void init_insmem(void){


    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int parsed;
    int i;

    fp = fopen("program.s", "r");
    if (fp == NULL){
        perror("File was not found or couldn't be opened\n");
        exit(EXIT_FAILURE);
    }

    i=0;
    while ((read = getline(&line, &len, fp)) != -1) {
  
       if((parsed = parse_instruction(line, &ins_memory[i])) == -3){
           printf("ERROR: Compiling line %d, SIMULATION ABORTED\n", i+1);
           if(line) free(line);
           exit(EXIT_FAILURE);
       }
           
       if(parsed == -1)
           printf("WARNING: Instruction syntax error on line %d, inserted NOP operation instead\n", i+1);
       i++;

    }

    if (line) free(line);

}


void init_register_bank(void){

    int i;
    for (i=0; i < REGB_SIZE; i++)
        register_bank[i]=i*10;
} 

void init_datamem(void){

    int i;
    for (i=0; i< DMEMB_SIZE; i++)
        data_memory[i]=i*2;
}

void print_processor_state(void){

    int i;
    printf("\t\t       REGISTER BANK   \n\n");
    printf("\t\t   Register   |   Value   \n");
    printf("\t\t-------------- -----------\n");

    for (i=0; i<REGB_SIZE; i++){
        printf("\t\t   %-12d   %-8d\n", i, register_bank[i]);
    }
    printf("\n\n");
}

void print_reg_state(){

    printf("\t\t   IF_TO_ID   |   ID_TO_ALU   |   ALU_TO_OS   \n");
    printf("\t\t-------------- --------------- ---------------\n");
    printf("\t\t   cod: %-7d   cod: %-8d   w: %-9d\n", if_to_id_reg.codop, id_to_exe_reg.codop, exe_to_wb_reg.w);
    printf("\t\t   r1: %-8d   rdest: %-6d   dest: %-6d\n", if_to_id_reg.r1, id_to_exe_reg.rdest, exe_to_wb_reg.rdest);
    printf("\t\t   r2: %-8d   op1: %-8d\n", if_to_id_reg.r2, id_to_exe_reg.op1);
    printf("\t\t   r3: %-8d   op2: %-8d   data: %-6d\n", if_to_id_reg.r3, id_to_exe_reg.op2, exe_to_wb_reg.data);
    printf("\t\t----------------------------------------------\n\n\n");
}

void print_mem_state(void){

    int i;
    printf("\t\t        MEMORY BANK      \n\n");
    printf("\t\t   Address   |   Value   \n");
    printf("\t\t------------- -----------\n");

    for (i=0; i<DMEMB_SIZE; i++){
        printf("\t\t   %-11d   %-8d\n", i*100, data_memory[i]);
    }
    printf("\n\n");
    
}

