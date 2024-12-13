@echo off

del %1.exe

g++ %1.cpp -o %1

.\%1