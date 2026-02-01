#ifndef SERDES_H
#define SERDES_H

#include <type_traits>
#include <string>

#include "core/currency.h"
#include "rpc/protocol.h"

template<typename T>
inline typename std::enable_if<std::is_arithmetic<T>::value, size_t>::type
serialize(const T& obj, char* out) {
  std::memcpy(out, std::addressof(obj), sizeof(obj));
  return sizeof(obj);
}

template<typename T>
inline typename std::enable_if<std::is_arithmetic<T>::value, size_t>::type
deserialize(const char* in, T& obj) {
  std::memcpy(std::addressof(obj), in, sizeof(obj));
  return sizeof(obj);
}

template<typename T, size_t N>
inline size_t serialize(const std::array<T, N>& arr, char* out) {
  char* ptr = out;
  size_t i = 0;
  for (const auto& elem : arr) {
    i += serialize(elem, out + i);
  }
  return i;
}

template<typename T, std::size_t N>
inline size_t deserialize(const char* in, std::array<T, N>& arr) {
  const char* ptr = in;
  size_t i = 0;
  for (auto& elem : arr) {
    i += deserialize(ptr + i, elem);
  }
  return i;
}

inline size_t serialize(const std::string& str, char* out) {
  size_t s = str.size();
  serialize(s, out);
  std::memcpy(out + sizeof(s), str.data(), str.size());
  return sizeof(s) + s;
}

inline size_t deserialize(const char* in, std::string& str) {
  size_t s;
  deserialize(in, s); // size tag of the str
  str.assign(in + sizeof(s), s);
  return sizeof(s) + s;
}

inline size_t serialize(const op_code& c, char* out) {
  int v = static_cast<int>(c);
  serialize(v, out);
  return sizeof(v);
}

inline size_t deserialize(const char* in, op_code& c) {
  int v;
  deserialize(in, v);
  std::optional<op_code> r = int_to_op_code(v);
  if (!r) {
    // to do
  } else {
    c = *r;
  }
  return sizeof(v);
}

inline size_t serialize(const status_code& c, char* out) {
  int v = static_cast<int>(c);
  serialize(v, out);
  return sizeof(v);
}

inline size_t deserialize(const char* in, status_code& c) {
  int v;
  deserialize(in, v);
  std::optional<status_code> r = int_to_status_code(v);
  if (!r) {
    // to do
  } else {
    c = *r;
  }
  return sizeof(v);
}

inline size_t serialize(const currency& c, char* out) {
  size_t i = 0;
  std::string str = currency_to_str(c);
  i += serialize(str, out);
  return i;
}

inline size_t deserialize(const char* in, currency& c) {
  size_t i = 0;
  std::string str;
  i += deserialize(in, str);
  std::optional<currency> r = str_to_currency(str);
  if (!r) {
    // to do
  } else {
    c = *r;
  }
  return i;
}

#endif /* SERDES_H */