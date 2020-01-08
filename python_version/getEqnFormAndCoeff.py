# to parse JSON and capture the equation
# form and coefficients given the 
# type of sorbent and refrigerant


def getEqnAndCoeff(ref,sorb,subtype):

    import json
    import sys
    import string

    print(ref+':'+sorb+':'+subtype)

    with open('sorpproplib.json','r') as mfile:
        data = mfile.read()
    lib = json.loads(data)


    entries = []

    for item in lib:
        sorbMatch = item.get('v').get('_s_')==str(sorb)
        refMatch = item.get('v').get('_r_')==str(ref)
        subTmatch = item.get('v').get('_t_')==str(subtype)
        if(sorbMatch&refMatch&subTmatch):
            entries.append(item.get('v').get('_ep_'))

    print(entries)
