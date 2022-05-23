namespace magma
{
inline bool DescriptorSetLayoutReflection::dirty()
{
    const std::vector<binding::DescriptorSetLayoutBinding *>& descriptorBindings = getDescriptorBindings();
    for (const auto binding : descriptorBindings)
    {
        if (binding->dirty())
            return true;
    }
    return false;
}

template<class... DescriptorSetLayoutBinding>
inline void DescriptorSetLayoutReflection::setReflection(DescriptorSetLayoutBinding&&... args)
{   // Use "temporary array" idiom
    // https://stackoverflow.com/questions/28866559/writing-variadic-template-constructor
    std::initializer_list<int>{
        (reflection.push_back(std::forward<DescriptorSetLayoutBinding>(args)), void(), 0)...
    };
}

inline const std::vector<binding::DescriptorSetLayoutBinding *>& DescriptorSetLayoutReflection::getReflection() const noexcept
{
    return reflection;
}

/* Variadic macro used to simplify reflection of layout structure members.
   It takes a variable number of arguments through __VA_ARGS__ and uses
   variadic template method to populate the list of descriptor set bindings. */

#define MAGMA_REFLECT(...)\
const std::vector<magma::binding::DescriptorSetLayoutBinding *>& getDescriptorBindings() override\
{\
    if (!hasReflection())\
        setReflection(__VA_ARGS__);\
    return getReflection();\
}
} // namespace magma
