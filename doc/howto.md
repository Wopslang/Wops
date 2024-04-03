<!--
   doc/howto.md
   Official Wopslang Install Guideline

   2023, Wops Team

-->

# :rocket: How to Launch Wopslang

## Introduction

It is a document containing how to install Wops. If you find an unexpected error during installation, check the installation order again and ask for help if the error continues to appear.

## Linux

1. Open a Terminal.
2. Install 'gcc', 'g++' and 'make', using the `apt` command:

   ```bash
   sudo apt install gcc
   sudo apt install g++
   sudo apt install make
   ```

3. Clone our project repository

   ```bash
   git clone https://github.com/Wopslang/Wops.git
   ```

4. Use this command in Terminal to build and make executable interpreter:

   ```bash
   bash build.sh
   ```

![image](https://user-images.githubusercontent.com/74172008/148312799-cf351f1b-3a83-4af0-a61d-e0fb81693d05.png)

Done!

## Windows OS

Use Windows Installer provided in [here](installer/wopsinstaller.exe).

2024, Wops Team
