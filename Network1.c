#include "MLearn.c"
//Include all Functions for Matrix operations..

typedef struct Layer_{
//Number of nodes for layer
long long int LayerSize;

//long double** W; //W from layer i to layer i+1.
Matrix* W;
//2D array, size LayerSize(i)*LayerSize(i+1)
//long double** H;
Matrix* H;
//Activation, typically size ix1, 1 column, i rows
long int LayerType;

//What Type of Activation to use
//0 NONE
//1 SIGMOID + NORMALIZED (DIVIDE)
//2 RELU + NORMALIZED (DIVIDE)
//3 SOFTMAX + NORMALIZED (DIVIDE)
//4 ... MORE SOON
int ActivationFunction;

}Layer;

typedef struct NN_{//Create Array of Layers (static) to access each layer quickly

Layer* L;

}NN;

//Constant Array of Layers...Rows, Cols
//NN Network1[50];

typedef struct RestoredNet_{

//Number of Layers for Net
long long int NetLen;

//Error, starting from Output
Matrix* ErrorR;

//Network -> Weights, Activation, Bias
NN * NetworkR;

}RestoredNet;


void clean_stdin(void);

NN* CreateNetwork(NN Network[],long long int Layers);

void RandomizeWeights(long double** W, long long int Row, long long int Col);

void PrintNetwork(NN Network[]);
//Save to a file
void SaveNetwork(NN Network[],FILE* F, Matrix* Error, long long int Layers);

RestoredNet* RestoreNetwork();

RestoredNet* RN = NULL;

void DeleteRestoredNet();

Matrix* StoreMatrix(Storage* S, long long int Row, long long int Col);

void PromptUserSave(NN Network[], Matrix* Error, long long int Layers);

int PromptGeneral(char* Ask);

void DeleteNetwork(NN Network[]);

void PrintWeights(long double**W, long long int Row, long long int Col);

void PrintActivation(long double**H, long long int Row);

char FGo(FILE* F,char c);

//FEEDING NETWORK INPUT LAYER AND RETURNING ERROR RESULT
Matrix* FeedIn(NN Network[],long long int Layers);
Matrix* FeedForward(NN Network[], Matrix* IN);
Matrix* BackProp(NN Network[],Matrix* Error,long long int Layer, bool Normalized);
///////////////////////////////////////////////////////

//USING ERROR RESULT TO OPTIMIZE NETWORK WEIGHTS
Matrix* Gradient(NN Network[], Matrix* Errors, long long int Layers);
////////////////////////////////////////////////

bool Nor = true;

long double INBPAM = 1;

NN * Network2;

int main(int argc, char**argv){

long long int L = 0;

long long int New=0;

//while(1){

//NN * Network2;

//if(RN==NULL){
//Create a NN
if(RN==NULL){

printf("\nCreate New Network <=0 or Restore Network >0");
scanf("%lld",&New);
clean_stdin();

}
else{
New=0;
}
if(New<=0){

if(RN==NULL){
printf("\n ENTER NUMBER OF LAYERS:");
scanf("%lld",&L);
clean_stdin();

if(L<2){
//In and out layer required!!
printf("\n Impossible layer #\n");
exit(-1);
}

NN Network[L];
Network2 = Network;
CreateNetwork(Network2,L);

}
else{
printf("\nUSING RESTORED NETWORK\n");
Network2 = RN->NetworkR;
L=RN->NetLen;
printf("\n SIZE:%lld",L);
//int Wait;
//scanf("%d",&Wait);
//clean_stdin();
}
//printf("\n Sigmoid 1:%LG",1/(1+expl(-1)));

Matrix* M2 = FeedIn(Network2,L);
printf("\n FIRST FEED\n");
printMatrixData(M2);

FeedForward(Network2,M2);

DeleteMatrixes(M2);

PrintNetwork(Network2);

long long int g=0;
while((Network2[g].L)->LayerType!=2){
g++;
}
long long int Layer = g;
long long int Size = (Network2[g].L)->LayerSize;

//Layer L3[] = (Network2[g].L);

long double** E = malloc(Size*sizeof(long double*));
long long int h = 0;
while(h<Size){
E[h]=malloc(sizeof(long double));
h++;
}
long double Expected = 0;

g = 0;

bool Err = false;

while(g<Size){
printf("\n Enter expected output[%lld]:",g+1);
scanf("%LG",&Expected);
clean_stdin();
///SUBTRACT EXPECTED FROM BIAS + WEIGHT[i]
E[g][0]=Expected-(((Network2[Layer].L)->H->M)[g][0]+((Network2[Layer].L)->LayerType==2? 0.0:((Network2[Layer].L)->H->M)[Size][0]));
//EXPECTED FROM BIAS+WEIGHT[i]

printf("\n %LG -- %LG",Expected ,(((Network2[Layer].L)->H->M)[g][0]+((Network2[Layer].L)->LayerType==2? 0.0:((Network2[Layer].L)->H->M)[Size][0])));

if(Expected - (((Network2[Layer].L)->H->M)[g][0]+((Network2[Layer].L)->LayerType==2? 0.0:((Network2[Layer].L)->H->M)[Size][0]))!=0){
//Error Margin
Err = true;
}
g++;
}

if(Err){
//Error expected
Matrix* Error = malloc(sizeof(Matrix));
Error->Row = Size;
Error->Column = 1;
Error->M = E;
Error->Next=NULL;
//Begin Adjusting Weights...
Matrix* E2 = BackProp(Network2,Error,Layer,true);

Error->Next=E2;

PromptUserSave(Network2,Error,Layer);

Matrix* GradientWeights = Gradient(Network2,E2,Layer);

//DeleteMatrixes(GradientWeights);

DeleteMatrixes(Error);

}
else{

//No Error
PromptUserSave(Network2,NULL,Layer);

freeMatrix(E,Size);

DeleteNetwork(Network2);

}

PrintNetwork(Network2);

if(RN!=NULL){
DeleteRestoredNet();
}

DeleteNetwork(Network2);

}
else{
//Not New Network... Train?

printf("\n USING SAVED NETWORK\n");

RN = RestoreNetwork();

//Network2 = RN->NetworkR;
//L=RN->NetLen;

//DeleteRestoredNet();

}



//}
/*
if(PromptGeneral("Do You want to Quit?")>0){
break;
}

}
*/
//DeleteRestoredNet();

int Quit = 0;
printf("\nExit? <=0 No >0 Yes");
scanf("%d",&Quit);
clean_stdin();
if(Quit<=0){
//char** Args = {"A","B"};

//char args[1][2] = {'a','a'};

//char[1][2] args = {"ABS","DEF"};

main(argc,argv);
}

//Finished with backpropogation/learning/training!
return 0;
}

