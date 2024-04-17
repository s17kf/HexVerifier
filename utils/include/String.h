//
// Created by stefan on 20/03/24.
//

#ifndef CALCULATORAADS_STRING_H
#define CALCULATORAADS_STRING_H

#include <cstdio>
#include <cstring>

#include "List.h"

namespace utils {

    class String {
    public:
        String() = delete;

        static void rstrip(std::string *str, const std::string &toRemove = " ");

        static void lstrip(std::string *str, const std::string &toRemove = " ");

        static void strip(std::string *str, const std::string &toRemove = " ");

        static void split(const std::string *str, data_structures::List<std::string> &result,
                          const std::string &delimiter = " ");
    };

} // utils


#endif //CALCULATORAADS_STRING_H
