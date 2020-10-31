## Chapter 1: Normalize Operator

### Single Mode Operators 

#### Problem:

>Given a arbitrary operator consists of  total length of **n** character representing by  

$$
a\space and\space a^{\dagger}
$$

> in arbitrary order  as follows:

$$
\{aa...a^{\dagger}a^{\dagger}...aaa....a^{\dagger}a^{\dagger}....aaa\} (length:m)
$$

> Following the rule of 

$$
[a,a^{\dagger}]=aa^{\dagger}-a^{\dagger}a=1
$$

> Decompose the input operator into several normalized operators which all the $a^{\dagger}$ are in front of $a$ s . For example:

$$
{aa^{\dagger}a}=a^{\dagger}aa+a
$$

> Output all the normalized operators and their coefficients due to the decomposition process.

#### Solution:

> Regardless of the data structure to be described in the next chapter, we simply assume that we are able to keep all the normalized operator' s coefficients in an array which can be access in constant time.(This is possible if we are going to use the hash table) 

> **Basic algorithm**:
>
> ​	Consider a normalized operator multiplied by $a$ on the left. Assuming this operator have **i** $a$ and **j** $a^{\dagger}$. Obviously the new operator can be decomposed into one normalized operator consisting of **i+1** $a$ and **j**  $a^{\dagger}$, and **j** normalized operators consisting of **i** $a$ and **j-1** $a^{\dagger}$. For example:

$$
a[a^{\dagger}a^{\dagger}aa]=a^{\dagger}a^{\dagger}aaa+2a^{\dagger}aa
$$

> **Solution**:
>
> ​	Basing on the simple algorithm described above, we simply scanning the operator from right to left. Every time we come across $a$, we do the composition process above and keep the result in a queue. 

> * **Step one:**
>
>   Starting from the right, record the current position with variable `i`. we ignore all the $a$ and when we come across the last $a$ , we put the scanned character together as an operator into the queue. And let the a variable `j` record the last $a$ 's position.

> * **Step two:**
>
>   Then, if we come across $a^{\dagger}$ , we do nothing. However, if we come across $a$, we out all the **current** operators in the queue. And for every operator, we do the decomposition process described above and add the new operator to the queue if it is not in the queue. (Notice that `i-j-1` tells us the additional $a^{\dagger}$ came across since last decomposition.) Besides, update the coefficient. Reset the value of `j` to `i`.

> * **Final step:**
>
>   When the scanning process is finished, use the value of `i` and `j` to out all the operators in the queue  and renew the coefficient.

> Obviously, the time complexity is

$$
O(N^2)
$$

> **conjecture**: There is no linear time algorithm. (Find a wise computer scientist to prove it :(

### Multimode Operators

#### Problem:

>Given a arbitrary operator(m modes) consists of  total length of **n** character representing by  

$$
a, a^{\dagger},b,b^{\dagger},c,c^{\dagger}...(2m\space kinds\space of\space character)
$$

> in arbitrary order  as follows:

$$
\{aa...a^{\dagger}a^{\dagger}...baab....b^{\dagger}a^{\dagger}....cac^\dagger\} (length:m)
$$

> Following the rule of 

$$
[O_i,O_i^{\dagger}]=O_iO_i^{\dagger}-O_i^{\dagger}O_i=1
$$

> Decompose the input operator into several normalized operators which all the $O_i^{\dagger}$'s are in front of $O_i$'s and all different modes' operators are in alphabetical order.

> Output all the normalized operators and their coefficients due to the decomposition process.

#### Solution:

> * **Step one:**
>
>   Using stable sort method to gather different modes' operators separately.**(They are in order!)**

> * **Step two:**
>
>   For different modes, we do the single mode operators decomposition process and save them in the coefficient array separately.

> * **Final step:**
>
>   Connect all the operators together based on the distributive law and we get all the normalized operators and their coefficients.

### Multiplied Operators

#### Problem:
>The high order terms which are produced by the cluster expansion process as **single operator**, and we always keep different multiplied parts of the operator in length increasing order (Or, we can say, integer increasing order). 
>
>Map all the basic operator to integer and translate the operator into array(or string for less space), use '0' to represent multiplication. For example:
>
>$$
><a^{\dagger}><c^{\dagger}cba^{\dagger}a>->[1,0,5,6,4,1,2]
>$$
>Given two normalized operators, output the normalized operator created by the multiplied process.
>
>For example:
>$$
><abc><a^{\dagger}b^{\dagger}bc>\times<a><bbc><abbc>=<a><abc><bbc><a^{\dagger}b^{\dagger}bc><abbc>
>$$
>or in array form:
>$$
>[2,4,6,0,1,3,4,6]\times[2,0,4,4,6,0,2,4,4,6]=[2,0,2,4,6,0,4,4,6,0,1,3,4,6,0,2,4,4,6]
>$$

#### Solution:

> The solution is quite similar to order array merging. 
>
> * **Step One**
>
>   Create variables `p1` and `p2` to indicate the current position of the first array and the second. Also, a boolean variable `Selection` to mark the selection and a variable `n` to records the scanning steps. Also, a new array `Result` to store the result whose length equal to sum of the length of input.
>
> * **Step Two** 
>
>   Scanning from left to right and compare the number from two array simultaneously, use `n` to record the steps. If the number scanned hits no '0', use the variable `Seletion` to mark the possible selection between two arrays basing on the comparison. If we come across '0' simultaneously, we use `selection` to tell us of the numbers we should put in the new array. Remember that variable `n` indicates the position we starting scanning. If we come across '0' in one array but none '0' in the other, we just simply select the former. If we reach the end of any array, just treat it like we come across '0'. 
>
> * **Step Three**
>
>   Reset the value of `n` to zeros, and the pointer of the unselected array to `pi-n`.
>
> * **Final Step**  
>
>   Repeat step two to step three until we reach the end of any array. Then we just simply put the remaining part of the other array in the new array.
>
> It is not difficult to prove that the time complexity is
> $$
> O(N)
> $$
> where N is the sum of the length of input array.



