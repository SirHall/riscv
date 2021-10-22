module;

#include <array>
#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

export module Assembler;

import InstructionProcessing;
import RISCVDefs;
import Util;
import RISCV.Opcodes;

namespace Assembler
{

#pragma region Type Declarations

    // We will try to never duplicate any portion of the inputted string
    using It = std::string::const_iterator;

    struct Label
    {
        std::string *name; // We don't want to make the SymbolData union too big, and can't store a string in a union
    };

    struct RType
    {
        Opcodes opcode;
        Sel     rd;
        Sel     rs1;
        Sel     rs2;
    };

    struct IType
    {
        Opcodes opcode;
        Sel     rd;
        Sel     rs1;
        Reg     imm;
    };

    struct SType
    {
        Opcodes opcode;
        Sel     rs1;
        Sel     rs2;
        Reg     imm;
    };

    struct BType
    {
        Opcodes opcode;
        Sel     rs1;
        Sel     rs2;
        Reg     imm;
    };

    struct UType
    {
        Opcodes opcode;
        Sel     rd;
        Reg     imm;
    };

    struct JType
    {
        Opcodes opcode;
        Sel     rd;
        Reg     imm;
    };

    union SymbolData
    {
        Label label;
        RType r;
        IType i;
        SType s;
        UType u;
        BType b;
        JType j;
    };

    enum class SymbolTag
    {
        Label,
        R,
        I,
        S,
        B,
        U,
        J,
    };

    struct Symbol
    {
        SymbolTag  tag;
        SymbolData dat;
    };

    using SymbolList = std::vector<Symbol>;

    void CleanupSymbolList(SymbolList &sl)
    {
        for (auto &el : sl)
            if (el.tag == SymbolTag::Label)
                delete el.dat.label.name;
    }

#pragma endregion

    // It is possible for a single instruction to expand into a multiple base instructions, so we return a vector of
    // instructions
    export SymbolList Compile(std::string const &str)
    {
        SymbolList instrs;

        std::istringstream strs(str);

        for (std::string line; std::getline(strs, line);) // Line by line
        {
            std::istringstream wordStream(line); // Don't know how slow doing this is
            std::string        main;             // This could either be and instruction or a label
            std::string        rd;
            std::string        rs1;
            std::string        rs2;
            std::string        imm;

            wordStream >> main;

            if (main[main.size() - 1] == ':')
            {
                // This is a label
                // instrs.push_back();
            }
        }

        return instrs;
    }
} // namespace Assembler
