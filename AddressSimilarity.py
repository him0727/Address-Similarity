import math
import re

class addressSimilarity:

    def __init__(self, CHS_COS_WEIGHT = 0.5, CHS_COM_WEIGHT = 0.5, CHS_THRESHOLD = 2, ENG_COS_WEIGHT = 0.3, ENG_COM_WEIGHT = 0.7, ENG_THRESHOLD = 3):
        self.CHS_COS_WEIGHT = CHS_COS_WEIGHT
        self.CHS_COM_WEIGHT = CHS_COM_WEIGHT
        self.CHS_THRESHOLD = CHS_THRESHOLD
        self.ENG_COS_WEIGHT = ENG_COS_WEIGHT
        self.ENG_COM_WEIGHT = ENG_COM_WEIGHT
        self.ENG_THRESHOLD = ENG_THRESHOLD
            
    def arabic2Chinese_full(self, num):
        unit = ['', '十', '百', '千', '萬', '十萬', '百萬', '千萬', '億', '十億', '百億', '千億', '萬億']
        arabicNum = {'0':'零', '1':'一', '2':'二', '3':'三', '4':'四', '5':'五',
            '6':'六', '7':'七', '8':'八', '9':'九', '10':'十'}
        strNum = str(num)
        if num < 0 or num > 9999999999999:
            return num
        if num <= 10:
            return arabicNum[strNum]
        numLen = len(strNum)
        chineseNum = ''
        for i, numChar in enumerate(strNum):
            if numChar == '0' and strNum[i - 1] != '0' and i < len(strNum) - 1:
                chineseNum += arabicNum[numChar]
            elif numChar != '0':
                chineseNum += arabicNum[numChar]
                if not set(unit[numLen - 1]) & set(unit[numLen - 2]):
                    chineseNum += unit[numLen - 1]
                else:
                    if strNum[i + 1] == '0':
                        chineseNum += (unit[numLen - 1])
                    else:
                        chineseNum += (unit[numLen - 1])[0]
            numLen -= 1
        if (len(strNum) > 5 or len(strNum) == 2) and strNum[0] == '1':
            chineseNum = chineseNum[1:]
        return chineseNum[:-1] if num % 100 == 0 else chineseNum

    def arabic2Chinese_simple(self, num):
        arabicNum = {'0':'零', '1':'一', '2':'二', '3':'三', '4':'四', '5':'五',
            '6':'六', '7':'七', '8':'八', '9':'九', '10':'十'}
        chineseNum = ''
        for numChar in str(num):
            chineseNum += arabicNum[numChar]
        return chineseNum
      
    def preProcessing(self, rawAddress1, rawAddress2, escapeChars = [], replaceChars = {}):
        address1 = rawAddress1.strip().lower()
        address2 = rawAddress2.strip().lower()
        for escapeChar in escapeChars:
            address1 = address1.replace(escapeChar, '')
            address2 = address2.replace(escapeChar, '')
        for replaceChar in replaceChars:
            address1 = address1.replace(replaceChar, replaceChars[replaceChar])
            address2 = address2.replace(replaceChar, replaceChars[replaceChar])
        return [address1, address2]
        
    def cosSimilarity(self, address1, address2):
        if address1 == address2:
            return 1.0
        addr1Len, addr2Len = len(address1), len(address2)
        addr1Set, addr2Set = set(address1), set(address2)
        addrSet = addr1Set.union(addr2Set)
        addr1Count, addr2Count = [], []
        crossProduct, addr1Product, addr2Product = [], [], []
        for setChar in addrSet:
            i, j = 0, 0
            for addrChar in address1:
                if setChar == addrChar:
                    i += 1
            addr1Count.append(i)
            for addrChar in address2:
                if setChar == addrChar:
                    j += 1
            addr2Count.append(j)
        for i in range(len(addr1Count)):
            crossProduct.append(addr1Count[i] * addr2Count[i])
            addr1Product.append(addr1Count[i] * addr1Count[i])
            addr2Product.append(addr2Count[i] * addr2Count[i])
        return sum(crossProduct) / (math.sqrt(sum(addr1Product)) * math.sqrt(sum(addr2Product)))
       
    def comSimilarity(self, address1, address2, threshold):
        if ((address1.isalpha() and address2.isalpha()) or (address1.isdigit() and address2.isdigit())) and address1 == address2:
            return 1.0
        addr1Len, addr2Len = len(address1), len(address2)
        strMatrix = [[0] * addr2Len for _ in range(addr1Len)]
        for i in range(addr1Len):
            for j in range(addr2Len):
                if address1[i] == address2[j]:
                    if i == 0 or j == 0:
                        strMatrix[i][j] = 1
                        continue
                    strMatrix[i][j] = strMatrix[i - 1][j - 1] + 1
        score = 0
        i = addr1Len - 1 
        previous = 0 if threshold == 1 else 1
        while i > 0:
            j = addr2Len - 1
            while j > 0:
                if strMatrix[i][j] >= threshold and strMatrix[i - 1][j - 1] >= previous:
                    score += strMatrix[i][j]
                    i -= strMatrix[i][j] - 1
                    break
                j -= 1 
            i -= 1 
        return score / max(addr1Len, addr2Len)
        
    def compareChsAddress(self, rawAddress1, rawAddress2):
        escapeChars = [' ', '　', ',', '，', '.', '。',
            '號', '号', '鋪', '铺', '舖', '九龍', '九龙', '香港', '新界',
            '(', ')', '（', '）', '[', ']', '〔', '〕', '{', '}', '｛', '｝', '|', '｜', '+', '＋',
            '/', '／', '\\', '＼', '~', '～', ';', '；', '*', '＊', '@', '＠', '\'', '’', '"', '”',
            '#', '＃', '!', '！', '$', '＄', '%', '％', '<', '＜', '>', '＞', '^', '︿', '=', '＝', '、']
        replaceChars = {'-':'至', '—':'至', 'o':'零', 'ｏ':'零', 'Ｏ':'零', '&':'及', '＆':'及'}
        address1 = self.preProcessing(rawAddress1, rawAddress2, escapeChars, replaceChars)[0]
        address2 = self.preProcessing(rawAddress1, rawAddress2, escapeChars, replaceChars)[1]
        
        def toChineseNum(address1, address2):
            for num in re.findall('\d+', address1):
                chineseNum1 = self.arabic2Chinese_full(int(num))
                chineseNum2 = self.arabic2Chinese_simple(int(num))
                address1 = address1.replace(num, chineseNum1) if chineseNum1 in address2 else address1.replace(num, chineseNum2)
            return address1
        address1 = toChineseNum(address1, address2)
        address2 = toChineseNum(address2, address1)
        
        cosScore = self.cosSimilarity(address1, address2)
        commonScore = self.comSimilarity(address1, address2, self.CHS_THRESHOLD)

        return cosScore * self.CHS_COS_WEIGHT + commonScore * self.CHS_COM_WEIGHT

    def compareEngAddress(self, rawAddress1, rawAddress2):
        escapeChars = ['no.', '-', '—', '.', '。', ',', '，', 
            'nt', 'kln', 'kl', 'hk', 'hong kong', 'kowloon', 'new territories',
            '(', ')', '（', '）', '[', ']', '〔', '〕', '{', '}', '｛', '｝', '|', '｜', '+', '＋',
            '/', '／', '\\', '＼', '~', '～', ';', '；', '*', '＊', '&', '＆', '@', '＠', '\'', '’', '"', '”',
            '#', '＃', '!', '！', '$', '＄', '%', '％', '<', '＜', '>', '＞', '^', '︿', '=', '＝', '、']
        address1 = self.preProcessing(rawAddress1, rawAddress2, escapeChars)[0]
        address2 = self.preProcessing(rawAddress1, rawAddress2, escapeChars)[1]

        addr1 = re.findall('\d+', address1)
        addr2 = re.findall('\d+', address2)
        addr1.extend(re.findall('[a-zA-Z]+', address1))
        addr2.extend(re.findall('[a-zA-Z]+', address2))
        totalCos = 0
        totalCom = 0
        for addr1Word in addr1:
            maxCom = 0
            maxCos = 0
            for addr2Word in addr2:
                maxCos = max(maxCos, self.cosSimilarity(addr1Word, addr2Word))
                maxCom = max(maxCom, self.comSimilarity(addr1Word, addr2Word, self.ENG_THRESHOLD))
            totalCos += maxCos
            totalCom += maxCom
        
        cosScore = totalCos / len(addr1)
        commonScore = totalCom / len(addr1)

        return cosScore * self.ENG_COS_WEIGHT + commonScore * self.ENG_COM_WEIGHT

