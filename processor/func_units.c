#include <stdlib.h>
#include <stdio.h>
#include "common/func_units.h"




void os_stage(int register_bank[], struct alu_to_os_reg * alu_to_os_reg){

#ifdef DEBUG
    printf("ETAPA OS:\n\n");
#endif
}
        
void alu_stage(struct id_to_alu_reg * id_to_alu_reg, struct alu_to_os_reg * alu_to_os_reg){
#ifdef DEBUG
    printf("ETAPA ALU:\n\n");
#endif

}

int id_stage(struct if_to_id_reg * if_to_id_reg, struct id_to_alu_reg * id_to_alu_reg){
#ifdef DEBUG
    printf("ETAPA ID:\n\n");
#endif

}

void if_stage(int ins_memory [] ,struct if_to_id_reg * if_to_id_reg,int i){
#ifdef DEBUG
    printf("ETAPA IF:\n\n");
#endif

}




