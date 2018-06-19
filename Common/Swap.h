// Copyright (c) 2012- PPSSPP Project / Dolphin Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0 or later versions.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official git repository and contact information can be found at
// https://github.com/hrydgard/ppsspp and http://www.ppsspp.org/.

#pragma once

#include <cstdlib>

#include "Common/CommonTypes.h"

// Android
#if defined(__ANDROID__)
#include <sys/endian.h>

#if _BYTE_ORDER == _LITTLE_ENDIAN && !defined(COMMON_LITTLE_ENDIAN)
#define COMMON_LITTLE_ENDIAN 1
#elif _BYTE_ORDER == _BIG_ENDIAN && !defined(COMMON_BIG_ENDIAN)
#define COMMON_BIG_ENDIAN 1
#endif

// GCC 4.6+
#elif __GNUC__ >= 5 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)

#if __BYTE_ORDER__ && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) && !defined(COMMON_LITTLE_ENDIAN)
#define COMMON_LITTLE_ENDIAN 1
#elif __BYTE_ORDER__ && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__) && !defined(COMMON_BIG_ENDIAN)
#define COMMON_BIG_ENDIAN 1
#endif

// LLVM/clang
#elif __clang__

#if __LITTLE_ENDIAN__ && !defined(COMMON_LITTLE_ENDIAN)
#define COMMON_LITTLE_ENDIAN 1
#elif __BIG_ENDIAN__ && !defined(COMMON_BIG_ENDIAN)
#define COMMON_BIG_ENDIAN 1
#endif

// MSVC
#elif defined(_MSC_VER) && !defined(COMMON_BIG_ENDIAN) && !defined(COMMON_LITTLE_ENDIAN)

#define COMMON_LITTLE_ENDIAN 1

#endif

// Worst case, default to little endian.
#if !COMMON_BIG_ENDIAN && !COMMON_LITTLE_ENDIAN
#define COMMON_LITTLE_ENDIAN 1
#endif

#ifdef _MSC_VER
inline unsigned long long bswap64(unsigned long long x) { return _byteswap_uint64(x); }
inline unsigned int bswap32(unsigned int x) { return _byteswap_ulong(x); }
inline unsigned short bswap16(unsigned short x) { return _byteswap_ushort(x); }
#elif defined(__DragonFly__) || defined(__FreeBSD__) || \
      defined(__NetBSD__) || defined(__OpenBSD__)
#include <sys/endian.h>
# ifdef __OpenBSD__
#define bswap16 swap16
#define bswap32 swap32
#define bswap64 swap64
# endif
#elif defined(__GNUC__)
#define bswap16 __builtin_bswap16
#define bswap32 __builtin_bswap32
#define bswap64 __builtin_bswap64
#else
// TODO: speedup
inline unsigned short bswap16(unsigned short x) { return (x << 8) | (x >> 8); }
inline unsigned int bswap32(unsigned int x) { return (x >> 24) | ((x & 0xFF0000) >> 8) | ((x & 0xFF00) << 8) | (x << 24); }
inline unsigned long long bswap64(unsigned long long x) { return ((unsigned long long)bswap32(x) << 32) | bswap32(x >> 32); }
#endif

inline float bswapf(float f) {
	union {
		float f;
		unsigned int u32;
	} dat1, dat2;

	dat1.f = f;
	dat2.u32 = bswap32(dat1.u32);

	return dat2.f;
}

inline double bswapd(double f) {
	union {
		double f;
		unsigned long long u64;
	} dat1, dat2;

	dat1.f = f;
	dat2.u64 = bswap64(dat1.u64);

	return dat2.f;
}

template <typename T, typename F>
struct swap_struct_t {
	typedef swap_struct_t<T, F> swapped_t;

protected:
	T value;

	static T swap(T v) {
		return F::swap(v);
	}
public:
	T const swap() const {
		return swap(value);
	}
	swap_struct_t() : value((T)0) {}
	swap_struct_t(const T &v): value(swap(v)) {}

	template <typename S>
	swapped_t& operator=(const S &source) {
		value = swap((T)source);
		return *this;
	}

	operator T() const { return swap(); }

	// v += 5
	swapped_t& operator+=(const swapped_t &i) {
		value = swap(swap() + i.swap());
		return *this;
	}
	template <typename S>
	swapped_t& operator+=(const S &i) {
		value = swap(swap() + (T)i);
		return *this;
	}
	// v -= 5
	swapped_t& operator-=(const swapped_t &i) {
		value = swap(swap() - i.swap());
		return *this;
	}
	template <typename S>
	swapped_t& operator-=(const S &i) {
		value = swap(swap() - (T)i);
		return *this;
	}

	// ++v
	swapped_t& operator++() {
		value = swap(swap()+1);
		return *this;
	}
	// --v
	swapped_t& operator--()  {
		value = swap(swap()-1);
		return *this;
	}

