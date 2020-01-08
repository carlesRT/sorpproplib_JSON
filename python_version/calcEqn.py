# define isotherm equations
# inputs: str:equation name; dict:coefficients;  :T(K)X(0.01)[abs]/T(K)P(kPa)[ads]
# outputs: P(kPa)[abs]/Y[ads]

import numpy as np


def duhring(coeff,Tsol_c,X):
    a0 = (coeff['a0'])
    a1 = (coeff['a1'])
    a2 = (coeff['a2'])
    a3 = (coeff['a3'])
    b0 = (coeff['b0'])
    b1 =  (coeff['b1'])
    b2 =  (coeff['b2'])
    b3 =  (coeff['b3'])
    c =  (coeff['c'])
    d =  (coeff['d'])
    e =  (coeff['e'])
    m =  (coeff['m'])
    n =  (coeff['n'])
    q =  (coeff['q'])
    r =  (coeff['r'])

    aa = a0 + a1*X + a2*np.power(X,2) + a3*np.power(X,3)
    bb = b0 + b1*X + b2*np.power(X,2) + b3*np.power(X,3)

    Trefsat_f = ((Tsol_c*n+m)-bb)/aa
    
    Pkpa = np.power(10,(c+d/(Trefsat_f+q)+e/np.power(Trefsat_f+q,2)))/r

    return Pkpa