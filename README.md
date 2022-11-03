# PlusCLI
Examples and unit tests for Arduino CLI and DSP software.

This pulls in my other Libraries and shows how to use the CLI in an Ardunio project.
It also gives some examples of how one might exercise the Fixed Point DSP code.

## Requirements
- [CONduinIO](https://github.com/tedkotz/CONduinIO) - A library that attempts to deliver the DOS `conio.h` functionality over the Arduino serial port.
- [simpleCLI](https://github.com/tedkotz/simpleCLI) - The CLI infrastructure
- [fpDSP](https://github.com/tedkotz/fpDSP) - A fixed point DSP libary optimized for the 16-bit operation 

### How-to get requirements
There are two ways to get the required libraires:
- Use git to download dependencies as submodules
- Or, install dependencies as independent libraries in global arduino `libraries` location

#### Option 1: Download Dependencies as Git Submodules
This pulls everything down in one go, but doesn't integrate it with your arduino environment.
If you just want to pull it down and load it this is a good approach.
1. Clone repo and submodules
   ```
   git clone --recurse-submodules https://github.com/tedkotz/PlusCLI.git
   ```
2. Open the `.ino` file in the Arduino IDE
3. Update `Sketchbook Location`
    - Click through the menu `File >> Preferences >> Settings >> Sketchbook Location`
    - You should make note the current setting so that you can change it back later.
    - Change `Sketchbook Location` to the same directory the `.ino` file is in.
4. Build and upload as normal. See [Use] below.


#### Option 2: Install Dependencies as Global Arduino Libraries
This is the better approach if you want to have the libraries available for other sketches 
or work with the library code independently.
1. Clone this code as normal without the submodules
2. Go to the `libraries` directory in your normal `Sketchbook Location`.
3. Clone each of the dependencies as normal under the `libraries` subdirectory
4. Open the `.ino` file in the Arduino IDE
5. Build and upload as normal. See [Use] below.


## Inspirations
- Mine Sweeper

## Use
1. Build and upload with arduino
2. Connect to the Arduino serial port with an ANSI compatible terminal emulator (i.e. PUTTY)
3. Type `help` to dget the list of commands.
    - `game` launches minesweeper
    - `adc` plays a tone on pin 8 while calculating an FFT on samples from PIN A0
    - `blink` flashes the built-in LED

## Features/TODO
- [x] Split out CLI callback framework
- [x] Split out conio.h wrappers
- [x] Split out DSP library
- [ ] Split out mine sweeper into standalone FreeDOS application

### Test Platforms
- Arduino UNO
- Arduino MEGA (1280 variant)
- Seeed XIAO RP2040
    - Doesn't have audio HW setup for DSP sampling, so it might have issues