	// v++
	swapped_t operator++(int) {
		swapped_t old = *this;
		value = swap(swap()+1);
		return old;
	}
	// v--
	swapped_t operator--(int) {
		swapped_t old = *this;
		value = swap(swap()-1);
		return old;
	}
	// Comparaison
	// v == i
	bool operator==(const swapped_t &i) const {
		return swap() == i.swap();
	}
	template <typename S>
	bool operator==(const S &i) const {
		return swap() == i;
	}

	// v != i
	bool operator!=(const swapped_t &i) const {
		return swap() != i.swap();
	}
	template <typename S>
	bool operator!=(const S &i) const {
		return swap() != i;
	}

	// v > i
	bool operator>(const swapped_t &i) const {
		return swap() > i.swap();
	}
	template <typename S>
	bool operator>(const S &i) const {
		return swap() > i;
	}

	// v < i
	bool operator<(const swapped_t &i) const {
		return swap() < i.swap();
	}
	template <typename S>
	bool operator<(const S &i) const {
		return swap() < i;
	}

	// v >= i
	bool operator>=(const swapped_t &i) const {
		return swap() >= i.swap();
	}
	template <typename S>
	bool operator>=(const S &i) const {
		return swap() >= i;
	}

	// v <= i
	bool operator<=(const swapped_t &i) const {
		return swap() <= i.swap();
	}
	template <typename S>
	bool operator<=(const S &i) const {
		return swap() <= i;
	}

	// logical
	bool operator !() const {
		return !swap();
	}
	
	bool operator ||(const swapped_t  & b) const {
		return swap() || b.swap();
	}
	template <typename S>
	bool operator ||(const S & b) const {
		return swap() || b;
	}

	swapped_t& operator &=(const swapped_t &b) {
		value = swap(swap() & b.swap());
		return *this;
	}
	template <typename S>
	swapped_t& operator &=(const S b) {
		value = swap(swap() & b);
		return *this;
	}

	swapped_t& operator |=(const swapped_t &b) {
		value = swap(swap() | b.swap());
		return *this;
	}
	template <typename S>
	swapped_t& operator |=(const S &b) {
		value = swap(swap() | b);
		return *this;
	}

	swapped_t& operator ^=(const swapped_t &b) {
		value = swap(swap() ^ b.swap());
		return *this;
	}
	template <typename S>
	swapped_t& operator ^=(const S &b) {
		value = swap(swap() ^ b);
		return *this;
	}

	template <typename S>
	swapped_t& operator <<=(const S &b) const {
		value = swap(swap() << b);
		return *this;
	}

	template <typename S>
	swapped_t& operator >>=(const S &b) const {
		value = swap(swap() >> b);
		return *this;
	}
};

template <typename T>
struct swap_64_t {
	static T swap(T x) {
		return (T)bswap64(*(u64 *)&x);
	}
};

template <typename T>
struct swap_32_t {
	static T swap(T x) {
		return (T)bswap32(*(u32 *)&x);
	}
};

template <typename T>
struct swap_16_t {
	static T swap(T x) {
		return (T)bswap16(*(u16 *)&x);
	}
};

template <typename T>
struct swap_float_t {
	static T swap(T x) {
		return (T)bswapf(*(float *)&x);
	}
};

template <typename T>
struct swap_double_t {
	static T swap(T x) {
		return (T)bswapd(*(double *)&x);
	}
};

#if COMMON_LITTLE_ENDIAN
typedef u32 u32_le;
typedef u16 u16_le;
typedef u64 u64_le;

typedef s32 s32_le;
typedef s16 s16_le;
typedef s64 s64_le;

typedef float float_le;
typedef double double_le;

typedef swap_struct_t<u64, swap_64_t<u64>> u64_be;
typedef swap_struct_t<s64, swap_64_t<s64>> s64_be;

typedef swap_struct_t<u32, swap_32_t<u32>> u32_be;
typedef swap_struct_t<s32, swap_32_t<s32>> s32_be;

typedef swap_struct_t<u16, swap_16_t<u16>> u16_be;
typedef swap_struct_t<s16, swap_16_t<s16>> s16_be;

typedef swap_struct_t<float, swap_float_t<float> > float_be;
typedef swap_struct_t<double, swap_double_t<double> > double_be;
#else

typedef swap_struct_t<u64, swap_64_t<u64>> u64_le;
typedef swap_struct_t<s64, swap_64_t<s64>> s64_le;

typedef swap_struct_t<u32, swap_32_t<u32>> u32_le;
typedef swap_struct_t<s32, swap_32_t<s32>> s32_le;

typedef swap_struct_t<u16, swap_16_t<u16>> u16_le;
typedef swap_struct_t<s16, swap_16_t<s16>> s16_le;

typedef swap_struct_t<float, swap_float_t<float> > float_le;
typedef swap_struct_t<double, swap_double_t<double> > double_le;

typedef u32 u32_be;
typedef u16 u16_be;
typedef u64 u64_be;

typedef s32 s32_be;
typedef s16 s16_be;
typedef s64 s64_be;

typedef float float_be;
typedef double double_be;
#endif
