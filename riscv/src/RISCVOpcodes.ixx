module;

#include <cstdint>
#include <map>
#include <string>

export module RISCV.Opcodes;

// Using simplified instruction format
export enum class Opcodes {
    // RV32I
    LUI,
    AUIPC,
    JAL,
    JALR,
    BEQ,
    BNE,
    BLT,
    BGE,
    BLTU,
    BGEU,
    LB,
    LH,
    LW,
    LBU,
    LHU,
    SB,
    SH,
    SW,
    ADDI,
    SLTI,
    SLTIU,
    XORI,
    ORI,
    ANDI,
    SLLI, // Use RV64I variant
    SRLI, // Use RV64I variant
    SRAI, // Use RV64I variant
    ADD,
    SUB,
    SLL,
    SLT,
    SLTU,
    XOR,
    SRL,
    SRA,
    OR,
    AND,
    FENCE,
    FENCE_I,
    ECALL,
    EBREAK,
    CSRRW,
    CSRRS,
    CSRRC,
    CSRRWI,
    CSRRSI,
    CSRRCI,

    // RV64I
    LWU,
    LD,
    SD,
    // SLLI, // Redefinition from RV32I
    // SRLI, // Redefinition from RV32I
    // SRAI, // Redefinition from RV32I
    ADDIW,
    SLLIW,
    SRLIW,
    SRAIW,
    ADDW,
    SUBW,
    SLLW,
    SRLW,
    SRAW,

    // RV32M
    MUL,
    MULH,
    MULHSU,
    MULHU,
    DIV,
    DIVU,
    REM,
    REMU,

    // RV64M
    MULW,
    DIVW,
    DIVUW,
    REMW,
    REMUW,

    // RV32F
    FLW,
    FSW,
    FMADD_S,
    FMSUB_S,
    FNMSUB_S,
    FNMADD_S,
    FADD_S,
    FSUB_S,
    FMUL_S,
    FDIV_S,
    FSQRT_S,
    FSGNJ_S,
    FSGNJN_S,
    FSGNJX_S,
    FMIN_S,
    FMAX_S,
    FCVT_W_S,
    FCVT_WU_S,
    FMV_X_W,
    FEQ_S,
    FLT_S,
    FLE_S,
    FCLASS_S,
    FCVT_S_W,
    FCVT_S_WU,
    FMV_W_X,

    // RV64F
    FCVT_L_S,
    FCVT_LU_S,
    FCVT_S_L,
    FCVT_S_LU,

    HALT, // Not in the standard

    // Surely we don't need RV*D :)

    LAST_INSTR // This key must always be at the end of this enum
};

export enum class InstrType { R, I, S, B, U, J };

export enum class OpcodeTypes {
    // RV32I
    LUI   = int(InstrType::U),
    AUIPC = int(InstrType::U),
    JAL   = int(InstrType::J),
    JALR  = int(InstrType::I),
    BEQ   = int(InstrType::B),
    BNE   = int(InstrType::B),
    BLT   = int(InstrType::B),
    BGE   = int(InstrType::B),
    BLTU  = int(InstrType::B),
    BGEU  = int(InstrType::B),
    LB    = int(InstrType::I),
    LH    = int(InstrType::I),
    LW    = int(InstrType::I),
    LBU   = int(InstrType::I),
    LHU   = int(InstrType::I),
    SB    = int(InstrType::S),
    SH    = int(InstrType::S),
    SW    = int(InstrType::S),
    ADDI  = int(InstrType::I),
    SLTI  = int(InstrType::I),
    SLTIU = int(InstrType::I),
    XORI  = int(InstrType::I),
    ORI   = int(InstrType::I),
    ANDI  = int(InstrType::I),
    SLLI  = int(InstrType::I),
    SRLI  = int(InstrType::I),
    SRAI  = int(InstrType::I),
    ADD   = int(InstrType::R),
    SUB   = int(InstrType::R),
    SLL   = int(InstrType::R),
    SLT   = int(InstrType::R),
    SLTU  = int(InstrType::R),
    XOR   = int(InstrType::R),
    SRL   = int(InstrType::R),
    SRA   = int(InstrType::R),
    OR    = int(InstrType::R),
    AND   = int(InstrType::R),
};

const std::map<std::string, Opcodes> strToOpcodeMap = {
    {"LUI", Opcodes::LUI},     {"AUIPC", Opcodes::AUIPC}, {"JAL", Opcodes::JAL},   {"JALR", Opcodes::JALR},
    {"BEQ", Opcodes::BEQ},     {"BNE", Opcodes::BNE},     {"BLT", Opcodes::BLT},   {"BGE", Opcodes::BGE},
    {"BLTU", Opcodes::BLTU},   {"BGEU", Opcodes::BGEU},   {"LB", Opcodes::LB},     {"LH", Opcodes::LH},
    {"LW", Opcodes::LW},       {"LBU", Opcodes::LBU},     {"LHU", Opcodes::LHU},   {"SB", Opcodes::SB},
    {"SH", Opcodes::SH},       {"SW", Opcodes::SW},       {"ADDI", Opcodes::ADDI}, {"SLTI", Opcodes::SLTI},
    {"SLTIU", Opcodes::SLTIU}, {"XORI", Opcodes::XORI},   {"ORI", Opcodes::ORI},   {"ANDI", Opcodes::ANDI},
    {"SLLI", Opcodes::SLLI},   {"SRLI", Opcodes::SRLI},   {"SRAI", Opcodes::SRAI}, {"ADD", Opcodes::ADD},
    {"SUB", Opcodes::SUB},     {"SLL", Opcodes::SLL},     {"SLT", Opcodes::SLT},   {"SLTU", Opcodes::SLTU},
    {"XOR", Opcodes::XOR},     {"SRL", Opcodes::SRL},     {"SRA", Opcodes::SRA},   {"OR", Opcodes::OR},
    {"AND", Opcodes::AND},
};

export Opcodes OpcodeNameToVal(std::string const &str) { return strToOpcodeMap.at(str); }

export using Opcode = std::uint8_t;
