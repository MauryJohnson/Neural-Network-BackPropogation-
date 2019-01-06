#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
//void printData();
char* AllData;

long long int row;
long long int column;
long long int NXN;
int power=0;

typedef struct Storage_{
long long int Row;
long long int Column;
long double Data;
struct Storage_* Next;
}Storage;

typedef struct Matrix_{

long long int Row;

long long int Column;

long double**M;

struct Matrix_* Next;

}Matrix;

void printData();

//Store the values ( Nums) for A
char readData(FILE* F, Storage** S, long long int Counted);

void putMatrixData(Matrix*M, FILE* F, long long int type);

void printMatrixData(Matrix* M);

void AddToStorage(long double data,Storage** Si,long long int rows, long int columns);

void PrintStorage(Storage*Si);

void DeleteStorage(Storage*Si);

//Delete all matrices 100%
void DeleteMatrixes(Matrix* M1);

//MATRIX TO A POWER 100%
long long int Power(Matrix* M1, long int power);

//Multiply Matrix 1 with Matrix 2, 100%
long double** Multiply (Matrix* M1, Matrix* M2);

//Sig all Values?
long double** Sigmoid(Matrix*M1);

long double** Transpose(Matrix* M);

void ToTranspose(Matrix* M);

//Type 1 for the identity matrix, anything else for normal matrix
long double ** CreateMatrix(long long int rows, long int columns,int type, Matrix** M, Storage* S);

//Appen Matrix 100%
void AppendMatrix(long double** D,long long int row,long long int column, Matrix** m);

//Copy Matrix 100%
//long double ** CopyMatrix(long long int rows, long int columns, long double**M);

Matrix * CopyMatrix(Matrix* M);


long double** Inverse(Matrix* M1, Matrix* M2);

bool isRREF(long double** M1);

long double** AddRow(long double entry, long double** M1, long long int row);

long double** AddRows(long double entry, long double** M1, long long int row1, long int row2);

long double** MultiplyRow(long double entry, long double** M1, long long int row);

void freeMatrix(long double** M, long long int column);

long double ** EMATRIX;

long long int numOfPow(long double entry);

long double *** MRecord;

long long int MCount = 0;

Matrix* BASE = NULL;

//Storage* St=NULL;

//TEST MAIN
/*
int main(int argc, char** argv){



}
*/
/*
int main(int argc ,  char** argv){

FILE* Accessed = fopen(argv[1],"r");
FILE* Accessed2 = fopen(argv[2],"r");

if(Accessed==NULL){
printf("Cannot open file 1");
return 0;
}
if(Accessed2==NULL){
printf("Cannot open file 2");
return 0;
}


else{

Storage* St = NULL;
Storage* S2 = NULL;
//
readData(Accessed,&St);

long long int rows = row;
long long int columns=column;

//return 0;

//columns++;

CreateMatrix(rows,columns,0,&BASE,St);
//CreateMatrix(rows,columns,0,&BASE,St);
CreateMatrix(columns, columns,1,&BASE,St);
//NXN echelon Matrix
//printf("\n\n\n");
//printMatrixData(BASE);
//return 0;

//Take transpose of same matrix
Transpose(BASE->Next);
//Multiply

//printf("\n\n\n");
//printMatrixData(BASE);
//return 0;

//A^t times A 
Multiply(BASE->Next->Next->Next,BASE->Next);

//Inverse of A^tA, given echelon format Matrix
Inverse(BASE->Next->Next->Next->Next,BASE->Next->Next);
//printf("\n\n\n");
//printMatrixData(BASE);
//return 0;
//Multiply (A^tA)^-1 with A^t
Multiply(BASE->Next->Next,BASE->Next->Next->Next);
//printf("\n\n\n");
//printMatrixData(BASE);
//return 0;
//Multiply this with values you want to learn
Multiply(BASE->Next->Next->Next->Next->Next,BASE);
//Mult A with (A^tA)^-1(A^t)
//This is matrix corresponding to weights needed
//Multiply(BASE->Next,BASE->Next->Next->Next->Next->Next);
//printf("\n\n\n");
//printMatrixData(BASE);
//return 0;
Multiply(BASE->Next,BASE->Next->Next->Next->Next->Next->Next);


//Transpose(BASE);
//printf("\n\n\n");
//printMatrixData(BASE);
//return 0;


readData(Accessed2,&S2);

rows = row;
columns= column;

columns++;
CreateMatrix(rows,columns,0,&BASE,S2);

//BASE->Next->Next->Next->Next->Next->Next->Row--;

Multiply(BASE->Next->Next->Next->Next->Next->Next->Next->Next->Next,BASE->Next->Next->Next->Next->Next->Next);

printMatrixData(BASE);

DeleteStorage(St);
DeleteStorage(S2);
}


return 0;
}

*/

