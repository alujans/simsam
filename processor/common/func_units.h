void os_stage(int register_bank[], struct alu_to_os_reg * alu_to_os_reg);
        
void alu_stage(struct id_to_alu_reg * id_to_alu_reg, struct alu_to_os_reg * alu_to_os_reg);

int id_stage(struct if_to_id_reg * if_to_id_reg, struct id_to_alu_reg * id_to_alu_reg);

void if_stage(int ins_memory [] ,struct if_to_id_reg * if_to_id_reg,int i);


