import QCLSolver.core as clucore
from typing import List, Dict, Tuple


def cluster_expansion(op: str, max_op_len: int = 5) -> Tuple[List[int], Dict[str, int], List[Tuple[complex, Tuple[int]]]]:
    """
    Applying 'Cluster Expansion' to a specific operator and print out the result.

    Example:
        arr_ops, map_list, ce = cluster_expansion('AAa', 3)

    :param op: operator in string representation
    :type op: str
    :param max_op_len: max length of operator
    :type max_op_len: int
    :return:
    The first result is the array-type representation of the operator.

    The second result is the mapping list.

    The third result is the answer of 'cluster_expansion':

    Each Tuple[complex, Tuple[int]] means a node.
    The 'complex' is the coefficient. The 'Tuple[int]' is the array-type representation of nodes.

    :rtype: Tuple[List[int], Dict[str, int], List[Tuple[complex, Tuple[int]]]]
    """
    arr_ops, map_list = map_ops(op)
    ans: List[Tuple[complex, Tuple[int]]] = clucore.ClusterExp(arr_ops, max_op_len)
    return arr_ops, map_list, ans


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
