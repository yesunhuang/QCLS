# Quick guide to QCLS
QCLS: Cluster Expansion Based Solver for Open Quantum System

## Install

### Requirement

* C++ Build environment
  (Simply download [VSBuildTools](https://visualstudio.microsoft.com/downloads/#other) and install C++ build tools if you don't know how to configure it.)

* Python 3.7 and later version

* Scipy 1.4.1 and later version

* numpy 1.16.5 and later version

  Quick install

  ```
  pip install -r requirements.txt
  ```

### Setup

#### Install manually

Run the following code under the path `./cluster_py`

```
python ./setup.py install
```

#### Install via pip

```
pip install QCLSolver
```

#### Install via conda (python<3.9)

```
conda install -c yesunhuang qclsolver
```

#### Suggested way of import

```python
from QCLSolver.solver import Solve
from QCLSolver.data import Data
from QCLSolver.tool import*
```



## Genernal procedure

Using `QCLS` is quite similar to using the `mesolve` in `QuTip`, a quite simple procedure can be followed:

* Create lists for representing the Hamiltonian, collapse operators and the initial state. Determine the tracking operators and the required cluster-expansion order.

  (We are using the upper class English letters to represent creation operators and lower class ones to represent the annihilation operators. **The system will automatically detect how many different letters u have inputted and assume the same number of modes in the system**. )
  
* Derive the equations for the systems using the construct
  function of `Data` class and store the instance.

* Feed the `Data` instance into the `Solve` method and evolve the system.

* Process the and visualize the result.

## Quick Example

The following code shows how to use `QCLS` to simulate the *Janes-Cumming model*. (See also the same title notebooks from *Quantum mechanics lectures with QuTiP*)

The Hamiltonian can be expressed as

![](http://latex.codecogs.com/svg.latex?\hat{H}=\hbar\omega_c\hat{a}^\dagger\hat{a}+\frac{1}{2}\hbar\omega_a\hat{\sigma}_z+{\hbar}g(\hat{a}^\dagger\hat{\sigma}_-+\hat{a}\hat{\sigma}_+))

```python
#parameters
wc = 1.0  * 2 * pi  # cavity frequency
wa = 1.0  * 2 * pi  # atom frequency
g  = 0.05 * 2 * pi  # coupling strength
kappa = 0.005       # cavity dissipation rate
gamma = 0.05  

#Create the time list
tlist = np.linspace(0,25,101) #numpy has been imported as np

#Build up the Hamiltonian operators
Hamilton = [['Aa',wc],['Bb',wa],['Ab',g],['aB',g]]

#Build up the Collapse operators
Co_ps = [['a',kappa ],['b',gamma ]]

#Define the inital states (Only folk states are supported now)
psi0 = [0,1]

#Add the tracking operators (The result will be returned at the same order of the operators in T_o)
T_o = ['Aa','Bb']

#Derive the equations under the expanson order of 2
data = Data(Hamilton ,Co_ps , T_o , 2)

#Evolve the system (We are using the ODE solver `solve_vip` from scipy, thus the parameters and the output forms are almost identity  )
sol = Solve(data , psi0, (0,25), t_eval = tlist )

#Visualize the result via matplotlib
fig, axes = plt.subplots(1, 1, figsize=(10,6))

axes.plot(tlist, np.real(sol.y[0]),color='red',linestyle='--',label="Cavity(QCLS)")
axes.plot(tlist, np.real(sol.y[1]),color='blue',linestyle='--',label="Atom excited state(QCLS)")
axes.legend(loc=0)
axes.set_xlabel('Time')
axes.set_ylabel('Occupation probability')

plt.show()
```

![Image text](https://github.com/yesunhuang/QCLS/blob/main/fig/example.png)
## API

### 1.Data Class

#### 1.1 Constructor

* Prototype

  ```python
  class Data:
      def __init__(self, H, Co_ps, trackList, maxOPLen)
  ```

* Parameters
  1. H: The list of Hamiltonian, the form should be of [['Operators','Coefficient']], Coefficient can be a complex number or **a function** 
  2. Co_ps: Collapse operators, the form is the same as Hamiltonian.
  3. trackOp: The list of the operators whose mean value you are going to track.
  4. maxOPLen: The order of cluster expansion

#### 1.2 UpdateInitialState(...)

* Prototype

  ```python
  def UpdateInitialState(self, initialState)
  ```

* Parameters

  1. initialState: A list of  initial folk states. The order of mode is following the alphabet order of the letters u used to represent the operators.

* Return

  ​	A list of new current mean value of tracking operators calculated based on the initial state.

#### 1.3 Calculate(...)

* Prototype

  ```python
  Calculate(self)
  ```

* Parameters

  None

* Return

  ​	A list of the slope values using to evolve the mean value of operators

#### 1.4 SetCoefHOList(...) 

**Warning: this function is for advanced use and might still have bugs.**

* Prototype

  ```python
  SetCoefHOList(self, coefHOList, args=None, t=0, ow=True)
  ```

* Parameters

  	1. coefHoList: New list of Hamiltonian' s coefficient.

  ​    Other default parameters is for helper use. Plz do not modify them.

* Important

  Run `UpdateCoef` with `ForceUpdate=true`  after running this method. 

#### 1.5 SetCoefCOList(...)

**Warning: this function is for advanced use and might still have bugs.**

* Prototype

  ```python
  SetCoefCOList(self, coefCOList, args=None, t=0, ow=True)
  ```

* Parameters

  1. coefCOList: New list of collapse operators' coefficient.

  ​    Other default parameters is for helper use. Plz do not modify them.

* Important

  Run `UpdateCoef` with `ForceUpdate=true`  after running this method. 

#### 1.6 SetCurrentValue(...)

**Warning: this function is for advanced use and might still have bugs.**

* Prototype

  ```python
  def SetCurrentValue(self, curValueList)
  ```

* Parameters

  1. curValueList: New list of the mean value of tracking operators. 

* Important

  **Please be noted that after the deriving process, the `T_o` list not only contains the initial tracking operators but all the operators needed to calculate the evolution.**

#### 1.7 GetCurrentValue(...)

**Warning: this function is for advanced use and might still have bugs.**

* Prototype

  ```python
  def GetCurrentValue(self)
  ```

* Important

  **Please be noted that after the deriving process, the `T_o` list not only contains the initial tracking operators but all the operators needed to calculate the evolution.**

#### 1.8 UpdateCoef(...)

**Warning: this function is for advanced use and might still have bugs.**

* Prototype

  ```python
  def UpdateCoef(self, t, args=None, ForceUpdate=False)
  ```

* Parameters

  ​	1.t: current time point.

  ​	2.ForceUpdate: Force an advanced update to the coefficient list.



### 2.Solve method

​	This function is simply connecting `QCLS` to outer solver.

* Prototype

  ```python
  def Solve(ddata, Initial_State, t_span, user_args=None, method='RK45', t_eval=None, dense_output=False, events=None, vectorized=False, **options)
  ```

* Parameters

  ​	1.ddata: The `Data` instance.

  ​	2.Initial_State: A list for representing the initial state.

  ​	3.other paramters: the same as the parameters requested by `solve_ivp` from *Scipy*

* Return:

  The same as the parameters requested by `solve_ivp` from *Scipy*. The result of y will be returned at the same order of the operators in T_o.

* Advanced
  * If u wanna use another solver, u need to implement a convert function following the examples above.

  ```python
  def ConvertToSolver(t,y,data):
      data.SetCurrentValue(y.tolist())
      dy=np.asarray(data.Calculate())
      return dy
  ```

  * Or if you are building your own solver.
   ```python
  def NaiveSolver(data,pace,t_range,initialState):
      data.UpdateInitialState(initialState)
      y0=data.GetCurrentValue()
      n=int((t_range[1]-t_range[0])//pace)
      tlist=np.linspace(t_range[0],t_range[1],n)
      y=np.zeros([len(y0),n],dtype=complex)
      for i in range(0,len(y0)):
        y[i,0]=y0[i]
      for i in range(1,n):
          k1=np.asarray(data.Calculate())
          y_s=y[...,i-1]+k1*pace*0.5
          data.SetCurrentValue(y_s.tolist())
          k2=np.asarray(data.Calculate())
          y_s=y[...,i-1]+k2*pace*0.5
          data.SetCurrentValue(y_s.tolist())
          k3=np.asarray(data.Calculate())
          y_s=y[...,i-1]+k3*pace
          data.SetCurrentValue(y_s.tolist())
          k4=np.asarray(data.Calculate())
          dy=(k1+2*k2+2*k3+k4)/6
          y[...,i]=y[...,i-1]+dy*pace
          data.SetCurrentValue(y[...,i].tolist())
      return (y,tlist)
   ```
  
### 3.cluster_expansion method

This function is used to investigate cluster expansion.

* Prototype

  ```python
  def cluster_expansion(op: str, max_op_len: int = 5) -> Tuple[List[int], Dict[str, int], List[Tuple[complex, Tuple[int]]]]:
* Parameters

  ​	1.op: The string representation of operator.

  ​	2.max_op_len:order of cluster expansion(please be noted that the operator inputted will be expanded for at least one time.)

* Return:

  The information of the cluster expansion form of inputted operator.
  
## More Information

* Check out the source codes and the jupyter notebook files.

* See also the published paper.

* If your are interesting in the implement, check up the *ImplementDetail* pdfs. (Warm prompt: those files are so messy and suck.)


