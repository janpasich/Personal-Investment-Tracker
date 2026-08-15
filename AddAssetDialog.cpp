#include "AddAssetDialog.h"
#include <stdexcept> // Library for exceptions

AddAssetDialog::AddAssetDialog(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    connect(ui.btnAdd, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui.btnCancel, &QPushButton::clicked, this, &QDialog::reject);

    connect(ui.comboType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        if (index == 3) { // Assuming index 3 is Cash/Gotowka
            ui.editName->setEnabled(false);
            ui.editPrice->setEnabled(false);
            ui.editName->setText("PLN");
            ui.editPrice->setText("1.0");
        }
        else {
            ui.editName->setEnabled(true);
            ui.editPrice->setEnabled(true);
            ui.editName->clear();
            ui.editPrice->clear();
        }
        });
}

AddAssetDialog::~AddAssetDialog()
{
}

QString AddAssetDialog::getType() {
    return ui.comboType->currentText();
}

QString AddAssetDialog::getName() {
    QString name = ui.editName->text().trimmed(); // Removes accidental spaces at the beginning and end

    if (name.isEmpty()) {
        throw std::invalid_argument("Asset name cannot be empty!");
    }

    bool isNumber;
    name.toDouble(&isNumber);
    // If the text was successfully converted to a number, it means there are no letters!
    if (isNumber) {
        throw std::invalid_argument("Asset name cannot be just a number!");
    }

    return name;
}

double AddAssetDialog::getQuantity() {
    QString txt = ui.editQuantity->text();
    txt.replace(",", "."); // Change comma to dot, so the user can enter e.g. "1,5"

    bool ok;
    double val = txt.toDouble(&ok);

    // If conversion failed (letters were entered), throw an exception!
    if (!ok) {
        throw std::invalid_argument("You must enter a number in the 'Quantity' field!");
    }
    return val;
}

double AddAssetDialog::getPrice() {
    QString txt = ui.editPrice->text();
    txt.replace(",", ".");

    bool ok;
    double val = txt.toDouble(&ok);

    if (!ok) {
        throw std::invalid_argument("You must enter a number in the 'Purchase price' field!");
    }
    return val;
}