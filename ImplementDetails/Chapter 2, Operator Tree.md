# Chapter 2: Operator Tree

>  It describes the data structure we used to implement the calculator. The inspiration came from *trie*s which is widely used in string searching.

## Introduction

> It should be emphasized that the operators to be stored in the operator tree actually refer to the ensemble average of certain operator, that is:

$$
<A>=tr(\rho A)
$$

> Thus <ab> and <a><b> have different meanings in the tree. And we put a '.' in the operator input to distinguish them. For example: <abc><bc> represented in the input is {bc.abc}.

> We use a order tree structure to describe a set of operators and their coefficient. And this structure support several operation which enable us to implement **cluster expansion** process in a efficient way.

## Operator Representation

> **All the operators to be put in the operator tree should be normalized operators. **


## Tree Structure

### Node

> Each node contain links that are either null or references to its child. (Maybe there is also a link to its parents .) Obviously, you can use a pointer (or reference) array to store the link, the array length is 2m+1, m is the numbers of modes.(U can improve it and use less space based on the fact that some link is absolutely null due to the property of normalized operator.). We store the number of operators in the root.
>
> Besides, each note except for the root is labelled with the integer value which we use to denote basic operators. **However, sometimes the labels do not refer to the basic operators but the position of certain array. In this case, we call the tree 'Position tree'.**
>
> Specifically, just like what we did in *tries*, we store the coefficient associated with each operator in the node corresponding to its last number in the array. *nodes with zero coefficients exist to facilitate search in the operator tree and do not correspond to certain operator.* 
>
> Obviously, the max deep of the tree is n, n is the length of the longest operator.

