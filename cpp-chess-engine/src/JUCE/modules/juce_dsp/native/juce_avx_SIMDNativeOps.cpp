/*
  ==============================================================================

   This file is part of the JUCE 6 technical preview.
   Copyright (c) 2020 - Raw Material Software Limited

   You may use this code under the terms of the GPL v3
   (see www.gnu.org/licenses).

   For this technical preview, this file is not subject to commercial licensing.

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace juce
{
    namespace dsp
    {
        DEFINE_AVX_SIMD_CONST (int32_t, float, kAllBitsSet)     = { -1, -1, -1, -1, -1, -1, -1, -1 };
        DEFINE_AVX_SIMD_CONST (int32_t, float, kEvenHighBit)    = { static_cast<int32_t>(0x80000000), 0, static_cast<int32_t>(0x80000000), 0, static_cast<int32_t>(0x80000000), 0, static_cast<int32_t>(0x80000000), 0 };
        DEFINE_AVX_SIMD_CONST (float, float, kOne)              = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

        DEFINE_AVX_SIMD_CONST (int64_t, double, kAllBitsSet)    = { -1, -1, -1, -1 };
        DEFINE_AVX_SIMD_CONST (int64_t, double, kEvenHighBit)   = { static_cast<int64_t> (0x8000000000000000), 0, static_cast<int64_t> (0x8000000000000000), 0 };
        DEFINE_AVX_SIMD_CONST (double, double, kOne)            = { 1.0, 1.0, 1.0, 1.0 };

        DEFINE_AVX_SIMD_CONST (int8_t, int8_t, kAllBitsSet)     = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

        DEFINE_AVX_SIMD_CONST (uint8_t, uint8_t, kAllBitsSet)   = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
        DEFINE_AVX_SIMD_CONST (uint8_t, uint8_t, kHighBit)      = { 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80 };

        DEFINE_AVX_SIMD_CONST (int16_t, int16_t, kAllBitsSet)   = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

        DEFINE_AVX_SIMD_CONST (uint16_t, uint16_t, kAllBitsSet) = { 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff };
        DEFINE_AVX_SIMD_CONST (uint16_t, uint16_t, kHighBit)    = { 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000 };

        DEFINE_AVX_SIMD_CONST (int32_t, int32_t, kAllBitsSet)   = { -1, -1, -1, -1, -1, -1, -1, -1 };

        DEFINE_AVX_SIMD_CONST (uint32_t, uint32_t, kAllBitsSet) = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff };
        DEFINE_AVX_SIMD_CONST (uint32_t, uint32_t, kHighBit)    = { 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000 };

        DEFINE_AVX_SIMD_CONST (int64_t, int64_t, kAllBitsSet)   = { -1LL, -1LL, -1LL, -1LL };

        DEFINE_AVX_SIMD_CONST (uint64_t, uint64_t, kAllBitsSet) = { 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL };
        DEFINE_AVX_SIMD_CONST (uint64_t, uint64_t, kHighBit)    = { 0x8000000000000000ULL, 0x8000000000000000ULL, 0x8000000000000000ULL, 0x8000000000000000ULL };
    }
}