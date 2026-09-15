# CentralPlatform

**CentralPlatform** is the core modular platform layer of the API Network System. It is engineered in modern **C++ (C++23)** to provide high-throughput, extensible backend services, clean domain boundaries, and pluggable infrastructure integrations.

The architecture is divided into explicit layers:
- **`Core/`**: Cross-cutting foundation (module lifecycle, error models, logging, config).
- **`api/`**: API gateway, routing, versioning, and response serialization.
- **`modules/`**: Isolated business domains (auth, users, messaging, files, etc.).
- **`providers/`**: Pluggable technology adapters (databases, auth providers, storage, caches).
- **`application/`**: Shared orchestration, cross-module workflows, and repositories.
- **`examples/`**: Reference applications showcasing platform integrations.
- **`tests/`**: Unit, integration, and API test suites.

---

## Architectural Breakdown & Components

```
CentralPlatform/
├── Core/                     # Shared platform infrastructure
│   ├── errors/               # Structured error handling (AppError, ErrorType)
│   ├── module_system/        # Module lifecycle management (IModule, ModuleManager)
│   ├── config/               # Configuration management
│   ├── events/               # Event dispatching
│   ├── logging/              # Centralized logging
│   ├── middleware/           # Pipeline and middleware hooks
│   ├── security/             # Cryptographic & security policies
│   └── validation/           # Domain and input validation
│
├── api/                      # Transport & API boundary
│   ├── gateway/              # HTTP API gateway (HttpServerModule via Crow)
│   ├── response/             # Response envelopes & status formatting
│   ├── router/               # Request routing
│   └── versioning/           # API version negotiation
│
├── modules/                  # Isolated business capabilities
│   ├── auth/                 # Authentication & authorization (AuthModule)
│   ├── users/                # User management & profile services
│   ├── messaging/            # Real-time messaging & WebSocket handlers
│   ├── files/                # File upload & metadata management
│   ├── pdf/                  # PDF generation & processing
│   ├── notifications/        # Notification dispatching
│   ├── payments/             # Payment processing
│   └── search/               # Search engine integrations
│
├── providers/                # Pluggable infrastructure adapters
│   ├── database/             # IDatabaseProvider interface & sqlite/sqlProvider
│   ├── authentication/       # JWT, OAuth, and custom identity providers
│   ├── storage/              # Local filesystem & S3 storage providers
│   └── cache/                # Redis and in-memory cache adapters
│
├── application/              # Shared orchestration & coordination
│   ├── business_logic/       # Cross-cutting business rules
│   ├── models/               # Application-level data structures
│   ├── repositories/         # Persistence interfaces
│   └── services/             # Multi-module orchestrators
│
├── examples/                 # Reference applications
│   ├── chat_app/             # End-to-end integration demo (Crow + SQLite + Modules)
│   ├── ecommerce_app/        # E-commerce reference app scaffold
│   └── pdf_app/              # PDF processing reference app scaffold
│
├── tests/                    # Test suites
│   ├── unit/                 # Unit tests (test_app_error)
│   ├── integration/          # Inter-module integration tests
│   └── api/                  # End-to-end API tests
│
├── config/                   # YAML environment configurations
├── cli/                      # Developer CLI utilities (platform-cli)
└── sdk/                      # Client SDKs (cpp, javascript, python)
```

---

## Core Subsystems & Implementation Details

### 1. Module System (`Core/module_system`)

Provides lifecycle management for independent platform capabilities:
- **`IModule`**: Abstract base class declaring:
  - `getName()`: Returns the unique module identifier.
  - `init()`: Returns `std::expected<void, AppError>` for safe startup.
  - `shutdown()`: Returns `std::expected<void, AppError>` for graceful cleanup.
- **`ModuleManager`**: Manages registration, ordered initialization, and guaranteed reverse-order teardown for all registered modules.

### 2. Error Handling (`Core/errors`)

