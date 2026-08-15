# Personal Investment Tracker

A desktop application built in C++ and the Qt Framework, designed for tracking and managing investment portfolios.

**Important Note:** This software functions as an asset manager, not a trading simulator. It is intended for investors holding long-term positions across various asset classes (dividend stocks, global ETFs, precious metals, cryptocurrencies) and provides a unified overview of net worth normalized to a single fiat currency (PLN).

<img width="986" height="777" alt="Zrzut ekranu 2026-08-15 120026" src="https://github.com/user-attachments/assets/179f79a2-c911-4de3-879e-bf9757c72196" />


## Core Design Principles

### 1. Cash as an Independent Asset Class
Unlike trading simulators where fiat currency is merely a transactional medium, this application treats cash as a distinct, standalone asset subclass. This reflects real-world portfolio management, where holding cash is an active allocation strategy. Cash balances contribute directly to the total portfolio value and asset allocation metrics.

### 2. API Rate Limit Security & Stability
Fetching live market data (Yahoo Finance, CoinGecko, NBP) is strictly optimized. API requests are executed only upon application startup or when a new asset is manually added. This architectural decision prevents the application from overloading external servers with background polling (e.g., using timers). It eliminates the risk of encountering HTTP 429 Too Many Requests errors or IP bans, ensuring stability for long-term portfolio tracking.

<img width="985" height="779" alt="Zrzut ekranu 2026-08-15 120040" src="https://github.com/user-attachments/assets/66552d3a-c0a5-437d-b5cb-81a62f27f449" />


## Technical Architecture (C++ / Qt)

The project leverages modern C++ paradigms and the Qt Framework to ensure memory safety, scalability, and clean code architecture.

*   **Object-Oriented Programming (OOP):** The system is built upon a base `Asset` class. Specific asset types (`Stock`, `Crypto`, `PreciousMetal`, `Cash`) inherit from this base, allowing the core portfolio engine to treat all holdings uniformly. Core functionalities, such as value calculation and UI data formatting, are implemented via virtual functions overridden in derived classes.
*   **Standard Template Library (STL):** Extensive use of STL containers, specifically `std::vector`, for dynamic data storage and iteration.
*   **Smart Pointers:** Memory management is handled via `std::unique_ptr`, preventing memory leaks during the lifecycle of polymorphically allocated asset objects.
*   **Qt6 Framework:** Utilizes `QtWidgets` for the graphical interface, `QtNetwork` (`QNetworkAccessManager`) for asynchronous HTTP REST API calls, and `QtCore` for JSON parsing and event-driven signals/slots.
*   **Data Serialization:** Binary `.dat` file management for local preservation of portfolio states across sessions.

## Market Data Integration

The application dynamically converts foreign assets to PLN in real-time using live NBP exchange rates. Standardized inputs are required for adding assets:

<img width="987" height="780" alt="Zrzut ekranu 2026-08-15 120127" src="https://github.com/user-attachments/assets/b480503d-2f45-4a4b-9d44-3a1dc6c47549" />


*   **Stocks & ETFs (Yahoo Finance):** Use standard exchange tickers. For the Warsaw Stock Exchange (GPW), append the `.WA` suffix (e.g., `XTB.WA`, `KRU.WA`). For global ETFs, use standard tickers.
*   **Precious Metals (Yahoo Finance):** Use global commodity tickers. The system auto-normalizes common inputs (e.g., parsing "silver" or "gold" directly into `XAG` and `XAU`).
*   **Cryptocurrencies (CoinGecko):** Use full CoinGecko API identification strings (e.g., `bitcoin`, `ethereum`).

## Roadmap & Future Improvements

While the application is currently fully functional for its primary use case, there are several planned enhancements for future releases:

*   **Multi-Currency Purchase Support:** Currently, the application uses PLN as the unified base currency for all inputs (including the `Purchase Price` of foreign assets). Future versions will introduce a currency selector in the "Add Asset" dialog, automatically fetching historical exchange rates from the NBP API for the specific day of purchase.
*   **Dividend Yield Tracking:** Implementing an automated historical dividend tracker for dividend-paying equities to calculate Total Return (Capital Gains + Dividends).
*   **Data Export:** Adding functionality to export the portfolio state and transaction history to a `.csv` or `.xlsx` format for tax calculation purposes.

## Build Instructions

1. Clone this repository to your local machine.
2. Open the project in Visual Studio (requires the Qt VS Tools extension).
3. Ensure the Qt environment is configured with `Core`, `GUI`, `Widgets`, and `Network` modules enabled for both Debug and Release builds.
4. Build the solution and run.
