namespace magma
{
inline bool DescriptorSetTable::dirty()
{
    const auto& list = getReflection();
    return std::any_of(list.begin(), list.end(),
        [](const auto& it)
        {
            return it.get().dirty();
        });
}

template<class... Descriptor>
inline void DescriptorSetTable::setReflection(Descriptor&&... args)
{   // Use "temporary array" idiom
    // https://stackoverflow.com/questions/28866559/writing-variadic-template-constructor
    std::initializer_list<int>{
        (reflection.push_back(std::forward<Descriptor&>(args)), void(), 0)...
    };
}
} // namespace magma

/* Variadic macro used to simplify reflection of layout structure members.
   It takes a variable number of arguments through __VA_ARGS__ and uses
   variadic template method to populate the list of descriptor set bindings. */

#define MAGMA_REFLECT(...)\
const magma::DescriptorList& getReflection() override\
{\
    if (reflection.empty())\
        setReflection(__VA_ARGS__);\
    return reflection;\
}
