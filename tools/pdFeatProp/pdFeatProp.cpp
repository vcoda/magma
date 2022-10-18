#define _CRT_SECURE_NO_WARNINGS
#include <map>
#include <iostream>
#include "../common.h"

std::string getMethodName(const std::string& name)
{
    const std::size_t prefixLen = strlen("VkPhysicalDevice");
    const std::size_t suffixPos = findExtensionSuffix(name);
    std::string methodName = name.substr(prefixLen, suffixPos - prefixLen);
    return "get" + methodName;
}

std::string getVariableName(const std::string& name)
{
    const std::size_t prefixLen = strlen("VkPhysicalDevice");
    const std::size_t suffixPos = findExtensionSuffix(name);
    std::string varName = name.substr(prefixLen, suffixPos - prefixLen);
    char c = varName.at(0);
    if (isdigit(c))
        varName = "_" + varName;
    else
    {   // Convert to camel case any leading upper case chars
        uint32_t i = 0;
        for (char& c: varName)
        {
            if (islower(c))
                break;
            c = tolower(c);
            ++i;
        }
        if (i > 1)
        {   // Back last char to upper case
            char& c = varName.at(--i);
            c = toupper(c);
        }
    }
    return varName;
}

void writeMethodDecl(std::ofstream& fs, const std::string& name)
{
    const std::string methodName = getMethodName(name);
    fs << "        " << name << " " << methodName << "() const;" << std::endl;
}

void writeMethodImpl(std::ofstream& fs, const std::string& name, bool features)
{
    writeGeneratedByUtilityToolWarning(fs);
    const std::string methodName = getMethodName(name);
    const std::string structureTypeName = convertStructureNameToStructureType(name);
    const std::string variableName = getVariableName(name);
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

    // Parse extended Vulkan features and properties
    struct ExtensionStructures
    {
        std::string featuresName;
        std::string propertiesName;
    };
    std::map<std::string, ExtensionStructures> extensions;
    std::string lastFoundExtension;
    std::string line;
    while (std::getline(header, line))
    {
        if (line.empty() ||
            line.find("PFN") != std::string::npos) // Ignore entrypoint definitions
            continue;
        std::string result = parseExtensionDefine(line);
        if (!result.empty())
        {
            lastFoundExtension = std::move(result);
            continue;
        }
        bool features = isFeaturesStructure(line);
        bool properties = isPropertiesStructure(line);
        if (features || properties)
        {
            result = parseStructureName(line);
            if (!result.empty())
            {
                if (lastFoundExtension == "VK_EXT_global_priority_query" || // Skip in favor of VK_KHR_global_priority
                    lastFoundExtension == "VK_EXT_tooling_info") // Has dedicated vkGetPhysicalDeviceToolPropertiesEXT() function
                    continue;
                if (features)
                    extensions[lastFoundExtension].featuresName = std::move(result);
                else // properties
                    extensions[lastFoundExtension].propertiesName = std::move(result);
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
            writeMethodDecl(decl, names.featuresName);
        if (!names.propertiesName.empty())
            writeMethodDecl(decl, names.propertiesName);
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
            writeMethodImpl(impl, names.featuresName, true);
        if (!names.propertiesName.empty())
        {
            if (!names.featuresName.empty())
                impl << std::endl; // Add spacing between methods
            writeMethodImpl(impl, names.propertiesName, false);
        }
        impl << "#endif // " << it.first << std::endl;
        impl << std::endl;
    }

    std::cout << "End of source generation" << std::endl;
    return 0;
}
