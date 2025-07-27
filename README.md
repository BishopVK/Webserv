| **Score**           | **Description**     |
|-----------------------|---------------|
| <p align="center"><img width="200px" alt="170px" src="./img/Score_100.png"></p> | **Mandatory part** |

---

# Webserv  

A lightweight **HTTP/1.1 server** written in **C++98**, developed as part of the curriculum at **42 Madrid**.  

The project demonstrates core concepts of **network programming, event-driven architectures, and HTTP protocol implementation**, providing a foundation for building real-world servers.

---

## 📑 Table of Contents  
- [✨ Features](#-features)  
- [🛠️ Compilation](#%EF%B8%8F-compilation)  
- [📖 Usage](#-usage)  
- [⚙️ Configuration](#%EF%B8%8F-configuration)  
- [🏗️ Architecture Overview](#%EF%B8%8F-architecture-overview)  
- [🧪 Testing](#-testing)  
- [🧹 Cleaning](#-cleaning)  
- [👥 Authors](#-authors)  
- [📌 Disclaimer](#-disclaimer)  

---

## ✨ Features  

- **Full HTTP/1.1 support** – Implements **GET**, **POST**, and **DELETE**.  
- **Event-driven architecture** – Uses a custom multiplexer to handle multiple clients efficiently.  
- **Flexible configuration system** – Nginx-style directives with `server` and `location` blocks.  
- **CGI execution** – Runs dynamic scripts (e.g., Python, PHP) via the Common Gateway Interface.  
- **File upload support** – Accepts and stores uploaded files via POST requests.  
- **Custom error pages & autoindex** – User-friendly error handling and directory listing.  
- **Virtual hosting** – Serve multiple domains on the same IP/port.  
- **Logging system** – Generates runtime logs for debugging and monitoring.

---

## 🛠️ Compilation  

```bash
make
```

- Uses strict compilation flags:  
  ```bash
  -Wall -Wextra -Werror -std=c++98
  ```
- Ensures **C++98 compliance** and adherence to 42’s code quality standards.

---

## 📖 Usage  

Run the server with a configuration file:  

```bash
make run
```
or
```bash
./webserv [configuration_file]
```

Example:

```bash
./webserv config.conf
```

The configuration file defines **server directives**, **locations**, and other behavior.

---

## ⚙️ Configuration  

Webserv uses a configuration format inspired by **Nginx**, with support for the following directives:

| Directive              | Context           | Description                                  |
|------------------------|------------------|----------------------------------------------|
| `listen`               | server           | IP address and port to bind (required)      |
| `server_name`          | server           | Virtual host name                           |
| `root`                 | server/location  | Document root for serving files             |
| `index`                | server/location  | Default index file(s)                       |
| `error_page`           | server/location  | Custom error page mappings                  |
| `client_max_body_size` | server/location  | Maximum allowed size for request bodies     |

---

## 🏗️ Architecture Overview  

Webserv is designed around an **event-driven loop** with non-blocking I/O:

- **HttpServer** – Core event loop, accepts and manages client connections.  
- **HttpRequestHandler** – Parses and routes HTTP requests.  
- **ClientConnection** – Maintains per-client state and lifecycle.  
- **Multiplexer** – Custom I/O manager to handle concurrent sockets without blocking.  

This architecture allows the server to manage multiple clients **simultaneously and efficiently**.

---

## 🧪 Testing  

The project includes **test resources** for validation:  
- HTML forms for **POST and file upload** testing.  
- CGI scripts for **dynamic content execution**.  
- Compatible with tools like **curl** and **ApacheBench (ab)** for functional and stress testing.  

---

## 🧹 Cleaning  

```bash
make clean    # Remove object files  
make fclean   # Full cleanup (including binary)  
make re       # Rebuild everything  
```

---

## 👥 Authors

Developed by students at **42 Madrid**:  

- [Daniel Jiménez Graindorge](https://github.com/BishopVK)
- [Sergio Fernández Fernández](https://github.com/zenix-s)
- [Inés Sainz Ramón](https://github.com/ines-sainz)

---

## 📌 Disclaimer

> This project was built **for educational purposes** as part of the **42 School curriculum**. It is not intended for production use but demonstrates a deep understanding of **network protocols, system programming, and C++ software architecture**.
[![Ask DeepWiki](https://deepwiki.com/badge.svg)](https://deepwiki.com/BishopVK/Webserv)
