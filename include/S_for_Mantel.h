// this function calculates the wire number for the EndCap modules

int calculateS_for_Mantel(int voxel,int scopy)

{
	int nw = 16;
	int ns = 96;  // 1/2 of the number of strips
	
	int factor,strip;
	
	switch (scopy) {
	
		case 1:    // TL before rotation
		
        		factor = (voxel - 2)/4;  // TL before rotation starts at 6, next is 10
        		if ((factor % nw) == 0) strip = ns + TMath::FloorNint(factor/nw); // strip 256
        			else strip = ns + TMath::FloorNint(factor/nw) + 1;
			
			break;
			
		case 2:      //TR before rotation
		
        		factor = (voxel - 4)/4;  // TR before rotation starts 8, next is 12
        		if ((factor % nw) == 0) strip = ns - TMath::FloorNint(factor/nw) + 1;  // strip 128
        			else strip = ns - TMath::FloorNint(factor/nw);
			break;
			
		case 3:      //BL before rotation, TL after rotation
		
        		factor = (voxel - 3)/4;  // BL before rotation starts at 7, next is 11
        		if ((factor % nw) == 0) strip = ns  + TMath::FloorNint(factor/nw);   // strip 256
        			else strip = ns + TMath::FloorNint(factor/nw) + 1;
			break;
			
		case 4:      // BR before rotation, BL after rotation 
		
        		factor = (voxel - 5)/4;  // BR before rotation starts at 6, next is 10
        		if ((factor % nw) == 0) strip = ns  - TMath::FloorNint(factor/nw) + 1;  // strip 128
        			else strip = ns - TMath::FloorNint(factor/nw);

			break;
		}
		
	return strip;
	
}	
