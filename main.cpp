#include <iostream>
#include <QApplication>
#include <QLocale> // <-- Added library to handle the decimal dot format
#include "MainWindow.h"

// Model includes (keeping them as they will be needed for GUI integration)
#include "Portfolio.h"
#include "Cash.h"
#include "Stock.h"
#include "PreciousMetal.h"
#include "Crypto.h" 
#include "SaveManager.h"

int main(int argc, char* argv[])
{
    // 1. Initialize the Qt GUI application (MUST be at the very beginning)
    QApplication a(argc, argv);

    // LOCALE OVERRIDE
    // Force the "C" locale across the entire application.
    // This ensures all windows, input fields, and charts use a dot (.) instead of a comma (,) for decimals.
    QLocale::setDefault(QLocale::C);

    // 2. Create and show the main window
    MainWindow w;
    w.show();

    // 3. Execute the main application loop (waits for user interactions in the window)
    return a.exec();
}