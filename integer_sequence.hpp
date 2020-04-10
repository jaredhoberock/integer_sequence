// Copyright (c) 2020, NVIDIA CORPORATION. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include <cstdint>
#include <type_traits>


// allow the user to define a namespace for these functions
#if !defined(INTEGER_SEQUENCE_NAMESPACE)

#  if defined(INTEGER_SEQUENCE_NAMESPACE_OPEN_BRACE) or defined(INTEGER_SEQUENCE_NAMESPACE_CLOSE_BRACE)
#    error "All or none of INTEGER_SEQUENCE_NAMESPACE, INTEGER_SEQUENCE_NAMESPACE_OPEN_BRACE, and INTEGER_SEQUENCE_NAMESPACE_CLOSE_BRACE must be defined."
#  endif

#  define INTEGER_SEQUENCE_NAMESPACE
#  define INTEGER_SEQUENCE_NAMESPACE_OPEN_BRACE
#  define INTEGER_SEQUENCE_NAMESPACE_CLOSE_BRACE
#  define INTEGER_SEQUENCE_NAMESPACE_NEEDS_UNDEF

#else

#  if !defined(INTEGER_SEQUENCE_NAMESPACE_OPEN_BRACE) or !defined(INTEGER_SEQUENCE_NAMESPACE_CLOSE_BRACE)
#    error "All or none of INTEGER_SEQUENCE_NAMESPACE, INTEGER_SEQUENCE_NAMESPACE_OPEN_BRACE, and INTEGER_SEQUENCE_NAMESPACE_CLOSE_BRACE must be defined."
#  endif

#endif


// allow the user to define a singly-nested namespace for private implementation details
#if !defined(INTEGER_SEQUENCE_DETAIL_NAMESPACE)
#  define INTEGER_SEQUENCE_DETAIL_NAMESPACE detail
#  define INTEGER_SEQUENCE_DETAIL_NAMESPACE_NEEDS_UNDEF
#endif


INTEGER_SEQUENCE_NAMESPACE_OPEN_BRACE


template<class T, T... Integers>
struct integer_sequence
{
  using value_type = T;
  static_assert(std::is_integral<T>::value, "integer_sequence can only be instantiated with an integral type" );
  static constexpr std::size_t size() noexcept { return sizeof...(Integers); }
};


template<std::size_t... Indices>
using index_sequence = integer_sequence<std::size_t, Indices...>;


namespace INTEGER_SEQUENCE_DETAIL_NAMESPACE
{


template<class T, T start, T end, class IntegerSequence>
struct make_integer_sequence_unchecked;

template<class T, T start, T end, T... Indices>
struct make_integer_sequence_unchecked<T, start, end, integer_sequence<T, Indices...>>
{
  using type = typename make_integer_sequence_unchecked<
    T, start+1, end,
    integer_sequence<T, Indices..., start>
  >::type;
};


template<class T, T end, T ...Indices>
struct make_integer_sequence_unchecked<T, end, end, integer_sequence<T, Indices...>>
{
  using type = integer_sequence<T, Indices...>;
};


template<class T, T N>
struct make_integer_sequence_impl
{
  static_assert(std::is_integral<T>::value, "make_integer_sequence can only be instantiated with an integral type" );
  static_assert(0 <= N, "make_integer_sequence input shall not be negative");
  using type = typename make_integer_sequence_unchecked<
    T, 0, N, integer_sequence<T>
  >::type;
};


} // end INTEGER_SEQUENCE_DETAIL_NAMESPACE


template<class T, T N>
using make_integer_sequence = typename INTEGER_SEQUENCE_DETAIL_NAMESPACE::make_integer_sequence_impl<T,N>::type;


template<std::size_t N>
using make_index_sequence = make_integer_sequence<std::size_t, N>;


template<class... Types>
using index_sequence_for = make_index_sequence<sizeof...(Types)>;


INTEGER_SEQUENCE_NAMESPACE_CLOSE_BRACE


#ifdef INTEGER_SEQUENCE_NAMESPACE_NEEDS_UNDEF
#  undef INTEGER_SEQUENCE_NAMESPACE
#  undef INTEGER_SEQUENCE_NAMESPACE_OPEN_BRACE
#  undef INTEGER_SEQUENCE_NAMESPACE_CLOSE_BRACE
#  undef INTEGER_SEQUENCE_NAMESPACE_NEEDS_UNDEF
#endif

#ifdef INTEGER_SEQUENCE_DETAIL_NAMESPACE_NEEDS_UNDEF
#  undef INTEGER_SEQUENCE_DETAIL_NAMESPACE
#  undef INTEGER_SEQUENCE_DETAIL_NAMESPACE_NEEDS_UNDEF
#endif

