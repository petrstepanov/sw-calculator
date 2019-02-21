/*
 * Debug.h
 *
 *  Created on: Feb 4, 2019
 *      Author: student
 */

#ifndef SRC_UTIL_DEBUG_H_
#define SRC_UTIL_DEBUG_H_

// http://www.cplusplus.com/forum/beginner/42190/
// https://stackoverflow.com/questions/3046889/optional-parameters-with-c-macros

#define USEDEBUG

#define PRINT_STRING_1_ARGS(A)   std::cout << std::endl << A << std::endl;
#define PRINT_STRING_2_ARGS(A,B) std::cout << std::endl << "[" << A << "]" << std::endl << B << std::endl;

#define GET_3RD_ARG(arg1, arg2, arg3, ...) arg3

#define PRINT_STRING_MACRO_CHOOSER(...) \
    GET_3RD_ARG(__VA_ARGS__, PRINT_STRING_2_ARGS, \
                PRINT_STRING_1_ARGS, )

#ifdef USEDEBUG
#define Debug(...) PRINT_STRING_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)
#else
#define Debug(...)
#endif

#endif /* SRC_UTIL_DEBUG_H_ */
