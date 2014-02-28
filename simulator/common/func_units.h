#include "types.h"

void wb_stage(int register_bank[], struct exe_to_wb_reg * exe_to_wb_reg);
        
void exe_stage(struct id_to_exe_reg * id_to_exe_reg, struct exe_to_wb_reg * exe_to_wb_reg, int data_memory[]);

int id_stage(struct if_to_id_reg * if_to_id_reg, struct id_to_exe_reg * id_to_exe_reg, int register_bank[]);

void if_stage(struct instruction ins_memory [] ,struct if_to_id_reg * if_to_id_reg,int i);


