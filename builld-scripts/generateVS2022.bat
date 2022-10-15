PUSHD ..\
CALL builld-scripts\premake\premake5.exe --file=sputnik-solution.lua vs2022
POPD

PAUSE