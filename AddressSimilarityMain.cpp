#include "AddressSimilarityMain.h"

static const std::vector<std::string> ESCAPE_CHARS = {
  ",", "，", ".", "。", "(", ")", "（", "）", "[", "]", "〔", "〕",
  "{", "}", "｛", "｝", "|", "｜", "+", "＋", "/", "／", "\\", "＼",
  "~", "～", ";", "；", "*", "＊", "@", "＠", "\"", "’", "”", "#", "＃",
  "!", "！", "$", "＄", "%", "％", "<", "＜", ">", "＞", "^", "︿", "=", "＝", "、"};

static const std::vector<std::string> CHS_ESCAPE_CHARS = {
  " ", "　", ",", "，", ".", "。",
  "號", "号", "鋪", "铺", "舖", "九龍", "九龙", "香港", "新界"};

static const std::vector<std::pair<std::string, std::string>> CHS_REPLACE_CHARS = {
  {"-", "至"}, {"—", "至"}, {"o", "零"}, {"ｏ", "零"}, {"Ｏ", "零"}, {"&", "及"}, {"＆", "及"}};

static const std::vector<std::string> ENG_ESCAPE_CHARS = {
  "no.", "-", "—", " nt", " kln", " kl", "hk", "hong kong", "kowloon", "new territories"};
                                  
static const std::vector<std::pair<std::string, std::string>> ENG_REPLACE_CHARS = {
  {"south", "s."}, {"north", "n."}, {"east", "e."}, {"west", "w."}, 
  {"street", "st."}, {"road", "rd."}, {"avenue", "ave."}, {"lane", "ln."}, {"terrace", "ter."}};

/**
 * Preprocess two given addresses by removing whitespace, changing to lowercase letters, 
 * replacing and escaping specified characters.
 * @param address1 (string) First address
 * @param address2 (string) Second address
 * @param is_chs (bool) Preprocess the chinese address or not
 * @return Two processed addresses (pointer of string array)
 */
std::string *pre_processing(std::string address1, std::string address2, const bool is_chs) {
  StringTools::strip(address1), StringTools::strip(address2);
  StringTools::lower(address1), StringTools::lower(address2);
  for (auto e : ESCAPE_CHARS) {
    StringTools::replace(address1, e, "");
    StringTools::replace(address2, e, "");
  }
  for (auto e : (is_chs ? CHS_ESCAPE_CHARS : ENG_ESCAPE_CHARS)) {
    StringTools::replace(address1, e, "");
    StringTools::replace(address2, e, "");
  }
  for (auto r : (is_chs ? CHS_REPLACE_CHARS : ENG_REPLACE_CHARS)) {
    StringTools::replace(address1, r.first, r.second);
    StringTools::replace(address2, r.first, r.second);
  }
  std::string *res = new std::string[2];
  res[0] = address1, res[1] = address2;
  return res;
}

/**
 * Split the english address into list of tokens by english alphabet and number.
 * @param address (string) English address
 * @return List of tokens (string vector)
 */
std::vector<std::string> split_processing(const std::string &address) {
  std::vector<std::string> tokens;
  std::regex rx_d(R"(([[:digit:]]+))");
  std::smatch sm;
  std::string str = address;
  while (std::regex_search(str, sm, rx_d)) {
    tokens.push_back(sm[0]);
    str = sm.suffix().str();
  }
  std::regex rx_s(R"(([a-zA-Z]+))");
  str = address;
  while (std::regex_search(str, sm, rx_s)) {
    tokens.push_back(sm[0]);
    str = sm.suffix().str();
  }
  return tokens;
}

/**
 * Calculate the cosine similarity of two given addresses.
 * @param raw_address1 (string) First address
 * @param raw_address2 (string) Second address
 * @return cosine similarity ranged from 0 to 1 (float)
 */
