#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PersonalInvestmentTracker.h"

class PersonalInvestmentTracker : public QMainWindow
{
    Q_OBJECT

public:
    PersonalInvestmentTracker(QWidget *parent = nullptr);
    ~PersonalInvestmentTracker();

private:
    Ui::PersonalInvestmentTrackerClass ui;
};

