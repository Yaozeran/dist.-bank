/* Copyright 2026 (c), Yao Zeran, Zhang Chenzhi, Zhang Senyao
 *
 * The <rpcpanel.h> file defines qt rpc panel widget which shows rpc calls */

#ifndef GUI_RPCPANEL_H
#define GUI_RPCPANEL_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>

#include "../rpc/request.h"
#include "../server/controller.h"


class RpcPanel: public QWidget, public RpcViewInterface
{
  Q_OBJECT
 public: 
  RpcPanel(QWidget* parent);
  void AddRpcRequest(std::string t, std::string ip, const Request& req) override;
  void AddRpcResponse(const Response& resp) override {};
  void AddRpcEntry(const QString& timestamp, const QString& rpc_type, 
    const QString& ip, const QString& id, const QString& op_code);
 private:
  QTableView* table_view_;
  QStandardItemModel* rpc_log_; 
};

#endif /* GUI_RPCPANEL_H */