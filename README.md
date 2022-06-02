# geant4_powtex

This repository is intended to store the files for the simulation of the POWTEX detector, 
the version of the Jalousie detector technology developed by CDT Heidelberg for 
the POWTEX diffractometer. 

Written by Irina Stefanescu, ESS Detector Group, 2022. 

Tested with geant4-11.0.2. 

Tested on Mac iOS Catalina and CentOS8_2. 

The simulation strategy is explained in detail in the following reference:

    I. Stefanescu, M. Christensen, R. Hall-Wilton, S. Holm-Dahlin, K. Iversen, 
    M. Klein, D. Mannix, J. Schefer, C.J. Schmidt, W. Schweika and U. Stuhr
    Performance study of the Jalousie detector baseline design for the ESS thermal 
    powder diffractometer HEIMDAL through GEANT4 simulations, 
    Journal of Instrumentation, 14 P10020, 2019. 

	https://iopscience.iop.org/article/10.1088/1748-0221/14/10/P10020 

1 - GEOMETRY DEFINITION
------------------------
 
The POWTEX detector is built out of large wire counters with trapezoidal shapes called segments. Each segment consists of two identical Multi Wire Proportional Counters sharing a segmented cathode. The Boron layer (made of enriched 10B4C) is applied on the inner sides of the segment walls and on each side of the segmented cathode. In order to facilitate the transportation and mounting of the segments in the detector frame,  modules consisting of 8 segments were built. In order to increase the detection efficiency, the individual modules are mounted in the detector frame such that the angle between the incoming neutron and the Boron layer is 10 deg.  

The cathode is segmented into 192 strips that have a constant angular resolution of 0.468 
deg for a distance sample2detector of 800 cm. Each MPWC counter has 16 wires. In the real detector the wires and strips are independently read-out and the position of the detected neutron is determined by taking the coincidence signal between the wires and strips. This leads to a sensitive detector volume consisting of 192 (strips) x 16 (wires) x 2 (counters) = 6144 voxels.  

In the GEANT4 codes the detector voxels are implemented as small volumes made of the detector counting gas. These gas voxels are arranged side by side to fill the space between the central cathode and detector housing, which are in close contact with a thin layer of the neutron converter (10B4C) (in the real detector the neutron converter is deposited on the housing and cathode Aluminum-alloy with the help of a high-temperature evaporation process called PVD). The primary particles (neutrons) hit the neutron converter and get absorbed in the neutron converter with some probability. One of the reaction products (either the 7Li-ion or the alpha -article) will escape the Boron converter (with some probability), and travel through the gas voxels that are closest to the escape positon until it loses its entire kinetic energy through collisions with the gas material (Ar-CO2 mixture) and comes to rest. The gas voxel that records the largest part of the kinetic energy of the reaction product is deemed the detection voxel.   

Please note that there is no electric field included in the model. GEANT4 cannot deal with the transportation and physics of the low-energy electrons generated by the ionization of the gas. An interface with GARFIELD is possible to do with some substantial computing effort, but it is outside the scope of this work.   

The detector setup is defined/constructed in the DetectorMantelConstruct.icc inline definition file by using the technical specifications from CDT. The detector segment is built by using the AssemblyVolume() class comprising the 5 independent daughter volumes: detector housing, the Boron converter on the detector housing, the Boron layer on one side of the cathode, the Boron layer on the other side of the cathode and the cathode substrate. Furthermore, the detector module is an assembled volume made of 8 identical copies of the segment. The module is then copied as many times as wished by the user and positioned at 800 cm around the sample position in cylindrical geometry. 

The PowtexConstruction() class is responsible for the construction and placement of the  detector in the world volume. The code structure consisting of *icc files for the definition of the detector is inherited from the simulation code developed for the DREAM detector system, which consists of several independent detector types. The PowtexConstruction.cc file also contains the declarations of some of the main variables used to build the Mantle segment. Some of them (for example Bthick, the thickness of the Boron layer) are common for all DREAM detector systems. Since this code is derived from the Mantle code for DREAM, the same structure was kept, although here it might look like a totally random list of variables. 

In the real detector the common cathode separating the two wire counters in the detector  segment is segmented into strips that cover the same solid angle with respect to the sample position. The strips have trapezoidal shapes and their size increases from the center of the segment to the edges (as their distance from the sample increases). Also, the strips belonging to the same wire counter (same side of the cathode) are rotated and mirrored with respect to the cathode center. The voxels in the second counter (the other side of the cathode) are also a rotated and mirrored copy with respect to the cathode plane of the voxels in the first counter. The gas voxels are being built taking into account the geometry of the real detector voxels (determined by the wire pitch and strip size) and the symmetry of the system. Therefore, out of the 6144 voxels of a detector segment one has to create only 192/2 x 16 = 1536 individual voxels volumes. The sensitive area of the segment will be filled with gas voxels belonging to 4 groups. Thus, in the geometry construction class  the naming 'top left' and  'top right' refer to the voxels filling the upper counter, left and right of the cathode center, respectively, (just remember that in the POWTEX Mantle detector the segments are mounted in horizontal position), while the naming  'bottom left' and 'bottom right' refer to the counter below the cathode, left and right of the cathode center, respectively.     
  