char readData(FILE* Accessed, Storage** St,long long int Counted){
	
//long double v;
char g = ' ';
long long int columns=0;
long long int rows=0;

char* str =malloc(2*sizeof(char));
bzero(str,2);
char* app = malloc(2*sizeof(char));
bzero(app,2);

int DCount=0;
long long int NCount=0;

long long int MaxCol = 0;
//int MaxRow = 0;
//Found first indicator
int first=0;
//Set First to -1 And MaxCol and MaxRow

bool C = false;

if(Counted>0){
C=true;
}
long long int MaxRow=Counted;

bool isNum=false;

while(fscanf(Accessed,"%c",&g)!=EOF){
//New row or new column...
app[0]=g;

if(g=='\n'){


if(C==true){
//If want to count rows
if(rows>=MaxRow){
//DONE
//printf("\n DONE COUNTED:%lld",Counted);
break;

}

}

/*if(rows==MaxRow){
break;
}
*/
//Update
//rows++;
//columns=0;
if(first<=0){
first=1;
MaxCol = columns+1;
}
//printf("\nNew row:%d",rows);

/*
if(rows>row-1){
printf("\n Row Counting failure\n");
exit(-1);
}
if(rows==row-1){
//Total row limit reached or exceeded
printf("\n Row limit reached\n");
break;
}
*/

if(isNum==true){
//Get str..

long double val;

sscanf(str,"%LG",&val);

if(NCount%2==1){
//Neg Num
val = val*-1.0;
}

AddToStorage(val,St,rows,columns);

free(str);
str=NULL;
str = malloc(2*sizeof(char));
bzero(str,2);
}

else{
/*
printf("\n Failed to find number:%s",str);
exit(-1);
*/
}

rows++;
columns=0;
//
//printf("\nNew row:%d",rows);

NCount=0;
DCount = 0;
isNum=false;
}

else if(g==','||g==' '){
//New Col. same row
//columns++;
//printf("\n NEW COL: %d",columns);
//

if(first==1){

if(columns>MaxCol-1){
printf("\n COL Counting failure\n");
exit(-1);
}

if(columns==MaxCol-1){
//Total row limit reached or exceeded
printf("\n COL limit reached\n");
break;
}

}

if(isNum==true){

long double val;

sscanf(str,"%LG",&val);

if(NCount%2==1){
//Neg Num
val = val*-1.0;
}

AddToStorage(val,St,rows,columns);

free(str);
str=NULL;
str = malloc(2*sizeof(char));
bzero(str,2);
}

else{

if(rows>=MaxRow){
printf("\nCOMPLETE COUNT");

break;
}

printf("\n Failed to find number:%s",str);
exit(-1);
}

columns++;
//printf("\n NEW COL: %d",columns);

NCount=0;
DCount = 0;
isNum=false;
}

else if(isdigit(g)){
//Append digit to str
isNum=true;
str=realloc(str,(strlen(str)+2)*sizeof(char));
strcat(str,app);
}

else if(g=='.'){
//Append digit decimal to str
DCount++;
if(DCount>1){
printf("\n INVALID DOUBLE:%s%c\n",str,g);
exit(-1);
}
str=realloc(str,(strlen(str)+2)*sizeof(char));
strcat(str,app);
}

else if (g=='-'){
if(isNum==true){
printf("\n FAILURE, NUMBER NOT VALID:%s",str);
//Negative must be in front of num...
exit(-1);
}
NCount++;
}
}


//printf("\nCOUNTED:%lld",Counted);

if(Counted<=0){
fclose(Accessed);
}

//MaxCol++;

//printf("ROWS: %lld COLUMNS:%lld",rows,MaxCol);
//int h=0;
columns=MaxCol;
//printf("%s",AllData);

//printf("STORAGE:\n");
//PrintStorage(*St);

//printf("\nFINAL CHAR:%c",g);

//printf("ROWS:%lld COLUMNS:%lld",rows,columns);

row=rows;
column=columns;

if(app!=NULL){
free(app);
app=NULL;
}
if(str!=NULL){
free(str);
str=NULL;
}
printf("ROWS:%lld COLUMNS:%lld",row,column);

printf("\n");
//PrintStorage(*St);

return g;
}	


