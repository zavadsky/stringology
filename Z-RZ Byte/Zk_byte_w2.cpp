int Z_w2(unsigned char *x, const int m, unsigned char *y, int n,int k=13) {
	int mp1=m+1, mm1=m-1, mm2=m-2, mm3=m-3, m2=m*2, r, count = 0,outs=0;
	unsigned char *ptr1, *ptr2, *half = y+n/2, *fin=y+n,*pos1, *pos2;
	const unsigned int b=8; // b bits in byte
	const unsigned int sigma=1<<b, _2_power_k=(1<<k);
	const unsigned int mask=_2_power_k-1, _2_power_k_2=_2_power_k*2;
unsigned char *Z=(unsigned char *)calloc(_2_power_k,sizeof(unsigned char));
	int QS[sigma];

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
        pos2=half;              // pos2 addresses 1/3 of a text
    if(m>2) {
        while (pos1<half) {
            // Fast loop, Alg. 3 - line 5
            while(Z[((*(unsigned short*)(pos1)))&mask] & Z[((*(unsigned short*)(pos2)))&mask]){
                pos1+=mm1;  // Alg. 3, line 6
                pos2+=mm1;
            }
            if(!Z[(*(unsigned short*)(--pos1))&mask]) {         // Alg. 3, line 6 combined with Alg. 5, line 7
                for (r = 0; r < m && pos1[r-mm3] == x[r]; r++);	// occurrence check, Alg. 3, line 7
                if (r == m)
                    if(pos1<half)
                        count++;
                pos1+=QS[*(pos1+3)]+1;  // Alg. 3, line 8
            } else
                pos1+=mm1;              // Alg. 3, line 10
            if(!Z[(*(unsigned short*)(--pos2))&mask]) {         // Alg. 3, line 6 combined with Alg. 5, line 7
                for (r = 0; r < m && pos2[r-mm3] == x[r]; r++);	// occurrence check, Alg. 3, line 7
                if (r == m)
                    if(pos2<fin)
                        count++;
                pos2+=QS[*(pos2+3)]+1;  // Alg. 3, line 8
            } else
                pos2+=mm1;              // Alg. 3, line 10
        }
        while(pos2<fin) {          // If pos1 has reached 1/2 of the text, but pos2 has not reached end of the text
            while(Z[((*(unsigned short*)(pos2)))&mask])
                pos2+=mm1;
            for (r = 0; r < m && pos2[r-mm2] == x[r]; r++);
            if (r == m)
                if(pos2<fin)
                    count++;
            pos2+=QS[*(pos2+2)];
        }
    } else {        // special case m==2
            while (pos1<half) {
                while(Z[((*(unsigned short*)(pos1)))&mask] & Z[((*(unsigned short*)(pos2)))&mask]) {
                    pos1++;
                    pos2++;
                }
                if(!Z[(*(unsigned short*)(pos1))&mask]) // occurrence check
                    if(pos1<half)
                        count++;
                pos1+=QS[*(pos1+2)];
                if(!Z[(*(unsigned short*)(pos2))&mask]) // occurrence check
                    if(pos2<fin)
                        count++;
                pos2+=QS[*(pos2+2)];
            }
            while(pos2<fin) {
                while(Z[((*(unsigned short*)(pos2)))&mask])
                    pos2++;
                if(pos2<fin)
                    count++;
                pos2++;
            }
    }
	return count;
}
