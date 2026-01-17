/* Copyright (c) 2026, Yaozeran, Zhangchenzhi, Zhangsenyao
 * 
 * This file implements serializer classes.
 */

#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <sstream>
#include <iostream>

#include "traits.h"
#include "access.h"

namespace serdes
{

/*
 * Base class: serialize an object into a stream. 
 */
template<class serializer_impl>
class serializer
{
 public:
  serializer(serializer_impl* const derived) : self(derived) {}
  
  template<typename... types> 
  inline serializer& operator() (types&& ... objs) {
    self->process(std::forward<types>(objs)...);
    return *self;
  }

 private:
  serializer_impl* self;

  template<typename f, typename... types>
  inline void process(f&& first, types&&... others) {
    self->process(std::forward<f>(first));
    self->process(std::forward<types>(others)...);
  }

  template<typename t>
  inline void process(t&& obj) {
    self->process_impl(obj);
  }

  template<typename t, enable_if<has_member_serdes<t, serializer_impl>::value> = sfinae>
  serializer_impl& process_impl(const t& obj) {
    access::member_serdes(*self, const_cast<t&>(obj));
    return *self;
  }

  template<typename t, enable_if<has_non_member_serdes<t, serializer_impl>::value> = sfinae>
  serializer_impl& process_impl(const t& obj) {
    serdes(*self, const_cast<t&>(obj));
    return *self;
  }

  template<typename t, enable_if<has_non_member_serialize<t, serializer_impl>::value> = sfinae>
  serializer_impl& process_impl(const t& obj) {
    serialize(*self, const_cast<t&>(obj));
    return *self;
  }
};

/*
 * A serializer to produce a byte stream from an object
 */
class byte_serializer : public serializer<byte_serializer>
{
 public: 
  byte_serializer(std::ostream &s) : serializer<byte_serializer>(this), stream_(s) {}
  ~byte_serializer() noexcept = default;

  void write(const void* data, std::streamsize size) {
    auto const written_size = stream_.rdbuf()->sputn(reinterpret_cast<const char*>(data), size);
    if(written_size != size) {/* todo */};
  }

 private:
  std::ostream& stream_;
};

} // namespace cser

#endif /* SERIALIZER_H */