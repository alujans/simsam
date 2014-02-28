
struct instruction {
    int codop;
    int r1, r2, r3;
};

struct if_to_id_reg{
    int codop;
    int r1, r2, r3;
};

struct id_to_exe_reg{
    int codop;
    int rdest;
    int op1;
    int op2;
};

struct exe_to_wb_reg{
    int rdest;
    int data;
    int w;
};
