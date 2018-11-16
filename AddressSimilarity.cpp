#include <bits/stdc++.h>
#include "ASutils.h"
#include "AddressSimilarity.h"

using namespace std;

// Private

/*--------------------------------------------------------------------------------------------------
 Pre-procoessing two given addresses
 @return: array of string -> Two processed addresses
 @params: string addr1 -> Address 1
          string addr2 -> Address 2
          (Optional) vector<string> escape_chars -> List of characters to be escaped
          (Optional) vector<pair<string, string>> replace_chars -> List of characters to be replaced
--------------------------------------------------------------------------------------------------*/
string *pre_processing(string addr1, string addr2, const vector<string> &escape_chars = {}, const vector<pair<string, string>> &replace_chars = {}) {
  strip(addr1), strip(addr2);
  lower(addr1), lower(addr2);
  for (auto e : ESCAPE_CHARS) {
    replace(addr1, e, "");
    replace(addr2, e, "");
  }
  for (auto e : escape_chars) {
    replace(addr1, e, "");
    replace(addr2, e, "");
  }
  for (auto r : replace_chars) {
    replace(addr1, r.first, r.second);
    replace(addr2, r.first, r.second);
  }
  string *res = new string[2];
  res[0] = addr1, res[1] = addr2;
  return res;
}