> * Example: 
>
> 	consider a set of operators:
>
> | operator    | array representation | coefficient |
> | ----------- | -------------------- | ----------- |
> | {a^dag a b^dag b c} | [1,2,3,4,6]          | 1           |
> | {a b^dag b } | [2,3,4]              | 16          |
> |{a.b^dag |[2,0,3]|6|
> |{a.c.a^dag a}|[2,0,6,0,1,2]|4|
> |{a.b.c}|[2,0,4,0,6]|6|
>
> The tree representation is:
> 	

> ```mermaid
> graph TB
>  	id1((root:5))
>  	id1-->id2((1:0))
>  	id1-->id3((2:0))
>  	id2-->id4((2:0))
>  	id4-->id16((3:0))
>  	id16-->id17((4:0))
>  	id17-->id18((6:1))
>  	id3-->id5((0:0))
>  	id5-->id8((3:6))
>  	id5-->id9((4:0))
>  	id5-->id10((6:0))
>  	id9-->id11((0:0))
>  	id11-->id12((6:6))
>  	id10-->id13((0:0))
>  	id13-->id14((1:0))
>  	id14-->id15((2:4))
>  	id3-->id6((3:0))
>  	id6-->id7((4:16))
> ```

### Search

> Given a certain normalized operator in form of array, simply scanning the array from left to right and do DFS using the pointer stored in current node' s pointer array. The pointer obviously can be reached in constant time. If we come across a null pointer or the coefficient of the node standing for the last basic operator is zero during the searching, it means that the operator does not exist in the tree. 
>
> The search process can be completed in O(n) time, where n is the length of the given operator.

### Insert

> Given a certain normalized operator and its coefficient, do the search process described above. If the search hit, renew the coefficient. If not, create new node following the branches according to the array form of the operator and store the coefficient in the last node. Renew the value of the root.
>
> The insert process can be completed in O(n) time, where n is the length of the given operator.

### T-T-Add
> Given two trees, add them together. Compare the total number of operators of two trees and select the larger one as output tree. Do DPS in the other tree and for every operator in the tree, do the insert process based on the coefficient.

### Delete

> To delete a certain operator in the tree need a bit more work. If the last node of the operator is not the leave, we just set its coefficient to zero. If it is the leave, destroy the node and the pointer linked to it, and in the DFS recalling process, we delete all the node way back until we reach the root, the node with none zero coefficient or the node with more than one child.(In lazy version, we can also just set its coefficient to zeros but might result in space wasting). Renew the value of the root.
>
> The delete process can guarantee to be completed in O(n) time and costs O(ln n) on average, where n is the length of the certain operator.

### T-O-Multiply

>In most case, we face the problem to multiply one tree with a certain operator of another tree. Use the latter tree as the output tree. Delete the certain operator in the tree but store its coefficient. Do DFS in the tree being multiplied and do the multiplied process between each operator in the tree and the certain operator described in the last chapter, multiplied their coefficient and insert the result to the output tree. 

### T-T-Multiply

> Multiply a tree with another will cost a great amount of time and have no better idea than brute force yet. Create a new root node, do DFS in both trees, multiply every operator in one and another, and add the result to the new tree. 

### Build from Position tree

> Given an operator represented by an array and the root node of a constructed *position tree*, we need to rebuild an *operator tree* from the input. This time, we do BFS in the *position tree* and use a variable `deep` to store the current depth. 
>
> Notice that there might be some same basic operators (in array representation, it means the same number) in a single operator, we need to merge the note which represent the same basic operators in the same depth. While building the tree according to the given array and the position tree, we use array and bags to store the references of certain basic operator in the same depth.
>
> Then we merge these nodes together, add up their coefficients. If they contain different children **in representation of position**, merge the references to children together and update their children's reference to parents. Besides, we just do the latter if their children are the same. Also, update the value of root if we both of the nodes we merged have none zero coefficients. 
>
> **Remember to do this after the searching of current depth or it might lead to serious error.**
>
> Here is an example:
>
> ​	Input:
>
> ​		 Operator：<aaa>   Array representation:[2,2,2]
>
> ​		 Position Tree:
>
> ```mermaid
> graph TB
>  	id1((root:5))
>  	id1-->id2((1:0))
>  	id1-->id3((2:0))
>  	id2-->id4((0:0))
>  	id4-->id5((2:0))
>  	id5-->id6((0:0))
>  	id6-->id7((3:2))
>  	id2-->id8((0:0))
>  	id8-->id9((2:0))
>  	id9-->id10((3:-1))
>  	id3-->id11((0:0))
>  	id11-->id12((1:0))
>  	id12-->id13((3:-1))
>  	id1-->id14((3:0))
>  	id14-->id15((0:0))
>  	id15-->id16((1:0))
>  	id16-->id17((2:-1))
>  	id2-->id18((2:0))
>  	id18-->id19((3:1))
> ```
>
> ​	Output:
>
> ```mermaid
> graph TB
>  	
>  	
>  	id1((root:3))
>  	id1-->id2((2:0))
>  	id2-->id4((0:0))
>  	id4-->id5((2:0))
>  	id5-->id6((0:0))
>  	id6-->id7((2:2))
>    	id5-->id10((2:-3))
>  	id2-->id18((2:0))
>  	id18-->id19((2:1))
>  ```

## API

*In pseudocode*

```c#
public class Node
{
    public int label{get;set;}
    public int value{get;set;}
    public Node [] children{get;set;}
    public Node parent{get;set;}
    public Node(int label,int value, int M){};//M is the total number of basic operator.
}

public class OPTree
{
    public Node root{get;set;}
    public int search(int[] operator){};//output the coefficient.
    public bool delete(int[] operator){};
    public bool insert(int[] operator){};
    public static bool ttadd(OPTree tree1,OPTree tree2){};
    public static bool tomultiply(OPTree mtree, Node moperator){};
    public static Node ttmultiply(OPTree tree1,OPTree tree2){};
    public OPTree(OPTree PositionTree){};//Build from Position Tree.
}

public static class Normalizer
{
    public static OPTree Normalize(int[] operator){};
    public static int[] Multiply(int[] operator1, int[] operator2[])
}
```

