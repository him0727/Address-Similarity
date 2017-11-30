#include <bits/stdc++.h>

using namespace std;

class AddressSimilarity {
  
  private:
  
    float chs_cos_weight, chs_com_weight;
    float eng_cos_weight, eng_com_weight;
    int chs_threshold, eng_threshold;
    string chs_num[11] = {"零", "一", "二", "三", "四", "五", "六", "七", "八", "九", "十"};
    
    string arabic_to_chs_simple(int num) {
      string res = "";
      string str_num = to_string(num);
      for (char &c : str_num) {
        res += chs_num[c - '0'];
      }
      return res;
    }
    
    string arabic_to_chs_full(int num, bool init = true) {
      string res = "";
      vector<pair<string, int>> unit = {make_pair("億", 1e8), make_pair("萬", 1e4), make_pair("千", 1e3), make_pair("百", 1e2), make_pair("十", 10)}; 
      if (init) {
        if (!num) return "零";
        if (num == 10) return "十";
        if (num > 10 && num < 20) return "十" + chs_num[num % 10];
      }
      if (!num) return "";
      for (int i = 0; i < unit.size(); i++) {
        if (num >= unit[i].second) {
          int num1 = num / unit[i].second;
          int num2 = num % unit[i].second;
          string add_zero = unit[i].second > 10 && num2 && num2 < 10 ? "零" : "";
          res = arabic_to_chs_full(num1, false) + unit[i].first + add_zero + arabic_to_chs_full(num2, false); 
          return res;
        }
      }
      res += arabic_to_chs_simple(num);
      return res;
    }
    
    string to_chs_num(string addr1, string addr2) {
      regex rx(R"(([[:digit:]]+))");
      smatch sm;
      string str = addr1, res = addr1;
      while (regex_search(str, sm, rx)) {
        string num1 = arabic_to_chs_full(stoi(sm[0]));
        string num2 = arabic_to_chs_simple(stoi(sm[0]));
        if (addr2.find(num1) != string::npos) {
          replace(res, sm[0], num1);
        } else {
          replace(res, sm[0], num2);
        }
        str = sm.suffix().str();
      }
      return res;
    }
    
    string strip(const string &addr) {
      const string WB = " \t";
      auto start = addr.find_first_not_of(WB);
      if (start == string::npos) return "";
      auto end = addr.find_last_not_of(WB);
      auto res = addr.substr(start, end - start + 1);
      start = res.find_first_of(WB);
      while (start != string::npos) {
        end = res.find_first_not_of(WB, start);
        res.replace(start, end - start, " ");
        start = res.find_first_of(WB, start + 1);
      }
      return res;
    }
    
    void replace(string &source, const string &find, const string &replace) {
      for(string::size_type i = 0; (i = source.find(find, i)) != string::npos; i += replace.length()) {
        source.replace(i, find.length(), replace);
      }
    }
    
    void lower(string &source) {
      transform(source.begin(), source.end(), source.begin(), ::tolower);
    }

    wstring utf8_to_wstring(const string& str) {
      wstring_convert<codecvt_utf8<wchar_t>> myconv;
      return myconv.from_bytes(str);
    }
    
    string *pre_processing(string raw_addr1, string raw_addr2, const vector<string> &escape_chars = {}, const vector<pair<string, string>> &replace_chars = {}) {
      string addr1 = strip(raw_addr1), addr2 = strip(raw_addr2);
      lower(addr1);
      lower(addr2);
      for (int i = 0; i < escape_chars.size(); i++) {
        replace(addr1, escape_chars[i], "");
        replace(addr2, escape_chars[i], "");
      }
      for (int i = 0; i < replace_chars.size(); i++) {
        replace(addr1, replace_chars[i].first, replace_chars[i].second);
        replace(addr2, replace_chars[i].first, replace_chars[i].second);
      }
      string *res = new string[2];
      res[0] = addr1, res[1] = addr2;
      return res;
    }
    
