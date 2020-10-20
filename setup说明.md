# 配置说明

### 配置环境

* Python 3.7及以上(低版本暂未测试)

* scipy 1.4.1及以上

  * 更新代码如下，在requirements.txt的同目录下运行

  * ```powershell
    pip install -r requirements.txt
    ```

### 配置步骤

1. 在./cluster_py下运行shell，输入代码

```shell
python ./setup.py install
```

### import方式

```python
from QCLSolver.solver import Solve
from QCLSolver.data import Data
```

### API说明

#### 1. Data类

#### 1.1 初始化

* 函数原型:

```
class Data:
    def __init__(self, H, Co_ps, trackList, maxOPLen)
```

* 参数列表:
  1. **H**: hamilton量，格式为[[hamilton字符串形式, 系数/系数函数指针]...]。例如，Hamilton量有'Aa'与'Bb'，系数分别为$1+j, -2j$，则**H**为[['Aa', $1+j$], ['Bb', $-2j$]]
  2. **Co_ps**: callapse量，格式同上。
3. **trackOp**: 含有多个tracking算符的列表。例如，两个tracking算符表示为'AaBb' ，'bB'，则\[Hamilton算符...\]为['AaBb' , 'bB']
  4. **maxOPLen**: 算符最大长度

#### 1.2 UpdateInitialState(...)

* 函数原型: 

```python
def UpdateInitialState(self, initialState)
```

* 参数说明:
  1. **initialState**: 一个表示初始状态量的list。例如$[1,2,0,3]$表示算符中$\{1,2\}$的初始量为1，$\{3,4\}$的初始量为2，$\{5,6\}$的初始量为0，$\{7,8\}$的初始量为3。
* 返回值:
  * 类型: list
  * 说明: 返回更新出来的current Value

#### 2. Solve(...)

```python
def Solve(ddata, Initial_State, t_span, user_args=None, method='RK45', t_eval=None, dense_output=False, events=None, vectorized=False, **options)
```

* 参数列表:
  1. **ddata**: 已经建立的Data类
  3. **Initial_State**: 一个表示初始状态量的list。例如$[1,2,0,3]$表示算符中$\{1,2\}$的初始量为1，$\{3,4\}$的初始量为2，$\{5,6\}$的初始量为0，$\{7,8\}$的初始量为3。
  6. 后续的参数: 见solve_ivp的文档
* 返回值：
  * 类型: Bunch类型
  * 说明: 见solve_ivp的文档