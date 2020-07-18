#ifndef _ADDRESSSIMILARITY_ASUTILS_STRINGTOOLS_H
#define _ADDRESSSIMILARITY_ASUTILS_STRINGTOOLS_H
#include <iostream>
#include <string>
#include <locale>
#include <vector>
#include <codecvt>
#include <iomanip>
#include <algorithm>

/*
 * This header file declares the static public methods for StringTools class, which is intended to be used by AddressSimilarity class. 
 * For the more details, refer to ASUtils/StringTools.cpp.
 */
class StringTools {
  
  public:
    static void strip(std::string& address);
    static void replace(std::string& address, const std::string& find, const std::string& replace);
    static void lower(std::string& address);
    static std::wstring utf8_to_wstring(std::string& address);
  
};

#endif