    vector<string> split_processing(string addr) {
      vector<string> tokens;
      regex rx_d(R"(([[:digit:]]+))");
      smatch sm;
      string str = addr;
      while (regex_search(str, sm, rx_d)) {
        tokens.push_back(sm[0]);
        str = sm.suffix().str();
      }
      regex rx_s(R"(([a-zA-Z]+))");
      str = addr;
      while (regex_search(str, sm, rx_s)) {
        tokens.push_back(sm[0]);
        str = sm.suffix().str();
      }
      return tokens;
    }
    
    float cos_similarity(string addr1, string addr2) {
      if (addr1 == addr2) return 1.0;
      set<wchar_t> addr1_set, addr2_set, addr_set;
      set<wchar_t>::iterator itr;
      wstring w_addr1 = utf8_to_wstring(addr1), w_addr2 = utf8_to_wstring(addr2);
      for (wchar_t &c : w_addr1) addr1_set.insert(c);
      for (wchar_t &c : w_addr2) addr2_set.insert(c);
      set_union(addr1_set.begin(), addr1_set.end(), addr2_set.begin(), addr2_set.end(), inserter(addr_set, addr_set.end()));
      vector<float> addr1_count, addr2_count;
      vector<float> cross_pd, addr1_pd, addr2_pd;
      for (itr = addr_set.begin(); itr != addr_set.end(); itr++) {
        addr1_count.push_back(count(w_addr1.begin(), w_addr1.end(), *itr));
        addr2_count.push_back(count(w_addr2.begin(), w_addr2.end(), *itr));
      }
      float addr1_sum = 0, addr2_sum = 0, cross_sum = 0;
      for (int i = 0; i < addr1_count.size(); i++) {
        cross_pd.push_back(addr1_count[i] * addr2_count[i]);
        addr1_pd.push_back(addr1_count[i] * addr1_count[i]);
        addr2_pd.push_back(addr2_count[i] * addr2_count[i]);
        cross_sum += cross_pd[i];
        addr1_sum += addr1_pd[i];
        addr2_sum += addr2_pd[i];
      }
      float diff = sqrt(addr1_sum) * sqrt(addr2_sum);
      return !diff ? 0 : cross_sum / diff;
    }
    
    float com_similarity(string addr1, string addr2, int threshold) {
      if (addr1 == addr2) return 1.0;
      wstring w_addr1 = utf8_to_wstring(addr1), w_addr2 = utf8_to_wstring(addr2);
      float val[w_addr1.size() + 1][w_addr2.size() + 1] = {0};
      for (int i = 1; i <= w_addr1.size(); i++) {
        for (int j = 1; j <= w_addr2.size(); j++) {
          if (w_addr1[i - 1] == w_addr2[j - 1]) {
            val[i][j] = val[i - 1][j - 1] + 1;
          }    
        }
      }
      float score = 0;
      int i = w_addr1.size(), j = w_addr2.size();
      int previous = threshold == 1 ? 1 : 0;
      while (i > 0) {
        j = w_addr2.size();
        while (j > 0) {
          if (val[i][j] >= threshold && val[i - 1][j - 1] >= previous) {
            score += val[i][j];
            i -= val[i][j] - 1;
            break;
          }
          j--;
        }
        i--;
      }
      return score / max(w_addr1.size(), w_addr2.size());
    }
    
  public:

    AddressSimilarity(float ccsw = 0.5, float ccmw = 0.5, int ct = 2, float ecsw = 0.3, float ecmw = 0.7, int et = 3) {
      chs_cos_weight = ccsw;
      chs_com_weight = ccmw;
      chs_threshold = ct ? ct : 1;
      eng_cos_weight = ecsw;
      eng_com_weight = ecmw;
      eng_threshold = et ? et : 1;
    }
     
