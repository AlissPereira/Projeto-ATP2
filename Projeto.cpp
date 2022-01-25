#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <locale.h>

FILE *abre(char str[100],FILE *imagem);

FILE *limiar(FILE *imagem);

FILE *negativo(FILE *imagem);

FILE *histograma(FILE *imagem);

int salva(int *mat,char ver[100],int w,int h,int cinza,FILE *imagem);

int histograma(int *vet,int cinza);

int evitaComentario(FILE *imagem);

int *criaMatriz(FILE *imagem, int *w,int *h, int *cinza, char ver[100],int *mat);

int main (int argc, char *argv[]){
	
	setlocale (LC_ALL,"");	
	
	int op;
	FILE *imagem;
	char str[100];
	
	printf("Digite o nome do arquivo .pmg que deseja abrir (sem .pgm no final): ");
	fgets(str,100,stdin);

	imagem = abre(str,imagem);
	if(imagem==NULL){
		return 0;
	}
	
	do{
		system("cls");
		
		printf("digite a operação que deseja realizar\n1 - Limiarização\n2 - Mudar imagem para negativo\n3 - Histograma\n4 - Sair\n");
		scanf("%d",&op);
		
		if(op<1||op>4){
			system("cls");
			printf("Operação invalida - aperte qualquer tecla para prosseguir\n");			
			getch();
			system("cls");
		}
		
		if(op==1){
			system("cls");
			limiar(imagem);
		}
		
		else if(op==2){
			system("cls");
			negativo(imagem);
		}
		
		else if(op==3){
			system("cls");
			histograma(imagem);
		}
		
		else{
			system("cls");
			printf("Encerrando programa. Aperte qualquer tecla para sair");
			getch();
		}		
		
	}while(op<1 || op>4);
	
	fclose(imagem);
	return 0;
}

FILE *abre(char str[100],FILE *imagem){

	int i;
	char *filename;

	for(i=0; str[i]; i++){
		if(str[i]=='\n'){
			str[i]=0;	/* Elimina o \n da string*/
		}
	}
	
	filename =(char*) malloc(strlen(str) + strlen(".pgm"));
	
	if(filename){
	  sprintf(filename, "%s.pgm", str);
	  imagem = fopen(filename, "r+");
	}
	
	if(imagem == NULL){
		printf("Não foi possivel abrir o arquivo");
		getchar();
		return NULL;
	}
	else{
		return imagem;
	}
	
	free(filename);
}

int evitaComentario(FILE *imagem){
	char ch[70];
	int ret,aux=0,i=0;
	
	do{
		fscanf(imagem,"%s",&ch);
		
		if(ch[i]=='#'){	
			aux=1;
		}
		else if(aux==1){
			for(i=0;i<strlen(ch);i++){
				if(ch[i] >= '0' && ch[i] <= '9'){				
					aux=0;
					break;
				}
				fscanf(imagem,"%s",&ch);
			}	
		}		
	}while(aux!=0);
	
	ret = atoi(ch);//converte o valor lido pra int		

	return ret;
};

int *criaMatriz(FILE *imagem, int *w,int *h, int *cinza, char ver[100],int *mat){
	int i,j;

	fscanf(imagem,"%s",ver);
	*w = evitaComentario(imagem);//evita possiveis comentarios entre as informações do header
	*h = evitaComentario(imagem);
	*cinza = evitaComentario(imagem);
	
	mat = (int*) malloc(((*w*2)*(*h))*sizeof(int));
	
	for(i=0;i<*w;i++){
		for(j=0;j<*h;j++){
			fscanf(imagem,"%d",&mat[(i*(*w))+j]);
		}
	}
	
	return mat;
}

FILE *limiar(FILE *imagem){
	int w,h,cinza,*mat, i,j,limiar,aux;
	char ver[100],ch[70];
	
	printf("Digite o limiar: ");
	scanf("%d",&limiar);
	
	mat = criaMatriz(imagem,&w,&h,&cinza,ver,mat);

	for(i=0;i<w;i++){
		for(j=0;j<h;j++){
			if(mat[(i*w)+j]>limiar){
				mat[(i*w)+j] = cinza;	
			}
			else{
				mat[(i*w)+j] = 0;	
			}
		}
	}
	aux = salva(mat,ver,w,h,cinza,imagem);
	
	if(aux==1){
		system("cls");
		printf("Limiarização feita com sucesso. Aperte qualquer tecla para continuar.");
		getchar();
	}
	else{
		system("cls");
		printf("falha na limiarização. Aperte qualquer tecla para continuar.");
		getchar();
	}

	free(mat);
}

