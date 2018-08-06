# Geant4 - an Object-Oriented Toolkit for Simulation in HEP                #

 This is a modified version of the AnaEx01 example in the "extended" folder
 of the examples.

 The code has been adapted to simulate a scintillation detector.
                         
## Detector description ##
 
 The detector is a plate made of a scintillating material (polystyrene).
 	
 Four parameters define the detector:
    * the material of the scintillator,
    * the thickness of the plate,
    * the lenght of theshort side,
    * the length of the long side. 
 
 The geometry is constructed in DetectorConstruction class,
  
## Physics list ##
 
   The particle's type and the physic processes which are available are set
   in the PhysicsList class. The actual setting is done in the
   following classes:
    * GeneralPhysics,
    * EMPhysics,
    * MuonPhysics. 
  
## Action Initialization ##

   A newly introduced class, ActionInitialization, 
   instantiates and registers to Geant4 kernel all user action classes
    
## An event: PrimaryGeneratorAction ##

   Primaries can be either generated with the General Particle Source (GPS) or
   with the Particle Gun. The GPS should be used only for a quick test via macro,
   whereas for generating primaries in a more complicated way the Particle Gun
   should be used (i.e. for efficiently samplying random values from a
   distribution).

## Histograms ##

 AnaEx01 can produce 3 histograms :
  
  EScint   : total energy deposit in scintillator per event,
  LScint   : total track length of particles in scintillator per event 	
	     computed through Geant4 methods,
  myLScint : total track length of particles in scintillator per event
	     computed via geometrical reasoning.
 
 These histos are booked in HistoManager and filled from 
 EventAction.
 
 The format of the histogram file can be : root (default),
 xml, csv. Include correct g4nnn.hh in HistoManager.hh 
 
## How to build ## 

 mkdir <build dir>
 cd <build dir>
 cmake ../AnaEx01
 make

