#include<stdlib.h>
#include<stdio.h>
#include "common/values.h"
#include "common/types.h"
#include "common/func_units.h"


instruction ins_memory[IMEMB_SIZE]; 
int register_bank[RGB_SIZE];
int data_memory[DMEMB_SIZE];

void init_mem(void);

int main (int argc, char * argv[]){

    init_mem();
   
//TODO: READ from file
/*    FILE *fp;

    char ins 
    fp = fopen("/home/alujan/data/ins_memory","r");


    if ( fp == NULL ){
        perror("Error while accessing instruction memory\n");
        exit(EXIT_FAILURE);
    }
*/

    struct if_to_id_reg if_to_id_reg;
    struct id_to_alu_reg id_to_alu_reg;
    struct alu_to_os_reg alu_to_os_reg;

    int i = 0;
    int trap = 0;
    while(!trap){
        
        os_stage(register_bank, &alu_to_os_reg);
        
        alu_stage(&id_to_alu_reg, &alu_to_os_reg);

        trap = id_stage(&if_to_id_reg, &id_to_alu_reg);

        if_stage(ins_memory,&if_to_id_reg,i);

        i++;
    }


}

void init_mem(void){

    ins_memory[0].codop=0;ins_memory[0].r1=1;ins_memory[0].r2=4;ins_memory[0].r3=6;
    //nop solventar dependencia
    ins_memory[1].codop=5;ins_memory[1].r1=0;ins_memory[1].r2=0;ins_memory[1].r3=0;
    ins_memory[2].codop=1;ins_memory[2].r1=2;ins_memory[2].r2=1;ins_memory[2].r3=7;

}
