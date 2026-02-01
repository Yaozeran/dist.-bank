/* Copyright 2026 (c), Yao Zeran, Zhang Chenzhi, Zhang Senyao */

#include "rpcpanel.h"

#include <QVBoxLayout>
#include <QHeaderView>

#include "../rpc/protocol.h"

RpcPanel::RpcPanel(QWidget* parent) : QWidget(parent) {
  QVBoxLayout* rpc_layout = new QVBoxLayout(this);
  table_view_ = new QTableView(this);
  rpc_log_ = new QStandardItemModel(0, 5, this);
  rpc_log_->setHorizontalHeaderLabels({"Timestamp", "Type", "Ip", "Id", "Code"});
  table_view_->setModel(rpc_log_);
  table_view_->setEditTriggers(QAbstractItemView::NoEditTriggers);
  QHeaderView* header = table_view_->horizontalHeader();
  header->setSectionResizeMode(QHeaderView::ResizeToContents);
  header->setStretchLastSection(true);
  rpc_layout->addWidget(table_view_);
  setLayout(rpc_layout);

  AddRpcEntry("[ 2026.1.30; 10:00:00 ]", "RECV", "0", "168.101.1.11", "0");
}

void RpcPanel::AddRpcRequest(std::string t, std::string ip, const Request& req) {
  QString t_ = QString::fromStdString(t);
  QString ip_ = QString::fromStdString(ip);
  QString id_ = QString::number(req.GetId());
  QString op_code_ = QString::fromStdString(op_code_to_str(req.GetOpCode()));
  AddRpcEntry(t_, "RECV", ip_, id_, op_code_);
}

void RpcPanel::AddRpcEntry(const QString& timestamp, const QString& rpc_type, 
  const QString& ip, const QString& id, const QString& op_code) {
  auto MakeItem = [](const QString& text) {
    QStandardItem* item = new QStandardItem(text);
    item->setTextAlignment(Qt::AlignCenter);
    return item;
  };
  QList<QStandardItem*> row;
  row << MakeItem(timestamp);
  QStandardItem* rpc_item = MakeItem(rpc_type);
  rpc_item->setForeground(rpc_type == "RECV" ? QBrush(Qt::blue) : QBrush(Qt::magenta));
  row << (rpc_item);
  row << MakeItem(ip);
  row << MakeItem(id);
  row << MakeItem(op_code);
  rpc_log_->appendRow(row);
  table_view_->scrollToBottom();
}