#!/usr/bin/python
from ROOT import *
import os, sys
from root_numpy import tree2array
import glob
import pandas as pd
gROOT.SetBatch(True)
gROOT.ProcessLine("gErrorIgnoreLevel = kFatal;")

#Channel and version
if len(sys.argv) < 3:
  print("Not enough arguements: Era, Input, Output")
  sys.exit()
#ver = "010101"
ver = "060606"
era = sys.argv[1]
input_filename = sys.argv[2]
output_filename = sys.argv[3]
#syst = ["","jecup","jecdown","jerup","jerdown"]
syst = ["","jecAbsoluteup","jecAbsolutedown", "jecAbsolute"+era+"up", "jecAbsolute"+era+"down",
        "jecBBEC1up", "jecBBEC1down", "jecBBEC1"+era+"up", "jecBBEC1"+era+"down",
        "jecFlavorQCDup", "jecFlavorQCDdown", "jecRelativeBalup", "jecRelativeBaldown",
        "jecRelativeSample"+era+"up", "jecRelativeSample"+era+"down",
        "jerup","jerdown"]
syst2 = ["TuneCP5up","TuneCP5down","hdampup","hdampdown"] #dedecative samples exist

for syst_ext in syst + syst2:
  #if not os.path.exists( era + "/root_" + syst_ext ):
  if not os.path.exists('/data1/users/minerva1993/work/fcnc_RunII' + era + '/finalMVA/current_ver/root_' + syst_ext):
    print "No folder, run 'python dir_manage.py'"
    sys.exit()
  #if not os.path.exists( era + "/hdf_" + syst_ext ):
  if not os.path.exists('/data1/users/minerva1993/work/fcnc_RunII' + era + '/finalMVA/current_ver/hdf_' + syst_ext):
    print "No folder, run 'python dir_manage.py'"
    sys.exit()

def runAna(input_filename, output_filename):
  print 'processing ' + input_filename.split('/')[-2] + '/' + input_filename.split('/')[-1]
#  print 'processing ' + input_filename

  for syst_ext in syst + syst2:
    if   ("Run201" in output_filename) and syst_ext != "": continue
    elif (syst_ext in syst2) and not (syst_ext in output_filename): continue
    elif (syst_ext in syst) and any(tmp in output_filename for tmp in syst2): continue
    else:
      if (syst_ext in syst2): output_filename = output_filename.replace(syst_ext,"")

      chain = TChain("fcncLepJets/tree","events")
      chain.Add(input_filename)
      chain.Process("makeTuple.C+", era + ver + syst_ext + '_' + output_filename)

      #if os.path.isfile(era + "/root_" + syst_ext + "/finalMVA_" + output_filename + ".root"):
      #  f = TFile.Open(era + "/root_" + syst_ext + "/finalMVA_" + output_filename + ".root")
      #  t = f.Get("tree")
      #  if t.GetEntries() != 0:
      #    a = tree2array(t)
      #    df = pd.DataFrame(a)
      #    df.to_hdf(era + "/hdf_" + syst_ext + "/finalMVA_" + output_filename + ".h5", key='df', mode='w', complib='bzip2', complevel=9)
      #  else: os.remove(era + "/root_" + syst_ext + "/finalMVA_" + output_filename + ".root")

      if os.path.isfile('/data1/users/minerva1993/work/fcnc_RunII' + era + '/finalMVA/current_ver/root_' + syst_ext + "/finalMVA_" + output_filename + ".root"):
        f = TFile.Open('/data1/users/minerva1993/work/fcnc_RunII' + era + '/finalMVA/current_ver/root_' + syst_ext + "/finalMVA_" + output_filename + ".root")
        t = f.Get("tree")
        if t.GetEntries() != 0:
          a = tree2array(t)
          df = pd.DataFrame(a)
          df.to_hdf( '/data1/users/minerva1993/work/fcnc_RunII' + era + '/finalMVA/current_ver/hdf_' + syst_ext + "/finalMVA_" + output_filename + ".h5", key='df', mode='w', complib='bzip2', complevel=9)
        else: os.remove('/data1/users/minerva1993/work/fcnc_RunII' + era + '/finalMVA/current_ver/root_' + syst_ext + "/finalMVA_" + output_filename + ".root")

runAna(input_filename, output_filename)
