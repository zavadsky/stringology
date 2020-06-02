// Zk search algorithm with 3 sliding windows Zk-Byte-w3
// This is the hybrid of Alg. 3 (left-to-right search) and Alg. 5 (sliding windows)
// x - pattern, m - pattern length, y - text, n>=4m - text length,  k - number of significant bits in 2-byte read
int Zk_byte_w3(unsigned char *x, const int m, unsigned char *y, int n,int k=13) {
	int mp1=m+1, mm1=m-1, mm2=m-2, mm3=m-3, m2=m*2, r, count = 0,outs=0;
	unsigned char *ptr1, *ptr2, *third = y+n/3, *twothird = y+2*n/3, *fin=y+n,*pos1, *pos2, *pos3,*stop;
	const unsigned int b=8; // b bits in byte
	const unsigned int sigma=1<<b, _2_power_k=(1<<k);
	const unsigned int mask=_2_power_k-1, _2_power_k_2=_2_power_k*2;
unsigned char *Z=(unsigned char *)calloc(_2_power_k,sizeof(unsigned char));
	int QS[sigma];

	// QueryPerformanceCounter(&start); // time start point, global LARGE_INTEGER start
// Preprocessing
        for (int i = 0; i < _2_power_k; i++)			// Alg. 2, line 2
            Z[i] = 1;
        for (int i = 0; i < mm1; i++)					// Alg. 2, lines 3-4
            Z[(*(ushort*)(x+i))&mask] = 0;

        for(int i=0;i<sigma;i++)				        // QS shift table
            QS[i]=mp1;
        for(int i=0;i<m;i++)
            QS[x[i]]=m-i;

    //Search
        memcpy(y+n,x,m);         // Appending the text with a stop pattern
        pos1=y+mm2;
        pos2=third;              // pos2 addresses 1/3 of a text
        pos3=twothird;           // pos3 addresses 2/3 of a text
    if(m>2) {
        while (pos1<third) {
            // Fast loop, Alg. 3 - line 5
            while(Z[((*(unsigned short*)(pos1)))&mask] & Z[((*(unsigned short*)(pos2)))&mask] & Z[((*(unsigned short*)(pos3)))&mask]){
                pos1+=mm1;  // Alg. 3, line 6
                pos2+=mm1;
                pos3+=mm1;
            }
            if(!Z[(*(unsigned short*)(--pos1))&mask]) {         // Alg. 3, line 7 combined with Alg. 5, line 7
                for (r = 0; r < m && pos1[r-mm3] == x[r]; r++);	// occurrence check, Alg. 3, line 9
                if (r == m)
                    if(pos1<third)
                        count++;
                pos1+=QS[*(pos1+3)]+1;  // Alg. 3, line 10 + Alg. 3, line 4
            } else
                pos1+=mm1;              // Alg. 3, line 4
            if(!Z[(*(unsigned short*)(--pos2))&mask]) {         // Alg. 3, line 7 combined with Alg. 5, line 12
                for (r = 0; r < m && pos2[r-mm3] == x[r]; r++);	// occurrence check, Alg. 3, line 9
                if (r == m)
                    if(pos2<twothird)
                        count++;
                pos2+=QS[*(pos2+3)]+1;  // Alg. 3, line 10 + Alg. 3, line 4
            } else
                pos2+=mm1;              // Alg. 3, line 4
            if(!Z[(*(unsigned short*)(--pos3))&mask]) {
                for (r = 0; r < m && pos3[r-mm3] == x[r]; r++);	// occurrence check, Alg. 3, line 9
                if (r == m)
                    if(pos3<fin)
                        count++;
                pos3+=QS[*(pos3+3)]+1;  // Alg. 3, line 10 + Alg. 3, line 4
            } else
                pos3+=mm1;              // Alg. 3, line 4
        }
        while(pos2<twothird) {          // If pos1 has reached 1/3 of text, but pos2 has not reached 2/3 of text
            while(Z[((*(unsigned short*)(pos2)))&mask])
                pos2+=mm1;
            for (r = 0; r < m && pos2[r-mm2] == x[r]; r++);
            if (r == m)
                if(pos2<twothird)
                    count++;
            pos2+=QS[*(pos2+2)];
        }
        while(pos3<fin) {               // If pos1 has reached 1/3 of text, but pos3 has not reached end of text
            while(Z[((*(unsigned short*)(pos3)))&mask])
                pos3+=mm1;
            for (r = 0; r < m && pos3[r-mm2] == x[r]; r++);
            if (r == m)
                if(pos3<fin)
                    count++;
            pos3+=QS[*(pos3+2)];
         }
    } else {        // special case m==2
            while (pos1<third) {
                while(Z[((*(unsigned short*)(pos1)))&mask] & Z[((*(unsigned short*)(pos2)))&mask] & Z[((*(unsigned short*)(pos3)))&mask]) {
                    pos1++; pos2++; pos3++;
                }
                if(!Z[(*(unsigned short*)(pos1))&mask]) // occurrence check
                    if(pos1<third)
                        count++;
                pos1+=QS[*(pos1+2)];
                if(!Z[(*(unsigned short*)(pos2))&mask]) // occurrence check
                    if(pos2<twothird)
                        count++;
                pos2+=QS[*(pos2+2)];
                if(!Z[(*(unsigned short*)(pos3))&mask]) // occurrence check
                    if(pos3<fin)
                        count++;
                pos3+=QS[*(pos3+2)];
            }
            while(pos2<twothird) {
                while(Z[((*(unsigned short*)(pos2)))&mask])
                    pos2++;
                if(pos2<twothird)
                    count++;
                pos2++;
            }
            while(pos3<fin) {
                while(Z[((*(unsigned short*)(pos3)))&mask])
                    pos3++;
                if(pos3<fin)
                    count++;
                pos3++;
            }
    }
    // QueryPerformanceCounter(&_end);  // time end point, global LARGE_INTEGER _end
	// u = (_end.QuadPart - start.QuadPart) * 10000 / freq.QuadPart; //algorithms' timing in milliseconds, double u
	return count;
}