void printData(){
long long int f = 0;
//int SIZE = sizeof(AllData)/sizeof(AllData[0]);
while(AllData[f]!= '\0'){


printf("%c", AllData[f]);

f++;
}	                 
return;

} 

void printMatrixData(Matrix* M){
if(M==NULL){
printf("NULL\n\n");
return;
}
long long int i = 0;// M->rows;
long long int k = 0;// M->columns;

printf("\nROWS:%lld COLUMNS:%lld\n",M->Row, M->Column);
//if(M->Next==NULL){
//printf("PREDICTED PRICES\n");
//}
for(i=0;i<M->Row;i++){
	for(k=0;k<M->Column;k++){
	printf("%LG ",/*i==M->Row-1? "Bias:":"Weight:",*/(M->M)[i][k]);
	}
	printf("\n");
}
//printf("\n");
printMatrixData(M->Next);
return;
}       

void putMatrixData(Matrix*M, FILE* F, long long int type){

if(M==NULL){
printf("NULL\n\n");
return;
}
long long int i = 0;// M->rows;
long long int k = 0;// M->columns;

char R[(long long int)((ceil(log10(M->Row))+1)*sizeof(char))];
bzero(R,(long long int)((ceil(log10(M->Row))+1)*sizeof(char)));
char C[(long long int)((ceil(log10(M->Column))+1)*sizeof(char))];
bzero(C,(long long int)((ceil(log10(M->Row))+1)*sizeof(char)));

//printf("\nROWS:%lld COLUMNS:%lld\n",M->Row, M->Column);

sprintf(R,"%lld",M->Row);
sprintf(C,"%lld",M->Column);

printf("\nROWS:%s COLUMNS:%s\n",R,C);


fputs(R,F);
fputs("\n",F);
fputs(C,F);
fputs("\n",F);


for(i=0;i<M->Row;i++){
    for(k=0;k<M->Column;k++){
    //printf("%LG ",(M->M)[i][k]);
    
    //char V[(long long int)((ceil(log10((long long int)(M->M)[i][k]))+1)*sizeof(char))];
    //bzero(V,(long long int)((ceil(log10((long long int)(M->M)[i][k]))+1)*sizeof(char)));
    char V[1000];
    //sprintf(V,"%LG",(M->M)[i][k]);
    snprintf(V,1000,"%LG",(M->M)[i][k]);
    printf("%s ",V);

    //
    fputs(V,F);
    fputs(" ",F);
    }
    fputs("\n",F);
    printf("\n");
    }
                                //printf("\n");
    if(type>0&&M->Next!=NULL){
    fputs("E\n",F);
    }
    putMatrixData(M->Next,F,type);
                              

//fputs("\n",F);
return;
}

