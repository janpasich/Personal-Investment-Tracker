#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QPushButton>
#include <QTableWidgetItem>
#include <QString>
#include <QMessageBox>
#include <QCoreApplication>
#include <QInputDialog>
#include <stdexcept>
#include "Stock.h" 
#include "AddAssetDialog.h"
#include "PreciousMetal.h"
#include "Crypto.h"
#include "Cash.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onApiReply);
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onApiReply);

    myCash = new Cash(0.0);

    if (!ui->tab_2->layout()) {
        ui->tab_2->setLayout(new QVBoxLayout());
    }
    chartLabel = new QLabel(this);
    chartLabel->setAlignment(Qt::AlignCenter);
    ui->tab_2->layout()->addWidget(chartLabel);

    QString filePath = QCoreApplication::applicationDirPath() + "/my_portfolio.dat";
    SaveManager::loadPortfolioBinary(myPortfolio, myCash, filePath.toStdString());

    refreshTable();

    connect(ui->btnAdd, &QPushButton::clicked, this, &MainWindow::onBtnAddClicked);
    connect(ui->btnDelete, &QPushButton::clicked, this, &MainWindow::onBtnDeleteClicked);
    connect(ui->btnSave, &QPushButton::clicked, this, &MainWindow::onBtnSaveClicked);
    connect(ui->tableWidget, &QTableWidget::cellDoubleClicked, this, &MainWindow::onTableDoubleClicked);

    fetchApiRates();
}

MainWindow::~MainWindow()
{
    delete myCash;
    delete ui;
}

void MainWindow::onBtnAddClicked()
{
    AddAssetDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        try {
            QString type = dialog.getType();
            QString name = dialog.getName();
            double quantity = dialog.getQuantity();
            double price = dialog.getPrice();

            if (type == "Cash" || type.contains("Cash")) {
                if (quantity < 0) {
                    myCash->subtractFromPosition(-quantity);
                }
                else {
                    myCash->addToPosition(quantity, 1.0);
                }
                refreshTable();
                return;
            }

            std::unique_ptr<Asset> newAsset = nullptr;

            if (type == "Stock") {
                newAsset = std::make_unique<Stock>(name.toStdString(), quantity, price, price, 0.0);
            }
            else if (type == "PreciousMetal") {
                QString lowerName = name.trimmed().toLower();

                if (lowerName == "gold" || lowerName == "gc=f") {
                    name = "XAU";
                }
                else if (lowerName == "silver" || lowerName == "si=f") {
                    name = "XAG";
                }

                newAsset = std::make_unique<PreciousMetal>(name.toStdString(), quantity, price, price);
            }
            else if (type == "Crypto") {
                newAsset = std::make_unique<Crypto>(name.toStdString(), quantity, price, price);
            }

            if (newAsset != nullptr) {
                myPortfolio.buyAsset(std::move(newAsset));
                refreshTable();
            }
        }
        catch (const std::invalid_argument& e) {
            QMessageBox::critical(this, "Data Input Error", e.what());
        }
    }
}

