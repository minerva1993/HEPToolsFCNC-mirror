#!/usr/bin/python
import os, sys

#base_path17 = '2017'
#base_path18 = '2018'
base_path17 = '/data1/users/minerva1993/work/fcnc_RunII2017/finalMVA/current_ver'
base_path18 = '/data1/users/minerva1993/work/fcnc_RunII2018/finalMVA/current_ver'

ch = ['', 'jecup', 'jecdown', 'jerup', 'jerdown',
      'hdampup', 'hdampdown', 'TuneCP5up', 'TuneCP5down',]

for chs in ch:
  if not os.path.exists( base_path17 ):
    os.makedirs( base_path17 )
  if not os.path.exists( base_path17 + "/root_" + chs ):
    os.makedirs( base_path17 + "/root_" + chs )
  if not os.path.exists( base_path17 + "/hdf_" + chs ):
    os.makedirs( base_path17 + "/hdf_" + chs )
  if not os.path.exists( base_path18 ):
    os.makedirs( base_path18 )
  if not os.path.exists( base_path18 + "/root_" + chs ):
    os.makedirs( base_path18 + "/root_" + chs )
  if not os.path.exists( base_path18 + "/hdf_" + chs ):
    os.makedirs( base_path18 + "/hdf_" + chs )


def nfile(dirname):
  tmp = []
  allfiles = os.listdir(dirname)
  for files in allfiles:
    if files.endswith(".root") or files.endswith(".h5"): tmp.append(files)
  print dirname.split('/')[-1] + ": " + str(len(tmp)).rjust(25-len(dirname.split('/')[-1]))

for chs in ch:
  nfile(base_path17 + "/root_" + chs)
for chs in ch:
  nfile(base_path17 + "/hdf_" + chs)
for chs in ch:
  nfile(base_path18 + "/root_" + chs)
for chs in ch:
  nfile(base_path18 + "/hdf_" + chs)