//Feed Forward Network
NN* CreateNetwork(NN Network[],long long int Layers){

//NN* Network=malloc(sizeof(Layers));

//NN Network[Layers];

long long int i=0;
//long int j=0;

int Activation;
long long int Size;

printf("\nEach layer can be any size > 0! , connectivity only increases\n");

for(i=0;i<Layers;i++){
printf("\n I: %lld",i);
if(i==0){
printf("\nEnter Input Layer Size:");
}
else if(i==Layers-1){
printf("\nEnter Output Layer Size:");
}
else{
printf("\n Enter Hidden Layer Size:");
}

scanf("%lld",&Size);
clean_stdin();

if(Size<=0){
printf("\n Impossible Layer Size:%lld",Size);
exit(-1);
}

if(i>0){
printf("\nEnter Activation Function [0=None,1=Sigmoid,2=Relu,3=SoftMax] ");
scanf("%d",&Activation);
clean_stdin();

if(Activation<0||Activation>2){
Activation=0;
}
}

(Network[i]).L = malloc(sizeof(Layer));
Layer* L2 = (Network[i]).L;
L2->LayerSize = Size;
if(i>0)
L2->ActivationFunction=Activation;

if(i>0){
//Create Weight Matrix for Layer i to Layer i+1
//Cols
//+1 FORRRRRRRRRRRRRRR BIAS
long long int S1 = 0.0;
//if(i<Layers-1){
//IF SIMPLE IN OUT NETWORK... OR MORE LAYERS
S1 = (Network[i-1].L)->LayerSize+1;
//}
//else{
//S1 = (Network[i-1].L)->LayerSize;
//}
//Rows
long long int S2 =0.0;
//if(i<Layers=1){
 //S2 = (Network[i].L)->LayerSize+1;
//}
//else{
S2 = (Network[i].L)->LayerSize;
//}
//calculate Combinations of all layer connections = S2*S1... |H|*|X|
//L2->W=malloc(S2*sizeof(long double));
L2->W=malloc(sizeof(Matrix));

//Matrix* M1 = L2->W;

L2->W->Row = S2;
L2->W->Column = S1;

L2->W->Next=NULL;

L2->W->M = malloc(S2*sizeof(long double*));

long long int k=0;
while(k<S2){
//L2->W[k]=malloc(S1*sizeof(long double));
(L2->W->M)[k]=malloc(S1*sizeof(long double));
k++;
}

RandomizeWeights(L2->W->M, S2,S1);

}

//Create H matrix (NULL for Now) UNTIL call FeedForward Func
L2->H=NULL;
//Name(Label) of Layer
if(i==0){
L2->LayerType = 0;
}
else if(i==Layers-1){
L2->LayerType = 2;
}
else{
//printf("\n\nEnter Layer Type Index [0 IN] [1 Special Hidden Type] [2 OUT]");
//scanf("%ld",&(L2->LayerType));
L2->LayerType=1;
}


}

PrintNetwork(Network);

return Network;
}

void RandomizeWeights(long double ** W, long long int Row, long long int Col){
if(W==NULL){
printf("\n Cannot Randomize Weights! NULL");
exit(-1);
}
long int i=0;
long int j=0;
for(i=0;i<Row;i++){
for(j=0;j<Col;j++){
if(j==Col-1){
W[i][j]=1.0;
}
else{
W[i][j]=(long double)rand()/(long double)RAND_MAX;
}
}
}

return;
}