//Return pointer to entire matrix multiplied
long double ** Multiply(Matrix* M1, Matrix* M2){
if(M1==NULL||M2==NULL){
printf("\nNULL MATRICES\n");
return NULL;
}

printf("\nMULTIPLY:\n");
printMatrixData(M1);
printf("\nWITH:\n");
printMatrixData(M2);

if(M1->Column!=M2->Row){
printf("\nRow:%lld is not the same as Column:%lld, CANNOT MULTIPLY\n",M2->Row,M1->Column);
return NULL;
}

long double** NewM = malloc(M1->Row*sizeof(long double*));
//Column by Row Matrix.
long long int g=0;

while(g<M1->Row){
NewM[g]=malloc(M2->Column*sizeof(long double));
g++;
}

long long int i=0;
long long int k=0;
long long int l=0;
//int m=0;
//Columns then rows
long double sum =0;
long double mult = 1;

//printf("MATRIX ROWS:%lld COLUMNS:%lld:\n",M1->Row,M2->Column);
//printMatrixData(NXN,Matrix1);
//printf(" Times MATRIX 2:\n");
//printMatrixData(NXN,Matrix2);
printf("\n");

for (i=0; i<M1->Row; i++){
     //m=0;
     for(k=0; k<M2->Column; k++){
	//OR M2->Row
	//m=k;
	//if(m<M1->Column){	
	for(l=0; l<M1->Column&&l<M2->Row; l++){
	//Iterate columns of M1, rows of M2 [col][row]
	//printf(" SECTION [%d,%d]*[%d,%d], %LG * %LG \n",i,l,l,k,(M1->M)[l][i],(M2->M)[k][l]);
	mult = (M1->M)[i][l]*(M2->M)[l][k];	
	//printf("MULTIPLY:%LG BY:%LG = :%LG",(M1->M)[l][i],(M2->M)[k][l],mult);
	sum+=mult;
	}
	printf("\nSUM[%lld][%lld] :%LG\n",i,k,sum);
	NewM[i][k]=sum;
	sum=0;
	//NEXT COLUM
	}
	
	//}
	//NEXT ROW
}

//printf("\nNEW MATRIX\n\n");
//printMatrixData(NXN,NewM);

//freeMatrix(Matrix1);
//freeMatrix(Matrix2);
/*
for(i=0;i<NXN;i++){
for(k=0;k<NXN;k++){
//Copy NewM to M1
Matrix1[k][i]=NewM[k][i];

}
}

freeMatrix(NewM);

*/

//freeMatrix(Matrix1);

AppendMatrix(NewM,M1->Row,M2->Column,&M1);
//Simpler to just return the new matrix
return NewM;
}

void freeMatrix(long double** Matrix, long long int column){
if(Matrix==NULL){

return;
}

long long int i=0;

while(i<column){
free(Matrix[i]);
Matrix[i]=NULL;
i++;
}

free(Matrix);
Matrix=NULL;


return;
}

long double** AddRow(long double entry, long double** M1, long long int column){
//long long int* TotalRow=M1[row][column];
//Total row space
if(column<0||column>=NXN){
printf("\n INVALID COLUMN #\n");
return M1;
}
long long int i=0;
while(i<NXN){
printf("\nROW:%lld:%LG\n",i,M1[i][column]);
M1[i][column]+=entry;
i++;
}

return M1;
}

long double** AddRows(long double entry, long double** M1, long long int column1, long int column2){
//long long int* TotalRow=M1[row][column];
////Total row space
if(column1<0||column1>=NXN||column2<0||column2>=NXN){
printf("\n INVALID COLUMN #\n");
return M1;
}

long long int i=0;
while(i<NXN){
//printf("\nROW:%LG:%LG\n",i,M1[i][column]);
M1[i][column2]=M1[i][column2]+(entry*M1[i][column1]);
i++;
}

return M1;
}

long double** MultiplyRow(long double entry, long double** M1,long long int column){
//long long int* TotalRow=M1[row][column];
////Total row space
if(column<0||column>=NXN){
printf("\n INVALID COLUMN #\n");
return M1;
}
long long int i=0;
while(i<NXN){
printf("\nROW:%lld:%LG\n",i,M1[i][column]);
M1[i][column]*=entry;
i++;
}

return M1;
}


