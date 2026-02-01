/* Copyright (c) 2026, Yaozeran, Zhangchenzhi, Zhangsenyao
 *
 * The <accounts.h> file implements bank accounts related business logic. */

#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include <string>

#include "../serdes.h"
#include "currency.h"

class Account 
{
 public:

  Account() = default;
  Account(int id, std::string name, std::string pass, currency cur, float bal) 
      : id_(id), user_name_(name), password_(pass), currency_(cur), balance_(bal) {}
  
  ~Account() = default;

  inline size_t Serialize(char* out) {
    size_t i = 0;
    i += serialize(id_, out + i);
    i += serialize(user_name_, out + i);
    i += serialize(password_, out + i);
    i += serialize(balance_, out + i);
    i += serialize(currency_, out + i);
    return i;
  }

  inline size_t Deserialize(const char* in) {
    size_t i = 0;
    i += deserialize(in + i, id_);
    i += deserialize(in + i, user_name_);
    i += deserialize(in + i, password_);
    i += deserialize(in + i, balance_);
    i += deserialize(in + i, currency_);
    return i;
  }

  std::string ToString() const {
    return "Account { id: " + std::to_string(id_) + ", holder_name: " + 
      user_name_ + ", password: " + password_ + ", balance: " + 
      std::to_string(balance_) + ", currency: " + currency_to_str(currency_) + " }";
  }

  inline int GetId() const { return id_; }

  inline void SetId(int id) { id_ = id; }

  inline std::string GetUserName() const { return user_name_; }

  inline void SetUserName(const std::string& str) { user_name_ = str; }

  inline std::string GetPassword() const { return password_; }

  inline void SetPassword(const std::string& str) { password_ = str; }

  inline float GetBalance() const { return balance_; }

  inline void SetBalance(float bal) { balance_ = bal; }

  inline currency GetCurrency() const { return currency_; }

  inline void SetCurrency(currency cur) { currency_ = cur; }
  
 private: 
 
  int id_;
  std::string user_name_;
  std::string password_;
  float balance_;
  currency currency_;

};

#endif /* ACCOUNTS_H */