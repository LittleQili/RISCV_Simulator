//
// Created by anna_Diao on 2019/7/10.
//

#include "../../header/Pipeline.h"

void Fstep_Decode(Buffer& buffer,Register& r){
    Ins_Base* bp = buffer.read_bp();
    bp->Decode(buffer,r);
}

void Ins_R::Decode(Buffer& buffer,Register& r) {
    buffer.modify_rs1_content(r.get_reg(rs1));
    buffer.modify_rs2_content(r.get_reg(rs2));

    if(instt == UnDecoded){///confirm
        if(funct7 == 0b0100000){
            switch (funct3){
                case 0:
                    instt = SUB;
                    break;
                case 0b101:
                    instt = SRA;
                    break;
                default:
                    throw exception::Decode_R_Invalidfunc3();
            }
        }else if(funct7 == 0){///confirm
            //以下进行二分的优化，用if套switch
            if(funct3>>2 == 0b0){
                if(funct3 < 0b10){
                    switch (funct3){
                        case 0:
                            instt = ADD;
                            break;
                        case 0b1:
                            instt = SLL;
                            break;
                        default:
                            throw exception::Decode_R_Invalidfunc3();
                    }
                }else{
                    switch (funct3){
                        case 0b10:
                            instt = SLT;
                            break;
                        case 0b11:
                            instt = SLTU;
                            break;
                        default:
                            throw exception::Decode_R_Invalidfunc3();
                    }
                }
            }else{
                if(funct3 < 0b110){
                    switch (funct3){
                        case 0b100:
                            instt = XOR;
                            break;
                        case 0b101:
                            instt = SRL;
                            break;
                        default:
                            throw exception::Decode_R_Invalidfunc3();
                    }
                }else{
                    switch (funct3){
                        case 0b110:
                            instt = OR;
                            break;
                        case 0b111:
                            instt = AND;
                            break;
                        default:
                            throw exception::Decode_R_Invalidfunc3();
                    }
                }
            }
        }
    }else throw exception::Decode_R_uninitial();
}
void Ins_I::Decode(Buffer& buffer,Register& r) {
    buffer.modify_rs1_content(r.get_reg(rs1));
    //buffer.modify_rs2_content(r.get_reg(rs2));

    switch(instt){
        case Shift_family:
            switch(funct3){
                case 0b001:instt = SLLI;break;
                case 0b101:
                    if(((unsigned_imm>>10)&1) == 0){
                        instt = SRLI;
                    }else if(((unsigned_imm>>10)&1) == 1){///confirm
                        instt = SRAI;
                    }else throw exception::Decode_I_Shift_Invalidfunc7();
                    break;
                default:throw exception::Finding();
            }
            break;
        case JALR:
            return;
        case Load_family:
            if(funct3 == 0b10){instt = LW;
            }else if(funct3 < 0b10){
                switch (funct3){
                    case 0:
                        instt = LB;
                        break;
                    case 0b1:
                        instt = LH;
                        break;
                    default:
                        throw exception::Decode_I_Invalidfunc3();
                }
            }else{
                switch (funct3){
                    case 0b100:
                        instt = LBU;
                        break;
                    case 0b101:
                        instt = LHU;
                        break;
                    default:
                        throw exception::Decode_I_Invalidfunc3();
                }
            }
            break;
        default:
            if(funct3 < 0b100){
                switch (funct3){
                    case 0:
                        instt = ADDI;
                        break;
                    case 0b10:
                        instt = SLTI;
                        break;
                    case 0b11:
                        instt = SLTIU;
                        break;
                    default:
                        throw exception::Decode_I_Invalidfunc3();
                }
            }else{
                switch (funct3){
                    case 0b100:
                        instt = XORI;
                        break;
                    case 0b110:
                        instt = ORI;
                        break;
                    case 0b111:
                        instt = ANDI;
                        break;
                    default:
                        throw exception::Decode_I_Invalidfunc3();
                }
            }
    }

}
void Ins_S::Decode(Buffer& buffer,Register& r) {
    buffer.modify_rs1_content(r.get_reg(rs1));
    buffer.modify_rs2_content(r.get_reg(rs2));

    switch (funct3){
        case 0:instt = SB;break;
        case 0b001:instt = SH;break;
        case 0b010:instt = SW;break;
        default:throw exception::Decode_S_Invalidfunc3();
    }
}
void Ins_B::Decode(Buffer& buffer,Register& r) {
    buffer.modify_rs1_content(r.get_reg(rs1));
    buffer.modify_rs2_content(r.get_reg(rs2));

    if(funct3 < 0b101){
        switch (funct3){
            case 0b0:instt = BEQ;break;
            case 0b1:instt = BNE;break;
            case 0b100:instt = BLT;break;
            default:throw exception::Decode_B_Invalidfunc3();
        }
    }else{
        switch (funct3){
            case 0b101:instt = BGE;break;
            case 0b110:instt = BLTU;break;
            case 0b111:instt = BGEU;break;
            default:throw exception::Decode_B_Invalidfunc3();
        }
    }
}
void Ins_U::Decode(Buffer& buffer,Register& r) {
    if(!(instt == LUI||instt == AUIPC)) throw exception::Decode_U_Invalidinstt();
}
void Ins_J::Decode(Buffer& buffer,Register& r) {
    if(instt != JAL)throw exception::Decode_J_Invalidinstt();
}