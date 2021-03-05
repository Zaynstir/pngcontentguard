#!/bin/bash

#to use this script, compile the barebones.c file to ouput as barebones
#install the PngSuite images or whatever you prefer
#change the 'd' variable to reflect the directory of the files relative to the compiled file
#then run this script in the same directory directory as the compiled file

d='../PngSuite'

echo "" > temp.txt

for entry in "$d"/*
do
    cmd=$(./barebones "$entry") 
    echo "$entry => $cmd" >> temp.txt
done