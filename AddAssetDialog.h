#pragma once

#include <QDialog>
#include "ui_AddAssetDialog.h"

class AddAssetDialog : public QDialog
{
    Q_OBJECT

public:
    AddAssetDialog(QWidget* parent = nullptr);
    ~AddAssetDialog();
    QString getType();
    QString getName();
    double getQuantity();
    double getPrice();

private:
    Ui::AddAssetDialogClass ui;
};