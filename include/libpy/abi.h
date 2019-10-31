#pragma once

#include "libpy/exception.h"

namespace py::abi {
/** Structure for holding the ABI version of the libpy library.

    @note This must match `libpy.load_library._abi_verson` in the Python support library.
 */
struct abi_version {
    int major;
    int minor;
    int patch;
};

std::ostream& operator<<(std::ostream& stream, abi_version v) {
    return stream << v.major << '.' << v.minor << '.' << v.patch;
}

namespace detail {
constexpr abi_version header_libpy_abi_version{0, 2, 0};
}  // namespace detail

extern "C" abi_version libpy_abi_version;

/** Check if two abi versions are compatible.

    @param provider The version of the implementation provider.
    @param consumer The version of the implementation consumer.
 */
inline bool compatible_versions(abi_version provider, abi_version consumer) {
    return provider.major == consumer.major && provider.minor >= consumer.minor;
}

/** Check that the ABI of the libpy object is compatible with an extension module
    compiled against libpy.

    @return true with a Python exception raised if the ABI versions are incompatible.
 */
inline bool ensure_compatible_libpy_abi() {
    if (!compatible_versions(libpy_abi_version,
                             detail::header_libpy_abi_version)) {
        py::raise(PyExc_ImportError)
            << "libpy compiled version is incompatible with the compiled version of this "
               "extension module\nlibpy version: "
            << libpy_abi_version
            << "\nthis library:  " << detail::header_libpy_abi_version;
        return true;
    }
    return false;
}
}  // namespace py::abi