import os, sys
import numpy as np
import array
from ROOT import *
import ROOT

#if len(sys.argv) < 2:
#  print "specify year"
#  sys.exit()
#year = sys.argv[1]
#if year not in ['2017', '2018']:
#  print "wrong year"
#  sys.exit()

do_ttbb_rescale = False
#do_ttbb_rescale = True

base_path = "./"
if not os.path.exists( base_path + "post_process" ):
  os.makedirs( base_path + "post_process" )

nrebin = 1
rebin = {'j3b2': [-1., -0.8, -0.6, -0.4, -0.2, 0, 0.2, 0.4, 0.6, 0.8, 1.0],
         'j4b2': [-1., -0.8, -0.6, -0.4, -0.2, 0, 0.2, 0.4, 0.6, 0.8, 1.0],
         'j3b3': [-1., -0.7, -0.5, -0.3, -0.1, 0.1, 0.3, 0.5, 1.0],
         'j4b3': [-1., -0.7, -0.5, -0.3, -0.1, 0.1, 0.3, 0.5, 0.75, 1.0],
         'j4b4': [-1., -0.8, -0.6, -0.4, -0.2, 0, 0.2, 0.4, 0.6, 1.0]}

def write_envelope(syst, nhists, new_sumW):

  if (histos + "__" + syst + "0")  in histo_list:
    var_list = []
    for x in range(0,nhists):
      h = f.Get(histos + "__" + syst + str(x))
      if any(x in syst for x in ['scale', 'ps']):
        pass
      elif 'pdf' in syst:
        if x == 0: continue
        h.Scale(EventInfo.GetBinContent(2) / new_sumW.GetBinContent(1))
      else: h.Scale(EventInfo.GetBinContent(2) / new_sumW.GetBinContent(x+1))
      #h.Rebin(nrebin)
      h = h.Rebin(len(rebin[h.GetName().split('_')[2]])-1, h.GetName(), array.array('d',rebin[h.GetName().split('_')[2]]))

      #ttbb rescale: TOP-18-002
      if do_ttbb_rescale:
        if   'TTpowhegttbb'   in files: h.Scale(1.2)
        elif 'TTLLpowhegttbb' in files: h.Scale(1.26)

      var_list.append(h)

    nominal = f.Get(histos)
    nominal.SetDirectory(ROOT.nullptr)
    #nominal.Rebin(nrebin)
    nominal = nominal.Rebin(len(rebin[h.GetName().split('_')[2]])-1, h.GetName(), array.array('d',rebin[h.GetName().split('_')[2]]))

    #ttbb rescale: TOP-18-002
    if do_ttbb_rescale:
      if   'TTpowhegttbb'   in files: nominal.Scale(1.2)
      elif 'TTLLpowhegttbb' in files: nominal.Scale(1.26)

    n_bins = nominal.GetNcells()
    up = nominal.Clone()
    up.SetDirectory(ROOT.nullptr)
    up.Reset()
    dn = nominal.Clone()
    dn.SetDirectory(ROOT.nullptr)
    dn.Reset()

    for i in range(0, n_bins+2):
      minimum = float("inf")
      maximum = float("-inf")

      for v in var_list:
        c = v.GetBinContent(i)
        minimum = min(minimum, c)
        maximum = max(maximum, c)

      up.SetBinContent(i, maximum)
      dn.SetBinContent(i, minimum)

    up = bSFNorm(up, bSFInfo)
    dn = bSFNorm(dn, bSFInfo)
    up.SetName(histos + "__" + syst + "up")
    dn.SetName(histos + "__" + syst + "down")

    up.Write()
    dn.Write()


def rescale(binNum, new_sumW): # rescale up/dn histos

  #binNum = [up_num, down_num] or [bin]
  if   len(binNum) == 2 : mode = 0 #up, down in one root file
  elif len(binNum) == 0 : mode = 1 #use dedicated sample
  else: mode == 99

  if mode == 0: #FIXME
    if (histos + "__" + syst_name + "up")  in histo_list:
      for x in binNum:
        if x == binNum[0]:
          up = f.Get(histos + "__" + syst_name + "up")
          up.Scale(EventInfo.GetBinContent(2) / sumW_hist.GetBinContent(x))
        elif x == binNum[1]:
          dn = f.Get(histos + "__" + syst_name + "down")
          dn.Scale(EventInfo.GetBinContent(2) / sumW_hist.GetBinContent(x))

      up.Write()
      dn.Write()

  elif mode == 1:
    if syst_name in files:
      h = f.Get(histos)
      if not any(i in h.GetName() for i in ['Info', 'Weight']):
        h = bSFNorm(h, bSFInfo)
        h.Scale(nom_EventInfo.GetBinContent(2) / EventInfo.GetBinContent(2))
        #h.Rebin(nrebin)
        h = h.Rebin(len(rebin[h.GetName().split('_')[2]])-1, h.GetName(), array.array('d',rebin[h.GetName().split('_')[2]]))

        #ttbb rescale: TOP-18-002
        if do_ttbb_rescale:
          if   'TTpowhegttbb'   in files: h.Scale(1.2)
          elif 'TTLLpowhegttbb' in files: h.Scale(1.26)

        bSFInfo_nom = fill_bSFInfo(nom_f)
        h_nom = nom_f.Get(histos)
        h_nom = bSFNorm(h_nom, bSFInfo_nom)
        #h_nom.Rebin(nrebin)
        h_nom = h_nom.Rebin(len(rebin[h.GetName().split('_')[2]])-1, h.GetName(), array.array('d',rebin[h.GetName().split('_')[2]]))

      f_new.cd()
      h.Write()


