# Simple Password Manager (C)

A simple terminal-based password manager written in C.
It allows you to store services and passwords locally in a binary file.

---

## Features

- Add new services with passwords
- List stored services
- List services with their passwords
- Remove services by ID 
- Persistent storage using a local file (`.passwords`)
- Change passwords by name
- Interactive confirmation prompts to avoid mistakes

---

## Commands

| Command | Description |
|---------|------------|
| `add "service_name"` | Add a new service |
| `remove <id>` | Remove a service by ID |
| `list` | List all services |
| `change "service_name"`| Change a service password |
| `list passwords` | List services and their passwords |
| `help` | Show help panel |
| `exit` | Save and exit |

---

## requirments `gcc` compiler and gnu `MAKE` tool.

---
## Build & run
```bash
make
make run
```

## clean executables and reset the program
```bash
make clean
```
## SCREENSHOT
![alt text](https://github.com/skinwalker3654/password_manager/blob/main/.github/screenshot.png?raw=true)
---