    float compare_chs_addr(string raw_addr1, string raw_addr2) {
      vector<string> escape_chars = {" ", "　", ",", "，", ".", "。",
                                    "號", "号", "鋪", "铺", "舖", "九龍", "九龙", "香港", "新界",
                                    "(", ")", "（", "）", "[", "]", "〔", "〕", "{", "}", "｛", "｝", "|", "｜", "+", "＋",
                                    "/", "／", "\\", "＼", "~", "～", ";", "；", "*", "＊", "@", "＠", "\"", "’", "”",
                                    "#", "＃", "!", "！", "$", "＄", "%", "％", "<", "＜", ">", "＞", "^", "︿", "=", "＝", "、"};
      vector<pair<string, string>> replace_chars = {make_pair("-", "至"), make_pair("—", "至"), make_pair("o", "零"), 
                                                    make_pair("ｏ", "零"), make_pair("Ｏ", "零"), make_pair("&", "及"), make_pair("＆", "及")};
      string addr1 = pre_processing(raw_addr1, raw_addr2, escape_chars, replace_chars)[0];
      string addr2 = pre_processing(raw_addr1, raw_addr2, escape_chars, replace_chars)[1];
      if (!addr1.size() || !addr2.size()) return 0;
      addr1 = to_chs_num(addr1, addr2);
      addr2 = to_chs_num(addr2, addr1);
      float cos_score = cos_similarity(addr1, addr2);
      float com_score = com_similarity(addr1, addr2, chs_threshold);
      return cos_score * chs_cos_weight + com_score * chs_com_weight;
    }
    
    float compare_eng_addr(string raw_addr1, string raw_addr2) {
      vector<string> escape_chars = {"no.", "-", "—", ".", "。", ",", "，", 
                                      " nt", " kln", " kl", "hk", "hong kong", "kowloon", "new territories",
                                      "(", ")", "（", "）", "[", "]", "〔", "〕", "{", "}", "｛", "｝", "|", "｜", "+", "＋",
                                      "/", "／", "\\", "＼", "~", "～", ";", "；", "*", "＊", "&", "＆", "@", "＠", "\"", "’", "”",
                                      "#", "＃", "!", "！", "$", "＄", "%", "％", "<", "＜", ">", "＞", "^", "︿", "=", "＝", "、"};
      string addr1 = pre_processing(raw_addr1, raw_addr2, escape_chars)[0];
      string addr2 = pre_processing(raw_addr1, raw_addr2, escape_chars)[1];
      if (!addr1.size() || !addr2.size()) return 0;
      vector<string> addr1_tokens = split_processing(addr1);
      vector<string> addr2_tokens = split_processing(addr2);
      float total_cos = 0, total_com = 0;
      for (string &s : addr1_tokens) {
        float max_cos = 0, max_com = 0;
        for (string &ss : addr2_tokens) {
          max_cos = max(max_cos, cos_similarity(s, ss));
          max_com = max(max_com, com_similarity(s, ss, eng_threshold));
        }
        total_cos += max_cos;
        total_com += max_com;
      }
      float cos_score = total_cos / addr1_tokens.size();
      float com_score = total_com / addr1_tokens.size();
      return cos_score * eng_cos_weight + com_score * eng_com_weight;
    }
    
    void get_params() {
      printf("CHS/ENG cos.w, com.w, thres. %f %f %d, %f %f %d\n", chs_cos_weight, chs_com_weight, chs_threshold, eng_cos_weight, eng_com_weight, eng_threshold);
    }
    
    void set_params(float ccsw = 0.5, float ccmw = 0.5, int ct = 2, float ecsw = 0.3, float ecmw = 0.7, int et = 3) {
      chs_cos_weight = ccsw;
      chs_com_weight = ccmw;
      chs_threshold = ct ? ct : 1;
      eng_cos_weight = ecsw;
      eng_com_weight = ecmw;
      eng_threshold = et ? et : 1;
    }
    
};