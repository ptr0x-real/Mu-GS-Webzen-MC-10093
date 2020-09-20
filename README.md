# Mu-GS-Webzen-MC-10093

This repository contains at the moment a very alpha version of the supposed original webzen game server code (found here: http://forum.ragezone.com/f197/release-original-source-gs-1-a-1180532/).

As the code seems to be indeed webzen original it does have some "trashy" not original code from unknown community developers (some evidences points to Mirage Continent team).

The idea of this project is to work on this src code aiming to create a playable stable src for the game server that can communicate with lower versions of the game (ie. the code seems to be compatible with game versions ranging from 0.97d to season 4.6).

The actual version (0.1) is compilable using Microsoft Visual Studio 2019 and MSVC Toolchain v142; for now we kept the game version macros system on 0.99b and we are compiling using the "KOR" compile directive. The code will probably not compile with other settings than desribed here but it is just a matter of matching the differences between "KOR" and another given configuration. When changing the game version macro ("GAME_VERSION") to any other value other than "G_V_99B" some errors in the code may arise.

Feel free to submit any pull request and open an issue.
