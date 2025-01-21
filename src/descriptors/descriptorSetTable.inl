namespace magma
{
inline bool DescriptorSetTable::valid()
{
    auto const& list = getReflection();
    return std::all_of(list.begin(), list.end(),
        [](auto const& it)
        {
            return it.get().resourceBinded();
        });
}

inline bool DescriptorSetTable::dirty()
{
    auto const& list = getReflection();
    return std::any_of(list.begin(), list.end(),
        [](auto const& it)
        {
            return it.get().modified();
        });
}

template<class... Args>
inline void DescriptorSetTable::setReflection(Args&&... args)
{
    static_assert(sizeof...(Args) > 0,
        "invalid count of reflected descriptor table members");
    // Use "temporary array" idiom
    // https://stackoverflow.com/questions/28866559/writing-variadic-template-constructor
    auto list = std::initializer_list<int>{
        (reflection.push_back(std::forward<Args&>(args)), void(), 0)...
    };
    MAGMA_UNUSED(list);
}
} // namespace magma

/* Variadic macro used to simplify reflection of layout structure members.
   It takes a variable number of arguments through __VA_ARGS__ and uses
   variadic template method to populate the list of descriptor set bindings. */

#define MAGMA_REFLECT(...)\
const magma::DescriptorSetTableBindings& getReflection() override\
{\
    if (reflection.empty())\
        setReflection(__VA_ARGS__);\
    return reflection;\
}
