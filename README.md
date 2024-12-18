# JBomb Steam Launcher

This is the official launcher for the Steam version of **JBomb**, designed to handle essential tasks like locating required Java dependencies, configuring the environment, and launching the game.

This launcher is implemented in the build of the game available on Steam. The game is available at the following **[link](https://store.steampowered.com/app/3368640/JBomb/)**.

---

## Overview

The launcher is implemented in C++ and performs the following tasks:

1. **Environment Configuration**:  
   It retrieves system-specific environment variables, such as `LOCALAPPDATA`, to find the game's directory.

2. **Dependency Management**:  
   Ensures that the required Java Runtime Environment (JRE) version is properly set up.

3. **Game Launching**:  
   Constructs and executes the command to run the game via the provided `JBomb.jar` file.

4. **Error Handling**:  
   Captures and logs errors during the launch process to aid in troubleshooting.
