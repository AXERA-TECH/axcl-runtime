#include <stdio.h>
#include "ax_module_version.h"

__attribute__((used)) void print_sdk_version() {
    printf("%s\n", axera_module_version);
}
