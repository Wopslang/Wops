#!/bin/bash
echo -e "\033[92m██╗    ██╗ ██████╗ ██████╗ ███████╗██╗      █████╗ ███╗   ██╗ ██████╗ \033[m"
echo -e "\033[92m██║    ██║██╔═══██╗██╔══██╗██╔════╝██║     ██╔══██╗████╗  ██║██╔════╝ \033[m"
echo -e "\033[92m██║ █╗ ██║██║   ██║██████╔╝███████╗██║     ███████║██╔██╗ ██║██║  ███╗\033[m"
echo -e "\033[92m██║███╗██║██║   ██║██╔═══╝ ╚════██║██║     ██╔══██║██║╚██╗██║██║   ██║\033[m"
echo -e "\033[92m╚███╔███╔╝╚██████╔╝██║     ███████║███████╗██║  ██║██║ ╚████║╚██████╔╝\033[m"
echo -e "\033[92m ╚══╝╚══╝  ╚═════╝ ╚═╝     ╚══════╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝ ╚═════╝ \033[m"
echo -e "\n"
echo -e "✨ \033[1mWopslang v0.1.5 alpha Builder\033[m"
echo -e "\033[91mWarning: This is alpha version. Some critical issues might be appeared.\033[m"
echo -e -n "- make install..."
make clean
LOG=`make`
RESULT=`echo $?`

if [ $RESULT -ne 0 ]
then
    echo -e "\033[91mError occured while building Wopslang.\033[m"
    echo -e "\033[91m┌─────────────── How to Fix ───────────────┐\033[m"
    echo -e "\033[91m│                                          │\033[m"
    echo -e "\033[91m│ 1. Check the error message fix your      │\033[m"
    echo -e "\033[91m│    environment setting (or installation) │\033[m"
    echo -e "\033[91m│                                          │\033[m"
    echo -e "\033[91m│ 2. Ask help to the Wops Community        │\033[m"
    echo -e "\033[91m│   - github.com/Wopslang/Wops/discussions │\033[m"
    echo -e "\033[91m│                                          │\033[m"
    echo -e "\033[91m└──────────────────────────────────────────┘\033[m"
else 
    echo -e "\033[94mDone! Happy coding with Wopslang :)\033[m"
fi