#!/bin/bash
if [ $1 = "-h" ];then
cd ../tests/ifj-code/
ls
else
./parser < ../tests/ifj-code/$1 > code.ifj
./ic17int code.ifj
fi
