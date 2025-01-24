#ifndef DIALOGCALIBRATION_H
#define DIALOGCALIBRATION_H

#include <QDialog>
#include <QPaintEvent>

//class DialogIntrinsic;
//class DialogStart;
//class DialogExtrinsic;

namespace Ui {
class DialogCalibration;
}

class DialogCalibration : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCalibration(QWidget *parent = nullptr);
    ~DialogCalibration();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void on_INTRINSIC_clicked();
    void on_EXTRINSIC_clicked();
    void on_cancelButton_clicked();

private:
    Ui::DialogCalibration *ui;
//    DialogIntrinsic *dialogIntrinsic;
//    DialogStart *dialogStart;
//    DialogExtrinsic* dialogExtrinsic;

};

#endif // DIALOGCALIBRATION_H