2 - PHYSICS LIST
------------------
 
 The physics processes are defined in the built-in library QGSP_BERT_HP. 
 This library contains the standard electromagnetic processes and the high 
 precision neutron model valid for neutrons with energies below 20 MeV. 
 It is one of the standard libraries recommended for use in thermal neutron scattering 
 applications, radiation protection and shielding applications. 
 
3 - PRIMARY GENERATOR
--------------------------

The primary generator is defined in the PrimaryGenerator class. 
This class reads the primary information from a Vitess Ascii file and generates a 	 
primary vertex for each neutron trajectory in the file. The vertex position is at the sample position, which corresponds to the origin of the center of reference in the GEANT4 
simulation. 
The neutron travels in the direction specified by the SetMomentum() method. 
The starting time of the neutrons from the sample position corresponds to the 
time-of-flight value read from the ascii file, which is the flight time through the 
VITESS instrument. The final time-of-flight equals the VITESS time-of-flight plus the 
travel time of the neutron from the sample until it disappears by absorption 
(detection) in the detector volume or scatters off some material present in the setup and leaves the World volume (the mother volume). 

Two sample files are provided as an example with the code. These files ar located in 
the directory called VitessDataPOWTEX. 

The 'noutascii_reduced.dat' file contains the first 100 neutrons events from the 
noutascii_biosample.dat. It should be used for when visualization is wanted and for 	
debugging purposes, see point 6 below. The large sample file, called 'noutascii_biosample.dat' contains ~2*10^8 neutron trajectories generated by Vitess for a biosample file. A calculation of the detector response consisting of 30 Mantle modules and a sample file of this size takes around ~10 h on my MacBookPro 8-Core Intel Core i9, 2.4 GHz.   

Before compiling the application please edit the PrimaryGenerator.cc file and replace the path to the sample file with your own path (lines 49-50). 

The file PrimaryGeneratorAction.cc.GPS is also provided for the case the user wishes to 
generate monochromatic neutrons from a source located at the sample position in order to test the detector setup. Copy this file to PrimaryGeneratorAction.cc, compile and run as usual (see pct. 5):  

% cp PrimaryGeneratorAction.cc PrimaryGeneratorAction.cc.file
% cp PrimaryGeneratorAction.cc.GPS PrimaryGeneratorAction.cc
% ./compile_Powtex_cmake.sh
% ./Powtex  run.mac

   
4 - TRACKING
------------------

Performed by the SteppingAction class. The only tracks interesting for this application are those from the charged particles (7Li and alpha) that reach the gas phase (gas voxel) inside the detector. 

The MakeImprint() method of the AssemblyVolume() class used to place the various volumes in the world volume generates automatically the physical volume name. For the gas voxels, the name looks like this: 

		av_XX_impr_YY_GasLayer_BR_M_pv_ZZ

where: 

	XX - assembly volume instance number
	YY - assembly volume imprint number 
	ZZ - index of the associated logical volume
	
	GasLayer_BR_M__   -  name of the placed logical volume in the assembly 
			
			where: 
					GasLayer - name of the gas voxel logical volume 
					BR - voxel belongs to the 'bottom left' group (see the Geometry 		
						   description) 
					M - volume that belongs to the Mantle detector system (inhereted from 
						DREAM, which consists of several systems)   

The imprint number is used by the code to extract the module and segment number of the gas voxel that is entered by the charged particle escaping the Boron layer. The module and  segment number along with the index of the associated logical volume (the 'vox_no' variable) are part of the information on the detected neutron that is saved in the output ascii file (myfile_info.txt) and it will be further processed offline with the help of ROOT scripts.    
  
5 - USAGE
--------------
 
  - compile the code with the help of the script after setting the correct path for 		
  	the variable 'DGeant4_DIR':
  
  % ./compile_Powtex_cmake.sh
  
  - execute the code in 'batch' mode from macro file:
  
	% ./Powtex  run.mac

  - execute the code in 'interactive mode' with visualization (see also Pct. 6 below):
  
	% ./Powtex
	.... 
	Idle> /control/execute vis.mac
	....
	Idle> exit
	Idle> exit
	
6 - USAGE for debugging and learning purposes
    --------------------------------------------
    
    If you want to get familar with the model and investigate the tracking results you can 
    save the standard GEANT4 output information in a file:
    
	% ./Powtex  run.mac | tee test.out
	
    The standard Geant4 verbosity can be activated via the following UI commands in the 
    run.mac macro:
    
	 /run/verbose 1
	 /event/verbose 1
 	 /tracking/verbose 1 
 	 
 	 It is recommended to run these 'debugging' runs including the visualisation of the 
 	 setup with the 'noutascii_reduced.dat' file included in the VitessDataPOWTEX 
 	 directory. 
 	 This reduced file contains only the first 100 neutron trajectories from the large 
 	 file. 
 	 
 	 In the case of running with a large VITESS input file it is recommended to set the UI 
 	 commands above to 0 in order to avoid a slowing down the calculation.  
    
6 - DATA ANALYSIS
    --------------------
	