#include <stdbool.h>
#include <stdint.h>

#include "rosalia/semver.h"

#ifdef __cplusplus
extern "C" {
#endif

bool rosa_semver_equal(rosa_semver l, rosa_semver r)
{
    return l.major == r.major && l.minor == r.minor && l.patch == r.patch;
}

bool rosa_semver_satisfies(rosa_semver required, rosa_semver provided)
{
    return provided.major == required.major && (provided.minor > required.minor || (provided.minor == required.minor && provided.patch >= required.patch));
}

#ifdef __cplusplus
}
#endif