void PrintNetwork(NN Network[]){
if(Network==NULL){
printf("\nNull pointer");
return;
}
long int i=0;

bool END = false;

while(!END){
if((Network[i].L)->LayerType==2){
END=true;
}
printf("\n=======================================\n");
printf("\nLayer:%ld, LayerSize:%lld, Weights:",i,(Network[i].L)->LayerSize);

//Only odd row gets weight matrix...
if(i>0){
//long int Row = (Network[i].L)->LayerSize;
//long int Column = (Network[i-1].L)->LayerSize;
//PrintWeights((Network[i].L)->W,Row,Column);
printMatrixData((Network[i].L)->W);
//printf(", Activation:%LG\n",(Network[i].L)->H==NULL? 0:(Network[i].L)->H);
//Always 1 column
printf("Activation:");
//PrintActivation((Network[i].L)->H,Row);
printMatrixData((Network[i].L)->H);
printf("^^^^ NEW BIAS ^^^^\n");
}

i++;
}

printf("=======================================\n");

return;
}

void DeleteNetwork(NN Network[]){

if(Network==NULL){
printf("\nNull pointer");
return;
}
long long int i=0;

bool END = false;

while(!END){
if((Network[i].L)->LayerType==2){
END=true;
}
printf("\n=======================================\n");
//printf("\nLayer:%ld, LayerSize:%lld, Weights:",i,(Network[i].L)->LayerSize);

//Only odd row gets weight matrix...
//if(i>0){
////long int Row = (Network[i].L)->LayerSize;
////long int Column = (Network[i-1].L)->LayerSize;
////PrintWeights((Network[i].L)->W,Row,Column);
//printMatrixData((Network[i].L)->W);
DeleteMatrixes((Network[i].L)->W);
////printf(", Activation:%LG\n",(Network[i].L)->H==NULL? 0:(Network[i].L)->H);
////Always 1 column
//printf("Activation:");
////PrintActivation((Network[i].L)->H,Row);
DeleteMatrixes((Network[i].L)->H);
//printMatrixData((Network[i].L)->H);
free((Network[i].L));
(Network[i].L)=NULL;
//}
//
i++;
}
//
printf("=======================================\n");
//
//
return;
}

