/*  This file is part of the Vc library. {{{

    Copyright (C) 2013 Matthias Kretz <kretz@kde.org>

    Vc is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    Vc is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with Vc.  If not, see <http://www.gnu.org/licenses/>.

}}}*/

#ifndef VC_COMMON_SIMD_ARRAY_H
#define VC_COMMON_SIMD_ARRAY_H

#include <type_traits>
#include <array>

#include "simd_array_data.h"
#include "simd_mask_array.h"
#include "macros.h"

namespace Vc_VERSIONED_NAMESPACE
{

// === having simd_array<T, N> in the Vc namespace leads to a ABI bug ===
//
// simd_array<double, 4> can be { double[4] }, { __m128d[2] }, or { __m256d } even though the type
// is the same.
// The question is, what should simd_array focus on?
// a) A type that makes interfacing between different implementations possible?
// b) Or a type that makes fixed size SIMD easier and efficient?
//
// a) can be achieved by using a union with T[N] as one member. But this may have more serious
// performance implications than only less efficient parameter passing (because compilers have a
// much harder time wrt. aliasing issues). Also alignment would need to be set to the sizeof in
// order to be compatible with targets with larger alignment requirements.
// But, the in-memory representation of masks is not portable. Thus, at the latest with AVX-512,
// there would be a problem with requiring simd_mask_array<T, N> to be an ABI compatible type.
// AVX-512 uses one bit per boolean, whereas SSE/AVX use sizeof(T) Bytes per boolean. Conversion
// between the two representations is not a trivial operation. Therefore choosing one or the other
// representation will have a considerable impact for the targets that do not use this
// representation. Since the future probably belongs to one bit per boolean representation, I would
// go with that choice.
//
// b) requires that simd_array<T, N> != simd_array<T, N> if
// simd_array<T, N>::vector_type != simd_array<T, N>::vector_type
//
// Therefore use simd_array<T, N, V>, where V follows from the above.
template <typename T,
          std::size_t N,
          typename VectorType = typename Common::select_best_vector_type<N,
#ifdef VC_IMPL_AVX
                                                                         Vc::Vector<T>,
                                                                         Vc::SSE::Vector<T>,
                                                                         Vc::Scalar::Vector<T>
#elif defined(VC_IMPL_Scalar)
                                                                         Vc::Vector<T>
#else
                                                                         Vc::Vector<T>,
                                                                         Vc::Scalar::Vector<T>
#endif
                                                                         >::type>
class simd_array
{
    static_assert(std::is_same<T,   double>::value ||
                  std::is_same<T,    float>::value ||
                  std::is_same<T,  int32_t>::value ||
                  std::is_same<T, uint32_t>::value ||
                  std::is_same<T,  int16_t>::value ||
                  std::is_same<T, uint16_t>::value, "simd_array<T, N> may only be used with T = { double, float, int32_t, uint32_t, int16_t, uint16_t }");

    static_assert((N & (N - 1)) == 0, "simd_array<T, N> must be used with a power of two value for N.");

public:
    using vector_type = VectorType;
    typedef T value_type;
    typedef simd_mask_array<T, N, vector_type> mask_type;

    static constexpr std::size_t size = N;
    static constexpr std::size_t register_count = size > vector_type::Size ? size / vector_type::Size : 1;

    // Vc compat:
    typedef mask_type Mask;
    typedef value_type EntryType;
    static constexpr std::size_t Size = size;

    // zero init
    simd_array() = default;

    // default copy ctor/operator
    simd_array(const simd_array &) = default;
    simd_array(simd_array &&) = default;
    simd_array &operator=(const simd_array &) = default;

    // broadcast
    Vc_ALWAYS_INLINE simd_array(value_type a) : d(a) {}

    explicit Vc_ALWAYS_INLINE simd_array(VectorSpecialInitializerZero::ZEnum x) : d(vector_type(x)) {}
    explicit Vc_ALWAYS_INLINE simd_array(VectorSpecialInitializerOne::OEnum x) : d(vector_type(x)) {}
    explicit Vc_ALWAYS_INLINE simd_array(VectorSpecialInitializerIndexesFromZero::IEnum x) : d(x) {}

    static Vc_ALWAYS_INLINE simd_array Zero() { return simd_array(VectorSpecialInitializerZero::Zero); }
    static Vc_ALWAYS_INLINE simd_array One() { return simd_array(VectorSpecialInitializerOne::One); }
    static Vc_ALWAYS_INLINE simd_array IndexesFromZero() { return simd_array(VectorSpecialInitializerIndexesFromZero::IndexesFromZero); }
    static Vc_ALWAYS_INLINE simd_array Random()
    {
        simd_array r;
        r.d.assign(&vector_type::Random);
        return r;
    }

