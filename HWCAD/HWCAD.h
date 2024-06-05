#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_HWCAD.h"

class HWCAD : public QMainWindow
{
    Q_OBJECT

public:
    HWCAD(QWidget *parent = nullptr);
    ~HWCAD();

private:
    Ui::HWCADClass ui;
};
