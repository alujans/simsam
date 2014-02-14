
typedef struct {
    int codop;
    int r1, r2, r3;
}intruction;

struct if_to_id_reg{
    int codop;
    int r1, r2, r3;
};

struct id_to_alu_reg{
    int codop;
    int rdest;
    int op1, op2;
};

struct alu_to_os_reg{
    int res;
    int dest;
};