void MainWindow::refreshTable()
{
    ui->tableWidget->setRowCount(0);
    // const auto& prevents attempting to copy the unique_ptr!
    const auto& assets = myPortfolio.getAssets();

    double totalValue = myCash->calculateValue() + myPortfolio.calculateTotalValue();
    double totalDividend = myPortfolio.calculateTotalDividend();
    double totalProfit = 0.0;

    for (const auto& asset : assets) {
        // Extract raw pointer using .get() for dynamic_cast
        if (Stock* stock = dynamic_cast<Stock*>(asset.get())) {
            totalProfit += stock->calculateProfitLoss();
        }
    }

    ui->lblTotalValue->setText(QString("Portfolio Value: %1 PLN").arg(totalValue, 0, 'f', 2));
    ui->lblDividend->setText(QString("Estimated Dividend: %1 PLN/year").arg(totalDividend, 0, 'f', 2));
    ui->lblProfit->setText(QString("Profit/Loss: %1 PLN").arg(totalProfit, 0, 'f', 2));

    ui->tableWidget->insertRow(0);
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("PLN"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("Cash"));
    ui->tableWidget->setItem(0, 2, new QTableWidgetItem(QString::number(myCash->getQuantity(), 'f', 2)));
    ui->tableWidget->setItem(0, 3, new QTableWidgetItem("1.00 PLN"));
    ui->tableWidget->setItem(0, 4, new QTableWidgetItem(QString::number(myCash->calculateValue(), 'f', 2) + " PLN"));
    ui->tableWidget->setItem(0, 5, new QTableWidgetItem("-"));

    for (size_t i = 0; i < assets.size(); ++i) {
        int row = i + 1;
        ui->tableWidget->insertRow(row);

        QString name = QString::fromStdString(assets[i]->getName());
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(name));

        QString category = "Other";
        QString purchasePriceStr = "-";
        QString profitStr = "-";

        if (Stock* stock = dynamic_cast<Stock*>(assets[i].get())) {
            category = "Stocks";
            purchasePriceStr = QString::number(stock->getAveragePrice(), 'f', 2) + " PLN";
            profitStr = QString::number(stock->calculateProfitLoss(), 'f', 2) + " PLN";
        }
        else if (PreciousMetal* metal = dynamic_cast<PreciousMetal*>(assets[i].get())) {
            category = "Precious Metal";
            purchasePriceStr = QString::number(metal->getAveragePrice(), 'f', 2) + " PLN";
            profitStr = QString::number(metal->calculateProfitLoss(), 'f', 2) + " PLN";
        }
        else if (Crypto* crypto = dynamic_cast<Crypto*>(assets[i].get())) {
            category = "Crypto";
            purchasePriceStr = QString::number(crypto->getAveragePrice(), 'f', 2) + " PLN";
            profitStr = QString::number(crypto->calculateProfitLoss(), 'f', 2) + " PLN";
        }

        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(category));

        QString quantity = QString::number(assets[i]->getQuantity());
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(quantity));

        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(purchasePriceStr));

        QString value = QString::number(assets[i]->calculateValue(), 'f', 2) + " PLN";
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(value));

        double dividend = assets[i]->calculateEstimatedDividend();
        QString divProfit = QString("Div: %1 | P/L: %2").arg(QString::number(dividend, 'f', 2) + " PLN").arg(profitStr);
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(divProfit));
    }
    drawChartQPainter();
}

void MainWindow::onBtnDeleteClicked()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) return;

    if (currentRow == 0) {
        double availableCash = myCash->getQuantity();

        if (availableCash <= 0) {
            QMessageBox::information(this, "Insufficient funds", "You don't have cash to withdraw.");
            return;
        }

        bool confirmed;
        double amountToRemove = QInputDialog::getDouble(
            this,
            "Cash Withdrawal",
            QString("How much PLN do you want to withdraw? (Available: %1 PLN)").arg(availableCash),
            availableCash,
            0.01,
            availableCash,
            2,
            &confirmed
        );

        if (confirmed && amountToRemove > 0) {
            myCash->subtractFromPosition(amountToRemove);
            refreshTable();
        }
        return;
    }

    const auto& assets = myPortfolio.getAssets();
    int vectorIndex = currentRow - 1;

    if (vectorIndex < assets.size()) {
        double availableQuantity = assets[vectorIndex]->getQuantity();
        QString assetName = QString::fromStdString(assets[vectorIndex]->getName());

        bool confirmed;
        double quantityToRemove = QInputDialog::getDouble(
            this,
            "Partial Sale",
            QString("How many units of '%1' do you want to sell? (Available: %2)").arg(assetName).arg(availableQuantity),
            availableQuantity,
            0.0001,
            availableQuantity,
            4,
            &confirmed
        );

        if (confirmed && quantityToRemove > 0) {
            myPortfolio.sellAsset(vectorIndex, quantityToRemove, 0.0);
            refreshTable();
        }
    }
}

void MainWindow::onBtnSaveClicked()
{
    QString filePath = QCoreApplication::applicationDirPath() + "/my_portfolio.dat";
    bool savedSuccessfully = SaveManager::savePortfolioBinary(myPortfolio, myCash, filePath.toStdString());

    if (savedSuccessfully) {
        QMessageBox::information(this, "Success", "Portfolio state has been saved successfully!");
    }
    else {
        QMessageBox::warning(this, "Error", "Failed to save portfolio to file.");
    }
}

