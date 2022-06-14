// this function calculates the wire number for the EndCap modules

int calculateW_for_Mantel(int voxel, int scopy)

{
	int nw = 16;
	int ns = 96;  // 1/2 of the number of strips
	
	int factor,wire;
	
	switch (scopy) {
	
		case 1:    // TL before rotation, TR after rotation
		
        		factor = (voxel - 2)/4;  // TL before rotation starts at 9, next is 13
			wire = factor % nw;  // the wire number = remainder 
			if (wire == 0) {wire = nw;}
			break;
			
		case 2:      //TR before rotation, BR after rotation
		
        		factor = (voxel - 4)/4;  // TR before rotation starts 8, next is 12
			wire = factor % nw;
			if (wire == 0) {wire = nw;}
			break;
			
		case 3:      //BL before rotation, TL after rotation
		
        		factor = (voxel - 3)/4;  // BL before rotation starts at 7, next is 11
			wire = factor % nw;
			if (wire == 0) {wire = nw;}
			break;
			
		case 4:      // BR before rotation, BL after rotation 
		
        		factor = (voxel - 5)/4;  // BR before rotation starts at 6, next is 10
			wire = factor % nw;
			if (wire == 0) {wire = nw;}
			break;
			
			}

		
	return wire;
	
}	
