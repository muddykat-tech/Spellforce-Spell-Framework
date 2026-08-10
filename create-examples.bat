@echo off
cd .\examples\

echo "|=== Running Make on Example Mods ===|"

for /D %%d in (*) do (
    cd %%d
    echo "|--- Making %%d"
    mingw32-make clean --quiet
    mingw32-make --quiet
    
    REM Check if the "bin" folder exists in the current subfolder
    if exist "bin" (
        REM Create the destination directory in the root if it doesn't exist
        if not exist "..\..\examples-bin" (
            mkdir "..\..\examples-bin"
        )
        REM Copy the contents of the "bin" folder to the destination directory and overwrite existing files
        xcopy "bin\*" "..\..\examples-bin\" /E /Y >nul
    )
    
    cd ..
)

echo "|=== Finished Making Example Mods ===|"