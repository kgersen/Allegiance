//Imago #192 8/10

typedef struct {
 char name[12];
 float mu;
 float sigma;
 int rank;
} Msr;

typedef struct {
 Msr * msrs;
 int cNum;
} MsrArray;
