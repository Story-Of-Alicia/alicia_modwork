/**
* Created by rgnt.xyz
*
*/

#include <Windows.h>
#include <iphlpapi.h>
#include <tlhelp32.h>

#include <cstdio>

#include "pak/pak.hpp"

uint32_t getPIDbyName(std::string name)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot)
    {
        PROCESSENTRY32 process;
        process.dwSize = sizeof(PROCESSENTRY32);
    

        if(Process32First(hSnapshot, &process))
        {
	        do
	        {
		        if(name.find(process.szExeFile) != std::string::npos)
		        {
                    return process.th32ProcessID;
		        }
            } while (Process32Next(hSnapshot, &process));
        }

    	
        CloseHandle(hSnapshot);
    }
    else
        printf("Failed to get process snapshot\n");

    return -1;
}


std::vector<uint32_t> GetProcessPorts(uint32_t pid) {
    DWORD tableBuff = sizeof(MIB_TCPTABLE_OWNER_PID);
    DWORD result    = 0; 
    PMIB_TCPTABLE_OWNER_PID tcpTable = static_cast<PMIB_TCPTABLE_OWNER_PID>(malloc(sizeof(PMIB_TCPTABLE_OWNER_PID)));
     

    if((result = GetExtendedTcpTable(tcpTable, &tableBuff, TRUE, AF_INET, TCP_TABLE_OWNER_PID_LISTENER, 0)) != NO_ERROR) {
        if(result == ERROR_INSUFFICIENT_BUFFER)
        {
            free(tcpTable);
	        tcpTable = static_cast<PMIB_TCPTABLE_OWNER_PID>(malloc(tableBuff));

            result = GetExtendedTcpTable(tcpTable, &tableBuff, TRUE, AF_INET, TCP_TABLE_OWNER_PID_LISTENER, 0);
            if (result == ERROR_INSUFFICIENT_BUFFER || !tcpTable) {
                printf("ERROR_INSUFFICIENT_BUFFER memory.");
            	if(tcpTable)
					free(tcpTable);
            	
                return {};
            }
        }
    }

    std::vector<uint32_t> ports;
	
    for (uint32_t i = 0; i < tcpTable->dwNumEntries; i++)
    {
        auto record = tcpTable->table[i];

        if (record.dwOwningPid == pid)
        {
            ports.push_back(record.dwLocalPort);
        }
    }

    free(tcpTable);
    return ports;
}

std::string SelectPathToPak(const char* workingDir)
{

    char path[MAX_PATH]       = { 0 };
	
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.lpstrFilter = "PAK File (*.pak)\0*.pak\0";
    ofn.lpstrFile = path;
    ofn.lpstrInitialDir = workingDir;
    ofn.lpstrTitle = "Select PAK";
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_ENABLESIZING | OFN_NOCHANGEDIR;
    ofn.lpstrDefExt = "";

    if (!GetOpenFileName(&ofn)) {
        printf("Path to PAK > ");
        scanf("%s", path);
    }
    else
        printf("File selected... \n");


    return std::string(path);
}

int main(int argc, char** argv) {
    const char* workingDir = argv[0];
    printf("Work dir: %s\n", workingDir);

    std::string path = SelectPathToPak(workingDir);

    const char* msgFormat = "Game files will be extracted to current working directory: \n(%s).\n\nDo you want to continue?";
    const uint32_t bufSize = strlen(msgFormat) + strlen(workingDir) + 1;
    char* msgBuff = new char[bufSize];

    sprintf(msgBuff, msgFormat, argv[0]);

    int result = MessageBox(nullptr, msgBuff, "Working directory", MB_ICONEXCLAMATION | MB_YESNO | MB_TOPMOST);
    if (result == IDNO)
    {
        printf("Cancelled by user.\n");
        return -2;
    }
    delete[] msgBuff;
	
	PakFile pak(path.c_str());
    pak.Explore();

    printf("\nType anything to exit.\n");
    scanf("%c");
    return 0;
}