long double** Inverse(Matrix* M1, Matrix* EM){
long long int i=0;
long long int j=0;
long long int k =0;
long double divisor = 0;
NXN=EM->Column;
long double** Matrix = M1->M;
long double** EMatrix = EM->M;

if(!isRREF(EMatrix)||M1->Row!=M1->Column||EM->Row!=EM->Column){
printf("DO NOT HAVE AN IDENTITY MATRIX");
return NULL;
}


for(i=0;i<NXN;i++){
    for(j=0;j<NXN;j++){
    if(i==j){
       if(Matrix[j][i]!=1){
	//Make row such that == 1!
	
	divisor = Matrix[j][i];
	if(divisor!=0){
	MultiplyRow(1.0/divisor,Matrix,i);
        //divisor=EMatrix[j][i];
        MultiplyRow(1.0/divisor,EMatrix,i);
	}
	else{
	printf("\nCANNOT REDUCE\n");
	//if(i<NXN)
	//i++;
	//continue;
	break;
	}
	}
	//PIVOT
	k=0;
	while(k<NXN){
	//Iterate down all rows above and below pivot column
	//Matrix[j][k]... 1 .....
	//NOT PIVOTS
	if(k!=j){   
	if(Matrix[j][k]<0||Matrix[j][k]>0){
        //AddRows(-Matrix[j][k],Matrix,i,k);
        //divisor =  
	AddRows(-Matrix[j][k],EMatrix,i,k);		
	AddRows(-Matrix[j][k],Matrix,i,k);


	}
	else{
	//printf("\nRow:%LG Column:%ld is already 0, %LG \n",j,k,Matrix[j][k]);
	}
	
	if(Matrix[j][j]!=1){
        //Make row such that == 1!
        divisor = Matrix[j][j];
        if(divisor!=0){
	MultiplyRow(1.0/divisor,Matrix,j);
        //divisor=EMatrix[k][k];
	MultiplyRow(1.0/divisor,EMatrix,j);
        }
	else{
	printf("CANNOT REDUCE");
	//if(j<NXN)
	//j++;
	break;
	}
	}
	
        //
	}
	k++;   
	}
	
	
        }
     printf("\n MATRIX NOW:\n");
	printMatrixData(M1);
     
     printf("\n EEEMATRIX NOW:\n");
        printMatrixData(EM);
     //printf("\nMATRIX[%LG][%ld] : %LG\n",j,i,Matrix[j][i]);
     //printf("EMATRIX[%LG][%ld] : %LG",j,i,EMatrix[j][i]);
    }
     printf("\n");
}

//printf("INVERSE!!!");
//printMatrixData(,EMatrix);

//AppendMatrix(EMatrix,j,i,&M1);

return Matrix;
}

bool isRREF(long double** Matrix){
long long int i = 0;
long long int j = 0;

for(i=0;i<NXN;i++){
    for(j=0;j<NXN;j++){
    if(i==j){ 
	if(Matrix[j][i]==1){

	}
	else{
        printf("\nNot RREF @ %lld,%lld,%LG\n",j,i,Matrix[j][i]);
        return false;
	}   	


    }
    else{
	if(Matrix[j][i]==0||numOfPow(Matrix[j][i])>14){	
	}
	else{
        printf("\nNot RREF @ %lld,%lld,%LG\n",j,i,Matrix[j][i]);
	return false;
	}
	}
    }
}
printf("\nIT Is RREF\n");
return true;
}


long long int numOfPow(long double entry){
long long int i=0;
if(entry>0&&entry<1){
while(entry<=1){
entry=entry*10;
i++;
}
}
else if(entry>-1&&entry<0){
while(entry>=-1){
entry=entry*10;
i++;
}
}
return i;
}

