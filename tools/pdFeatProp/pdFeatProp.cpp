#define _CRT_SECURE_NO_WARNINGS
#include <map>
#include <iostream>
#include <cstdlib>

#include "../common.h"

std::string buildGetterMethodName(const std::string& name)
{
    std::size_t prefixLen = strlen("VkPhysicalDevice");
    std::size_t suffixPos;
    for (const auto& ext: vendors)
    {
        suffixPos = name.find(ext);
        if (suffixPos != std::string::npos)
            break;
    }
    std::string getterName = name.substr(prefixLen, suffixPos - prefixLen);
    return "get" + getterName;
}

std::string buildVariableName(const std::string& name)
{
    std::size_t prefixLen = strlen("VkPhysicalDevice");
    std::size_t suffixPos;
    for (const auto& ext: vendors)
    {
        suffixPos = name.find(ext);
        if (suffixPos != std::string::npos)
            break;
    }
    std::string varName = name.substr(prefixLen, suffixPos - prefixLen);
    char c = varName.at(0);
    if (isdigit(c))
        varName = "_" + varName;
    else
        varName.at(0) = tolower(c);
    return varName;
}

void writeGetterMethodDecl(std::ofstream& fs, const std::string& name)
{
    std::string getterMethodName = buildGetterMethodName(name);
    fs << "        " << name << " " << getterMethodName << "() const;" << std::endl;
}

void writeGetterMethodImpl(std::ofstream& fs, const std::string& name, bool features)
{
    const std::string methodName = buildGetterMethodName(name);
    const std::string structureTypeName = convertStructureNameToStructureType(name);
    const std::string variableName = buildVariableName(name);
    writeGeneratedByUtilityToolWarning(fs);
    fs << name << " PhysicalDevice::" << methodName << "() const" << std::endl;
    fs << "{" << std::endl;
    fs << "    " << name << " " << variableName << " = {};" << std::endl;
    fs << "    " << variableName << ".sType = " << structureTypeName << ";" << std::endl;
    if (features)
        fs << "    getFeatures2(&";
    else
        fs << "    getProperties2(&";
    fs << variableName << ");" << std::endl;
    fs << "    return " << variableName << ";" << std::endl;
    fs << "}" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Usage: pdFeatProp <output_decl_file> <output_impl_file>" << std::endl;
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

    // Parse extended features and properties of PhysicalDevice
    struct PhysicalDeviceFeaturePropertyNames
    {
        std::string featuresName;
        std::string propertiesName;
    };
    std::map<std::string, PhysicalDeviceFeaturePropertyNames> extensions;
    std::string lastFoundExtension;
    std::string line;
    while (std::getline(header, line))
    {
        if (line.empty())
            continue;
        std::string result = parseExtensionDefine(line);
        if (!result.empty())
            lastFoundExtension = std::move(result);
        else
        {
            if (line.find("PFN") != std::string::npos)
                continue; // Ignore entrypoint definitions

            bool isFeatures = isFeaturesStructure(line);
            bool isProperties = isPropertiesStructure(line);
            if (isFeatures || isProperties)
            {
                result = parseFeaturePropertyStructureName(line);
                if (!result.empty())
                {
                    if (lastFoundExtension == "VK_EXT_global_priority_query")
                        continue; // Skip in favor of VK_KHR_global_priority
                    if (lastFoundExtension == "VK_EXT_tooling_info")
                        continue; // Has dedicated vkGetPhysicalDeviceToolPropertiesEXT() function
                    if (isFeatures)
                        extensions[lastFoundExtension].featuresName = std::move(result);
                    else // isProperties
                        extensions[lastFoundExtension].propertiesName = std::move(result);
                }
            }
        }
    }

    // Generate method declarations
    std::ofstream decl(argv[1]);
    if (!decl.is_open())
    {
        std::cout << "Couldn't write to file " << argv[1] << "." << std::endl;
        return -1;
    }
    decl << "    ";
    writeGeneratedByUtilityToolWarning(decl);
    for (const auto& it: extensions)
    {
        decl << "    #ifdef " << it.first << std::endl;
        const auto& names = it.second;
        if (!names.featuresName.empty())
            writeGetterMethodDecl(decl, names.featuresName);
        if (!names.propertiesName.empty())
            writeGetterMethodDecl(decl, names.propertiesName);
        decl << "    #endif" << std::endl;
    }

    // Generate method implementations
    std::ofstream impl(argv[2]);
    if (!impl.is_open())
    {
        std::cout << "Couldn't write to file " << argv[2] << "." << std::endl;
        return -1;
    }
    for (const auto& it: extensions)
    {
        impl << "#ifdef " << it.first << std::endl;
        const auto& names = it.second;
        if (!names.featuresName.empty())
            writeGetterMethodImpl(impl, names.featuresName, true);
        if (!names.propertiesName.empty())
        {
            if (!names.featuresName.empty())
                impl << std::endl; // Add spacing between methods
            writeGetterMethodImpl(impl, names.propertiesName, false);
        }
        impl << "#endif // " << it.first << std::endl;
        impl << std::endl;
    }

    std::cout << "Output source generated successfully" << std::endl;
    return 0;
}
