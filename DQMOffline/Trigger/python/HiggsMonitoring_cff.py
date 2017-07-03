import FWCore.ParameterSet.Config as cms

from DQMOffline.Trigger.MssmHbbMonitoring_cff import *


higgsMonitorHLT = cms.Sequence(
   mssmHbbMonitorHLT
)

