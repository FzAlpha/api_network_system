# CentralPlatform

CentralPlatform is the modular platform layer of the API Network System. This
directory is organized as a set of explicit boundaries: shared infrastructure
belongs in `core`, public API concerns belong in `api`, business capabilities
belong in `modules`, and external technology integrations belong in
`providers`.

The directories are currently architectural placeholders. Add implementation
files inside the existing boundaries as each capability is developed rather
than moving cross-cutting logic into individual modules.

## Directory structure

### `core/`

Shared platform infrastructure used by multiple parts of the system:

- `config/` - configuration loading, environment handling, and settings.
- `errors/` - common error types and error translation.
- `logging/` - logging setup, formatting, and shared logging utilities.
- `security/` - security primitives and security policy helpers.
- `validation/` - reusable input and domain validation.
- `middleware/` - request/response middleware and cross-cutting pipeline
  behavior.
- `events/` - event contracts, dispatching, and event handling.
- `module_system/` - module registration, discovery, and lifecycle support.

### `api/`

The platform-facing API boundary:

- `gateway/` - API entry point, transport integration, and gateway concerns.
- `router/` - route registration and request dispatch.
- `versioning/` - API version negotiation and compatibility boundaries.
- `response/` - common response envelopes, serialization, and status mapping.

### `modules/`

Business capabilities are isolated here. Each module owns its own API,
services, models, and provider-facing code where applicable:

- `auth/` - authentication and authorization workflows.
- `users/` - user-facing domain operations.
- `messaging/` - messaging workflows, WebSocket support, and message models.
- `pdf/` - PDF operations and PDF provider integrations.
- `files/` - file operations and storage provider integrations.
- `notifications/` - notification capability boundary.
- `payments/` - payment capability boundary.
- `search/` - search capability boundary.

The more developed modules use these subdirectories:

- `api/` - module endpoints and transport adapters.
- `service/` - module application and domain services.
- `models/` - module data and domain models.
- `providers/` - module-specific external integrations.
- `websocket/` - real-time messaging transport concerns.

### `providers/`

Replaceable infrastructure adapters. Interfaces define the contract and
technology-specific directories contain implementations:

- `database/` - `interface/`, `postgres/`, `mysql/`, `mongodb/`, and `sqlite/`.
- `authentication/` - `interface/`, `jwt/`, `oauth/`, and `custom/`.
- `storage/` - `interface/`, `local/`, and `s3/`.
- `cache/` - `interface/` and `redis/`.

Provider implementations should be selected through configuration or
composition rather than being hard-coded into business modules.

### `application/`

Shared application-level coordination:

- `models/` - application-wide models.
- `services/` - services that coordinate multiple modules or providers.
- `business_logic/` - shared business rules that do not belong to one module.
- `repositories/` - persistence and data-access abstractions.

### `sdk/`

Client SDK locations for `cpp/`, `javascript/`, and `python/`.

### `cli/`

Command-line tooling lives here. `platform-cli` is the reserved entry point
for platform administration and developer workflows.

### `tests/`

Test boundaries are split into `unit/`, `integration/`, and `api/`.

### `docs/` and `examples/`

`docs/` contains platform documentation. `examples/` contains reference
applications for `chat_app/`, `pdf_app/`, and `ecommerce_app`.

### `config/`

Environment configuration files:

- `default.yaml` - shared defaults.
- `development.yaml` - local development overrides.
- `production.yaml` - production overrides.

Configuration files should contain deployment settings, not secrets. Secrets
should be supplied through the deployment environment or a secret manager.

## Build entry point

[`CMakeLists.txt`](./CMakeLists.txt) is the build entry point for this
directory. It currently provides a scaffold-safe interface target and a
`validate-structure` target. As source implementations are added, executable
and library targets can be introduced there without changing the top-level
architecture.