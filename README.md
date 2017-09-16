# Address-Similarity-Hong-Kong-
A python module for calculating the similarity of two given addresses either in Chinese or English.

## Features
- Support Chinese address and English address
- Address pre-processing to increase accuracy 
- Customized weight and threshold of consine similarity and non-overlapping common substrings

## Guideline
Use `import AddressSimilarity` to import the module.

Construct `AddressSimilarity.addressSimilarity(CHS_COS_WEIGHT, CHS_COM_WEIGHT, CHS_THRESHOLD, ENG_COS_WEIGHT, ENG_COM_WEIGHT, ENG_THRESHOLD )` object, parameters in order are costomized weight of consine similarity and non-overlapping common substrings, threshold of minimum length of common substrings in Chinese, the same followed by English. Default as `CHS_COS_WEIGHT = 0.5, CHS_COM_WEIGHT = 0.5, CHS_THRESHOLD = 2, ENG_COS_WEIGHT = 0.3, ENG_COM_WEIGHT = 0.7, ENG_THRESHOLD = 3`.

`compareChsAddress(address1, address2)` and `compareEngAddress(address1, address2)` for Chinese and English address, respectively. Return is the percentage of similarity, with 1 as totally the same and 0 as totally different.

## Algorithm
This algorithm is based on two criteria: 1. Cosine similarity (COS) & 2. Non-overlapping common substrings (COM), final similarity is computed using this formula: *COS \* COS_WEIGHT + COM \* COM_WEIGHT*.

For English address, address is splitted by whitespacee and all numbers are extracted. Then, compute the maximum cosine and non-overlapping commmon substrings similarity of each token of address1, using the current token of address1 and all tokens of address2. Lastly the sum of them divided by the length of address1.

- Address pre-processing
  - Convert to lowercase
  - Escape special characters, usual words, regions in Hong Kong of address
  - Replace some meaningful characters eg. "-" : "至", "&" : "及"
  - For Chinese address, convert arabic number to chinese number. If chinese number representation found in address 2, detecting which display format it is, like "一二三四" (simple) vs "一千二百三十四" (full) and the numbers in address 1 will be converted to corresponding format. If not, both are converted to full format. Eg. "2017 & 二Ｏ一七" : "二零一七", "190 & 190" : "一百九十", "33 & 三十三" : "三十三"

- Cosine similarity
  - Refer to https://en.wikipedia.org/wiki/Cosine_similarity
  
- Non-overlapping common substrings
  - Implemented by dynamic programming
  - Form a string matrix `strMatrix` of two given addresses, storing the length of all common substrings
  - Iterate backwards from bottom right `strMaxtrix[i][j]` to `strMaxtrix[1][1]`, where `i` and `j` are the length of address1 - 1 and the length of address2 - 1, respectively. The current value will be added to score if and only if: 
    - For threshold == 1
    
      `strMatrix[i][j] >= threshold and strMatrix[i - 1][j - 1] >= 1`
    - For threshold > 1
    
      `strMatrix[i][j] >= threshold and strMatrix[i - 1][j - 1] >= 0`
    
    Then, `i` is substracted by the added value and `j` is set to the length of address2 - 1 again, to avoid counting the lenght of overlapping substrings
  - Final result is the ratio of final score and the maximum length of two given addresses, which is computed by `score / max(len(address1), len(address2))`
  - For English address, since the method for computing non-overlapping common substrings similarity is token-based, an exception is considered when the length of token is less than threshold. When it happens, the maximum similarity of a token will be 1.0 if both are the same and of the same type: `((address1.isalpha() and address2.isalpha()) or (address1.isdigit() and address2.isdigit())) and address1 == address2`

--------------------------------------
 @HIM0727 https://him0727.github.io/
