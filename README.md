# Corewar

> Virtual machine for the Corewar tournament - EPITECH B-CPE-200 Project

---

## Table of contents

1. [Overview](#overview)
2. [Compilation](#compilation)
3. [Usage](#usage)
   - [Flags](#flags)
4. [The Virtual Machine](#the-virtual-machine)
   - [Memory layout](#memory-layout)
   - [Processes](#processes)
   - [Cycle management](#cycle-management)
   - [Output](#output)
5. [Instruction set](#instruction-set)
6. [Champions](#champions)
7. [Project architecture](#project-architecture)
8. [Bonus - Graphical visualizer](#bonus---graphical-visualizer)

---

## Overview

**Corewar** is a virtual machine simulating a battle arena where programs called **Champions** fight to be the last one alive. Each champion is a binary compiled from an assembly language specific to the virtual machine.

The machine loads each champion into a shared circular memory (`MEM_SIZE` bytes) and executes their processes concurrently. A champion wins by being the last one to call the `live` instruction before the cycle counter runs out.

Subject is in [B-CPE-200_Corewar.pdf](B-CPE-200_Corewar.pdf).

---

## Compilation

```sh
# Compile the main virtual machine
make

# Compile the bonus graphical visualizer
make bonus

# Install Python dependencies for the bonus
make install

# Clean object files
make clean

# Remove everything (objects + binaries + libraries)
make fclean

# Recompile from scratch
make re

# Run unit tests (Criterion)
make unit_tests

# Coverage report
make gcovr

# Run functional tests
make func_tests

# Run both test suites
make tests

# Check coding style
make coding_style
```

---

## Usage

```sh
./corewar [-dump nbr_cycle] [[-n prog_number] [-a load_address] prog_name] ...
```

**Example — 2 champions fight:**

```sh
./corewar champions/abel.cor champions/jin.cor
```

**Example — dump memory after cycle 100:**

```sh
./corewar -dump 100 champions/abel.cor champions/jin.cor
```

### Flags

| Flag | Description |
| --- | --- |
| `-dump nbr_cycle` | Dumps the memory after `nbr_cycle` cycles (32 bytes/line, hexadecimal). The round is not interrupted if it is not over yet. |
| `-n prog_number` | Sets the player number for the next champion. By default, the first free number in parameter order. |
| `-a load_address` | Sets the loading address of the next champion. Without this flag, addresses are optimized so that champions are as far apart as possible (`MEM_SIZE` modulo). |

> Error messages are written to stderr and the program exits with code **84** on failure (code **0** on success).

---

## The Virtual Machine

### Memory layout

The VM provides a circular memory of `MEM_SIZE` bytes shared by all champions. At startup, each champion's binary is loaded at its assigned address.

### Processes

Each program spawns at least one **process**. A process has:

- `REG_NUMBER` registers of `REG_SIZE` bytes each (defined in `op.h`)
- A **PC** (Program Counter) — the address of the next instruction to decode and execute
- A **carry flag** — set to `1` if and only if the last relevant operation returned zero

### Cycle management

The machine simulates a parallel execution model:

- Each instruction executes entirely **at the end** of its last cycle and waits for its full duration.
- Instructions starting on the same cycle execute in ascending **player number** order.
- Every `CYCLE_TO_DIE` cycles, the machine checks how many `live` calls were made.
  - If fewer than `NBR_LIVE` calls were made, `CYCLE_TO_DIE` is decreased by `CYCLE_DELTA`.
  - This repeats until no processes remain alive.

The last champion to have called `live` **wins**.

### Output

With each `live` instruction executed:
```
The player NB_OF_PLAYER(NAME_OF_PLAYER) is alive.
```

When a champion wins:
```
The player NB_OF_PLAYER(NAME_OF_PLAYER)has won.
```

---

## Instruction set

| Opcode | Mnemonic | Parameters | Description |
| --- | --- | --- | --- |
| `0x01` | `live` | `4-byte player number` | Signals that the player is alive. |
| `0x02` | `ld` | `value, register` | Loads the first parameter into a register. **Modifies carry.** |
| `0x03` | `st` | `register, register/address` | Stores a register's value into a register or memory address. |
| `0x04` | `add` | `reg, reg, reg` | Adds two registers and stores the result in the third. **Modifies carry.** |
| `0x05` | `sub` | `reg, reg, reg` | Subtracts two registers, stores result in third. **Modifies carry.** |
| `0x06` | `and` | `value, value, reg` | Bitwise AND on first two parameters, stores in third. **Modifies carry.** |
| `0x07` | `or` | `value, value, reg` | Bitwise OR. **Modifies carry.** |
| `0x08` | `xor` | `value, value, reg` | Bitwise XOR. **Modifies carry.** |
| `0x09` | `zjmp` | `index` | Jumps to the index if carry equals 1; consumes cycles otherwise. |
| `0x0a` | `ldi` | `value, value, reg` | Indirect indexed load. Reads `IND_SIZE` bytes at `PC + (p1 + p2) % IDX_MOD`. **Modifies carry.** |
| `0x0b` | `sti` | `reg, value, value` | Indirect indexed store: copies a register to `PC + (p2 + p3) % IDX_MOD`. |
| `0x0c` | `fork` | `index` | Creates a new process at `PC + index % IDX_MOD` inheriting the parent's state. |
| `0x0d` | `lld` | `value, register` | Like `ld` without `% IDX_MOD`. **Modifies carry.** |
| `0x0e` | `lldi` | `value, value, reg` | Like `ldi` without `% IDX_MOD`. **Modifies carry.** |
| `0x0f` | `lfork` | `index` | Like `fork` without `% IDX_MOD`. |
| `0x10` | `aff` | `register` | Displays the ASCII character whose code is `register % 256` on stdout. |

---

## Champions

Pre-compiled champions and their `.s` source files are available in the [champions/](champions/) directory.

| Champion | Description |
| --- | --- |
| `abel.cor` | Simple warrior |
| `jin.cor` | Aggressive attacker |
| `42.cor` | Minimal live-caller |
| `fenix.cor` | Resurrection strategy |
| `bill.cor` | Defensive champion |
| `king_of_the_hill.cor` | Arena domination |
| `mathematician.cor` | Arithmetic-based fighter |
| `world_trotter.cor` | Memory-traversal strategy |
| `pdd.cor` / `tyron.cor` | Various strategies |

Additional test champions covering individual instructions (`add`, `aff`, `and`, `fork`, `ld`, `ldi`, `lfork`, `live`, `lld`, `lldi`, `or`, `st`, `sti`, `sub`, `xor`, `zjmp`) are also included.

---

## Project architecture

```
.
├── include/
│   ├── corewar.h           # Main structures (t_vm, t_champion, t_process...)
│   ├── op.h                # Constants (MEM_SIZE, REG_SIZE, CYCLE_TO_DIE, etc.)
│   └── my.h                # Utility library header
├── src/
│   ├── check_lives.c       # Live-count checking and CYCLE_TO_DIE management
│   ├── create_champion.c   # Champion allocation and initialization
│   ├── creates.c           # Process creation (fork handling)
│   ├── decode_args.c       # Argument decoding (no coding byte)
│   ├── decode_args_with_coding_byte.c  # Argument decoding (with coding byte)
│   ├── dump_memory.c       # -dump flag: hexadecimal memory output
│   ├── init_and_free.c     # VM init and memory cleanup
│   ├── instructions.c      # Instruction dispatch table
│   ├── load_champion.c     # Binary loading into arena memory
│   ├── op.c                # op_tab definition
│   ├── read.c              # Binary file reading
│   ├── run_vm.c            # Main execution loop
│   ├── instructions/       # One file per opcode (op_live, op_ld, op_fork, ...)
│   ├── utilities/          # Error, endian swap, address loader, helpers
│   └── parser/             # CLI flags and arguments parsing
├── lib/my/                 # Custom C standard library
├── bonus/                  # Graphical visualizer (PyQt5)
│   ├── src/                # VM variant with JSON output for visualization
│   ├── include/            # Bonus-specific headers
│   ├── visual.py           # Arena renderer
│   └── welcome.py          # GUI launcher
├── champions/              # Champion binaries (.cor) and sources (.s)
├── binaries/               # Pre-built assembler and VM binaries
├── tests/                  # Unit tests (Criterion)
├── main.c
└── Makefile
```

---

## Bonus - Graphical visualizer

The bonus adds a **PyQt5 graphical interface** to watch the fight in real time.

### Dependencies

```sh
make install   # installs PyQt5 via pip
```

### Launch

```sh
make bonus
```

This compiles a special VM variant (`bonus/corewar`) that outputs a JSON stream, then launches the graphical front-end (`welcome.py`) which lets you:

- Select champion `.cor` files through a file picker
- Watch the arena memory update cycle by cycle
- Observe process positions and memory ownership evolve in real time

### Keyboard shortcuts

| Key | Action |
| --- | --- |
| `F` | Toggle fullscreen |
