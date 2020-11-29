#include "ss_error.h"

void ss_exit_if_error(ss_error *error) {
    if (error) {
        printf("\033[0;31m""[StaticScript Runtime Error]:\n\tcode: %d\n\tdescription: %s\n""\033[0m", error->code, error->description);
        ss_error_delete(error);
        abort();
    } else {
        ss_error_delete(error);
    }
}