int salva(int *mat,char ver[100],int w,int h,int cinza,FILE *imagem){
	char *filename;
	char str[100];
	int i,j;
	
	printf("Digite o nome com que deseja salvar essa imagem: ");
	getchar();
	fgets(str,100,stdin);
	
	for(i=0; str[i]; i++){
		if(str[i]=='\n'){
			str[i]=0;	/* Elimina o \n da string*/
		}
	}
	
	filename =(char*) malloc(strlen(str) + strlen(".pgm"));
	
	if(filename){
		sprintf(filename, "%s.pgm", str);
		imagem = fopen(filename, "a+");
	}
	
	if(imagem == NULL){
		printf("Não foi possivel abrir o arquivo");
		getchar();
		return 0;
	}
	
	fprintf(imagem,"%s\n",ver);
	fprintf(imagem,"%d\n",w);
	fprintf(imagem,"%d\n",h);
	fprintf(imagem,"%d\n",cinza);
	
	for(i=0;i<w;i++){
		for(j=0;j<h;j++){
			fprintf(imagem,"%d ",mat[(i*w)+j]);
		}
		fprintf(imagem,"\n");
	}
	
	free(filename);
	return 1;
}

FILE *negativo(FILE *imagem){
	int w,h,cinza,*mat,i,j,result;
	char ver[100];

	mat = criaMatriz(imagem,&w,&h,&cinza,ver,mat);

	for(i=0;i<w;i++){
		for(j=0;j<h;j++){
			mat[(i*w)+j] = cinza - mat[(i*w)+j];
		}
	}

	result = salva(mat,ver,w,h,cinza,imagem);
	
	if(result==1){
		system("cls");
		printf("Imagem negativa feita com sucesso. Aperte qualquer tecla para continuar.");
		getchar();
	}
	else{
		system("cls");
		printf("falha no processo. Aperte qualquer tecla para continuar.");
		getchar();
	}
	
	free(mat);
}

FILE *histograma(FILE *imagem){
	int w,h,cinza,*vet,i,j,result,aux;
	char ver[100];
	
	fscanf(imagem,"%s",ver);
	w = evitaComentario(imagem);//evita possiveis comentarios entre as informações do header
	h = evitaComentario(imagem);
	cinza = evitaComentario(imagem);
	
	vet = (int*) malloc((cinza+1)*sizeof(int));
	
	for(i=0;i<cinza+1;i++){
		vet[i]=0;
	}
	
	for(i=0;i<w;i++){
		for(j=0;j<h;j++){
			fscanf(imagem,"%d",&aux);
			vet[aux] = vet[aux]+1;
		}
	}
	
	result = histograma(vet,cinza);
		
	if(result==1){
		system("cls");
		printf("Histograma feito com sucesso. Aperte qualquer tecla para continuar.");
		getchar();
	}
	else{
		system("cls");
		printf("falha no processo. Aperte qualquer tecla para continuar.");
		getchar();
	}
	
	free(vet);
}

int histograma(int *vet,int cinza){
	char *filename;
	FILE *hist;
	char str[100],aux=178;
	int i,j,op;
	
	printf("Digite o nome com que deseja salvar essa imagem: ");
	getchar();
	fgets(str,100,stdin);
	
	for(i=0; str[i]; i++){
		if(str[i]=='\n'){
			str[i]=0;	/* Elimina o \n da string*/
		}
	}
	
	filename =(char*) malloc(strlen(str) + strlen(".txt"));
	
	if(filename){
		sprintf(filename, "%s.txt", str);
		hist = fopen(filename, "a+");
	}
	
	if(hist == NULL){
		printf("Não foi possivel abrir o arquivo");
		getchar();
		return 0;
	}

	do{
		system("cls");
		printf("Escolha o estilo de Histograma que deseja criar\n\n0 - Apenas os valores em números da contagem de pixels que cada tonalidade de cinza possui\n");
		printf("\n1 - Apresentação em barras da contagem de pixels, com o número de pixels de cada tonalidade no final da barra (pode não funcionar bem em arquivos grandes ou com muitos pixels da mesma cor)\n");	

		scanf("%d",&op);
		if(op<0|| op>1){
			system("cls");
			printf("Opção invalida - aperte qualquer tecla para continuar");
			getchar();
		}
	}while(op<0||op>1);
	
	if(op==1){	
		for(i=0;i<cinza;i++){
			if(vet[i]!=0){
				fprintf(hist,"     |\n");
				fprintf(hist,"%d |",i);
				for(j=0;j<vet[i];j++){
					fprintf(hist,"\u25A0");
				}
				fprintf(hist," %d\n",vet[i]);
			}
		}
	}
	else{
		for(i=0;i<cinza;i++){
			if(vet[i]!=0){
				fprintf(hist,"%d |",i);	
				fprintf(hist," %d\n",vet[i]);
			}
		}
	}
	
	free(filename);
	fclose(hist);
	return 1;
}
