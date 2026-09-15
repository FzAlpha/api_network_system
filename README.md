# API Network System

## Overview

**API Network System** is the foundation for **CentralPlatform**, a high-performance, modular backend platform engineered in modern **C++ (C++23)**. It is designed to host shared platform capabilities, domain business modules, and external infrastructure integrations behind a unified, robust API gateway.

The platform enforces a strict separation of concerns among platform core services, domain business modules, and interchangeable infrastructure providers. This enables individual services to be developed, tested, and scaled independently without vendor lock-in.

---

## Architectural Highlights

- **Modern C++23 Standard**: Employs `std::expected` for type-safe, monadic error propagation and modern RAII patterns across all modules and providers.
- **Pluggable Module Architecture**: Core `IModule` interface and `ModuleManager` providing dynamic registration, sequential initialization, and safe reverse-order teardown.
- **Decoupled Infrastructure Providers**: Technology-agnostic interface layer (`IDatabaseProvider`) with concrete implementations (e.g., SQLite via `SQLiteCpp`), allowing database backends (PostgreSQL, MySQL, SQLite, MongoDB) to be swapped with zero impact on business modules.
- **Asynchronous HTTP API Gateway**: Multithreaded web server module (`HttpServerModule`) powered by Crow and Asio for high-throughput HTTP routing.
- **Structured Error Handling**: Unified `AppError` model with `ErrorType` categorization and automatic HTTP status code mapping.
- **Reference Applications & Test Suite**: Ready-to-run examples (such as `chat_app`) demonstrating end-to-end module lifecycle, database transactions, and REST endpoints.

---

## Repository Layout

All platform implementation and scaffold work lives under [`CentralPlatform/`](./CentralPlatform/). Detailed internal specifications are available in the [CentralPlatform README](./CentralPlatform/README.md).

| Path | Responsibility | Implementation Status |
| --- | --- | --- |
| [`CentralPlatform/Core/`](./CentralPlatform/Core/) | Cross-cutting infrastructure: module lifecycle, error models, logging, config | Active (`module_system`, `errors`) |
| [`CentralPlatform/api/`](./CentralPlatform/api/) | HTTP API gateway, routing, versioning, response envelopes | Active (`gateway/HttpServerModule`) |
| [`CentralPlatform/modules/`](./CentralPlatform/modules/) | Isolated business domains (auth, users, messaging, files, payments, etc.) | Active (`auth/AuthModule`), Scaffolded |
| [`CentralPlatform/providers/`](./CentralPlatform/providers/) | Pluggable infrastructure adapters (database, authentication, storage, cache) | Active (`database/sqlite`), Scaffolded |
| [`CentralPlatform/application/`](./CentralPlatform/application/) | Application orchestration, shared business logic, and repository contracts | Scaffolded |
| [`CentralPlatform/examples/`](./CentralPlatform/examples/) | Reference applications demonstrating platform features | Active (`chat_app`) |
| [`CentralPlatform/tests/`](./CentralPlatform/tests/) | Unit, integration, and end-to-end API test suites | Active (`unit/test_app_error`) |
| [`CentralPlatform/sdk/`](./CentralPlatform/sdk/) | Client SDKs (C++, JavaScript, Python) | Scaffolded |
| [`CentralPlatform/config/`](./CentralPlatform/config/) | Environment-specific configuration files (`default`, `development`, `production`) | Active |
| [`CentralPlatform/cli/`](./CentralPlatform/cli/) | Command-line developer and administration utilities | Scaffolded |

---

## Tech Stack & Dependencies

- **Language**: C++23 (`-std=c++23`)
- **Build System**: CMake 3.20+
- **HTTP Server**: [Crow v1.2.0](https://github.com/CrowCpp/Crow) (via CMake `FetchContent`)
- **Asynchronous I/O**: [Asio 1-30-2](https://github.com/chriskohlhoff/asio)
- **Database Wrapper**: [SQLiteCpp 3.3.1](https://github.com/SRombauts/SQLiteCpp) & SQLite3

---

## Prerequisites

- **CMake** >= 3.20
- **C++23 Compliant Compiler**: GCC 13+, Clang 16+, or MSVC 2022+
- **Git** (for CMake `FetchContent` automated dependency downloads)
- **Pthreads** (standard on POSIX systems)

---

## Getting Started

### 1. Configure the Build

```sh
cmake -S CentralPlatform -B CentralPlatform/build
```

### 2. Build the Project

```sh
cmake --build CentralPlatform/build
```

### 3. Run Unit Tests

Execute the error handling unit test suite:

```sh
./CentralPlatform/build/test_app_error
```

### 4. Run the Example Application (`chat_app`)

Launch the reference chat application:

```sh
./CentralPlatform/build/chat_app
```

The example initializes the `ModuleManager`, connects to an SQLite database, creates a user table, executes queries, and starts the Crow HTTP server on port `8080`.

In another terminal, test the running API gateway:

```sh
curl http://localhost:8080/api/status
```

Expected response:
```json
{"status": "Engine is running"}
```

### 5. Validate Scaffold Structure

```sh
cmake --build CentralPlatform/build --target validate-structure
```

---

## Development Roadmap

- [x] **C++23 Base Setup & Modern CMake Configuration**
- [x] **Core Error System (`AppError` & `ErrorType`) with `std::expected`**
- [x] **Dynamic Module System (`IModule` & `ModuleManager`)**
- [x] **Database Provider Interface (`IDatabaseProvider`) & SQLite Integration (`sqlProvider`)**
- [x] **HTTP API Gateway Module (`HttpServerModule` via Crow)**
- [x] **Chat App Reference Example**
- [ ] Database providers for PostgreSQL, MySQL, and MongoDB
- [ ] JWT / OAuth2 Authentication Provider
- [ ] Real-time WebSocket Messaging Pipeline
- [ ] Centralized Configuration Manager (`YAML-CPP`)
- [ ] Client SDKs (C++, Python, TypeScript/JavaScript)