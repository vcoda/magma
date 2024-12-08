REM Run in x64 Native Tools Command Prompt
cl /std:c++17 /EHsc /I%VK_SDK_PATH%\Include /I..\..\src /I..\..\src\core unittest.cpp ../../src/misc/structureChain.cpp /link
