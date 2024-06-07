REM Run in x64 Native Tools Command Prompt
cl /I%VK_SDK_PATH%\Include /I..\..\src /I..\..\src\core /D MAGMA_SSE /EHsc unittest.cpp /link
