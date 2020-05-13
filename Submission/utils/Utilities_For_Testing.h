//
// Created by noam on 01/05/2020.
//

#ifndef EX_1_UTILITIES_FOR_TESTING_H
#define EX_1_UTILITIES_FOR_TESTING_H

#include <stdbool.h>
#include <stdio.h>




#define ASSERT_TEST_FOR_VOTES(expr)                                      \
    do {                                                                          \
        if (!(expr)) {                                                            \
            printf("\nAssertion failed at %d : %d iteration %s:%d %s ",idx , j, __FILE__, __LINE__, #expr); \
            return false;                                                         \
        }                                                                         \
    } while (0)

#endif //EX_1_UTILITIES_FOR_TESTING_H
