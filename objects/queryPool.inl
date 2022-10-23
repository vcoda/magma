namespace magma
{
template<class Type>
inline std::vector<Type> QueryPool::getQueryResults(uint32_t firstQuery, uint32_t queryCount, VkQueryResultFlags flags) const noexcept
{
    std::vector<Type> data;
    try {
        data.resize(queryCount, {0});
    } catch (...) {
        return {};
    }
#ifdef MAGMA_DEBUG
    // Initialize query results with dirty value to let application know that
    // results are not yet available and that retrieved values are incorrect.
    if (flags & VK_QUERY_RESULT_64_BIT)
        fillDirty<Type, uint64_t>(data);
    else
        fillDirty<Type, uint32_t>(data);
#endif // MAGMA_DEBUG
    constexpr VkDeviceSize stride = sizeof(Type);
    const size_t dataSize = sizeof(Type) * queryCount;
    const VkResult result = vkGetQueryPoolResults(MAGMA_HANDLE(device), handle, firstQuery, queryCount, dataSize, data.data(), stride, flags);
    MAGMA_ASSERT((VK_SUCCESS == result) || (VK_NOT_READY == result));
    return data;
}

template<class Type, class Int>
inline void QueryPool::fillDirty(std::vector<Type>& data) const noexcept
{
    Int *p = reinterpret_cast<Int *>(data.data());
    MAGMA_ASSERT((sizeof(Type) * data.size()) % sizeof(Int) == 0);
    const std::size_t n = (sizeof(Type) * data.size()) / sizeof(Int);
    for (std::size_t i = 0; i < n; ++i)
        p[i] = BadQueryResult<Int>::value;
}

template<class Type>
inline std::vector<Type> OcclusionQuery::getResults(uint32_t firstQuery, uint32_t queryCount, bool wait) const noexcept
{
    static_assert(std::is_same<Type, uint32_t>::value || std::is_same<Type, uint64_t>::value,
        "OcclusionQuery::getResults() defined only for uint32_t and uint64_t types");
    return std::vector<Type>();
}

template<>
inline std::vector<uint32_t> OcclusionQuery::getResults<uint32_t>(uint32_t firstQuery, uint32_t queryCount, bool wait) const noexcept
{
    return getQueryResults<uint32_t>(firstQuery, queryCount, wait ? VK_QUERY_RESULT_WAIT_BIT : 0);
}

template<>
inline std::vector<uint64_t> OcclusionQuery::getResults<uint64_t>(uint32_t firstQuery, uint32_t queryCount, bool wait) const noexcept
{
    return getQueryResults<uint64_t>(firstQuery, queryCount, VK_QUERY_RESULT_64_BIT | (wait ? VK_QUERY_RESULT_WAIT_BIT : 0));
}

template<class Type>
inline std::vector<QueryResult<Type, Type>> OcclusionQuery::getResultsWithAvailability(uint32_t firstQuery, uint32_t queryCount) const noexcept
{
    static_assert(std::is_same<Type, uint32_t>::value || std::is_same<Type, uint64_t>::value,
        "OcclusionQuery::getResultsWithAvailability() defined only for uint32_t and uint64_t types");
    return std::vector<QueryResult<Type, Type>>();
}

template<>
inline std::vector<QueryResult<uint32_t, uint32_t>> OcclusionQuery::getResultsWithAvailability<uint32_t>(uint32_t firstQuery, uint32_t queryCount) const noexcept
{
    return getQueryResults<QueryResult<uint32_t, uint32_t>>(firstQuery, queryCount, VK_QUERY_RESULT_WITH_AVAILABILITY_BIT);
}

template<>
inline std::vector<QueryResult<uint64_t, uint64_t>> OcclusionQuery::getResultsWithAvailability<uint64_t>(uint32_t firstQuery, uint32_t queryCount) const noexcept
{
    return getQueryResults<QueryResult<uint64_t, uint64_t>>(firstQuery, queryCount, VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WITH_AVAILABILITY_BIT);
}

template<class Type>
inline std::vector<Type> TimestampQuery::getResults(uint32_t firstQuery, uint32_t queryCount, bool wait) const noexcept
{
    static_assert(std::is_same<Type, uint32_t>::value || std::is_same<Type, uint64_t>::value,
        "TimestampQuery::getResults() defined only for uint32_t and uint64_t types");
    return std::vector<Type>();
}

template<>
inline std::vector<uint32_t> TimestampQuery::getResults<uint32_t>(uint32_t firstQuery, uint32_t queryCount, bool wait) const noexcept
{
    return getQueryResults<uint32_t>(firstQuery, queryCount, wait ? VK_QUERY_RESULT_WAIT_BIT : 0);
}

template<>
inline std::vector<uint64_t> TimestampQuery::getResults<uint64_t>(uint32_t firstQuery, uint32_t queryCount, bool wait) const noexcept
{
    return getQueryResults<uint64_t>(firstQuery, queryCount, VK_QUERY_RESULT_64_BIT | (wait ? VK_QUERY_RESULT_WAIT_BIT : 0));
}

template<class Type>
inline std::vector<QueryResult<Type, Type>> TimestampQuery::getResultsWithAvailability(uint32_t firstQuery, uint32_t queryCount) const noexcept
{
    static_assert(std::is_same<Type, uint32_t>::value || std::is_same<Type, uint64_t>::value,
        "TimestampQuery::getResultsWithAvailability() defined only for uint32_t and uint64_t types");
    return std::vector<QueryResult<Type, Type>>();
}

template<>
inline std::vector<QueryResult<uint32_t, uint32_t>> TimestampQuery::getResultsWithAvailability<uint32_t>(uint32_t firstQuery, uint32_t queryCount) const noexcept
{
    return getQueryResults<QueryResult<uint32_t, uint32_t>>(firstQuery, queryCount, VK_QUERY_RESULT_WITH_AVAILABILITY_BIT);
}

template<>
inline std::vector<QueryResult<uint64_t, uint64_t>> TimestampQuery::getResultsWithAvailability<uint64_t>(uint32_t firstQuery, uint32_t queryCount) const noexcept
{
    return getQueryResults<QueryResult<uint64_t, uint64_t>>(firstQuery, queryCount, VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WITH_AVAILABILITY_BIT);
}

#ifdef VK_NV_ray_tracing
template<class Type>
inline std::vector<Type> AccelerationStructureCompactedSizeQuery::getResults(uint32_t firstQuery, uint32_t queryCount, bool wait) const noexcept
{
    static_assert(std::is_same<Type, uint32_t>::value || std::is_same<Type, uint64_t>::value,
        "AccelerationStructureCompactedSizeQuery::getResults() defined only for uint32_t and uint64_t types");
    return std::vector<Type>();
}

template<>
inline std::vector<uint32_t> AccelerationStructureCompactedSizeQuery::getResults<uint32_t>(uint32_t firstQuery, uint32_t queryCount, bool wait) const noexcept
{
    return getQueryResults<uint32_t>(firstQuery, queryCount, wait ? VK_QUERY_RESULT_WAIT_BIT : 0);
}

template<>
inline std::vector<uint64_t> AccelerationStructureCompactedSizeQuery::getResults<uint64_t>(uint32_t firstQuery, uint32_t queryCount, bool wait) const noexcept
{
    return getQueryResults<uint64_t>(firstQuery, queryCount, VK_QUERY_RESULT_64_BIT | (wait ? VK_QUERY_RESULT_WAIT_BIT : 0));
}

template<class Type>
inline std::vector<QueryResult<Type, Type>> AccelerationStructureCompactedSizeQuery::getResultsWithAvailability(uint32_t firstQuery, uint32_t queryCount) const noexcept
{
    static_assert(std::is_same<Type, uint32_t>::value || std::is_same<Type, uint64_t>::value,
        "AccelerationStructureCompactedSizeQuery::getResultsWithAvailability() defined only for uint32_t and uint64_t types");
    return std::vector<QueryResult<Type, Type>>();
}

template<>
inline std::vector<QueryResult<uint32_t, uint32_t>> AccelerationStructureCompactedSizeQuery::getResultsWithAvailability<uint32_t>(uint32_t firstQuery, uint32_t queryCount) const noexcept
{
    return getQueryResults<QueryResult<uint32_t, uint32_t>>(firstQuery, queryCount, VK_QUERY_RESULT_WITH_AVAILABILITY_BIT);
}

template<>
inline std::vector<QueryResult<uint64_t, uint64_t>> AccelerationStructureCompactedSizeQuery::getResultsWithAvailability<uint64_t>(uint32_t firstQuery, uint32_t queryCount) const noexcept
{
    return getQueryResults<QueryResult<uint64_t, uint64_t>>(firstQuery, queryCount, VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WITH_AVAILABILITY_BIT);
}
#endif // VK_NV_ray_tracing
} // namespace magma