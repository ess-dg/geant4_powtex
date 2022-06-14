# geant4_powtex

This repository is intended to store the code for the simulation of the POWTEX detector, 
the version of the Jalousie detector technology developed by CDT Heidelberg for 
the POWTEX diffractometer at FRM2. 

Written by Irina Stefanescu, ESS Detector Group, 2022. 

irina.stefanescu@ess.eu

Tested with geant4-11.0.2 in May 2022.  

Tested on Mac iOS Catalina and CentOS8_2 in February 2022. 

The simulation strategy is explained in detail in the following reference:

    I. Stefanescu, M. Christensen, R. Hall-Wilton, S. Holm-Dahlin, K. Iversen, 
    M. Klein, D. Mannix, J. Schefer, C.J. Schmidt, W. Schweika and U. Stuhr
    Performance study of the Jalousie detector baseline design for the ESS thermal 
    powder diffractometer HEIMDAL through GEANT4 simulations, 
    Journal of Instrumentation, 14 P10020, 2019. 

(https://iopscience.iop.org/article/10.1088/1748-0221/14/10/P10020)
	
The usage of the code is sketched in the figures included below, showing the simulation flow:

<img width="717" alt="Screenshot 2022-06-08 at 13 31 12" src="https://user-images.githubusercontent.com/54177908/172606854-c245faa2-4672-4e82-8b5a-94c916e7cfff.png">


<img width="721" alt="Screenshot 2022-06-08 at 13 31 19" src="https://user-images.githubusercontent.com/54177908/172606379-1570482b-1815-4922-9241-b6d141d4a9ff.png">



1 - GEOMETRY DEFINITION
------------------------
 
The POWTEX detector is built out of a number of large wire counters with trapezoidal shapes called segments. Each segment consists of two identical Multi Wire Proportional Counters sharing a segmented cathode. The Boron layer (neutron converter made of enriched 10B4C) is applied on the inner sides of the segment walls and on each side of the segmented cathode. The transportation and mounting of the segments in the detector frame is greatly facilitated by assemblying 8 segments in a common structure called module. In order to increase the detection efficiency, the individual modules are mounted in the detector frame such that the angle between the incoming neutron and the Boron layer is 10 deg.  

The cathode is segmented into 192 strips that have a constant angular resolution of 0.468 
deg for a distance sample2detector of 800 cm. Each MWPC is read-out by 16 wires. In the real detector the wires and strips are independently read-out and the position of the detected neutron is determined by taking the coincidence signal between the wires and strips. This leads to a sensitive detector volume consisting of 192 (strips) x 16 (wires) x 2 (counters) = 6144 voxels per counter.  

In the GEANT4 codes the detector voxels are implemented as small volumes made of the detector counting gas. These gas voxels are arranged side by side to fill out the space between the central cathode and detector housing and in close contact with a thin layer of the neutron converter (10B4C) (in the real detector the neutron converter is deposited on the housing and cathode Aluminum-alloy with the help of a high-temperature evaporation process called PVD). The primary particles (neutrons) hit the neutron converter and get absorbed in the neutron converter with some probability. One of the reaction products (either the 7Li-ion or the alpha-particle) will escape the Boron converter (with some probability), and travel through the gas voxel closest to the escape position until it loses its entire kinetic energy through collisions with the gas material (Ar-CO2 mixture) and comes to rest. The gas voxel that records the largest part of the kinetic energy of the reaction product is chosen as the detection voxel.   

Please note that there is no electric field included in the model. GEANT4 cannot deal with the transportation and physics of the low-energy electrons generated by the ionization of the gas. An interface with GARFIELD is possible to implement, but with substantial computing effort. Such approach was (and still is) outside the scope of the current work.   

The detector setup is defined/constructed in the *DetectorMantelConstruct.icc* inline definition file by using the technical specifications from CDT. The detector segment is built by using the AssemblyVolume() class comprising the 5 independent daughter volumes: detector housing, the Boron converter on the detector housing, the Boron layer on one side of the cathode, the Boron layer on the other side of the cathode and the cathode substrate. Furthermore, the detector module is an 'assembled volume' made of 8 identical copies of the segment. The module is then copied as many times as wished by the user and positioned at 800 cm around the sample position in cylindrical geometry. 

The *PowtexConstruction.cc* file contains the PowtexConstruction() class which is responsible for the construction and placement of the  detector in the world volume. The code structure consisting of <icc> files for the definition of the detector is inherited from the simulation code developed for the DREAM detector system, which consists of several independent detector types. The PowtexConstruction.cc file also contains the declarations of some of the main variables used to build the Mantle segment. 

In the real detector the common cathode separating the two wire counters in the detector  segment is segmented into strips that cover the same solid angle with respect to the sample position. The gas voxels are being built taking into account the geometry of the real detector voxels (determined by the wire pitch and strip size) and the symmetry of the system. The strips have trapezoidal shapes and their size increases from the center of the segment to the edges (as their location from the sample increases). Also, the strips belonging to the same wire counter (same side of the cathode) are  mirrored with respect to the line that defines the cathode center. The voxels in the second counter (the other side of the cathode) are also a mirrored copy with respect to the cathode plane of the voxels in the first counter. Therefore, out of the 6144 voxels of a detector segment one has to create only 192/2 x 16 = 1536 individual voxels volumes. The sensitive area of the segment will be filled with gas voxels belonging to 4 groups. Thus, in the geometry construction class  the naming 'top left' and  'top right' refer to the voxels filling the upper counter, left and right of the cathode center, respectively, (just remember that in the POWTEX Mantle detector the segments are mounted in horizontal position). The naming  'bottom left' and 'bottom right' refers to the bottom counter (located below the cathode), left and right of the cathode center, respectively.     
	
**Generation of the voxel lookup table**

In order to convert the data from the TOF space to the wavelength space one needs to know the precise location in the coordinate space of the voxel in which the neutron was detected. The PowtexMantleConstruction.icc file also generates the table with the x-, y- and z- coordinates of the centers of the gas voxels with respect to the origin of the reference system (i.e., the sample position). The table is created and saved in the *LookupTablePowtex.txt* ascii file each time the code is executed. The format of the table is as following:  
	
 |  det_ID	 | sector# |  100*module#+segment# | wire#  | strip# |   counter#  |	x-voxel  |   y-voxel  |   z-voxel  |
 |---------------|---------|-----------------------|--------|--------|-------------|-------------|------------|------------|
 |      7     	 |    1	   |	   101     	   |   1    |    1   |      1      |  278.436    |   759.200  |    3.339   |
 |  	7        |    1    |   	   101     	   |   1    |    2   |      1      |  278.436    |   759.200  |   10.017   |
 |  	7        |    1    |	   101             |   1    |    3   |      1      |  278.436    |   759.200  |   16.697   |
 |  	7        |    1    |       101     	   |   1    |    4   |      1      |  278.436    |   759.200  |   23.380   |
 |  	7        |    1    |       101             |   1    |    5   |      1      |  278.436    |   759.200  |   30.066   |	
 |  	7        |    1    |       101             |   1    |    6   |      1      |  278.436    |   759.200  |   36.757  |
	
 and so on .......	

where:  
	
	det_ID = detector system ID, = 7 for Mantle.
	
	sector# = sector number, only relevant for the DREAM High-Resolution and SANS detectors, always 1 otherwise.
	
	100*module# + segment# = linear combination of the module and segment number. segment# takes values from 1 to 8. 
	
	wire# = wire number. It takes values from 1 to 16.
	
	strip# = strip number. It takes values from 1 to 96 (1/2 of the total number of strips).
	
	counter# = counter number. It takes values from 1 to 4 corresponding to the 4 groups of voxels as describe above.
	
	x-voxel, y-voxel, z-voxel = coordinates of the voxel centers with respect to the sample position, which is located at (0,0,0).
	
  
2 - PHYSICS LIST
------------------
 
 The physics processes are defined in the built-in library **QGSP_BERT_HP**. 
 This library contains the standard electromagnetic processes and the high 
 precision neutron model valid for neutrons with energies below 20 MeV. 
 It is one of the standard libraries recommended for use in thermal neutron scattering 
 applications, radiation protection and shielding applications. 
 
3 - PRIMARY GENERATOR
--------------------------

The primary generator is defined in the *PrimaryGenerator.cc* file. 
The PrimaryGenerator() class reads the primary information from the Vitess ascii file and generates a 	 
primary vertex for each neutron trajectory in the file. The vertex position is at the sample position, which corresponds to the origin of the center of reference in the GEANT4 simulation. The neutron travels in the direction specified by the SetMomentum() method. 
The starting time of the neutrons from the sample position corresponds to the 
time-of-flight value read from the ascii file, which is the flight-time through the 
VITESS instrument. The final time-of-flight equals the VITESS time-of-flight plus the 
travel time of the neutron from the sample until it disappears by absorption 
(detection) in the detector volume or scatters off some material present in the setup and leaves the World volume (the mother volume). 

Two sample files are provided as an example with the code. These files ar located in 
the directory called *VitessDataPOWTEX*. 

The *noutascii_reduced.dat* file contains the first 100 neutrons events from the large file 
*noutascii_biosample_r.dat*, which can be found at the following link:
	
https://project.esss.dk/owncloud/index.php/s/pRxCSh8aGA2sHEz	
	
The small file should be used whenever visualization is wanted and for 	
debugging purposes, see point 6 below. The large sample file contains ~1.1*10^7 neutron trajectories generated by Vitess for a biosample. A calculation of the detector response consisting of 30 Mantle modules and a sample file of this size takes around ~8 h on my MacBookPro 8-Core Intel Core i9, 2.4 GHz.   
Before compiling the application please edit the *PrimaryGenerator.cc* file and replace the path to the sample file with your own path (lines 49-50). 

The file *PrimaryGeneratorAction.cc.GPS* is also provided for the case the user wishes to 
generate monochromatic neutrons from a source located at the sample position in order to test the detector setup. Replace the *PrimaryGeneratorAction.cc* file with this one, compile and run as usual (see pct. 5):  

% cp PrimaryGeneratorAction.cc PrimaryGeneratorAction.cc.file 

% cp PrimaryGeneratorAction.cc.GPS PrimaryGeneratorAction.cc 

% ./compile_Powtex_cmake.sh 

% ./Powtex  run.mac 

   
4 - TRACKING
------------------

Tracking is performed by the SteppingAction() class. The only tracks interesting for this application are those from the charged particles (7Li and alpha) that reach the gas phase (gas voxel) inside the detector. 

The MakeImprint() method of the AssemblyVolume() class used to place the various volumes in the world volume generates automatically the physical volume name. For the gas voxels, the name looks like this: 

		av_XX_impr_YY_GasLayer_BR_M_pv_ZZ

where: 

	XX - assembly volume instance number
	YY - assembly volume imprint number 
	ZZ - index of the associated logical volume
	
	GasLayer_BR_M__   -  name of the placed logical volume in the assembly 
			
			where: 
					GasLayer - name of the gas voxel logical volume 
					BR - voxel belongs to the 'bottom right' group (see the Geometry 		
						   description) 
					M - volume that belongs to the Mantle detector system 

The imprint number is used by the code to extract the module and segment number of the gas voxel that is entered by the charged particle escaping the Boron layer. The module and  segment number along with the index of the associated logical volume (the 'vox_no' variable) are part of the information on the detected neutron that is saved in the output ascii file *(myfile_info.txt)* and it will be further processed offline with the help of ROOT scripts.    
  
5 - USAGE
--------------
 
  - compile the code with the help of the script provided after setting the correct path for 		
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
    
7 - DATA ANALYSIS WITH ROOT
---------------------------
	
	https://root.cern/

The results of the tracking performed by the SteppingAction() class are saved in ascii format in the *myfile_info.txt* output file. 
Each line of the file corresponds to a detected neutron in the detector and consists of the information that allows for the hardware (module, segment, wire and strip numbers) and spatial identification of the position of the neutron detected in the detector volume (x, y, z coordinates with respect to the sample position):
	
 | tof [ns]  | StepNo | dd | detID | sector# | module# | segment# | voxel# | counter# |  weight | Edep[MeV]|  x-pos  |  y-pos   |   z-pos  |
 |-----------|--------|----|-------|---------|---------|----------|--------|----------|---------|----------|---------|----------|----------|
 | 46952554  |   2    |	10 |	7  |	1    |	 25    |    2     |  2380  |	2     |2.207e-07| 1.37726  | -828.739|	-3.56731| -263.792 |
 | 46953099  |   2    |	10 |	7  |	1    |	 25    |    2     |  2381  |	4     |2.222e-07| 1.20842  | -831.393|	-1.94727| -264.648 |
 | 46953100  |   3    |	10 |	7  |	1    |	 25    |    2     |  2385  |	4     |2.222e-07| 1.06368  | -832.684|	-1.53864| -264.836 |
 | 47054207  |   2    |	10 |	7  |	1    |	 18    |    3     |  3985  |	4     |3.704e-07| 1.36819  | -219.064|	-808.841| -471.545 |
 | 47054208  |   4    |	10 | 	7  |	1    |	 18    |    3     |  3921  | 	4     |3.704e-07| 1.10039  | -220.057|	-809.705| -470.734 |
 | 47282399  |   2    |	10 | 	7  |	1    |	 16    |    7     |  5845  | 	4     |9.481e-07| 1.11933  | 165.571 |	-834.323| -801.981 |
 | 47212947  |   3    |	20 |	7  |	1    |	 16    |    7     |  5832  |	2     |9.471e-07| 0.487803 | 156.567 |	-789.423| -756.576 |
	
 and so on .......	

where:  
	
	tof = total time-of-flight in [ns] from the inception of the neutron in the Vitess moderator until detection in detector (=t<sub>instr</sub> + t<sub>sample-detector</sub>)
	
	StepNo = number of interactions of the incident neutron with the detector materials before absorption in the Boron converter. 
	
	dd = identifier for the Boron layer in which the neutron was absorbed. dd=10 if the absorption took place in the converter applied on the cathode and dd=20 if the conversion took place in the Boron layer applied on the segment housing.  
	
	det_ID = detector system ID. det_ID = 7 for Mantle.
	
	sector# = sector number, only relevant for the DREAM High-Resolution and SANS detectors, always 1 otherwise.
	
	module# = detector module number.  
	
	segment# = segment number. It takes values from 1 to 8. 
	
	voxel# = gas voxel number. 
	
	counter# = counter number. It takes values from 1 to 4 corresponding to the 4 groups of voxels as describe above.
	
	weight = neutron weight. Value inherited from VITESS. 
	
	Edep = energy deposited by the reaction product in the Ar-CO2 material of the gas voxel. In MeV units. 
	
	x-voxel, y-voxel, z-voxel = spatial coordinates of location in which the charged particle lost entirely its kinetic energy in the gas voxels.  Coordinates measured with respect to the sample position, which is located at (0,0,0).
	
The data included in the ascii file is first sorted into the root file *powtex.root* with the help of the script *CreatePowtexTree.C*. With the help of this script each line in the file (i.e., each neutron event) is converted into an entry of the 'powtex_ev' root tree. 
The script contains the function CreatePowtexTree() that is loaded into ROOT and executed: 
	
	% root -l 
	root[0] .L CreatePowtexTree.C            (load script)
	root[1] CreatePowtexTree()               (execute function)
	
	       *...........it creates the root tree........*
	
	root[..] .q                              (type .q to exit ROOT)


	
If you want to check the content of the *powtex.root* file: 
	
	% root -l powtex.root
	root[0] .ls                        (to list the content of the rootfile)
	root[1] powtex_ev->Show(10)        (show content of entry #10 of the root tree powtex_ev)
	
	       ======> EVENT:10
 		tof             = 47027165
		step_no         = 8
 		coat            = 10
 		ss_no           = 7
 		sec_no          = 1
 		module          = 30
 		seg             = 2
 		voxel           = 3728
 		scopy           = 2
 		energy          = 0.184066
 		weight          = 4.576e-07
 		posx            = -512.962
 		posy            = 653.351
 		posz            = -441.021

	
	root[..] .q           (quit ROOT)
	
The script *ReadLookupTable.C* reads in the voxel lookup table generated during the execution of the code and creates the rootfile powtex_lookup that contains a root tree in which the entries are the individual detector voxels. This script is executed in the same way as the previous one. 
	
The script *analysis_powtex.C* reads in the powtex.root and powtex_lookup.root files. This script does several things:
	- it uses the ID of the detection voxel (information stored in the 'voxel'-branch of the *powtex_ev* tree) and generates the wire and the strip numbers corresponding to that specific voxel. 
	- it uses the position and TOF information for each detected neutron to calculate the distance from the sample (variable *rad*), the 2theta and phi angles, the wavelength lambda and the d_spacing. 
	- it creates a new root tree *powtex_new* which contains the same branches as the old data tree *powtex_ev* plus a few new branches storing the information on the wire and strip numbers, wavelength, 2theta, phi, d_spacing, etc. 
	- it loops over all neutron events stored in the *powtex_new* tree and for each neutron event it finds the center of the voxel, information which is stored in the powtex_lookup.root file. The voxel centers are saved in the powtex_new_cal.root file as the branches *nvoxel_x, nvoxel_y* and  *nvoxel_z* of the powtex_new_cal *tree friend* (read here the concept of *tree friend*: https://root.cern/root/htmldoc/guides/users-guide/Trees.html#example-3-adding-friends-to-trees). The tree friend *powtex_new_cal* will have the same number of entries as the event data tree *powtex_new*. The branches of the tree friend can be easily accessed like any other branch of the *powtex_new* event tree after being opened with the help of the 'AddFriend' method:
	
	% root -l powtex.root
	root[0] powtex_new->AddFriend("powtex_new_cal","powtex_new_cal.root")
	root[1] powtex_new->Draw("nposx")                  (it draws the histogram containing the x-pos data of all detected neutrons)
	root[2] powtex_new->Draw("n_voxel")                  (it draws the histogram containing the x-coordinates of the voxel centers for all detected neutrons)
	
A list of ROOT commands that are often used to plot and analyze the ROOT data files generated by the code is included at the end of the analysis_powtex.C script (the commands are commented out, just copy-paste a specific command in the ROOT window).
	
	

	
	

	
