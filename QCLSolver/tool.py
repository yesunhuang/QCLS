import QCLSolver.core as clucore
from typing import List, Dict, Tuple


def cluster_expansion(op: str) -> None:
    """
    Applying 'Cluster Expansion' to a specific operator and print out the result.

    :param op: operator in string representation
    :type op: str
    :return: None
    :rtype: None
    """
    arr_ops, map_list = map_ops(op)
    print('Array representation for "%s":' % op)
    print(arr_ops)
    print('Str to Array type mapping:')
    print(map_list)
    print('CE tree:')
    clucore.ClusterExp(arr_ops)


def map_ops(op: str) -> Tuple[List[int], Dict[str, int]]:
    """
    Mapping a operator from str to array type.

    Args:
        op: operator.

    Returns:
        The first element is the array type of 'ops'.
        The second is the mapping list.
    """
    # 26个字母的桶
    lettersBucket: List[str] = [None] * 26
    for letter in op.upper():
        lettersBucket[ord(letter) - 65] = letter
    totalLetters: List[str] = []
    for stuff in lettersBucket:
        if stuff is not None:
            totalLetters.append(stuff)
    ansMap: Dict[str, int] = {}
    cnt = 0
    for c in totalLetters:
        ansMap[c.upper()] = 2 * cnt + 1
        ansMap[c.lower()] = 2 * cnt + 2
        cnt += 1
    # mapping raw str
    ansArr: List[int] = []
    for c in op:
        ansArr.append(ansMap[c])
    return ansArr, ansMap
