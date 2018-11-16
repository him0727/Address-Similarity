#include <bits/stdc++.h>
#include "ASutils.h"

using namespace std;

const string CHS_NUM[11] = {"零", "一", "二", "三", "四", "五", "六", "七", "八", "九", "十"};

/*--------------------------------------------------------------------------------------------------
 Strip tabs and spaces
 @return: reference of addr
 @params: string addr -> Address
--------------------------------------------------------------------------------------------------*/
void strip(string &addr) {
  const string WB = " \t";
  auto start = addr.find_first_not_of(WB);
  if (start == string::npos) {
    return;
  }
  auto end = addr.find_last_not_of(WB);
  addr = addr.substr(start, end - start + 1);
  start = addr.find_first_of(WB);
  while (start != string::npos) {
    end = addr.find_first_not_of(WB, start);
    addr.replace(start, end - start, " ");
    start = addr.find_first_of(WB, start + 1);
  }
}

/*--------------------------------------------------------------------------------------------------
 Replace escape or replace word
 @return: reference of addr
 @params: string addr -> Address
          string find -> word to be replaced
          string replace -> word to replace
--------------------------------------------------------------------------------------------------*/
void replace(string &addr, const string &find, const string &replace) {
  for(string::size_type i = 0; (i = addr.find(find, i)) != string::npos; i += replace.length()) {
    addr.replace(i, find.length(), replace);
  }
}

/*--------------------------------------------------------------------------------------------------
 Transform to English alphabet lower case letters
 @return: reference of addr
 @params: string addr -> English address
--------------------------------------------------------------------------------------------------*/
void lower(string &addr) {
  transform(addr.begin(), addr.end(), addr.begin(), ::tolower);
}

/*--------------------------------------------------------------------------------------------------
 Transform UTF8 to wide characters
 @return: wstring -> Address in wide string
 @params: string addr -> Address
--------------------------------------------------------------------------------------------------*/
wstring utf8_to_wstring(string &addr) {
  wstring_convert<codecvt_utf8<wchar_t>> conv;
  return conv.from_bytes(addr);
}

/*--------------------------------------------------------------------------------------------------
 Transform Arabic numbers to simple Chinese numbers
 @return: string -> Number in simple Chinese number format
 @params: int num -> Arabic number
--------------------------------------------------------------------------------------------------*/
string arabic_to_chs_simple(int num) {
  string res = "";
  string str_num = to_string(num);
  for (char c : str_num) {
    res += CHS_NUM[c - '0'];
  }
  return res;
}

/*--------------------------------------------------------------------------------------------------
 Transform Arabic numbers to full Chinese numbers
 @return: string -> Number in full Chinese number format
 @params: int num -> Arabic number
          (Optional) bool init -> default true, first time to call this function
--------------------------------------------------------------------------------------------------*/
string arabic_to_chs_full(int num, bool init) {
  string res = "";
  vector<pair<string, int>> unit = {{"億", 1e8}, {"萬", 1e4}, {"千", 1e3}, {"百", 1e2}, {"十", 10}}; 
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
  for (auto uni : unit) {
    if (num >= uni.second) {
      int digs = num / uni.second;
      int dig = num % uni.second;
      string add_zero = uni.second > 10 && dig > 0 && dig < 10 ? "零" : "";
      res = arabic_to_chs_full(digs, false) + uni.first + add_zero + arabic_to_chs_full(dig, false); 
      return res;
    }
  }
  res += arabic_to_chs_simple(num);
  return res;
}

/*--------------------------------------------------------------------------------------------------
 Decide which type of Chinese number to be applied to addr1 in order to make it as similar as addr2
 @return: string -> Formatted address 1 in either simple or full Chinese number format
 @params: string addr1 -> Address 1
          string addr2 -> Address 2
--------------------------------------------------------------------------------------------------*/
string to_chs_num(string addr1, string addr2) {
  regex rx(R"(([[:digit:]]+))");
  smatch sm;
  string str = addr1, res = addr1;
  while (regex_search(str, sm, rx)) {
    string full = arabic_to_chs_full(stoi(sm[0]));
    string simple = arabic_to_chs_simple(stoi(sm[0]));
    if (addr2.find(full) != string::npos) {
      replace(res, sm[0], full);
    } else {
      replace(res, sm[0], simple);
    }
    str = sm.suffix().str();
  }
  return res;
}