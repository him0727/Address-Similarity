#include "AddressSimilarityBuilder.h"

AddressSimilarityBuilder::AddressSimilarityBuilder() {}

/**
 * Set weight for cosine similarity of Chinese address, weight for non-overlapping common substrings of Chinese address
 * will be set to 1 - chs_cos_weight automatically so that the sum of weights equals 1.
 * @param chs_cos_weight (float) Weight for cosine similarity of Chinese address
 * @return AddressSimilarityBuilder instance (pointer of AddressSimilarityBuilder instance)
 */
AddressSimilarityBuilder& AddressSimilarityBuilder::chs_cos_weight(float chs_cos_weight) {
  _chs_cos_weight = chs_cos_weight;
  _chs_com_weight = 1 - chs_cos_weight;
  return *this;
}

/**
 * Set weight for non-overlapping common substrings of Chinese address, weight for cosine similarity of Chinese address
 * will be set to 1 - chs_com_weight automatically so that the sum of weights equals 1.
 * @param chs_com_weight (float) Weight for non-overlapping common substrings of Chinese address
 * @return AddressSimilarityBuilder instance (pointer of AddressSimilarityBuilder instance)
 */
AddressSimilarityBuilder& AddressSimilarityBuilder::chs_com_weight(float chs_com_weight) {
  _chs_com_weight = chs_com_weight;
  _chs_cos_weight = 1 - chs_com_weight;
  return *this;
}

/**
 * Set threshold of non-overlapping common substrings of Chinese address.
 * @param chs_threshold (int) Threshold of non-overlapping common substrings of Chinese address
 * @return AddressSimilarityBuilder instance (pointer of AddressSimilarityBuilder instance)
 */
AddressSimilarityBuilder& AddressSimilarityBuilder::chs_threshold(int chs_threshold) {
  _chs_threshold = chs_threshold;
  return *this;
}

/**
 * Set weight for cosine similarity of English address, weight for non-overlapping common substrings of English address
 * will be set to 1 - eng_cos_weight automatically so that the sum of weights equals 1.
 * @param eng_cos_weight (float) Weight for cosine similarity of English address
 * @return AddressSimilarityBuilder instance (pointer of AddressSimilarityBuilder instance)
 */
AddressSimilarityBuilder& AddressSimilarityBuilder::eng_cos_weight(float eng_cos_weight) {
  _eng_cos_weight = eng_cos_weight;
  _eng_com_weight = 1 - eng_cos_weight;
  return *this;
}

/**
 * Set weight for non-overlapping common substrings of English address, weight for cosine similarity of English address
 * will be set to 1 - eng_com_weight automatically so that the sum of weights equals 1.
 * @param eng_com_weight (float) Weight for non-overlapping common substrings of English address
 * @return AddressSimilarityBuilder instance (pointer of AddressSimilarityBuilder instance)
 */
AddressSimilarityBuilder& AddressSimilarityBuilder::eng_com_weight(float eng_com_weight) {
  _eng_com_weight = eng_com_weight;
  _eng_cos_weight = 1 - eng_com_weight;
  return *this;
}

/**
 * Set threshold of non-overlapping common substrings of English address.
 * @param eng_threshold (int) Threshold of non-overlapping common substrings of English address
 * @return AddressSimilarityBuilder instance (pointer of AddressSimilarityBuilder instance)
 */
AddressSimilarityBuilder& AddressSimilarityBuilder::eng_threshold(int eng_threshold) {
  _eng_threshold = eng_threshold;
  return *this;
}

/**
 * Instantize AddressSimilarity class by existing parameters.
 * @return AddressSimilarity instance (pointer of AddressSimilarity instance)
 */
AddressSimilarity& AddressSimilarityBuilder::build() {
  AddressSimilarity* addressSimilarity = new AddressSimilarity(_chs_cos_weight, _chs_com_weight, _chs_threshold, 
                                                               _eng_cos_weight, _eng_com_weight, _eng_threshold);
  return *addressSimilarity;
}