Matrix* FeedForward(NN Network[],Matrix* IN){
//IN is vector, MUST be size of Layer 1 size.. OR error
//ith position is H layer
//jth position is IN layer

//long long int Norm = 0;
//printf("\n Normalized + Activation Function Feed >0 or regular? <=0:\n");
//scanf("%lld",&Norm);

if(IN==NULL){
printf("\n NULL FEED!! \n");
exit(-1);
}
if(IN->M==NULL){
printf("\n NULL MATRIX!!! \n");
exit(-1);
}
long long int i=0;

bool END = false;

while(!END){
if((Network[i]).L->LayerType==2){
END=true;
}

if(i==1){
Matrix*Wn = (Network[i].L)->W;
if(Wn==NULL){
printf("\n WEIGHT MATRIX NULL!!! \n");
exit(-1);
}
//Wn->Next = NULL;
//H For Layer i+1

//IF NEW MATRIX
////////////////////NEW MATRIX
(Network[i].L)->H = malloc(sizeof(Matrix));
((Network[i].L)->H)->Row = Wn->Row;
((Network[i].L)->H)->Column = IN->Column;
(Network[i].L)->H->Next=NULL;
//////////////////////NEW MATRIX
//

//Save MAtrix
((Network[i].L)->H)->M=(Multiply(Wn,IN));

//Only add bias if not final layer
if(!END){

((Network[i].L)->H)->M=realloc(((Network[i].L)->H)->M,((((Network[i].L)->H)->Row)+1)*sizeof(long double*));
if(((Network[i].L)->H)->M==NULL){
printf("\n Error Appending Bias to H...");
exit(-1);
}
((Network[i].L)->H)->M[((Network[i].L)->H)->Row]=malloc(sizeof(long double));

//


//THIS BIAS MAY BE PREDETERMINED BY LEARNING, SO SET TO APPROPRIATE BIAS, not NECESSARILY 1.0 EVERY TIME
//((Network[i].L)->H)->M[((Network[i].L)->H)->Row][0]=1.0;//(long double)rand()/(long double)RAND_MAX;
((Network[i].L)->H)->M[((Network[i].L)->H)->Row][0]=INBPAM;
//SET BIAS TO BPAM, OR THE FINAL VALUE OF INPUT
//THIS BIAS MAY BE PREDETERMINED BY LEARNING, SO SET TO APPROPRIATE BIAS, not NECESSARILY 1.0 EVERY TIME

//Set appropriate row size for INPUT!!!!!!!!!!!!!!!!!!!!!!!!!!!
((Network[i].L)->H)->Row = Wn->Row+1;

}

//if(Norm>0){
printf("\nUsing Activation Function:");
switch((Network[i].L)->ActivationFunction){
	case 1: printf("Sigmoid\n");
		Sigmoid(((Network[i].L)->H));
		break;
	case 2: printf("Relu\n");
		Relu(((Network[i].L)->H));
		break;
	case 3: printf("SoftMax\n");
		SoftMax(((Network[i].L)->H));
		break;
	default:
		printf("NONE\n");
		Nor=false;
		break;
}
//}
//else{
//Nor=false;
//}
}
else if(i>1){

Matrix*Wn = (Network[i].L)->W;
Wn->Next = NULL;
//H For Layer i+1
//Multiple ith Weight Matrix by the INPUT of prev H->Column~~~

//IF NEW MAP
////////////////////NEW MAP
(Network[i].L)->H = malloc(sizeof(Matrix));
((Network[i].L)->H)->Row = Wn->Row;
///////////////////////////////////////////IMPORTANT
((Network[i].L)->H)->Column = (Network[i-1].L)->H->Column;
//
(Network[i].L)->H->Next=NULL;
/////////////////////NEW MAP
//IF NEW MAP

((Network[i].L)->H)->M=(Multiply(Wn,(Network[i-1].L)->H));

//MULTIPLY ONLY

//Only add bias to every layer except final layer

if(!END){

//Append a bias to ACTIVATION
((Network[i].L)->H)->M=realloc(((Network[i].L)->H)->M,((((Network[i].L)->H)->Row)+1)*sizeof(long double*));
if(((Network[i].L)->H)->M==NULL){
printf("\n Error Appending Bias to H...");
exit(-1);
}
((Network[i].L)->H)->M[((Network[i].L)->H)->Row]=malloc(sizeof(long double));

//THIS BIAS MAY BE PREDETERMINED BY LEARNING, SO SET TO APPROPRIATE BIAS, not NECESSARILY 1.0 EVERY TIME
//Can Be Anything
((Network[i].L)->H)->M[((Network[i].L)->H)->Row][0]=1.0;
//Set to final double from multiply
////////////////////////////////////////////
////////////////////////////////////////////
////////////////////////////////////////////

((Network[i].L)->H)->Row = Wn->Row+1;
}

//if(Norm>0){
printf("\nUsing Activation Function:");
switch((Network[i].L)->ActivationFunction){
        case 1: printf("Sigmoid\n");
		Sigmoid(((Network[i].L)->H));
		break;
        case 2: printf("Relu\n");
		Relu(((Network[i].L)->H));
		break;
        case 3: printf("SoftMax\n");
		SoftMax(((Network[i].L)->H));
		break;
        default:
		printf("NONE\n");
                Nor=false;
		break;
}
//}
//else{
//Nor=false;
//}

//
}

i++;
}

return NULL;
}


void PrintWeights(long double**W, long long int Row, long long int Col){
if(W==NULL){
printf("NULL");
return;
}
long long int i=0;
long long int j=0;
printf("\n");
for(i=0;i<Row;i++){
for(j=0;j<Col;j++){
printf("%LG ",W[i][j]);
}
printf("\n");
}

return;
}

void PrintActivation(long double**H, long long int Row){
if(H==NULL){
printf("\nNULL");
return;
}
long int i=0;
for(i=0;i<Row;i++){
printf("%LG\n",H[i][0]);
}
return;
}

Matrix* FeedIn(NN Network[],long long int Layers){

if(Network==NULL){
printf("\n No Network to Feed\n");
return NULL;
}
if(Network[0].L==NULL){

printf("\n No IN Layer to Feed\n");
return NULL;

}

long long int i=0;
//IMPORTANT

long long int LSize;
//if(Layers>2){
LSize = (Network[0].L)->LayerSize+1;
//}
//else{
 //LSize = (Network[0].L)->LayerSize;
//}
long double** IN = malloc(LSize*sizeof(long double*));

while(i<LSize){
IN[i]=malloc(1*sizeof(long double));
i++;
}

////////////////////////////
i=0;
////////////////////////////
long double input;
while(i<LSize){
//if(Layers>2){
if(i<LSize-1){
printf("\nEnter Input %lld:",i);
}
else{
//int BPAM = 2;
printf("\nSETTING BIAS PARAM TO %LG\n",INBPAM);
IN[i][0]=INBPAM;
break;
}
//}
//else{
//printf("\nEnter Input %lld:",i);
//}
scanf("%LG",&input);
clean_stdin();

IN[i][0]=input;
i++;
}
//
i=0;
//
while(i<LSize){
printf("\nIN[%lld]:%LG",i,IN[i][0]);
i++;
}

Matrix* M = malloc(sizeof(Matrix));
M->Row = i;
M->Column = 1;
M->M = IN;
M->Next = NULL;

return M;
}

