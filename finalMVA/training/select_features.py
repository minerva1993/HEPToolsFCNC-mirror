#!/usr/bin/env python
import sys, os, shutil
import subprocess
import signal
import time
from variables import input_variables_bdt

#Version of classifier
if len(sys.argv) < 5:
  print("Not enough arguements: Ch, JetCat, Ver, Era")
  sys.exit()
ch = sys.argv[1]
jetcat = sys.argv[2] #will be overidden as 99
ver = sys.argv[3]
era = sys.argv[4]

nfeat = {'j3b2':10, 'j3b3':10, 'j4b2':10, 'j4b3':10, 'j4b4':5}

#Run training for check separation
proc = subprocess.Popen('python training_bdt.py ' + ch + ' ' + jetcat + ' 99 ' + era + ' > ' + 'log_' + ch + '_' + jetcat + '_99 &', shell=True, preexec_fn=os.setsid)

#wait until log file created: for double check!!
time.sleep(3)

log_file = open('./log_' + ch + '_' + jetcat + '_99', 'r')

while True:
  logline = log_file.readline()
  #if logline.find('number of events passed') > 0: print logline
  #if logline.find('Separation') > 0: print logline
  if logline.find('Train method') > 0:
    os.killpg(proc.pid, signal.SIGTERM)
    break

keep_line = False
rank_list = []

#going to the first line of log file, check NEvents and ranking
log_file = open('./log_' + ch + '_' + jetcat + '_99', 'r')
loglines = log_file.readlines()

for line in loglines:
  if any(i in line for i in ['number of events passed', '-- training events', '-- testing events']): print line
  if 'Train method' in line: keep_line = False
  if 'Separation' in line: keep_line = True
  if keep_line and line.count(':') == 3: rank_list.append(line.split(':')[2].strip())

rank_list = rank_list[1:]
#Select top N = nfeat[jetcat] variables and sort by the order of features in root file
rank_list = rank_list[:nfeat[jetcat]]
print rank_list

all_vars = input_variables_bdt(jetcat)
sorted_rank_list = []

for var in all_vars:
  if var in rank_list: sorted_rank_list.append(var)

print sorted_rank_list

