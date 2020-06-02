// the reverse bitstream search algorithm with 2 sliding windows RZk-bit-w2
//x - pattern, l - pattern bit length, y - text, n - text byte length,  k - number of significant bits in 2-byte read
int RZk_bit_w2(unsigned char *x, int l, unsigned char *y, int n,int k=15) {
const unsigned int b=8; // b bits in byte
const unsigned int b2=b*2,sigma=1<<b,_2_power_k=(1<<k);
const unsigned int _2_power_k_2=_2_power_k*2;
int m=(int)(l/b),mb=l%b,q,pos,mp1=m+1,mm1=m-1,mm2=m-2,count=0,m2=2*m,m2m2=m2-2,m2m1=m2-1,r,s,jj,st;
unsigned char* ptr,*prev;
unsigned char lastMask=(sigma-1)<<(b-mb);
unsigned int t,mask=_2_power_k-1,mask1=(sigma-1)<<b2,mask2=mask&((sigma-1)<<b);
int lambda[sigma][b];
unsigned char Backup[P_MAX];
unsigned char *Z=(unsigned char *)calloc(_2_power_k,sizeof(unsigned char)),*pos1,*pos2,*stop,*start,*half=y+n/2,*j;
ushort tt;

	//QueryPerformanceCounter(&start_time); // time start point, global LARGE_INTEGER start_time
// Preprocessing
	for(jj=0;jj<b;jj++)
		for(int i=0;i<sigma;i++)
			lambda[i][jj]=-1;	// lambda[c][j] is the j-th element of the set lambda[c]
	for(int i=0;i<b;i++) {
		unsigned char c=(x[0]<<i)|(x[1]>>(b-i));
		for(jj=0;lambda[c][jj]>=0;jj++);
		lambda[c][jj]=i;
	}
	for(int i=0;i<_2_power_k;i++)
		Z[i]=1;

	for(int i=0;i<l-b;i++) {
		/*********** Bitword(i) **************/
		r=i/b;
		s=(((unsigned int)x[r])<<b2)|(((unsigned int)x[r+1])<<b)|x[r+2];
		s<<=(i%b);
		t=((s&mask1)>>b2)|(s&mask2);
		/*********** end Bitword **************/
		Z[t]=0;
		if(l-i<b2){
			int stop=1<<(l-i>3*b-k?b2-l+i:k-b);			// stop = 2^s (lines 12-14 of Alg. 9)
			for(jj=0;jj<stop;jj++)
				Z[t|(jj<<b)]=0;
		}
	}

// Search
	pos1=half;
	pos2=y+n-m;
	stop=y+m+1;
	memcpy(Backup,y,m+1);
	memcpy(y,x,m+1);
	if(l>23)    // patterns longer than 23 bits
        while (pos1>stop) {
            while(Z[((*(unsigned short*)(pos1)))&mask] & Z[((*(unsigned short*)(pos2)))&mask]) {
                pos1-=mm1;
                pos2-=mm1;
            }
            if(!Z[(*(unsigned short*)(++pos1))&mask]) {
                pos1--;
                for(int i=0;(q=lambda[*pos1][i])>=0;i++) {
                    //********** CheckMatch(q,pos) ************
                    for(j=start=pos1-1;j-start<m && (unsigned char)(((*j)<<(b-q))|(j[1]>>q))==x[(int)(j-start)];j++);
                    if(j-start==m)
                        if(((((*j)<<(b-q))|(*(j+1)>>q))&lastMask)==x[m])
                            if(pos1>y+m+1)
                                count++;
                    //********** end CheckMatch ************
                }
                pos1--;
            } else
                pos1-=mm1;
            if(!Z[(*(unsigned short*)(++pos2))&mask]) {
                pos2--;
                for(int i=0;(q=lambda[*pos2][i])>=0;i++) {
                    //********** CheckMatch(q,pos) ************
                    for(j=start=pos2-1;j-start<m && (unsigned char)(((*j)<<(b-q))|(j[1]>>q))==x[(int)(j-start)];j++);
                    if(j-start==m)
                        if(((((*j)<<(b-q))|(*(j+1)>>q))&lastMask)==x[m])
                            if(pos2>half)
                                count++;
                    //********** end CheckMatch ************
                }
                pos2--;
            } else
                pos2-=mm1;

        }
    else    // patterns shorter than 23 bits
        while (pos1>stop) {
            while(Z[((*(unsigned short*)(pos1)))&mask] & Z[((*(unsigned short*)(pos2)))&mask]) {
                pos1-=mm1;
                pos2-=mm1;
            }
            for(int i=0;(q=lambda[*pos1][i])>=0;i++) {
            //********** CheckMatch(q,pos) ************
                for(j=start=pos1-1;j-start<m && (unsigned char)(((*j)<<(b-q))|(j[1]>>q))==x[(int)(j-start)];j++);
                if(j-start==m)
                    if(((((*j)<<(b-q))|(*(j+1)>>q))&lastMask)==x[m])
                        if(pos1>y+m2+1)
                            count++;
            //********** end CheckMatch ************
            }
            pos1--;
            for(int i=0;(q=lambda[*pos2][i])>=0;i++) {
            //********** CheckMatch(q,pos) ************
                for(j=start=pos2-1;j-start<m && (unsigned char)(((*j)<<(b-q))|(j[1]>>q))==x[(int)(j-start)];j++);
                if(j-start==m)
                    if(((((*j)<<(b-q))|(*(j+1)>>q))&lastMask)==x[m])
                        if(pos2>half)
                            count++;
            //********** end CheckMatch ************
            }
            pos2--;
        }
        while(pos2>half){
            while(Z[((*(unsigned short*)(pos2)))&mask])
                pos2-=mm1;
            for(int i=0;(q=lambda[*pos2][i])>=0;i++) {
                    //********** CheckMatch(q,pos) ************
                    for(j=start=pos2-1;j-start<m && (unsigned char)(((*j)<<(b-q))|(j[1]>>q))==x[(int)(j-start)];j++);
                    if(j-start==m)
                        if(((((*j)<<(b-q))|(*(j+1)>>q))&lastMask)==x[m])
                            if(pos2>half)
                                count++;
                    //********** end CheckMatch ************
                }
            pos2--;
        }

	memcpy(y+m+1,Backup,m+1);
	//ShortSearch in T[0...2m-1]
	pos=m+1;
	do {
		for(int i=0;(q=lambda[y[pos+1]][i])>=0;i++) {
			for(jj=st=pos;jj-st<m && (unsigned char)((y[jj]<<(b-q))|(y[jj+1]>>q))==x[jj-st];jj++);
			if(jj-st==m)
				if((((y[jj]<<(b-q))|(y[jj+1]>>q))&lastMask)==x[m])
					count++;
		}
		pos--;
	} while(pos>=0);
	//QueryPerformanceCounter(&_end);             // time end point, global LARGE_INTEGER _end
	// u = (_end.QuadPart - start.QuadPart) * 10000 / freq.QuadPart; //algorithms' timing in milliseconds, double u
	return count;
}
