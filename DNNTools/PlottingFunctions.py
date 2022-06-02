import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from printing_utils import *

from tdrstyle_all import *
import tdrstyle_all as TDR

def plot_losses(history, mode='loss', name='history', min_epoch=0, dynamic=True, writeExtraText = True, extraText  = 'Simulation', extraText2 = 'Work in progress', lumi_text=''):
    TDR.writeExtraText = writeExtraText
    TDR.extraText = extraText
    TDR.extraText2 = extraText2
    TDR.cms_lumi_TeV = lumi_text
    print blue('    --> Plotting '+mode+' now...')
    isLoss = mode =='loss'
    if dynamic:
        ranges = np.array(history[mode]+history['val_'+mode])
        ranges = (ranges.min()*0.9, ranges.max()*1.1)
    else:
        ranges = (0, 3 if isLoss else 1.05)
    x,y,g = ({},{},{})
    modes = [mode,'val_'+mode]
    for m in modes:
        x[m] = array('d',range(1,1+len(history[m][min_epoch:])))
        y[m] = array('d',history[m][min_epoch:])
    canv = tdrCanvas(mode, 0, len(x[mode])+1, ranges[0], ranges[1], 'Number of training epochs', 'Loss' if isLoss else 'Prediction accuracy', kSquare)
    leg = tdrLeg(0.60, 0.70 if isLoss else 0.20, 0.95, 0.89 if isLoss else 0.39, 0.035, 42, rt.kBlack)
    for m in modes:
        g[m] = rt.TGraph(len(x[m]), x[m], y[m])
        g[m].SetLineWidth(2)
        color = rt.kAzure+2 if m == mode else rt.kOrange+1
        tdrDraw(g[m], 'L', mcolor=color, lcolor=color)
        leg.AddEntry(g[m], 'Training set' if m == mode else 'Validation set' , 'l')
    canv.SaveAs(name+'.pdf')
    canv.Close()


def plot_rocs(rocs={}, name='ROCs', x_title='Signal efficiency', y_title='Background efficiency', writeExtraText = True, extraText  = 'Simulation', extraText2 = 'Work in progress', lumi_text=''):
    TDR.writeExtraText = writeExtraText
    TDR.extraText = extraText
    TDR.extraText2 = extraText2
    TDR.cms_lumi_TeV = lumi_text
    print blue('    --> Plotting ROCs now...')

    canv = tdrCanvas('ROCs', 0, 1.1, 1e-04, 1.2, x_title, y_title, kSquare)
    canv.SetLogy(True)
    leg = tdrLeg(0.60, 0.70, 0.95, 0.89, 0.035, 42, rt.kBlack)
    for roc, info in rocs.items():
        color = info[color]
        roc.SetLineWidth(2)
        tdrDraw(roc, 'L', mcolor=color, lcolor=color)
        leg.AddEntry(roc, info['legendtext'], 'l')
    canv.SaveAs(name+'.pdf')
    canv.Close()
#
# 'Bkg: '+classtitles[i] + ', AUC: '+str(round(aucss_lum[i][0],3)),
# plt.xticks(np.arange(0.1,1.1,0.1))
# plt.grid(True, which='both')
# title = 'ROC_val_class%i' % cl
# if use_best_model: title += '_best'
# else: title += '_last'
# title += '.pdf'
# fig.savefig(os.path.join(plotfolder, title))
# plt.close()



def plot_losses_python(history, mode='loss', name='history', min_epoch=0, dynamic=True):
    print blue('    --> Plotting '+mode+' now...')
    isLoss = mode =='loss'
    plt.clf()
    fig = plt.figure()
    plt.plot(range(1,1+len(history[mode][min_epoch:])), history[mode][min_epoch:], label='Training set')
    plt.plot(range(1,1+len(history['val_'+mode][min_epoch:])), history['val_'+mode][min_epoch:], label='Validation set')
    plt.grid()
    plt.legend(loc='upper right' if isLoss else 'lower right')
    plt.xlabel('Number of training epochs')
    plt.ylabel('Loss' if isLoss else 'Prediction accuracy')
    plt.xlim(left=0)
    if dynamic:
        ranges = np.array(history[mode]+history['val_'+mode])
        ranges = (ranges.min()*0.9, ranges.max()*1.1)
    else:
        ranges = (0, 3 if isLoss else 1.05)
    plt.ylim(ranges)
    plt.savefig(name+'.pdf')
    plt.close()