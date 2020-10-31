# Cluster Expansion

> *Cluster Expansion* process is the key part of the calculator, but with the help of the fascinating data structure, it is not hard to implement.

## Introduction

> In order to calculate the evolution of the ensemble average of certain operator basing on the *Lindblad master equation*, we need to know the ensemble average of  the higher order term, which make it inevitable to solve infinite system of differential equations. However, with the help of *Cluster Expansion* approach, we are able to make some approximation and simplify the problem.
>
> Here is the simple introduction to the recursive definition of cluster expansion process:
> $$
> <N>=<N>_s
> 	\\+<N-2>_s\Delta<2>\\+<N-3>_s\Delta<3>\\+<N-4>_s\Delta<4>+<N-4>\Delta<2>\Delta<2>
> 	\\+...
> 	\\+\Delta<N>
> $$
> where each product term represents one factorization symbolically and implicitly includes a sum over all factorizations within the class of terms identified. The purely correlated part is denoted by the last term. By omitting the last term, we are able to make good approximation and avoid solving infinite system of differential equations.

##  Problem

> Given an operator consist of n **basic operators**, use the cluster expansion approach to decompose it into a set of operators consist of no more than n-1 basic operators.
>
> For example:
> $$
> <aaa>=3<a><aa>-2<a>^3\\
> <abc>=<a><bc>+<b><ac>+<c><ab>-2<a><b><c>
> $$
>
> Output the decomposed result in the form of operator tree.

## Solution

### Basic Idea

>Consider a simple condition of the problem where all the basic operators in a single operator are different. In that way, the problem reduce to a well-known model: finding all different ways to put n different balls in n same boxes. By assuming that the boxes with one ball represent single basic operator factorization, boxes with  m(>1) balls represent the m-basic-operators correlations terms. It is not difficult to figure out that each distribution way of the balls represent one certain term in the cluster expansion.
>
>By applying the theory of *Bell*, the total number of the terms in n-length-operator cluster expansion is
>$$
>B_n 
>$$
>where *B_i* is the *Bell Number*.
>
>And the recursion formula of *Bell Number* is
>$$
>B_{n+1}=\Sigma_{k=0}^nC_{n}^{n-k}B_k
>$$
>Following the instruction of this formula, we can use recursion technique to calculate *cluster expansion*.

### Tree representation

> We will use two kind of trees in the recursion process. One is the *B-Tree*, corresponding to the right side of the cluster-expansion equation. The other is the *Delta-Tree*, which represent the expression of Delta<N> where:
> $$
> \Delta<N>=<N>-<N>_s-<N-2>_s\Delta<2>-<N-3>_s\Delta<3>-...
> $$
> **By assuming the operator with no '0' element in array representation refers to Delta<N> in B-Tree but <N> in delta tree**, it is obvious that if we set all the coefficients in *B-Tree* to their opposite number except for this operator, we can generate the corresponding *Delta-Tree*. In practice, we only store the latter as a *position tree*. For convenience, we consider that
> $$
> Delta_{tree}(1)=<1> (not <1>-<1>_s)
> $$
> 

### Combination

> Notice that there is combinatorial number as factor in the recursion formula, thus we need to generate certain combination while performing the recursion.
>
> The algorithm is simple. To choose m elements in a n-array, we just need m loop structures, each loops from the outer loop's current value to the end of the array. The current value of each loop produce one way of combination.

### Recursion

> With the help of the reduction, we can use the recursion formula of *Bell Number* to perform cluster expansion in an effective way.  Recall that *B-Tree* is also equal to <N>, we simplify the recursion formula of cluster expansion to:
> $$
> B_{tree}(N+1)=<N+1>=<a_{n+1}><N>+\Delta<a_{n+1}\times\{1\}><N-1>+\\
> \Delta<a_{n+1}\times\{2\}><N-2>+...+\Delta<N+1>
> $$
> where a_{n+1) refer to the n+1-th element of the given array. And the latter factors of each term do not contain the n+1-th element.

### Specific Implement

> * Pre-calculation
>
>   Before performing cluster-expansion for specific operator, we follow the following steps to build *Delta-Tree* corresponding to n-length-operator.
>
>   * Step One
>
>     Suppose that we already have stored the *Delta-Tree* for the operator whose length is less than n+1. To calculate the sum of the set of terms
>     $$
>     <a_{n+1}\times\{i\}><N-i>,
>     $$
>     we use the combination process to produce all the possible combinations of  *i* elements from the first *n* elements of the array and put the n+1-th element in front of the combinations to produce the operators:
>
>     $$
> <a_{n+1}\times\{i\}>
>     $$
>     For each operator describe above, we gather the remaining elements to produce the operators:
>     $$
>     <N-i>
>     $$
>     It should be emphasized that they need to keep the original order to maintain the property of normalized operators.
>     
>   * Step Two
>   
>     Insert the <N-i> operator into the  *Delta-Tree(n+1)* with coefficient -1. Then build an operator tree from the *position tree* *Delta-tree(i+1)* according to the operator 
>     $$
>     <a_{n+1}\times\{i\}>.
>     $$
>     Use the T-O-Multiply method to multiply the <N-i> operator and the operator tree together.
>   
>    * Step Three
>   
>      Perform all the above steps for i from 0 to n-1.
>   
>    * Final Step
>   
>      Insert the operator <N+1> to the *Delta-Tree*(n+1) with the coefficient 1 and we finish the process of building *Delta-Tree* for n+1-length-operator.
>   
> * Online Calculation
>
>   For certain input n-length-operator, we simply build an operator tree from the *Position Tree* **Delta-Tree(n)**. Output all the operators in the tree with the opposite number of the coefficient except for the operator <n> and we generate the cluster expansion approximation of the operator.

## API

```c#
public static OPTree ClusterExpansion(int[] operator)
```