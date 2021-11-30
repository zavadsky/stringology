int Bricks25_DNA(unsigned char *P, int m, unsigned char *T, int n,int k=14) {
    int mp1=m+1, mm1=m-1, mm2=m-2, mm3=m-3, mm4=m-4, mm5=m-5, mm6=m-6, m2=m*2, r, count = 0;
    const unsigned int _2_power_k=(1<<k),mask=_2_power_k-1;
    unsigned char *Z=(unsigned char *)calloc(_2_power_k,sizeof(unsigned char));
    unsigned char *s, *stop;
    int QS[256];
    if(m<=5)
        return -1;

    //Preprocessing
    for(int i=0;i<256;i++)				        // QS shift table
        QS[i]=mp1;
    for(int i=0;i<m;i++)
        QS[P[i]]=m-i;
    uint32_t p=*(uint32_t*)P;
    for (int i = 0; i < _2_power_k; i++)
		Z[i] = 1;
    for(int i=0;i<mm4;i++)
        Z[((P[i]>>1)+(P[i+1]<<1)+(P[i+2]<<3)+(P[i+3]<<5)+(P[i+4]<<7))&mask]=0;

   // Search
   s = T+m-5;       //current position
   stop = T+n-4;
   count = 0;
   memcpy(T+n,P,m);
   while(s<stop) {
        while(Z[((*s>>1)+(*(s+1)<<1)+(*(s+2)<<3)+(*(s+3)<<5)+(*(s+4)<<7))&mask])
            s+=mm4;
        if(*(uint32_t*)(s-mm5)==p) {   // occurrence check;
            for (r = 4; r < m && s[r-mm5] == P[r]; r++);
            if (r == m && s<stop)
                count++;
        }
        s+=QS[*(s+5)];
    }
	return count;
}
