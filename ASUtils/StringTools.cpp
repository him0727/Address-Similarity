#include "../StringTools.h"

static const std::string WB = " \t";

/**
 * Strip tabs and spaces.
 * @param address (string reference) Address
 */
void StringTools::strip(std::string& address) {
  auto start = address.find_first_not_of(WB);
  if (start == std::string::npos) {
    return;
  }
  auto end = address.find_last_not_of(WB);
  address = address.substr(start, end - start + 1);
  start = address.find_first_of(WB);
  while (start != std::string::npos) {
    end = address.find_first_not_of(WB, start);
    address.replace(start, end - start, " ");
    start = address.find_first_of(WB, start + 1);
  }
}

/**
 * Replace escape or replace word
 * @param address (string reference) Address
 * @param find (string reference) Word to be replace
 * @param replace (string reference) Word to replace
 */
void StringTools::replace(std::string& address, const std::string& find, const std::string& replace) {
  for (std::string::size_type i = 0; (i = address.find(find, i)) != std::string::npos; i += replace.length()) {
    address.replace(i, find.length(), replace);
  }
}

/**
 * Transform to English alphabet lower case letters
 * @param address (string reference) Address
 */
void StringTools::lower(std::string& address) {
  std::transform(address.begin(), address.end(), address.begin(), ::tolower);
}

/**
 * Transform UTF8 to wide characters.
 * @param address (string reference) Address
 * @return Address represented as wide string (wstring)
 */
std::wstring StringTools::utf8_to_wstring(std::string& address) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
  return conv.from_bytes(address);
}
