@echo off
setlocal enabledelayedexpansion

echo Starting compile_commands.json generation and path fixing...

REM Run compiledb
echo Running compiledb -n make...
call compiledb -n make
if %ERRORLEVEL% neq 0 (
    echo Error running compiledb
    exit /b 1
)
echo Successfully generated compile_commands.json

REM Create build directory if it doesn't exist
if not exist "build" (
    echo Creating build directory...
    mkdir build
)

REM Copy the file to build folder
echo Moving compile_commands.json to build folder...
copy /Y compile_commands.json build\compile_commands.json
if %ERRORLEVEL% neq 0 (
    echo Error copying file to build folder
    exit /b 1
)

echo All done! compile_commands.json is now in the build folder.
echo A copy has been kept in the root directory.

endlocal