
<div>
<h1>Quartz 🚀</h1>
<img src="./quartz.png" alt="logo" width="80" height="200">
<p><b>Quartz</b> is a WIP high-performance, statically-typed language designed specifically for quantitative trading. Built for speed, it features a concise and easy-to-learn syntax while providing the power and flexibility needed for algorithmic trading strategies.</p>
</div>


## 📋 Table of Contents
- [🔍 Overview](#overview)
- [🧑‍💻 Key Features](#key-features)
- [⚡Quick Start](#quick-start)
  - [🛠️ Installation](#installation)
  - [📝 Syntax](#syntax)
- [🚀 Usage](#usage)
- [💻 Examples](#examples)
- [🤝 Contributing](#contributing)
  - [How to Contribute](#how-to-contribute)
- [📜 License](#license)
  - [Intellectual Property](#intellectual-property)
## Overview
Quartz is designed for quantitative traders who need a fast, reliable language for building and testing strategies. It integrates both compiled and interpreted modes, allowing you to choose the right trade-off between performance and rapid prototyping.
## Key Features
- **⏩ Static Typing:** Ensures high-speed execution and prevents runtime overhead.
- **📊 Optimized Data Handling:** Handles large market data efficiently for real-time trading.
- **⚡ Low Latency:** Essential for high-frequency trading strategies.
- **🔄 Compiled and Interpreted Modes:** Compile for maximum performance & deployment or interpret for fast prototyping & backtesting.
- **✨Concise Syntax:** Built to be easy to learn and write, especially for algorithmic trading.
## Quick Start
### Installation
#### Requirements
- C++ compiler
- Cmake
#### Setup
To get started with Quartz, clone the repository:

```bash
git clone https://github.com/londonmax12/quartz.git
```
### Syntax
#### Strategy Definition
A strategy in Quartz is defined with the `strategy` keyword. It includes initialization, backtesting, and logic to handle market data.
```
strategy MovingAverageCrossover {
    // Initialize strategy
    init() -> void {
        // Initialization code
    }

    // Process incoming data, generate trade signals
    on_data() -> void {
        // Trading logic
    }
}
```
#### Defining Accessible Variables
Define input variables that can be accessed within the `on_data()` method using the `define_input_variables()` function.
- E.g. `define_input_variables(price)`;
#### Adding Data Sources
Add multiple data sources with the `add_data_source()` function.
```
add_data_source("AAPL", "1d");  // Daily data for AAPL
add_data_source("GOOG");        // Default interval for GOOG
```
#### Emitting Signals
Signals trigger trade actions (`BUY`, `SELL`, `HOLD`). You can only emit signals during the `on_data()` method.
```
emit_signal(BUY);   // To buy the asset
emit_signal(SELL);  // To sell the asset
emit_signal(HOLD);  // To hold the asset
```
#### Defining Variables
- **Constants**: Use `const` for parameters that remain unchanged
- **Mutable Variables**: Use `var` for variables that change during execution
##### Example
```
strategy MovingAverageCrossover {
    // Constants
    const data_source: string = "AAPL";
    const interval: string = "1d";
    const short_window = 10;
    const long_window = 50;

    // Initialize strategy
    init() -> void {
        add_data_source(data_source, interval);
        define_input_variables(price, short_ma, long_ma);
    }

    // Process incoming data
    on_data() -> void {
        if (short_ma > long_ma) {
            emit_signal(BUY);   // Buy when short MA crosses above long MA
        } else if (short_ma < long_ma) {
            emit_signal(SELL);  // Sell when short MA crosses below long MA
        } else {
            emit_signal(HOLD);  // Hold if there is no crossover
        }
    }
}
```
## Usage
1. Define Your Strategy: Use the `strategy` keyword to declare a new strategy.
2. Add Data Sources: Use the `add_data_source()` function to bring in market data.
3. Define Variables: Use `define_input_variables()` to specify the variables that can be used within the strategy.
4. Implement Logic: Define trading logic in `on_data()` and generate trade signals with `emit_signal()`.
## Examples
You'll find various code examples in the `examples` folder
## Contributing
We welcome and appreciate contributions to Quartz! Whether it's issue reports, suggesting new features, submitting bug fixes, or even typo fixes.
### How to Contribute
1. **Fork** this repo to your own Github account
2. Create a **branch** for your new changes
3. **Implement** your fixes or features
4. Submit a **pull** request with a description of your changes
## Liscense
- Distrubted under the MIT license, for more details read the LICENSE file
### Intellectual Property
- All logos, trademarks, images, and other creative works are exclusive property of the **Quartz** project. You are permitted to use the Quartz logo, as long as you provide proper attribution to the **Quartz** project
#### Logo Usage Guidelines
- The logo may be used in your own projects, websites, or materials, provided that the **Quartz Project** is credited.
- The attribution must be clear and visible, ideally in a prominent location near the logo or in a credits section.

For example, you can include an attribution statement like:
> Logo used with permission from the [Quartz Project](https://github.com/londonmax12/quartz).
