PUSHD ..\

del /s /q *.sln
del /s /q *.vcxproj
del /s /q *.vcxproj.filters
del /s /q *.vcxproj.user
rmdir /s /q _output

POPD