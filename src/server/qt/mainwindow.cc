#include "mainwindow.h"

#include <QGuiApplication>
#include <QScreen>
#include <QLabel>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {

  setWindowTitle("Bank Server");
  QScreen* screen = QGuiApplication::primaryScreen();
  QRect available = screen->availableGeometry();
  resize(available.width() * 0.7, available.height() * 0.6);
  move(available.topLeft());
  
  QWidget* main_widget = new QWidget();
  setCentralWidget(main_widget);

  QHBoxLayout* main_layout = new QHBoxLayout(main_widget);

  QVBoxLayout* net_layout = new QVBoxLayout();
  rpc_panel_ = new RpcPanel(main_widget);
  net_layout->addWidget(rpc_panel_);

  QVBoxLayout* data_layout = new QVBoxLayout();
  acnt_panel_ = new AccountPanel(main_widget);
  data_layout->addWidget(acnt_panel_);

  main_layout->addLayout(net_layout);
  main_layout->addLayout(data_layout);
}

MainWindow::~MainWindow() {
  delete rpc_panel_;
  delete acnt_panel_;
}