# Limit Order Book

A C++ implementation of a Limit Order Book with price-time priority, order matching, multiple order types, time-in-force conditions, and market statistics.

## Overview

This project implements a simplified electronic trading engine that maintains a central limit order book and matches incoming buy and sell orders according to price-time priority.

The order book maintains separate buy and sell sides, organizes orders by price level, and executes trades whenever compatible orders are received.

The project is developed in C++ with an emphasis on efficient data structures, modular design, and order matching logic.

## Features

### Order Management
- Place BUY and SELL orders.
- Support for LIMIT and MARKET orders.
- Cancel existing orders.
- Modify existing orders.
- Maintain unique order IDs.

### Order Matching
- Price-time priority matching.
- Automatic execution of compatible orders.
- Partial order execution.
- Multiple trades from a single incoming order.
- Execution at the resting order's price.

### Time-in-Force Conditions

| Type | Description |
|---|---|
| GTC | Unfilled quantity remains in the order book until cancelled or executed. |
| IOC | Executes immediately against available liquidity and cancels the remaining quantity. |
| FOK | Executes only if the entire order quantity can be filled immediately. |
| POST_ONLY | Prevents an order from executing immediately by rejecting it if it would match an existing order. |

### Market Information
- Display the complete order book.
- Display executed trades.
- Display market statistics.
- Display aggregated order book depth.

### Market Statistics

The system calculates:
- Last Traded Price
- Highest Traded Price
- Lowest Traded Price
- Total Traded Quantity
- Total Number of Trades
- Volume Weighted Average Price

## Data Structures

The order book uses the following data structures:

| Data Structure | Purpose |
|---|---|
| `std::map` | Organizes price levels in sorted order. |
| `std::list` | Maintains FIFO order within each price level. |
| `std::unordered_map` | Provides efficient lookup of orders by ID. |
| `std::vector` | Stores executed trades. |
| `std::unordered_set` | Tracks previously used order IDs. |

### Price-Time Priority

The order book follows price-time priority:

1. Orders with better prices receive execution priority.
2. Orders at the same price level are executed in FIFO order.
3. Buy orders are organized in descending price order.
4. Sell orders are organized in ascending price order.

This ensures that the best available prices are matched first while preserving time priority within each price level.

## Usage

The program provides an interactive terminal interface.

| Command | Operation |
|---|---|
| P | Place Order |
| C | Cancel Order |
| M | Modify Order |
| O | Display Order Book |
| T | Display Executed Trades |
| S | Display Market Statistics |
| B | Display Order Book Depth |
| E | Exit |

## Author

**Sarvesh Senthil**

GitHub: [@srvshsenthil](https://github.com/srvshsenthil)
