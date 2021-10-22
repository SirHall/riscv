module;

#include <string>

export module Util;

import Defs;

namespace Util
{
    export template <bool, typename T1, typename T2>
    struct IsCond
    {
        typedef T1 type;
    };

    export template <typename T1, typename T2>
    struct IsCond<false, T1, T2>
    {
        typedef T2 type;
    };

    export template <typename AT, typename BT>
    struct LargerType
    {
        typedef typename IsCond<(sizeof(AT) > sizeof(BT)), AT, BT>::type type;
    };

    export template <typename AT, typename BT>
    struct SmallerType
    {
        typedef typename IsCond<(sizeof(AT) < sizeof(BT)), AT, BT>::type type;
    };

#pragma region Misc

    // Swaps two integers around using XOR without a temporary variable.
    // Make sure 'a' and 'b' do not refer to the same variable!
    // TODO: Test to see if this is actually faster than just using a temporary.
    export template <typename T>
    inline void Swap(T &a, T &b)
    {
        a ^= b;
        b ^= a;
        a ^= b;
    }

#pragma endregion

#pragma region Flood Fill

    // Flood fills the first LSB in a u8 to the rest of the bits
    // Thankyou abcq2 for this stroke of genius
    export template <typename T>
    inline T FloodFill(T v)
    {
        // TODO: Test `T(0) - (v & T(1))`
        return T(1) - (~v & T(1));
    }

#pragma endregion

#pragma region Sign Extension

    // Sign-extend an input bit-width integer to a larger type, using the leading bit
    export template <typename IT, typename OT>
    inline OT ExtendSigned(IT const v)
    {
        return OT(v) & FloodFill((OT(v) & ~(~OT(0) >> 1)) >> ((sizeof(OT) * 8) - 1));
    }

    // Sign-extend an input bit-width integer to a larger type, using a specified bit position to extend by
    export template <unsigned B, typename IT, typename OT>
    inline OT ExtendSignedSpec(IT const v)
    {
        return OT(v) & FloodFill((OT(v) & (OT(1) << B)) >> B);
    }

    // Sign-extend an input bit-width integer to a larger type, using a specified bit position to extend by -
    // non-template version
    export template <typename IT, typename OT>
    inline OT ExtendSignedSpecNT(IT const v, u8 const B)
    {
        return OT(v) & FloodFill((OT(v) & (OT(1) << B)) >> B);
    }

    // Sign Extend 8 to 32
    export inline u32 se8to32(u8 v) { return ExtendSigned<u8, u32>(v); }

    // Sign Extend 16 to 32
    export inline u32 se16to32(u16 v) { return ExtendSigned<u16, u32>(v); }

    // Sign Extend 8 to 64
    export inline u64 se8to64(u8 v) { return ExtendSigned<u8, u64>(v); }

    // Sign Extend 16 to 64
    export inline u64 se16to64(u16 v) { return ExtendSigned<u16, u64>(v); }

    // Sign Extend 32 to 64
    export inline u64 se32to64(u32 v) { return ExtendSigned<u32, u64>(v); }

#pragma endregion

#pragma region Extract

    // Extract an integer from low and high bit positions in a source integer
    export template <unsigned LSB, unsigned MSB, typename IT, typename OT>
    inline OT Extract(IT const v)
    {
        using Lrg = LargerType<IT, OT>::type;
        return (OT)(Lrg(v) >> LSB) & ~(~Lrg(0) << (MSB - LSB + 1));
    }

#pragma endregion

#pragma region Bitmask Helper

    export template <unsigned LSB, unsigned MSB, typename T>
    inline T BitMask()
    {
        const auto maxBitIndex = (sizeof(T) * 8) - 1;
        // May need to reduce the instructions for this one
        return ((~T(0) << maxBitIndex - MSB) >> ((maxBitIndex - MSB) + LSB)) << LSB;
    }

#pragma endregion

#pragma region Rewrap

    export template <typename IT, typename OT>
    inline OT &Rewrap(IT &v)
    {
        return *(OT *)(&v);
    }

    export template <typename IT, typename OT>
    inline OT const &Rewrap(IT const &v)
    {
        return *(OT const *)(&v);
    }

#pragma endregion

#pragma region Byte Merging

    // RISC-V is little endian

    // Extremely explicit
    export inline u16 Merge16(u8 v0, u8 v1) { return u16(v0) | u16(v1) << 8; }

    export inline u32 Merge32(u8 v0, u8 v1, u8 v2, u8 v3)
    {
        return u32(v0) | u32(v1) << 8 | u32(v2) << 16 | u32(v3) << 24;
    }

    export inline u32 Merge64(u8 v0, u8 v1, u8 v2, u8 v3, u8 v4, u8 v5, u8 v6, u8 v7)
    {
        return u64(v0) | u64(v1) << 8 | u64(v2) << 16 | u64(v3) << 24 | u64(v4) << 32 | u64(v5) << 40 | u64(v6) << 48 |
               u64(v7) << 56;
    }

    export template <unsigned B, typename T>
    inline u8 GetByte(T v)
    {
        return u8(v >> (8 * B));
    }

    // Non-templated and very explicit versions, should probably follow DRY
    export inline u8 GetByte16NT(u16 v, u8 B) { return u8(v >> (8 * B)); }
    export inline u8 GetByte32NT(u32 v, u8 B) { return u8(v >> (8 * B)); }
    export inline u8 GetByte64NT(u64 v, u8 B) { return u8(v >> (8 * B)); }

#pragma endregion

} // namespace Util
