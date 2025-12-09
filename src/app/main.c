#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "parse_args.h"

int main(int argc, char** argv){
    setlocale(LC_ALL, "ru_RU.UTF-8");

    parse_args(argc, argv);

    puts("success");
    return 0;
}