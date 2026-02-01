#include "controller.h"

#include <iostream>
#include <utility>
#include <chrono>

#include "../core/accounts.h"

void Controller::BindRpcViewModel(RpcViewInterface* view) {
  rpc_view_ = view;
}

void Controller::BindAccountViewModel(AccountViewInterface* view) {
  account_view_ = view;
}

static constexpr std::string GetCurrentTimeStamp() {
  auto now = std::chrono::system_clock::now();
  std::time_t now_t = std::chrono::system_clock::to_time_t(now);
  std::string time_str = std::ctime(&now_t);
  if (!time_str.empty() && time_str.back() == '\n') {
    time_str.pop_back();
  }
  return time_str;
}

void Controller::ReceiveRpcRequest(const std::string& ip, const Request& req) {
  std::string time_stamp = GetCurrentTimeStamp();
  rpc_view_->AddRpcRequest(time_stamp, ip, req);
}

void Controller::PostRpcResponse(const Response& resp) {
  rpc_view_->AddRpcResponse(resp);
}

void Controller::CreateAccount(const Account& account) {
  account_view_->CreateAccount(account);
  std::cout << account.ToString() << " created. " << std::endl;
}

void Controller::DeleteAccount(const Account& account) {
  std::cout << account.ToString() << " deleted. " << std::endl; 
}