# Compiling

## Requirements

- CMake 3.24+
- Python 3.9+ (accessed via `py` command)
- Python 2.7 (located at `C:/Python27/Python.exe`)

Open directory in Visual Studio 2022, configure and compile

# Directory structure

## api/

Anything exposed to JS is inside this directory

## core/

This directory has the runtime of d2bs and core features

## game/

Contains anything interacting with the game in any way, most of it is copy-paste from the old d2bs framework and is subject to change in the future.

When we say "subject to change" this means absolutely everything will change. This is not your grandpapas d2bs anymore, follow good practices and design patterns when adding stuff!

## scripting/

The scripting runtime

## utils/

Anything that doesn't fit inside any other directory, mostly small utility classes and functions that is used cross the entire framework

Must be completely stand-alone without dependencies!
