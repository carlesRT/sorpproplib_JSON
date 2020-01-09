# controller program for SorpPropLib inquiries
# inputs: sorbent/refrigerant/abs or ads/Tc/X/Peq
# outputs: Peq/Yeq/sorbent/refrigerant/Tc/X/equation/citation

import string
from getEqnFormAndCoeff import getEqnAndCoeff
from calcEqn import *

# define working pair
ref = 'water'
sorb = 'licl'
subtype = ''

# define operating condition
Tc = 65.5
X = 30

# get equation and coefficients from JSON
entries = getEqnAndCoeff(ref,sorb,subtype)
for entry in entries:
    keys=entry.keys()
    for key in keys:
        reference = entry[key].get('_c_')
        eqnForm = entry[key].get('_e_')
        param = entry[key].get('_p_')

# switch
if len(entries) == 0:
    eqnForm = 'custom'
    
    if (ref=='water') & (sorb=='licl'):
        results = custom_LiCl_water(Tc,X)        
        for key,value in results.items():
            Peq = key
            reference = value
    elif (ref=='water') & (sorb=='cacl2'):
        results = custom_CaCl2_water(Tc,X)      
        for key,value in results.items():
            Peq = key
            reference = value

elif eqnForm == 'Duhring':
    Peq = duhring(param,Tc,X)

print(ref+':'+sorb+', T='+str(Tc)+' degC, X='+str(X)+' %')
print('Peq='+str(Peq)+' kPa')
print('equation form is: '+eqnForm)
print('reference: '+reference)