Utilizes C++23 monadic error handling:
- **`AppError`**: Uniform error structure containing:
  - `ErrorType` enum (`NotFound`, `ValidationFailed`, `Unauthorized`, `Internal`)
  - `http_status` integer mapping (e.g., `404`, `400`, `401`, `500`)
  - `message` detailed description
- Functions return `std::expected<T, AppError>` instead of throwing runtime exceptions.

### 3. Database Providers (`providers/database`)

Decouples storage engines from domain modules:
- **`IDatabaseProvider`**: Abstract interface defining:
  - `connect(const std::string& connectionString)`
  - `disconnect()`
  - `execute(const std::string& sqlQuery)` returning `std::expected<QueryResult, AppError>`
- **`QueryResult`**: Structured rows (`std::vector<std::vector<std::string>>`) and `affectedRows`.
- **`sqllite::sqlProvider`**: Production-ready SQLite driver built on top of `SQLiteCpp`.

### 4. API Gateway (`api/gateway`)

- **`HttpServerModule`**: Integrates [Crow](https://github.com/CrowCpp/Crow) to expose asynchronous, multithreaded REST API endpoints.
- Runs the web server in a dedicated background worker thread and conforms to the `IModule` lifecycle.
- Built-in health check endpoint: `GET /api/status`.

### 5. Reference Application (`examples/chat_app`)

Demonstrates complete system integration:
1. Instantiates `ModuleManager`.
2. Connects and registers `sqllite::sqlProvider` into `ModuleManager` as `std::shared_ptr<IDatabaseProvider>`.
3. Registers `AuthModule` (passing `manager` reference) and `HttpServerModule`.
4. Calls `manager.initializeAll()`, where `AuthModule` dynamically resolves `IDatabaseProvider` from the registry.
5. Executes database operations (creates table, inserts user record, queries user).
6. Runs the Crow HTTP server listening on `http://localhost:8080/api/status`.
7. Performs safe, reverse-order shutdown and database disconnect on exit.

---

## Build Targets & Dependencies

The project uses CMake (>= 3.20) with automated dependency management via `FetchContent`:
- **`asio`**: Low-level asynchronous I/O (`asio-1-30-2`).
- **`Crow`**: Fast and easy-to-use C++ web framework (`v1.2.0`).
- **`SQLiteCpp`**: RAII SQLite C++ wrapper (`3.3.1`).

### CMake Targets

| Target | Type | Description |
| --- | --- | --- |
| `central_platform` | Interface Library | Base include directories and C++23 compilation flags |
| `central_platform_errors` | Interface Library | Header-only error handling (`Core/errors/apperrors.h`) |
| `test_app_error` | Executable | Unit test suite for `AppError` and `std::expected` |
| `chat_app` | Executable | End-to-end reference application |
| `validate-structure` | Custom Target | Scaffold verification check |

---

## Building and Running

### 1. Build

```sh
cmake -S . -B build
cmake --build build
```

### 2. Run Unit Tests

```sh
./build/test_app_error
```

Output:
```
User found: user_1
Error: User not found | HTTP Status: 404
```

### 3. Run the Reference Application

```sh
./build/chat_app
```

Output:
```
SQL provider successfully connected to chat_users.db
AuthModel initializing ...
AuthModel successfully recieved the database provider
Server starting on port 8080
chat app, engine started successfully!
Fetched User: AdminUser | admin@chat.com
Press Enter to stop the server...
```

Test the live endpoint:
```sh
curl http://localhost:8080/api/status
# Output: {"status": "Engine is running"}
```

### 4. Validate Structure

```sh
cmake --build build --target validate-structure
```

---

## Design Principles & Guidelines

1. **Modern C++23**: Use RAII, smart pointers (`std::unique_ptr`), and `std::expected` for explicit error handling without overhead.
2. **Strict Layering**: Never import providers directly into core or higher-level business modules. Always depend on provider interfaces.
3. **Lifecycle Encapsulation**: Any service managing threads, connections, or background loops must implement `IModule` and support clean shutdown.
4. **Configuration over Hardcoding**: Infrastructure backends must be configurable through environment settings or dependency injection.