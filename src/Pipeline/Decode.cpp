//
// Created by anna_Diao on 2019/7/10.
//

#include "../../header/Parallel.h"

void Ins_R::Decode(Register& r,Buffer_IF_ID& buffer_if_id,Buffer_ID_EX& buffer_id_ex){
    buffer_id_ex.modify_rs1_content(r.get_reg(rs1));
    buffer_id_ex.modify_rs2_content(r.get_reg(rs2));
    buffer_id_ex.modify_rd(rd);

    if(funct7 == 0b0100000){
        switch (funct3){
            case 0:
                buffer_id_ex.modify_instt(SUB);
                break;
            default:
                buffer_id_ex.modify_instt(SRA);
                break;
        }
    }else{
        //以下进行二分的优化，用if套switch
        if(funct3>>2 == 0b0){
            if(funct3 < 0b10){
                switch (funct3){
                    case 0:
                        buffer_id_ex.modify_instt(ADD);
                        break;
                    default:
                        buffer_id_ex.modify_instt(SLL);
                        break;
                }
            }else{
                switch (funct3){
                    case 0b10:
                        buffer_id_ex.modify_instt(SLT);
                        break;
                    default:
                        buffer_id_ex.modify_instt(SLTU);
                        break;
                }
            }
        }else{
            if(funct3 < 0b110){
                switch (funct3){
                    case 0b100:
                        buffer_id_ex.modify_instt(XOR);
                        break;
                    default:
                        buffer_id_ex.modify_instt(SRL);
                        break;
                }
            }else{
                switch (funct3){
                    case 0b110:
                        buffer_id_ex.modify_instt(OR);
                        break;
                    default:
                        buffer_id_ex.modify_instt(AND);
                        break;
                }
            }
        }
    }


}
void Ins_I::Decode(Register& r,Buffer_IF_ID& buffer_if_id,Buffer_ID_EX& buffer_id_ex) {
    buffer_id_ex.modify_rs1_content(r.get_reg(rs1));
    buffer_id_ex.modify_rd(rd);
    buffer_id_ex.modify_imm(imm);
    buffer_id_ex.modify_unsigned_imm(unsigned_imm);

    switch(instt){
        case Shift_family:
            switch(funct3){
                case 0b001:buffer_id_ex.modify_instt(SLLI);break;
                default:
                    if(((unsigned_imm>>10)&1) == 0){
                        buffer_id_ex.modify_instt(SRLI);
                    }else buffer_id_ex.modify_instt(SRAI);
                    break;
            }
            break;
        case JALR:
            buffer_id_ex.modify_instt(JALR);
            return;
        case Load_family:
            if(funct3 == 0b10) buffer_id_ex.modify_instt(LW);
            else if(funct3 < 0b10){
                switch (funct3){
                    case 0:
                        buffer_id_ex.modify_instt(LB);
                        break;
                    default:
                        buffer_id_ex.modify_instt(LH);
                        break;
                }
            }else{
                switch (funct3){
                    case 0b100:
                        buffer_id_ex.modify_instt(LBU);
                        break;
                    default:
                        buffer_id_ex.modify_instt(LHU);
                        break;
                }
            }
            break;
        default:
            if(funct3 < 0b100){
                switch (funct3){
                    case 0:
                        buffer_id_ex.modify_instt(ADDI);
                        break;
                    case 0b10:
                        buffer_id_ex.modify_instt(SLTI);
                        break;
                    default:
                        buffer_id_ex.modify_instt(SLTIU);
                        break;
                }
            }else{
                switch (funct3){
                    case 0b100:
                        buffer_id_ex.modify_instt(XORI);
                        break;
                    case 0b110:
                        buffer_id_ex.modify_instt(ORI);
                        break;
                    default:
                        buffer_id_ex.modify_instt(ANDI);
                        break;
                }
            }
    }
}
void Ins_S::Decode(Register& r,Buffer_IF_ID& buffer_if_id,Buffer_ID_EX& buffer_id_ex) {
    buffer_id_ex.modify_rs1_content(r.get_reg(rs1));
    buffer_id_ex.modify_rs2_content(r.get_reg(rs2));
    buffer_id_ex.modify_imm(imm);
    buffer_id_ex.modify_unsigned_imm(unsigned_imm);
    switch (funct3){
        case 0:
            buffer_id_ex.modify_instt(SB);break;
        case 0b001:
            buffer_id_ex.modify_instt(SH);break;
        default:
            buffer_id_ex.modify_instt(SW);break;
    }
}
void Ins_B::Decode(Register& r,Buffer_IF_ID& buffer_if_id,Buffer_ID_EX& buffer_id_ex) {
    buffer_id_ex.modify_rs1_content(r.get_reg(rs1));
    buffer_id_ex.modify_rs2_content(r.get_reg(rs2));
    buffer_id_ex.modify_imm(imm);
    buffer_id_ex.modify_unsigned_imm(unsigned_imm);

    if(funct3 < 0b101){
        switch (funct3){
            case 0b0:
                buffer_id_ex.modify_instt(BEQ);break;
            case 0b1:
                buffer_id_ex.modify_instt(BNE);break;
            default:
                buffer_id_ex.modify_instt(BLT);break;
        }
    }else{
        switch (funct3){
            case 0b101:
                buffer_id_ex.modify_instt(BGE);break;
            case 0b110:
                buffer_id_ex.modify_instt(BLTU);break;
            default:
                buffer_id_ex.modify_instt(BGEU);break;
        }
    }
}
void Ins_U::Decode(Register& r,Buffer_IF_ID& buffer_if_id,Buffer_ID_EX& buffer_id_ex) {
    buffer_id_ex.modify_rd(rd);
    buffer_id_ex.modify_imm(imm);
    buffer_id_ex.modify_unsigned_imm(unsigned_imm);
    buffer_id_ex.modify_instt(instt);
}
void Ins_J::Decode(Register& r,Buffer_IF_ID& buffer_if_id,Buffer_ID_EX& buffer_id_ex) {
    buffer_id_ex.modify_rd(rd);
    buffer_id_ex.modify_imm(imm);
    buffer_id_ex.modify_unsigned_imm(unsigned_imm);
    buffer_id_ex.modify_instt(instt);
}