namespace magma::exception
{
inline Exception::Exception(const char *message) noexcept:
    message(message),
    location{}
{}

inline Exception::Exception(const char *message, const source_location& location) noexcept:
    message(message),
    location(location)
{}
} // namespace magma::exception
