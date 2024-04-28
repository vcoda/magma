#pragma once
#include <array>
#include <map>
#include <functional>
#include <string>
#include <sstream>
#include <fstream>

#define VK_SDK_PATH "VK_SDK_PATH"
#define VULKAN_SDK "VULKAN_SDK"
#define VULKAN_CORE_HEADER "\\Include\\vulkan\\vulkan_core.h"

// https://registry.khronos.org/vulkan/
const std::string vendors[] = {
    "AMD", "ANDROID", "ARM", "EXT",
    "FUCHSIA", "GGP", "GOOGLE", "HUAWEI",
    "IMG", "INTEL", "KHR", "MVK", "NN",
    "NV", "NVX", "QCOM", "QNX",
    "SEC", "VALVE"
};

const std::string platforms[] = {
    "ANDROID", "FUCHSIA", "GGP",
    "MVK", "NN", "QNX"
};

bool isExtensionSuffix(const std::string& token)
{
    for (const auto& ext: vendors)
    {
        if (token == ext)
            return true;
    }
    return false;
}

bool isPlatformSuffix(const std::string& token)
{
    for (const auto& ext: platforms)
    {
        if (token == ext)
            return true;
    }
    for (auto platform: {
        "D3D12",
        "DIRECTFB",
        "LOADER", // Reserved for internal use by the loader
        "METAL",
        "XLIB",
        "XCB",
        "WAYLAND",
        "WIN32"})
    {
        if (token == platform)
            return true;
    }
    return false;
}

std::string::size_type findExtensionPrefix(const std::string& line, std::string& prefix)
{
    for (const auto& ext: vendors)
    {
        prefix = "VK_" + ext + "_";
        auto pos = line.find(prefix);
        if (pos != std::string::npos)
            return pos;
    }
    return std::string::npos;
}

std::string::size_type findExtensionSuffix(const std::string& line)
{
    for (const auto& ext: vendors)
    {
        auto pos = line.find(ext);
        if (pos != std::string::npos)
            return pos;
    }
    return std::string::npos;
}

std::string parseExtensionDefine(const std::string& line)
{
    std::string name;
    auto pos = findExtensionPrefix(line, name);
    if (pos != std::string::npos)
    {
        for (auto it = line.begin() + pos + name.length();
            it != line.end(); ++it)
        {
            char c = *it;
            if (isupper(c))
                return std::string();
            if (isspace(c))
                break;
            name.append(1, c);
        }
        return name;
    }
    return std::string();
}

std::string parseStructureName(const std::string& family, const std::string& line)
{
    auto pos = line.find(family);
    auto nextPos = line.find(family, pos + 1);
    if ((nextPos > pos) && (nextPos < std::string::npos))
        pos = nextPos; // Go to extension typedef
    std::string name;
    for (auto it = line.begin() + pos;
        it != line.end(); ++it)
    {
        char c = *it;
        if (isspace(c) || c == '{' || c == ';')
            break;
        name.append(1, c);
    }
    return name;
}

std::string extensionVersionStr(int version, const std::string& vendor)
{
    if (1 == version)
        return vendor;
    return std::to_string(version) + vendor;
}

bool isFeaturesStructure(const std::string& line)
{
    if (line.find("VkPhysicalDevice") == std::string::npos)
        return false;
    for (const auto& vendor: vendors)
    {   // Handle all extension versions
        for (int ver = 1; ver < 10; ++ver)
        {
            auto pos = line.find("Features" + extensionVersionStr(ver, vendor));
            if (pos != std::string::npos)
                return true;
        }
    }
    return false;
}

bool isPropertiesStructure(const std::string& line)
{
    if (line.find("VkPhysicalDevice") == std::string::npos)
        return false;
    for (const auto& vendor: vendors)
    {   // Handle all extension versions
        for (int ver = 1; ver < 10; ++ver)
        {
            auto pos = line.find("Properties" + extensionVersionStr(ver, vendor));
            if (pos != std::string::npos)
                return true;
        }
    }
    return false;
}

bool isCreateInfoStructure(const std::string& line) noexcept
{
    if (line.find_first_of("()*,") != std::string::npos)
        return false;
    if ((line.find("typedef Vk") == std::string::npos) &&
        (line.find("typedef struct Vk") == std::string::npos))
        return false;
    for (const auto& ext: vendors)
    {   // Handle all extension versions
        for (int ver = 1; ver < 10; ++ver)
        {
            auto pos = line.find("CreateInfo" + extensionVersionStr(ver, ext));
            if (pos != std::string::npos)
                return true;
        }
    }
    return false;
}

