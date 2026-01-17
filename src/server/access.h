/* Copyright (c) 2026, Yaozeran, Zhangchenzhi, Zhangsenyao
 * 
 * This file implements access helper token in serialization module.
 */

#ifndef ACCESS_H
#define ACCESS_H

#include <type_traits>

namespace serdes
{

/* A token class to be included as a private member of a class, 
 *   to allow users to define a private serdes function
 */
class access
{
 public:

  /* call class t's implemented serdes function */
  template<class cserdes, class t>
  inline static auto member_serdes(cserdes &serdes, t& cls) -> decltype(cls.serdes(serdes)) {
    return cls.serdes(serdes);
  }
  
};

} // namespace cser

#endif