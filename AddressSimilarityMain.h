#ifndef _ADDRESSSIMILARITYMAIN_H
#define _ADDRESSSIMILARITYMAIN_H
#include "StringTools.h"
#include "NumberConverter.h"
#include "OutOfRangeException.h"
#include <stdio.h>
#include <vector>
#include <cstddef>
#include <cmath>
#include <algorithm>
#include <set>

/*
 * This header file declares the public methods for AddressSimilarity class, which is the public APIs called by user. 
 * Default settings for Chinese and English address are encapsulated, which can be found in private scope. 
 * Variables follow the expression _[chs|eng]_[cos_weight|com_weight|threshold], 
 * where [chs|eng] stands for Chinese or English address, [cos|com] is cosine similarity weight and non-overlapping common substrings similarity, 
 * and threshold only applicable to common substrings similarity, indicating the minimum length of common substring to be considered valid.
 * For the implementation and comments of the methods, refer to AddressSimilarityMain.cpp.
 */
class AddressSimilarity {
 
  private:
    float _chs_cos_weight = 0.5;
    float _chs_com_weight = 0.5;
    float _eng_cos_weight = 0.3;
    float _eng_com_weight = 0.7;
    int _chs_threshold = 2;
    int _eng_threshold = 3;
    
  public:
    AddressSimilarity(float chs_cos_weight = 0.5, float chs_com_weight = 0.5, int chs_threshold = 2,
                      float eng_cos_weight = 0.3, float eng_com_weight = 0.7, int eng_threshold = 3);
    
    ~AddressSimilarity();
    
    float compare_address(std::string raw_address1, std::string raw_address2);
    
    float compare_chs_address(std::string raw_address1, std::string raw_address2);
    
    float compare_eng_address(std::string raw_address1, std::string raw_address2);
    
    void get_params();
    
    float get_chs_cos_weight() {
      return _chs_cos_weight;
    }
    
    void set_chs_cos_weight(float chs_cos_weight) {
      if (chs_cos_weight < 0 || chs_cos_weight > 1) {
        throw OutOfRangeException("chs_cos_weight", chs_cos_weight);
      }
      _chs_cos_weight = chs_cos_weight;
      _chs_com_weight = 1 - chs_cos_weight;
    }
    
    float get_chs_com_weight() {
      return _chs_com_weight;
    }
    
    void set_chs_com_weight(float chs_com_weight) {
      if (chs_com_weight < 0 || chs_com_weight > 1) {
        throw OutOfRangeException("chs_com_weight", chs_com_weight);
      }
      _chs_com_weight = chs_com_weight;
      _chs_cos_weight = 1 - chs_com_weight;
    }
    
    float get_eng_cos_weight() {
      return _eng_cos_weight;
    }
    
    void set_eng_cos_weight(float eng_cos_weight) {
      if (eng_cos_weight < 0 || eng_cos_weight > 1) {
        throw OutOfRangeException("eng_cos_weight", eng_cos_weight);
      }
      _eng_cos_weight = eng_cos_weight;
      _eng_com_weight = 1 - eng_cos_weight;
    }
    
    float get_eng_com_weight() {
      return _eng_com_weight;
    }
    
    void set_eng_com_weight(float eng_com_weight) {
      if (eng_com_weight < 0 || eng_com_weight > 1) {
        throw OutOfRangeException("eng_com_weight", eng_com_weight);
      }
      _eng_com_weight = eng_com_weight;
      _eng_cos_weight = 1 - eng_com_weight;
    }
    
    int get_chs_threshold() {
      return _chs_threshold;
    }
    
    void set_chs_threshold(int chs_threshold) {
      if (chs_threshold < 1) {
        throw OutOfRangeException("chs_threshold", chs_threshold);
      }
      _chs_threshold = chs_threshold;
    }
    
    int get_eng_threshold() {
      return _eng_threshold;
    }
    
    void set_eng_threshold(int eng_threshold) {
      if (eng_threshold < 1) {
        throw OutOfRangeException("eng_threshold", eng_threshold);
      }
      _eng_threshold = eng_threshold;
    }
    
};

#endif