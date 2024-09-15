namespace magma
{
template<class Type>
inline std::vector<Type> QueryPool::getQueryResults(uint32_t firstQuery, uint32_t queryCount, VkQueryResultFlags flags) const
{
    std::vector<Type> data(queryCount);
    const VkResult result = vkGetQueryPoolResults(getNativeDevice(), handle, firstQuery, queryCount,
        sizeof(Type) * queryCount, data.data(), sizeof(Type), flags);
    MAGMA_ASSERT((VK_SUCCESS == result) || (VK_NOT_READY == result));
    MAGMA_UNUSED(result);
    return data;
}

template<class Type>
inline std::vector<Type> IntegerQueryPool::getResults(uint32_t firstQuery, uint32_t queryCount, bool wait) const
{
    static_assert(std::is_same<Type, uint32_t>::value || std::is_same<Type, uint64_t>::value,
        "IntegerQueryPool::getResults() specialized only for uint32_t and uint64_t types");
    return std::vector<Type>();
}

template<>
inline std::vector<uint32_t> IntegerQueryPool::getResults<uint32_t>(uint32_t firstQuery, uint32_t queryCount, bool wait) const
{
    return getQueryResults<uint32_t>(firstQuery, queryCount, wait ? VK_QUERY_RESULT_WAIT_BIT : 0);
}

template<>
inline std::vector<uint64_t> IntegerQueryPool::getResults<uint64_t>(uint32_t firstQuery, uint32_t queryCount, bool wait) const
{
    return getQueryResults<uint64_t>(firstQuery, queryCount, VK_QUERY_RESULT_64_BIT | (wait ? VK_QUERY_RESULT_WAIT_BIT : 0));
}

template<class Type>
inline std::vector<QueryPool::Result<Type, Type>> IntegerQueryPool::getResultsWithAvailability(uint32_t firstQuery, uint32_t queryCount) const
{
    static_assert(std::is_same<Type, uint32_t>::value || std::is_same<Type, uint64_t>::value,
        "IntegerQueryPool::getResultsWithAvailability() specialized only for uint32_t and uint64_t types");
    return std::vector<Result<Type, Type>>();
}

template<>
inline std::vector<QueryPool::Result<uint32_t, uint32_t>> IntegerQueryPool::getResultsWithAvailability<uint32_t>(uint32_t firstQuery, uint32_t queryCount) const
{
    return getQueryResults<Result<uint32_t, uint32_t>>(firstQuery, queryCount, VK_QUERY_RESULT_WITH_AVAILABILITY_BIT);
}

template<>
inline std::vector<QueryPool::Result<uint64_t, uint64_t>> IntegerQueryPool::getResultsWithAvailability<uint64_t>(uint32_t firstQuery, uint32_t queryCount) const
{
    return getQueryResults<Result<uint64_t, uint64_t>>(firstQuery, queryCount, VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WITH_AVAILABILITY_BIT);
}
} // namespace magma
