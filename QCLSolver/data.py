import QCLSolver.core as clucore
import copy
from math import floor
import types
import functools


class Data:
    def __init__(self, H, Co_ps, trackList, maxOPLen):
        """
        :param trackList: tracking operator的字符形式HO列表
        :param maxOPLen: 最大operator长度
        """
        # deriveData的存储
        self.__deriveData = None
        # 从数字到字母的映射表
        self.__numToLetterMap = [None] * 53
        # 从字母到数字的映射表
        self.__letterToNumMap = [None] * 52
        # 数字形态的Hamilton operator列表
        self.__numHOList = []
        # 数字形态的Collapse operator列表
        self.__numCOList = []
        # 现在的Hamilton operator列表系数表
        self.__rawCoefHOList = []
        # 现在的Collapse operator列表系数表
        self.__rawCoefCOList = []
        # 现在的Hamilton operator列表系数表
        self.__coefHOList = []
        # 现在的Collapse operator列表系数表
        self.__coefCOList = []
        # tracking operator列表
        self.__numTrackList = []
        # 最大算符长度
        self.__maxOPLen = maxOPLen

        # 检查输入正确性, 并返回raw数据
        (HOList, self.__rawCoefHOList, COList, self.__rawCoefCOList) = \
            Data.__CheckAndParse(H, Co_ps, trackList, maxOPLen)

        self.__CreateNumLetterMap(HOList, COList, trackList)
        (self.__numHOList, self.__numCOList, self.__numTrackList) = \
            self.__MapLetterListToNumList(HOList, COList, trackList)

        # 计算t=0时刻的系数
        self.__coefHOList = [1] * len(self.__rawCoefHOList)
        self.__coefCOList = [1] * len(self.__rawCoefCOList)

        maxnum = -1
        for item in self.__numHOList:
            for item2 in item:
                maxnum = max(maxnum, item2)
        for item in self.__numCOList:
            for item2 in item:
                maxnum = max(maxnum, item2)
        InitList = [0] * floor((maxnum + 1) / 2)

        self.__deriveData = clucore.DeriveAssign(InitList, self.__numHOList, self.__coefHOList,
                                                 self.__numCOList, self.__coefCOList, self.__numTrackList, maxOPLen)

    def SetCoefHOList(self, coefHOList, args=None, t=0, ow=True):
        """
        :param coefHOList: HO系数的列表
        """
        for c in coefHOList:
            if not isinstance(c, int) and not isinstance(c, float) \
                    and not isinstance(c, complex) and not isinstance(c, types.FunctionType):
                raise TypeError("Invalid types in coefficient list.")
        for i in range(len(coefHOList)):
            if isinstance(coefHOList[i], int) or isinstance(coefHOList[i], float) or isinstance(coefHOList[i], complex):
                self.__coefHOList[i] = coefHOList[i]
            else:
                fun = coefHOList[i]
                if args is not None:
                    self.__coefHOList[i] = fun(t, *args)
                else:
                    self.__coefHOList[i] = fun(t)

        if ow:
            for i in range(len(coefHOList)):
                if isinstance(coefHOList[i], int) or isinstance(coefHOList[i], float) or isinstance(coefHOList[i], complex):
                    self.__rawCoefHOList[i] = coefHOList[i]
                else:
                    self.__rawCoefHOList[i] = Copy_Func(coefHOList[i])
        clucore.SetHamiltonCoef(self.__deriveData, self.__coefHOList)

    def SetCoefCOList(self, coefCOList, args=None, t=0, ow=True):
        """
        :param coefCOList: CO系数的列表
        """
        for c in coefCOList:
            if not isinstance(c, int) and not isinstance(c, float) \
                    and not isinstance(c, complex) and not isinstance(c, types.FunctionType):
                raise TypeError("Invalid types in coefficient list.")
        for i in range(len(coefCOList)):
            if isinstance(coefCOList[i], int) or isinstance(coefCOList[i], float) or isinstance(coefCOList[i], complex):
                self.__coefCOList[i] = coefCOList[i]
            else:
                fun = coefCOList[i]
                if args is not None:
                    self.__coefCOList[i] = fun(t, *args)
                else:
                    self.__coefCOList[i] = fun(t)

        if ow:
            for i in range(len(coefCOList)):
                if isinstance(coefCOList[i], int) or isinstance(coefCOList[i], float) or isinstance(coefCOList[i], complex):
                    self.__rawCoefCOList[i] = coefCOList[i]
                else:
                    self.__rawCoefCOList[i] = Copy_Func(coefCOList[i])
        clucore.SetCollapseCoef(self.__deriveData, self.__coefCOList)

    def SetCurrentValue(self, curValueList):
        """
        :param curValueList: current value的列表
        """
        for c in curValueList:
            if not isinstance(c, int) and not isinstance(c, float) and not isinstance(c, complex):
                raise TypeError("Invalid types in coefficient list.")
        clucore.SetCurrentValue(self.__deriveData, curValueList)

    def GetStrHOList(self):
        """
        :return: 字符形式的HO列表
        """
        ans = []
        for numlist in self.__numHOList:
            strname = self.__MapNumListToStr(numlist)
            ans.append(strname)
        return ans

    def GetStrCOList(self):
        """
        :return: 字符形式的CO列表
        """
        ans = []
        for numlist in self.__numCOList:
            strname = self.__MapNumListToStr(numlist)
            ans.append(strname)
        return ans

    def GetNumHOList(self):
        """
        :return: 数字形式的HO列表
        """
        return copy.deepcopy(self.__numHOList)

    def GetNumCOList(self):
        """
        :return: 数字形式的CO列表
        """
        return copy.deepcopy(self.__numCOList)

    def GetHamiltonCoef(self):
        return clucore.GetHamiltonCoef(self.__deriveData)

    def GetCollapseCoef(self):
        return clucore.GetCollapseCoef(self.__deriveData)

    def GetCurrentValue(self):
        """
        :return: 当前的current value列表
        """
        return clucore.GetCurrentValue(self.__deriveData)

    def Calculate(self):
        """
        :return: 计算的结果, 类型为列表
        """
        return clucore.CalEvolution(self.__deriveData)

    def UpdateCoef(self, t, args=None, ForceUpdate=False):
        isChanged = False
        HOCoefList0 = []
        COCoefList0 = []
        for stuff in self.__rawCoefHOList:
            if isinstance(stuff, types.FunctionType):
                if args is not None:
                    HOCoefList0.append(stuff(t, *args))
                else:
                    HOCoefList0.append(stuff(t))
                isChanged = True
            else:
                HOCoefList0.append(stuff)
        for stuff in self.__rawCoefCOList:
            if isinstance(stuff, types.FunctionType):
                if args is not None:
                    COCoefList0.append(stuff(t, *args))
                else:
                    COCoefList0.append(stuff(t))
                isChanged = True
            else:
                COCoefList0.append(stuff)

        if isChanged or ForceUpdate:
            self.__coefHOList = HOCoefList0
            self.__coefCOList = COCoefList0
            clucore.SetHamiltonCoef(self.__deriveData, self.__coefHOList)
            clucore.SetCollapseCoef(self.__deriveData, self.__coefCOList)

    def UpdateInitialState(self, initialState):
        # TODO: 字符串形式的initialState
        if isinstance(initialState, str):
            raise TypeError("Sorry. This kind of initial state is not supported for now.")
        if not isinstance(initialState, list):
            raise TypeError("Invalid types for initial state.")
        for item in initialState:
            if not isinstance(item, int):
                raise TypeError("Invalid types in initial state list.")
        return clucore.UpdateInitialState(self.__deriveData, initialState)

    @staticmethod
    def __CheckAndParse(H, Co_ps, trackOp, maxOpLen):
        # check
        for item in H:
            if not isinstance(item, list) and len(item) != 2:
                raise SyntaxError("Invalid parameter in parameter H.")
            if not isinstance(item[0], str):
                raise TypeError("Invalid types in parameter H.")
            if not isinstance(item[1], complex) and not isinstance(item[1], int) \
                    and not isinstance(item[1], float) and not isinstance(item[1], types.FunctionType):
                raise TypeError("Invalid types in parameter H.")
        for item in Co_ps:
            if not isinstance(item, list) and len(item) != 2:
                raise SyntaxError("Invalid parameter in parameter Co_ps.")
            if not isinstance(item[0], str):
                raise TypeError("Invalid types in parameter Co_ps.")
            if not isinstance(item[1], complex) and not isinstance(item[1], int) \
                    and not isinstance(item[1], float) and not isinstance(item[1], types.FunctionType):
                raise TypeError("Invalid types in parameter Co_ps.")
        for item in trackOp:
            if not isinstance(item, str):
                raise TypeError("Invalid types in parameter trackOp.")
        if maxOpLen <= 0:
            raise SyntaxError("Parameter maxOpLen must be greater than zero.")
        # parse
        hoList = []
        coList = []
        hoCoefList = []
        coCoefList = []
        for item in H:
            hoList.append(item[0])
            hoCoefList.append(item[1])
        for item in Co_ps:
            coList.append(item[0])
            coCoefList.append(item[1])
        return hoList, hoCoefList, coList, coCoefList

    def __CreateNumLetterMap(self, strHOList, strCOList, trackList):
        for s in strHOList:
            if not isinstance(s, str):
                raise TypeError("Invalid types in string hamilton list.")
        for s in strCOList:
            if not isinstance(s, str):
                raise TypeError("Invalid types in string collapse list.")
        for s in trackList:
            if not isinstance(s, str):
                raise TypeError("Invalid types in string tracking list.")
        # 26个字母的桶
        lettersBucket = [None] * 26
        for s in strHOList:
            for letter in s.upper():
                lettersBucket[ord(letter) - 65] = letter
        for s in strCOList:
            for letter in s.upper():
                lettersBucket[ord(letter) - 65] = letter
        for s in trackList:
            for letter in s.upper():
                lettersBucket[ord(letter) - 65] = letter
        totalLetters = []
        for stuff in lettersBucket:
            if stuff is not None:
                totalLetters.append(stuff)
        cnt = 0
        self.__numToLetterMap[0] = '.'
        for c in totalLetters:
            self.__numToLetterMap[2 * cnt + 1] = c
            self.__numToLetterMap[2 * cnt + 2] = c.lower()
            self.__letterToNumMap[ord(c) - 65] = 2 * cnt + 1
            self.__letterToNumMap[ord(c) - 39] = 2 * cnt + 2
            cnt += 1

    def __MapLetterListToNumList(self, strHOList, strCOList, trackList):
        numHOList = self.__MapLetterListToNumList_Build(strHOList)
        numCOList = self.__MapLetterListToNumList_Build(strCOList)
        numTrackList = self.__MapLetterListToNumList_Build(trackList)
        return numHOList, numCOList, numTrackList

    def __MapLetterListToNumList_Build(self, strList):
        ans = []
        for s in strList:
            li = []
            for letter in s:
                if letter == '.':
                    li.append(0)
                elif 90 >= ord(letter) >= 65:
                    li.append(self.__letterToNumMap[ord(letter) - 65])
                else:
                    li.append(self.__letterToNumMap[ord(letter) - 71])
            ans.append(li)
        return ans

    def __MapNumListToStr(self, numList):
        s = ''
        for n in numList:
            if not isinstance(n, int):
                raise TypeError("Invalid types in num list.")
            s += self.__numToLetterMap[n]
        return s

    def Debug(self):
        clucore.PrintData(self.__deriveData)


def Copy_Func(f):
    g = types.FunctionType(f.__code__, f.__globals__, name=f.__name__,
                           argdefs=f.__defaults__,
                           closure=f.__closure__)
    g = functools.update_wrapper(g, f)
    g.__kwdefaults__ = f.__kwdefaults__
    return g