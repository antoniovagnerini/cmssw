#ifndef MultiTrackValidator_h
#define MultiTrackValidator_h

/** \class MultiTrackValidator
 *  Class that prodecs histrograms to validate Track Reconstruction performances
 *
 *  \author cerati
 */
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
#include "Validation/RecoTrack/interface/MultiTrackValidatorBase.h"
#include "Validation/RecoTrack/interface/MTVHistoProducerAlgoForTracker.h"
#include "SimDataFormats/Associations/interface/VertexToTrackingVertexAssociator.h"
#include "CommonTools/RecoAlgos/interface/RecoTrackSelectorBase.h"
#include "SimTracker/TrackAssociation/interface/ParametersDefinerForTP.h"
#include "CommonTools/Utils/interface/DynArray.h"

class MultiTrackValidator : public DQMEDAnalyzer, protected MultiTrackValidatorBase {
 public:
  /// Constructor
  MultiTrackValidator(const edm::ParameterSet& pset);
  
  /// Destructor
  virtual ~MultiTrackValidator();


  /// Method called once per event
  void analyze(const edm::Event&, const edm::EventSetup& ) override;
  /// Method called to book the DQM histograms
  void bookHistograms(DQMStore::IBooker&, edm::Run const&, edm::EventSetup const&) override;


 protected:
  //these are used by MTVGenPs
  bool UseAssociators;
  const bool parametersDefinerIsCosmic_;
  const bool calculateDrSingleCollection_;
  const bool doPlotsOnlyForTruePV_;
  const bool doSummaryPlots_;
  const bool doSimPlots_;
  const bool doSimTrackPlots_;
  const bool doRecoTrackPlots_;
  const bool dodEdxPlots_;
  const bool doPVAssociationPlots_;
  const bool doSeedPlots_;
  const bool doMVAPlots_;
  std::unique_ptr<MTVHistoProducerAlgoForTracker> histoProducerAlgo_;

 private:
  const TrackingVertex::LorentzVector *getSimPVPosition(const edm::Handle<TrackingVertexCollection>& htv) const;
  const reco::Vertex::Point *getRecoPVPosition(const edm::Event& event, const edm::Handle<TrackingVertexCollection>& htv) const;
  void tpParametersAndSelection(const TrackingParticleRefVector& tPCeff,
                                const ParametersDefinerForTP& parametersDefinerTP,
                                const edm::Event& event, const edm::EventSetup& setup,
                                const reco::BeamSpot& bs,
                                std::vector<std::tuple<TrackingParticle::Vector, TrackingParticle::Point> >& momVert_tPCeff,
                                std::vector<size_t>& selected_tPCeff) const;
  size_t tpDR(const TrackingParticleRefVector& tPCeff,
              const std::vector<size_t>& selected_tPCeff,
              DynArray<float>& dR_tPCeff) const;
  void trackDR(const edm::View<reco::Track>& trackCollection,
               const edm::View<reco::Track>& trackCollectionDr,
               DynArray<float>& dR_trk) const;

  std::vector<edm::EDGetTokenT<reco::TrackToTrackingParticleAssociator>> associatorTokens;
  std::vector<edm::EDGetTokenT<reco::SimToRecoCollection>> associatormapStRs;
  std::vector<edm::EDGetTokenT<reco::RecoToSimCollection>> associatormapRtSs;

  edm::EDGetTokenT<edm::ValueMap<unsigned int> > tpNLayersToken_;
  edm::EDGetTokenT<edm::ValueMap<unsigned int> > tpNPixelLayersToken_;
  edm::EDGetTokenT<edm::ValueMap<unsigned int> > tpNStripStereoLayersToken_;


  using MVACollection = std::vector<float>;
  using QualityMaskCollection = std::vector<unsigned char>;
  std::vector<std::vector<std::tuple<edm::EDGetTokenT<MVACollection>, edm::EDGetTokenT<QualityMaskCollection> > > > mvaQualityCollectionTokens_;

  std::string dirName_;

  bool useGsf;
  const double simPVMaxZ_;
  // select tracking particles 
  //(i.e. "denominator" of the efficiency ratio)
  TrackingParticleSelector tpSelector;				      
  CosmicTrackingParticleSelector cosmictpSelector;
  TrackingParticleSelector dRtpSelector;				      
  TrackingParticleSelector dRtpSelectorNoPtCut;
  std::unique_ptr<RecoTrackSelectorBase> dRTrackSelector;

  edm::EDGetTokenT<SimHitTPAssociationProducer::SimHitTPAssociationList> _simHitTpMapTag;
  edm::EDGetTokenT<edm::View<reco::Track> > labelTokenForDrCalculation;
  edm::EDGetTokenT<edm::View<reco::Vertex> > recoVertexToken_;
  edm::EDGetTokenT<reco::VertexToTrackingVertexAssociator> vertexAssociatorToken_;

  std::vector<MonitorElement *> h_reco_coll, h_assoc_coll, h_assoc2_coll, h_simul_coll, h_looper_coll, h_pileup_coll;
  std::vector<MonitorElement *> h_assoc_coll_allPt, h_simul_coll_allPt;
};


#endif
