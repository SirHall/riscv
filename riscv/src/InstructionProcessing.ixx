export module InstructionProcessing;

import Util;
import RISCVDefs;
import RISCV.Opcodes;
import Defs;

export inline Opcode ExtractOpcode(Instr ins) { return Util::Extract<0, 6, Instr, Opcode>(ins); }

export inline u8 ExtractRD(Instr ins) { return Util::Extract<7, 11, Instr, u8>(ins); }
export inline u8 ExtractRS1(Instr ins) { return Util::Extract<12, 16, Instr, u8>(ins); }
export inline u8 ExtractRS2(Instr ins) { return Util::Extract<17, 21, Instr, u8>(ins); }

export inline SReg SignExtend12Imm(u32 imm)
{
    return Util::Rewrap<UReg, SReg>(Util::ExtendSignedSpec<11, Reg, Reg>(imm));
}

#pragma region Op Format Specific

#pragma region R Type

export inline Instr FormR(Opcodes op, Sel rd, Sel rs1, Sel rs2)
{
    return Instr(op) | (Instr(rd) << 7) | (Instr(rs1) << 12) | (Instr(rs2) << 17);
}

#pragma endregion

#pragma region I Type

export inline UReg Extract_I_Imm(Instr ins) { return Util::Extract<20, 31, Instr, u32>(ins); }

// Extract Immediate Sign Extended
export inline SReg Extract_I_ImmSE(Instr ins) { return SignExtend12Imm(Extract_I_Imm(ins)); }

export inline UReg Extract_I_ImmLo(Instr ins) { return Util::Extract<25, 31, Instr, u32>(ins); }
export inline UReg Extract_I_ImmHi(Instr ins) { return Util::Extract<20, 34, Instr, u32>(ins); }

export inline Instr FormI(Opcodes op, Sel rd, Sel rs1, Reg imm)
{
    return Instr(op) | (Instr(rd) << 7) | (Instr(rs1) << 12) | (Instr(imm) << 20);
}

#pragma endregion

#pragma region S Type

export inline UReg Extract_S_ImmLo(Instr ins) { return Util::Extract<7, 11, Instr, u32>(ins); }
export inline UReg Extract_S_ImmHi(Instr ins) { return Util::Extract<25, 31, Instr, u32>(ins); }

export inline UReg Extract_S_Imm(Instr ins) { return Extract_S_ImmLo(ins) | (Extract_S_ImmHi(ins) << 5); }

export inline Instr FormS(Opcodes op, Sel rs1, Sel rs2, Reg imm)
{
    return Instr(op) | (Instr(imm & 0x0000001F) << 7) | (Instr(rs1) << 12) | (Instr(rs2) << 17) |
           ((Instr(imm) & 0x0001FFE0) << 20);
}

#pragma endregion

#pragma region U Type

export inline UReg Extract_U_Imm(Instr ins) { return Util::Extract<12, 31, Instr, u32>(ins) << 12; }

export inline Instr FormU(Opcodes op, Sel rd, Reg imm)
{
    return Instr(op) | (Instr(rd) << 7) | ((Reg(imm) & 0xFFFFF000) << 12);
}

#pragma endregion

#pragma region B Type

// IMM indexing starts from the LSB towards the MSB from the source instruction
export inline UReg Extract_B_Imm0(Instr ins) { return ((u32(1) << 7) & ins) >> 7; }
export inline UReg Extract_B_Imm1(Instr ins) { return ((u32(0xF) << 8) & ins) >> 8; }
export inline UReg Extract_B_Imm2(Instr ins) { return ((u32(0x111111) << 25) & ins) >> 25; }
export inline UReg Extract_B_Imm3(Instr ins) { return ((u32(1) << 31) & ins) >> 31; }

export inline UReg Extract_B_Imm(Instr ins)
{
    return UReg(0) | Extract_B_Imm0(ins) << 11 | Extract_B_Imm1(ins) << 1 | Extract_B_Imm2(ins) << 5 |
           Extract_B_Imm3(ins) << 12;
}

export inline Instr FormB(Opcodes op, Sel rs1, Sel rs2, Reg imm)
{
    // The compiler should optimize the redundant shifts
    return Instr(op) | (Instr(rs1) << 12) | (Instr(rs2) << 17) | ((Instr(imm) & (Instr(1) << 11) >> 11) << 7) |
           (Util::Extract<1, 4, Reg, Instr>(imm) << 8) | (Util::Extract<5, 10, Reg, Instr>(imm) << 25) |
           ((Instr(imm) & (Instr(1) << 12) >> 12) << 31);
}

#pragma endregion

#pragma region J Type

export inline UReg Extract_J_Imm0(Instr ins) { return Util::Extract<12, 19, Instr, u32>(ins); }
export inline UReg Extract_J_Imm1(Instr ins) { return (u32(ins) & (u32(1) << 20)) >> 20; }
export inline UReg Extract_J_Imm2(Instr ins) { return Util::Extract<21, 30, Instr, u32>(ins); }
export inline UReg Extract_J_Imm3(Instr ins) { return (u32(ins) & (u32(1) << 31)) >> 31; }

export inline UReg Extract_J_Imm(Instr ins)
{
    return u32(0) | (Extract_J_Imm0(ins) << 12) | (Extract_J_Imm1(ins) << 11) | (Extract_J_Imm2(ins) << 1) |
           (Extract_J_Imm3(ins) << 20);
}

export inline Instr FormJ(Opcodes op, Sel rd, Reg imm)
{
    return Instr(op) | (Instr(rd) << 7) | (Util::Extract<12, 19, Reg, Instr>(imm) << 12) |
           (((imm & (Instr(1) << 11)) >> 11) << 12) | (Util::Extract<1, 10, Reg, Instr>(imm) << 21) |
           (((imm & (Instr(1) << 20)) >> 20) << 31);
}

#pragma endregion

#pragma endregion
