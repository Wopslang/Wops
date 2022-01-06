# How to install Wopslang



## Introduction

It is a document containing how to install Wops. If you find an unexpected error during installation, check the installation order again and ask for help if the error continues to appear.





## Ubuntu OS

1. Open a Terminal.

   

2. Install 'gcc', 'g++' and 'make', using the `apt` command:

   ```tex
   sudo apt install gcc
   sudo apt install g++
   sudo apt install make
   ```

   

3. Download&Extract Zip file.

![image](https://user-images.githubusercontent.com/74172008/148310845-5c3fcd76-1de5-4810-8f96-80750ee70c1d.png)



4. Use this command in Terminal to build and make executable interpreter:

   ```go
   bash build.sh
   ```

​	![image](https://user-images.githubusercontent.com/74172008/148312799-cf351f1b-3a83-4af0-a61d-e0fb81693d05.png)

​	Done!





## Windows OS



1. Install Git Bash

   English:https://www.stanleyulili.com/git/how-to-install-git-bash-on-windows/
   Korean:https://xangmin.tistory.com/102

   

2. Install gcc

   English:

   Korean:https://jeunna.tistory.com/entry/Windows-gcc-g-%EC%84%A4%EC%B9%98%ED%95%98%EA%B8%B0-MinGW

   if "the file has been downloaded incorrectly" error occurred, refer to this:https://suji-choi.tistory.com/34

   

3. Install make

   English:

   Korean:https://ndb796.tistory.com/381

   

4. Download&Extract Zip file.

   ![image](https://user-images.githubusercontent.com/74172008/148310845-5c3fcd76-1de5-4810-8f96-80750ee70c1d.png)

   

5. Use this command in Git Bash to build and make executable interpreter:

   ```go
   bash build.sh
   ```

   ​	![image](https://user-images.githubusercontent.com/74172008/148321362-b722b387-5fb0-4795-8aab-8dfb984fcf90.png)

   Done!

   

   



## To run



```
./Wopslang <file directory>.wops
```