bool isMemoryAllocateStructure(const std::string& line) noexcept
{
    if (line.find_first_of("()*,") != std::string::npos)
        return false;
    if ((line.find("typedef Vk") == std::string::npos) &&
        (line.find("typedef struct Vk") == std::string::npos))
        return false;
    if (line.find("MemoryDedicated") != std::string::npos)
        return true;
    for (const auto& ext: vendors)
    {
        if ((line.find("AllocateInfo" + ext) != std::string::npos) ||
            (line.find("AllocateFlagsInfo" + ext) != std::string::npos))
            return true;
    }
    return false;
}

std::string fixupStructureEnumName(const std::string& name)
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
        {"VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CORE_PROPERTIES_2AMD",
         "VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_CORE_PROPERTIES_2_AMD"}
    };
    // Vulkan naming convention doesn't follow strict rules about
    // underscores with digits and also has another issues,
    // so use this name mapping to fix conversion errors.
    auto it = mapping.find(name);
    if (it == mapping.end())
        return name;
    return it->second;
}

std::string fixupStructureTypeName(const std::string& name)
{
    const std::map<std::string, std::string> mapping = {
        {"VkPhysicalDeviceIdProperties",
         "VkPhysicalDeviceIDProperties"},
        {"VkPhysicalDeviceTextureCompressionASTCHdrFeatures",
         "VkPhysicalDeviceTextureCompressionASTCHDRFeatures"},
        {"VkPhysicalDeviceTextureCompressionASTCHdrFeaturesEXT",
         "VkPhysicalDeviceTextureCompressionASTCHDRFeaturesEXT"},
        {"VkTextureLodGatherFormatPropertiesAMD",
         "VkTextureLODGatherFormatPropertiesAMD"},
        {"VkDebugReportCreateInfoEXT",
         "VkDebugReportCallbackCreateInfoEXT"},
        {"VkPhysicalDeviceIdPropertiesKHR",
         "VkPhysicalDeviceIDPropertiesKHR"}
    };
    // Vulkan naming convention doesn't follow strict rules about
    // underscores with digits and also has another issues,
    // so use this name mapping to fix conversion errors.
    auto it = mapping.find(name);
    if (it == mapping.end())
        return name;
    return it->second;
}

std::string convertStructureNameToStructureType(const std::string& structureName)
{
    constexpr size_t firstChar = 2; // Skip "Vk"
    bool prevLower = true;
    bool prevDigit = false;
    std::string name = "VK_STRUCTURE_TYPE_";
    for (size_t i = firstChar, len = structureName.length(); i < len; ++i)
    {
        char c = structureName.at(i);
        if (isupper(c) && i > firstChar)
        {
            if (prevLower)
                name.append(1, '_');
            else // Sequence of upper case sybmols
            {
                if (i + 1 < len)
                {   // Check if the next character is lower case
                    char c2 = structureName.at(i + 1);
                    if (islower(c2))
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
        if (prevDigit && i < len)
            name.append(1, '_');
    }
    return fixupStructureEnumName(name);
}

std::string pascalCaseName(const std::string& name)
{
    std::string pascalName;
    bool prevDigit = false;
    for (size_t i = 0, size = name.length(); i < size; ++i)
    {
        char c = name.at(i);
        if (i == 0 || prevDigit)
            pascalName.append(1, toupper(c));
        else
            pascalName.append(1, tolower(c));
        prevDigit = isdigit(c);
    }
    return pascalName;
}

bool isSpecialToken(const std::string& token)
{
    const std::string tokens[] = {
        "2D", "3D", "ASTC", "AABB",
        "D3D12", "IOS",
        "RGBA10X6", "RDMA",
        "PCI", "SM"
    };
    for (const auto& tok: tokens)
    {
        if (token == tok)
            return true;
    }
    return false;
}

std::string convertStructureEnumToStructureType(std::string structureEnum)
{
    structureEnum.erase(0, strlen("VK_STRUCTURE_TYPE_"));
    std::stringstream ss(structureEnum);
    std::vector<std::string> tokens;
    std::string token;
    while (std::getline(ss, token, '_'))
        tokens.push_back(token);
    std::string structureType = "Vk";
    for (const auto& token: tokens)
    {
        if (isPlatformSuffix(token))
            return std::string(); // Skip platform-dependent structures for now
        if (isExtensionSuffix(token) || isSpecialToken(token))
            structureType += token;
        else
            structureType += pascalCaseName(token);
    }
    return fixupStructureTypeName(structureType);
}

void writeGeneratedByUtilityToolWarning(std::ofstream& fs)
{
    fs << "// Generated by utility tool. DO NOT MODIFY!" << std::endl;
}
