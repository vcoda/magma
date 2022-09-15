#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <array>
#include <string>
#include <map>
#include <fstream>
#include <iostream>

#define VK_SDK_PATH "VK_SDK_PATH"
#define VULKAN_SDK "VULKAN_SDK"
#define VULKAN_CORE_HEADER "\\Include\\vulkan\\vulkan_core.h"

const std::array<std::string, 15> vendors = {
    "AMD", "ANDROID", "ARM", "EXT", "FUCHSIA",
    "GOOGLE", "HUAWEI", "IMG", "INTEL", "KHR",
    "MVK", "NV", "NVX", "QCOM", "VALVE"
};

std::string parseExtensionDefine(const std::string& line)
{
    for (const auto& ext: vendors)
    {
        std::string name =  "VK_" + ext + "_";
        auto pos = line.find(name);
        if (pos != std::string::npos)
        {
            for (size_t i = pos + name.length(), size = line.size(); i < size; ++i)
            {
                char c = line.at(i);
                if (isupper(c))
                    return std::string();
                if (isspace(c))
                    break;
                name.append(1, c);
            }
            return name;
        }
    }
    return std::string();
}

std::string parseFeatureStructureName(const std::string& line)
{
    auto pos = line.find("VkPhysicalDevice");
    auto nextPos = line.find("VkPhysicalDevice", pos + 1);
    if ((nextPos > pos) && (nextPos < std::string::npos))
        pos = nextPos; // Go to extension typedef
    std::string name;
    for (size_t i = pos, size = line.size(); i < size; ++i)
    {
        char c = line.at(i);
        if (isspace(c) || c == '{' || c == ';')
            break;
        name.append(1, c);
    }
    return name;
}

bool isFeatureStructure(const std::string& line) noexcept
{
    if (line.find("VkPhysicalDevice") == std::string::npos)
        return false;
    for (const auto& suffix: vendors)
        if (line.find("Features" + suffix) != std::string::npos)
            return true;
    return false;
}

std::string fixupStructureTypeName(const std::string& name)
{
    const std::map<std::string, std::string> mapping = {
        {"VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16_BIT_STORAGE_FEATURES_KHR",
         "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES_KHR"},
        {"VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8_BIT_STORAGE_FEATURES_KHR",
         "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES_KHR"},
        {"VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT_16_INT_8_FEATURES_KHR",
         "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT16_INT8_FEATURES_KHR"},
        {"VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INDEX_TYPE_UINT_8_FEATURES_EXT",
         "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_INDEX_TYPE_UINT8_FEATURES_EXT"},
        {"VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RGBA_10X_6_FORMATS_FEATURES_EXT",
         "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RGBA10X6_FORMATS_FEATURES_EXT"},
        {"VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_INT_64_FEATURES_KHR",
         "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_ATOMIC_INT64_FEATURES_KHR"},
        {"VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_IMAGE_ATOMIC_INT_64_FEATURES_EXT",
         "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_IMAGE_ATOMIC_INT64_FEATURES_EXT"},
        {"VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXTURE_COMPRESSION_ASTCHDR_FEATURES_EXT",
         "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TEXTURE_COMPRESSION_ASTC_HDR_FEATURES_EXT"},
    };
    // Vulkan naming convention doesn't follow strict rules about
    // underscores with digits and also has another issues,
    // so use this name mapping to fix conversion errors.
    auto it = mapping.find(name);
    if (it == mapping.end())
        return name;
    return it->second;
}

std::string convertStructureNameToStructureType(const std::string& featureName)
{
    constexpr size_t firstChar = 2; // Skip "Vk"
    bool prevLower = true;
    bool prevDigit = false;
    std::string name = "VK_STRUCTURE_TYPE_";
    for (size_t i = firstChar, len = featureName.length(); i < len; ++i)
    {
        char c = featureName.at(i);
        if (isupper(c) && i > firstChar)
        {
            if (prevLower)
                name.append(1, '_');
            else // Sequence of upper case sybmols
            {
                if (i + 1 < len)
                {   // Check if the next character is lower case
                    char nc = featureName.at(i + 1);
                    if (islower(nc))
                        name.append(1, '_');
                }
            }
        }
        else
        {
            if (isdigit(c) && !prevDigit)
                name.append(1, '_');
        }
        name.append(1, toupper(c));
        prevLower = islower(c);
        prevDigit = isdigit(c);
    }
    return fixupStructureTypeName(name);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: featSpecGen <output_file>" << std::endl;
        return -1;
    }
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
    std::map<std::string, std::string> features;
    std::string lastFoundExtension;
    std::string line;
    while (std::getline(header, line))
    {
        if (line.empty())
            continue;
        std::string result = parseExtensionDefine(line);
        if (!result.empty())
            lastFoundExtension = std::move(result);
        else if (isFeatureStructure(line))
        {
            result = parseFeatureStructureName(line);
            if (!result.empty())
            {
                if (lastFoundExtension == "VK_EXT_global_priority_query")
                    continue; // Skip in favor of VK_KHR_global_priority
                features[lastFoundExtension] = std::move(result);
            }
        }
    }
    std::ofstream source(argv[1]);
    if (!source.is_open())
    {
        std::cout << "Couldn't write to file " << argv[1] << "." << std::endl;
        return -1;
    }
    source << "// Generated by utility tool. DO NOT MODIFY!" << std::endl;
    source << "namespace magma" << std::endl << "{" << std::endl;
    for (const auto& pair: features)
    {
        std::string structureTypeName = convertStructureNameToStructureType(pair.second);
        source << "#ifdef " << pair.first << std::endl
            << "MAGMA_SPECIALIZE_PHYSICAL_DEVICE_FEATURES(" << pair.second << ", " << structureTypeName << ")" << std::endl
            << "#endif" << std::endl;
    }
    source << "} // namespace magma" << std::endl;
    std::cout << "Output source generated successfully" << std::endl;
    return 0;
}
