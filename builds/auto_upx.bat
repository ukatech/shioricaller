cd Release
upx --ultra-brute --no-align --best --compress-resources=1 shioricaller.exe
copy shioricaller.exe shioricaller_NUPX.exe
upx -d shioricaller_NUPX.exe
cd ..
