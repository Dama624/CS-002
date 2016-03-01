/**
 * @file
 * @author The CS2 TA Team <<cs2-tas@ugcs.caltech.edu>>
 * @version 1.0
 * @date 2013-2014
 * @copyright This code is in the public domain.
 *
 * @brief A command-line argument example.
 *
 */

#include <cstdio>

int main(int argc, char const *argv[])
{
    printf("argc: %d\n", argc);
    printf("argv: ");
    for (int i = 0; i < argc; ++i)
    {
        printf("%s ", argv[i]);
    }
    printf("\n");
    return 0;
}