    // load ctors
    explicit Vc_ALWAYS_INLINE simd_array(const value_type *x) : d(x) {}
    template<typename Flags = DefaultLoadTag> explicit Vc_ALWAYS_INLINE simd_array(const value_type *x, Flags flags = Flags())
        : d(x, flags) {}
    template<typename OtherT, typename Flags = DefaultLoadTag> explicit Vc_ALWAYS_INLINE simd_array(const OtherT *x, Flags flags = Flags())
        : d(x, flags) {}

    ///////////////////////////////////////////////////////////////////////////////////////////
    // load member functions
    Vc_ALWAYS_INLINE void load(const value_type *x) {
        d.load(x, DefaultLoadTag());
    }
    template<typename Flags>
    Vc_ALWAYS_INLINE void load(const value_type *x, Flags f) {
        d.load(x, f);
    }
    template<typename U, typename Flags = DefaultLoadTag>
    Vc_ALWAYS_INLINE void load(const U *x, Flags f = Flags()) {
        d.load(x, f);
    }

    // implicit casts
    template<typename U> Vc_ALWAYS_INLINE simd_array(const simd_array<U, N> &x) {
        d[0] = x.data(0);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////
    // store member functions
    Vc_ALWAYS_INLINE void store(value_type *x) {
        d.store(x, DefaultStoreTag());
    }
    template <typename Flags> Vc_ALWAYS_INLINE void store(value_type *x, Flags f)
    {
        d.store(x, f);
    }
    template <typename U, typename Flags = DefaultStoreTag>
    Vc_ALWAYS_INLINE void store(U *x, Flags f = Flags())
    {
        d.store(x, f);
    }


#define VC_COMPARE_IMPL(op)                                                                        \
    Vc_ALWAYS_INLINE Vc_PURE mask_type operator op(const simd_array &x) const                      \
    {                                                                                              \
        mask_type r;                                                                               \
        r.d.assign(d, x.d, &vector_type::operator op);                                             \
        return r;                                                                                  \
    }
    VC_ALL_COMPARES(VC_COMPARE_IMPL)
#undef VC_COMPARE_IMPL

#define VC_OPERATOR_IMPL(op)                                                                       \
    Vc_ALWAYS_INLINE simd_array &operator op##=(const simd_array & x)                              \
    {                                                                                              \
        d op## = x.d;                                                                              \
        return *this;                                                                              \
    }                                                                                              \
    inline simd_array operator op(const simd_array &x) const                                       \
    {                                                                                              \
        simd_array r = *this;                                                                      \
        r op## = x;                                                                                \
        return r;                                                                                  \
    }
    VC_ALL_BINARY     (VC_OPERATOR_IMPL)
    VC_ALL_ARITHMETICS(VC_OPERATOR_IMPL)
    VC_ALL_SHIFTS     (VC_OPERATOR_IMPL)
#undef VC_OPERATOR_IMPL

    value_type operator[](std::size_t i) const {
        typedef value_type TT Vc_MAY_ALIAS;
        auto m = reinterpret_cast<const TT *>(&d);
        return m[i];
    }

    //////////////////////
    // unary operators

    //prefix
    Vc_INTRINSIC simd_array &operator++()
    {
        d.call(static_cast<vector_type &(vector_type::*)()>(&vector_type::operator++));
        return *this;
    }
    Vc_INTRINSIC simd_array &operator--()
    {
        d.call(static_cast<vector_type &(vector_type::*)()>(&vector_type::operator--));
        return *this;
    }
    // postfix
    Vc_INTRINSIC simd_array operator++(int)
    {
        const auto r = *this;
        d.call(static_cast<vector_type &(vector_type::*)()>(&vector_type::operator++));
        return r;
    }
    Vc_INTRINSIC simd_array operator--(int)
    {
        const auto r = *this;
        d.call(static_cast<vector_type &(vector_type::*)()>(&vector_type::operator--));
        return r;
    }

    Vc_INTRINSIC mask_type operator!() const
    {
        mask_type r;
        r.d.assign(d, &vector_type::operator!);
        return r;
    }

    // TODO: perform integral promotion, simply return simd_array<decltype(-std::declval<T>()), N>
    Vc_INTRINSIC simd_array operator-() const
    {
        simd_array r;
        r.d.assign(d, static_cast<vector_type (vector_type::*)() const>(&vector_type::operator-));
        return r;
    }

private:
    Common::ArrayData<vector_type, register_count> d;
};

} // namespace Vc_VERSIONED_NAMESPACE

#include "undomacros.h"

#endif // VC_COMMON_SIMD_ARRAY_H
