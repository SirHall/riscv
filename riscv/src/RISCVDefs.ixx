export module RISCVDefs;

// TODO: Move definitions out of Util.ixx
import Util;
import Defs;

export using UReg     = u32;
export using SReg     = s32;
export using Reg      = UReg;            // Register type
export const u64 XLEN = sizeof(Reg) * 8; // Number of bits in a register
export using Sel      = u8;              // Used as index to select register
export using Opcode   = u8;
export using Instr    = Reg; // Instruction type
