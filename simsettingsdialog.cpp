#include "simsettingsdialog.h"
#include "ui_simsettingsdialog.h"

SimSettingsDialog::SimSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimSettingsDialog)
{
    ui->setupUi(this);
    intValidator = new QIntValidator(0, 40000, this);

    connect(ui->applyButton, SIGNAL(clicked()), this, SLOT(apply()));

    QDoubleValidator *v = new QDoubleValidator(0, 2500, 1, this);
    QLocale locale = QLocale::c();
    v->setLocale(locale);

    ui->timeFrom->setValidator(v);
    ui->timeTo->setValidator(v);
    ui->timeStep->setValidator(v);
    ui->epsilon->setValidator(v);

    ui->timeFrom->insert(QString::number(0));
    ui->timeTo->insert(QString::number(1500));
    ui->timeStep->insert(QString::number(1));
    ui->epsilon->insert(QString::number(0.3));

    currentSimSettings.timeFrom = 0.0;
    currentSimSettings.timeTo = 1500.0;
    currentSimSettings.timeStep = 1.0;
    currentSimSettings.epsilon = 0.3;
}

SimSettingsDialog::~SimSettingsDialog()
{
    delete ui;
}

SimSettingsDialog::SimSettings SimSettingsDialog::simSettings() const
{
    return currentSimSettings;
}

void SimSettingsDialog::apply()
{
    currentSimSettings.timeFrom = ui->timeFrom->text().toDouble();
    currentSimSettings.timeTo = ui->timeTo->text().toDouble();
    currentSimSettings.timeStep = ui->timeStep->text().toDouble();
    currentSimSettings.epsilon = ui->epsilon->text().toDouble();

    hide();
}
