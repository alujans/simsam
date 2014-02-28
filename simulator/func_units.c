#include <stdlib.h>
#include <stdio.h>
#include "common/func_units.h"

//#define DEBUG


void os_stage(int register_bank[], struct alu_to_os_reg * alu_to_os_reg){

#ifdef DEBUG
    printf("\t\tOS STAGE:\n\n");
#endif

    if(alu_to_os_reg->w == 1)
        register_bank[alu_to_os_reg->dest]=alu_to_os_reg->res;

}
        
void alu_stage(struct id_to_alu_reg * id_to_alu_reg, struct alu_to_os_reg * alu_to_os_reg){
#ifdef DEBUG
    printf("\t\tALU STAGE:\n\n");
#endif


    alu_to_os_reg->w=1;
    alu_to_os_reg->dest = id_to_alu_reg->rdest;

    switch( id_to_alu_reg->codop ){

        case 0:
            alu_to_os_reg->res = id_to_alu_reg->op1 + id_to_alu_reg->op2;
            break;
        case 1:
            alu_to_os_reg->res = id_to_alu_reg->op1 - id_to_alu_reg->op2;
            break;
        case 2:
            alu_to_os_reg->res = id_to_alu_reg->op1 * id_to_alu_reg->op2;
            break;
        case 3:
            alu_to_os_reg->res = id_to_alu_reg->op1 * id_to_alu_reg->op2;
            break;
        case 4:
            alu_to_os_reg->res = id_to_alu_reg->op1 * id_to_alu_reg->op2;
            break;
        case 5:
            alu_to_os_reg->res=0;
            alu_to_os_reg->w=0;
        case 6:
            alu_to_os_reg->res=-1;
            alu_to_os_reg->w=0;
    }

}

int id_stage(struct if_to_id_reg * if_to_id_reg, struct id_to_alu_reg * id_to_alu_reg, int register_bank[]){
#ifdef DEBUG
    printf("\t\tID STAGE:\n\n");
#endif

    id_to_alu_reg->codop=if_to_id_reg->codop;
    id_to_alu_reg->rdest=if_to_id_reg->r1;
    id_to_alu_reg->op1=register_bank[if_to_id_reg->r2];
    id_to_alu_reg->op2=register_bank[if_to_id_reg->r3];
    
    if(if_to_id_reg->codop == 6)
        return 1;
    return 0;
}

void if_stage(struct instruction ins_memory [] ,struct if_to_id_reg * if_to_id_reg, int i){

#ifdef DEBUG
    printf("\t\tIF STAGE:\n\n");
#endif

    struct instruction current;
    current=ins_memory[i];
    if_to_id_reg->codop = current.codop;
    if_to_id_reg->r1=current.r1;
    if_to_id_reg->r2=current.r2;
    if_to_id_reg->r3=current.r3;

}