//Compute Gradient Descent for each layer... The change in weights for each layer, GIVEN ERROR VECTORS FOR EACH LAYER
Matrix* Gradient(NN Network[], Matrix* Errors, long long int Layers){
if(Network==NULL){
printf("\nNo Network Given");
return NULL;
}
if(Errors==NULL){
printf("\n No Error Vector sequence given");
return NULL;
}

long double Lr = 0.0;

while(1){
printf("\nEnter Learning Rate(Recommended to be small if given lots of data, large if not:");
scanf("%LG",&Lr);
clean_stdin();

if(Lr<=0.0){
printf("\nLearning Rate must be >0!");
}
else{
break;
}
}

Matrix* En = Errors;

//Weight Matrix for All Weights to be Changed
Matrix* DeltaW = malloc(sizeof(Matrix));
long long int Size = 0;
//DeltaW=malloc((Layers-1)*sizeof(Matrix));
//
//long long int Rows = 0;
//long long int Columns = 0;
//FIRST ERROR VECTOR IS ERROR OUTPUT TO HIDDEN N-1
//AppendMatrix(long double ** M, row, column, Matrix** Mat)
long long int i=0;
long long int j=0;
long double val = 0.0;
for(i=0,En=Errors;i<Layers&&En!=NULL;i++,En=En->Next){
//for(En=Errors;En!=NULL;En=En->Next){

//ITERATE THROUGH ERROR VECTOR n
//Sum together learning rate, error row, derivative of sigmoid
for(j=0; j<(En->Row); j++){
printf("\n ROW:%lld",j);
switch(((Network[Layers-i]).L)->ActivationFunction){
	case 1:
		val = DSig(En->M[j][0]);
		break;
	case 2:	
		val = DRelu(En->M[j][0]);
		break;
	//case 3:
	//	val = DSoftMax(En->M[j][0]);	
	//	break;
	default:
		val = 1.0;
		break;
}

printf("\nLr:%LG ** E:%LG ** D:%LG\n",Lr,En->M[j][0],val);
En->M[j][0]=Lr*En->M[j][0]*val;

}

Matrix* AT=(CopyMatrix(((Network[Layers-i]).L)->H));
ToTranspose(AT);
printf("\nActivation Transpose Matrix:\n");
printMatrixData(AT);
//APPEND 1 to activation to get back the BIAS!!

if(Size>0){
AppendMatrix(Multiply(En,AT),En->Row,AT->Column,&DeltaW);
}
else{
DeltaW->Row=En->Row;
DeltaW->Column=AT->Column;
DeltaW->M=Multiply(En,AT);
DeltaW->Next=NULL;
Size+=1;
}

ToTranspose(DeltaW);

//printf("\nResult Weight Matrix:\n");

//printMatrixData(DeltaW);

//AFTER ITERATE THROUGH ERROR VECTOR

//}
}

printf("\nResult Gradient Descent Weight Matrix FOR ALL ACTIVATIONS!:\n");

printMatrixData(DeltaW);

//exit(0);
return DeltaW;
}

Matrix* BackProp(NN Network[],Matrix* Error,long long int Layer,bool Normalized){

//Layer--;

//long long int u=0;

/*

printf("\nNormalized (>0) or Not (<=0)?");

scanf("%lld",&u);

if(u<=0){

Normalized=true;

}

else{

Normalized=false;

}

*/



printf("\nStarting Layer:%lld\n",Layer);

printf("\nError[0]: %LG",Error->M[0][0]);

printMatrixData(Error);

//printf("\nError[1]: %LG",Error->M[1][0]);

Matrix* M = NULL;

//long long int i=0;

long long int j=0;

long long int k =0;

long long int l=0;

//long long int Nexti=0;



//long long int incr = 0;



long double sum = 0.0;



long double Ns = 0.0;



long double w = 0.0;



//Add each error Matrix to this E

long double **E = NULL;



Matrix* EM=NULL;



bool first = false;





while(Layer>0){



M = (Network[Layer].L)->W;



//NEW ERROR MATRIX[PER LAYER]

//Matrix* NewE = malloc(sizeof(Matrix));



//ROWs depend on if going from small to big or big to small!!!!!!!!!!!!!!

//

long long int Row = 0.0;

//if(first){

//Row = M->Row>=M->Column? M->Column:M->Row;

Row = M->Column;

//}

//else{

//Row = M->Row;

//}

/*M->ColumnM->Row;*///(Network[Layer].L)->LayerSize;



long long int Column = 1;

long double ** NewE=malloc((Row)*sizeof(long double*));

long long int r=0;

while(r<Row){

NewE[r]=malloc(Column*sizeof(long double));

r++;

}



//printf("=======================");

printf("=======================================");



printf("\n\n New Error Matrix Size: %lld",Row);



for(k=0;k<Row;k++){

//Next Row

for(l=0;l<M->Row;l++){

//for(i=0;i<M->Column;i++){

//Find portional value per node weighed

w = M->M[l][k];

if(!Nor){

for(j=0;j<M->Column;j++){

//For every node in row

//J> i so not repeat

//Sum up weight

printf("\n %LG + %LG\n",Ns,M->M[l][j]);

Ns+=M->M[l][j];



//}



}

}

//break;

//}

//printf("\n %LG // %LG **\n",w,sum);

if(Nor){

Ns=1;

}

printf("\nROW ITERATED TO:%lld\n",l);

//if(Ns!=0.0){

if(!first){

if(Ns!=0)
printf("\n %LG ++ (%LG // %LG) ** %LG\n~~~~~~~ADD NEXT RATIO~FRM FIRST~~~~~~",sum,w,Ns,Error->M[l][0]);
else
printf("\n %LG + 0.0\n",sum);

sum+= Ns==0? 0.0:(w/Ns)*Error->M[l][0];

}

else{

if(E==NULL){

printf("\n PREV ERROR MUST BE SET!\n");

exit(-1);

}
if(Ns!=0)
printf("\n %LG ++ %LG // %LG **%LG\n~~~~~~~ADD NEXT RATIO~~~~~~~",sum,w,Ns,E[l][0]);
else
printf("\n %LG ++ 0.0\n",sum);

sum+=Ns==0? 0.0:(w/Ns)*E[l][0];

}

//}

//else{

//sum+=0;

//}

Ns=0.0;


//if(l+1==M->Column){

//break;

//}

}



printf("\n\n\n------NEXT ERROR MATRIX ROW:%lld------\n\n",k);

NewE[k][0]=sum;



//printf("=======================");



sum=0;



//Nexti++;

}

//Append to Error Matrix

AppendMatrix(NewE, Row, Column, &EM);

//New Error Vector for next Layer!

printf("\n OUTPUT Error: ");

printMatrixData(Error);

printMatrixData(EM);

E=NewE;

if(!first){

first=true;

}

printf("=======================================");



//Nexti=0;

if(Layer-1>0){

printf("\n\n-----NEW ERROR VECTOR-----");

}



Layer--;

}



printf("\nSuccessful BackPropogation: 1st vector is Error output\nNext Vectors are error vectors for next hidden layer with biases\n");



printMatrixData(EM);



return EM;

}

