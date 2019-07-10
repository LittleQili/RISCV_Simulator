//
// Created by 13683 on 2019/7/10.
//

#ifndef RISCV_SIMULATOR_EXCEPTION_H
#define RISCV_SIMULATOR_EXCEPTION_H

class terminate{};
class exception{
public:
    class Memory_invalid_input{};
    class Buffer_PC_outofbound{};
    class Register_Reg_outofbound{};

    class Decode_R_Invalidfunc3{};
    class Decode_I_Shift_Invalidfunc7{};
    class Decode_R_uninitial{};

    class Decode_I_Invalidfunc3{};
    class Decode_I_InvalidEimm{};

    class Decode_S_Invalidfunc3{};
    class Decode_B_Invalidfunc3{};

    class Decode_U_Invalidinstt{};
    class Decode_J_Invalidinstt{};

    class Finding{};

    class ALU_InvalidLogical{};
    class ALU_InvalidShift{};
    class ALU_InvalidComp{};
    class ALU_InvalidCommand{};
    class ALU_InvalidStore{};
    class ALU_InvalidBranch{};

    class MA_InvalidLoad{};
    class MA_InvalidSTORE{};
};

#endif //RISCV_SIMULATOR_EXCEPTION_H