float cos_similarity(std::string raw_address1, std::string raw_address2) {
  if (raw_address1 == raw_address2) {
    return 1.0;
  }
  if ((int) raw_address1.size() == 0 || (int) raw_address2.size() == 0) {
    return 0;
  }
  std::set<wchar_t> address1_set, address2_set, address_set;
  std::set<wchar_t>::iterator itr;
  std::wstring address1 = StringTools::utf8_to_wstring(raw_address1);
  std::wstring address2 = StringTools::utf8_to_wstring(raw_address2);
  for (wchar_t c : address1) {
    address1_set.insert(c);
  }
  for (wchar_t c : address2) {
    address2_set.insert(c);
  }
  std::set_union(address1_set.begin(), address1_set.end(), address2_set.begin(), address2_set.end(), std::inserter(address_set, address_set.end()));
  std::vector<float> address1_count, address2_count;
  for (auto itr : address_set) {
    address1_count.push_back(std::count(address1.begin(), address1.end(), itr));
    address2_count.push_back(std::count(address2.begin(), address2.end(), itr));
  }
  float address1_sum = 0, address2_sum = 0, cross_sum = 0;
  for (int i = 0; i < (int) address1_count.size(); i++) {
    cross_sum += address1_count[i] * address2_count[i];
    address1_sum += address1_count[i] * address1_count[i];
    address2_sum += address2_count[i] * address2_count[i];
  }
  float diff = std::sqrt(address1_sum) * std::sqrt(address2_sum);
  return diff == 0 ? 0 : cross_sum / diff;
}

/**
 * Calculate the non-overlapping common substrings similarity of two given addresses based on the threshold.
 * @param raw_address1 (string) First address
 * @param raw_address2 (string) Second address
 * @param threshold (int) Number of characters to be considered valid for length of substring
 * @return non-overlapping common substrings similarity ranged from 0 to 1 (float)
 */
