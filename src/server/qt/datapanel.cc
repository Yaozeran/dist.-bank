/* Copyright 2026 (c), Yao Zeran, Zhang Chenzhi, Zhang Senyao */

#include "datapanel.h"

#include <QVBoxLayout>

AccountPanel::AccountPanel(QWidget* parent) : QWidget(parent) {
  QVBoxLayout* acnts_layout = new QVBoxLayout(this);
  table_ = new QTableView(this);
  accounts_ = new QStandardItemModel(0, 5, this);
  table_->setModel(accounts_);
  acnts_layout->addWidget(table_);

  CreateAccount("0", "zeran", "12345678", "10.00", "RMB");
}

void AccountPanel::CreateAccount(const Account& account) {
  QString id = QString::number(account.GetId());
  QString user_name = QString::fromStdString(account.GetUserName());
  QString password = QString::fromStdString(account.GetPassword());
  QString balance = QString::number(account.GetBalance());
  QString currency = QString::fromStdString(currency_to_str(account.GetCurrency())); 
  CreateAccount(id, user_name, balance, password, currency);
}

void AccountPanel::CreateAccount(const QString& id, const QString& user_name, 
    const QString& password, const QString& balance, const QString& currency) {
  QList<QStandardItem*> row;
  row << new QStandardItem(id);
  row << new QStandardItem(user_name);
  row << new QStandardItem(password);
  row << new QStandardItem(balance);
  row << new QStandardItem(currency);
  accounts_->appendRow(row);
  table_->scrollToBottom();
}

CallbackPanel::CallbackPanel(QWidget* parent) : QWidget(parent) {
  
}