#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
//#define OBFUSCATE
#ifdef OBFUSCATE
#include <vulkan/vulkan.h>
#include "sizeOf.h"
#endif
#include <iostream>
#include <vector>
#include "../common.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: structSize <output_file>" << std::endl;
        return -1;
    }

    std::ofstream source(argv[1]);
    if (!source.is_open())
    {
        std::cout << "Couldn't write to file " << argv[1] << "." << std::endl;
        return -1;
    }
    writeGeneratedByUtilityToolWarning(source);

#ifdef OBFUSCATE
    source << "#include \"pch.h\"" << std::endl;
    source << "#pragma hdrstop" << std::endl;
    source << "#include \"structureChain.h\"" << std::endl << std::endl;
    source << "namespace magma" << std::endl << "{" << std::endl;
    source << "size_t StructureChain::sizeOf(VkStructureType sType) noexcept" << std::endl << "{" << std::endl;
    source << "    switch (sType)" << std::endl;
    source << "    {" << std::endl;
    for (uint32_t i = VK_STRUCTURE_TYPE_APPLICATION_INFO; i <= VK_STRUCTURE_TYPE_AMIGO_PROFILING_SUBMIT_INFO_SEC; ++i)
    {
        if (i == 100) // Jump to extended enums
            i = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        // Use earlier generated values
        const size_t size = sizeOf((VkStructureType)i);
        if (size)
            source << "    case " << i << ": return " << size << ";" << std::endl;
    }
    source << "    default: MAGMA_ASSERT(false);" << std::endl;
    source << "    }" << std::endl;
    source << "    return 0;" << std::endl;
    source << "}" << std::endl;
    source << "} // namespace magma" << std::endl;
#else
    // Find Vulkan SDK header
    const char *vulkanSdkPath = getenv(VK_SDK_PATH);
    if (!vulkanSdkPath)
        vulkanSdkPath = getenv(VULKAN_SDK);
    if (!vulkanSdkPath)
    {
        std::cout << "Vulkan SDK not found" << std::endl;
        return -1;
    }
    const std::string fileName = std::string(vulkanSdkPath) + VULKAN_CORE_HEADER;
    std::ifstream header(fileName);
    if (!header.is_open())
    {
        std::cout << "Header file " << fileName << " not found" << std::endl;
        return -1;
    }

    std::vector<std::pair<std::string, std::string>> structureTypes;
    std::string line;
    bool insideEnum = false;
    bool underIfDef = false;
    while (std::getline(header, line))
    {
        if (line.empty())
            continue;
        if (line == "typedef enum VkStructureType {")
        {
            insideEnum = true;
            continue;
        }
        else if (line == "} VkStructureType;")
        {
            insideEnum = false;
            continue;
        }
        else if (insideEnum)
        {
            if (line.find("VK_STRUCTURE_TYPE_MAX_ENUM") != std::string::npos)
                continue;
            if (line.front() == '#')
            {   // Skip any beta API structures
                underIfDef = !underIfDef;
                continue;
            }
            if (!underIfDef)
            {
                const std::string prefix = "VK_STRUCTURE_TYPE_";
                const auto begin = line.find(prefix);
                const auto end = line.find("=");
                const std::string structureEnum = line.substr(begin, end - begin - 1);
                const std::string structureType = convertStructureEnumToStructureType(structureEnum);
                if (structureType.empty())
                    continue;
                if (structureType.find("FullScreen") != std::string::npos)
                    continue; // Skip as platform (Win32)
                std::cout << "Added " << structureEnum << std::endl;
                structureTypes.push_back({structureEnum, structureType});
            }
        }
    }

    source << "size_t sizeOf(VkStructureType sType)" << std::endl << "{" << std::endl;
    for (auto [structureEnum, structureType]: structureTypes)
    {   // Use "if" expression instead of "switch" to avoid collisions due to ext-to-core promotions
        source << "    if (" << structureEnum << " == sType) return sizeof(" << structureType << ");" << std::endl;
    }
    source << "    return 0;" << std::endl;
    source << "}" << std::endl;
#endif // OBFUSCATE

    std::cout << "End of source generation" << std::endl;
    return 0;
}
