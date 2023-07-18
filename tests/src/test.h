#ifndef _TESTS_COMMON_H_
#define _TESTS_COMMON_H_

#include "rexo/include/rexo.h"

struct test_suite {
    int (*run)(void);
};

#endif /* _TESTS_COMMON_H_ */