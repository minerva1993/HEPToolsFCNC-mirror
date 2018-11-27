def input_variables(ch):
  var_list = []
  if ch == "STFCNC":
    var_list = ['jet0pt', 'jet0eta', 'jet0m', 'jet0csv', 'jet1pt', 'jet1eta', 'jet1m', 'jet1csv',
                'jet2pt', 'jet2eta', 'jet2m', 'jet2csv'
                'jet12pt', 'jet12eta', 'jet12deta', 'jet12dphi', 'jet12dR', 'jet12m',
                'lepWpt', 'lepWdphi', 'lepWm', 'lepTdphi', 'lepTm',
                'jet0lepdR', 'jet1lepdR', 'jet2lepdR', 'jet01dR', 'jet02dR', 'jet12_lepdR', 'jet12_0dR', 'lepTjet12dphi']

  elif ch == "TTFCNC":
    var_list = ['jet0pt', 'jet0eta', 'jet0m', 'jet0csv', 'jet1pt', 'jet1eta', 'jet1m', 'jet1csv',
                'jet2pt', 'jet2eta', 'jet2m', 'jet2csv', 'jet3pt', 'jet3eta', 'jet3m', 'jet3csv', 
                'jet12pt', 'jet12eta', 'jet12deta', 'jet12dphi', 'jet12dR', 'jet12m',
                'jet23pt', 'jet23eta', 'jet23deta', 'jet23dphi', 'jet23dR', 'jet23m',
                'jet31pt', 'jet31eta', 'jet31deta', 'jet31dphi', 'jet31dR', 'jet31m',
                'lepWpt', 'lepWdphi', 'lepWm', 'lepTdphi', 'lepTm',
                'hadTpt', 'hadTeta', 'hadT12_3deta', 'hadT23_1deta', 'hadT31_2deta',
                'hadT12_3dphi', 'hadT23_1dphi', 'hadT31_2dphi',
                'hadT12_3dR', 'hadT23_1dR', 'hadT31_2dR', 'hadTm',
                'jet0lepdR', 'jet1lepdR', 'jet2lepdR', 'jet3lepdR', 'jet01dR', 'jet02dR', 'jet03dR', 
                'jet12_lepdR', 'jet23_lepdR', 'jet31_lepdR', 'jet12_0dR', 'jet23_0dR', 'jet31_0dR',
                'lepTjet12dphi', 'lepTjet23dphi', 'lepTjet31dphi', 'hadT_jet0dR',]

  elif ch == "TTBKG":
    var_list = ['jet0pt', 'jet0eta', 'jet0m', 'jet0csv', 'jet1pt', 'jet1eta', 'jet1m', 'jet1csv',
                'jet2pt', 'jet2eta', 'jet2m', 'jet2csv', 'jet3pt', 'jet3eta', 'jet3m', 'jet3csv',
                'jet12pt', 'jet12eta', 'jet12deta', 'jet12dphi', 'jet12dR', 'jet12m',
                'jet23pt', 'jet23eta', 'jet23deta', 'jet23dphi', 'jet23dR', 'jet23m',
                'jet31pt', 'jet31eta', 'jet31deta', 'jet31dphi', 'jet31dR', 'jet31m',
                'lepWpt', 'lepWdphi', 'lepWm', 'lepTdphi', 'lepTm',
                'hadTpt', 'hadTeta', 'hadT12_3deta', 'hadT23_1deta', 'hadT31_2deta',
                'hadT12_3dphi', 'hadT23_1dphi', 'hadT31_2dphi',
                'hadT12_3dR', 'hadT23_1dR', 'hadT31_2dR', 'hadTm',
                'jet0lepdR', 'jet1lepdR', 'jet2lepdR', 'jet3lepdR', 'jet01dR', 'jet02dR', 'jet03dR',               
                'jet12_lepdR', 'jet23_lepdR', 'jet31_lepdR', 'jet12_0dR', 'jet23_0dR', 'jet31_0dR',
                'lepTjet12dphi', 'lepTjet23dphi', 'lepTjet31dphi', 'hadT_jet0dR',]

  else: var_list = []

  return var_list


def gen_label(ch):  
  if ch == "STFCNC"  : label = 1011
  elif ch == "TTFCNC": label = 1111
  elif ch == "TTBKG" : label = 1111
  else               : label = 0
  return label

def train_files(ch):
  file_list = []
  if ch == "STFCNC":
    file_list = ['deepReco_STTH1L3BHct_000.h5', 'deepReco_STTH1L3BHut_000.h5',]

  elif ch == "TTFCNC":
    file_list = ['deepReco_TTTH1L3BaTLepHut_000.h5', 'deepReco_TTTH1L3BTLepHut_000.h5', 'deepReco_TTTH1L3BaTLepHct_000.h5', 'deepReco_TTTH1L3BTLepHct_000.h5']

  elif ch == "TTBKG":
    file_list = ['deepReco_TTpowhegttbb_000.h5', 'deepReco_TTpowhegttbj_000.h5', 'deepReco_TTpowhegttcc_000.h5',
                 'deepReco_TTpowhegttlf_000.h5', 'deepReco_TTpowhegttother_000.h5',
                 'deepReco_TTpowhegttbb_001.h5', 'deepReco_TTpowhegttbj_001.h5', 'deepReco_TTpowhegttcc_001.h5',
                 'deepReco_TTpowhegttlf_001.h5', 'deepReco_TTpowhegttother_001.h5',]

  else: file_list = []

  return file_list
