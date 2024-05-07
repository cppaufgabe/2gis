комиляция выполнялась с помощью:
gcc version 11.4.0 (Ubuntu 11.4.0-1ubuntu1~22.04) 

сборка выполнялась с помощью:
cmake version 3.22.1 

операционная система:
Ubuntu 22.04.3 LTS

Программа собирается и компилируется с помощью CMake.

mkdir build
cd build
cmake ..
cmake --build .

получаем файл test

инструкция по использованию опций командной строки
./test -h 


for testing please run unittests, see also gtest/tests.cpp
