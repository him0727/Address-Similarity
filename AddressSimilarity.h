#ifndef _ADDRESSSIMILARITY_H
#define _ADDRESSSIMILARITY_H

using namespace std;

/*--------------------------------------------------------------------------------------------------
 Header file listing all variables and methods of AddressSimilarity class
--------------------------------------------------------------------------------------------------*/
class AddressSimilarity {
  private:
    
    float chs_cos_weight, chs_com_weight;
    float eng_cos_weight, eng_com_weight;
    int chs_threshold, eng_threshold;
    
  public:
  
    AddressSimilarity(float chs_cos_weight = 0.5, float chs_com_weight = 0.5, int chs_threshold = 2,
                      float eng_cos_weight = 0.3, float eng_com_weight = 0.7, int eng_threshold = 3);
    
    ~AddressSimilarity();
    
    float compare_addr(string raw_addr1, string raw_addr2, bool enable_rand = true);
    
    float compare_chs_addr(string raw_addr1, string raw_addr2);
    
    float compare_eng_addr(string raw_addr1, string raw_addr2, bool enable_rand = true);
    
    void get_params();
    
    void set_params(float chs_cos_weight = 0.5, float chs_com_weight = 0.5, int chs_threshold = 2,
                    float eng_cos_weight = 0.3, float eng_com_weight = 0.7, int eng_threshold = 3);
    
};

#endif