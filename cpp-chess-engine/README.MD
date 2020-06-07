# C++ Engine of the chess-project
---
## Building the project

To build this project, [CMake](https://cmake.org "CMake website") version 3.12 or later is required.
To check whether this is installed on your system, type:
``` bash
cmake --version
```
---
#### Install CMake
If you don't have CMake installed, you can install it:
###### Linux:
``` bash
sudo apt-get install cmake
```

###### macOS:
``` bash
brew install cmake
```

---

#### Upgrade CMake

If the version of your CMake build is below 3.12, you can upgrade:
###### Linux
``` bash
sudo apt-get upgrade cmake
```
###### macOS
``` bash
brew upgrade cmake
```

---
#### Compile Chess Engine

Build by running the build.sh shell script:
``` bash
./build.sh
```

If your system doesn't give the privileges to execute the shell script,
run the following command first:
``` bash
sudo chmod +x build.sh
```

The resulting binary can be found afterwards in the build directory.

---

## Ins and Outs of the Chess Engine
The engine is entirely dependent on OSC messages that tell it what to do.

#### OSC Input (port 6001)

| OSC Address   | Argument          | Description               |
|-------------  |-------------      |-------------              |
| /new_game     | 1                 | Signal start of new game  |
| /commit       | 1                 | Signal a push of the commit button |
| /pull | [square index]    | Signal that a chess piece has been lifted from the board |
| /put  | [square index]    | Signal that a chess piece has been put on the board |

---

#### OSC Output (port 5001)

| OSC Address | Argument | Description |
|---|---|---|
| /new_game | 1 | Signal start of new game |
| /pos | [num_string] | String of number where each number is a piece (see ChessEngine.h) |
| /move_building_error |1| Sent when a move construction error occurs|
| /move_failed |1| Sent when a move wasn't valid in current pos|
| /colour | [is_white] | Sends 1 if active colour is white, else 0 |
| /check_mate |1 | Sent when someone is check mate|
| /move_result| [ 0 \| 1 \| 2 ] | Sends whether move was bad (0), neutral (1) or good (2)|

\**Some other messages that will be sent, can be found in [ChessEngine.h](https://github.com/w-ensink/chess-project/blob/master/cpp-chess-engine/src/ChessEngine.h "ChessEngine.h")*

---