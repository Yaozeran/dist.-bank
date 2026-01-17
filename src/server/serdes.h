/* Copyright (c) 2026, Yaozeran, Zhangchenzhi, Zhangsenyao
 * 
 * This document implements global serialize and deserialize impl for 
 *   different types: current supported arithmetic, baisc strings, array
 */

#ifndef SERDES_H
#define SERDES_H

#include <cstdlib>
#include <array>
#include <string>

#include "serializer.h"
#include "deserializer.h"

namespace serdes
{

template<class t>
struct binary_data {
  /* cannot implicitly convert const ptr to void* , use pt */
  using pt = typename std::conditional<
    std::is_const<typename std::remove_pointer<typename std::remove_reference<t>::type>::type>::value,
    const void*, void*>::type;
  pt data_;
  uint64_t size_;
  binary_data(t&& d, uint64_t s) : data_(std::forward<t>(d)), size_(s) {}
};

template<class t>
inline binary_data<t> make_binary_data(t&& data, size_t size) {
  return {std::forward<t>(data), size};
}

template<typename t>
class size_tag 
{
 using ty = typename std::conditional<std::is_lvalue_reference<t>::value, t, typename std::decay<t>::type>::type;
 public:
  ty size_;
  size_tag(t&& sz) : size_(std::forward<t>(sz)) {}
 private:
  size_tag& operator=(const size_tag&) = delete;
};

template<class t>
inline size_tag<t> make_size_tag(t sz) {
  return {std::forward<t>(sz)};
}

template<typename t>
inline typename std::enable_if<std::is_arithmetic<t>::value, void>::type
serialize(byte_serializer& ser, const t& v) {
  ser.write(std::addressof(v), sizeof(v));
}

template<typename t>
inline typename std::enable_if<std::is_arithmetic<t>::value, void>::type
deserialize(byte_deserializer& des, t& v) {
  des.read(std::addressof(v), sizeof(v));
}

template<class t>
void serialize(byte_serializer& ser, const binary_data<t>& bd) {
  ser.write(bd.data_, static_cast<std::streamsize>(bd.size_));
}

template<class t>
void deserialize(byte_deserializer& des, binary_data<t>& bd) {
  des.read(bd.data_, static_cast<std::streamsize>(bd.size_));
}

template<class t>
void serialize(byte_serializer& ser, const size_tag<t>& tag) {
  ser(tag.size_);
}

template<class t>
void deserialize(byte_deserializer& des, size_tag<t>& tag) {
  des(tag.size_);
}

template<class serializer, class t, size_t n>
inline typename std::enable_if<
  is_serializable<binary_data<t>, serializer>::value && 
  std::is_arithmetic<t>::value, void>::type 
serialize(serializer &ser, const std::array<t, n> &arr) {
  ser( make_binary_data(arr.data(), n * sizeof(t)) );
}

template<class deserializer, class t, size_t n>
inline typename std::enable_if<
  is_deserializable<binary_data<t>, deserializer>::value && 
  std::is_arithmetic<t>::value, void>::type
deserialize(deserializer &des, std::array<t, n> &arr) {
  des( make_binary_data(arr.data(), n * sizeof(t)) );
}

template<class serializer, class char_t, class traits, class alloc>
inline typename std::enable_if<is_serializable<binary_data<char_t>, serializer>::value, void>::type
serialize(serializer &ser, const std::basic_string<char_t, traits, alloc> &str) {
  ser( make_size_tag(str.size()) );
  ser( make_binary_data(const_cast<char_t*>(str.data()), str.size() * sizeof(char_t)) );
}

template<class deserializer, class char_t, class traits, class alloc>
inline typename std::enable_if<is_deserializable<binary_data<char_t>, deserializer>::value, void>::type
deserialize(deserializer &des, std::basic_string<char_t, traits, alloc> &str) {
  size_t sz;
  size_tag<size_t> tag = make_size_tag(sz);  // <-- use a named variable (lvalue) instead of temporary
  des( tag );
  str.resize(tag.size_);
  des( make_binary_data(const_cast<char_t*>(str.data()), str.size() * sizeof(char_t)) );
}

} // namespace cser

#endif /* SERIALIZE_H */