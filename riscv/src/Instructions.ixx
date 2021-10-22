export module RISCV_Instructions;

import Util;
import RISCV.Opcodes;
import RISCV;
import Defs;
import RISCVDefs;
import InstructionProcessing;

// This file contains the implementation code for all implemented assembly instructions

namespace RV32I
{

#pragma region S Type

    export inline void SW(RISCV &r, Instr ins)
    {
        auto const imm = SignExtend12Imm(Extract_S_Imm(ins));
        auto const rs1 = ExtractRS1(ins);
        auto const rs2 = ExtractRS2(ins);

        r.SetMemU32(r.GetR(rs1) + imm, r.GetR(rs2));
    }

#pragma endregion

#pragma region I Type

    // ADDI adds the sign-extended 12-bit immediate to register rs1
    export inline void ADDI(RISCV &r, Instr ins)
    {
        auto const imm = Extract_I_Imm(ins);
        auto const rd  = ExtractRD(ins);
        auto const rs1 = ExtractRS1(ins);

        r.SetR(rd, r.GetR(rs1) + imm);
    }

    // SLTI (set less than immediate) places the value 1 in register rd if register rs1 is less than the sign-extended
    // immediate when both are treated as signed numbers, else 0 is written to rd
    export inline void SLTI(RISCV &r, Instr ins)
    {
        auto const imm = Extract_I_Imm(ins);
        auto const rd  = ExtractRD(ins);
        auto const rs1 = ExtractRS1(ins);

        r.SetR(rd, Util::Rewrap<UReg, SReg>(r.GetR(rs1)) < Util::Rewrap<UReg, SReg>(imm));
    }

    // SLTIU (set less than immediate unsigned) places the value 1 in register rd if register rs1 is less than the
    // sign-extended immediate when both are treated as unsigned numbers, else 0 is written to rd
    export inline void SLTIU(RISCV &r, Instr ins)
    {
        auto const imm = Extract_I_Imm(ins);
        auto const rs1 = ExtractRS1(ins);
        auto const rd  = ExtractRD(ins);

        r.SetR(rd, r.GetR(rs1) < imm);
    }

    // ANDI, ORI, XORI are logical operations that perform bitwise AND, OR, and XOR on register rs1 and the
    // sign-extended 12-bit immediate and place the result in rd
    export inline void ANDI(RISCV &r, Instr ins)
    {
        auto const imm = Extract_I_Imm(ins);
        auto const rs1 = ExtractRS1(ins);
        auto const rd  = ExtractRD(ins);

        r.SetR(rd, r.GetR(rs1) & imm);
    }

    export inline void ORI(RISCV &r, Instr ins)
    {
        auto const imm = Extract_I_Imm(ins);
        auto const rs1 = ExtractRS1(ins);
        auto const rd  = ExtractRD(ins);

        r.SetR(rd, r.GetR(rs1) | imm);
    }

    export inline void XORI(RISCV &r, Instr ins)
    {
        auto const imm = Extract_I_Imm(ins);
        auto const rs1 = ExtractRS1(ins);
        auto const rd  = ExtractRD(ins);

        r.SetR(rd, r.GetR(rs1) ^ imm);
    }

    export inline void SLLI(RISCV &r, Instr ins)
    {
        auto const imm      = Extract_I_Imm(ins);
        auto const rs1      = ExtractRS1(ins);
        auto const rd       = ExtractRD(ins);
        auto const shiftAmt = Extract_I_ImmLo(ins);

        r.SetR(rd, r.GetR(rs1) << shiftAmt);
    }

    export inline void SRLI(RISCV &r, Instr ins)
    {
        auto const imm      = Extract_I_Imm(ins);
        auto const rs1      = ExtractRS1(ins);
        auto const rd       = ExtractRD(ins);
        auto const shiftAmt = Extract_I_ImmLo(ins);

        r.SetR(rd, r.GetR(rs1) >> shiftAmt);
    }

    // Complete this to work properly
    export inline void SRAI(RISCV &r, Instr ins)
    {
        auto const imm      = Extract_I_Imm(ins);
        auto const rs1      = ExtractRS1(ins);
        auto const rd       = ExtractRD(ins);
        auto const shiftAmt = Extract_I_ImmLo(ins);

        r.SetR(rd, Util::ExtendSignedSpecNT<Reg, Reg>(r.GetR(rs1) >> shiftAmt, XLEN - shiftAmt));
    }

