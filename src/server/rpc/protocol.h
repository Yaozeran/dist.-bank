/* Copyright (c) 2026, Yao Zeran, Zhang Chenzhi, Zhang Senyao
 * 
 * The <protocol.h> file defines udp and rpc protocols */

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include <optional>

/* udp payload size */
constexpr int payload_size = 1200;

/* rpc operation code */
enum class op_code {
  open = 1, close = 2, deposit = 3, check_balance = 4, monitor = 5
};

inline std::optional<op_code> int_to_op_code(int i) {
  switch (i) {
    case 1: return op_code::open;
    case 2: return op_code::close;
    case 3: return op_code::deposit;
    case 4: return op_code::check_balance;
    case 5: return op_code::monitor;
    default: return std::nullopt;
  }
}

inline int op_code_to_int(op_code c) {
  switch (c){
    case op_code::open: return 1;
    case op_code::close: return 2;
    case op_code::deposit: return 3;
    case op_code::check_balance: return 4;
    case op_code::monitor: return 5;
    default: return -1;
  }
}

inline std::string op_code_to_str(op_code c) {
  switch (c){
    case op_code::open: return "open";
    case op_code::close: return "close";
    case op_code::deposit: return "deposit";
    case op_code::check_balance: return "check";
    case op_code::monitor: return "monitor";
    default: return "error";
  }
}

enum class status_code {
  success = 1, fail = 2, error = 3
};

inline std::optional<status_code> int_to_status_code(int i) {
  switch (i) {
    case 1: return status_code::success;
    case 2: return status_code::fail;
    case 3: return status_code::error;
    default: return std::nullopt;
  }
}

inline int status_code_to_int(status_code c) {
  switch (c){
    case status_code::success: return 1;
    case status_code::fail: return 2;
    case status_code::error: return 3;
    default: return -1;
  }
}

inline std::string status_code_to_str(status_code c) {
  switch (c){
    case status_code::success: return "success";
    case status_code::fail: return "fail";
    case status_code::error: return "error";
    default: return "error";
  }
}

#endif /* PROTOCOL_H */