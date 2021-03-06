// The reverse search algorithm with 2 sliding windows RZk-Byte-w2
// x - pattern, m - pattern length, y - text, n>=4m - text length,  k - number of significant bits in 2-byte read
int RZk_byte_w2(unsigned char *x, const int m, unsigned char *y, int n,int k=13) {
int mp1=m+1, mm1=m-1, r, pos, count = 0;
unsigned char *half = y+n/2, *pos1, *pos2;
const unsigned int b=8; // b bits in byte
const unsigned int sigma=1<<b, _2_power_k=(1<<k);
const unsigned int mask=_2_power_k-1, _2_power_k_2=_2_power_k*2;
// Z is the main search table. Backup is to save the first m bytes of a text before replacement with a stop pattern
unsigned char *Z=(unsigned char *)calloc(_2_power_k,sizeof(unsigned char)),*Backup=(unsigned char *)calloc(m,sizeof(unsigned char));
int RQS[sigma]; // The reverse QS shift table

	// QueryPerformanceCounter(&start); // time start point, global LARGE_INTEGER start
// Preprocessing
    	for (int i = 0; i < _2_power_k; i++)				// Alg. 4 - line 2
		Z[i] = 1;
	for (int i = 0; i < mm1; i++)						// Alg. 4 - lines 3-4
		Z[(*(ushort*)(x+i))&mask] = 0;  // Pair of adjacent pattern bytes coincides with the first 2 bytes of a search window
	for (ushort i = 0;i<(1<<(k-b)); i++)				// Alg. 4 - lines 5-6
		Z[(i<<b)|x[mm1]] = 0;   // Last byte of the pattern coincides with the first byte of a search window


	for(int i=0;i<sigma;i++)				// The reverse QS shift table filling
		RQS[i]=mp1;
	for(int i=m-1;i>=0;i--)
		RQS[x[i]]=i+1;

//Search
    	memcpy(Backup,y,m); // Backup the beginning of the text
	memcpy(y,x,m);      // Copy the stop pattern to the beginning of the text
	pos1=half;
	pos2=y+n-m;
	while (pos1>y) {
		while(Z[((*(unsigned short*)(pos1)))&mask] & Z[((*(unsigned short*)(pos2)))&mask]) {    // Fast loop, Alg. 5 - line 4
			pos1-=m; // Alg. 5 - line 5
			pos2-=m; // Alg. 5 - line 6
        	}
		if(!Z[(*(unsigned short*)(pos1+1))&mask]) { // Alg. 5 - line 7
		    for (r = 0; r < m && pos1[r] == x[r]; r++);	// occurrence check, Alg. 5 - line 8
		    if (r == m)
			if(pos1>=y+m) // If pattern match is after the stop pattern
			    count++;
		    pos1-=RQS[*(pos1-1)];   // Alg. 5 - line 9
		} else
		    pos1-=mm1;      // Alg. 5 - line 11
		if(!Z[(*(unsigned short*)(pos2+1))&mask]) { // Alg. 5 - line 12
		    for (r = 0; r < m && pos2[r] == x[r]; r++); // occurrence check, Alg. 5 - line 13
		    if (r == m)
			if(pos2>half) // If pattern match is in the right sliding window
			    count++;
		    pos2-=RQS[*(pos2-1)]; // Alg. 5 - line 14
		} else
		    pos2-=mm1;  // Alg. 5 - line 16
    	}
    //If pos1 has reached stop, but pos2 has not reached half
	while(pos2>half){                           // Alg. 5 - lines 17-20
		while(Z[((*(unsigned short*)(pos2)))&mask] )
			pos2-=m;
		for (r = 0; r < m && pos2[r] == x[r]; r++);
		if (r == m)
            		if(pos2>half)
                		count++;
		pos2-=RQS[*(pos2-1)];
	}
	memcpy(y,Backup,m);
//ShortSearch in T[0...m-1]
	pos=m-1;
	do {
		for (r = 0; r < m && y[pos + r] == x[r]; r++);	// occurrence check
		if (r == m)
			count++;
		pos-=RQS[y[pos-1]];
	} while(pos>=0);
	//QueryPerformanceCounter(&_end);  // time end point, global LARGE_INTEGER _end
	//u = (_end.QuadPart - start.QuadPart) * 10000 / freq.QuadPart; //algorithms' timing in milliseconds, double u
	return count;
}
