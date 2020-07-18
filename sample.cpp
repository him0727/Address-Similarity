#include <bits/stdc++.h>
#include "AddressSimilarity.h"

using namespace std;

// g++ -o sample.exe sample.cpp AddressSimilarityMain.cpp AddressSimilarityBuilder.cpp ASUtils/StringTools.cpp ASUtils/NumberConverter.cpp ASExceptions/OutOfRangeException.cpp

const vector<vector<string>> chs_addrs = {
  {"香港島中西區摩星嶺1號", "香港島中西區摩星嶺1號"},
  {"香港九龍元朗站3號客運大樓", "九龍元朗站三號客運大樓"},
  {"九龍紅磡理工大學", "九龍紅磡灣育才道11號"},
  {"新界沙田沙角街8號舖", "沙田大圍沙角街八號"},
  {"香港新界老圍村一街1號3座地下", "新界老圍村1街第3座地下"},
  {"旺角西洋菜南街一百九十號西洋大廈地下190-192B", "香港九龍西洋菜街190號第190至192B號"},
  {"香港九龍深水埗深旺道1號巴士站", "九龍彩虹巴士總站"},
  {"新界屯門海皇路海皇商場9樓902號舖", "新界屯門V City 09樓902號"},
  {"九龍尖沙咀寶勒巷10號10 PRAT 6樓", "九龍尖沙咀寶勒巷10號寶勒10 6樓"},
  {"九龍尖沙咀寶勒巷五號30樓F室", "九龍尖沙咀寶勒巷5號30F"},
  {"新蒲崗錦榮街333號1號A - C", "九龍新蒲崗康強街223號地下1A-1B號舖"},
  {"香港島小西灣富欣花園商場地下1號", "香港島銅鑼灣東角島一號地下"}
};
vector<vector<string>> eng_addrs = {
  {"Mount Davis Bus Terminus, HK", "Mount Davis Bus Terminus, HK"},
  {"No. 1, Mount Davis Bus Terminus, Hong Kong", "No. 1, Mount Davis Bus Terminus"},
  {"11 Yuk Choi Rd, Hung Hom", "No. 11, PolyU, Yuk Choi Road, Hung Hom, Kowloon"},
  {"1A, 1/F Fu Cheong Shopping Mall, Sham Mong Road, Sham Shui Po, HK", "Shop 1A, Fu Cheong Shopping Mall, Fu Cheong Estate, Sham Shui Po, KLN"},
  {"ABERDEEN. G01-02, Port Centre, Aberdeen, Hong Kong", "G01 - G02, G/F, Aberdeen Port Center, HK island, HK"},
  {"1/F, Yee Hing Building, No. 19 Leighton Road, Causeway Bay", "Floor 1, YeeHing Building, 19 Leighton Road, Causeway Bay, Hong Kong"},
  {"Shop 119, 2/F, Kwai Chung Plaza, 7-11 Kwai Foo Road", "No. 119, Floor 2, Kwai Chung Plaza, 7 - 11 Kwai Foo Rd., Kwai Chung, N.T."},
  {"Tin Shui Wai Station, Exit A, New Territories", "Exit A, Tin Shui stn., NT"},
  {"15/F., Sing Tao News Corporation Building, 3 Tung Wong Road, Shau Kei Wan", "1/F., 8 Chun Ying Street, Tseung Kwan O Industrial Estate West, N.T., Hong Kong"},
  {"LG2, High Court, 38 Queensway, Hong Kong.", "Shop 3E-08, Level 3 /F, Sunshine City Plaza, Ma On Shan, New Territories."},
  {"Canossa Hospital (Caritas}, 1 Old Peak Road", "Caritas Medical Centre, 111 Wing Hong Street, Sham Shui Po"},
  {"3/F, Tung Che Commercial Centre, 246 Des Voeux Road West, Sai Ying Pun", "7/F, To Kwa Wan Government Offices, 165 Ma Tau Wai Road, To Kwa Wan"}
};

void print_result(AddressSimilarity &comparer, const vector<vector<string>> &addrs, string lang = "") {
  for (int i = 0; i < (int) addrs.size(); i++) {
    cout << "******************************* RESULT *******************************" << '\n';
    cout << "Address 1: " << addrs[i][0] << endl;
    cout << "Address 2: " << addrs[i][1] << endl;
    comparer.set_chs_cos_weight(1);
    comparer.set_chs_threshold(1);
    comparer.set_eng_cos_weight(1);
    comparer.set_eng_threshold(1);
    cout << "Cosine similarity score: ";
    if (lang == "CHS") cout << comparer.compare_chs_address(addrs[i][0], addrs[i][1]) << '\n';
    if (lang == "ENG") cout << comparer.compare_eng_address(addrs[i][0], addrs[i][1]) << '\n';
    if (lang == "") cout << comparer.compare_address(addrs[i][0], addrs[i][1]) << '\n';
    comparer.set_chs_cos_weight(0);
    comparer.set_chs_threshold(2);
    comparer.set_eng_com_weight(1);
    comparer.set_eng_threshold(3);
    cout << "Common substring score: ";
    if (lang == "CHS") cout << comparer.compare_chs_address(addrs[i][0], addrs[i][1]) << '\n';
    if (lang == "ENG") cout << comparer.compare_eng_address(addrs[i][0], addrs[i][1]) << '\n';
    if (lang == "") cout << comparer.compare_address(addrs[i][0], addrs[i][1]) << '\n';
    comparer.set_chs_cos_weight(0.5);
    comparer.set_chs_threshold(2);
    comparer.set_eng_cos_weight(0.3);
    comparer.set_eng_threshold(3);
    cout << "Total similarity: ";
    if (lang == "CHS") cout << comparer.compare_chs_address(addrs[i][0], addrs[i][1]) << '\n';
    if (lang == "ENG") cout << comparer.compare_eng_address(addrs[i][0], addrs[i][1]) << '\n';
    if (lang == "") cout << comparer.compare_address(addrs[i][0], addrs[i][1]) << '\n';
    cout << "**********************************************************************" << "\n\n";
  }
}

int main() {
  AddressSimilarityBuilder addressSimilarityBuilder;
  AddressSimilarity comparer = addressSimilarityBuilder
    .chs_cos_weight(0.85)
    .eng_com_weight(0.4)
    .eng_threshold(5)
    .build();
  comparer.get_params();
  print_result(comparer, chs_addrs);
  print_result(comparer, eng_addrs);
  return 0;
}