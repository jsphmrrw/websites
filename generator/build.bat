@echo off
if not exist build mkdir build
pushd build
cl /Zi /nologo ../source/generator.c
popd