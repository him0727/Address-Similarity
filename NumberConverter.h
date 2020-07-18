#ifndef _ADDRESSSIMILARITY_ASUTILS_NUMBERCONVERTER_H
#define _ADDRESSSIMILARITY_ASUTILS_NUMBERCONVERTER_H
#include "StringTools.h"
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <regex>

/*
 * This header file declares the static public methods for NumberConverter class, which is intended to be used by AddressSimilarity class. 
 * For the more details, refer to ASUtils/NumberConverter.cpp.
 */
class NumberConverter {
 
  public:
    static std::string arabic_to_chs_simple(int num);
    static std::string arabic_to_chs_full(int num, bool init = true);
    static std::string to_chs_num(std::string address1, std::string address2);
  
};

#endif