long double** CreateMatrix(long long int rows,long int columns, int type, Matrix **M1, Storage* S){
// 0 for normal matrix, 1 for  identity matrix
if(rows<1||columns<1){
return NULL;
}

//long double** Y;

long double**M=malloc((columns)*sizeof(long double*));
long long int i=0;
while(i<columns){
M[i]=malloc(rows*sizeof(long double));
i++;
}

long double** Y=NULL;

long long int j=0;
if(type==1){
for(i=0;i<columns;i++){
for(j=0;j<rows;j++){
if(j==i){
M[j][i]=1;
}
else{
M[j][i]=0;
}

}
}
}
else{
printf("\nHERE");
Y=malloc(sizeof(long double*));
Y[0] =malloc(rows*sizeof(long double));
Storage** Su=&S;
while(*Su!=NULL){
printf("Row:%lld Column:%lld Data: %LG ADDED\n",(*Su)->Row,(*Su)->Column,(*Su)->Data);
//printf("\n\n\n");
if((*Su)->Column<columns-1){
M[(*Su)->Column+1][(*Su)->Row]=(*Su)->Data;
}
/*
if((*Su)->Column==0){
M[(*Su)->Column][(*Su)->Row]=1.0;
}
*/
if((*Su)->Column==columns-1){
Y[0][(*Su)->Row]=(*Su)->Data;
}

Su=&((*Su)->Next);
}
}

/*
Matrix* M2=M1;
Matrix* M3;
while(M2!=NULL){
M3=M2->Next;
}
*M1=malloc(sizeof(Matrix));
(*M1)->Row=rows;
(*M1)->Column=columns;
(*M1)->Next=NULL;
(*M1)->M=M;
*/

if(Y!=NULL){
AppendMatrix(Y,rows,1,M1);
}

if(*M1==NULL){
 *M1=malloc(sizeof(Matrix));
(*M1)->Row=rows;
(*M1)->Column=columns;
(*M1)->Next=NULL;
(*M1)->M=M;
}
else{
AppendMatrix(M,rows,columns,M1);
}
return M;
}

//Returns a copy of the matrix long double ** form
/*long double** CopyMatrix(long long int rows, long int columns, long double** Matrix){

if(rows<1||columns<1){
return NULL;
}

long double**M=malloc(rows*sizeof(long double*));
long long int i=0;

while(i<rows){
M[i]=malloc(columns*sizeof(long double));
i++;
}
long long int j=0;

for(i=0;i<rows;i++){
for(j=0;j<columns;j++){
M[i][j]=Matrix[i][j];
}

}

return M;
}
*/

Matrix * CopyMatrix(Matrix* M){
if(M==NULL){
return NULL;
}

Matrix*Copy = malloc(sizeof(Matrix));
Copy->Row=M->Row;
Copy->Column=M->Column;
Copy->Next=NULL;
Copy->M=malloc(Copy->Row*sizeof(long double*));

long long int i=0;

while(i<Copy->Row){
Copy->M[i]=malloc(M->Column*sizeof(long double));
i+=1;
}

long long int j=0;

for(i=0;i<Copy->Row;i+=1){
for(j=0;j<Copy->Column;j+=1){
Copy->M[i][j]=M->M[i][j];
}
}

return Copy;
}

void AddToStorage(long double data, Storage** S,long long int rows, long int columns){

if(*S==NULL){
//printf("\nADDING: %LG\n",data);
*S=malloc(sizeof(Storage));
(*S)->Next=NULL;
(*S)->Data=data;
(*S)->Row=rows;
(*S)->Column=columns;
PrintStorage(*S);
return;
}

AddToStorage(data,&((*S)->Next),rows,columns);

return;
}

void DeleteStorage(Storage*S){
Storage* G = NULL;
while(S!=NULL){
G=S->Next;
free(S);
S=NULL;
S=G;
//printf("FREE");
}
return;
}

void PrintStorage(Storage* S){
if(S!=NULL){
printf("Row:%lld Column:%lld Data:%LG\n",S->Row,S->Column,S->Data);
//printf("\n");
PrintStorage(S->Next);
}
//printf("NULL");
return;
}