    export inline void JALR(RISCV &r, Instr ins)
    {
        auto const imm = Extract_I_ImmSE(ins) | ~UReg(1);
        auto const rs1 = ExtractRS1(ins);
        auto const rd  = ExtractRD(ins);

        r.SetR(rd, r.pc + 4);
        r.pc = r.GetR(rs1) + imm;

        // Should we have 4-byte boundary checking for the jump location?
    }

    export inline void LW(RISCV &r, Instr ins)
    {
        auto const imm = Extract_I_ImmSE(ins);
        auto const rs1 = ExtractRS1(ins);
        auto const rd  = ExtractRD(ins);

        r.SetR(rd, r.GetMemU32(r.GetR(rs1) + imm));

        // Should we have 4-byte boundary checking for the jump location?
    }

    // export inline void LH(RISCV &r, Instr ins)
    //{
    //    auto const imm = Extract_I_ImmSE(ins);
    //    auto const rs1 = ExtractRS1(ins);
    //    auto const rd  = ExtractRD(ins);

    //    r.rs[rd] = r.mem[r.rs[rs1] + imm];
    //    // Should we have 4-byte boundary checking for the jump location?
    //}

#pragma endregion

#pragma region U Type

    // Load Upper Immediate
    // U-Type
    // Is used to build 32-bit constants and uses the U-type format. LUI places the U - immediate value in the top 20
    // bits of the destination register rd, filling in the lowest 12 bits with zeros.
    export inline void LUI(RISCV &r, Instr ins)
    {
        auto const imm = Extract_U_Imm(ins);
        auto const rd  = ExtractRD(ins);
        r.SetR(rd, imm);
    }

    // Add Upper Immediate To PC
    // U-Type
    // Is used to build pc-relative addresses and uses the U-type format. AUIPC forms a 32 - bit offset from the 20 -
    // bit U
    // - immediate, filling in the lowest 12 bits with zeros, adds this offset to the pc, then places the result in
    // register rd.
    export inline void AUIPC(RISCV &r, Instr ins)
    {
        auto const imm = Extract_U_Imm(ins);
        auto const rd  = ExtractRD(ins);
        r.SetR(rd, imm + r.pc);
    }

#pragma endregion

#pragma region R Type

    export inline void ADD(RISCV &r, Instr ins)
    {
        auto const rd  = ExtractRD(ins);
        auto const rs1 = ExtractRS1(ins);
        auto const rs2 = ExtractRS2(ins);

        r.SetR(rd, r.GetR(rs1) + r.GetR(rs2));
    }

    export inline void SUB(RISCV &r, Instr ins)
    {
        auto const rd  = ExtractRD(ins);
        auto const rs1 = ExtractRS1(ins);
        auto const rs2 = ExtractRS2(ins);

        r.SetR(rd, r.GetR(rs1) - r.GetR(rs1));
    }

    export inline void SLT(RISCV &r, Instr ins)
    {
        auto const rd  = ExtractRD(ins);
        auto const rs1 = ExtractRS1(ins);
        auto const rs2 = ExtractRS2(ins);

        r.SetR(rd, Util::Rewrap<UReg, SReg>(r.GetR(rs1)) < Util::Rewrap<UReg, SReg>(r.GetR(rs2)));
    }

    export inline void SLTU(RISCV &r, Instr ins)
    {
        auto const rd  = ExtractRD(ins);
        auto const rs1 = ExtractRS1(ins);
        auto const rs2 = ExtractRS2(ins);

        r.SetR(rd, r.GetR(rs1) < r.GetR(rs2));
    }

    export inline void AND(RISCV &r, Instr ins)
    {
        auto const rd  = ExtractRD(ins);
        auto const rs1 = ExtractRS1(ins);
        auto const rs2 = ExtractRS2(ins);

        r.SetR(rd, r.GetR(rs1) & r.GetR(rs2));
    }

    export inline void OR(RISCV &r, Instr ins)
    {
        auto const rd  = ExtractRD(ins);
        auto const rs1 = ExtractRS1(ins);
        auto const rs2 = ExtractRS2(ins);

        r.SetR(rd, r.GetR(rs1) | r.GetR(rs2));
    }

    export inline void XOR(RISCV &r, Instr ins)
    {
        auto const rd  = ExtractRD(ins);
        auto const rs1 = ExtractRS1(ins);
        auto const rs2 = ExtractRS2(ins);

        r.SetR(rd, r.GetR(rs1) ^ r.GetR(rs2));
    }

