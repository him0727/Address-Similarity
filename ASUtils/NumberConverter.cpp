#include "../NumberConverter.h"

static const std::string CHS_NUM[11] = {"零", "一", "二", "三", "四", "五", "六", "七", "八", "九", "十"};
static const std::vector<std::pair<std::string, int>> UNITS = {{"億", 1e8}, {"萬", 1e4}, {"千", 1e3}, {"百", 1e2}, {"十", 10}};

/**
 * Transform Arabic number to simple Chinese number.
 * @param num (int) Arabic number
 * @return Simple Chinese number (string)
 */
std::string NumberConverter::arabic_to_chs_simple(int num) {
  std::string res = "";
  std::string str_num = std::to_string(num);
  for (char c : str_num) {
    res += CHS_NUM[c - '0'];
  }
  return res;
}

/**
 * Transform Arabic number to full Chinese number recursively.
 * @param num (int) Arabic number
 * @param init (bool) First time to call or not
 * @return Full Chinese number (string)
 */
std::string NumberConverter::arabic_to_chs_full(int num, bool init) {
  std::string res = "";
  if (init) {
    if (num == 0) {
      return "零";
    } else if (num == 10) {
      return "十";
    } else if (num > 10 && num < 20){
      return "十" + CHS_NUM[num % 10];
    }
  }
  if (num == 0) {
    return "";
  }
  for (auto unit : UNITS) {
    if (num >= unit.second) {
      int digs = num / unit.second;
      int dig = num % unit.second;
      std::string add_zero = unit.second > 10 && dig > 0 && dig < 10 ? "零" : "";
      res = NumberConverter::arabic_to_chs_full(digs, false) + unit.first + add_zero + NumberConverter::arabic_to_chs_full(dig, false); 
      return res;
    }
  }
  res += NumberConverter::arabic_to_chs_simple(num);
  return res;
}

/**
 * Decide which type of Chinese number to be applied to address1 in order to make it as similar as address2.
 * @param address1 (string) First Chinese address
 * @param address2 (string) Second Chinese address
 * @return Address1 replaced by either simple or full Chinese number (string)
 */
std::string NumberConverter::to_chs_num(std::string address1, std::string address2) {
  std::regex rx(R"(([[:digit:]]+))");
  std::smatch sm;
  std::string str = address1, res = address1;
  while (std::regex_search(str, sm, rx)) {
    std::string full = NumberConverter::arabic_to_chs_full(stoi(sm[0]));
    std::string simple = NumberConverter::arabic_to_chs_simple(stoi(sm[0]));
    if (address2.find(full) != std::string::npos) {
      StringTools::replace(res, sm[0], full);
    } else {
      StringTools::replace(res, sm[0], simple);
    }
    str = sm.suffix().str();
  }
  return res;
}