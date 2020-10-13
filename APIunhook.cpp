#include "pch.h"
#include <windows.h>
#include <ImageHlp.h>
#include <iostream>
#include <vector>

using namespace std;
#pragma comment(lib,"imagehlp")

DWORD RvaToOffset(IMAGE_NT_HEADERS * nth, DWORD RVA)
{
	int i;
	int sections;
	PIMAGE_SECTION_HEADER sectionHeader;
	sectionHeader = IMAGE_FIRST_SECTION(nth);
	sections = nth->FileHeader.NumberOfSections;

	for (i = 0; i < sections; i++)
	{
		if (sectionHeader->VirtualAddress <= RVA)
			if ((sectionHeader->VirtualAddress + sectionHeader->Misc.VirtualSize) > RVA)
			{
				RVA -= sectionHeader->VirtualAddress;
				RVA += sectionHeader->PointerToRawData;
				return RVA;
			}
		sectionHeader++;
	}
	return 0;
}

void unhookAPI(const char* functionName) {
	HMODULE lib = LoadLibrary(L"C:\\Windows\\System32\\ntdll.dll");
	BYTE assemblyBytes[5] = {};
	char assemblyString[60];
	int buffSize;

	if (lib) {
		DWORD base = (DWORD)lib;
		void* fa = GetProcAddress(lib, functionName);
		if (fa) {
			IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)lib;
			IMAGE_NT_HEADERS* nth = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);
			BYTE* read = (BYTE*)fa;
			for (int i = 0; i < 5; i++) {
				assemblyBytes[i] = read[i];
			}
		}
		else
			printf("Function not found!\n");
	}
	else
		printf("Error loading library!\n");
	WriteProcessMemory(GetCurrentProcess(), GetProcAddress(GetModuleHandle(L"ntdll"), functionName), (LPCVOID)assemblyBytes, 5, NULL);
	FreeLibrary(lib);
	}

int main() {

	
	unhookAPI("NtReadVirtualMemory");

	return 0;
}
