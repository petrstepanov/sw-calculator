mkdir _vsbuild
cd _vsbuild
cmake ../
msbuild swcalculator.sln /p:Configuration=Release
msbuild INSTALL.vcxproj