SCENARIO=TTBKG
ERA=2017
#ERA=2018
VER=01
MODEL=model_42_0.9501.h5
#MODEL=model_38_0.9508.h5
nohup python evaluation_kerasTF.py ${SCENARIO} ${VER} ${ERA} False 0 ${MODEL} > log_${SCENARIO}_${VER}_${ERA}_0 &
nohup python evaluation_kerasTF.py ${SCENARIO} ${VER} ${ERA} False 1 ${MODEL} > log_${SCENARIO}_${VER}_${ERA}_1 &
nohup python evaluation_kerasTF.py ${SCENARIO} ${VER} ${ERA} False 2 ${MODEL} > log_${SCENARIO}_${VER}_${ERA}_2 &
nohup python evaluation_kerasTF.py ${SCENARIO} ${VER} ${ERA} False 3 ${MODEL} > log_${SCENARIO}_${VER}_${ERA}_3 &
nohup python evaluation_kerasTF.py ${SCENARIO} ${VER} ${ERA} False 4 ${MODEL} > log_${SCENARIO}_${VER}_${ERA}_4 &
nohup python evaluation_kerasTF.py ${SCENARIO} ${VER} ${ERA} False 5 ${MODEL} > log_${SCENARIO}_${VER}_${ERA}_5 &
nohup python evaluation_kerasTF.py ${SCENARIO} ${VER} ${ERA} False 6 ${MODEL} > log_${SCENARIO}_${VER}_${ERA}_6 &
#python evaluation_kerasTF.py ${SCENARIO} ${VER} ${ERA} False 0 ${MODEL} &
#python evaluation_kerasTF.py ${SCENARIO} ${VER} ${ERA} False 1 ${MODEL} &
#python evaluation_kerasTF.py ${SCENARIO} ${VER} ${ERA} False 2 ${MODEL} &
#python evaluation_kerasTF.py ${SCENARIO} ${VER} ${ERA} False 3 ${MODEL} &
#python evaluation_kerasTF.py ${SCENARIO} ${VER} ${ERA} False 4 ${MODEL} &
#python evaluation_kerasTF.py ${SCENARIO} ${VER} ${ERA} False 5 ${MODEL} &
#python evaluation_kerasTF.py ${SCENARIO} ${VER} ${ERA} False 6 ${MODEL} &