void MainWindow::drawChartQPainter()
{
    int width = 700;
    int height = 400;
    QPixmap pixmap(width, height);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    double cash = myCash->calculateValue();
    double stocks = 0.0, metals = 0.0, crypto = 0.0;

    for (const auto& asset : myPortfolio.getAssets()) {
        if (dynamic_cast<Stock*>(asset.get())) stocks += asset->calculateValue();
        else if (dynamic_cast<PreciousMetal*>(asset.get())) metals += asset->calculateValue();
        else if (dynamic_cast<Crypto*>(asset.get())) crypto += asset->calculateValue();
    }

    double total = cash + stocks + metals + crypto;
    QColor textColor = QApplication::palette().color(QPalette::WindowText);
    painter.setPen(textColor);

    if (total <= 0) {
        painter.drawText(QRect(0, 0, width, height), Qt::AlignCenter, "Portfolio is empty. Add assets to see the chart.");
        chartLabel->setPixmap(pixmap);
        return;
    }

    QRectF rect(50, 50, 300, 300);
    int startAngle = 0;
    QColor colorCash(119, 221, 119);
    QColor colorStocks(119, 158, 203);
    QColor colorMetals(253, 253, 150);
    QColor colorCrypto(203, 153, 201);
    painter.setPen(Qt::NoPen);
    int legendY = 100;

    auto drawSlice = [&](double value, QColor color, QString name, int& legendPos) {
        if (value > 0) {
            int spanAngle = (value / total) * 360 * 16;
            painter.setBrush(color);
            painter.drawPie(rect, startAngle, spanAngle);
            startAngle += spanAngle;

            painter.setBrush(color);
            painter.drawRect(420, legendPos, 20, 20);

            painter.setPen(textColor);
            double percent = (value / total) * 100.0;
            QString label = QString("%1 - %2 PLN (%3%)").arg(name).arg(value, 0, 'f', 2).arg(percent, 0, 'f', 1);
            painter.drawText(450, legendPos + 15, label);

            painter.setPen(Qt::NoPen);
            legendPos += 40;
        }
        };

    drawSlice(cash, colorCash, "Cash", legendY);
    drawSlice(stocks, colorStocks, "Stocks", legendY);
    drawSlice(metals, colorMetals, "Precious Metals", legendY);
    drawSlice(crypto, colorCrypto, "Crypto", legendY);

    chartLabel->setPixmap(pixmap);
}

void MainWindow::onApiReply(QNetworkReply* reply)
{

    if (reply->error() == QNetworkReply::NoError) {
        int index = reply->property("assetIndex").toInt();
        QString apiEngine = reply->property("apiName").toString();

        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);


        if (jsonDoc.isObject()) {
            QJsonObject jsonObj = jsonDoc.object();

    
            if (apiEngine == "NBP_USD") {
                if (jsonObj.contains("rates") && jsonObj["rates"].isArray()) {
                    QJsonArray rates = jsonObj["rates"].toArray();
                    if (!rates.isEmpty()) {
                        rateUSD = rates[0].toObject()["mid"].toDouble();
                    }
                }
                reply->deleteLater();
                return;
            }
            else if (apiEngine == "NBP_EUR") {
                if (jsonObj.contains("rates") && jsonObj["rates"].isArray()) {
                    QJsonArray rates = jsonObj["rates"].toArray();
                    if (!rates.isEmpty()) {
                        rateEUR = rates[0].toObject()["mid"].toDouble();
                    }
                }
                reply->deleteLater();
                return;
            }

            double fetchedPrice = -1.0;


            if (apiEngine == "Yahoo") {
  
                if (jsonObj.contains("chart") && jsonObj["chart"].isObject()) {
                    QJsonObject chartObj = jsonObj["chart"].toObject();

                    if (chartObj.contains("result") && chartObj["result"].isArray()) {
                        QJsonArray resultArr = chartObj["result"].toArray();

                        if (!resultArr.isEmpty()) {
                            QJsonObject meta = resultArr[0].toObject()["meta"].toObject();

                            if (meta.contains("regularMarketPrice")) {
                                fetchedPrice = meta["regularMarketPrice"].toDouble();

                               
                                if (meta.contains("currency")) {
                                    QString currency = meta["currency"].toString();

                                    if (currency == "USD") {
                                        fetchedPrice = fetchedPrice * rateUSD;
                                    }
                                    else if (currency == "EUR") {
                                        fetchedPrice = fetchedPrice * rateEUR;
                                    }
                                    
                                }
                            }
                        }
                    }
                }
            }

            else if (apiEngine == "CoinGecko") {
                QStringList keys = jsonObj.keys();
                if (!keys.isEmpty()) {
                    QString cryptoKey = keys.first();
                    QJsonObject cryptoObj = jsonObj[cryptoKey].toObject();

                    if (cryptoObj.contains("pln")) {
                        fetchedPrice = cryptoObj["pln"].toDouble();
                    }
                }
            }

            if (fetchedPrice > 0.0) {
                const auto& assets = myPortfolio.getAssets();
                if (index >= 0 && index < assets.size()) {
                    assets[index]->setCurrentPrice(fetchedPrice);
                }
            }
        }
    }

    refreshTable();
    reply->deleteLater();
}

