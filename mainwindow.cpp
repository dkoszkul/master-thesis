#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "paintwidget.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->simulation = new Simulation();

    connect(ui->pushButton, &QPushButton::clicked, simulation, &Simulation::simulate);
}

MainWindow::~MainWindow()
{
    delete ui;
}
