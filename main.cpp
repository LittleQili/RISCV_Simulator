#include "header/Pipeline.h"

void test_newbuffer(){
    memory *m = new memory;
    m->inimem();
    Register r;
    Buffer_IF_ID buffer_if_id;
    Buffer_ID_EX buffer_id_ex;
    Buffer_EX_MA buffer_ex_ma;
    Buffer_MA_WB buffer_ma_wb;
    while (1) {
        //Fetch
        FStep_Fetch(m,buffer_if_id);
        //Decode
        Fstep_Decode(r,buffer_if_id,buffer_id_ex);
        //Excute
        try {
            Fstep_excute(buffer_id_ex,buffer_ex_ma);
        }
        catch (terminate) {
            std::cout << std::dec << ((int) r.get_reg(10) & 0XFF);
            delete m;
            return ;
        }
        buffer_if_id.modify_PC(buffer_ex_ma.read_PC());//MUX_GREEN写到这一步里了
        //MA
        Fstep_MemoryAccess(m,buffer_ex_ma,buffer_ma_wb);
        //WB
        Fstep_WriteBack(buffer_ma_wb, r);
    }
}
int main() {
    test_newbuffer();
}