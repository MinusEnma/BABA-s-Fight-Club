# BABA's Fight Club

**A Terminal-Based Multiplayer Fighting Game in C**

BABA's Fight Club is a terminal-based multiplayer fighting game written in C. Players choose their fighting styles, select cards to build combos, and battle against each other using dice rolls and combat mechanics.

## Features

- **Fighting Styles** – Choose a fighting style that influences your combat abilities.
- **Card-Based Combat** – Select and queue cards to create attack combinations.
- **Dice System** – Use dice rolls to determine combat outcomes.
- **Player System** – Manage player stats, health, energy, and other combat attributes.
- **Stagger System** – Build up stagger damage to affect your opponent during combat.
- **Multiplayer Support** – Connect two players through a client-server system.
- **Round-Based Combat** – Battle through multiple rounds until a player is defeated.

## Project Structure

```text
BABA-s-Fight-Club/
│
├── card/
│   ├── card.h
│   ├── cardDatabase.c
│   └── cardDatabase.h
│
├── dice/
│   ├── dice.c
│   └── dice.h
│
├── fightingStyle/
│   ├── fightingStyle.c
│   └── fightingStyle.h
│
├── player/
│   ├── player.c
│   └── player.h
│
├── server/
│   ├── client.c
│   ├── server.c
│   └── server.h
│
├── .gitignore
├── compiler.sh
└── README.md
```

## Directory Overview

| Directory / File | Description |
|---|---|
| `card/` | Contains card structures, definitions, and the card database. |
| `dice/` | Handles dice mechanics and rolls. |
| `fightingStyle/` | Defines the available fighting styles and their attributes. |
| `player/` | Handles player data, stats, and combat-related mechanics. |
| `server/` | Contains the server and client programs for multiplayer communication. |
| `compiler.sh` | Shell script used to compile the server and client executables. |

## Requirements

- GCC compiler
- Bash shell
- Linux environment
- POSIX socket support

## How to Compile

1. Open a terminal in the project directory.
2. Give the compiler script permission to execute:

   ```bash
   chmod +x compiler.sh
   ```

3. Run the compiler script:

   ```bash
   ./compiler.sh
   ```

After successful compilation, two executables will be generated inside the `build/` directory:

```text
build/
├── server
└── client
```

## How to Run

The game uses a client-server architecture.

1. Start the server:

   ```bash
   ./build/server <PORT_NUMBBER>
   ```

2. Open two separate terminals and start one client in each:

   ```bash
   ./build/client <IP_ADDRESS> <PORT_NUMBER>
   ```

3. Both players connect to the server and participate in the match.

## Technologies Used

- C
- GCC
- Bash
- POSIX Sockets
- Git and GitHub

## Developers

- **B**reboneria, Mark Jerome M.
- **A**gsao, Christian Josef A.
- **B**añas, Karl Matthew S.
- **A**biog, Theo Angelo A.