float com_similarity(std::string raw_address1, std::string raw_address2, int threshold) {
  if (raw_address1 == raw_address2) {
    return 1.0;
  }
  if ((int) raw_address1.size() == 0 || (int) raw_address2.size() == 0) {
    return 0;
  }
  std::wstring address1 = StringTools::utf8_to_wstring(raw_address1);
  std::wstring address2 = StringTools::utf8_to_wstring(raw_address2);
  int address1_sz = (int) address1.size();
  int address2_sz = (int) address2.size();
  std::vector<std::vector<float>> val(address1_sz + 1, std::vector<float>(address2_sz + 1, 0));
  for (int i = 1; i <= address1_sz; i++) {
    for (int j = 1; j <= address2_sz; j++) {
      if (address1[i - 1] == address2[j - 1]) {
        val[i][j] = val[i - 1][j - 1] + 1;
      }
    }
  }
  float score = 0;
  int i = address1_sz;
  int previous = threshold != 1;
  while (i > 0) {
    int j = address2_sz;
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
  return score / std::max(address1_sz, address2_sz);
}

/**
 * Constructor to instantize AddressSimilarity class.
 * @param chs_cos_weight (float) Weight for cosine similarity of Chinese address
 * @param chs_com_weight (float) Weight for non-overlapping common substrings of Chinese address
 * @param chs_threshold (int) Threshold of non-overlapping common substrings of Chinese address
 * @param eng_cos_weight (float) Weight for cosine similarity of English address
 * @param eng_com_weight (float) Weight for non-overlapping common substrings of English address
 * @param eng_threshold (int) Threshold of non-overlapping common substrings of English address
 * @return AddressSimilarity instance (AddressSimilarity)
 */
AddressSimilarity::AddressSimilarity(float chs_cos_weight, float chs_com_weight, int chs_threshold,
                                     float eng_cos_weight, float eng_com_weight, int eng_threshold) {
  _chs_cos_weight = chs_cos_weight == -1 ? _chs_cos_weight : chs_cos_weight;
  _chs_com_weight = chs_com_weight == -1 ? _chs_com_weight : chs_com_weight;
  _chs_threshold = chs_threshold == -1 ? _chs_threshold : chs_threshold;
  _eng_cos_weight = eng_cos_weight == -1 ? _eng_cos_weight : eng_cos_weight;
  _eng_com_weight = eng_com_weight == -1 ? _eng_com_weight : eng_com_weight;
  _eng_threshold = eng_threshold == -1 ? _eng_threshold : eng_threshold;
}

/**
 * Destructor.
 * @return: None
 */
AddressSimilarity::~AddressSimilarity() {}

/**
 * Show the configuration of AddressSimilarity instance.
 * @return: None
 */
void AddressSimilarity::get_params() {
  fprintf(stderr, "CHINESE -> cos weight: %.4f, com weight: %.4f, threshold: %d\n", _chs_cos_weight, _chs_com_weight, _chs_threshold);
  fprintf(stderr, "ENGLISH -> cos weight: %.4f, com weight: %.4f, threshold: %d\n", _eng_cos_weight, _eng_com_weight, _eng_threshold);
}

/**
 * Calculate the total similarity of two given addresses by auto-detecting language.
 * @param raw_address1 (string) First address
 * @param raw_address2 (string) Second address
 * @return total similarity ranged from 0 to 1 (float)
 */
float AddressSimilarity::compare_address(std::string raw_address1, std::string raw_address2) {
  if (raw_address1 == raw_address2) {
    return 1.0;
  }
  if ((int) raw_address1.size() == 0 || (int) raw_address2.size() == 0) {
    return 0;
  }
  std::string tmp_address1 = raw_address1;
  std::string tmp_address2 = raw_address2;
  StringTools::strip(tmp_address1), StringTools::strip(tmp_address2);
  for (auto e : ESCAPE_CHARS) {
    StringTools::replace(tmp_address1, e, "");
    StringTools::replace(tmp_address2, e, "");
  }
  int d_address1_count = 0, d_address2_count = 0;
  int a_address1_count = 0, a_address2_count = 0;
  for (auto c : tmp_address1) {
    if (c - '0' >= 0 && c - '0' <= 9) {
      d_address1_count++;
    } else if ((c - 'a' >= 0 && c - 'a' < 26) || (c - 'A' >= 0 && c - 'A' < 26)) {
      a_address1_count++;
    }
  }
  for (auto c : tmp_address2) {
    if (c - '0' >= 0 && c - '0' <= 9) {
      d_address2_count++;
    } else if ((c - 'a' >= 0 && c - 'a' < 26) || (c - 'A' >= 0 && c - 'A' < 26)) {
      a_address2_count++;
    }
  }
  bool address1_is_chs = (int) tmp_address1.size() - d_address1_count > a_address1_count * 2;
  bool address2_is_chs = (int) tmp_address2.size() - d_address2_count > a_address2_count * 2;
  if (address1_is_chs && address2_is_chs) {
    return compare_chs_address(raw_address1, raw_address2);
  }
  if (!address1_is_chs && !address2_is_chs) {
    return compare_eng_address(raw_address1, raw_address2);
  }
  return 0;
}

/**
 * Calculate the total similarity of two given Chinese addresses 
 * by preprocessing and coverting the number to Chinese number representation.
 * @param raw_address1 (string) First address
 * @param raw_address2 (string) Second address
 * @return total similarity ranged from 0 to 1 (float)
 */
float AddressSimilarity::compare_chs_address(std::string raw_address1, std::string raw_address2) {
  std::string *address = pre_processing(raw_address1, raw_address2, true);
  if ((int) address[0].size() == 0 || (int) address[1].size() == 0) {
    return 0;
  }
  address[0] = NumberConverter::to_chs_num(address[0], address[1]);
  address[1] = NumberConverter::to_chs_num(address[1], address[0]);
  float cos_score = cos_similarity(address[0], address[1]);
  float com_score = com_similarity(address[0], address[1], _chs_threshold);
  return cos_score * _chs_cos_weight + com_score * _chs_com_weight;
}

/**
 * Calculate the total similarity of two given English addresses by preprocessing and splitting into tokens, 
 * with penalty proportional the difference between size of two lists of tokens and difference between cosine and common substring similarity.
 * @param raw_address1 (string) First address
 * @param raw_address2 (string) Second address
 * @return total similarity ranged from 0 to 1 (float)
 */
float AddressSimilarity::compare_eng_address(std::string raw_address1, std::string raw_address2) {
  std::string *address = pre_processing(raw_address1, raw_address2, false);
  if ((int) address[0].size() == 0 || (int) address[1].size() == 0) {
    return 0;
  }
  std::vector<std::string> address1_tokens = split_processing(address[0]);
  std::vector<std::string> address2_tokens = split_processing(address[1]);
  float total_cos = 0, total_com = 0;
  for (std::string s : address1_tokens) {
    float max_cos = 0, max_com = 0;
    for (std::string ss : address2_tokens) {
      max_cos = std::max(max_cos, cos_similarity(s, ss));
      max_com = std::max(max_com, com_similarity(s, ss, _eng_threshold));
    }
    total_cos += max_cos;
    total_com += max_com;
  }
  int address1_token_sz = (int) address1_tokens.size();
  int address2_token_sz = (int) address2_tokens.size();
  float ratio = 1 - std::min(address1_token_sz, address2_token_sz) * 1.0 / std::max(address1_token_sz, address2_token_sz);
  float cos_score = total_cos / address1_token_sz;
  float com_score = total_com / address1_token_sz;
  float penalty = fabs(cos_score - com_score) * ratio;
  float final_score = cos_score * _eng_cos_weight + com_score * _eng_com_weight - penalty;
  if (final_score < 0.0) {
    return 0.0;
  }
  return final_score;
}
