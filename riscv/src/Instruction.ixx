module;

#include <vector>

export module RISCV_Instruction;

import RISCV_Instructions;
import RISCV;
import InstructionProcessing;
import RISCVDefs;
import RISCV.Opcodes;

using InstrExecFunc = void (*)(RISCV &, Instr);
using InstrMap      = std::array<InstrExecFunc, std::size_t(Opcodes::LAST_INSTR)>;

constexpr InstrMap MkInstrMap()
{
    InstrMap iMap;

    iMap[std::size_t(Opcodes::LUI)]   = &RV32I::LUI;
    iMap[std::size_t(Opcodes::AUIPC)] = &RV32I::AUIPC;
    iMap[std::size_t(Opcodes::JAL)]   = &RV32I::JAL;
    iMap[std::size_t(Opcodes::JALR)]  = &RV32I::JALR;
    iMap[std::size_t(Opcodes::BEQ)]   = &RV32I::BEQ;
    iMap[std::size_t(Opcodes::BNE)]   = &RV32I::BNE;
    iMap[std::size_t(Opcodes::BLT)]   = &RV32I::BLT;
    iMap[std::size_t(Opcodes::BGE)]   = &RV32I::BGE;
    iMap[std::size_t(Opcodes::BLTU)]  = &RV32I::BLTU;
    iMap[std::size_t(Opcodes::BGEU)]  = &RV32I::BGEU;
    // iMap[std::size_t(Opcodes::LB)] = &RV32I::LB;
    // iMap[std::size_t(Opcodes::LH)] = &RV32I::LH;
    iMap[std::size_t(Opcodes::LW)] = &RV32I::LW;
    // iMap[std::size_t(Opcodes::LBU)] = &RV32I::LBU;
    // iMap[std::size_t(Opcodes::LHU)] = &RV32I::LHU;
    // iMap[std::size_t(Opcodes::SB)] = &RV32I::SB;
    // iMap[std::size_t(Opcodes::SH)] = &RV32I::SH;
    iMap[std::size_t(Opcodes::SW)]    = &RV32I::SW;
    iMap[std::size_t(Opcodes::ADDI)]  = &RV32I::ADDI;
    iMap[std::size_t(Opcodes::SLTI)]  = &RV32I::SLTI;
    iMap[std::size_t(Opcodes::SLTIU)] = &RV32I::SLTIU;
    iMap[std::size_t(Opcodes::XORI)]  = &RV32I::XORI;
    iMap[std::size_t(Opcodes::ORI)]   = &RV32I::ORI;
    iMap[std::size_t(Opcodes::ANDI)]  = &RV32I::ANDI;

    iMap[std::size_t(Opcodes::SLLI)] = &RV32I::SLLI;
    iMap[std::size_t(Opcodes::SRLI)] = &RV32I::SRLI;
    iMap[std::size_t(Opcodes::SRAI)] = &RV32I::SRAI;

    iMap[std::size_t(Opcodes::ADD)]  = &RV32I::ADD;
    iMap[std::size_t(Opcodes::SUB)]  = &RV32I::SUB;
    iMap[std::size_t(Opcodes::SLL)]  = &RV32I::SLL;
    iMap[std::size_t(Opcodes::SLT)]  = &RV32I::SLT;
    iMap[std::size_t(Opcodes::SLTU)] = &RV32I::SLTU;
    iMap[std::size_t(Opcodes::XOR)]  = &RV32I::XOR;
    iMap[std::size_t(Opcodes::SRL)]  = &RV32I::SRL;
    iMap[std::size_t(Opcodes::SRA)]  = &RV32I::SRA;
    iMap[std::size_t(Opcodes::OR)]   = &RV32I::OR;
    iMap[std::size_t(Opcodes::AND)]  = &RV32I::AND;
    // iMap[std::size_t(Opcodes::FENCE)]   = &RV32I::FENCE;
    // iMap[std::size_t(Opcodes::FENCE_I)] = &RV32I::FENCE_I;
    // iMap[std::size_t(Opcodes::ECALL)]   = &RV32I::ECALL;
    // iMap[std::size_t(Opcodes::EBREAK)]  = &RV32I::EBREAK;
    // iMap[std::size_t(Opcodes::CSRRW)]   = &RV32I::CSRRW;
    // iMap[std::size_t(Opcodes::CSRRS)]   = &RV32I::CSRRS;
    // iMap[std::size_t(Opcodes::CSRRC)]   = &RV32I::CSRRC;
    // iMap[std::size_t(Opcodes::CSRRWI)]  = &RV32I::CSRRWI;
    // iMap[std::size_t(Opcodes::CSRRSI)]  = &RV32I::CSRRSI;
    // iMap[std::size_t(Opcodes::CSRRCI)]  = &RV32I::CSRRCI;

    iMap[std::size_t(Opcodes::HALT)] = &RVExt::HALT;

    return iMap;
}

// This file bundles together all C++ side instructions into an easy-to-index array

// Returns false to signal that execution should halt
export inline bool ExecInstruction(RISCV &r, Instr ins)
{
    static const InstrMap instrMap(std::move(MkInstrMap()));
    Opcode                op = ExtractOpcode(ins);
    instrMap[op](r, ins);
    return op != Opcode(Opcodes::HALT);
}

export inline void Run(RISCV &r)
{
    bool run = true;

    do
    {
        run = ExecInstruction(r, r.GetMemU32(r.pc));
        r.pc += sizeof(Instr);
    } while (run);
}

// Primarily used for tests, constructing and executing code in a single line
// Returns the RISCV state for easy evaluation
export inline RISCV CreateAndRun(std::vector<Instr> instructions)
{
    RISCV cpu;
    RISCV::Init(cpu);

    // Load the instructions into CPU's RAM
    for (std::size_t i = 0; i < instructions.size(); i++)
        cpu.SetMemU32(i * sizeof(Instr), instructions[i]);

    Run(cpu);
    return cpu;
}
