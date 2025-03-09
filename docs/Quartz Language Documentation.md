# Quartz Language Documentation
## Overview
Quartz is a statically-typed language, high-performance language designed for quantitive trading. Built to be fast, it provides easy-to-learn and concise syntax
### Key Features
#### Current
#### Future
- Static Typing - Prevents runtime overhead and ensures high speed execution
- Optimized Data Handling - Efficiently processes market data
- Low Latency - Key to the high frequency nature of quantative trading
- Compiled and Interpreted Modes - Able to compiling for max performance, or interperate for fast prototyping and testing
## Syntax
### Strategy
In Quartz, a strategy is defined using the `strategy` keyword, followed by the strategy name. The strategy consists of initialization, backtesting definition, and  logic to handle incoming data
#### Example
```
strategy MovingAverageCrossover {
    // Initialize strategy
    init() -> void {

    }

    // Process incoming data, generate trade signals
    on_data() -> void {

    }
}
```
### Define Acccessible Variables
You an define what variables you want to access within `on_data()` by using the `define_input_variables()` function.
#### Example
```
define_input_variables(price, short_window, long_window)
```
#### Accessible Variables
- `price`
- `short_window`
- `long_window`
### Adding Data Sources
You can add multiple data soures by using the `add_data_source()` function
#### Example
```
// add_data_source(ticker, interval[optional])
add_data_source("AAPL", "1d")
add_data_source("GOOG")
```
### Emitting Signals
- Signals are emitted using the `emit_signal` function, which immediately triggers a trade signal, signals can only be emitted during an `on_data()` cycle.
- If no signal is emitted in an `on_data()` cycle, a hold signal will be assumed
#### Example
```
// emit_signal(BUY, SELL, or HOLD)
emit_signal(BUY)
```
### Defining Variables
#### Constants
- `const`: Used for parameters that remain the same throughout the strategy. These are optimized for speed and handled at compile time
#### Mutable Variables
- `var`: Used for variables that have changing values
## Code Examples
###
```
strategy MovingAverageCrossover {
    // Constants
    const data_source: string = "AAPL"   // Data source: AAPL stock
    const interval: string = "1d"   // Data interval: 1 day
    const short_window = 10   // Short moving average window
    const long_window = 50   // Long moving average window

    // Initialize strategy - this will run once when strategy is loaded
    init() -> void {
        add_data_source(data_source, interval)   // Add AAPL stock data with interval of 1 day

        // Define input variables for the strategy
        define_input_variables(price, short_ma, long_ma)
    }

    // Process incoming data - this is called on each new data point
    on_data() -> void {
        // Generate a trade signal based on the moving average crossover
        if (short_ma > long_ma) {
            emit_signal(BUY)   // Buy when short MA crosses above long MA
            return
        } else if (short_ma < long_ma) {
            emit_signal(SELL)   // Sell when short MA crosses below long MA
            return
        } else {
            emit_signal(HOLD)   // Hold if there is no crossover, this is optional as no emitted signal will assume hold
            return
        }
    }
}
```