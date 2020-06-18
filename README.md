# APIunhooker
C++ function that will automagically unhook a specified Windows API

Simply add the functions to whatever you need it for, and call the function with <b>unhookAPI([DLLname],[APIname]);</b>

<b>ex. unhookAPI(L"ntdll.dll", "NtReadVirtualMemory"); </b>
  
Use it as many times as needed in your code to unhook all the APIs you want/need to for whatever reason.

There are some debugging <b>printf</b> you might want to remove for steal reasons, but I left them.

This tool is based off this article https://ired.team/offensive-security/defense-evasion/bypassing-cylance-and-other-avs-edrs-by-unhooking-windows-apis by [@spotheplanet](https://twitter.com/spotheplanet)
