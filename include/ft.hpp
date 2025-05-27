
#ifndef FT_HPP
# define FT_HPP

# include <string>
# include <sstream>

namespace ft
{
    template <typename T> std::string to_string(T val) {
        std::ostringstream oss;

        oss << val;
        return oss.str();
    }

    void ltrim(std::string &str, const std::string &set = " ");
} // namespace FT

#endif
