int Bricks34(unsigned char *P, int m, unsigned char *T, int n,int k=14) {
    int mp1=m+1, mm1=m-1, mm2=m-2, mm3=m-3, mm4=m-4, r, count = 0;
    const unsigned int _2_power_k=(1<<k),mask=_2_power_k-1,_2_power_k_2=_2_power_k*2;
    unsigned char *Z=(unsigned char *)calloc(_2_power_k_2,sizeof(unsigned char));
    unsigned char *s, *stop;
    int QS[256];
    if(m<=2)
        return -1;

    //Preprocessing
    for(int i=0;i<256;i++)				        // QS shift table
        QS[i]=mp1;
    for(int i=0;i<m;i++)
        QS[P[i]]=m-i;
    uint32_t p=*(uint32_t*)P;
    for (int i = 0; i < _2_power_k; i++)
		    Z[i] = 1;
    for(int i=0;i<mm3;i++)
        Z[(P[i]+(P[i+1]<<3)+(P[i+2]<<6)+(P[i+3]<<9))&mask]=0;

   // Search
   s = T+m-4;       //current position
   stop = T+n-3;
   count = 0;
   memcpy(T+n,P,m);
   while(s<stop) {
        while(Z[(*s+(*(s+1)<<3)+(*(s+2)<<6)+(*(s+3)<<9))&mask])
            s+=mm3;
        if(*(uint32_t*)(s-mm4)==p) {   // occurrence check;
            for (r = 4; r < m && s[r-mm4] == P[r]; r++);
            if (r == m && s<stop)
                count++;
        }
        s+=QS[*(s+4)];
    }
	return count;
}
