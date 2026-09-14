#include <stdio.h>

#include "ansi.h"

int main() {
    printf(ANSI_FG_RED "hi " ANSI_RESET "yo");
    return 0;
}
