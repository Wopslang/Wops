<!--
   doc/howto.md
   Official Wopslang Install Guideline

   2023, Wops Team

-->

# :rocket: How to Launch Wopslang

## Introduction

It is a document containing how to install Wops. If you find an unexpected error during installation, check the installation order again and ask for help if the error continues to appear.

## Ubuntu OS

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

1. Install Git Bash

   - English: https://git-scm.com/downloads

   - Korean: https://xangmin.tistory.com/102

2. Install gcc

   - English: https://dev.to/gamegods3/how-to-install-gcc-in-windows-10-the-easier-way-422j
   - Korean: https://jeunna.tistory.com/entry/Windows-gcc-g-%EC%84%A4%EC%B9%98%ED%95%98%EA%B8%B0-MinGW

   - if "the file has been downloaded incorrectly" error occurred, check [this website](https://suji-choi.tistory.com/34)

3. Install make

   - English: https://www.stanleyulili.com/git/how-to-install-git-bash-on-windows/

   - Korean: https://ndb796.tistory.com/381

4. Clone our project repository

   ```bash
   git clone https://github.com/Wopslang/Wops.git
   ```

5. Use this command in Git Bash to build and make executable interpreter:

   ```bash
   bash build.sh
   ```

![image](https://user-images.githubusercontent.com/74172008/148321362-b722b387-5fb0-4795-8aab-8dfb984fcf90.png)

Done!

## To run

The library file(`library.so`) should be placed in the path `${ExecutableFilePath}/lib/library.so`.

```bash
./Wopslang <file directory>.wops
```

2023, Wops Team
