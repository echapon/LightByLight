import FWCore.ParameterSet.Config as cms

import sys
args =sys.argv[1:]
if len(args) < 2: 
   dataOrMC = "Data"
else: 
      dataOrMC = args[1]
print "Will run on ", dataOrMC

process = cms.Process("TagProbe")
process.load('FWCore.MessageService.MessageLogger_cfi')
process.source = cms.Source("EmptySource")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )    
PDFName = "nompdf"


EFFICIENCYSET =cms.PSet(
        recoid_1bin = cms.PSet(
            EfficiencyCategoryAndState = cms.vstring("nPho_notag","true","phoIDok_notag","true"),
            UnbinnedVariables = cms.vstring("probetkAco"),
            BinnedVariables = cms.PSet(
                probetkMinDpt = cms.vdouble(2, 20), # to be replaced with et(tag)-pt(tk)
                # pt = cms.vdouble(0, 20),
                probetkEta = cms.vdouble(-2.4, 2.4),
            ),
         BinToPDFmap = cms.vstring(PDFName)
        ),
        recoid_pt0 = cms.PSet(
            EfficiencyCategoryAndState = cms.vstring("nPho_notag","true","phoIDok_notag","true"),
            UnbinnedVariables = cms.vstring("probetkAco"),
            BinnedVariables = cms.PSet(
                # pt = cms.vdouble(0, 2, 3, 4, 5, 6, 7, 10, 14, 20),
                probetkMinDpt = cms.vdouble(2, 4, 6, 8, 20),
                probetkEta = cms.vdouble(-2.4, 2.4),
            ),
         BinToPDFmap = cms.vstring(PDFName)
        )
)


process.tagProbeFitTreeAnalyzer = cms.EDAnalyzer("TagProbeFitTreeAnalyzer",
    # IO parameters:
    InputFileNames = cms.vstring("file:/afs/cern.ch/user/e/echapon/workspace/private/LightByLight/EmiliensCode/hardbrem/TnP/output%sAll_recoid.root" % dataOrMC),
    InputDirectoryName = cms.string("tpTree_recoid"),
    InputTreeName = cms.string("fitter_tree"),
    OutputFileName = cms.string("tnp_Ana_%s_recoid.root" % ( dataOrMC)),
    #numbrer of CPUs to use for fitting
    NumCPU = cms.uint32(4),
    # specifies whether to save the RooWorkspace containing the data for each bin and
    # the pdf object with the initial and final state snapshots
    binnedFit = cms.bool(False),
    #binsForFit = cms.uint32(30),
    binsForMassPlots = cms.uint32(30),
    SaveWorkspace = cms.bool(False),
    
    # defines all the real variables of the probes available in the input tree and intended for use in the efficiencies
    Variables = cms.PSet(
                         probetkAco       = cms.vstring("Probe-tag aco", "0.0", "0.5", ""),
                         probetkPt        = cms.vstring("Probe p_{T}", "0.0", "2", "GeV/c"),
                         probetkMinDpt    = cms.vstring("p_{T}^{e} - p_{T}^{tk}", "2.0", "20", "GeV/c"),
                         probetkEta       = cms.vstring("Probe #eta", "-2.4", "2.4", ""),
    ),
    # defines all the discrete variables of the probes available in the input tree and intended for use in the efficiency calculations
    Categories = cms.PSet(
                        nPho_notag    = cms.vstring("nPho_notag", "dummy[true=1,false=0]"),
                        phoIDok_notag = cms.vstring("phoIDok_notag", "dummy[true=1,false=0]"),

    ),

    # defines all the PDFs that will be available for the efficiency calculations; uses RooFit's "factory" syntax;
    # each pdf needs to define "signal", "backgroundPass", "backgroundFail" pdfs, "efficiency[0.9,0,1]" and "signalFractionInPassing[0.9]" are used for initial values  
    PDFs = cms.PSet(
        # Nominal
	    nompdf = cms.vstring(
          "EXPR::signal('1./pow(probetkAco-asig,2)',{probetkAco, asig[-3.3e-2]})",#,-8e-2,-2e-2]})",
	    	"Exponential::backgroundFail(probetkAco, decayBkgFail[-4.6,-2,-6])",
          # "Exponential::backgroundFail(diAcop, decayBkgFail[-1000,-1e4,-10])",
          "Exponential::backgroundPass(probetkAco, decayBkgPass[-4.6,-2,-6])",
	    	"efficiency[0.9,0.,1]",
		    "signalFractionInPassing[0.9]",
    	),    
    ),

    # defines a set of efficiency calculations, what PDF to use for fitting and how to bin the data;
    # there will be a separate output directory for each calculation that includes a simultaneous fit, side band subtraction and counting. 
    Efficiencies = EFFICIENCYSET 
)

process.fitness = cms.Path(
    process.tagProbeFitTreeAnalyzer
)
