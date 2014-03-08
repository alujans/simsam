#include <stdlib.h>
#include <stdio.h>
#include "common/func_units.h"

#define DEBUG


void wb_stage(int register_bank[], struct exe_to_wb_reg * exe_to_wb_reg){

#ifdef DEBUG
    printf("\t\tWB STAGE:\n\n");
#endif

    if(exe_to_wb_reg->w)
        register_bank[exe_to_wb_reg->rdest]=exe_to_wb_reg->data;
}
        
void exe_stage(struct id_to_exe_reg * id_to_exe_reg, struct exe_to_wb_reg * exe_to_wb_reg, int data_memory[]){
#ifdef DEBUG
    printf("\t\tEXE STAGE:\n\n");
#endif

    exe_to_wb_reg->w=( id_to_exe_reg->codop <=3 );
    exe_to_wb_reg->rdest = id_to_exe_reg->rdest;

    int address;
    switch( id_to_exe_reg->codop ){

        case 0:
            exe_to_wb_reg->data = id_to_exe_reg->op1 + id_to_exe_reg->op2;
            break;
        case 1:
            exe_to_wb_reg->data = id_to_exe_reg->op1 - id_to_exe_reg->op2;
            break;
        case 2:
            exe_to_wb_reg->data = id_to_exe_reg->op1 * id_to_exe_reg->op2;
            break;
        case 3:
            address = id_to_exe_reg->op1 + id_to_exe_reg->op2;
            exe_to_wb_reg->data=data_memory[address/100];
            break;
        case 4:
            address = id_to_exe_reg->op1 + id_to_exe_reg->op2;
            data_memory[address/100]= id_to_exe_reg->rdest;
            break;
        case 5:
            exe_to_wb_reg->data=0;
            break;
        case 6:
            exe_to_wb_reg->data=-1;
            break;
    }
    
}

int id_stage(struct if_to_id_reg * if_to_id_reg, struct id_to_exe_reg * id_to_exe_reg, int register_bank[]){

#ifdef DEBUG
    printf("\t\tID STAGE:\n\n");
#endif

    int result=0;


    switch(if_to_id_reg->codop){

        case 6:
            return -1;
        case 5:
            id_to_exe_reg->codop=if_to_id_reg->codop;
            return result;
        case 0:case 1:case 2:
            result=((id_to_exe_reg->rdest == if_to_id_reg->r3) || (id_to_exe_reg->rdest == if_to_id_reg->r2))? 10 : 0;
            id_to_exe_reg->codop=if_to_id_reg->codop;
            id_to_exe_reg->rdest=if_to_id_reg->r1;
            id_to_exe_reg->op1=register_bank[if_to_id_reg->r2];
            id_to_exe_reg->op2=register_bank[if_to_id_reg->r3];
            break;
        case 3:
            result=(id_to_exe_reg->rdest == if_to_id_reg->r2)? 10 : 0;
            id_to_exe_reg->codop=if_to_id_reg->codop;
            id_to_exe_reg->rdest=if_to_id_reg->r1;
            id_to_exe_reg->op1=register_bank[if_to_id_reg->r2];
            id_to_exe_reg->op2=if_to_id_reg->r3;
            break;
        case 4:
            result=(id_to_exe_reg->rdest == if_to_id_reg->r2)? 10 : 0;
            id_to_exe_reg->codop=if_to_id_reg->codop;
            id_to_exe_reg->rdest=if_to_id_reg->r1;
            id_to_exe_reg->op1=register_bank[if_to_id_reg->r2];
            id_to_exe_reg->op2=if_to_id_reg->r3;
            break;
    }
            
    return result;
}

void if_stage(struct instruction ins_memory [] ,struct if_to_id_reg * if_to_id_reg, int i){

#ifdef DEBUG
    printf("\t\tIF STAGE:\n\n");
#endif

    struct instruction current;
    current=ins_memory[i];
    if_to_id_reg->codop = current.codop;
    if_to_id_reg->r1=current.args[0];
    if_to_id_reg->r2=current.args[1];
    if_to_id_reg->r3=current.args[2];

}

