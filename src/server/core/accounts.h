/* Copyright (c) 2026, Yaozeran, Zhangchenzhi, Zhangsenyao
 *
 * This file implements bank accounts related business logic.
 */

#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include <string>

#include "currency.h"

class Account 
{
 public:

  Account() = default;
  Account(unsigned long id, std::string name, std::string pass, currency cur, float bal) 
    : id_(id), holder_name_(name), password_(pass), currency_(cur), balance_(bal) {}
  ~Account() = default;

  template<class cserdes>
  inline void serdes(cserdes& s) {
    s(id_, holder_name_, password_, currency_, balance_);
  }
  
 private:
 
  unsigned long id_;
  std::string holder_name_;
  std::string password_;
  currency currency_;
  float balance_;
};

#endif /* ACCOUNTS_H */