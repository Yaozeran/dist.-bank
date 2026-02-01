/* Copyright (c) 2026, Yaozeran, Zhangchenzhi, Zhangsenyao
 * 
 * This file implements currency related business logic.
 */

#ifndef CURRENCY_H
#define CURRENCY_H

#include <string>
#include <optional>

enum class currency {
  usd, rmb, jpy, bpd 
};

inline std::optional<currency> str_to_currency(std::string str) {
  if (str == "USD") return currency::usd;
  if (str == "RMB") return currency::rmb; 
  if (str == "JPY") return currency::jpy;
  if (str == "BPD") return currency::bpd;
  return std::nullopt;
}

inline std::string currency_to_str(currency c) {
  switch (c) {
    case currency::usd: return "USD";
    case currency::rmb: return "RMB";
    case currency::jpy: return "JPY";
    case currency::bpd: return "BPD";
    default: return "ERR";
  }
}

#endif /* CURRENCY_H */