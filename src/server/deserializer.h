/* Copyright (c) 2026, Yaozeran, Zhangchenzhi, Zhangsenyao
 * 
 * This file implements deserializer classes.
 */

#ifndef DESERIALIZER_H
#define DESERIALIZER_H

#include <sstream>
#include <iostream>

namespace serdes
{

/*
 * Base class: transform a serialized stream into an obj.
 */
template<class deserializer_impl>
class deserializer
{
 public:
  deserializer(deserializer_impl* const derived) : self(derived) {}
  
  template<typename... types> 
  inline deserializer& operator() (types&& ... objs) {
    self->process(std::forward<types>(objs)...);
    return *self;
  }

 private:
  deserializer_impl* self;

  template<typename f, typename... types>
  inline void process(f&& first, types&&... others) {
    self->process(std::forward<f>(first));
    self->process(std::forward<types>(others)...);
  }
  
  template<typename t>
  inline void process(t&& obj) {
    self->process_impl(obj);
  }

  template<typename t, enable_if<has_member_serdes<t, deserializer_impl>::value> = sfinae>
  deserializer_impl& process_impl(t& obj) {
    access::member_serdes(*self, obj);
    return *self;
  }

  template<typename t, enable_if<has_non_member_serdes<t, deserializer_impl>::value> = sfinae>
  deserializer_impl& process_impl(t& obj) {
    serdes(*self, obj);
    return *self;
  }

  template<typename t, enable_if<has_non_member_deserialize<t, deserializer_impl>::value> = sfinae>
  deserializer_impl& process_impl(t& obj) {
    deserialize(*self, obj);
    return *self;
  }
};

class byte_deserializer : public deserializer<byte_deserializer>
{
 public:
  byte_deserializer(std::istream &s) : deserializer<byte_deserializer>(this), stream_(s) {}
  ~byte_deserializer() noexcept = default;

  void read(void* data, std::streamsize size) {
    auto const read_size = stream_.rdbuf()->sgetn( reinterpret_cast<char*>( data ), size );
    if(read_size != size) { 
      std::cout << "Failed to read " + std::to_string(size) + " bytes from input stream! Read " << std::to_string(read_size) << std::endl;
    }
  }

 private:
  std::istream& stream_;
};

} // namespace cser

#endif /* DESERIALIZER_H */