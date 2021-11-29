int Bricks26(unsigned char *P, int m, unsigned char *T, int n,int k=14) {
    int mp1=m+1, mm1=m-1, mm2=m-2, mm3=m-3, mm4=m-4, mm5=m-5, mm6=m-6, r, count = 0;
	const unsigned int _2_power_k=(1<<k),mask=_2_power_k-1,_2_power_k_2=_2_power_k*2;
    unsigned char *Z=(unsigned char *)calloc(_2_power_k_2,sizeof(unsigned char));
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
    for(int i=0;i<mm5;i++)
        Z[((P[i])+(P[i+1]<<2)+(P[i+2]<<4)+(P[i+3]<<6)+(P[i+4]<<8)+(P[i+5]<<10))&mask]=0;

   // Searching
   s = T+m-6;       //current position
   stop = T+n-5;
   count = 0;
   memcpy(T+n,P,m);
   while(s<stop) {
        while(Z[((*s)+(*(s+1)<<2)+(*(s+2)<<4)+(*(s+3)<<6)+(*(s+4)<<8)+(*(s+5)<<10))&mask])
            s+=mm5;
        if(*(uint32_t*)(s-mm6)==p) {   // occurrence check;
            for (r = 4; r < m && s[r-mm6] == P[r]; r++);
            if (r == m && s<stop)
                count++;
        }
        s+=QS[*(s+6)];
    }
	return count;
}
