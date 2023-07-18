#ifndef _TESTS_SRC_LIST_EXPERIMENTAL_H_
#define _TESTS_SRC_LIST_EXPERIMENTAL_H_

#include "common/alloc.h"
#include "common/lcg.h"
#include "common/parameters.h"
#include "list-experimental/data.h"

#define list_init list_experimental_init
#define list_destroy list_experimental_destroy

#include "../tests/src/test.h"

#endif /* _TESTS_SRC_LIST_EXPERIMENTAL_H_ */