def fill_bSFInfo(f_in):

  bSFInfo_tmp = {}
  bSFInfo_tmp['Ch0_J0'] = f_in.Get("bSFInfo_Ch0_J0")
  bSFInfo_tmp['Ch0_J1'] = f_in.Get("bSFInfo_Ch0_J1")
  bSFInfo_tmp['Ch0_J2'] = f_in.Get("bSFInfo_Ch0_J2")
  bSFInfo_tmp['Ch1_J0'] = f_in.Get("bSFInfo_Ch1_J0")
  bSFInfo_tmp['Ch1_J1'] = f_in.Get("bSFInfo_Ch1_J1")
  bSFInfo_tmp['Ch1_J2'] = f_in.Get("bSFInfo_Ch1_J2")
  bSFInfo_tmp['Ch2_J0'] = f_in.Get("bSFInfo_Ch2_J0")
  bSFInfo_tmp['Ch2_J1'] = f_in.Get("bSFInfo_Ch2_J1")
  bSFInfo_tmp['Ch2_J2'] = f_in.Get("bSFInfo_Ch2_J2")
  if bSFInfo_tmp['Ch2_J0'].Integral() == 0 and bSFInfo_tmp['Ch2_J2'].Integral() == 0:
    bSFInfo_tmp.clear() #If sum is 0, remove items: for data, J0 = 0 in MVA

  return bSFInfo_tmp


def bSFNorm(htmp, infos):

  if any(infos):
    hname = htmp.GetName()
    keystr = ''
    if   'Ch0' in hname: keystr += 'Ch0_'
    elif 'Ch1' in hname: keystr += 'Ch1_'
    elif 'Ch2' in hname: keystr += 'Ch2_'

    if 'h_DNN' in hname:
      #case1: MVA -> j3b2, j3b3, j4b2, j4b3, j4b4
      if   'j3' in hname: keystr += 'J1'
      elif 'j4' in hname: keystr += 'J2'
      infotmp = infos[keystr].Clone()
    else:
      #case2: fullAna -> S1,2,3,5,6,7,8
      if   'S0'in hname: keystr += 'J0'
      elif any(i in hname for i in ['S1','S2','S3','S4']): keystr += 'J1'
      elif any(i in hname for i in ['S5','S6','S7','S8']): keystr += 'J2'
      infotmp = infos[keystr].Clone()
      if 'S4' in hname: infotmp.Add(infos[keystr.replace('J1','J2')], 1.0)

    binnum = 2 #nominal = 1
    if any(i in hname for i in ['__lf', '__hf', '__cferr']):
      binnum = infotmp.GetXaxis().FindBin(str(hname.split('__')[-1]))
    if infotmp.GetBinContent(binnum) > 0:
      htmp.Scale(infotmp.GetBinContent(1)/infotmp.GetBinContent(binnum))

  return htmp


#Starts loop over histogram root files
file_list = os.listdir( os.path.join(base_path, "pre_process") )
if "systamatics" in file_list: file_list.remove("systematics")

pre_path = os.path.join(base_path, "pre_process")

if not os.path.exists( pre_path ):
  os.makedirs( pre_path )

for files in file_list:

  print files

  #Prepare root file
  f = TFile.Open( os.path.join(pre_path, files), "READ")

  histo_list = []
  histo_list = [x.GetName() for x in f.GetListOfKeys()]

  EventInfo = f.Get("EventInfo")
  ScaleWeights = f.Get("ScaleWeights")
  PSWeights = f.Get("PSWeights")
  PDFWeights = f.Get("PDFWeights")
  nScaleWeight = ScaleWeights.Integral()
  nPSWeight = PSWeights.Integral()
  nPDFWeight = PDFWeights.Integral()

  #bSF - 6 histos
  bSFInfo = fill_bSFInfo(f)

  #Prepare nominal file for rescaling
  syst_name = ""
  if "__" in files:
    run_on_syst = True
    syst_name = files.split("__")[1].replace(".root","")
    nom_f = TFile.Open( os.path.join(pre_path, files.replace("__" + syst_name,"")), "READ")
    nom_EventInfo = nom_f.Get("EventInfo")
  else: run_on_syst = False

  #Creat output file, in post_process folder
  post_path = os.path.join(base_path, "post_process")
  if os.path.exists(os.path.join(post_path, files)):
    print files + " exists!"
    continue
  f_new = TFile.Open( os.path.join(post_path, files), "RECREATE")

  #Store nominal names, drop scale vars.
  nominal_list = []
  for histos in histo_list:
    if "__" not in histos: nominal_list.append(histos)
    if run_on_syst: continue
    if "scale" in histos: continue
    if "ps" in histos: continue
    if "pdf" in histos: continue
    h = f.Get(histos)
    h.SetDirectory(ROOT.nullptr)
    if not any(i in h.GetName() for i in ['Info', 'Weight']):
      h = bSFNorm(h, bSFInfo)
      #h.Rebin(nrebin)
      h = h.Rebin(len(rebin[h.GetName().split('_')[2]])-1, h.GetName(), array.array('d',rebin[h.GetName().split('_')[2]]))

      #ttbb rescale: TOP-18-002
      if do_ttbb_rescale:
        if   'TTpowhegttbb'   in files: h.Scale(1.2)
        elif 'TTLLpowhegttbb' in files: h.Scale(1.26)

    else: pass

    h.Write()

  #Store envelope, rescale histos
  for histos in nominal_list:

    if nScaleWeight > 0: write_envelope("scale", 6, ScaleWeights)
    if nPSWeight > 0: write_envelope("ps", 4, PSWeights)
    if nPDFWeight > 0:
      if 'STTH' in files: write_envelope("pdf", 30, PDFWeights)
      else:               write_envelope("pdf", 103, PDFWeights)
    if run_on_syst: rescale([], nom_EventInfo)

  f_new.Write()
  f_new.Close()
  f.Close()
