//
// Created by anna_Diao on 2019/7/10.
//

#include "../../header/Pipeline.h"

void Fstep_Decode(Register& r,Buffer_IF_ID& buffer_if_id,Buffer_ID_EX& buffer_id_ex){
    Ins_Base* bp = buffer_if_id.read_bp();
    bp->Decode(r,buffer_if_id,buffer_id_ex);
}

void Ins_R::Decode(Register& r,Buffer_IF_ID& buffer_if_id,Buffer_ID_EX& buffer_id_ex){
    buffer_id_ex.modify_rs1_content(r.get_reg(rs1));
    buffer_id_ex.modify_rs2_content(r.get_reg(rs2));
    buffer_id_ex.modify_rd(rd);
    buffer_id_ex.modify_PC(buffer_if_id.read_PC());

    if(funct7 == 0b0100000){
        switch (funct3){
            case 0:
                buffer_id_ex.modify_instt(SUB);
                break;
            case 0b101:
                buffer_id_ex.modify_instt(SRA);
                break;
            default:
                throw exception::Decode_R_Invalidfunc3();
        }
    }else{
        //以下进行二分的优化，用if套switch
        if(funct3>>2 == 0b0){
            if(funct3 < 0b10){
                switch (funct3){
                    case 0:
                        buffer_id_ex.modify_instt(ADD);
                        break;
                    case 0b1:
                        buffer_id_ex.modify_instt(SLL);
                        break;
                    default:
                        throw exception::Decode_R_Invalidfunc3();
                }
            }else{
                switch (funct3){
                    case 0b10:
                        buffer_id_ex.modify_instt(SLT);
                        break;
                    case 0b11:
                        buffer_id_ex.modify_instt(SLTU);
                        break;
                    default:
                        throw exception::Decode_R_Invalidfunc3();
                }
            }
        }else{
            if(funct3 < 0b110){
                switch (funct3){
                    case 0b100:
                        buffer_id_ex.modify_instt(XOR);
                        break;
                    case 0b101:
                        buffer_id_ex.modify_instt(SRL);
                        break;
                    default:
                        throw exception::Decode_R_Invalidfunc3();
                }
            }else{
                switch (funct3){
                    case 0b110:
                        buffer_id_ex.modify_instt(OR);
                        break;
                    case 0b111:
                        buffer_id_ex.modify_instt(AND);
                        break;
                    default:
                        throw exception::Decode_R_Invalidfunc3();
                }
            }
        }
    }


}
void Ins_I::Decode(Register& r,Buffer_IF_ID& buffer_if_id,Buffer_ID_EX& buffer_id_ex) {
    buffer_id_ex.modify_rs1_content(r.get_reg(rs1));
    buffer_id_ex.modify_rd(rd);
    buffer_id_ex.modify_PC(buffer_if_id.read_PC());
    buffer_id_ex.modify_imm(imm);
    buffer_id_ex.modify_unsigned_imm(unsigned_imm);

    switch(instt){
        case Shift_family:
            switch(funct3){
                case 0b001:buffer_id_ex.modify_instt(SLLI);break;
                case 0b101:
                    if(((unsigned_imm>>10)&1) == 0){
                        buffer_id_ex.modify_instt(SRLI);
                    }else buffer_id_ex.modify_instt(SRAI);
                    break;
                default:throw exception::Finding();
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
                    case 0b1:
                        buffer_id_ex.modify_instt(LH);
                        break;
                    default:
                        throw exception::Decode_I_Invalidfunc3();
                }
            }else{
                switch (funct3){
                    case 0b100:
                        buffer_id_ex.modify_instt(LBU);
                        break;
                    case 0b101:
                        buffer_id_ex.modify_instt(LHU);
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
                        buffer_id_ex.modify_instt(ADDI);
                        break;
                    case 0b10:
                        buffer_id_ex.modify_instt(SLTI);
                        break;
                    case 0b11:
                        buffer_id_ex.modify_instt(SLTIU);
                        break;
                    default:
                        throw exception::Decode_I_Invalidfunc3();
                }
            }else{
                switch (funct3){
                    case 0b100:
                        buffer_id_ex.modify_instt(XORI);
                        break;
                    case 0b110:
                        buffer_id_ex.modify_instt(ORI);
                        break;
                    case 0b111:
                        buffer_id_ex.modify_instt(ANDI);
                        break;
                    default:
                        throw exception::Decode_I_Invalidfunc3();
                }
            }
    }
}
void Ins_S::Decode(Register& r,Buffer_IF_ID& buffer_if_id,Buffer_ID_EX& buffer_id_ex) {
    buffer_id_ex.modify_rs1_content(r.get_reg(rs1));
    buffer_id_ex.modify_rs2_content(r.get_reg(rs2));
    buffer_id_ex.modify_PC(buffer_if_id.read_PC());
    buffer_id_ex.modify_imm(imm);
    buffer_id_ex.modify_unsigned_imm(unsigned_imm);
    switch (funct3){
        case 0:
            buffer_id_ex.modify_instt(SB);break;
        case 0b001:
            buffer_id_ex.modify_instt(SH);break;
        case 0b010:
            buffer_id_ex.modify_instt(SW);break;
        default:throw exception::Decode_S_Invalidfunc3();
    }
}
void Ins_B::Decode(Register& r,Buffer_IF_ID& buffer_if_id,Buffer_ID_EX& buffer_id_ex) {
    buffer_id_ex.modify_rs1_content(r.get_reg(rs1));
    buffer_id_ex.modify_rs2_content(r.get_reg(rs2));
    buffer_id_ex.modify_PC(buffer_if_id.read_PC());
    buffer_id_ex.modify_imm(imm);
    buffer_id_ex.modify_unsigned_imm(unsigned_imm);

    if(funct3 < 0b101){
        switch (funct3){
            case 0b0:
                buffer_id_ex.modify_instt(BEQ);break;
            case 0b1:
                buffer_id_ex.modify_instt(BNE);break;
            case 0b100:
                buffer_id_ex.modify_instt(BLT);break;
            default:throw exception::Decode_B_Invalidfunc3();
        }
    }else{
        switch (funct3){
            case 0b101:
                buffer_id_ex.modify_instt(BGE);break;
            case 0b110:
                buffer_id_ex.modify_instt(BLTU);break;
            case 0b111:
                buffer_id_ex.modify_instt(BGEU);break;
            default:throw exception::Decode_B_Invalidfunc3();
        }
    }
}
void Ins_U::Decode(Register& r,Buffer_IF_ID& buffer_if_id,Buffer_ID_EX& buffer_id_ex) {
    buffer_id_ex.modify_rd(rd);
    buffer_id_ex.modify_PC(buffer_if_id.read_PC());
    buffer_id_ex.modify_imm(imm);
    buffer_id_ex.modify_unsigned_imm(unsigned_imm);
    buffer_id_ex.modify_instt(instt);
}
void Ins_J::Decode(Register& r,Buffer_IF_ID& buffer_if_id,Buffer_ID_EX& buffer_id_ex) {
    buffer_id_ex.modify_rd(rd);
    buffer_id_ex.modify_PC(buffer_if_id.read_PC());
    buffer_id_ex.modify_imm(imm);
    buffer_id_ex.modify_unsigned_imm(unsigned_imm);
    buffer_id_ex.modify_instt(instt);
}