//
// Created by anna_Diao on 2019/7/10.
//

#include "../../header/Pipeline.h"

ALU::ALU(Buffer& buffer)
:rs1_content(buffer.read_rs1_content()),rs2_content(buffer.read_rs2_content()),
unsigned_imm(buffer.read_bp()->unsigned_imm),imm(buffer.read_bp()->imm),
instt(buffer.read_bp()->instt)
{};

inline unsigned int ALU::ADDer(unsigned int n1,unsigned int n2){return n1 + n2;}

inline unsigned int ALU::SUBer(unsigned int n1,unsigned int n2){return n1 - n2;}

inline unsigned int ALU::XORer(unsigned int n1,unsigned int n2){return (n1^n2);}

inline unsigned int ALU::ANDer(unsigned int n1,unsigned int n2){return (n1&n2);}

inline unsigned int ALU::ORer(unsigned int n1,unsigned int n2){return (n1|n2);}

int ALU::LOAD_STORE_offset(){return static_cast<int> (imm + rs1_content);}
unsigned int ALU::ARITHer(){
    switch (instt){
        case ADDI:return ADDer(rs1_content,imm);
        case ADD:return ADDer(rs1_content,rs2_content);
        case SUB:return SUBer(rs1_content,rs2_content);
        case LUI:return ADDer(unsigned_imm,0);
        default:throw exception::ALU_InvalidLogical();
    }
}
unsigned int ALU::LOGICer(){
    switch (instt){
        case XOR:return XORer(rs1_content,rs2_content);
        case XORI:return XORer(rs1_content,imm);
        case AND:return ANDer(rs1_content,rs2_content);
        case ANDI:return ANDer(rs1_content,imm);
        case OR:return ORer(rs1_content,rs2_content);
        case ORI:return ORer(rs1_content,imm);
        default:throw exception::ALU_InvalidLogical();
    }
}
unsigned int ALU::SHIFTer(){
    bool isleft = false,issigned = false;
    unsigned int t;
    switch(instt){
        case SLL:isleft = true;t = rs2_content&0x1f;break;
        case SLLI:isleft = true;t = imm&0x1f;break;
        case SRL:t = rs2_content&0x1f;break;
        case SRLI:t = imm&0x1f;break;
        case SRA:issigned = true;t = rs2_content&0x1f;break;
        case SRAI:issigned = true;t = imm&0x1f;break;
        default:throw exception::ALU_InvalidShift();
    }

    if(isleft)return (rs1_content<<t);
    if(issigned){
        int srs1 = static_cast<int>(rs1_content),st = static_cast<int>(t);
        return static_cast<unsigned int>(srs1>>st);
    }else return (rs1_content>>t);

}
unsigned int ALU::COMPer(){
    bool issigned;
    unsigned int n2 = rs2_content;
    switch (instt){
        case SLT:issigned = true;break;
        case SLTU:issigned = false;break;
        case SLTI:n2 = imm;issigned = true;break;
        case SLTIU:n2 = imm;issigned = false;break;
        default:throw exception::ALU_InvalidComp();
    }
    if(issigned){
        int sn1 = static_cast<int>(rs1_content);
        int sn2 = static_cast<int>(n2);
        if(sn1 < sn2)return static_cast<unsigned int>(1);
        else return static_cast<unsigned int>(0);
    }else{
        if(rs1_content < n2)return static_cast<unsigned int>(1);
        else return static_cast<unsigned int>(0);
    }
}
int ALU::BRANCHer(){
    if(instt == BEQ)return rs1_content == rs2_content? static_cast<int>(imm):4;
    if(instt == BNE)return rs1_content != rs2_content? static_cast<int>(imm):4;

    if(instt == BLTU)return rs1_content < rs2_content? static_cast<int>(imm):4;
    if(instt == BGEU)return rs1_content >= rs2_content? static_cast<int>(imm):4;

    int sn1 = static_cast<int>(rs1_content),sn2 = static_cast<int>(rs2_content);
    if(instt == BLT)return sn1 < sn2?static_cast<int>(imm):4;
    if(instt == BGE)return sn1 >= sn2?static_cast<int>(imm):4;
    throw exception::ALU_InvalidBranch();
}
int ALU::JAL(){
    return static_cast<int>(imm);
}
int ALU::JALR(){
    return static_cast<int>((imm+rs1_content)& 0xfffffffe);
}
int ALU::AUIPC(){
    return static_cast<int>(unsigned_imm);
}

void Fstep_excute(Buffer& buffer){
    ALU exe(buffer);
    int tmptype = static_cast<int>(buffer.read_bp()->instt);
    if(tmptype < 40){
        ///关于L和S的解释：
        ///L:在EX阶段仅仅计算出offset;
        ///S:计算offset，并且将rs2处理掉。
        if(tmptype < 20) {
            buffer.modify_mem_offset(exe.LOAD_STORE_offset());///???
            if((tmptype >= 10)&&buffer.read_mem_offset() == (int)0x30004) throw terminate();
            //if(tmptype >= 10)buffer.modify_rs2_content(exe.STOREer());
        }
        else{
            if(tmptype < 30)buffer.modify_rd_value(exe.ARITHer());
            else buffer.modify_rd_value(exe.LOGICer());
        }
    }else{
        if(tmptype < 60){
            if(tmptype < 50)buffer.modify_rd_value(exe.SHIFTer());
            else buffer.modify_rd_value(exe.COMPer());
        }
        else{
            if(tmptype < 70)buffer.jumpcommon_PC(exe.BRANCHer() - 4);
            else{
                if(tmptype == AUIPC){///AUIPC
                    buffer.jumpcommon_PC(exe.AUIPC() - 4);
                    buffer.modify_rd_value(static_cast<unsigned int>(buffer.read_PC()));
                }else if(tmptype == JAL){///JAL
                    buffer.modify_rd_value(static_cast<unsigned int>(buffer.read_PC()));
                    buffer.jumpcommon_PC(exe.JAL() - 4);
                }else if(tmptype == JALR){///JALR
                    buffer.modify_rd_value(static_cast<unsigned int>(buffer.read_PC()));
                    buffer.jumpcommon_PC(exe.JALR() - buffer.read_PC());
                }
            }
        }

    }
}