void DeleteMatrixes(Matrix* M){
if(M==NULL){
//printf("\nFREED ALL\n");
return;

}
Matrix* M2=(M)->Next;

long long int i=0;
long long int j=(M)->Row;

for(i=0;i<j;i++){
//if(i!=0){
free(((M)->M)[i]);
((M)->M)[i]=NULL;
//}
}

if(M->M!=NULL)
free(((M)->M));
(M)->M=NULL;


free((M));
(M)=NULL;

DeleteMatrixes(M2);

return;
}

void ToTranspose(Matrix* M){
if(M==NULL){
return ;
}

long double** M1 = malloc((M->Column)*sizeof(long double*));
long long int i=0;

while(i<M->Column){
//long double* M11 = M1[i];
M1[i]=malloc((M->Row)*sizeof(long double));
i++;
}
//
long long int j=0;
for(i=0;i<M->Row;i++){
for(j=0;j<M->Column;j++){

////Reverse
M1[j][i]=(M->M)[i][j];

}
}

freeMatrix(M->M,M->Row);

int R = M->Row;
M->Row=M->Column;
M->Column=R;

M->M=M1;

}

long double** Transpose(Matrix* M){
if(M==NULL){
return NULL;
}
long double** M1 = malloc((M->Column)*sizeof(long double*));
long long int i=0;

while(i<M->Column){
//long double* M11 = M1[i];
M1[i]=malloc((M->Row)*sizeof(long double));
i++;
}

long long int j=0;
for(i=0;i<M->Row;i++){
for(j=0;j<M->Column;j++){

//Reverse
M1[j][i]=(M->M)[i][j];

}
}

//APPEND MATRIX by Column, Row now
AppendMatrix(M1,M->Column,M->Row,&M);

return M->M;

}

void AppendMatrix(long double** D, long long int row,long long int column, Matrix** M){

if(M==NULL){
printf("\nNULL POINTER, MALLOC IT\n");
//M = malloc(sizeof(Matrix*));
return;
}

if(*M==NULL){
(*M)=malloc(sizeof(Matrix));
(*M)->Row=row;
(*M)->Column=column;
(*M)->Next=NULL;
(*M)->M=D;
return;
}

AppendMatrix(D,row,column,&((*M)->Next));
return;
}

/**
 * One Activation Function!
 *
 */
long double** Sigmoid(Matrix*M1){
if(M1==NULL){
printf("\nNO MATRIX GIVEN FOR RELU");
return NULL;
}
long long int i=0;
long double val = 0;
for(i=0;i<M1->Row;i++){
//Compute Sigmoid for each
val = M1->M[i][0];
M1->M[i][0] = 1/(1+expl(-val));
}
return M1->M;
}

/**
 * Derivative of Activated Sigmoids
 */
long double DSig(long double D){
return D*(1-D);
}

/**
 *Relu Activation Function
 */
long double** Relu(Matrix*M1){
if(M1==NULL){
printf("NO MATRIX GIVEN FOR RELU");
return NULL;
}
long long int i=0;

for(i=0;i<M1->Row;i++){
if(M1->M[i][0]<=0){
M1->M[i][0]=0.0;
}	
}

return M1->M;
}

//Derivative of Activated Relu
long double DRelu(long double D){
return D>0? 1:0;
}

/**
 * SoftMax Activation Function
 * e^aj/sum1,...,N(e^ak) for all 1....N
 */
long double** SoftMax(Matrix*M){
long long int i=0;
long long int k=0;
long double sum=0.0;

for(i=0;i<M->Row;i++){
k=0;
sum =0.0;
for(k=0;k<M->Row;k++){
if(i!=k){
//Sum over all values !=i for k
sum+=expl(M->M[k][0]);
}
}
//Perform operation on row
M->M[i][0] = expl(M->M[i][0])/sum;
}

return M->M;
}


