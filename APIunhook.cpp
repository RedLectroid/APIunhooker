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

void unhookAPI(wstring temp, const char *functionName) {
	
	LPCWSTR dllName;
	dllName = temp.c_str();

	HMODULE lib = LoadLibrary(dllName);

	BYTE assemblyBytes[4] = {};
	char assemblyString[60];

	if (lib) {
		DWORD base = (DWORD)lib;
		void *fa = GetProcAddress(lib, functionName);
		if (fa) {
			IMAGE_DOS_HEADER * dos = (IMAGE_DOS_HEADER *)lib;
			IMAGE_NT_HEADERS * nth = (IMAGE_NT_HEADERS *)(base + dos->e_lfanew);
			BYTE *read = (BYTE *)fa;
			for (int i = 0; i < 5; i++) {
				assemblyBytes[i] = read[i];
			}
			
		}
		else
			printf("Function not found!\n");
	}
	else
		printf("Error loading library!\n");
	
	sprintf_s(assemblyString, "\\x%02x\\x%02x\\x%02x\\x%02x\\x%02x", assemblyBytes[0], assemblyBytes[1], assemblyBytes[2], assemblyBytes[3], assemblyBytes[4]);
	printf("First 5 bytes of %s are %s\n", functionName, assemblyString );

	WriteProcessMemory(GetCurrentProcess(), GetProcAddress(GetModuleHandle(dllName), functionName), assemblyString, 5, NULL);

	FreeLibrary(lib);


}

int main() {

	
	unhookAPI(L"C:\\Windows\\System32\\ntdll.dll", "NtReadVirtualMemory");

	return 0;
}
