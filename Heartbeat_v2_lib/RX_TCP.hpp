#include <optional>
#include <string>

namespace roboseals {

/**
 * @brief Resolves a DNS hostname into an IP address.
 * @param ipHostAddress DNS hostname to be resolved into IP address
 * @return an empty std::optional if hostname cannot be resolved, otherwise returns IP address string 
 */
std::optional<std::string> resolveHost(const std::string &ipHostAddress);

}