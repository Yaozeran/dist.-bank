#pragma once

#include <QMainWindow>

#include "rpcpanel.h"
#include "datapanel.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT
 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  RpcPanel* GetRpcPanel() { return rpc_panel_; }
  AccountPanel* GetAccountPanel() { return acnt_panel_; }
 private:
  RpcPanel* rpc_panel_; 
  AccountPanel* acnt_panel_;
};