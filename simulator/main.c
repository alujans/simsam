#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include "common/values.h"
#include "common/func_units.h"


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
    int trap = 0;

    setbuf(stdout,NULL);
    printf("\n\n");
    printf("\t\tSIMSAM Simulator, PIPELINED PROCESSOR\n");
    printf("\t\t=====================================\n\n");

    printf("\n\n\t\tINITIAL PROCESSOR STATE\n\n\n");
    print_processor_state();
    print_mem_state();
    sleep(7);
    
    printf("\n\n\t\tBEGINS SIMULATION\n\n\n");


    while(!trap){
        
        printf("\t\t   Cycle: %-4d   \n", i+1);
        printf("\t\t-----------------\n\n");
        wb_stage(register_bank, &exe_to_wb_reg);
        
        exe_stage(&id_to_exe_reg, &exe_to_wb_reg, data_memory);

        trap = id_stage(&if_to_id_reg, &id_to_exe_reg, register_bank);

        if_stage(ins_memory,&if_to_id_reg,i);

        print_reg_state();
        print_processor_state();
        print_mem_state();
        sleep(3);
        i++;
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
        
    print_processor_state();
    print_mem_state();

    return 0;

}

void init_insmem(void){

    // add r1,r4,r6
    ins_memory[0].codop=0;ins_memory[0].r1=1;ins_memory[0].r2=4;ins_memory[0].r3=6;
    //nop overcome dependency
    ins_memory[1].codop=5;ins_memory[1].r1=0;ins_memory[1].r2=0;ins_memory[1].r3=0;
    //sub r2, r1, r7
    ins_memory[2].codop=1;ins_memory[2].r1=2;ins_memory[2].r2=1;ins_memory[2].r3=7;
    //lw r3, r0, 600
    ins_memory[3].codop=3;ins_memory[3].r1=3;ins_memory[3].r2=0;ins_memory[3].r3=600;
    //sw r0, 1200, r1
    ins_memory[4].codop=4;ins_memory[4].r1=1200;ins_memory[4].r2=0;ins_memory[4].r3=1;
    //trap, program ends
    ins_memory[5].codop=6;ins_memory[5].r1=0;ins_memory[5].r2=0;ins_memory[5].r3=0;


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
    fflush(stdout);
}
void print_reg_state(){

    printf("\t\t   IF_TO_ID   |   ID_TO_ALU   |   ALU_TO_OS   \n");
    printf("\t\t-------------- --------------- ---------------\n");
    printf("\t\t   cod: %-7d   cod: %-8d   w: %-9d\n", if_to_id_reg.codop, id_to_exe_reg.codop, exe_to_wb_reg.w);
    printf("\t\t   r1: %-8d   rdest: %-6d   dest: %-6d\n", if_to_id_reg.r1, id_to_exe_reg.rdest, exe_to_wb_reg.rdest);
    printf("\t\t   r2: %-8d   op1: %-8d\n", if_to_id_reg.r2, id_to_exe_reg.op1);
    printf("\t\t   r3: %-8d   op2: %-8d   data: %-6d\n", if_to_id_reg.r3, id_to_exe_reg.op2, exe_to_wb_reg.data);
    printf("\t\t----------------------------------------------\n\n\n");
    fflush(stdout);
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
    fflush(stdout);
    
}

