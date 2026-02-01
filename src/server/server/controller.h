
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <memory>

#include "../core/accounts.h"
#include "../rpc/request.h"
#include "../rpc/response.h"

class RpcViewInterface
{
 public:
  virtual void AddRpcRequest(std::string t, std::string ip, const Request& req) = 0;
  virtual void AddRpcResponse(const Response& resp) = 0;
};

class AccountViewInterface
{
 public:
  virtual void CreateAccount(const Account& account) = 0;
};

class Controller
{
 public:

  Controller() = default;

  ~Controller() {};

  void BindRpcViewModel(RpcViewInterface* rpc_view);

  void BindAccountViewModel(AccountViewInterface* account_view);

  void Dispatch() {};

  void ReceiveRpcRequest(const std::string& ip, const Request& req);

  void PostRpcResponse(const Response& resp);

  void CreateAccount(const Account& account);

  void DeleteAccount(const Account& account);

 private:

  RpcViewInterface* rpc_view_;
  
  AccountViewInterface* account_view_;

};

#endif /* CONTROLLER_H */