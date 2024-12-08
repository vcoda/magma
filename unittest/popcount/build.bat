REM Run in x64 Native Tools Command Prompt
cl /std:c++17 /I%VK_SDK_PATH%\Include /I..\..\src /EHsc unittest.cpp /link
