int Bricks22(unsigned char *P, int m, unsigned char *T, int n) {
int mp1=m+1, mm1=m-1, mm2=m-2, r, count = 0;
const unsigned int _2_power_k=(1<<16);
unsigned char *Z=(unsigned char *)calloc(_2_power_k,sizeof(unsigned char));
unsigned char *s,*stop;
int QS[256];
    if(m<2)
        return -1;

    //Preprocessing
    for(int i=0;i<256;i++)				        // QS shift table
        QS[i]=mp1;
    for(int i=0;i<m;i++)
        QS[P[i]]=m-i;
    uint16_t p=*(uint16_t*)P;
    for (int i = 0; i < _2_power_k; i++)
	Z[i] = 1;
    for(int i=0;i<mm1;i++)
        Z[(P[i])+(P[i+1]<<2)]=0;

   // Search
   s = T+m-2;       //current position
   stop = T+n-1;
   count = 0;
   memcpy(T+n,P,m);
   while(s<stop) {
        while(Z[(*s)+(*(s+1)<<2)])
            s+=mm1;
        if(*(uint16_t*)(s-mm2)==p) {   // occurrence check;
            for (r = 2; r < m && s[r-mm2] == P[r]; r++);
            if (r == m && s<stop)
                count++;
        }
        s+=QS[*(s+2)];
    }
return count;
}
