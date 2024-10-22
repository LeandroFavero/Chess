# C++ Chess in Unreal Engine 4
## Features
- [UCI](https://en.wikipedia.org/wiki/Universal_Chess_Interface) chess engine compatibility
- [FEN](https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation) import/export
- Undo
- Highlight for mouseovered chess pieces with valid moves and for valid moves when a piece is lifted
- Notation
- Multiplayer using Unreal's replication system
- Skin support for the chess pieces (3 hight quality programmer art skins included :) )
- And of course all the chess rules are implemented

## How to try:
### Binary
- Download the zipped game from the [releases](https://github.com/buta/Chess/releases) tab and have fun (win64bit only, [stockfish](https://stockfishchess.org/) executable included)
### With editor
- 4.27 branch requires that version, main requires UE 5.4+
- Download the repository and open the project file 
- Start with standalone net mode to start in the main menu
- Start with listen server net mode (don't forget to set the number of players to 2) to start the match right away
- If you want to play against AI, you need to download a [UCI compatible chess engine](https://stockfishchess.org/download/) and point the "Executable path" to its .exe

## Todo:
- Chess clock?
- Steam integration?

## Assets used from:
- [RescueStudios11](https://www.blendswap.com/blend/21963)
- [Greg Zaal](https://polyhaven.com/a/wooden_lounge)