void PromptUserSave(NN Network2[], Matrix* E, long long int Layer){

long long int answer = 0;

printf("\n Save Network and Error? >0 for yes, <=0 for no");

scanf("%lld",&answer);
clean_stdin();

if(answer>0){
printf("\n Enter Model #:");

scanf("%lld",&answer);
clean_stdin();

char* s = NULL;

long long int neg = 0;
if(answer<0){
neg = 1;
answer = answer*-1;
}

if(answer!=0){
if(neg!=0){
char *  str = malloc((long long int)((ceil(log10(answer))+7)*sizeof(char)));
str[0] = 'M';
str[1] = 'o';
str[2] = 'd';
str[3] = 'e';
str[4] = 'l';
str[5] = '-';

sprintf((str+6), "%lld", answer);

printf("\n FILE:%s",str);
s = str;
}
else{
char * str = malloc((long long int)((ceil(log10(answer))+7)*sizeof(char)));
str[0] = 'M';
str[1] = 'o';
str[2] = 'd';
str[3] = 'e';
str[4] = 'l';
str[5] = '-';
//str[6] = '-';

sprintf((str+6), "%lld", answer);

printf("\n FILE:%s",str);
s = str;

}
}
else{

//long long int answer = 0;
/*char str[7*sizeof(char)];
str[0] = 'M';
str[1] = 'o';
str[2] = 'd';
str[3] = 'e';
str[4] = 'l';
str[5] = '-';
str[6] = '0';
*/

char* str = "Model-0";

printf("\n FILE:%s",str);
s = str;

}
if(access(s,F_OK)!=-1){
printf("\n FILE:%s Found, OverWrite >0 or Not? <=0",s);
scanf("%lld",&answer);
clean_stdin();

if(answer<=0){
printf("\n Not Saving Network");
//Nothing else to do, perhaps ask user to try again
return;
}
}

FILE* F = fopen(s,"w+");

if(F==NULL){
printf("FAILED TO OPEN:%s",s);
exit(0);
}

SaveNetwork(Network2,F,E,Layer);

fclose(F);

}
else{
printf("\nNot Saving Network");
//Nothing else to do, perhaps ask user to try again
}

return;
}

