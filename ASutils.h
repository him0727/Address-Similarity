#ifndef _ASUTILS_H
#define _ASUTILS_H

using namespace std;

/*--------------------------------------------------------------------------------------------------
 Header file listing all variables and methods of AddressSimilarity String utilties
--------------------------------------------------------------------------------------------------*/

const vector<string> ESCAPE_CHARS = {",", "，", ".", "。", "(", ")", "（", "）", "[", "]", "〔", "〕",
                                      "{", "}", "｛", "｝", "|", "｜", "+", "＋", "/", "／", "\\", "＼",
                                      "~", "～", ";", "；", "*", "＊", "@", "＠", "\"", "’", "”", "#", "＃",
                                      "!", "！", "$", "＄", "%", "％", "<", "＜", ">", "＞", "^", "︿", "=", "＝", "、"};

void strip(string &addr);

void replace(string &addr, const string &find, const string &replace);

void lower(string &addr);

wstring utf8_to_wstring(string &addr);

string arabic_to_chs_simple(int num);

string arabic_to_chs_full(int num, bool init = true);

string to_chs_num(string addr1, string addr2);

#endif