from os import listdir
import os
import sys
import re
import json

def read_time( type="sequential", id="", nlin=100, ncol=100 ):
    result_dir = f"../result/matrix_result_{type}_{nlin}x{ncol}"
    
    if id != "":
        result_dir += f"_{id}"

    result_file = open(result_dir, "r")
    result_data = result_file.read()
    time = result_data.split('\n')[-2]
    result_file.close()

    return int(time)


MATRIX_DIR = "../matrix"
PRO_EXEC = "../build/bin/process"
TRE_EXEC = "../build/bin/thread"
 
MATRIX_SIDE = 2188

n_elements_mid = ( MATRIX_SIDE*MATRIX_SIDE )//8 + int((MATRIX_SIDE*MATRIX_SIDE)%8>0)
n_elements_list = [ int( n_elements_mid*(2**i) ) for i in range(-2,2+1) ]
n_process_list = [ (MATRIX_SIDE*MATRIX_SIDE)//j + int((MATRIX_SIDE*MATRIX_SIDE)%j>0) for j in n_elements_list ]

pro_time = -1
tre_time = -1

result_filename = "results_mult.json"

results_list = []

if len(sys.argv) > 1:
    result_filename = sys.argv[1]+'.json'

print(
      f"""
       \n=============== EXPERIMENT LOG ===============
       Executing parallel and thread for 
       p = {n_elements_list}
       Matrix size: {MATRIX_SIDE}x{MATRIX_SIDE} ({MATRIX_SIDE*MATRIX_SIDE} elements)
       """
     )

for p, n_process in zip(n_elements_list, n_process_list):

    print(
          f"""
           \n=============== EXPERIMENT LOG ===============
           Executing parallel and thread for 
           p = {n_process}, {n_process} process or threads
        """
         )

    os.system( f"./{PRO_EXEC} matrix_{MATRIX_SIDE}x{MATRIX_SIDE}_0 matrix_{MATRIX_SIDE}x{MATRIX_SIDE}_1 {p}" )
    os.system( f"./{TRE_EXEC} matrix_{MATRIX_SIDE}x{MATRIX_SIDE}_0 matrix_{MATRIX_SIDE}x{MATRIX_SIDE}_1 {p}" )

    pro_time = -1
    tre_time = -1
    for i in range(0,n_process):
        pro_time = max( pro_time, 
                        read_time( type="process", nlin=MATRIX_SIDE, ncol=MATRIX_SIDE, id=f"{i}" ) 
                      )
        tre_time = max( tre_time, 
                        read_time( type="thread" , nlin=MATRIX_SIDE, ncol=MATRIX_SIDE, id=f"{i}" ) 
                      )

    results_list.append( dict(type="process", time=pro_time, nlin=MATRIX_SIDE, ncol=MATRIX_SIDE, p=p) )
    results_list.append( dict(type="thread", time=tre_time, nlin=MATRIX_SIDE, ncol=MATRIX_SIDE, p=p) )

with open(result_filename, 'w') as f:
    json.dump(results_list, f, indent=4)
    