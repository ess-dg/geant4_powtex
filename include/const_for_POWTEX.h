// these are constants used in analysis_dream.constants

const double pi = 3.14159265; 
const double mn = 1.674927471e-27;  // in kg
const double hp = 6.62607004e-34;   // in kg * m2/s
const double m2a = 1e+10;    // unit to convert m in A

Int_t n_wires = 16;  //EndCap
Int_t n_strips = 16;    //EndCap
Int_t n_wiresM = 16;    // Mantel
Int_t n_stripsM = 96;  //  1/2 of the total no of strips for Mantel

// time shifts and delays for the WFM time corrections

double tdelay = 4.;
double tshift[6] = {8.2,10.6,12.3,14.,16.,18.4};
double tmin[6] = {14.8,21.8,31.6,40.6,49.2,57.2};
double tmax[6] = {20.1,30.5,39.4,47.7,55.6,65.5};

