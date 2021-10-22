module;

#include <array>
#include <cstdint>

export module RISCV;

import RISCV.Opcodes;
import Util;
import RISCVDefs;
import Defs;

export enum class Privilege { 
    Machine = 0b11,
    Supervisor = 0b01,
    User = 0b00,
};

// RISC-V CPU state
export struct RISCV
{
    // There are 31 general purpose registers.
    // Out of which 7 are temporary registers (t0-t6).
    // a0-a7 are used for function arguments.
    // s0-s11 are used as saved registers or within function definitions.
    // There is one stack pointer, one global pointer and one thread pointer register.
    // A return address register (x1) to store the return address in a function call.
    // One program counter (pc). pc holds the address of the current instruction.
    // All the registers can be used as a general purpose register.

    // Base Registers
    std::array<Reg, 32> rs; // We include th zero register as later it lets us remove a branch
    // The layout for the above register array can be seen below
    // Reg                 zero;          // Zero register
    // std::array<Reg, 7>  tReg;          // Temporary registers - Temp data for instruction execution (t0-t6)
    // std::array<Reg, 8>  aReg;          // 'a' registers - function arguments (x10-x17)
    // std::array<Reg, 12> sReg;          // 's' registers
    // Reg                 stackPtr = 0;  // Stack pointer register (sp, x2)
    // Reg                 gblPtr   = 0;  // Global pointer register (gp, x3)
    // Reg                 thrPtr   = 0;  // Thread pointer register (tp, x4)
    // Reg                 returnAddrPtr; // Return address pointer register (ra, x1)

    Reg pc; // Program counter register (pc)

    // TODO: Move this outside this class - memory should be an external component - which can maybe even be moved
    // between machines!
    // 128 KiB
    std::array<u8, 4096> mem;

    static void Init(RISCV &r)
    {
        for (std::size_t i = 0; i < r.rs.size(); i++)
            r.rs[i] = 0;
        for (std::size_t i = 0; i < r.mem.size(); i++)
            r.mem[i] = 0;
        r.pc = 0;
    }

    // TODO: Test to see if it's faster to have an if statement - or to actually have a zero register which we write
    // nothing to using some non-branching conditional goodness
    inline void SetR(Sel select, Reg v) { rs.at(select) = (select > 0) * v; }

    inline Reg GetR(Sel select) { return rs.at(select); }

    inline void SetMemU8(Reg addr, u8 val) { mem.at(addr) = val; }
    inline u8   GetMemU8(Reg addr) { return mem.at(addr); }

    inline void SetMemU16(Reg addr, u16 val)
    {
        mem.at(addr)     = Util::GetByte<0, u16>(val);
        mem.at(addr + 1) = Util::GetByte<1, u16>(val);
    }
    inline u16 GetMemU16(Reg addr) { return Util::Merge16(mem.at(addr), mem.at(addr + 1)); }

    inline void SetMemU32(Reg addr, u32 val)
    {
        mem.at(addr)     = Util::GetByte<0, u32>(val);
        mem.at(addr + 1) = Util::GetByte<1, u32>(val);
        mem.at(addr + 2) = Util::GetByte<2, u32>(val);
        mem.at(addr + 3) = Util::GetByte<3, u32>(val);
    }
    inline u32 GetMemU32(Reg addr)
    {
        return Util::Merge32(mem.at(addr), mem.at(addr + 1), mem.at(addr + 2), mem.at(addr + 3));
    }

    inline void SetMemU64(Reg addr, u64 val)
    {
        mem.at(addr)     = Util::GetByte<0, u64>(val);
        mem.at(addr + 1) = Util::GetByte<1, u64>(val);
        mem.at(addr + 2) = Util::GetByte<2, u64>(val);
        mem.at(addr + 3) = Util::GetByte<3, u64>(val);
        mem.at(addr + 4) = Util::GetByte<4, u64>(val);
        mem.at(addr + 5) = Util::GetByte<5, u64>(val);
        mem.at(addr + 6) = Util::GetByte<6, u64>(val);
        mem.at(addr + 7) = Util::GetByte<7, u64>(val);
    }
    inline u64 GetMemU64(Reg addr)
    {
        return Util::Merge64(mem.at(addr), mem.at(addr + 1), mem.at(addr + 2), mem.at(addr + 3), mem.at(addr + 4),
                             mem.at(addr + 5), mem.at(addr + 6), mem.at(addr + 7));
    }
};

#pragma region Helper

#pragma endregion
