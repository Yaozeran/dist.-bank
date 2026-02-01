#include "server.h"

void Server::StartListening(int port)  {
  if ((sockfd_ = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }
  addr_.sin_family = AF_INET;
  addr_.sin_addr.s_addr = INADDR_ANY;
  addr_.sin_port = htons(port);
  if (bind(sockfd_, (sockaddr*)(&addr_), sizeof(addr_)) < 0) {
    perror("bind");
    close(sockfd_);
    exit(1);
  }
  std::cout << "server listening on port " << port << std::endl;
  while (running_) {

    sockaddr_in client_addr{};
    socklen_t client_addr_len = sizeof(client_addr);
    ssize_t n = recvfrom(sockfd_, in_.data(), sizeof(in_) - 1, 0, (sockaddr*)(&client_addr), &client_addr_len);
    if (n < 0) {
      perror("recvfrom");
      continue;
    }
    in_[n] = '\0';
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));

    Request* request = new Request();
    request->Deserialize(in_.data());
    Response* response = new Response();
    Dispatch(request, response);
    response->Serialize(out_.data());

    ssize_t sent = sendto(sockfd_, out_.data(), sizeof(out_) - 1, 0, (sockaddr*)(&client_addr), client_addr_len);
    if (sent < 0) {
      perror("sendto");
    }

    memset(in_.data(), 0, payload_size);
    memset(out_.data(), 0, payload_size);
  }
}

void Server::Dispatch(Request* request, Response* response) {
  if (requests_.contains(request->GetId())) {
    switch (mode_) {
      case mode::at_least_once: break;
      case mode::at_most_once: {
        response->SetStatusCode(status_code::error);
        response->SetPayload("duplicated operation");
      } // ignore
      default: break;
    }
  }
  switch (request->GetOpCode()) {
    case op_code::open: {
      HandleCreateAccount(request->GetPayload(), *response);
      break;
    }
    case op_code::close: {
      break;
    } 
    case op_code::deposit: {
      break;
    }
    case op_code::check_balance: {
      HandleCheckBalance(request->GetPayload(), *response);
      break;
    }
    case op_code::monitor: {
      break;
    }
    default: return;
  }
}

void Server::HandleCreateAccount(const char* payload, Response& response) {
  size_t i = 0;
  std::string user_name;
  std::string password;
  float balance;
  currency currency;
  i += deserialize(payload + i, user_name);
  i += deserialize(payload + i, password);
  i += deserialize(payload + i, balance);
  i += deserialize(payload + i, currency);
  auto iter = accounts_.find(user_name);
  if (iter != accounts_.end()) { 
    Account* account = new Account();
    account->SetId(account_id_ctr_++);
    account->SetUserName(user_name);
    account->SetPassword(password);
    account->SetBalance(balance);
    account->SetCurrency(currency);
    accounts_[user_name] = account;
    response.SetStatusCode(status_code::success);
    response.SetPayload("account created: " + account->ToString());
    controller_.CreateAccount(*account);
  } else {
    response.SetStatusCode(status_code::error);
    response.SetPayload("error: user_name already exists");
  }
}

void Server::HandleDeleteAccount(const char* payload, Response& repsonse) {

}

void Server::HandleCheckBalance(const char* payload, Response& response) {
  size_t i = 0;
  std::string user_name;
  std::string password;
  i += deserialize(payload + i, user_name);
  i += deserialize(payload + i, password);
  auto iter = accounts_.find(user_name);
  if (iter == accounts_.end()) {
    response.SetStatusCode(status_code::error);
    response.SetPayload("account not found with user name: " + user_name);
  } else if (password != iter->second->GetPassword()) {
    response.SetStatusCode(status_code::fail);
    response.SetPayload("password not correct");
  } else {
    float bal = iter->second->GetBalance();
    response.SetStatusCode(status_code::success);
    response.SetPayload("your current account balance is: " + std::to_string(bal));
  }
}