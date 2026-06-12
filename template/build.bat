@echo off
set MSYS2=C:\msys64\ucrt64
set PATH=%MSYS2%\bin;%PATH%

if not exist build mkdir build
cd build

cmake .. ^
  -G "MinGW Makefiles" ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DCMAKE_CXX_COMPILER="%MSYS2%\bin\g++.exe" ^
  -DCMAKE_PREFIX_PATH="%MSYS2%"

cmake --build . --config Release
cd ..

copy /Y "%MSYS2%\bin\glfw3.dll"        . 2>nul
copy /Y "%MSYS2%\bin\libstdc++-6.dll"  . 2>nul
copy /Y "%MSYS2%\bin\libgcc_s_seh-1.dll" . 2>nul
copy /Y "%MSYS2%\bin\libwinpthread-1.dll" . 2>nul

echo.
echo Fertig!
