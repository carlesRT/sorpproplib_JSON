# define isotherm equations
# inputs: str:equation name; dict:coefficients;  :T(K)X(0.01)[abs]/T(C)P(kPa)[ads]
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


def custom_LiCl_water(Tsol_c, X):

    pi0 = 0.28
    pi1 = 4.3
    pi2 = 0.6
    pi3 = 0.21
    pi4 = 5.1
    pi5 = 0.49
    pi6 = 0.362
    pi7 = -4.75
    pi8 = -0.4
    pi9 = 0.03

    xx = X/100
    Tsol_k = Tsol_c + 273.15

    pi25 = 1-np.power(1+np.power(xx/pi6,pi7),pi8)-pi9*np.exp(-np.power(xx-0.1,2)/0.005)

    ps_w = psat_water(Tsol_c)

    Pkpa = pi25 * ps_w * (2-np.power(1+np.power(xx/pi0,pi1),pi2)+Tsol_k/647*(np.power(1+np.power(xx/pi3,pi4),pi5)-1))

    reference = 'Conde, M. R. (2004). Properties of aqueous solutions of lithium and calcium chlorides: formulations for use in air conditioning equipment design. International Journal of Thermal Sciences, 43(4), 367-382.'
    return {Pkpa:reference}


def custom_CaCl2_water(Tsol_c, X):

    pi0 = 0.31
    pi1 = 3.698
    pi2 = 0.6
    pi3 = 0.231
    pi4 = 4.584
    pi5 = 0.49
    pi6 = 0.478
    pi7 = -5.2
    pi8 = -0.4
    pi9 = 0.018

    xx = X/100
    Tsol_k = Tsol_c + 273.15

    pi25 = 1-np.power(1+np.power(xx/pi6,pi7),pi8)-pi9*np.exp(-np.power(xx-0.1,2)/0.005)

    ps_w = psat_water(Tsol_c)

    Pkpa = pi25 * ps_w * (2-np.power(1+np.power(xx/pi0,pi1),pi2)+Tsol_k/647*(np.power(1+np.power(xx/pi3,pi4),pi5)-1))

    reference = 'Conde, M. R. (2004). Properties of aqueous solutions of lithium and calcium chlorides: formulations for use in air conditioning equipment design. International Journal of Thermal Sciences, 43(4), 367-382.'
    return {Pkpa:reference}


def psat_water(Tc):
    Pkpa = 1/7.5 * np.power(10,8.07131-1730.63/(233.426+Tc))

    # reference: http://ddbonline.ddbst.com/AntoineCalculation/AntoineCalculationCGI.exe?component=Water
    return Pkpa