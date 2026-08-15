#pragma once
#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QPainter>
#include "Portfolio.h"
#include "Cash.h"
#include "SaveManager.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    // Function that reads the vector from Portfolio and populates the UI table
    void refreshTable();

private slots:
    void onBtnAddClicked();
    void onBtnDeleteClicked();
    void onBtnSaveClicked();
    void onApiReply(QNetworkReply* reply);
    void onTableDoubleClicked(int row, int column);

private:
    Ui::MainWindow* ui;
    Portfolio myPortfolio;
    Cash* myCash;
    QLabel* chartLabel;
    void drawChartQPainter();
    QNetworkAccessManager* networkManager;

    // --- API ---
    void fetchApiRates();

    double rateUSD = 1.0;
    double rateEUR = 1.0;
};