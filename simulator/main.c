#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include "common/values.h"
#include "common/func_units.h"


struct instruction ins_memory[IMEMB_SIZE]; 
int register_bank[REGB_SIZE];
int data_memory[DMEMB_SIZE];

void init_mem(void);
void init_register_bank(void);
void print_processor_state(void);
void print_reg_state(struct if_to_id_reg if_to_id_reg, struct id_to_alu_reg id_to_alu_reg, struct alu_to_os_reg alu_to_os_reg);

int main (int argc, char * argv[]){

    init_mem();
   
    init_register_bank();


    struct if_to_id_reg if_to_id_reg={5,0,0,0};
    struct id_to_alu_reg id_to_alu_reg={5,0,0,0};
    struct alu_to_os_reg alu_to_os_reg={0,0,0};

    int i = 0;
    int trap = 0;

    printf("\n\n");
    printf("\t\tSIMSAM Simulator, PIPELINED PROCESSOR\n");
    printf("\t\t=====================================\n\n");

    printf("\n\n\t\tINITIAL PROCESSOR STATE\n\n\n");
    print_processor_state();
    sleep(7);
    
    printf("\n\n\t\tBEGINS SIMULATION\n\n\n");


    while(!trap){
        
        printf("\t\t   Cycle: %-4d   \n", i+1);
        printf("\t\t-----------------\n\n");
        os_stage(register_bank, &alu_to_os_reg);
        
        alu_stage(&id_to_alu_reg, &alu_to_os_reg);

        trap = id_stage(&if_to_id_reg, &id_to_alu_reg, register_bank);

        if_stage(ins_memory,&if_to_id_reg,i);

        print_reg_state(if_to_id_reg, id_to_alu_reg, alu_to_os_reg);
        print_processor_state();
        sleep(3);
        i++;
    }
   
    //Complete last cyle - remains one stage to be executed when trap is detected
    os_stage(register_bank, &alu_to_os_reg);
    printf("\t\t   Cycle: %-4d   \n", i+1);
    printf("\t\t-----------------\n\n");
    print_reg_state(if_to_id_reg, id_to_alu_reg, alu_to_os_reg);
    print_processor_state();
    sleep(3);

        
    printf("\n\t\t===============================\n");
    printf("\t\tTRAP REACHED, END OF SIMULATION\n");
    printf("\t\t===============================\n\n");
        
    print_processor_state();

    return 0;

}

void init_mem(void){

    // add r1,r4,r6
    ins_memory[0].codop=0;ins_memory[0].r1=1;ins_memory[0].r2=4;ins_memory[0].r3=6;
    //nop overcome dependency
    ins_memory[1].codop=5;ins_memory[1].r1=0;ins_memory[1].r2=0;ins_memory[1].r3=0;

    //sub r2, r1, r7
    ins_memory[2].codop=1;ins_memory[2].r1=2;ins_memory[2].r2=1;ins_memory[2].r3=7;
    //trap, program ends
    ins_memory[3].codop=6;ins_memory[3].r1=0;ins_memory[3].r2=0;ins_memory[3].r3=0;

}


void init_register_bank(void){

    int i;
    for (i=0; i < REGB_SIZE; i++)
        register_bank[i]=i*10;
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
void print_reg_state(struct if_to_id_reg if_to_id_reg, struct id_to_alu_reg id_to_alu_reg, struct alu_to_os_reg alu_to_os_reg){

    printf("\t\t   IF_TO_ID   |   ID_TO_ALU   |   ALU_TO_OS   \n");
    printf("\t\t-------------- --------------- ---------------\n");
    printf("\t\t   cod: %-7d   cod: %-8d   w: %-9d\n", if_to_id_reg.codop, id_to_alu_reg.codop, alu_to_os_reg.w);
    printf("\t\t   r1: %-8d   rdest: %-6d   dest: %-6d\n", if_to_id_reg.r1, id_to_alu_reg.rdest, alu_to_os_reg.dest);
    printf("\t\t   r2: %-8d   op1: %-8d\n", if_to_id_reg.r2, id_to_alu_reg.op1);
    printf("\t\t   r3: %-8d   op2: %-8d   res: %-7d\n", if_to_id_reg.r3, id_to_alu_reg.op2, alu_to_os_reg.res);
    printf("\t\t----------------------------------------------\n\n\n");
}
