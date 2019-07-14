//
// Created by anna_Diao on 2019/7/10.
//

#include "../../header/Parallel.h"
ALU::ALU(Buffer_ID_EX& buffer_id_ex):
        rs1_content(buffer_id_ex.read_rs1_content()),rs2_content(buffer_id_ex.read_rs2_content()),
        unsigned_imm(buffer_id_ex.read_unsigned_imm()),imm(buffer_id_ex.read_imm()),
        instt(buffer_id_ex.read_instt()){};

inline unsigned int ALU::ADDer(unsigned int n1,unsigned int n2){return n1 + n2;}

inline unsigned int ALU::SUBer(unsigned int n1,unsigned int n2){return n1 - n2;}

inline unsigned int ALU::XORer(unsigned int n1,unsigned int n2){return (n1^n2);}

inline unsigned int ALU::ANDer(unsigned int n1,unsigned int n2){return (n1&n2);}

inline unsigned int ALU::ORer(unsigned int n1,unsigned int n2){return (n1|n2);}

int ALU::LOAD_STORE_offset(){return static_cast<int> (imm + rs1_content);}
unsigned int ALU::STOREr(){
    return rs2_content;
}
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
int ALU::JALR(){
    return static_cast<int>((imm+rs1_content)& 0xfffffffe);
}