/*--------------------------------------------------------------------------------------------------
 Split the given English address
 @return: vector of string -> Vector of splitted words
 @params: string addr -> English address
--------------------------------------------------------------------------------------------------*/
vector<string> split_processing(const string &addr) {
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

/*--------------------------------------------------------------------------------------------------
 Compute the consine similarity
 @return: float -> Score of the consine similarity between two addresses
 @params: string raw_addr1 -> Address 1
          string raw_addr2 -> Address 2
--------------------------------------------------------------------------------------------------*/
float cos_similarity(string raw_addr1, string raw_addr2) {
  if (raw_addr1 == raw_addr2) {
    return 1.0;
  }
  if (raw_addr1.size() == 0 || raw_addr2.size() == 0) {
    return 0;
  }
  set<wchar_t> addr1_set, addr2_set, addr_set;
  set<wchar_t>::iterator itr;
  wstring addr1 = utf8_to_wstring(raw_addr1);
  wstring addr2 = utf8_to_wstring(raw_addr2);
  for (wchar_t c : addr1) {
    addr1_set.insert(c);
  }
  for (wchar_t c : addr2) {
    addr2_set.insert(c);
  }
  set_union(addr1_set.begin(), addr1_set.end(), addr2_set.begin(), addr2_set.end(), inserter(addr_set, addr_set.end()));
  vector<float> addr1_count, addr2_count;
  for (auto itr : addr_set) {
    addr1_count.push_back(count(addr1.begin(), addr1.end(), itr));
    addr2_count.push_back(count(addr2.begin(), addr2.end(), itr));
  }
  float addr1_sum = 0, addr2_sum = 0, cross_sum = 0;
  for (int i = 0; i < addr1_count.size(); i++) {
    cross_sum += addr1_count[i] * addr2_count[i];
    addr1_sum += addr1_count[i] * addr1_count[i];
    addr2_sum += addr2_count[i] * addr2_count[i];
  }
  float diff = sqrt(addr1_sum) * sqrt(addr2_sum);
  return diff == 0 ? 0 : cross_sum / diff;
}

/*--------------------------------------------------------------------------------------------------
 Compute the non-overlapping common substrings similarity
 @return: float -> Score of the non-overlapping common substring similarity between two addresses
 @params: string raw_addr1 -> Address 1
          string raw_addr2 -> Address 2
          int threshold -> Threshold of non-overlapping common substrings
--------------------------------------------------------------------------------------------------*/
float com_similarity(string raw_addr1, string raw_addr2, int threshold) {
  if (raw_addr1 == raw_addr2) {
    return 1.0;
  }
  if (raw_addr1.size() == 0 || raw_addr2.size() == 0) {
    return 0;
  }
  wstring addr1 = utf8_to_wstring(raw_addr1);
  wstring addr2 = utf8_to_wstring(raw_addr2);
  float val[addr1.size() + 1][addr2.size() + 1];
  for (int i = 0; i <= addr1.size(); i++) {
    for (int j = 0; j <= addr2.size(); j++) {
      val[i][j] = 0;
    }
  }
  for (int i = 1; i <= addr1.size(); i++) {
    for (int j = 1; j <= addr2.size(); j++) {
      if (addr1[i - 1] == addr2[j - 1]) {
        val[i][j] = val[i - 1][j - 1] + 1;
      }    
    }
  }
  float score = 0;
  int i = addr1.size();
  int previous = threshold != 1;
  while (i > 0) {
    int j = addr2.size();
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
  return score / max(addr1.size(), addr2.size());
}

// Public

/*--------------------------------------------------------------------------------------------------
 AddressSimilarity constructor
 @return: instantiate AddressSimilarity
 @params: float chs_cos_weight -> Default 0.5, Weight for cosine similarity of Chinese address
          float chs_com_weight -> Default 0.5, Weight for non-overlapping common substrings of Chinese address
          int chs_threshold -> Default 2, Threshold of non-overlapping common substrings of Chinese address (at least 1)
          float eng_cos_weight -> Default 0.3, Weight for cosine similarity of English address
          float eng_com_weight -> Default 0.7, Weight for non-overlapping common substrings of English address
          int eng_threshold -> Default 3, Threshold of non-overlapping common substrings of English address (at least 1)
--------------------------------------------------------------------------------------------------*/
AddressSimilarity :: AddressSimilarity(float chs_cos_weight, float chs_com_weight, int chs_threshold,
                                        float eng_cos_weight, float eng_com_weight, int eng_threshold) {
  fprintf(stderr, "AddressSimilarity CREATED\n");
  this -> chs_cos_weight = chs_cos_weight;
  this -> chs_com_weight = chs_com_weight;
  this -> chs_threshold = chs_threshold ? chs_threshold : 1;
  this -> eng_cos_weight = eng_cos_weight;
  this -> eng_com_weight = eng_com_weight;
  this -> eng_threshold = eng_threshold ? eng_threshold : 1;
  srand(time(NULL));
}

/*--------------------------------------------------------------------------------------------------
 AddressSimilarity destructor
--------------------------------------------------------------------------------------------------*/
AddressSimilarity :: ~AddressSimilarity() {
  fprintf(stderr, "AddressSimilarity DESTROYED\n");
}

/*--------------------------------------------------------------------------------------------------
 Function to compare two addresses by auto-detecting language 
 @return: float -> Overall similarity
 @params: string raw_addr1 -> Address 1
          string raw_addr2 -> Address 2
          (Optional) bool enable_rand -> Default true, Enable randomized computation
--------------------------------------------------------------------------------------------------*/
float AddressSimilarity :: compare_addr(string raw_addr1, string raw_addr2, bool enable_rand) {
  if (raw_addr1 == raw_addr2) {
    return 1;
  }
  if (raw_addr1.size() == 0 || raw_addr2.size() == 0) {
    return 0;
  }
  string tmp_addr1 = raw_addr1;
  string tmp_addr2 = raw_addr2;
  strip(tmp_addr1), strip(tmp_addr2);
  for (auto e : ESCAPE_CHARS) {
    replace(tmp_addr1, e, "");
    replace(tmp_addr2, e, "");
  }
  int d_addr1_count = 0, d_addr2_count = 0;
  int a_addr1_count = 0, a_addr2_count = 0;
  for (auto c : tmp_addr1) {
    if (c - '0' >= 0 && c - '0' <= 9) {
      d_addr1_count++;
    } else if ((c - 'a' >= 0 && c - 'a' <= 26) || (c - 'A' >= 0 && c - 'A' <= 26)) {
      a_addr1_count++;
    }
  }
  for (auto c : tmp_addr2) {
    if (c - '0' >= 0 && c - '0' <= 9) {
      d_addr2_count++;
    } else if ((c - 'a' >= 0 && c - 'a' <= 26) || (c - 'A' >= 0 && c - 'A' <= 26)) {
      a_addr2_count++;
    }
  }
  bool addr1_is_chs = tmp_addr1.size() - d_addr1_count > a_addr1_count * 2;
  bool addr2_is_chs = tmp_addr2.size() - d_addr2_count > a_addr2_count * 2;
  if (addr1_is_chs && addr2_is_chs) {
    return compare_chs_addr(raw_addr1, raw_addr2);
  }
  if (!addr1_is_chs && !addr2_is_chs) {
    return compare_eng_addr(raw_addr1, raw_addr2, enable_rand);
  }
  return -1;
}

/*--------------------------------------------------------------------------------------------------
 Function to compare two Chinese addresses
 @return: float -> Overall similarity
 @params: string raw_addr1 -> Address 1
          string raw_addr2 -> Address 2
--------------------------------------------------------------------------------------------------*/
float AddressSimilarity :: compare_chs_addr(string raw_addr1, string raw_addr2) {
  vector<string> escape_chars = {" ", "　", ",", "，", ".", "。",
                                  "號", "号", "鋪", "铺", "舖", "九龍", "九龙", "香港", "新界"};
  vector<pair<string, string>> replace_chars = {{"-", "至"}, {"—", "至"}, {"o", "零"}, {"ｏ", "零"}, {"Ｏ", "零"}, {"&", "及"}, {"＆", "及"}};
  string addr1 = pre_processing(raw_addr1, raw_addr2, escape_chars, replace_chars)[0];
  string addr2 = pre_processing(raw_addr1, raw_addr2, escape_chars, replace_chars)[1];
  if (addr1.size() == 0 || addr2.size() == 0) {
    return 0;
  }
  addr1 = to_chs_num(addr1, addr2);
  addr2 = to_chs_num(addr2, addr1);
  float cos_score = cos_similarity(addr1, addr2);
  float com_score = com_similarity(addr1, addr2, chs_threshold);
  return cos_score * chs_cos_weight + com_score * chs_com_weight;
}

/*--------------------------------------------------------------------------------------------------
 Function to compare two English addresses
 @return: float -> Overall similarity
 @params: string raw_addr1 -> Address 1
          string raw_addr2 -> Address 2
          (Optional) bool enable_rand -> Default true, Enable randomized computation
--------------------------------------------------------------------------------------------------*/
float AddressSimilarity :: compare_eng_addr(string raw_addr1, string raw_addr2, bool enable_rand) {
  vector<string> escape_chars = {"no.", "-", "—",
                                  " nt", " kln", " kl", "hk", "hong kong", "kowloon", "new territories"};
  string addr1 = pre_processing(raw_addr1, raw_addr2, escape_chars)[0];
  string addr2 = pre_processing(raw_addr1, raw_addr2, escape_chars)[1];
  if (addr1.size() == 0 || addr2.size() == 0) {
    return 0;
  }
  vector<string> addr1_tokens = split_processing(addr1);
  vector<string> addr2_tokens = split_processing(addr2);
  float total_cos = 0, total_com = 0;
  for (string s : addr1_tokens) {
    float max_cos = 0, max_com = 0;
    for (string ss : addr2_tokens) {
      max_cos = max(max_cos, cos_similarity(s, ss));
      max_com = max(max_com, com_similarity(s, ss, eng_threshold));
    }
    total_cos += max_cos;
    total_com += max_com;
  }
  float ratio = 1 - min(addr1_tokens.size(), addr2_tokens.size()) * 1.0 / max(addr1_tokens.size(), addr2_tokens.size());
  int offset = (int) (ratio * addr1_tokens.size());
  float cos_score = total_cos / (addr1_tokens.size() + (enable_rand * (rand() % 2) * offset));
  float com_score = total_com / (addr1_tokens.size() + (enable_rand * (rand() % 2) * offset));
  return cos_score * eng_cos_weight + com_score * eng_com_weight;
}

/*--------------------------------------------------------------------------------------------------
 Show current settings of this AddressSimilarity object
 @return: None
 @params: None
--------------------------------------------------------------------------------------------------*/
void AddressSimilarity :: get_params() {
  fprintf(stderr, "CHINESE -> cos weight: %.4f, com weight: %.4f, threshold: %d\n", chs_cos_weight, chs_com_weight, chs_threshold);
  fprintf(stderr, "ENGLISH -> cos weight: %.4f, com weight: %.4f, threshold: %d\n", eng_cos_weight, eng_com_weight, eng_threshold);
}

/*--------------------------------------------------------------------------------------------------
 Edit settings of this AddressSimilarity object
 @return: None
 @params: float chs_cos_weight -> Default 0.5, Weight for cosine similarity of Chinese address
          float chs_com_weight -> Default 0.5, Weight for non-overlapping common substrings of Chinese address
          int chs_threshold -> Default 2, Threshold of non-overlapping common substrings of Chinese address (at least 1)
          float eng_cos_weight -> Default 0.3, Weight for cosine similarity of English address
          float eng_com_weight -> Default 0.7, Weight for non-overlapping common substrings of English address
          int eng_threshold -> Default 3, Threshold of non-overlapping common substrings of English address (at least 1)
--------------------------------------------------------------------------------------------------*/
void AddressSimilarity :: set_params(float chs_cos_weight, float chs_com_weight, int chs_threshold,
                                      float eng_cos_weight, float eng_com_weight, int eng_threshold) {
  this -> chs_cos_weight = chs_cos_weight;
  this -> chs_com_weight = chs_com_weight;
  this -> chs_threshold = chs_threshold ? chs_threshold : 1;
  this -> eng_cos_weight = eng_cos_weight;
  this -> eng_com_weight = eng_com_weight;
  this -> eng_threshold = eng_threshold ? eng_threshold : 1;
}