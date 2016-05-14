#include "simsettingsdialog.h"
#include "ui_simsettingsdialog.h"

SimSettingsDialog::SimSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimSettingsDialog)
{
    ui->setupUi(this);
}

SimSettingsDialog::~SimSettingsDialog()
{
    delete ui;
}
