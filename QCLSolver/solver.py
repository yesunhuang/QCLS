from QCLSolver.data import Data
from QCLSolver.data import Copy_Func
import types
from typing import List, Tuple, Any, Union, Dict
import numpy as np
import scipy.integrate
import copy


def Solve(ddata: Data, Initial_State, t_span, user_args=None, method='RK45',
          t_eval=None, dense_output=False, events=None, vectorized=False, **options: Dict[Any, Any]):
    # 构造正确的t0, 并传入真正的系数初值
    ddata.UpdateCoef(t_span[0], args=user_args, ForceUpdate=True)
    # ddata.Debug()
    ddata.UpdateInitialState(Initial_State)

    y0c = np.array(ddata.GetCurrentValue())
    n = len(y0c)

    # 复制新的options和user_args
    options_new = None
    if len(options) > 0:
        options_new = copy.copy(options)
    user_args_new = copy.copy(user_args)
    if user_args_new is None:
        user_args_new = ()

    # 包装event
    events_wrapped_copy = None
    if events is not None:
        events_wrapped_copy = Copy_Func(__EventWrapper)
        events_wrapped_copy.terminal = False
        events_wrapped_copy.direction = float(0)
        if hasattr(events, 'terminal'):
            events_wrapped_copy.terminal = events.terminal
        if hasattr(events, 'direction'):
            events_wrapped_copy.direction = events.direction

    # 包装jac
    jac_func = None
    if options_new is not None and options_new.get('jac') is not None \
            and isinstance(options_new.get('jac'), types.FunctionType):
        jac_func = options.get('jac')
        # noinspection PyTypeChecker
        options_new['jac'] = __JacWrapper

    # 包装user_args为args_wrapped, 格式为[ddata, n, events, jac_func, user_args]
    if not isinstance(user_args, tuple) and user_args is not None:
        raise TypeError("Parameter user_args is not a tuple.")
    args_wrapped = (ddata, n, events, jac_func, user_args_new)

    if options_new is not None:
        return scipy.integrate.solve_ivp(__ConvertToSolver, t_span, y0c, method=method, t_eval=t_eval,
                                         events=events_wrapped_copy,
                                         dense_output=dense_output, vectorized=vectorized, args=args_wrapped,
                                         **options_new)
    else:
        return scipy.integrate.solve_ivp(__ConvertToSolver, t_span, y0c, method=method, t_eval=t_eval,
                                         events=events_wrapped_copy,
                                         dense_output=dense_output, vectorized=vectorized, args=args_wrapped)


def __ConvertToSolver(t, y, ddata: Data, n, events, jac_func, user_args: Tuple[Any]):
    ddata.SetCurrentValue(y.tolist())
    #print(ddata.GetCurrentValue())
    # 进行时变的系数
    ddata.UpdateCoef(t, user_args, ForceUpdate=False)
    dydtc = ddata.Calculate()
    #print(dydtc)
    return np.asarray(dydtc)


def __EventWrapper(t, y, ddata, n, events, jac_func, user_args: Tuple[Any]):
    return events(t, y, *user_args)


def __JacWrapper(t, y, ddata, n, events, jac_func, user_args: Tuple[Any]):
    return jac_func(t, y, *user_args)


if __name__ == '__main__':
    Hamilton = [['.', 31.41592653589793], ['Bb', -9.42477796076938], ['Cc', -6.283185307179586], ['Ab', 0.06283185307179587], ['aB', 0.06283185307179587], ['Ac', 0.07853981633974483], ['aC', 0.07853981633974483]]
    #Hamilton = [['Aa', 0.8], ['Bb', 0], ['Cc', 1.6], ['AAb', 1], ['aaC', 1], ['A', 2],['a', 2]]
    print(Hamilton)
    Coo_ps = [['a', 2], ['c', 4]]
    #print(Coo_ps)
    T_o = ['Aa', 'Bb', 'Cc']
    print(T_o)
    d = Data(Hamilton, Coo_ps, T_o, 2)
    d.SetCoefCOList([4, 8])
    t = ['suck']
    sol2 = Solve(d, [0, 1, 0], (0, 10))
    print('good')