RestoredNet*  RestoreNetwork(){

bool Quit = false;
long long int answer = 0;
do{

printf("\n Enter Model #:");

scanf("%lld",&answer);
clean_stdin();

long long int neg = 0;

if(answer<0){
neg = 1;
answer = answer*-1;
}

char* s=NULL;

if(answer!=0){
if(neg!=1){
char * str = malloc((long long int)((ceil(log10(answer))+7)*sizeof(char)));
str[0] = 'M';
str[1] = 'o';
str[2] = 'd';
str[3] = 'e';
str[4] = 'l';
str[5] = '-';

sprintf((str+6), "%lld", answer);

printf("\n RESTORE FILE:%s",str);
s = str;
}
else{
char * str = malloc((long long int)((ceil(log10(answer))+7)*sizeof(char)));
str[0] = 'M';
str[1] = 'o';
str[2] = 'd';
str[3] = 'e';
str[4] = 'l';
str[5] = '-';
//str[6] = '-';
sprintf((str+6), "%lld", answer);

printf("\n RESTORE FILE:%s",str);
s = str;
}
}
else{
char * str = malloc(8*sizeof(char));
str[0] = 'M';
str[1] = 'o';
str[2] = 'd';
str[3] = 'e';
str[4] = 'l';
str[5] = '-';
str[6]= '0';

//sprintf((str+6), "%lld", answer);

printf("\n RESTORE FILE:%s",str);
s = str;
}

if(access(s,F_OK)!=-1){
printf("\n FILE:%s Found",s);

//RestoredNet* Restored=malloc(sizeof(RestoredNet));
//Restored->NetworkR = NULL;
//Restored->ErrorR=NULL;

FILE* F = fopen(s,"r");
if(F==NULL){
printf("\n Error Opening: %s",s);
return NULL;
}

RestoredNet* Restored=malloc(sizeof(RestoredNet));
//GET NETWORK LAYER COUNT
long long int NetLen;
fscanf(F,"%lld",&NetLen);
//Poing to Static array
Restored->NetLen=NetLen;

printf("\n Network Length: %lld",NetLen);

if(NetLen<=0){
printf("\n Net LEngth has to be >0!");
exit(-2);
}

Restored->NetworkR = malloc(NetLen*sizeof(NN));
//AppendMatrix(**,r,c,M**)
Restored->ErrorR=NULL;
//Error For each layer

NN* Network = Restored->NetworkR;
Matrix* E = Restored->ErrorR;


bool End = false;

//INPUT LAYER......
Network[0].L=malloc(sizeof(Layer));
Network[0].L->W=NULL;
Network[0].L->H=NULL;
Network[0].L->LayerType = 0;
//INPUT LAYER.....

long long int i=1;

/*
char* s = malloc(2*sizeof(char));
s[0]='\0';
s[1]='\0';
*/

char c1 = ' ';

char c2 = ' ';

FGo(F,'W');

while(i<NetLen){
//Get First digit for weight matrix
//if(c1!='\n'){
FGo(F,'W');
//}

((Network[i].L))=malloc(sizeof(Layer));


if(i==NetLen-1){
//OUTPUT LAYER TYPE
(Network[i].L)->LayerType=2;
}
else{
////HIDDEN LAYER TYPE
(Network[i].L)->LayerType=1;
}

/*
((Network[i]).L)->W=malloc(sizeof(Matrix));
Matrix* W = ((Network[i]).L)->W;
*/
long long int R1 = 0;
long long int C1 = 0;
//ROW
fscanf(F,"%lld\n",&(R1));
//COLUMN
fscanf(F,"%lld\n",&(C1));

printf("\n W ROW: %lld, W COLUMN: %lld\n",R1,C1);

//NOW READ MATRIX DATA TO W

Storage* St = NULL;
//
c2 = readData(F,&St,R1);

Matrix* BASE = StoreMatrix(St,R1,C1);
/*
free(((Network[i]).L)->W);
((Network[i]).L)->W=NULL;
*/
((Network[i]).L)->W=BASE;

//AppendMatrix(BASE->M,W->Row,W->Column,&W);

//free(BASE);
//BASE=NULL;

DeleteStorage(St);

if(c2!='\n'){
FGo(F,'A');
}

//((Network[i].L))=malloc(sizeof(Layer));

long long int R2 = 0;
long long int C2 = 0;

//((Network[i]).L)->H=malloc(sizeof(Matrix));
//Matrix* H = ((Network[i]).L)->H;


//ROW
fscanf(F,"%lld\n",&(R2));
////COLUMN
fscanf(F,"%lld\n",&(C2));
//
printf("\n Activation ROW: %lld, Activation COLUMN: %lld\n",R2,C2);
//

//NOW READ MATRIX DATA TO H
Storage* St2 = NULL;
//

c1 = readData(F,&St2,R2);

Matrix* BASE2 = StoreMatrix(St2,R2,C2);
/*
free(((Network[i]).L)->H);
((Network[i]).L)->H=NULL;
*/
((Network[i]).L)->H=BASE2;

//AppendMatrix(BASE2->M,H->Row,H->Column,&H);

//free(BASE2);
//BASE2=NULL;

DeleteStorage(St2);

//
//
//
printf("\nLAYER TYPE:%ld\n",(Network[i].L)->LayerType);

i++;
}
//printf("\n\n");
i=0;
long long int R;
long long int C;

char c=c2;

printf("\nSTART ERROR MATRICES\n");

while(i<NetLen){

if(c!='\n'){
FGo(F,'E');
}

printf("\n");

fscanf(F,"%lld\n",&R);
//////COLUMN
fscanf(F,"%lld\n",&C);
////
printf("\n Error ROW: %lld, Error COLUMN: %lld\n",R,C);
////
//NOW READ MATRIX DATA TO E. APPEND TO E(READ DATA)
Storage* St = NULL;
c = readData(F,&St,R);
Matrix* M = StoreMatrix(St,R,C);
AppendMatrix(M->M,R,C,&E);
free(M);
M=NULL;

DeleteStorage(St);

i++;
}

printf("\nNETWORK INFO:\n");

PrintNetwork(Network);

printf("\nERROR INFO:\n");
printMatrixData(E);

Restored->ErrorR=E;

Quit=true;

fclose(F);

return Restored;
}

else{
printf("\nFile Not Found, Quit <=0 or Continue? >0");
scanf("%lld",&answer);
clean_stdin();

if(answer<=0){
Quit=true;
}
}

}while(!Quit);

return NULL;
}

