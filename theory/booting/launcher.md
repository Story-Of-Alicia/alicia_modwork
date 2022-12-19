# How to boot the game w/o the launcher
Use the parameters `Alicia.exe -GameID 4`, gameID here is specifying the launch info grabbed from the `libconfig.dat` configuration. 
Then open shared memory file named `i dont remember` and following buffer is expected: 
```
\n\t\t|GameId=Alicia|MemberNo=<id>|LoginID=<name>|AuthKey=<key>|InstallUrl=http://127.0.0.1:8512/Client/Alicia_mini_setup_04.exe|ServerType=0|ServerInfo=:|Age=16|Sex=2|Birthday=|WardNo=0|CityCode=00|ZipCode=|PCBangNo=0
```
