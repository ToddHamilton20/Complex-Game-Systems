del /s/q ..\Build\*.*
xcopy /s/e/y ..\bin\*.exe ..\Build\Bin\
xcopy /s/e/y ..\Resources\Textures\*.png ..\Build\Resources\Textures\
xcopy /s/e/y ..\Resources\Shaders\*.vert ..\Build\Resources\Shaders\
xcopy /s/e/y ..\Resources\Shaders\*.frag ..\Build\Resources\Shaders\
del /q ..\Build\Bin\*_d.exe