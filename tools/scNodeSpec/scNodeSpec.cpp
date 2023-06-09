#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <iostream>
#include <set>
#include "../common.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: scNodeSpec <output_file>" << std::endl;
        return -1;
    }

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

    // Parse extended CreateInfo structures
    std::multimap<std::string, std::string> chainNodes;
    std::set<std::string> nodeExtensions;
    std::string lastFoundExtension;
    std::string line;
    while (std::getline(header, line))
    {
        if (line.empty())
            continue;
        std::string result = parseExtensionDefine(line);
        if (!result.empty())
        {
            lastFoundExtension = std::move(result);
            continue;
        }
        if (isCreateInfoStructure(line) || isMemoryAllocateStructure(line))
        {
            result = parseStructureName("Vk", line);
            if (!result.empty())
            {
                if (result == "VkQueryPoolCreateInfoINTEL")
                    continue; // Avoid double definition error in specialization
                if (lastFoundExtension == "VK_EXT_global_priority")
                    continue; // Skip in favor of VK_KHR_global_priority
                if (!lastFoundExtension.empty())
                {
                    std::cout << "Found " << result << std::endl;
                    chainNodes.emplace(lastFoundExtension, result);
                    nodeExtensions.insert(lastFoundExtension);
                }
            }
        }
    }

    // Generate template specialization code
    std::ofstream source(argv[1]);
    if (!source.is_open())
    {
        std::cout << "Couldn't write to file " << argv[1] << "." << std::endl;
        return -1;
    }
    writeGeneratedByUtilityToolWarning(source);
    source << "namespace magma" << std::endl << "{" << std::endl;
    for (const std::string& name: nodeExtensions)
    {
        source << "#ifdef " << name << std::endl;
        const auto range = chainNodes.equal_range(name);
        for (auto it = range.first; it != range.second; ++it)
        {
            std::string structureTypeName = convertStructureNameToStructureType(it->second);
            source << "MAGMA_SPECIALIZE_STRUCTURE_CHAIN_NODE(" << it->second << ", " << structureTypeName << ")" << std::endl;
        }
        source << "#endif" << std::endl;
    }
    source << "} // namespace magma" << std::endl;

    std::cout << "End of source generation" << std::endl;
    return 0;
}
