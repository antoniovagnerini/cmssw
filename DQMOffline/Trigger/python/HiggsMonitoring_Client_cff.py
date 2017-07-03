import FWCore.ParameterSet.Config as cms

from DQMOffline.Trigger.MssmHbbMonitoring_Client_cfi import *

higgsClient = cms.Sequence(
   mssmHbbHLTEfficiency
)
