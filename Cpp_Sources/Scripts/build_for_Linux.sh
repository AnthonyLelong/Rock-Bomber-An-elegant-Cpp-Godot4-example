#!/bin/bash

cd "$(dirname "$0")" # cd to this script's folder
cd ..

clear
echo ""
echo "--- Building ---"
scons platform=linux
echo ""

if [ ! -f "./libRockBomber.linux.template_debug.x86_64.so" ];
then
    echo "Lib compilation has failed"
    echo ""
else
    echo "Lib compilation was successfull"
    echo ""
    \cp ./libRockBomber.linux.template_debug.x86_64.so ../Godot_Sources/addons/RockBomber/bin/libRockBomber.linux.template_debug.x86_64.so
fi