void MainWindow::fetchApiRates()
{
    const auto& assets = myPortfolio.getAssets();

    QNetworkRequest reqUSD(QUrl("http://api.nbp.pl/api/exchangerates/rates/a/usd/?format=json"));
    QNetworkReply* repUSD = networkManager->get(reqUSD);
    repUSD->setProperty("apiName", "NBP_USD");

    QNetworkRequest reqEUR(QUrl("http://api.nbp.pl/api/exchangerates/rates/a/eur/?format=json"));
    QNetworkReply* repEUR = networkManager->get(reqEUR);
    repEUR->setProperty("apiName", "NBP_EUR");

    for (size_t i = 0; i < assets.size(); ++i) {
        QString name = QString::fromStdString(assets[i]->getName()).trimmed().toLower();
        QString urlStr;
        QString apiEngine = "";

        if (dynamic_cast<Stock*>(assets[i].get())) {
            urlStr = QString("https://query1.finance.yahoo.com/v8/finance/chart/%1").arg(QString::fromStdString(assets[i]->getName()));
            apiEngine = "Yahoo";
        }
        else if (dynamic_cast<Crypto*>(assets[i].get())) {
            urlStr = QString("https://api.coingecko.com/api/v3/simple/price?ids=%1&vs_currencies=pln").arg(name);
            apiEngine = "CoinGecko";
        }
        else if (dynamic_cast<PreciousMetal*>(assets[i].get())) {
            if (name == "silver" || name == "xag" || name == "si=f") {
                urlStr = "https://query1.finance.yahoo.com/v8/finance/chart/SI=F";
                apiEngine = "Yahoo";
            }
            else if (name == "gold" || name == "xau" || name == "gc=f") {
                urlStr = "https://query1.finance.yahoo.com/v8/finance/chart/GC=F";
                apiEngine = "Yahoo";
            }
        }

        if (!urlStr.isEmpty()) {
            QNetworkRequest request((QUrl(urlStr)));
            request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36");
            QNetworkReply* reply = networkManager->get(request);

            reply->setProperty("assetIndex", (uint)i);
            reply->setProperty("apiName", apiEngine);
        }
    }
}

void MainWindow::onTableDoubleClicked(int row, int column)
{
    if (row <= 0) return;

    int vectorIndex = row - 1;
    const auto& assets = myPortfolio.getAssets();

    if (vectorIndex < assets.size()) {
        if (Stock* stock = dynamic_cast<Stock*>(assets[vectorIndex].get())) {

            QString name = QString::fromStdString(stock->getName());
            double currentDivPerShare = stock->calculateEstimatedDividend() / stock->getQuantity();

            bool confirmed;
            double newDividend = QInputDialog::getDouble(
                this,
                "Dividend Update",
                QString("Enter the new estimated dividend per share for %1 (PLN):").arg(name),
                currentDivPerShare,
                0.0,
                10000.0,
                2,
                &confirmed,
                Qt::WindowFlags(),
                0.01
            );

            if (confirmed) {
                stock->updateDividend(newDividend);

                QString filePath = QCoreApplication::applicationDirPath() + "/my_portfolio.dat";
                SaveManager::savePortfolioBinary(myPortfolio, myCash, filePath.toStdString());

                refreshTable();
            }
        }
    }
}