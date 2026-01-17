/* Copyright (c) 2026, Yaozeran, Zhangchenzhi, Zhangsenyao
 * 
 * This file implements cpp's sfinae metaprogramming idioms to search for 
 *   implemented member function serdes, non member function serdes, serialize 
 *   and deserialize methods, with corresponding order preference.
 */

#ifndef TRAITS_H
#define TRAITS_H

#include <type_traits>

#include "access.h"

namespace serdes
{

  namespace detail
  {  
  enum class sfinae {};

  template<bool h, bool... t>
  struct meta_bool_and : std::integral_constant<bool, h && meta_bool_and<t...>::value> {};
  template<bool b>
  struct meta_bool_and<b> : std::integral_constant<bool, b> {};

  template<bool... conds>
  struct enable_if_helper : std::enable_if<meta_bool_and<conds...>::value, sfinae> {};
  } // namespace detail

static const detail::sfinae sfinae = {};

template <bool... conds>
using enable_if = detail::enable_if_helper<conds...>::type;

#define MAKE_HAS_MEMBER_TEST(test_name, name) \
  namespace detail  \
  { \
  template<class t, class s>  \
  struct has_member_##name##_impl { \
    template<class tt, class ss>  \
    static auto test(int) -> decltype(serdes::access::member_##name(std::declval<ss&>(), std::declval<tt&>()), std::true_type()); \
    template<class, class> \
    static std::false_type test(...); \
    static const bool value = std::is_same<decltype(test<t, s>(0)), std::true_type>::value; \
  }; \
  } \
  template<class t, class s> \
  struct has_member_##test_name : std::integral_constant<bool, detail::has_member_##name##_impl<t, s>::value> {};

#define MAKE_HAS_NON_MEMBER_TEST(test_name, func) \
  namespace detail \
  { \
  template<class t, class s> \
  struct has_non_member_##func##_impl { \
    template<class tt, class ss> \
    static auto test(int) -> decltype(func(std::declval<ss&>(), std::declval<tt&>()), std::true_type()); \
    template<class, class> \
    static std::false_type test(...); \
    static const bool value = std::is_same<decltype(test<t, s>(0)), std::true_type>::value; \
  }; \
  } \
  template<class t, class s> \
  struct has_non_member_##test_name : std::integral_constant<bool, detail::has_non_member_##func##_impl<t, s>::value> {};

MAKE_HAS_MEMBER_TEST(serdes, serdes);
MAKE_HAS_NON_MEMBER_TEST(serdes, serdes);

  namespace detail
  {
  template<class t, class s>
  struct has_non_member_serialize_impl {
    template<class tt, class ss>
    static auto test(int) -> decltype(serialize(std::declval<ss&>(), std::declval<const tt&>()), std::true_type());
    template<class, class>
    static std::false_type test(...);
    static const bool value = std::is_same<decltype(test<t, s>(0)), std::true_type>::value;
  }; 
  } // namespace detail
template<class t, class s>
struct has_non_member_serialize : std::integral_constant<bool, detail::has_non_member_serialize_impl<t,s>::value> {};

  namespace detail
  {
  template<class t, class s>
  struct has_non_member_deserialize_impl {
    template<class tt, class ss>
    static auto test(int) -> decltype(deserialize(std::declval<ss&>(), std::declval<tt&>()), std::true_type());
    template<class, class>
    static std::false_type test(...);

    static const bool value = std::is_same<decltype(test<t, s>(0)), std::true_type>::value;
  };  
  } // namespace detail
template<class t, class s>
struct has_non_member_deserialize : std::integral_constant<bool, detail::has_non_member_deserialize_impl<t,s>::value> {};

  namespace detail
  {
  template<typename t, class serializer>
  struct count_serialize_impl : std::integral_constant<int, 
    has_member_serdes<t, serializer>::value + 
    has_non_member_serdes<t, serializer>::value + 
    has_non_member_serialize<t, serializer>::value> {};
  
  template<typename t, class deserializer>
  struct count_deserialize_impl : std::integral_constant<int,
    has_member_serdes<t, deserializer>::value + 
    has_non_member_serdes<t, deserializer>::value +
    has_non_member_deserialize<t, deserializer>::value> {};
  } // namespace detail

template<typename t, class serializer>
struct is_serializable : std::integral_constant<bool, detail::count_serialize_impl<t, serializer>::value == 1> {};
template<typename t, class deserializer>
struct is_deserializable : std::integral_constant<bool, detail::count_deserialize_impl<t, deserializer>::value == 1> {};

} // namespace cser

#endif /* TRAITS_H */