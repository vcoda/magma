REM Run in x64 Native Tools Command Prompt
cl /I%VK_SDK_PATH%\Include /I..\..\src /D MAGMA_SSE /EHsc unittest.cpp /link
