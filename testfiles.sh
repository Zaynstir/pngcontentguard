#!/bin/bash

#to use this script, compile the barebones.c file to ouput as barebones
#install the PngSuite images or whatever you prefer
#change the 'd' variable to reflect the directory of the files relative to the compiled file
#then run this script in the same directory directory as the compiled file

d='../testsuite'
f='./OKfiles'

echo "" > temp.txt

for entry in "$d"/*
do
    cmd=$(./barebones "$entry")
    echo $cmd
    if echo "$cmd" | grep -q "File OK"; then
	cp "$entry" OKfiles/
    fi
done

for entry in "$f"/*
do
    fim $entry
done
