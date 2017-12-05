#ifndef _ADDRESSSIMILARITY_H
#define _ADDRESSSIMILARITY_H

using namespace std;

class AddressSimilarity {
  private:
    
    float chs_cos_weight, chs_com_weight;
    float eng_cos_weight, eng_com_weight;
    int chs_threshold, eng_threshold;
    
  public:
    
    AddressSimilarity(float ccsw = 0.5, float ccmw = 0.5, int ct = 2, float ecsw = 0.3, float ecmw = 0.7, int et = 3);
    
    float compare_chs_addr(string raw_addr1, string raw_addr2);
    
    float compare_eng_addr(string raw_addr1, string raw_addr2);
    
    void get_params();
    
    void set_params(float ccsw = 0.5, float ccmw = 0.5, int ct = 2, float ecsw = 0.3, float ecmw = 0.7, int et = 3);
    
};

#endif