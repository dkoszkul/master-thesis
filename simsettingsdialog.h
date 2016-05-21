#ifndef SIMSETTINGSDIALOG_H
#define SIMSETTINGSDIALOG_H

#include <QDialog>
#include <iostream>
namespace Ui {
class SimSettingsDialog;
}

class QIntValidator;

class SimSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    struct SimSettings {
        double timeFrom;
        double timeTo;
        double timeStep;

        double epsilon;
    };

    explicit SimSettingsDialog(QWidget *parent = 0);
    ~SimSettingsDialog();

    SimSettings simSettings() const;

private:
    Ui::SimSettingsDialog *ui;
    SimSettings currentSimSettings;
    QIntValidator *intValidator;

private slots:
    void apply();
};

#endif // SIMSETTINGSDIALOG_H
