#include <stdio.h>

#include "ansi.h"

int main() {
    printf(ANSI_BOLD "hi" ANSI_RESET " yo");
    return 0;
}
