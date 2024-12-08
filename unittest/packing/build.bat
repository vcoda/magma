REM Run in x64 Native Tools Command Prompt
cl /std:c++17 /EHsc /D MAGMA_SSE /I%VK_SDK_PATH%\Include /I..\..\src unittest.cpp /link
