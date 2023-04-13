#define _CRT_SECURE_NO_WARNINGS
#include <map>
#include <iostream>
#include "../common.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: ldFeatSpec <output_file>" << std::endl;
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

    // Parse extended Vulkan features
    std::map<std::string, std::string> features;
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
        if (isFeaturesStructure(line))
        {
            result = parseStructureName("VkPhysicalDevice", line);
            if (!result.empty())
            {
                if (lastFoundExtension == "VK_EXT_global_priority_query")
                    continue; // Skip in favor of VK_KHR_global_priority
                features[lastFoundExtension] = std::move(result);
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
    for (const auto& it: features)
    {
        std::string structureTypeName = convertStructureNameToStructureType(it.second);
        source << "#ifdef " << it.first << std::endl
            << "MAGMA_SPECIALIZE_PHYSICAL_DEVICE_FEATURES(" << it.second << ", " << structureTypeName << ")" << std::endl
            << "#endif" << std::endl;
    }
    source << "} // namespace magma" << std::endl;

    std::cout << "End of source generation" << std::endl;
    return 0;
}
