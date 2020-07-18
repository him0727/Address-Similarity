#ifndef _ADDRESSSIMILARITYBUILDER_H
#define _ADDRESSSIMILARITYBUILDER_H

#include "AddressSimilarityMain.h"

/*
 * This header file declares the public methods for AddressSimilarityBuilder class, which is the public APIs called by user. 
 * Builder pattern is used to instantiate AddressSimilarity class.
 * For the more details, refer to AddressSimilarityBuilder.cpp, AddressSimilarityMain.h and AddressSimilarityMain.cpp.
 */
class AddressSimilarityBuilder {
 
  private:
    float _chs_cos_weight = -1;
    float _chs_com_weight = -1;
    float _eng_cos_weight = -1;
    float _eng_com_weight = -1;
    int _chs_threshold = -1;
    int _eng_threshold = -1;
    
  public:
    AddressSimilarityBuilder();
    AddressSimilarityBuilder& chs_cos_weight(float chs_cos_weight);
    AddressSimilarityBuilder& chs_com_weight(float chs_com_weight);
    AddressSimilarityBuilder& eng_cos_weight(float eng_cos_weight);
    AddressSimilarityBuilder& eng_com_weight(float eng_com_weight);
    AddressSimilarityBuilder& chs_threshold(int chs_threshold);
    AddressSimilarityBuilder& eng_threshold(int eng_threshold);
    AddressSimilarity& build();
  
};

#endif