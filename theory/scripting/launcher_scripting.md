# Gametree Launcher Scripting
Script functions (without parameters) for Gametree Launcher version `0.11`

Reverse engineered with Ghidra and Wireshark. 

## Functions
I haven't started on writing down the parameters of each function, so this is just name list. 

Example of a script sent to Gametree Launcher to display a message box.
![image](https://user-images.githubusercontent.com/32541639/126475564-0a77a85e-7c55-4cde-9100-eb3c8744e1be.png)


#### Namespace `LauncherScriptObject`:
```lua
  GetGameInfoManager()
  GetLauncherGUISetup()
  GetLauncherGUI()
  GetGameID()
  GetGameInstallURL()
  GetGameInstallFile()
  GetClientPath()
  GetClientPatcherName()
  GetClientArgument()
  GetRegistryRootKey()

  GetGlobalFunction()
  GetLauncherInfo()
  GetGameInfo()
  GetLauncherImpl()
  MessageBox(string)
  ShowLauncher()
  GetLauncherRootPath()
```

#### Namespace `GlobalFunction`:
```lua
  RegCreateKey_LocalMachine()
  RegGetNumber_LocalMachine()
  RegSetNumber_LocalMachine()
  RegGetText_LocalMachine()
  RegSetText_LocalMachine()
  RegDeleteValue_LocalMachine()
  RegCreateKey_CurrentUser()
  RegGetNumber_CurrentUser()
  RegSetNumber_CurrentUser()
  RegGetText_CurrentUser()
  RegSetText_CurrentUser()
  RegDeleteValue_CurrentUser()
  CreateProcess()
  CreateProcess_WaitEnd()
  ShellExecute()
  ShellExecute_WaitEnd()
  IsProcessRunning()
  HTTP_GetFileSize()
  HTTP_DownloadFile()
  MakeDir()
  RemoveDir()
  IsDirExists()
  IsFileExists()
  CopyFile()
  RenameFile()
  RemoveFile()
  GetReceiveArgumentFromParentAll()
  GetReceiveArgumentFromParent()
  GetEnvironmentString()
  ShowSelectPath_Terms_Dialog()
  GetNewInterMessageQueue()
  GetNewStartProcess()
  GetNewShellExecute()
  CreateFileShortcut()
  CreateURLShortcut()
  ShowSelectPath_Terms_Dialog()
  GetWindowsOSVersionString()
```

#### Namespace `LauncherGUISetup`:
```lua
  SetImageForGameArea()

  SetCheckBoxText_01()
  SetCheckBoxText_02()
  SetCheckBoxText_03()

  SetIEStartPage()
  SetStartMenu()

  SetShortcutURL()
  SetShortcutName()
  SetShortcutIncludeIconFileName()
  SetShortcutURL2()
  SetShortcutName2()
  SetShortcutIncludeIconFileName2()

  SetDefaultFont()
  SetDefaultFontSize()
```



## Classes

#### Class `LauncherGUI`:
```lua
  SetPosition_CurrentFileProgress()
  SetPosition_TotalFileProgress()

  SetText_FileText()
  SetText_StatusText()
  SetText_CheckBox()

  SetCheck_CheckBox()
```


#### Class `GameInfo`: 
```lua
  GetServerURL()
  SetServerURL()
  GetPakDir()
  SetPakDir()
  GetPakName()
  SetPakName()
  GetExeFileName()
  SetExeFileName()
  GetArgument()
  SetArgument()
  GetGameVersion()
  SetGameVersion()
  GetDefaultWebAddress()
  SetDefaultWebAddress()
  AddNewDistPackageInfo()
  GetLauncherProcessStatusFunctionList()
```

#### Class `LauncherInfo`: 
```lua
  SetLauncherVersion()
  GetLauncherVersion()
  SetLauncherServerURL()
  GetLauncherServerURL()
```

#### Class `GameInfoManager`:
```lua
  SetRegistryRootKey_Root()
```

#### Class `DistPackageInfo`:
```lua
  .strInstallFileURL
  .strInstallFileName
  .strInstallFileNameArguments
  .strCustomCheckFunctionName
```

#### Class `LauncherProcessStatusFunction`:
```lua
  .strFunction_LauncherSameGame
  .strFunction_LauncherNotSameGame
  .strFunction_Application
  .strFunction_Patcher
```
