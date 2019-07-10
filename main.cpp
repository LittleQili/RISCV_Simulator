#include "header/Pipeline.h"

int main() {
    memory *m = new memory;
    m->inimem();
    Register r;
    Buffer buffer;
    while (1) {
        //Fetch
        FStep_Fetch(m, r, buffer);
        //Decode
        Fstep_Decode(buffer, r);
        //Excute
        try {
            Fstep_excute(buffer);
        }
        catch (terminate) {
            std::cout << std::dec << ((int) r.get_reg(10) & 0XFF);
            delete m;
            return 0;
        }
        //MA
        Fstep_MemoryAccess(buffer, m);
        //WB
        Fstep_WriteBack(buffer, r);
    }
}