/*
 * Given prompt, state it, then return input
 */
int PromptGeneral(char* Prompt){
printf("\n %s \n",Prompt);
int Input;
scanf("%d",&Input);
clean_stdin();

return Input;
}


/**
 * Save Network
 */
void SaveNetwork(NN Network[],FILE* F, Matrix* E, long long int Layer){
//Get Info from Net and Save it into file given
long long int i=0;
//fputc(c,F) PUTS CHARACTER
bool END = false;

long long int Truncate = 0;

printf("\n Truncate Save? >0 Yes <=0 No");
scanf("%lld",&Truncate);
clean_stdin();

Layer++;

if(Layer<=0){
printf("SAVE NET LAYER FAILURE");
exit(0);
}

printf("\n\n\nLAYER:%lld",Layer);

char Lf[(long long int)(ceil(log10(Layer))*sizeof(char))];

sprintf(Lf,"%lld",Layer);

fputs(Lf,F);

fputs("\n",F);

while(!END){
if((Network[i].L)->LayerType==2){
END=true;
}

//printf("\n=======================================\n");
//printf("\nLayer:%lld, LayerSize:%lld, Weights:",i,(Network[i].L)->LayerSize);

//char L[(int)(ceil(log10(i))+ceil(log10((Network[i].L->LayerSize)))+1)];
if(Truncate<=0){
fputs("=======================================\n",F);
}
else{
fputs("=\n",F);
}
//char L1[(long long int)(ceil(log10(i))*sizeof(char))+1];
//sprintf(L1,"%lld",i);
if(Truncate<=0){
char L2[(long long int)(ceil(log10((Network[i].L)->LayerSize))*sizeof(char))+1];
sprintf(L2,"%lld",(Network[i].L)->LayerSize);

fputs("Layer:",F);

if(i!=0){
char L1[(long long int)(ceil(log10(i))*sizeof(char))+1];
sprintf(L1,"%lld",i);

fputs(L1,F);

}else{

fputs("0",F);

}
fputs(", ",F);
fputs("LayerSize:",F);
fputs(L2,F);
fputs(",",F);
fputs(" Weights:\n",F);
}
else{
fputs("W\n",F);
}
//fputs("\n=======================================\n\nLayer:%lld, LayerSize:%lld, Weights:\n",F);
//Only odd row gets weight matrix...
if(i>0){
if(Truncate<=0){
putMatrixData((Network[i].L)->W,F,0);
//fputs("\n",F);
fputs("Activation\n",F);
putMatrixData((Network[i].L)->H,F,0);
//fputs("\n",F);
}
else{
putMatrixData((Network[i].L)->W,F,0);
fputs("A\n",F);
putMatrixData((Network[i].L)->H,F,0);
}
}
//

i++;
}
if(Truncate<=0){
//printf("\n Error\n");
fputs("Error\n",F);
}
else{
//printf("\n E\n");
fputs("E\n",F);
}
putMatrixData(E,F,1);


return;
}


char FGo(FILE* F,char c){

char d = '0';

while(fscanf(F,"%c",&d)!=EOF){
//If Found char
printf("%c",d);
/*if(isdigit(d)){
break;
}
*/
if(d==c){

while(fscanf(F,"%c",&d)!=EOF){
//Now Find First digit
printf("%c",d);
if(d=='\n'){
break;
}
}

break;
}

}
if(d!='\n'){
printf("\n Error, Restore Size Invalid");
exit(-1);
}

return d;
}


Matrix* StoreMatrix(Storage* S,long long int Row, long long int Col){
//Return matrix
if(Row<=0||Col<=0||S==NULL){
printf("\nNo Matrix To Store\n");
return NULL;
}

Matrix*M = malloc(sizeof(Matrix));
M->Row=Row;
M->Column = Col;
M->M=malloc(Row*sizeof(long double*));

long long int i=0;
while(i<Row){
M->M[i]=malloc(Col*sizeof(long double));
i++;
}

Storage*S1 = S;

while(S1!=NULL){
M->M[S1->Row][S1->Column]=S1->Data;
Storage* S2 = S1->Next;
S1=S2;
}


return M;
}

void DeleteRestoredNet(){
RestoredNet* R = RN;
if(R==NULL){
return;
}
DeleteNetwork(R->NetworkR);
free(R->NetworkR);
R->NetworkR=NULL;
DeleteMatrixes(R->ErrorR);
free(R);
R=NULL;
return;
}

void clean_stdin(void){
int c;
    while((c = getchar()) != '\n' && c != EOF);
}
