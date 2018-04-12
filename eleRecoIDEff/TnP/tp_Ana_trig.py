import FWCore.ParameterSet.Config as cms

import sys
args =sys.argv[1:]
if len(args) < 3: 
   type = "GED"
   dataOrMC = "Data"
else: 
      type = args[1]
      dataOrMC = args[2]
print "Will run type ", type, " on ", dataOrMC

process = cms.Process("TagProbe")
process.load('FWCore.MessageService.MessageLogger_cfi')
process.source = cms.Source("EmptySource")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )    
PDFName = "expoPlusBkg"
if dataOrMC == "MC":
   PDFName = "expo"

pass_str = "pass%s" % type

EFFICIENCYSET =cms.PSet(
        recoIDEle_1bin = cms.PSet(
            EfficiencyCategoryAndState = cms.vstring(pass_str,"true"),
            UnbinnedVariables = cms.vstring("diAcop"),
            BinnedVariables = cms.PSet(
                pt = cms.vdouble(0, 20),
                abseta = cms.vdouble(0, 2.4),
            ),
         BinToPDFmap = cms.vstring(PDFName)
        ),
        recoIDEle_EBEE = cms.PSet(
            EfficiencyCategoryAndState = cms.vstring(pass_str,"true"),
            UnbinnedVariables = cms.vstring("diAcop"),
            BinnedVariables = cms.PSet(
                pt = cms.vdouble(0, 20),
                abseta = cms.vdouble(0, 1.5, 2.4),
            ),
         BinToPDFmap = cms.vstring(PDFName)
        ),
        recoIDEle_abseta = cms.PSet(
            EfficiencyCategoryAndState = cms.vstring(pass_str,"true"),
            UnbinnedVariables = cms.vstring("diAcop"),
            BinnedVariables = cms.PSet(
                pt = cms.vdouble(0, 20),
                abseta = cms.vdouble(0, 0.5, 1, 1.5, 2, 2.4),
            ),
         BinToPDFmap = cms.vstring(PDFName)
        ),
        recoIDEle_pt0 = cms.PSet(
            EfficiencyCategoryAndState = cms.vstring(pass_str,"true"),
            UnbinnedVariables = cms.vstring("diAcop"),
            BinnedVariables = cms.PSet(
                pt = cms.vdouble(0, 2, 4, 6, 10, 14, 20),
                abseta = cms.vdouble(0, 1.5),
            ),
         BinToPDFmap = cms.vstring(PDFName)
        ),
        recoIDEle_pt1 = cms.PSet(
            EfficiencyCategoryAndState = cms.vstring(pass_str,"true"),
            UnbinnedVariables = cms.vstring("diAcop"),
            BinnedVariables = cms.PSet(
                pt = cms.vdouble(0, 2, 4, 6, 10, 14, 20),
                abseta = cms.vdouble(1.5, 2.4),
            ),
         BinToPDFmap = cms.vstring(PDFName)
        )
)


process.tagProbeFitTreeAnalyzer = cms.EDAnalyzer("TagProbeFitTreeAnalyzer",
    # IO parameters:
    InputFileNames = cms.vstring("file:/afs/cern.ch/user/e/echapon/workspace/public/LbyL/EmiliensCode/eleRecoIDEff/output%sAll.root" % dataOrMC),
    InputDirectoryName = cms.string("eleTree"),
    InputTreeName = cms.string("fitter_tree"),
    OutputFileName = cms.string("tnp_Ana_%s_%s_recoIDEle.root" % ( dataOrMC, type )),
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
                         diMass           = cms.vstring("diele Mass", "0", "100.0", "GeV/c^{2}"), 
                         diPt             = cms.vstring("diele Pt", "0", "50.0", "GeV/c"), 
                         diRap            = cms.vstring("diele y", "-3", "3", ""), 
                         diAcop           = cms.vstring("diele acop", "0", "0.1", ""), 
                         pt               = cms.vstring("Probe p_{T}", "0.0", "20", "GeV/c"),
                         eta              = cms.vstring("Probe #eta", "-2.4", "2.4", ""),
                         abseta           = cms.vstring("Probe |#eta|", "0", "2.4", ""),
    ),
    # defines all the discrete variables of the probes available in the input tree and intended for use in the efficiency calculations
    Categories = cms.PSet(
                        passGED = cms.vstring("passGED", "dummy[true=1,false=0]"),
                        passGSF = cms.vstring("passGSF", "dummy[true=1,false=0]"),

    ),

    # defines all the PDFs that will be available for the efficiency calculations; uses RooFit's "factory" syntax;
    # each pdf needs to define "signal", "backgroundPass", "backgroundFail" pdfs, "efficiency[0.9,0,1]" and "signalFractionInPassing[0.9]" are used for initial values  
    PDFs = cms.PSet(
        # Nominal
	    expoPlusBkg = cms.vstring(
		    "Exponential::signal(diAcop, decaySig[-6.8e2,-1e4,0])",
	    	"Exponential::backgroundPass(diAcop, decayBkgPass[-12,-1e4,0])",
		    "Exponential::backgroundFail(diAcop, decayBkgFail[-12,-1e4,0])",
	    	"efficiency[0.9,0.,1]",
		    "signalFractionInPassing[0.9]",
    	),    
        # Nominal
	    expo = cms.vstring(
		    "Exponential::signal(diAcop, decaySig[-6.8e2,-1e4,0])",
	    	"Uniform::backgroundPass(diAcop)",
	    	"Uniform::backgroundFail(diAcop)",
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
