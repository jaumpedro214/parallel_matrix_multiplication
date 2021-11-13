from os import listdir
import os
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
SEQ_EXEC = "../build/bin/sequential"
PRO_EXEC = "../build/bin/process"
TRE_EXEC = "../build/bin/thread"
AUX_EXEC = "../build/bin/auxiliar"

# 2 minutes in milisseconds
MIN_SEQ_TIME = 2*60*1000
mat_side = 100 

seq_time = -1
pro_time = -1
tre_time = -1

results_list = []

while seq_time < MIN_SEQ_TIME:
    os.system( f"./{AUX_EXEC} {mat_side} {mat_side} {mat_side} {mat_side}" )
    p = ( mat_side*mat_side )//8 + int( (mat_side*mat_side)%8>0 )
    
    # Executing the multiplicationdata
    os.system( f"./{SEQ_EXEC} matrix_{mat_side}x{mat_side}_0 matrix_{mat_side}x{mat_side}_1" )
    os.system( f"./{PRO_EXEC} matrix_{mat_side}x{mat_side}_0 matrix_{mat_side}x{mat_side}_1 {p}" )
    os.system( f"./{TRE_EXEC} matrix_{mat_side}x{mat_side}_0 matrix_{mat_side}x{mat_side}_1 {p}" )

    # Reading the times
    seq_time = read_time( type="sequential", nlin=mat_side, ncol=mat_side )
    
    pro_time = -1
    tre_time = -1
    for i in range(0,8):
        pro_time = max( pro_time, 
                        read_time( type="process", nlin=mat_side, ncol=mat_side, id=f"{i}" ) 
                      )
        tre_time = max( tre_time, 
                        read_time( type="thread" , nlin=mat_side, ncol=mat_side, id=f"{i}" ) 
                      )

    results_list.append( dict(type="sequential", time=seq_time, nlin=mat_side, ncol=mat_side) )
    results_list.append( dict(type="process", time=pro_time, nlin=mat_side, ncol=mat_side, p=p) )
    results_list.append( dict(type="thread", time=tre_time, nlin=mat_side, ncol=mat_side, p=p) )

    mat_side = 2*mat_side
    break

# Saving the results to a file
with open('results.json', 'w') as f:
    json.dump(results_list, f, indent=4)
    