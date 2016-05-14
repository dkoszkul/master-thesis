#ifndef SIMSETTINGSDIALOG_H
#define SIMSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SimSettingsDialog;
}

class SimSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SimSettingsDialog(QWidget *parent = 0);
    ~SimSettingsDialog();

private:
    Ui::SimSettingsDialog *ui;
};

#endif // SIMSETTINGSDIALOG_H
