int Bricks23(unsigned char *P, int m, unsigned char *T, int n) {
int mp1=m+1, mm1=m-1, mm2=m-2, mm3=m-3, r, count = 0;
const unsigned int _2_power_k=(1<<16);
unsigned char *Z=(unsigned char *)calloc(_2_power_k,sizeof(unsigned char));
    unsigned char *s, *stop;
    int QS[256];
    if(m<3)
        return -1;

  //Preprocessing
    for(int i=0;i<256;i++)				        // QS shift table
        QS[i]=mp1;
    for(int i=0;i<m;i++)
        QS[P[i]]=m-i;
    uint16_t p=*(uint16_t*)P;
    for (int i = 0; i < _2_power_k; i++)
		Z[i] = 1;
    for(int i=0;i<mm2;i++)
        Z[(P[i])+(P[i+1]<<2)+(P[i+2]<<4)]=0;

   // Search
   s = T+m-3;       //current position
   stop = T+n-2;
   count = 0;
   memcpy(T+n,P,m);
   while(s<stop) {
        while(Z[(*s)+(*(s+1)<<2)+(*(s+2)<<4)])
            s+=mm2;
        if(*(uint16_t*)(s-mm3)==p) {   // occurrence check;
            for (r = 2; r < m && s[r-mm3] == P[r]; r++);
            if (r == m && s<stop)
                count++;
        }
        s+=QS[*(s+3)];
    }
	return count;
}
