/* Copyright (c) 2026, Yao Zeran, Zhang Chenzhi, Zhang Senyao
 *
 * The <server.h> file implements a simple udp server. */

#ifndef SERVER_H
#define SERVER_H

#include <cstddef>
#include <memory>
#include <thread>
#include <atomic>
#include <unordered_map>
#include <iostream>
#include <chrono>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../core/accounts.h"
#include "../rpc/include.h"
#include "../serdes.h"
#include "controller.h"

enum class mode {
  at_least_once = 1, at_most_once = 2, maybe = 3
};

class Server
{
 public:

  Server(int port) : controller_{}, running_(true), in_{}, out_{}, 
      account_id_ctr_(0), mode_(mode::at_most_once) {
    thread_ptr_ = std::make_unique<std::thread>(&Server::StartListening, this, port);
  }

  ~Server() {
    running_ = false;
    close(sockfd_);
    thread_ptr_->join();

    for (auto& [_, req] : requests_) {
      delete req;
    }
    for (auto& [_, resp] : responses_) {
      delete resp;
    }
    for (auto& [_, acc] : accounts_) {
      delete acc;
    }
  };

  void BindRpcViewModel(RpcViewInterface* view) { controller_.BindRpcViewModel(view); }

  void BindAccountViewModel(AccountViewInterface* view) { controller_.BindAccountViewModel(view); }

 private:

  Controller controller_;

  std::atomic<bool> running_;
  std::unique_ptr<std::thread> thread_ptr_;

  int sockfd_;
  sockaddr_in addr_;

  std::array<char, payload_size> in_;
  std::array<char, payload_size> out_;

  std::unordered_map<int, Request*> requests_;
  std::unordered_map<int, Response*> responses_;

  int account_id_ctr_;
  std::unordered_map<std::string, Account*> accounts_;

  mode mode_;

  void StartListening(int port);

  void Dispatch(Request* request, Response* response);

  void HandleCreateAccount(const char* payload, Response& response);

  void HandleDeleteAccount(const char* payload, Response& repsonse);

  void HandleCheckBalance(const char* payload, Response& response);

};

#endif /* SERVER_H */