import pandas as pd
import numpy as np
from scipy import interpolate

def fwhm_oscar(E):
    return np.sqrt(60.6499 + 0.458252*E + 0.000265552*E**2)

df_data = pd.read_csv("ocl135_response/resp.csv")
df_data['FWHM_rel_norm'] = (fwhm_oscar(df_data['Eg'])/df_data['Eg'])/(fwhm_oscar(1332.)/1332.)
SE = df_data['SE'].to_numpy()
SE[SE < 0] = 0
df_data['SE'] = SE
DE = df_data['DE'].to_numpy()
DE[DE < 0] = 0
df_data['DE'] = DE
c511 = df_data['c511'].to_numpy()
c511[c511 < 0] = 0
df_data['c511'] = c511
print(c511)
df_data.to_csv("resp.csv", index=False)




