@echo off
if not exist build mkdir build
pushd build
cl /Zi /nologo ../source/color_mapper.c
popd