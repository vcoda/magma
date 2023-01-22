namespace magma
{
inline bool DescriptorSetLayoutReflection::dirty()
{
    for (const auto descriptor: getBindingDescriptors())
    {
        if (descriptor->dirty())
            return true;
    }
    return false;
}

template<class... Descriptor>
inline void DescriptorSetLayoutReflection::setReflection(Descriptor&&... args)
{   // Use "temporary array" idiom
    // https://stackoverflow.com/questions/28866559/writing-variadic-template-constructor
    std::initializer_list<int>{
        (reflection.push_back(std::forward<Descriptor>(args)), void(), 0)...
    };
}

inline const std::vector<descriptor::Descriptor*>& DescriptorSetLayoutReflection::getReflection() const noexcept
{
    return reflection;
}
} // namespace magma

/* Variadic macro used to simplify reflection of layout structure members.
   It takes a variable number of arguments through __VA_ARGS__ and uses
   variadic template method to populate the list of descriptor set bindings. */

#define MAGMA_REFLECT(...)\
const std::vector<magma::descriptor::Descriptor*>& getBindingDescriptors() override\
{\
    if (!hasReflection())\
        setReflection(__VA_ARGS__);\
    return getReflection();\
}
