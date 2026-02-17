# 🏛️ 7 Wonders Duel - C++ Implementation

A comprehensive, digital implementation of the award-winning board game *7 Wonders Duel*.  
This project demonstrates advanced software engineering concepts in **Modern C++**, featuring a robust **MVC architecture**, a custom **AI opponent**, and network capabilities.

![C++](https://img.shields.io/badge/C%2B%2B-23-00599C?style=flat&logo=c%2B%2B)
![Qt](https://img.shields.io/badge/Qt-6.0%2B-41CD52?style=flat&logo=qt)
![CMake](https://img.shields.io/badge/Build-CMake-064F8C?style=flat&logo=cmake)
![License](https://img.shields.io/badge/License-MIT-green)

---

## 🌟 Key Features

- **Professional Build System (CMake):** Unlike standard Visual Studio solutions, this project uses **CMake**, ensuring portability across Windows, Linux, and macOS.
- **MVC Architecture:** Strict separation of concerns using the **Model-View-Controller** pattern for scalable and maintainable code.
- **Design Patterns:** Heavy usage of **Factory Pattern** for card and wonder generation and **Observer Pattern** for UI updates.
- **Artificial Intelligence:** Includes a single-player mode against an AI opponent capable of making strategic decisions.
- **Multiplayer Networking:** Client-Server architecture allowing networked gameplay.
- **Qt GUI:** A rich graphical interface built with Qt Widgets, featuring custom assets and animations.
- **Unit Testing:** Comprehensive test suite ensures logic stability.

---

## 🛠️ Build & Run (CMake)

This project uses **CMake** to manage dependencies and the build process, making it environment-agnostic.

### Prerequisites

- C++ Compiler (GCC, Clang, or MSVC) supporting C++17
- CMake 3.10+
- Qt 6 Framework

---

## 🔧 Building the Project

### 1️⃣ Clone the repository

```bash
git clone https://github.com/your-username/7wondersduel.git
cd 7wondersduel
```

---

### 2️⃣ Generate Build Files

```bash
mkdir build
cd build
cmake ..
```

---

### 3️⃣ Compile

```bash
cmake --build .
```

---

### 4️⃣ Run

- On **Windows**:
  ```
  Debug\ProjectModernCpp.exe
  ```

- On **Linux / macOS**:
  ```
  ./ProjectModernCpp
  ```

---

## 🏗️ Architecture Overview

The codebase is organized to reflect modern C++ standards.

---

### 1️⃣ The Model (`src/model`)

Contains the core business logic and state of the game. It is completely independent of the UI.

- **Entities:** `Player`, `Card`, `Wonder`, `ProgressToken`
- **State Management:** `GameState` manages the flow, resources, and victory conditions

---

### 2️⃣ The View (`src/view`)

Handles user interaction and visualization.

- **Qt Implementation:** `MainWindow` uses `.ui` forms for layout
- **Console Fallback:** `ConsoleUI` allows for text-based debugging and gameplay

---

### 3️⃣ The Controller (`src/controller`)

Mediates between Model and View.

- **GameController:** Processes user inputs and updates the model
- **AIController:** Automates moves for the computer opponent

---

### 4️⃣ Networking (`src/server`)

Implements the multiplayer logic.

- **GameServer:** Manages connections and game synchronization
- **JSON Serialization:** Uses `nlohmann/json` for data packet transmission

---

## 🧪 Testing

The project includes a dedicated test suite located in the `tests/` folder.

To run tests:

```bash
cd build
ctest -V
```

Tests cover:

- Card effects
- Resource production logic
- Factory instantiation

---

## 📂 Project Structure

```
├── assets/             # Game images (cards, wonders, tokens)
├── include/            # External libraries (nlohmann/json)
├── src/
│   ├── controller/     # Game logic coordination & AI
│   ├── factories/      # Object creation patterns
│   ├── model/          # Game data and rules
│   ├── server/         # Networking logic
│   ├── utils/          # Helper functions & constants
│   ├── view/           # Qt GUI & Console UI
│   ├── main.cpp        # Entry point
│   └── resources.qrc   # Qt Resource System
├── tests/              # Unit tests
├── CMakeLists.txt      # Build configuration
└── README.md           # Documentation
```

---

## 📄 License

This project is for educational purposes.  
*7 Wonders Duel* is a trademark of Repos Production.