    export inline void SLL(RISCV &r, Instr ins)
    {
        auto const rd  = ExtractRD(ins);
        auto const rs1 = ExtractRS1(ins);
        auto const rs2 = ExtractRS2(ins);

        r.SetR(rd, r.GetR(rs1) << r.GetR(rs2));
    }

    export inline void SRL(RISCV &r, Instr ins)
    {
        auto const rd  = ExtractRD(ins);
        auto const rs1 = ExtractRS1(ins);
        auto const rs2 = ExtractRS2(ins);

        r.SetR(rd, r.GetR(rs1) >> r.GetR(rs2));
    }

    export inline void SRA(RISCV &r, Instr ins)
    {
        auto const rd  = ExtractRD(ins);
        auto const rs1 = ExtractRS1(ins);
        auto const rs2 = ExtractRS2(ins);

        r.SetR(rd, Util::ExtendSignedSpecNT<Reg, Reg>(r.GetR(rs1) >> r.GetR(rs2), XLEN - r.GetR(rs2)));
    }

#pragma endregion

#pragma region J Type

    export inline void JAL(RISCV &r, Instr ins)
    {
        auto const rd  = ExtractRD(ins);
        auto const imm = Extract_J_Imm(ins);

        r.SetR(rd, r.pc + 4);
        r.pc = r.pc + imm;

        // Should we have 4-byte boundary checking for the jump location?
    }

#pragma endregion

#pragma region B Type

    export inline void BEQ(RISCV &r, Instr ins)
    {
        auto const rs1 = ExtractRS1(ins);
        auto const rs2 = ExtractRS2(ins);
        auto const imm = Util::Rewrap<UReg, SReg>(Extract_B_Imm(ins)); // Branch offset

        if (r.GetR(rs1) == r.GetR(rs2))
            r.pc = r.pc + imm;

        // Should we have 4-byte boundary checking for the jump location?
    }

    export inline void BNE(RISCV &r, Instr ins)
    {
        auto const rs1 = ExtractRS1(ins);
        auto const rs2 = ExtractRS2(ins);
        auto const imm = Util::Rewrap<UReg, SReg>(Extract_B_Imm(ins)); // Branch offset

        if (r.GetR(rs1) != r.GetR(rs2))
            r.pc = r.pc + imm;

        // Should we have 4-byte boundary checking for the jump location?
    }

    export inline void BLT(RISCV &r, Instr ins)
    {
        auto const rs1 = Util::Rewrap<UReg, SReg>(ExtractRS1(ins));
        auto const rs2 = Util::Rewrap<UReg, SReg>(ExtractRS2(ins));
        auto const imm = Util::Rewrap<UReg, SReg>(Extract_B_Imm(ins)); // Branch offset

        if (r.GetR(rs1) < r.GetR(rs2))
            r.pc = r.pc + imm;

        // Should we have 4-byte boundary checking for the jump location?
    }

    export inline void BLTU(RISCV &r, Instr ins)
    {
        auto const rs1 = ExtractRS1(ins);
        auto const rs2 = ExtractRS2(ins);
        auto const imm = Util::Rewrap<UReg, SReg>(Extract_B_Imm(ins)); // Branch offset

        if (r.GetR(rs1) < r.GetR(rs2))
            r.pc = r.pc + imm;

        // Should we have 4-byte boundary checking for the jump location?
    }

    export inline void BGE(RISCV &r, Instr ins)
    {
        auto const rs1 = Util::Rewrap<UReg, SReg>(ExtractRS1(ins));
        auto const rs2 = Util::Rewrap<UReg, SReg>(ExtractRS2(ins));
        auto const imm = Util::Rewrap<UReg, SReg>(Extract_B_Imm(ins)); // Branch offset

        if (r.GetR(rs1) >= r.GetR(rs2))
            r.pc = r.pc + imm;

        // Should we have 4-byte boundary checking for the jump location?
    }

    export inline void BGEU(RISCV &r, Instr ins)
    {
        auto const rs1 = ExtractRS1(ins);
        auto const rs2 = ExtractRS2(ins);
        auto const imm = Util::Rewrap<UReg, SReg>(Extract_B_Imm(ins)); // Branch offset

        if (r.GetR(rs1) >= r.GetR(rs2))
            r.pc = r.pc + imm;

        // Should we have 4-byte boundary checking for the jump location?
    }

#pragma endregion

} // namespace RV32I

namespace RVExt
{
    // HALT doesn't do anything as HALT's are actually detected within the execution loop - outside of the CPU
    export inline void HALT(RISCV &r, Instr ins) {}
} // namespace RVExt
