#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include<sys/stat.h>
#include<cuda.h>
#include<cuda_runtime.h>
#include<ctype.h>

__constant__ int const_int[20];
__constant__ double parameter[5730];

char str2int_CPU(char c)
{
        switch (c)
        {
                case 'A':
                        return 0;
                case 'C':
                        return 1;
                case 'G':
                        return 2;              
                case 'T':  
                        return 3;       
        }
        return 4;
}

__device__ void str2int(char c,char *d_numSeq,int id)
{
        switch (c)
        {
                case 'A':
                        d_numSeq[id]=0;
			break;
                case 'C':
                        d_numSeq[id]=1;
			break;
                case 'G':
                        d_numSeq[id]=2;
			break;
                case 'T':
                        d_numSeq[id]=3;
			break;
		default:
		        d_numSeq[id]=4;
			break;
	}
}

__device__ void str2int_rev(char c,char *d_numSeq,int id)
{
        switch (c)
        {
                case 'T':
                        d_numSeq[id]=0;
			break;
                case 'G':
                        d_numSeq[id]=1;
			break;
                case 'C':
                        d_numSeq[id]=2;
			break;                 
                case 'A':               
                        d_numSeq[id]=3;
			break;
		default:
			d_numSeq[id]=4;
			break;
	}
}

void readLoop(FILE *file,double *v1,double *v2,double *v3)
{
        char *line,*p,*q;
        
        line=(char *)malloc(200);
        memset(line,'\0',200);
        fgets(line,200,file);

        p = line;
        while (*p==' '||*p=='\t')
                p++;
        while (*p=='0'||*p=='1'||*p=='2'||*p=='3'||*p=='4'||*p=='5'||*p=='6'||*p=='7'||*p=='8'||*p=='9') 
                p++;
        while (*p==' '||*p=='\t') 
                p++;

        q = p;
        while (!(*q==' '||*q=='\t')) 
                q++;
        *q = '\0';
        q++;
        if (!strcmp(p, "inf"))
                *v1 =1.0*INFINITY;
        else 
                sscanf(p, "%lf", v1);
        while (*q==' '||*q=='\t')
                q++;

        p = q;
        while (!(*p==' '||*p=='\t'))
                p++;
        *p = '\0';
        p++;
        if (!strcmp(q, "inf"))
                *v2 =1.0*INFINITY;
        else 
                sscanf(q, "%lf", v2);
        while (*p==' '||*p=='\t')
                p++;

        q = p;
        while (!(*q==' '||*q=='\t') && (*q != '\0'))
                q++;
        *q = '\0';
        if (!strcmp(p, "inf"))
                *v3 =1.0*INFINITY;
        else 
                sscanf(p, "%lf", v3);
}

void getStack(char *path,double *parameter)
{
        int i, j, ii, jj;
        FILE *sFile, *hFile;
        char *line;

        i=strlen(path)+20;
        line=(char *)malloc(i);
        memset(line,'\0',i);
        strcpy(line,path);
        strcat(line,"stack.ds");
        if(access(line,0)==-1)
        {
                printf("Error! Don't have %s file!\n",line);
                exit(1);
        }
        sFile=fopen(line,"r");
        if(sFile==NULL)
        {
                printf("Error! Can't open the %s file!\n",line);
                exit(1);
        }

        memset(line,'\0',i);
        strcpy(line,path);
        strcat(line,"stack.dh");
        if(access(line,0)==-1)
        {
                printf("Error! Don't have %s file!\n",line);
                exit(1);
        }
        hFile=fopen(line,"r");
        if(hFile==NULL)
        {
                printf("Error! Can't open the %s file!\n",line);
                exit(1);
        }
        free(line);

        line=(char *)malloc(20);
        memset(line,'\0',20);
        for (i = 0; i < 5; ++i)
        {
                for (ii = 0; ii < 5; ++ii)
                {
                        for (j = 0; j < 5; ++j)
                        {
                                for (jj = 0; jj < 5; ++jj)
                                {
                                        if (i == 4 || j == 4 || ii == 4 || jj == 4) //N 
                                        {
                                                parameter[i*125+ii*25+j*5+jj] = -1.0;
                                                parameter[625+i*125+ii*25+j*5+jj]=1.0*INFINITY;
                                        }
                                        else 
                                        {
                                                if(fgets(line,20,sFile)==NULL)
                                                {
                                                        printf("Error! When read parameters in getStack function!\n");
                                                        exit(1);
                                                }
                                                if(strncmp(line, "inf", 3)==0)
                                                        parameter[i*125+ii*25+j*5+jj]=1.0*INFINITY;
                                                else
                                                        parameter[i*125+ii*25+j*5+jj] = atof(line);

                                                if(fgets(line,20,hFile)==NULL)
                                                {
                                                        printf("Error! When read parameters in getStack function!\n");
                                                        exit(1);
                                                }
                                                if(strncmp(line, "inf", 3)==0)
                                                        parameter[625+i*125+ii*25+j*5+jj]=1.0*INFINITY;
                                                else
                                                        parameter[625+i*125+ii*25+j*5+jj] = atof(line);

                                                if (fabs(parameter[i*125+ii*25+j*5+jj])>999999999 ||fabs(parameter[625+i*125+ii*25+j*5+jj])>999999999) 
                                                {
                                                        parameter[i*125+ii*25+j*5+jj] = -1.0;
                                                        parameter[625+i*125+ii*25+j*5+jj] =1.0*INFINITY;
                                                }
                                        }
                                }
                        }
                }
        }
        fclose(sFile);
        fclose(hFile);
        free(line);
}

void getStackint2(char *path,double *parameter)
{
        int i, j, ii, jj;
        FILE *sFile, *hFile;
        char *line;

        i=strlen(path)+20;
        line=(char *)malloc(i);
        memset(line,'\0',i);
        strcpy(line,path);
        strcat(line,"stackmm.ds");
        if(access(line,0)==-1)
        {
                printf("Error! Don't have %s file!\n",line);
                exit(1);
        }
        sFile=fopen(line,"r");
        if(sFile==NULL)
        {
                printf("Error! Can't open the %s file!\n",line);
                exit(1);
        }

        memset(line,'\0',i);
        strcpy(line,path);
        strcat(line,"stackmm.dh");
        if(access(line,0)==-1)
        {
                printf("Error! Don't have %s file!\n",line);
                exit(1);
        }
        hFile=fopen(line,"r");
        if(hFile==NULL)
        {
                printf("Error! Can't open the %s file!\n",line);
                exit(1);
        }
        free(line);

        line=(char *)malloc(20);
        memset(line,'\0',20);
        for (i = 0; i < 5; ++i)
        {
                for (ii = 0; ii < 5; ++ii)
                {
                        for (j = 0; j < 5; ++j)
                        {
                                for (jj = 0; jj < 5; ++jj)
                                {
                                        if (i == 4 || j == 4 || ii == 4 || jj == 4)
                                        {
                                                parameter[1250+i*125+ii*25+j*5+jj] = -1.0;
                                                parameter[1875+i*125+ii*25+j*5+jj] =1.0*INFINITY;
                                        } 
                                        else 
                                        {
                                                if(fgets(line,20,sFile)==NULL)
                                                {
                                                        printf("Error! When read parameters in getStackint2 function!\n");
                                                        exit(1);
                                                }
                                                if(strncmp(line, "inf", 3)==0)
                                                        parameter[1250+i*125+ii*25+j*5+jj]=1.0*INFINITY;
                                                else
                                                        parameter[1250+i*125+ii*25+j*5+jj] = atof(line);

                                                if(fgets(line,20,hFile)==NULL)
                                                {
                                                        printf("Error! When read parameters in getStackint2 function!\n");
                                                        exit(1);
                                                }
                                                if(strncmp(line, "inf", 3)==0)
                                                        parameter[1875+i*125+ii*25+j*5+jj]=1.0*INFINITY;
                                                else
                                                        parameter[1875+i*125+ii*25+j*5+jj] = atof(line);

                                                if(fabs(parameter[1250+i*125+ii*25+j*5+jj])>999999999||fabs(parameter[1875+i*125+ii*25+j*5+jj])>999999999)
                                                {
                                                        parameter[1250+i*125+ii*25+j*5+jj] = -1.0;
                                                        parameter[1875+i*125+ii*25+j*5+jj] =1.0*INFINITY;
                                                }
                                        }
                                }
                        }
                }
        }
        fclose(sFile);
        fclose(hFile);
        free(line);
}

void getDangle(char *path,double *parameter)
{
        int i, j, k;
        FILE *sFile, *hFile;
        char *line;
        
        i=strlen(path)+20;
        line=(char *)malloc(i);
        memset(line,'\0',i);
        strcpy(line,path);
        strcat(line,"dangle.ds");
        if(access(line,0)==-1)
        {
                printf("Error! Don't have %s file!\n",line);
                exit(1);
        }
        sFile=fopen(line,"r");
        if(sFile==NULL)
        {
                printf("Error! Can't open the %s file!\n",line);
                exit(1);
        }

        memset(line,'\0',i);
        strcpy(line,path);
        strcat(line,"dangle.dh");
        if(access(line,0)==-1)
        {
                printf("Error! Don't have %s file!\n",line);
                exit(1);
        }
        hFile=fopen(line,"r");
        if(hFile==NULL)
        {
                printf("Error! Can't open the %s file!\n",line);
                exit(1);
        }
        free(line);

        line=(char *)malloc(20);
        memset(line,'\0',20);
        for (i = 0; i < 5; ++i)
                for (j = 0; j < 5; ++j)
                        for (k = 0; k < 5; ++k) 
                        {
                                if (i == 4 || j == 4) 
                                {
                                        parameter[2500+i*25+k*5+j] = -1.0;
                                        parameter[2625+i*25+k*5+j] =1.0*INFINITY;
                                }
                                else if (k == 4)
                                {
                                        parameter[2500+i*25+k*5+j] = -1.0;
                                        parameter[2625+i*25+k*5+j] =1.0*INFINITY;
                                } 
                                else
                                {
                                        if(fgets(line,20,sFile)==NULL)
                                        {
                                                printf("Error! When read parameters in getDangle function!\n");
                                                exit(1);
                                        }
                                        if(strncmp(line, "inf", 3)==0)
                                                parameter[2500+i*25+k*5+j]=1.0*INFINITY;
                                        else
                                                parameter[2500+i*25+k*5+j]=atof(line);

                                        if(fgets(line,20,hFile)==NULL)
                                        {
                                                printf("Error! When read parameters in getDangle function!\n");        
                                                exit(1);        
                                        }
                                        if(strncmp(line, "inf", 3)==0)        
                                                parameter[2625+i*25+k*5+j]=1.0*INFINITY;           
                                        else        
                                                parameter[2625+i*25+k*5+j]=atof(line);

                                        if(fabs(parameter[2500+i*25+k*5+j])>999999999||fabs(parameter[2625+i*25+k*5+j])>999999999) 
                                        {
                                                parameter[2500+i*25+k*5+j] = -1.0;
                                                parameter[2625+i*25+k*5+j] =1.0*INFINITY;
                                        }
                                }
                        }

        for (i = 0; i < 5; ++i)
                for (j = 0; j < 5; ++j)
                        for (k = 0; k < 5; ++k) 
                        {
                                if (i == 4 || j == 4)
                                {
                                        parameter[2750+i*25+j*5+k] = -1.0;
                                        parameter[2875+i*25+j*5+k] =1.0*INFINITY;
                                } 
                                else if (k == 4) 
                                {
                                        parameter[2750+i*25+j*5+k] = -1.0;
                                        parameter[2875+i*25+j*5+k] =1.0*INFINITY;
                                }
                                else
                                {
                                        if(fgets(line,20,sFile)==NULL)
                                        {
                                                printf("Error! When read parameters in getDangle function!\n");
                                                exit(1);
                                        }
                                        if(strncmp(line, "inf", 3)==0)
                                                parameter[2750+i*25+j*5+k]=1.0*INFINITY;
                                        else
                                                parameter[2750+i*25+j*5+k]=atof(line);

                                        if(fgets(line,20,hFile)==NULL)
                                        {
                                                printf("Error! When read parameters in getDangle function!\n");        
                                                exit(1);        
                                        }
                                        if(strncmp(line, "inf", 3)==0)        
                                                parameter[2875+i*25+j*5+k]=1.0*INFINITY;           
                                        else        
                                                parameter[2875+i*25+j*5+k]=atof(line);

                                        if(fabs(parameter[2750+i*25+j*5+k])>999999999||fabs(parameter[2875+i*25+j*5+k])>999999999)
                                        {
                                                parameter[2750+i*25+j*5+k] = -1.0;
                                                parameter[2875+i*25+j*5+k] =1.0*INFINITY;
                                        }
                                }
                        }
        fclose(sFile);
        fclose(hFile);
        free(line);
}

void getLoop(char *path,double *parameter)
{
        int k;
        FILE *sFile, *hFile;
        char *line;

        k=strlen(path)+20;
        line=(char *)malloc(k);
        memset(line,'\0',k);
        strcpy(line,path);
        strcat(line,"loops.ds");
        if(access(line,0)==-1)
        {
                printf("Error! Don't have %s file!\n",line);
                exit(1);
        }
        sFile=fopen(line,"r");
        if(sFile==NULL)
        {
                printf("Error! Can't open the %s file!\n",line);
                exit(1);
        }

        memset(line,'\0',k);
        strcpy(line,path);
        strcat(line,"loops.dh");
        if(access(line,0)==-1)
        {
                printf("Error! Don't have %s file!\n",line);
                exit(1);
        }
        hFile=fopen(line,"r");
        if(hFile==NULL)
        {
                printf("Error! Can't open the %s file!\n",line);
                exit(1);
        }
        free(line);

        for (k = 0; k < 30; ++k)
        {
                readLoop(sFile, &parameter[3030+k], &parameter[3060+k], &parameter[3000+k]);
                readLoop(hFile, &parameter[3120+k], &parameter[3150+k], &parameter[3090+k]);
        }
        fclose(sFile);
        fclose(hFile);
}

void getTstack(char *path,double *parameter)
{
        int i1, j1, i2, j2;
        FILE *sFile, *hFile;
        char *line;

        i1=strlen(path)+20;
        line=(char *)malloc(i1);
        memset(line,'\0',i1);
        strcpy(line,path);
        strcat(line,"tstack_tm_inf.ds");
        if(access(line,0)==-1)
        {
                printf("Error! Don't have %s file!\n",line);
                exit(1);
        }
        sFile=fopen(line,"r");
        if(sFile==NULL)
        {
                printf("Error! Can't open the %s file!\n",line);
                exit(1);
        }

        memset(line,'\0',i1);
        strcpy(line,path);      
        strcat(line,"tstack.dh");
        if(access(line,0)==-1)
        {
                printf("Error! Don't have %s file!\n",line);
                exit(1);
        }             
        hFile=fopen(line,"r");
        if(sFile==NULL)
        {
                printf("Error! Can't open the %s file!\n",line);
                exit(1);   
        }
        free(line);

        line=(char *)malloc(20);
        memset(line,'\0',20);
        for (i1 = 0; i1 < 5; ++i1)
                for (i2 = 0; i2 < 5; ++i2)
                        for (j1 = 0; j1 < 5; ++j1)
                                for (j2 = 0; j2 < 5; ++j2)
                                        if (i1 == 4 || j1 == 4)
                                        {
                                                parameter[3805+i1*125+i2*25+j1*5+j2]=1.0*INFINITY;
                                                parameter[3180+i1*125+i2*25+j1*5+j2] = -1.0;
                                        }
                                        else if (i2 == 4 || j2 == 4)
                                        {
                                                parameter[3180+i1*125+i2*25+j1*5+j2] = 0.00000000001;
                                                parameter[3805+i1*125+i2*25+j1*5+j2] = 0.0;
                                        }
                                        else
                                        {
                                                if(fgets(line,20,sFile)==NULL)
                                                {
                                                        printf("Error! When read parameters in getTstack function!\n");
                                                        exit(1);
                                                }
                                                if(strncmp(line, "inf", 3)==0)
                                                        parameter[3180+i1*125+i2*25+j1*5+j2]=1.0*INFINITY;
                                                else
                                                        parameter[3180+i1*125+i2*25+j1*5+j2]=atof(line);

                                                if(fgets(line,20,hFile)==NULL)
                                                {
                                                        printf("Error! When read parameters in getTstack function!\n");
                                                        exit(1);
                                                }
                                                if(strncmp(line, "inf", 3)==0)
                                                        parameter[3805+i1*125+i2*25+j1*5+j2]=1.0*INFINITY;
                                                else
                                                        parameter[3805+i1*125+i2*25+j1*5+j2]=atof(line);

                                                if (fabs(parameter[3180+i1*125+i2*25+j1*5+j2])>999999999||fabs(parameter[3805+i1*125+i2*25+j1*5+j2])>999999999)
                                                {
                                                        parameter[3180+i1*125+i2*25+j1*5+j2] = -1.0;
                                                        parameter[3805+i1*125+i2*25+j1*5+j2] =1.0*INFINITY;
                                                }
                                        }
        fclose(sFile);
        fclose(hFile);
        free(line);
}

void getTstack2(char *path,double *parameter)
{
        int i1, j1, i2, j2;
        FILE *sFile, *hFile;
        char *line;

        i1=strlen(path)+20;
        line=(char *)malloc(i1);
        memset(line,'\0',i1);
        strcpy(line,path);
        strcat(line,"tstack2.ds");
        if(access(line,0)==-1)
        {
                printf("Error! Don't have %s file!\n",line);
                exit(1);
        }
        sFile=fopen(line,"r");
        if(sFile==NULL)
        {
                printf("Error! Can't open the %s file!\n",line);
                exit(1);
        }

        memset(line,'\0',i1);
        strcpy(line,path);      
        strcat(line,"tstack2.dh");
        if(access(line,0)==-1)
        {
                printf("Error! Don't have %s file!\n",line);
                exit(1);
        }             
        hFile=fopen(line,"r");
        if(sFile==NULL)
        {
                printf("Error! Can't open the %s file!\n",line);
                exit(1);   
        }
        free(line);

        line=(char *)malloc(20);
        memset(line,'\0',20);
        for (i1 = 0; i1 < 5; ++i1)
                for (i2 = 0; i2 < 5; ++i2)
                        for (j1 = 0; j1 < 5; ++j1)
                                for (j2 = 0; j2 < 5; ++j2)
                                        if (i1 == 4 || j1 == 4)
                                        {
                                                parameter[5055+i1*125+i2*25+j1*5+j2] =1.0*INFINITY;
                                                parameter[4430+i1*125+i2*25+j1*5+j2] = -1.0;
                                        }
                                        else if (i2 == 4 || j2 == 4)
                                        {
                                                parameter[4430+i1*125+i2*25+j1*5+j2] = 0.00000000001;
                                                parameter[5055+i1*125+i2*25+j1*5+j2] = 0.0;
                                        }
                                        else
                                        {
                                                if(fgets(line,20,sFile)==NULL)
                                                {
                                                        printf("Error! When read parameters in getTstack2 function!\n");
                                                        exit(1);
                                                }
                                                if(strncmp(line, "inf", 3)==0)
                                                        parameter[4430+i1*125+i2*25+j1*5+j2]=1.0*INFINITY;
                                                else
                                                        parameter[4430+i1*125+i2*25+j1*5+j2]=atof(line);

                                                if(fgets(line,20,hFile)==NULL)
                                                {
                                                        printf("Error! When read parameters in getTstack2 function!\n");
                                                        exit(1);
                                                }
                                                if(strncmp(line, "inf", 3)==0)
                                                        parameter[5055+i1*125+i2*25+j1*5+j2]=1.0*INFINITY;
                                                else
                                                        parameter[5055+i1*125+i2*25+j1*5+j2]=atof(line);


                                                if (fabs(parameter[4430+i1*125+i2*25+j1*5+j2])>999999999||fabs(parameter[5055+i1*125+i2*25+j1*5+j2])>999999999)
                                                {
                                                        parameter[4430+i1*125+i2*25+j1*5+j2] = -1.0;
                                                        parameter[5055+i1*125+i2*25+j1*5+j2] =1.0*INFINITY;
                                                }
                                        }
        fclose(sFile);
        fclose(hFile);
        free(line);
}

void tableStartATS(double atp_value,double parameter[] )
{
        int i, j;

        for (i = 0; i < 5; ++i)
                for (j = 0; j < 5; ++j)
                        parameter[5680+i*5+j] = 0.00000000001;
        parameter[5680+3] = parameter[5680+15] = atp_value;
}

void tableStartATH(double atp_value,double parameter[])
{
        int i, j;

        for (i = 0; i < 5; ++i)
                for (j = 0; j < 5; ++j)
                        parameter[5705+i*5+j] = 0.0;
        parameter[5705+3] = parameter[5705+15] = atp_value;
}

//end read parameter
__device__ void Ss(int i,int j,int k,int *d_ps,char *d_numSeq,int id,double parameter[],double *d_DPT)
{
	if(k==2)
	{
		if(i>=j)
		{
			d_DPT[id*1264+1263]=-1.0;
			return;
		}
		if(i==d_ps[id*81+50]||j==d_ps[id*81+51]+1)
		{
			d_DPT[id*1264+1263]=-1.0;
			return;
		}

		if(i>d_ps[id*81+50])
			i-=d_ps[id*81+50];
		if(j>d_ps[id*81+51])
			j-=d_ps[id*81+51];
		d_DPT[id*1264+1263]=parameter[d_numSeq[id*54+i]*125+d_numSeq[id*54+i+1]*25+d_numSeq[id*54+27+j]*5+d_numSeq[id*54+27+j-1]];
	}
	else
		d_DPT[id*1264+1263]=parameter[d_numSeq[id*54+i]*125+d_numSeq[id*54+i+1]*25+d_numSeq[id*54+27+j]*5+d_numSeq[id*54+27+j+1]];
}

__device__ void Hs(int i,int j,int k,int *d_ps,char *d_numSeq,int id,double parameter[],double *d_DPT)
{
	if(k==2)
	{
		if(i>= j)
		{
			d_DPT[id*1264+1263]=1.0*INFINITY;
			return;
		}
		if(i==d_ps[id*81+50]||j==d_ps[id*81+51]+1)
		{
			d_DPT[id*1264+1263]=1.0*INFINITY;
			return;
		}

		if(i>d_ps[id*81+50])
			i-=d_ps[id*81+50];
		if(j>d_ps[id*81+51])
			j-=d_ps[id*81+51];
		if(fabs(parameter[625+d_numSeq[id*54+i]*125+d_numSeq[id*54+i+1]*25+d_numSeq[id*54+27+j]*5+d_numSeq[id*54+27+j-1]])<999999999)
			d_DPT[id*1264+1263]=parameter[625+d_numSeq[id*54+i]*125+d_numSeq[id*54+i+1]*25+d_numSeq[id*54+27+j]*5+d_numSeq[id*54+27+j-1]];
		else
			d_DPT[id*1264+1263]=1.0*INFINITY;
	}
	else

		d_DPT[id*1264+1263]=parameter[625+d_numSeq[id*54+i]*125+d_numSeq[id*54+i+1]*25+d_numSeq[id*54+27+j]*5+d_numSeq[id*54+27+j+1]];
}

__device__ void equal(double a,double b,int *d_ps,int id)
{
	if(fabs(a)>999999999||fabs(b)>999999999)
		d_ps[id*81+62]=0;
	else
	{
		if(fabs(a-b)<1e-5)
			d_ps[id*81+62]=1;
		else
			d_ps[id*81+62]=0;
	}
}

__device__ void initMatrix(int *d_ps,double *d_DPT,int id,char *d_numSeq)
{
	for(d_ps[id*81+54]=1;d_ps[id*81+54]<=d_ps[id*81+50];++d_ps[id*81+54])
	{
		for(d_ps[id*81+55]=1;d_ps[id*81+55]<=d_ps[id*81+51];++d_ps[id*81+55])
		{
			if(d_numSeq[id*54+d_ps[id*81+54]]+d_numSeq[id*54+27+d_ps[id*81+55]]!=3)
			{
				d_DPT[id*1264+(d_ps[id*81+54]-1)*d_ps[id*81+51]+d_ps[id*81+55]-1]=1.0*INFINITY;
				d_DPT[id*1264+625+(d_ps[id*81+54]-1)*d_ps[id*81+51]+d_ps[id*81+55]-1]=-1.0;
			}
			else
			{
				d_DPT[id*1264+(d_ps[id*81+54]-1)*d_ps[id*81+51]+d_ps[id*81+55]-1]=0.0;
				d_DPT[id*1264+625+(d_ps[id*81+54]-1)*d_ps[id*81+51]+d_ps[id*81+55]-1]=-3224.0;
			}
		}
	}
}

__device__ void LSH(int i,int j,int *d_ps,double *d_DPT,int id,char *d_numSeq,double parameter[])
{
	if(d_numSeq[id*54+i]+d_numSeq[id*54+27+j]!=3)
	{
		d_DPT[id*1264+625+(i-1)*d_ps[id*81+51]+j-1]=-1.0;
		d_DPT[id*1264+(i-1)*d_ps[id*81+51]+j-1]=1.0*INFINITY;
		return;
	}

	d_DPT[id*1264+1257]=parameter[5680+d_numSeq[id*54+i]*5+d_numSeq[id*54+27+j]]+parameter[4430+d_numSeq[id*54+27+j]*125+d_numSeq[id*54+27+j-1]*25+d_numSeq[id*54+i]*5+d_numSeq[id*54+i-1]];
	d_DPT[id*1264+1258]=parameter[5705+d_numSeq[id*54+i]*5+d_numSeq[id*54+27+j]]+parameter[5055+d_numSeq[id*54+27+j]*125+d_numSeq[id*54+27+j-1]*25+d_numSeq[id*54+i]*5+d_numSeq[id*54+i-1]];
	if(fabs(d_DPT[id*1264+1258])>999999999)
	{
		d_DPT[id*1264+1258]=1.0*INFINITY;
		d_DPT[id*1264+1257]=-1.0;
	}
// If there is two dangling ends at the same end of duplex
	if(fabs(parameter[2625+d_numSeq[id*54+27+j]*25+d_numSeq[id*54+27+j-1]*5+d_numSeq[id*54+i]])<999999999&&fabs(parameter[2875+d_numSeq[id*54+27+j]*25+d_numSeq[id*54+i]*5+d_numSeq[id*54+i-1]])<999999999)
	{
		d_DPT[id*1264+1260]=parameter[5680+d_numSeq[id*54+i]*5+d_numSeq[id*54+27+j]]+parameter[2500+d_numSeq[id*54+27+j]*25+d_numSeq[id*54+27+j-1]*5+d_numSeq[id*54+i]]+parameter[2750+d_numSeq[id*54+27+j]*25+d_numSeq[id*54+i]*5+d_numSeq[id*54+i-1]];
		d_DPT[id*1264+1261]=parameter[5705+d_numSeq[id*54+i]*5+d_numSeq[id*54+27+j]]+parameter[2625+d_numSeq[id*54+27+j]*25+d_numSeq[id*54+27+j-1]*5+d_numSeq[id*54+i]]+parameter[2875+d_numSeq[id*54+27+j]*25+d_numSeq[id*54+i]*5+d_numSeq[id*54+i-1]];
		if(fabs(d_DPT[id*1264+1261])>999999999)
		{
			d_DPT[id*1264+1261]=1.0*INFINITY;
			d_DPT[id*1264+1260]=-1.0;
		}
		d_DPT[id*1264+1262]=(d_DPT[id*1264+1261]+200)/(d_DPT[id*1264+1260]-5.7+d_DPT[id*1264+1252]);
		if(fabs(d_DPT[id*1264+1258])<999999999)
		{
			d_DPT[id*1264+1259]=(d_DPT[id*1264+1258]+200)/(d_DPT[id*1264+1257]-5.7+d_DPT[id*1264+1252]);
			if(d_DPT[id*1264+1259]<d_DPT[id*1264+1262])
			{
				d_DPT[id*1264+1257]=d_DPT[id*1264+1260];
				d_DPT[id*1264+1258]=d_DPT[id*1264+1261];
				d_DPT[id*1264+1259]=d_DPT[id*1264+1262];
			}
		}
		else
		{
			d_DPT[id*1264+1257]=d_DPT[id*1264+1260];
			d_DPT[id*1264+1258]=d_DPT[id*1264+1261];
			d_DPT[id*1264+1259]=d_DPT[id*1264+1262];
		}
	}
	else if(fabs(parameter[2625+d_numSeq[id*54+27+j]*25+d_numSeq[id*54+27+j-1]*5+d_numSeq[id*54+i]])<999999999)
	{
		d_DPT[id*1264+1260]=parameter[5680+d_numSeq[id*54+i]*5+d_numSeq[id*54+27+j]]+parameter[2500+d_numSeq[id*54+27+j]*25+d_numSeq[id*54+27+j-1]*5+d_numSeq[id*54+i]];
		d_DPT[id*1264+1261]=parameter[5705+d_numSeq[id*54+i]*5+d_numSeq[id*54+27+j]]+parameter[2625+d_numSeq[id*54+27+j]*25+d_numSeq[id*54+27+j-1]*5+d_numSeq[id*54+i]];
		if(fabs(d_DPT[id*1264+1261])>999999999)
		{
			d_DPT[id*1264+1261]=1.0*INFINITY;
			d_DPT[id*1264+1260]=-1.0;
		}
		d_DPT[id*1264+1262]=(d_DPT[id*1264+1261]+200)/(d_DPT[id*1264+1260]-5.7+d_DPT[id*1264+1252]);
		if(fabs(d_DPT[id*1264+1258])<999999999)
		{
			d_DPT[id*1264+1259]=(d_DPT[id*1264+1258]+200)/(d_DPT[id*1264+1257]-5.7+d_DPT[id*1264+1252]);
			if(d_DPT[id*1264+1259]<d_DPT[id*1264+1262])
			{
				d_DPT[id*1264+1257]=d_DPT[id*1264+1260];
				d_DPT[id*1264+1258]=d_DPT[id*1264+1261];
				d_DPT[id*1264+1259]=d_DPT[id*1264+1262];
			}
		}
		else
		{
			d_DPT[id*1264+1257]=d_DPT[id*1264+1260];
			d_DPT[id*1264+1258]=d_DPT[id*1264+1261];
			d_DPT[id*1264+1259]=d_DPT[id*1264+1262];
		}
	}
	else if(fabs(parameter[2875+d_numSeq[id*54+27+j]*25+d_numSeq[id*54+i]*5+d_numSeq[id*54+i-1]])<999999999)
	{
		d_DPT[id*1264+1260]=parameter[5680+d_numSeq[id*54+i]*5+d_numSeq[id*54+27+j]]+parameter[2750+d_numSeq[id*54+27+j]*25+d_numSeq[id*54+i]*5+d_numSeq[id*54+i-1]];
		d_DPT[id*1264+1261]=parameter[5705+d_numSeq[id*54+i]*5+d_numSeq[id*54+27+j]]+parameter[2875+d_numSeq[id*54+27+j]*25+d_numSeq[id*54+i]*5+d_numSeq[id*54+i-1]];
		if(fabs(d_DPT[id*1264+1261])>999999999)
		{
			d_DPT[id*1264+1261]=1.0*INFINITY;
			d_DPT[id*1264+1260]=-1.0;
		}
		d_DPT[id*1264+1262]=(d_DPT[id*1264+1261]+200)/(d_DPT[id*1264+1260]-5.7+d_DPT[id*1264+1252]);
		if(fabs(d_DPT[id*1264+1258])<999999999)
		{
			d_DPT[id*1264+1259]=(d_DPT[id*1264+1258]+200)/(d_DPT[id*1264+1257]-5.7+d_DPT[id*1264+1252]);
			if(d_DPT[id*1264+1259]<d_DPT[id*1264+1262])
			{
				d_DPT[id*1264+1257]=d_DPT[id*1264+1260];
				d_DPT[id*1264+1258]=d_DPT[id*1264+1261];
				d_DPT[id*1264+1259]=d_DPT[id*1264+1262];
			}
		}
		else
		{
			d_DPT[id*1264+1257]=d_DPT[id*1264+1260];
			d_DPT[id*1264+1258]=d_DPT[id*1264+1261];
			d_DPT[id*1264+1259]=d_DPT[id*1264+1262];
		}
	}

	d_DPT[id*1264+1260]=parameter[5680+d_numSeq[id*54+i]*5+d_numSeq[id*54+27+j]];
	d_DPT[id*1264+1261]=parameter[5705+d_numSeq[id*54+i]*5+d_numSeq[id*54+27+j]];
	d_DPT[id*1264+1262]=(d_DPT[id*1264+1261]+200)/(d_DPT[id*1264+1260]-5.7+d_DPT[id*1264+1252]);
	if(fabs(d_DPT[id*1264+1258])<999999999)
	{
		if(d_DPT[id*1264+1259]<d_DPT[id*1264+1262])
		{
			d_DPT[id*1264+1255]=d_DPT[id*1264+1260];
			d_DPT[id*1264+1256]=d_DPT[id*1264+1261];
		}
		else
		{
			d_DPT[id*1264+1255]=d_DPT[id*1264+1257];
			d_DPT[id*1264+1256]=d_DPT[id*1264+1258];
		}
	}
	else
	{
		d_DPT[id*1264+1255]=d_DPT[id*1264+1260];
		d_DPT[id*1264+1256]=d_DPT[id*1264+1261];
	}
	return;
}

__device__ void maxTM(int i,int j,int *d_ps,double *d_DPT,int id,char *d_numSeq,double parameter[])
{
	d_DPT[id*1264+1259]=d_DPT[id*1264+625+(i-1)*d_ps[id*81+51]+j-1];
	d_DPT[id*1264+1261]=d_DPT[id*1264+(i-1)*d_ps[id*81+51]+j-1];
	d_DPT[id*1264+1257]=(d_DPT[id*1264+1261]+200)/(d_DPT[id*1264+1259]-5.7+d_DPT[id*1264+1252]); // at current position 
	if(fabs(d_DPT[id*1264+(i-2)*d_ps[id*81+51]+j-2])<999999999)
	{
		Hs(i-1,j-1,1,d_ps,d_numSeq,id,parameter,d_DPT);
		if(fabs(d_DPT[id*1264+1263])<999999999)
		{
			d_DPT[id*1264+1262]=(d_DPT[id*1264+(i-2)*d_ps[id*81+51]+j-2]+d_DPT[id*1264+1263]);
			Ss(i-1,j-1,1,d_ps,d_numSeq,id,parameter,d_DPT);
			d_DPT[id*1264+1260]=(d_DPT[id*1264+625+(i-2)*d_ps[id*81+51]+j-2]+d_DPT[id*1264+1263]);
		}
	}
	else
	{
		d_DPT[id*1264+1260]=-1.0;
		d_DPT[id*1264+1262]=1.0*INFINITY;
	}
	d_DPT[id*1264+1258]=(d_DPT[id*1264+1262]+200)/(d_DPT[id*1264+1260]-5.7+d_DPT[id*1264+1252]);

	if(d_DPT[id*1264+1260]<-2500.0)
	{
// to not give dH any value if dS is unreasonable
		d_DPT[id*1264+1260]=-3224.0;
		d_DPT[id*1264+1262]=0.0;
	}
	if(d_DPT[id*1264+1259]<-2500.0)
	{
// to not give dH any value if dS is unreasonable
		d_DPT[id*1264+1259]=-3224.0;
		d_DPT[id*1264+1261]=0.0;
	}
	if((d_DPT[id*1264+1258]>d_DPT[id*1264+1257])||(d_DPT[id*1264+1259]>0&&d_DPT[id*1264+1261]>0)) // T1 on suurem 
	{
		d_DPT[id*1264+625+(i-1)*d_ps[id*81+51]+j-1]=d_DPT[id*1264+1260];
		d_DPT[id*1264+(i-1)*d_ps[id*81+51]+j-1]=d_DPT[id*1264+1262];
	}
	else if(d_DPT[id*1264+1257]>=d_DPT[id*1264+1258])
	{
		d_DPT[id*1264+625+(i-1)*d_ps[id*81+51]+j-1]=d_DPT[id*1264+1259];
		d_DPT[id*1264+(i-1)*d_ps[id*81+51]+j-1]=d_DPT[id*1264+1261];
	}
}

__device__ void calc_bulge_internal(int i,int j,int ii,int jj,int traceback,int *d_ps,double *d_DPT,int id,char *d_numSeq,double parameter[])
{
	d_DPT[id*1264+1259]=-3224.0;
	d_DPT[id*1264+1260]=0;
	d_ps[id*81+59]=ii-i-1;
	d_ps[id*81+60]=jj-j-1;
	d_ps[id*81+61]=d_ps[id*81+59]+d_ps[id*81+60]-1;
	if((d_ps[id*81+59]==0&&d_ps[id*81+60]>0)||(d_ps[id*81+60]==0&&d_ps[id*81+59]>0))// only bulges have to be considered
	{
		if(d_ps[id*81+60]==1||d_ps[id*81+59]==1) // bulge loop of size one is treated differently the intervening nn-pair must be added
		{
			if((d_ps[id*81+60]==1&&d_ps[id*81+59]==0)||(d_ps[id*81+60]==0&&d_ps[id*81+59]==1))
			{
				d_DPT[id*1264+1260]=parameter[3150+d_ps[id*81+61]]+parameter[625+d_numSeq[id*54+i]*125+d_numSeq[id*54+ii]*25+d_numSeq[id*54+27+j]*5+d_numSeq[id*54+27+jj]];
				d_DPT[id*1264+1259]=parameter[3060+d_ps[id*81+61]]+parameter[d_numSeq[id*54+i]*125+d_numSeq[id*54+ii]*25+d_numSeq[id*54+27+j]*5+d_numSeq[id*54+27+jj]];
			}
			d_DPT[id*1264+1260]+=d_DPT[id*1264+(i-1)*d_ps[id*81+51]+j-1];
			d_DPT[id*1264+1259]+=d_DPT[id*1264+625+(i-1)*d_ps[id*81+51]+j-1];
			if(fabs(d_DPT[id*1264+1260])>999999999)
			{
				d_DPT[id*1264+1260]=1.0*INFINITY;
				d_DPT[id*1264+1259]=-1.0;
			}

			d_DPT[id*1264+1257]=(d_DPT[id*1264+1260]+200)/((d_DPT[id*1264+1259]-5.7)+d_DPT[id*1264+1252]);
			d_DPT[id*1264+1258]=(d_DPT[id*1264+(ii-1)*d_ps[id*81+51]+jj-1]+200)/((d_DPT[id*1264+625+(ii-1)*d_ps[id*81+51]+jj-1])-5.7+d_DPT[id*1264+1252]);
			if((d_DPT[id*1264+1257]>d_DPT[id*1264+1258])||((traceback&&d_DPT[id*1264+1257]>=d_DPT[id*1264+1258])||(traceback==1)))
			{
				d_DPT[id*1264+1255]=d_DPT[id*1264+1259];
				d_DPT[id*1264+1256]=d_DPT[id*1264+1260];
			}
		}
		else // we have _not_ implemented Jacobson-Stockaymayer equation; the maximum bulgeloop size is 30
		{
			d_DPT[id*1264+1260]=parameter[3150+d_ps[id*81+61]]+parameter[5705+d_numSeq[id*54+i]*5+d_numSeq[id*54+27+j]]+parameter[5705+d_numSeq[id*54+ii]*5+d_numSeq[id*54+27+jj]];
			d_DPT[id*1264+1260]+=d_DPT[id*1264+(i-1)*d_ps[id*81+51]+j-1];

			d_DPT[id*1264+1259]=parameter[3060+d_ps[id*81+61]]+parameter[5680+d_numSeq[id*54+i]*5+d_numSeq[id*54+27+j]]+parameter[5680+d_numSeq[id*54+ii]*5+d_numSeq[id*54+27+jj]];
			d_DPT[id*1264+1259]+=d_DPT[id*1264+625+(i-1)*d_ps[id*81+51]+j-1];
			if(fabs(d_DPT[id*1264+1260])>999999999)
			{
				d_DPT[id*1264+1260]=1.0*INFINITY;
				d_DPT[id*1264+1259]=-1.0;
			}
			d_DPT[id*1264+1257]=(d_DPT[id*1264+1260]+200)/((d_DPT[id*1264+1259]-5.7)+d_DPT[id*1264+1252]);
			d_DPT[id*1264+1258]=(d_DPT[id*1264+(ii-1)*d_ps[id*81+51]+jj-1]+200)/(d_DPT[id*1264+625+(ii-1)*d_ps[id*81+51]+jj-1]-5.7+d_DPT[id*1264+1252]);
			if((d_DPT[id*1264+1257]>d_DPT[id*1264+1258])||((traceback&&d_DPT[id*1264+1257]>=d_DPT[id*1264+1258])||(traceback==1)))
			{
				d_DPT[id*1264+1255]=d_DPT[id*1264+1259];
				d_DPT[id*1264+1256]=d_DPT[id*1264+1260];
			}
		}
	}
	else if(d_ps[id*81+59]==1&&d_ps[id*81+60]==1)
	{
		d_DPT[id*1264+1259]=parameter[1250+d_numSeq[id*54+i]*125+d_numSeq[id*54+i+1]*25+d_numSeq[id*54+27+j]*5+d_numSeq[id*54+27+j+1]]+parameter[1250+d_numSeq[id*54+27+jj]*125+d_numSeq[id*54+27+jj-1]*25+d_numSeq[id*54+ii]*5+d_numSeq[id*54+ii-1]];
		d_DPT[id*1264+1259]+=d_DPT[id*1264+625+(i-1)*d_ps[id*81+51]+j-1];

		d_DPT[id*1264+1260]=parameter[1875+d_numSeq[id*54+i]*125+d_numSeq[id*54+i+1]*25+d_numSeq[id*54+27+j]*5+d_numSeq[id*54+27+j+1]]+parameter[1875+d_numSeq[id*54+27+jj]*125+d_numSeq[id*54+27+jj-1]*25+d_numSeq[id*54+ii]*5+d_numSeq[id*54+ii-1]];
		d_DPT[id*1264+1260]+=d_DPT[id*1264+(i-1)*d_ps[id*81+51]+j-1];
		if(fabs(d_DPT[id*1264+1260])>999999999)
		{
			d_DPT[id*1264+1260]=1.0*INFINITY;
			d_DPT[id*1264+1259]=-1.0;
		}
		d_DPT[id*1264+1257]=(d_DPT[id*1264+1260]+200)/((d_DPT[id*1264+1259]-5.7)+d_DPT[id*1264+1252]);
		d_DPT[id*1264+1258]=(d_DPT[id*1264+(ii-1)*d_ps[id*81+51]+jj-1]+200)/(d_DPT[id*1264+625+(ii-1)*d_ps[id*81+51]+jj-1]-5.7+d_DPT[id*1264+1252]);
		if((d_DPT[id*1264+1257]-d_DPT[id*1264+1258]>=0.000001)||traceback==1)
		{
			if((d_DPT[id*1264+1257]>d_DPT[id*1264+1258])||(traceback&&d_DPT[id*1264+1257]>=d_DPT[id*1264+1258]))
			{
				d_DPT[id*1264+1255]=d_DPT[id*1264+1259];
				d_DPT[id*1264+1256]=d_DPT[id*1264+1260];
			}
		}
		return;
	}
	else // only internal loops
	{
		d_DPT[id*1264+1260]=parameter[3120+d_ps[id*81+61]]+parameter[3805+d_numSeq[id*54+i]*125+d_numSeq[id*54+i+1]*25+d_numSeq[id*54+27+j]*5+d_numSeq[id*54+27+j+1]]+parameter[3805+d_numSeq[id*54+27+jj]*125+d_numSeq[id*54+27+jj-1]*25+d_numSeq[id*54+ii]*5+d_numSeq[id*54+ii-1]];
		d_DPT[id*1264+1260]+=d_DPT[id*1264+(i-1)*d_ps[id*81+51]+j-1];

		d_DPT[id*1264+1259]=parameter[3030+d_ps[id*81+61]]+parameter[3180+d_numSeq[id*54+i]*125+d_numSeq[id*54+i+1]*25+d_numSeq[id*54+27+j]*5+d_numSeq[id*54+27+j+1]]+parameter[3180+d_numSeq[id*54+27+jj]*125+d_numSeq[id*54+27+jj-1]*25+d_numSeq[id*54+ii]*5+d_numSeq[id*54+ii-1]]+(-300/310.15*abs(d_ps[id*81+59]-d_ps[id*81+60]));
		d_DPT[id*1264+1259]+=d_DPT[id*1264+625+(i-1)*d_ps[id*81+51]+j-1];
		if(fabs(d_DPT[id*1264+1260])>999999999)
		{
			d_DPT[id*1264+1260]=1.0*INFINITY;
			d_DPT[id*1264+1259]=-1.0;
		}
		d_DPT[id*1264+1257]=(d_DPT[id*1264+1260]+200)/((d_DPT[id*1264+1259]-5.7)+d_DPT[id*1264+1252]);
		d_DPT[id*1264+1258]=(d_DPT[id*1264+(ii-1)*d_ps[id*81+51]+jj-1]+200)/((d_DPT[id*1264+625+(ii-1)*d_ps[id*81+51]+jj-1])-5.7+d_DPT[id*1264+1252]);
		if((d_DPT[id*1264+1257]>d_DPT[id*1264+1258])||((traceback&&d_DPT[id*1264+1257]>=d_DPT[id*1264+1258])||(traceback==1)))
		{
			d_DPT[id*1264+1255]=d_DPT[id*1264+1259];
			d_DPT[id*1264+1256]=d_DPT[id*1264+1260];
		}
	}
	return;
}

__device__ void fillMatrix(int *d_ps,double *d_DPT,int id,char *d_numSeq)
{
	for(d_ps[id*81+57]=1;d_ps[id*81+57]<=d_ps[id*81+50];d_ps[id*81+57]++)
	{
		for(d_ps[id*81+58]=1;d_ps[id*81+58]<=d_ps[id*81+51];d_ps[id*81+58]++)
		{
			if(fabs(d_DPT[id*1264+(d_ps[id*81+57]-1)*d_ps[id*81+51]+d_ps[id*81+58]-1])<999999999)
			{
				d_DPT[id*1264+1255]=-1.0;
				d_DPT[id*1264+1256]=1.0*INFINITY;
				LSH(d_ps[id*81+57],d_ps[id*81+58],d_ps,d_DPT,id,d_numSeq,parameter);

				if(fabs(d_DPT[id*1264+1256])<999999999)
				{
					d_DPT[id*1264+625+(d_ps[id*81+57]-1)*d_ps[id*81+51]+d_ps[id*81+58]-1]=d_DPT[id*1264+1255];
					d_DPT[id*1264+(d_ps[id*81+57]-1)*d_ps[id*81+51]+d_ps[id*81+58]-1]=d_DPT[id*1264+1256];
				}
				if(d_ps[id*81+57]>1&&d_ps[id*81+58]>1)
				{
					maxTM(d_ps[id*81+57],d_ps[id*81+58],d_ps,d_DPT,id,d_numSeq,parameter);
					for(d_ps[id*81+54]=3;d_ps[id*81+54]<=32;d_ps[id*81+54]++)
					{
						d_ps[id*81+56]=1-d_ps[id*81+54]+d_ps[id*81+58];
						if(d_ps[id*81+56]<1)
						{
							d_ps[id*81+55]=d_ps[id*81+57]-1+d_ps[id*81+56]-1;
							d_ps[id*81+56]=1;
						}
						else
							d_ps[id*81+55]=d_ps[id*81+57]-1;
						for(;d_ps[id*81+55]>0&&d_ps[id*81+56]<d_ps[id*81+58];d_ps[id*81+55]--,d_ps[id*81+56]++)
						{
							if(fabs(d_DPT[id*1264+(d_ps[id*81+55]-1)*d_ps[id*81+51]+d_ps[id*81+56]-1])<999999999)
							{
								d_DPT[id*1264+1255]=-1.0;
								d_DPT[id*1264+1256]=1.0*INFINITY;
								calc_bulge_internal(d_ps[id*81+55],d_ps[id*81+56],d_ps[id*81+57],d_ps[id*81+58],0,d_ps,d_DPT,id,d_numSeq,parameter);

								if(d_DPT[id*1264+1255]<-2500.0)
								{
									d_DPT[id*1264+1255] =-3224.0;
									d_DPT[id*1264+1256] = 0.0;
								}
								if(fabs(d_DPT[id*1264+1256])<999999999)
								{
									d_DPT[id*1264+(d_ps[id*81+57]-1)*d_ps[id*81+51]+d_ps[id*81+58]-1]=d_DPT[id*1264+1256];
									d_DPT[id*1264+625+(d_ps[id*81+57]-1)*d_ps[id*81+51]+d_ps[id*81+58]-1]=d_DPT[id*1264+1255];
								}
							}
						}
					}
				} // if 
			}
		} // for 
	} //for
}

__device__ void RSH(int i,int j,double *d_DPT,char *d_numSeq,int id)
{
	if(d_numSeq[id*54+i]+d_numSeq[id*54+27+j]!=3)
	{
		d_DPT[id*1264+1250]=-1.0;
		d_DPT[id*1264+1251]=1.0*INFINITY;
		return;
	}
	d_DPT[id*1264+1255]=parameter[5680+d_numSeq[id*54+i]*5+d_numSeq[id*54+27+j]]+parameter[4430+d_numSeq[id*54+i]*125+d_numSeq[id*54+i+1]*25+d_numSeq[id*54+27+j]*5+d_numSeq[id*54+27+j+1]];
	d_DPT[id*1264+1257]=parameter[5705+d_numSeq[id*54+i]*5+d_numSeq[id*54+27+j]]+parameter[5055+d_numSeq[id*54+i]*125+d_numSeq[id*54+i+1]*25+d_numSeq[id*54+27+j]*5+d_numSeq[id*54+27+j+1]];
	if(fabs(d_DPT[id*1264+1257])>999999999)
	{
		d_DPT[id*1264+1257]=1.0*INFINITY;
		d_DPT[id*1264+1255]=-1.0;
	}
	if(fabs(parameter[2625+d_numSeq[id*54+i]*25+d_numSeq[id*54+i+1]*5+d_numSeq[id*54+27+j]])<999999999&&fabs(parameter[2875+d_numSeq[id*54+i]*25+d_numSeq[id*54+27+j]*5+d_numSeq[id*54+27+j+1]])<999999999)
	{
		d_DPT[id*1264+1256]=parameter[5680+d_numSeq[id*54+i]*5+d_numSeq[id*54+27+j]]+parameter[2500+d_numSeq[id*54+i]*25+d_numSeq[id*54+i+1]*5+d_numSeq[id*54+27+j]]+parameter[2750+d_numSeq[id*54+i]*25+d_numSeq[id*54+27+j]*5+d_numSeq[id*54+27+j+1]];
		d_DPT[id*1264+1258]=parameter[5705+d_numSeq[id*54+i]*5+d_numSeq[id*54+27+j]]+parameter[2625+d_numSeq[id*54+i]*25+d_numSeq[id*54+i+1]*5+d_numSeq[id*54+27+j]]+parameter[2875+d_numSeq[id*54+i]*25+d_numSeq[id*54+27+j]*5+d_numSeq[id*54+27+j+1]];
		if(fabs(d_DPT[id*1264+1258])>999999999)
		{
			d_DPT[id*1264+1258]=1.0*INFINITY;
			d_DPT[id*1264+1256]=-1.0;
		}
		d_DPT[id*1264+1260]=(d_DPT[id*1264+1258]+200)/(d_DPT[id*1264+1256]-5.7+d_DPT[id*1264+1252]);
		if(fabs(d_DPT[id*1264+1257])<999999999)
		{
			d_DPT[id*1264+1259]=(d_DPT[id*1264+1257]+200)/(d_DPT[id*1264+1255]-5.7+d_DPT[id*1264+1252]);
			if(d_DPT[id*1264+1259]<d_DPT[id*1264+1260])
			{
				d_DPT[id*1264+1255]=d_DPT[id*1264+1256];
				d_DPT[id*1264+1257]=d_DPT[id*1264+1258];
				d_DPT[id*1264+1259]=d_DPT[id*1264+1260];
			}
		}
		else
		{
			d_DPT[id*1264+1255]=d_DPT[id*1264+1256];
			d_DPT[id*1264+1257]=d_DPT[id*1264+1258];
			d_DPT[id*1264+1259]=d_DPT[id*1264+1260];
		}
	}

	if(fabs(parameter[2625+d_numSeq[id*54+i]*25+d_numSeq[id*54+i+1]*5+d_numSeq[id*54+27+j]])<999999999)
	{
		d_DPT[id*1264+1256]=parameter[5680+d_numSeq[id*54+i]*5+d_numSeq[id*54+27+j]]+parameter[2500+d_numSeq[id*54+i]*25+d_numSeq[id*54+i+1]*5+d_numSeq[id*54+27+j]];
		d_DPT[id*1264+1258]=parameter[5705+d_numSeq[id*54+i]*5+d_numSeq[id*54+27+j]]+parameter[2625+d_numSeq[id*54+i]*25+d_numSeq[id*54+i+1]*5+d_numSeq[id*54+27+j]];
		if(fabs(d_DPT[id*1264+1258])>999999999)
		{
			d_DPT[id*1264+1258]=1.0*INFINITY;
			d_DPT[id*1264+1256]=-1.0;
		}
		d_DPT[id*1264+1260]=(d_DPT[id*1264+1258]+200)/(d_DPT[id*1264+1256]-5.7+d_DPT[id*1264+1252]);
		if(fabs(d_DPT[id*1264+1257])<999999999)
		{
			d_DPT[id*1264+1259]=(d_DPT[id*1264+1257]+200)/(d_DPT[id*1264+1255]-5.7+d_DPT[id*1264+1252]);
			if(d_DPT[id*1264+1259]<d_DPT[id*1264+1260])
			{
				d_DPT[id*1264+1255]=d_DPT[id*1264+1256];
				d_DPT[id*1264+1257]=d_DPT[id*1264+1258];
				d_DPT[id*1264+1259]=d_DPT[id*1264+1260];
			}
		}
		else
		{
			d_DPT[id*1264+1255]=d_DPT[id*1264+1256];
			d_DPT[id*1264+1257]=d_DPT[id*1264+1258];
			d_DPT[id*1264+1259]=d_DPT[id*1264+1260];
		}
	}

	if(fabs(parameter[2875+d_numSeq[id*54+i]*25+d_numSeq[id*54+27+j]*5+d_numSeq[id*54+27+j+1]])<999999999)
	{
		d_DPT[id*1264+1256]=parameter[5680+d_numSeq[id*54+i]*5+d_numSeq[id*54+27+j]]+parameter[2750+d_numSeq[id*54+i]*25+d_numSeq[id*54+27+j]*5+d_numSeq[id*54+27+j+1]];
		d_DPT[id*1264+1258]=parameter[5705+d_numSeq[id*54+i]*5+d_numSeq[id*54+27+j]]+parameter[2875+d_numSeq[id*54+i]*25+d_numSeq[id*54+27+j]*5+d_numSeq[id*54+27+j+1]];
		if(fabs(d_DPT[id*1264+1258])>999999999)
		{
			d_DPT[id*1264+1258]=1.0*INFINITY;
			d_DPT[id*1264+1256]=-1.0;
		}
		d_DPT[id*1264+1260]=(d_DPT[id*1264+1258]+200)/(d_DPT[id*1264+1256]-5.7+d_DPT[id*1264+1252]);
		if(fabs(d_DPT[id*1264+1257])<999999999)
		{
			d_DPT[id*1264+1259]=(d_DPT[id*1264+1257]+200)/(d_DPT[id*1264+1255]-5.7+d_DPT[id*1264+1252]);
			if(d_DPT[id*1264+1259]<d_DPT[id*1264+1260])
			{
				d_DPT[id*1264+1255]=d_DPT[id*1264+1256];
				d_DPT[id*1264+1257]=d_DPT[id*1264+1258];
				d_DPT[id*1264+1259]=d_DPT[id*1264+1260];
			}
		}
		else
		{
			d_DPT[id*1264+1255]=d_DPT[id*1264+1256];
			d_DPT[id*1264+1257]=d_DPT[id*1264+1258];
			d_DPT[id*1264+1259]=d_DPT[id*1264+1260];
		}
	}
	d_DPT[id*1264+1256]=parameter[5680+d_numSeq[id*54+i]*5+d_numSeq[id*54+27+j]];
	d_DPT[id*1264+1258]=parameter[5705+d_numSeq[id*54+i]*5+d_numSeq[id*54+27+j]];
	d_DPT[id*1264+1260]=(d_DPT[id*1264+1258]+200)/(d_DPT[id*1264+1256]-5.7+d_DPT[id*1264+1252]);
	if(fabs(d_DPT[id*1264+1257])<999999999)
	{
		if(d_DPT[id*1264+1259]<d_DPT[id*1264+1260])
		{
			d_DPT[id*1264+1250]=d_DPT[id*1264+1256];
			d_DPT[id*1264+1251]=d_DPT[id*1264+1258];
		}
		else
		{
			d_DPT[id*1264+1250]=d_DPT[id*1264+1255];
			d_DPT[id*1264+1251]=d_DPT[id*1264+1257];
		}
	}
	else
	{
		d_DPT[id*1264+1250]=d_DPT[id*1264+1256];
		d_DPT[id*1264+1251]=d_DPT[id*1264+1258];
	}
	return;
}

__device__ void traceback(int i,int j,int *d_ps,double *d_DPT,int id,char *d_numSeq)
{
	d_ps[id*81+i-1]=j;
	d_ps[id*81+25+j-1]=i;
	while(1)
	{
		d_DPT[id*1264+1255]=-1.0;
		d_DPT[id*1264+1256]=1.0*INFINITY;
		LSH(i,j,d_ps,d_DPT,id,d_numSeq,parameter);
		equal(d_DPT[id*1264+625+(i-1)*d_ps[id*81+51]+j-1],d_DPT[id*1264+1255],d_ps,id);
		if(d_ps[id*81+62]==1)
		{
			equal(d_DPT[id*1264+(i-1)*d_ps[id*81+51]+j-1],d_DPT[id*1264+1256],d_ps,id);
			if(d_ps[id*81+62]==1)
				break;
		}

		d_ps[id*81+57]=0;
		if(i>1&&j>1)
		{
			Ss(i-1,j-1,1,d_ps,d_numSeq,id,parameter,d_DPT);
			equal(d_DPT[id*1264+625+(i-1)*d_ps[id*81+51]+j-1],d_DPT[id*1264+1263]+d_DPT[id*1264+625+(i-2)*d_ps[id*81+51]+j-2],d_ps,id);
			if(d_ps[id*81+62]==1)
			{
				i=i-1;
				j=j-1;
				d_ps[id*81+i-1]=j;
				d_ps[id*81+25+j-1]=i;
				d_ps[id*81+57]=1;
			}
		}
		for(d_ps[id*81+54]=3;!d_ps[id*81+57]&&d_ps[id*81+54]<=32;++d_ps[id*81+54])
		{
			d_ps[id*81+55]=i-1;
			d_ps[id*81+56]=-d_ps[id*81+55]-d_ps[id*81+54]+(j+i);
			if(d_ps[id*81+56]<1)
			{
				d_ps[id*81+55]-=abs(d_ps[id*81+56]-1);
				d_ps[id*81+56]=1;
			}
			for(;!d_ps[id*81+57]&&d_ps[id*81+55]>0&&d_ps[id*81+56]<j;--d_ps[id*81+55],++d_ps[id*81+56])
			{
				d_DPT[id*1264+1255]=-1.0;
				d_DPT[id*1264+1256]=1.0*INFINITY;
				calc_bulge_internal(d_ps[id*81+55],d_ps[id*81+56],i,j,1,d_ps,d_DPT,id,d_numSeq,parameter);
				equal(d_DPT[id*1264+625+(i-1)*d_ps[id*81+51]+j-1],d_DPT[id*1264+1255],d_ps,id);
				if(d_ps[id*81+62]==1)
				{
					equal(d_DPT[id*1264+(i-1)*d_ps[id*81+51]+j-1],d_DPT[id*1264+1256],d_ps,id);
					if(d_ps[id*81+62]==1)
					{
						i=d_ps[id*81+55];
						j=d_ps[id*81+56];
						d_ps[id*81+i-1]=j;
						d_ps[id*81+25+j-1]=i;
						d_ps[id*81+57]=1;
						break;
					}
				}
			}
		}
	}
}

__device__ void drawDimer(int *d_ps,int id,double H,double S,double *d_DPT)
{
        if(fabs(d_DPT[id*1264+1253])>999999999)
                d_DPT[id*1264+1263]=0.0;
        else
        {
                d_ps[id*81+55]=0;
                for(d_ps[id*81+54]=0;d_ps[id*81+54]<d_ps[id*81+50];d_ps[id*81+54]++)
                {
                        if(d_ps[id*81+d_ps[id*81+54]]>0)
                                ++d_ps[id*81+55];
                }
                for(d_ps[id*81+54]=0;d_ps[id*81+54]<d_ps[id*81+51];d_ps[id*81+54]++)
                {
                        if(d_ps[id*81+25+d_ps[id*81+54]]>0)
                                ++d_ps[id*81+55];
                }
                d_ps[id*81+55]=(d_ps[id*81+55]/2)-1;
                d_DPT[id*1264+1263]=(H/(S+(d_ps[id*81+55]*-0.51986)+d_DPT[id*1264+1252])-273.15);
        }
}

__device__ void symmetry_thermo(char *d_seq,int start,int length,int *d_ps,int id)
{
	d_ps[id*81+54]=0;
	if(length%2==1)
		d_ps[id*81+62]=0;
	else
	{
		d_ps[id*81+62]=1;
		while(d_ps[id*81+54]<length/2)
		{
			if((d_seq[d_ps[id*81+54]+start]=='A'&&d_seq[start+length-1-d_ps[id*81+54]]!='T')||(d_seq[d_ps[id*81+54]+start]=='T'&&d_seq[start+length-1-d_ps[id*81+54]]!='A')||(d_seq[start+length-1-d_ps[id*81+54]]=='A'&&d_seq[d_ps[id*81+54]+start]!='T')||(d_seq[start+length-1-d_ps[id*81+54]]=='T'&&d_seq[d_ps[id*81+54]+start]!='A'))
			{
				d_ps[id*81+62]=0;
				break;
			}
			if((d_seq[d_ps[id*81+54]+start]=='C'&&d_seq[start+length-1-d_ps[id*81+54]]!='G')||(d_seq[d_ps[id*81+54]+start]=='G'&&d_seq[start+length-1-d_ps[id*81+54]]!='C')||(d_seq[start+length-1-d_ps[id*81+54]]=='C'&&d_seq[d_ps[id*81+54]+start]!='G')||(d_seq[start+length-1-d_ps[id*81+54]]=='G'&&d_seq[d_ps[id*81+54]+start]!='C'))
			{
				d_ps[id*81+62]=0;
				break;
			}
			d_ps[id*81+54]++;
		}
	}
}

__device__ void thal(char *d_seq,int *d_primer,int one_turn,int two_turn,int one_flag,int two_flag,int type,double *d_DPT,int id,int *d_ps,char *d_numSeq)
{
	int i, j;

/*** INIT values for unimolecular and bimolecular structures ***/
	symmetry_thermo(d_seq,d_primer[one_turn*10],d_primer[one_turn*10+1],d_ps,id);
	if(d_ps[id*81+62]==0)
		d_DPT[id*1264+1252]=1.9872* log(38/4000000000.0);
	else
	{
		symmetry_thermo(d_seq,d_primer[two_turn*10],d_primer[two_turn*10+1],d_ps,id);
		if(d_ps[id*81+62]==1)
			d_DPT[id*1264+1252]=1.9872* log(38/1000000000.0);
		else
			d_DPT[id*1264+1252]=1.9872* log(38/4000000000.0);
	}
/* convert nucleotides to numbers */
	if(type==1 || type==2)
	{
		d_ps[id*81+50]=d_primer[one_turn*10+1];
		d_ps[id*81+51]=d_primer[two_turn*10+1];
		if(one_flag==0) //plus
		{
	 		for(i=1;i<=d_ps[id*81+50];++i)
				str2int(d_seq[d_primer[one_turn*10]+i-1],d_numSeq,(id*54+i));
		}
		else
		{
			for(i=1;i<=d_ps[id*81+50];++i)
				str2int_rev(d_seq[d_primer[one_turn*10]+d_primer[one_turn*10+1]-i],d_numSeq,(id*54+i));
		}

		if(two_flag==0)
		{
			for(i=1;i<=d_ps[id*81+51];++i)
				str2int(d_seq[d_primer[two_turn*10]+d_primer[two_turn*10+1]-i],d_numSeq,(id*54+27+i));
		}
		else
		{
			for(i=1;i<=d_ps[id*81+51];++i)
				str2int_rev(d_seq[d_primer[two_turn*10]+i-1],d_numSeq,(id*54+27+i));
		}
	}
	else if(type==3)
	{
		d_ps[id*81+50]=d_primer[two_turn*10+1];
		d_ps[id*81+51]=d_primer[one_turn*10+1];
		if(two_flag==0)
		{
			for(i=1;i<=d_ps[id*81+50];++i)
				str2int(d_seq[d_primer[two_turn*10]+i-1],d_numSeq,(id*54+i));
		}
		else
		{
			for(i=1;i<=d_ps[id*81+50];++i)
				str2int_rev(d_seq[d_primer[two_turn*10]+d_primer[two_turn*10+1]-i],d_numSeq,(id*54+i));
		}
		if(one_flag==0)
		{
			for(i=1;i<=d_ps[id*81+51];++i)
				str2int(d_seq[d_primer[one_turn*10]+d_primer[one_turn*10+1]-i],d_numSeq,(id*54+27+i));
		}
		else
		{
			for(i=1;i<=d_ps[id*81+51];++i)
				str2int_rev(d_seq[d_primer[one_turn*10]+i-1],d_numSeq,(id*54+27+i));
		}
	}
	d_numSeq[id*54+0]=d_numSeq[id*54+d_ps[id*81+50]+1]=d_numSeq[id*54+27+0]=d_numSeq[id*54+27+d_ps[id*81+51]+1]=4; /* mark as N-s */

	initMatrix(d_ps,d_DPT,id,d_numSeq);
	fillMatrix(d_ps,d_DPT,id,d_numSeq);

	d_DPT[id*1264+1253]=-1.0*INFINITY;
/* calculate terminal basepairs */
	d_ps[id*81+52]=d_ps[id*81+53]=0;
	if(type==1)
		for (i=1;i<=d_ps[id*81+50];i++)
		{
			for (j=1;j<=d_ps[id*81+51];j++)
			{
				RSH(i,j,d_DPT,d_numSeq,id);
				d_DPT[id*1264+1250]=d_DPT[id*1264+1250]+0.000001; /* this adding is done for compiler, optimization -O2 vs -O0 */
				d_DPT[id*1264+1251]=d_DPT[id*1264+1251]+0.000001;
				d_DPT[id*1264+1254]=((d_DPT[id*1264+(i-1)*d_ps[id*81+51]+j-1]+d_DPT[id*1264+1251]+200)/((d_DPT[id*1264+625+(i-1)*d_ps[id*81+51]+j-1])+d_DPT[id*1264+1250]-5.7+d_DPT[id*1264+1252]))-273.15;
				if(d_DPT[id*1264+1254]>d_DPT[id*1264+1253]&&((d_DPT[id*1264+625+(i-1)*d_ps[id*81+51]+j-1]+d_DPT[id*1264+1250])<0&&(d_DPT[id*1264+1251]+d_DPT[id*1264+(i-1)*d_ps[id*81+51]+j-1])<0))
				{
					d_DPT[id*1264+1253]=d_DPT[id*1264+1254];
					d_ps[id*81+52]=i;
					d_ps[id*81+53]=j;
				}
			}
		}
	if(type==2||type==3)
	{
	 //THAL_END1
		d_ps[id*81+53]=0;
		d_ps[id*81+52]=d_ps[id*81+50];
		i=d_ps[id*81+50];
		d_DPT[id*1264+1253]=-1.0*INFINITY;
		for (j=1;j<=d_ps[id*81+51];++j)
		{
			RSH(i,j,d_DPT,d_numSeq,id);
			d_DPT[id*1264+1250]=d_DPT[id*1264+1250]+0.000001; // this adding is done for compiler, optimization -O2 vs -O0,that compiler could understand that SH is changed in this cycle 
			d_DPT[id*1264+1251]=d_DPT[id*1264+1251]+0.000001;
			d_DPT[id*1264+1254]=((d_DPT[id*1264+(i-1)*d_ps[id*81+51]+j-1]+d_DPT[id*1264+1251]+200)/((d_DPT[id*1264+625+(i-1)*d_ps[id*81+51]+j-1])+d_DPT[id*1264+1250]-5.7+d_DPT[id*1264+1252]))-273.15;
			if (d_DPT[id*1264+1254]>d_DPT[id*1264+1253]&&((d_DPT[id*1264+1250]+d_DPT[id*1264+625+(i-1)*d_ps[id*81+51]+j-1])<0&&(d_DPT[id*1264+1251]+d_DPT[id*1264+(i-1)*d_ps[id*81+51]+j-1])<0))
			{
				d_DPT[id*1264+1253]=d_DPT[id*1264+1254];
				d_ps[id*81+53]=j;
			}
		}
	}
	if(fabs(d_DPT[id*1264+1253])>999999999)
		d_ps[id*81+52]=d_ps[id*81+53]=1;
	RSH(d_ps[id*81+52],d_ps[id*81+53],d_DPT,d_numSeq,id);
 // tracebacking 
	for (i=0;i<d_ps[id*81+50];++i)
		d_ps[id*81+i]=0;
	for (j=0;j<d_ps[id*81+51];++j)
		d_ps[id*81+25+j] = 0;
	if(fabs(d_DPT[id*1264+(d_ps[id*81+52]-1)*d_ps[id*81+51]+d_ps[id*81+53]-1])<999999999)
	{
		traceback(d_ps[id*81+52],d_ps[id*81+53],d_ps,d_DPT,id,d_numSeq);
		drawDimer(d_ps,id,(d_DPT[id*1264+(d_ps[id*81+52]-1)*d_ps[id*81+51]+d_ps[id*81+53]-1]+d_DPT[id*1264+1251]+200),(d_DPT[id*1264+625+(d_ps[id*81+52]-1)*d_ps[id*81+51]+d_ps[id*81+53]-1]+d_DPT[id*1264+1250]-5.7),d_DPT);
		d_DPT[id*1264+1263]=(int)(100*d_DPT[id*1264+1263]+0.5)/100.0;
	}
	else
	        d_DPT[id*1264+1263]=0.0;
}

struct Node
{
	int pos;
	int gi;
	int strand;  //1:plus,2:minus,3:all ok
	struct Node *next;
};

struct Primer
{
	int pos;
	int len;
	int strand;
	int total_common;
	int total_special;
	float Tm;
	int total; //common number
	struct Primer *next;
	struct Node *common;
	struct Node *special;
};

struct INFO
{
        char name[301];
        int turn;
        struct INFO *next;
};

int check_add(int F3_pos,int *par,int have)
{
        int i,dis;

        for(i=0;i<have;i++)
        {
		if(par[i]==-1)
			return 1;
                dis=par[i]-F3_pos;
                if(abs(dis)<300)              
                        return 0;
        }
        return 1;        
}

void generate_primer(char *seq,char primer[],int start,int length,int flag)
{
        int i;
        if(flag==0)
        {
                for(i=0;i<length;i++)
                	primer[i]=seq[start+i];
        }
        else
        {
                for(i=0;i<length;i++)
                {
                        if(seq[start+length-1-i]=='A')
                                primer[i]='T';
                        else if(seq[start+length-1-i]=='T')
                                primer[i]='A';
                        else if(seq[start+length-1-i]=='C')
                                primer[i]='G';
                        else
                                primer[i]='C';
                }
        }
	primer[length]='\0';
}

__device__ void check_structure(char *d_seq,int *d_primer,int turn[],int ID_thread,int id,double *d_DPT,int *d_ps,char *d_numSeq,int GC[])
{
	for(d_ps[id*81+64]=0;d_ps[id*81+64]<7;d_ps[id*81+64]++)
	{
		for(d_ps[id*81+65]=d_ps[id*81+64]+1;d_ps[id*81+65]<8;d_ps[id*81+65]++)
		{
			if((d_ps[id*81+64]==2||d_ps[id*81+64]==5||d_ps[id*81+65]==2||d_ps[id*81+65]==5)&&(turn[ID_thread*8+2]==-1&&turn[ID_thread*8+5]==-1))
				continue;  //without-loop
			if(turn[ID_thread*8+d_ps[id*81+64]]==-1||turn[ID_thread*8+d_ps[id*81+65]]==-1)
				continue; //when loop, don't have loop
			thal(d_seq,d_primer,turn[ID_thread*8+d_ps[id*81+64]],turn[ID_thread*8+d_ps[id*81+65]],const_int[11+d_ps[id*81+64]],const_int[11+d_ps[id*81+65]],1,d_DPT,id,d_ps,d_numSeq);
			if(d_DPT[id*1264+1263]>49-5*(GC[ID_thread]/2%2))
			{
				d_ps[id*81+66]=0;
                                return;
			}
			thal(d_seq,d_primer,turn[ID_thread*8+d_ps[id*81+64]],turn[ID_thread*8+d_ps[id*81+65]],const_int[11+d_ps[id*81+64]],const_int[11+d_ps[id*81+65]],2,d_DPT,id,d_ps,d_numSeq);
                        if(d_DPT[id*1264+1263]>49-5*(GC[ID_thread]/2%2))
			{
				d_ps[id*81+66]=0;
                                return;
			}
			thal(d_seq,d_primer,turn[ID_thread*8+d_ps[id*81+64]],turn[ID_thread*8+d_ps[id*81+65]],const_int[11+d_ps[id*81+64]],const_int[11+d_ps[id*81+65]],3,d_DPT,id,d_ps,d_numSeq);
                        if(d_DPT[id*1264+1263]>49-5*(GC[ID_thread]/2%2))
			{
				d_ps[id*81+66]=0;
                                return;
			}

			thal(d_seq,d_primer,turn[ID_thread*8+d_ps[id*81+65]],turn[ID_thread*8+d_ps[id*81+64]],(1-const_int[11+d_ps[id*81+65]]),(1-const_int[11+d_ps[id*81+64]]),2,d_DPT,id,d_ps,d_numSeq);
                        if(d_DPT[id*1264+1263]>49-5*(GC[ID_thread]/2%2))
			{
				d_ps[id*81+66]=0;
                                return;
			}
                        thal(d_seq,d_primer,turn[ID_thread*8+d_ps[id*81+65]],turn[ID_thread*8+d_ps[id*81+64]],(1-const_int[11+d_ps[id*81+65]]),(1-const_int[11+d_ps[id*81+64]]),3,d_DPT,id,d_ps,d_numSeq);
                        if(d_DPT[id*1264+1263]>49-5*(GC[ID_thread]/2%2))
			{
				d_ps[id*81+66]=0;
                                return;
			}
		}
	}
	d_ps[id*81+66]=1;
}

void how_many(struct Primer *head,int common)
{
        struct Primer *p_primer;
        struct Node *p_node;
        int i,num,*list;

	list=(int *)malloc(common*sizeof(int));
        p_primer=head;
        while(p_primer)
        {
                p_node=p_primer->common;
		for(i=0;i<common;i++)
                {
                        list[i]=0;
                }
                i=0;
                while(p_node)
                {
                        i++;
			list[p_node->gi]=1;
                        p_node=p_node->next;
                }
		p_primer->total_common=i;

        //special
                p_node=p_primer->special;
                i=0;
                while(p_node)       
                {
                        i++;
                        p_node=p_node->next;
                }
		p_primer->total_special=i;

		num=0;
                for(i=0;i<common;i++)
                {
                        num=num+list[i];
                }
		p_primer->total=num;
                p_primer=p_primer->next;
        }
	free(list);
}

//get the file size
int file_size2(char* filename)
{
        struct stat statbuf;
        stat(filename,&statbuf);
        int size=statbuf.st_size;
        return size;
}

void merge(int *store,int *temp,int *data,int start,int end,int middle)
{
        int i,j,k;
        i=start;
        j=middle+1;
        k=start;
        while(i<=middle&&j<=end)
        {
                if(data[store[i]*5]<data[store[j]*5])
                {
                        temp[k]=store[i];
                        k++;
                        i++;
                        continue;
                }
                if(data[store[j]*5]<data[store[i]*5])
                {
                        temp[k]=store[j];
                        k++;
                        j++;
                        continue;
                }
        //len
                if(data[store[i]*5+1]<data[store[j]*5+1])
                {
                        temp[k]=store[i];
                        k++;
                        i++;
                        continue;
                }
                if(data[store[j]*5+1]<data[store[i]*5+1])
                {
                        temp[k]=store[j];
                        k++;
                        j++;
                        continue;
                }
        //gi
                if(data[store[i]*5+2]<data[store[j]*5+2])
                {
                        temp[k]=store[i];
                        k++;
                        i++;
                        continue;
                }
                if(data[store[j]*5+2]<data[store[i]*5+2])
                {
                        temp[k]=store[j];
                        k++;
                        j++;
                        continue;
                }
        //position
                if(data[store[i]*5+3]<data[store[j]*5+3])
                {
                        temp[k]=store[i];
                        k++;
                        i++;
                }
                else
                {
                        temp[k]=store[j];
                        k++;
                        j++;
                        continue;
                }
        }
        while(i<=middle)
        {
                temp[k]=store[i];
                k++;
                i++;
        }
        while(j<=end)
        {
                temp[k]=store[j];
                k++;
                j++;
        }
        for(i=start;i<=end;i++)
                store[i]=temp[i];
}

void sort_merge(int *store,int *temp,int *data,int start,int end)
{
        int middle;

        if(start<end)
        {
                middle=(start+end)/2;
                sort_merge(store,temp,data,start,middle);
                sort_merge(store,temp,data,(middle+1),end);
                merge(store,temp,data,start,end,middle);
        }
}

////function read primer informatin and align information 
struct Primer *read_par(char *path,int common_flag,int special_flag)
{
        char *in,line[100];
	float Tm;
        int pos,len,gi,position,plus,minus,size,i,flag,total,*store,*temp,*data;
        struct Primer *new_primer,*p_primer,*head;
        struct Node *new_node,*p_node;
        FILE *fp;

///read the  primer file
        if(access(path,0)==-1)
        {
                printf("Error! Don't have the %s file!\n",path);
                exit(1);
        }
        fp=fopen(path,"r");
        if(fp==NULL)
        {
                printf("Error: can't open the %s file!\n",path);
                exit(1);
        }
        
        size=sizeof(struct Primer);
        i=0;
        while(fscanf(fp,"pos:%d\tlength:%d\t+:%d\t-:%d\t%f\n",&pos,&len,&plus,&minus,&Tm)!=EOF)
        {
                new_primer=(struct Primer *)malloc(size);
                new_primer->pos=pos;
                new_primer->len=len;
		new_primer->Tm=Tm;
                new_primer->total=1;
		new_primer->total_common=0;
		new_primer->total_special=0;
		new_primer->strand=10*minus+plus;
                new_primer->next=NULL;
                new_primer->common=NULL;
                new_primer->special=NULL;

                if(i==0)
                {
                        head=new_primer;
                        p_primer=new_primer;
                        i++;
                }
                else
                {
                        p_primer->next=new_primer;
                        p_primer=new_primer;
                }
        }
        fclose(fp);
        if(i==0)
        {
                printf("Sorry! Don't have any candidate single primers in %s!\n",path);
                exit(1);
        }

//parameter of common
        if(common_flag==1)
        {
                i=strlen(path);
                in=(char *)malloc(i+20);
                memset(in,'\0',i+20);
                strcpy(in,path);
                strcat(in,"-common.txt"); //suffix of parameter
                if(access(in,0)==-1)
                {
                        printf("Error! Don't have the %s file!\n",in);
                        exit(1);
                }

                fp=fopen(in,"r");
                if(fp==NULL)
                {
                        printf("Error: can't open the %s file!\n",in);
                        exit(1);
                }

		total=0;
                while(fgets(line,100,fp)!=NULL)
                        total++;
                rewind(fp);
                store=(int *)malloc(total*sizeof(int));
                temp=(int *)malloc(total*sizeof(int));
                data=(int *)malloc(5*total*sizeof(int));
                total=0;
                while(fscanf(fp,"%d\t%d\t%d\t%d\t%d\t%d\n",&pos,&len,&gi,&position,&plus,&minus)!=EOF)
                {
                        data[5*total]=pos;
                        data[5*total+1]=len;
                        data[5*total+2]=gi;
                        data[5*total+3]=position;
			if(plus)
                        {
                                if(minus)
                                        data[5*total+4]=3;
                                else
					data[5*total+4]=1;
                        }
                        else
                                data[5*total+4]=2;
                        store[total]=total;
                        total++;
                }
                fclose(fp);
                sort_merge(store,temp,data,0,(total-1));

                p_primer=head;
                size=sizeof(struct Node);
                flag=0;
                i=0;
                while(p_primer&&i<total)
                {
                //pos
                        if(data[store[i]*5]<p_primer->pos)
                        {
                                i++;
                                continue;
                        }
                        if(data[store[i]*5]>p_primer->pos)
                        {
                                p_primer=p_primer->next;
                                flag=0;
                                continue;
                        }
                //len
                        if(data[store[i]*5+1]<p_primer->len)
                        {
                                i++;
                                continue;
                        }
                        if(data[store[i]*5+1]>p_primer->len)
                        {
                                p_primer=p_primer->next;
                                flag=0;
                                continue;
                        }
                        new_node=(struct Node *)malloc(size);
                        new_node->gi=data[store[i]*5+2];
                        new_node->pos=data[store[i]*5+3];
                        new_node->strand=data[store[i]*5+4];
                        new_node->next=NULL;
                        if(flag==0)
                        {
                                flag++;
                                p_primer->common=new_node;
                                p_node=new_node;
                        }
                        else
                        {
                                p_node->next=new_node;
                                p_node=new_node;
                        }
			p_primer->total_common++;
                        i++;
                }
                free(in);
                free(data);
                free(store);
                free(temp);
        }
//paramter for special
        if(special_flag==1)
        {
                i=strlen(path);
                in=(char *)malloc(i+20);
                memset(in,'\0',i+20);
                strcpy(in,path);
                strcat(in,"-specific.txt"); //suffix of parameter
                if(access(in,0)==-1)
                {
                        printf("Error! Don't have the %s file!\n",in);
                        exit(1);
                }

                fp=fopen(in,"r");
                if(fp==NULL)
                {
                        printf("Error: can't open the %s file!\n",in);
                        exit(1);
                }

		total=0;
                while(fgets(line,100,fp)!=NULL)
                        total++;
                rewind(fp);
                store=(int *)malloc(total*sizeof(int));
                temp=(int *)malloc(total*sizeof(int));
                data=(int *)malloc(5*total*sizeof(int));
                total=0;
                while(fscanf(fp,"%d\t%d\t%d\t%d\t%d\t%d\n",&pos,&len,&gi,&position,&plus,&minus)!=EOF)
                {
                        data[5*total]=pos;
                        data[5*total+1]=len;
                        data[5*total+2]=gi;
                        data[5*total+3]=position;
                        data[5*total+4]=plus;
			if(plus)
                        {
                                if(minus)
                                        data[5*total+4]=3;
                                else
                                        data[5*total+4]=1;
                        }
                        else
                                data[5*total+4]=2;
                        store[total]=total;
                        total++;
                }
                fclose(fp);
                sort_merge(store,temp,data,0,(total-1));

                p_primer=head;
                size=sizeof(struct Node);
                flag=0;
                i=0;
                while(p_primer&&i<total)
                {
                //pos
                        if(data[store[i]*5]<p_primer->pos)
                        {
                                i++;
                                continue;
                        }
                        if(data[store[i]*5]>p_primer->pos)
                        {
                                p_primer=p_primer->next;
                                flag=0;
                                continue;
                        }
                //len
                        if(data[store[i]*5+1]<p_primer->len)
                        {
                                i++;
                                continue;
                        }
                        if(data[store[i]*5+1]>p_primer->len)
                        {
                                p_primer=p_primer->next;
                                flag=0;
                                continue;
                        }
                        new_node=(struct Node *)malloc(size);
                        new_node->gi=data[store[i]*5+2];
                        new_node->pos=data[store[i]*5+3];
                        new_node->strand=data[store[i]*5+4];
                        new_node->next=NULL;
                        if(flag==0)
                        {
                                flag++;
                                p_primer->special=new_node;
                                p_node=new_node;
                        }
                        else
                        {
                                p_node->next=new_node;
                                p_node=new_node;
                        }
			p_primer->total_special++;
                        i++;
                }
                free(in);
                free(data);
                free(store);
                free(temp);
        }
        return head;
}

//check this LAMP primers are uniq or not
//return=0: stop and return=1: go on
__device__ void check_uniq(int *d_primer,int *d_info,int turn[],int ID_thread,int id,int *d_ps)
{
//plus
	d_ps[id*81+74]=d_primer[turn[ID_thread*8+1]*10+5]; //F2
	d_ps[id*81+75]=d_primer[turn[ID_thread*8+3]*10+5]; //F1c
	d_ps[id*81+76]=d_primer[turn[ID_thread*8+4]*10+5]; //B1c
	d_ps[id*81+77]=d_primer[turn[ID_thread*8+6]*10+5];
	d_ps[id*81+78]=d_primer[turn[ID_thread*8+7]*10+5];
        for(d_ps[id*81+64]=d_primer[turn[ID_thread*8]*10+5];d_ps[id*81+64]<d_primer[turn[ID_thread*8]*10+6];d_ps[id*81+64]++)
        {
                if((d_info[d_ps[id*81+64]*3+2]&1)!=1)
                        continue;
		d_ps[id*81+70]=d_info[d_ps[id*81+64]*3]; //gi
                for(d_ps[id*81+65]=d_ps[id*81+74];d_ps[id*81+65]<d_primer[turn[ID_thread*8+1]*10+6];d_ps[id*81+65]++)
                {
		//check gi
			if(d_info[d_ps[id*81+65]*3]<d_ps[id*81+70])
			{
				d_ps[id*81+74]++;
				continue;
			}
			if(d_info[d_ps[id*81+65]*3]>d_ps[id*81+70])
				break;
			if(d_info[d_ps[id*81+65]*3+1]<d_info[d_ps[id*81+64]*3+1])
			{
				d_ps[id*81+74]++;
				continue;
			}
			if(d_info[d_ps[id*81+65]*3+1]-d_info[d_ps[id*81+64]*3+1]>1000)
				break;
                        if((d_info[d_ps[id*81+65]*3+2]&1)!=1)
				continue;
                        for(d_ps[id*81+66]=d_ps[id*81+75];d_ps[id*81+66]<d_primer[turn[ID_thread*8+3]*10+6];d_ps[id*81+66]++) //F1c
                        {
			//check
				if(d_info[d_ps[id*81+66]*3]<d_ps[id*81+70])
				{
					d_ps[id*81+75]++;
					continue;
				}
				if(d_info[d_ps[id*81+66]*3]>d_ps[id*81+70])
					break;
				if(d_info[d_ps[id*81+66]*3+1]<d_info[d_ps[id*81+64]*3+1])
				{
					d_ps[id*81+75]++;
                                        continue;
                                }
				if(d_info[d_ps[id*81+66]*3+1]-d_info[d_ps[id*81+64]*3+1]>1000)
					break;
                                if((d_info[d_ps[id*81+66]*3+2]&2)!=2)
                                        continue;
                                for(d_ps[id*81+67]=d_ps[id*81+76];d_ps[id*81+67]<d_primer[turn[ID_thread*8+4]*10+6];d_ps[id*81+67]++) //B1c
                                {
					if(d_info[d_ps[id*81+67]*3]<d_ps[id*81+70])
					{
						d_ps[id*81+76]++;
						continue;
					}
                                        if(d_info[d_ps[id*81+67]*3]>d_ps[id*81+70])
						break;
					if(d_info[d_ps[id*81+67]*3+1]<d_info[d_ps[id*81+64]*3+1])
					{
						d_ps[id*81+76]++;
                                                continue;
                                        }
					if(d_info[d_ps[id*81+67]*3+1]-d_info[d_ps[id*81+64]*3+1]>1000)
						break;
                                        if((d_info[d_ps[id*81+67]*3+2]&1)!=1)
                                                continue;
                                        for(d_ps[id*81+68]=d_ps[id*81+77];d_ps[id*81+68]<d_primer[turn[ID_thread*8+6]*10+6];d_ps[id*81+68]++) //B2
                                        {
                                                if(d_info[d_ps[id*81+68]*3]<d_ps[id*81+70])
						{
							d_ps[id*81+77]++;
                                                        continue;
						}
						if(d_info[d_ps[id*81+68]*3]>d_ps[id*81+70])
							break;
						if(d_info[d_ps[id*81+68]*3+1]<d_info[d_ps[id*81+64]*3+1])
						{
							d_ps[id*81+77]++;
                                                        continue;
                                                }
						if(d_info[d_ps[id*81+68]*3+1]-d_info[d_ps[id*81+64]*3+1]>1000)
							break;
                                                if((d_info[d_ps[id*81+68]*3+2]&2)!=2)
                                                        continue;
                                                for(d_ps[id*81+69]=d_ps[id*81+78];d_ps[id*81+69]<d_primer[turn[ID_thread*8+7]*10+6];d_ps[id*81+69]++)
                                                {
                                                        if(d_info[d_ps[id*81+69]*3]<d_ps[id*81+70])
							{
								d_ps[id*81+78]++;
                                                                continue;
							}
							if(d_info[d_ps[id*81+69]*3]>d_ps[id*81+70])
								break;
							if(d_info[d_ps[id*81+69]*3+1]<d_info[d_ps[id*81+64]*3+1])
							{
								d_ps[id*81+78]++;
                                                                continue;
                                                        }
							if(d_info[d_ps[id*81+69]*3+1]-d_info[d_ps[id*81+64]*3+1]>1000)
								break;
                                                        if((d_info[d_ps[id*81+69]*3+2]&2)!=2)
                                                                continue;
                                                //F3-F2 
                                                        if(d_info[d_ps[id*81+65]*3+1]<d_info[d_ps[id*81+64]*3+1])
                                                                continue;
                                                //F2-F1c
                                                        if(d_info[d_ps[id*81+66]*3+1]<d_info[d_ps[id*81+65]*3+1]+d_primer[turn[ID_thread*8+1]*10+1])
                                                                continue;
                                                //F1c-B1c
                                                        if(d_info[d_ps[id*81+67]*3+1]<d_info[d_ps[id*81+66]*3+1]+d_primer[turn[ID_thread*8+3]*10+1])
                                                                continue;
                                                //B1c-B2
                                                        if(d_info[d_ps[id*81+68]*3+1]<d_info[d_ps[id*81+67]*3+1]+d_primer[turn[ID_thread*8+4]*10+1])
                                                                continue;
                                                //B2-B3
                                                        if(d_info[d_ps[id*81+69]*3+1]<d_info[d_ps[id*81+68]*3+1])
                                                                continue;
                                                //whole
                                                        if(d_info[d_ps[id*81+69]*3+1]-d_info[d_ps[id*81+64]*3+1]>1000)
                                                                continue;
                                                        d_ps[id*81+71]=0;
							return;
                                                }//B3
                                        }
                                }//B1c
                        }
                }//F2
        }

//minus
	d_ps[id*81+74]=d_primer[turn[ID_thread*8+1]*10+5]; //F2
        d_ps[id*81+75]=d_primer[turn[ID_thread*8+3]*10+5]; //F1c
        d_ps[id*81+76]=d_primer[turn[ID_thread*8+4]*10+5]; //B1c
        d_ps[id*81+77]=d_primer[turn[ID_thread*8+6]*10+5];
        d_ps[id*81+78]=d_primer[turn[ID_thread*8+7]*10+5];
        for(d_ps[id*81+64]=d_primer[turn[ID_thread*8]*10+5];d_ps[id*81+64]<d_primer[turn[ID_thread*8]*10+6];d_ps[id*81+64]++)
        {
                if((d_info[d_ps[id*81+64]*3+2]&2)!=2)
                        continue;
		d_ps[id*81+70]=d_info[d_ps[id*81+64]*3];
                for(d_ps[id*81+65]=d_ps[id*81+74];d_ps[id*81+65]<d_primer[turn[ID_thread*8+1]*10+6];d_ps[id*81+65]++)
                {
			if(d_info[d_ps[id*81+65]*3]<d_ps[id*81+70])
                        {
                                d_ps[id*81+74]++;
                                continue;
                        }
                        if(d_info[d_ps[id*81+65]*3]>d_ps[id*81+70])
                                break;
                        if(d_info[d_ps[id*81+65]*3+1]>d_info[d_ps[id*81+64]*3+1])
				break;
			if(d_info[d_ps[id*81+65]*3+1]-d_info[d_ps[id*81+64]*3+1]<-1000)
			{
				d_ps[id*81+74]++;
                                continue;
                        }
                        if((d_info[d_ps[id*81+65]*3+2]&2)!=2)
                                continue;
                        for(d_ps[id*81+66]=d_ps[id*81+75];d_ps[id*81+66]<d_primer[turn[ID_thread*8+3]*10+6];d_ps[id*81+66]++)
                        {
				if(d_info[d_ps[id*81+66]*3]<d_ps[id*81+70])
                                {
                                        d_ps[id*81+75]++;
                                        continue;
                                }
                                if(d_info[d_ps[id*81+66]*3]>d_ps[id*81+70])
                                        break;
				if(d_info[d_ps[id*81+66]*3+1]-d_info[d_ps[id*81+64]*3+1]<-1000)
				{
                                        d_ps[id*81+75]++;
                                        continue;
                                }
                                if(d_info[d_ps[id*81+66]*3+1]>d_info[d_ps[id*81+64]*3+1])
					break;				
                                if((d_info[d_ps[id*81+66]*3+2]&1)!=1)
                                        continue;
                                for(d_ps[id*81+67]=d_ps[id*81+76];d_ps[id*81+67]<d_primer[turn[ID_thread*8+4]*10+6];d_ps[id*81+67]++)
                                {
					if(d_info[d_ps[id*81+67]*3]<d_ps[id*81+70])
                                        {
                                                d_ps[id*81+76]++;
                                                continue;
                                        }
                                        if(d_info[d_ps[id*81+67]*3]>d_ps[id*81+70])
                                                break;
					if(d_info[d_ps[id*81+67]*3+1]-d_info[d_ps[id*81+64]*3+1]<-1000)
					{
                                                d_ps[id*81+76]++;
                                                continue;
                                        }
                                        if(d_info[d_ps[id*81+67]*3+1]>d_info[d_ps[id*81+64]*3+1])
						break;
                                        if((d_info[d_ps[id*81+67]*3+2]&2)!=2)
                                                continue;
                                        for(d_ps[id*81+68]=d_ps[id*81+77];d_ps[id*81+68]<d_primer[turn[ID_thread*8+6]*10+6];d_ps[id*81+68]++)
                                        {
						if(d_info[d_ps[id*81+68]*3]<d_ps[id*81+70])
                                                {
                                                        d_ps[id*81+77]++;
                                                        continue;
                                                }
                                                if(d_info[d_ps[id*81+68]*3]>d_ps[id*81+70])
                                                        break;
						if(d_info[d_ps[id*81+68]*3+1]-d_info[d_ps[id*81+64]*3+1]<-1000)
						{
                                                        d_ps[id*81+77]++;
                                                        continue;
                                                }
                                                if(d_info[d_ps[id*81+68]*3+1]>d_info[d_ps[id*81+64]*3+1])
							break;
                                                if((d_info[d_ps[id*81+68]*3+2]&1)!=1)
                                                        continue;
                                                for(d_ps[id*81+69]=d_ps[id*81+78];d_ps[id*81+69]<d_primer[turn[ID_thread*8+7]*10+6];d_ps[id*81+69]++)
                                                {
							if(d_info[d_ps[id*81+69]*3]<d_ps[id*81+70])
                                                        {
                                                                d_ps[id*81+78]++;
                                                                continue;
                                                        }
                                                        if(d_info[d_ps[id*81+69]*3]>d_ps[id*81+70])
                                                                break;
							if(d_info[d_ps[id*81+69]*3+1]-d_info[d_ps[id*81+64]*3+1]<-1000)
							{
                                                                d_ps[id*81+78]++;
                                                                continue;
                                                        }
                                                        if(d_info[d_ps[id*81+69]*3+1]>d_info[d_ps[id*81+64]*3+1])
								break;
                                                        if((d_info[d_ps[id*81+69]*3+2]&1)!=1)
                                                                continue;
                                                //F3-F2 
                                                        if(d_info[d_ps[id*81+64]*3+1]<d_info[d_ps[id*81+65]*3+1])
                                                                continue;
                                                //F2-F1c
                                                        if(d_info[d_ps[id*81+65]*3+1]<d_info[d_ps[id*81+66]*3+1]+d_primer[turn[ID_thread*8+3]*10+1])
                                                                continue;
                                                //F1c-B1c
                                                        if(d_info[d_ps[id*81+66]*3+1]<d_info[d_ps[id*81+67]*3+1]+d_primer[turn[ID_thread*8+4]*10+1])
                                                                continue;
                                                //B1c-B2
                                                        if(d_info[d_ps[id*81+67]*3+1]<d_info[d_ps[id*81+68]*3+1]+d_primer[turn[ID_thread*8+6]*10+1])
                                                                continue;
                                                //B2-B3
                                                        if(d_info[d_ps[id*81+68]*3+1]<d_info[d_ps[id*81+69]*3+1])
                                                                continue;
                                                //whole
                                                        if(d_info[d_ps[id*81+64]*3+1]-d_info[d_ps[id*81+69]*3+1]>1000)
                                                                continue;
                                                        d_ps[id*81+71]=0;
							return;
                                                }
                                        }
                                }
                        }
                }
        }
        d_ps[id*81+71]=1;
}

//from first to second
__global__ void next_one(int *d_primer,int one_start,int one_end,int two_start,int two_end,int pos) //7,8,9
{
        int id=blockDim.x*blockIdx.x+threadIdx.x;
	int i;

	while(one_start+id<one_end)
	{
		i=id+two_start;
		if(i>=two_end)
		{
			i=two_end-1;
		}
		if(d_primer[10*i]>=d_primer[(id+one_start)*10]+d_primer[(id+one_start)*10+1])
		{
			while((i>=two_start)&&(d_primer[10*i]>=d_primer[(id+one_start)*10]+d_primer[(id+one_start)*10+1]))
			{
				d_primer[10*(id+one_start)+pos]=i;
				i--;
			}
		}
		else
		{
			while((i<two_end)&&(d_primer[10*i]<d_primer[(id+one_start)*10]+d_primer[(id+one_start)*10+1]))
				i++;
			if(i==two_end)
				d_primer[10*(id+one_start)+pos]=-1;
			else
				d_primer[10*(id+one_start)+pos]=i;
		}
		id=id+blockDim.x*gridDim.x;
	}
	__syncthreads();
}

__device__ int check_gc(char *d_seq,int start,int end,int *d_ps,int id)
{
	d_ps[id*81+65]=0;
        for(d_ps[id*81+64]=start;d_ps[id*81+64]<end;d_ps[id*81+64]++)
        {
                if(d_seq[d_ps[id*81+64]]=='C'||d_seq[d_ps[id*81+64]]=='G')
                        d_ps[id*81+65]++;
        }
        if(d_ps[id*81+65]*100.0/(end-start)>=60)
		return 1;
        if(d_ps[id*81+65]*100.0/(end-start)<=45)
		return 2;

	return 4;
}

__device__ void check_common(int *d_primer,int *d_info,int turn[],int ID_thread,int id,int *d_result,int *d_ps)
{
	for(d_ps[id*81+72]=0;d_ps[id*81+72]<const_int[7];d_ps[id*81+72]++)
        {
                d_result[(8+const_int[7])*turn[ID_thread*8]+8+d_ps[id*81+72]]=0;
        }
//plus
	d_ps[id*81+74]=d_primer[turn[ID_thread*8+1]*10+3]; //F2
	d_ps[id*81+75]=d_primer[turn[ID_thread*8+3]*10+3];//F1c
	d_ps[id*81+76]=d_primer[turn[ID_thread*8+4]*10+3];//B1c
	d_ps[id*81+77]=d_primer[turn[ID_thread*8+6]*10+3];//B2
	d_ps[id*81+78]=d_primer[turn[ID_thread*8+7]*10+3];//B3
	if(turn[ID_thread*8+2]!=-1)
		d_ps[id*81+79]=d_primer[turn[ID_thread*8+2]*10+3];//LF
	if(turn[ID_thread*8+5]!=-1)
		d_ps[id*81+80]=d_primer[turn[ID_thread*8+5]*10+3];

        for(d_ps[id*81+64]=d_primer[turn[ID_thread*8]*10+3];d_ps[id*81+64]<d_primer[turn[ID_thread*8]*10+4];d_ps[id*81+64]++)
        {
                if((d_info[d_ps[id*81+64]*3+2]&1)!=1)
                        continue;
		d_ps[id*81+72]=d_info[d_ps[id*81+64]*3]; //gi
                if(d_result[(8+const_int[7])*turn[ID_thread*8]+8+d_ps[id*81+72]]!=0)
                        continue;
                for(d_ps[id*81+65]=d_ps[id*81+74];d_ps[id*81+65]<d_primer[turn[ID_thread*8+1]*10+4];d_ps[id*81+65]++)
                {
                        if(d_info[d_ps[id*81+65]*3]<d_ps[id*81+72])
			{
				d_ps[id*81+74]++;
                                continue;
			}
			if(d_info[d_ps[id*81+65]*3]>d_ps[id*81+72])
				break;
                        if((d_info[d_ps[id*81+65]*3+2]&1)!=1)
                                continue;
			if(d_info[d_ps[id*81+65]*3+1]<d_info[d_ps[id*81+64]*3+1])
                        {
                                d_ps[id*81+74]++;
                                continue;
                        }
			if(d_info[d_ps[id*81+65]*3+1]-d_info[d_ps[id*81+64]*3+1]>300)
				break;
                        for(d_ps[id*81+66]=d_ps[id*81+75];d_ps[id*81+66]<d_primer[turn[ID_thread*8+3]*10+4];d_ps[id*81+66]++) //F1c
                        {
                                if(d_info[d_ps[id*81+66]*3]<d_ps[id*81+72])
				{
					d_ps[id*81+75]++;
                                        continue;
				}
				if(d_info[d_ps[id*81+66]*3]>d_ps[id*81+72])
					break;
                                if((d_info[d_ps[id*81+66]*3+2]&2)!=2)
                                        continue;
				if(d_info[d_ps[id*81+66]*3+1]<d_info[d_ps[id*81+64]*3+1])
                                {
                                        d_ps[id*81+75]++;
                                        continue;
                                }
				if(d_info[d_ps[id*81+66]*3+1]-d_info[d_ps[id*81+64]*3+1]>300)
					break;
                                for(d_ps[id*81+67]=d_ps[id*81+76];d_ps[id*81+67]<d_primer[turn[ID_thread*8+4]*10+4];d_ps[id*81+67]++) //B1c
                                {
                                        if(d_info[d_ps[id*81+67]*3]<d_ps[id*81+72])
					{
						d_ps[id*81+76]++;
                                                continue;
					}
					if(d_info[d_ps[id*81+67]*3]>d_ps[id*81+72])
						break;
                                        if((d_info[d_ps[id*81+67]*3+2]&1)!=1)
                                                continue;
					if(d_info[d_ps[id*81+67]*3+1]<d_info[d_ps[id*81+64]*3+1])
                                        {
                                                d_ps[id*81+76]++;
                                                continue;
                                        }
					if(d_info[d_ps[id*81+67]*3+1]-d_info[d_ps[id*81+64]*3+1]>300)
						break;
                                        for(d_ps[id*81+68]=d_ps[id*81+77];d_ps[id*81+68]<d_primer[turn[ID_thread*8+6]*10+4];d_ps[id*81+68]++) //B2
                                        {
                                                if(d_info[d_ps[id*81+68]*3]<d_ps[id*81+72])
						{
							d_ps[id*81+77]++;
                                                        continue;
						}
						if(d_info[d_ps[id*81+68]*3]>d_ps[id*81+72])
							break;
                                                if((d_info[d_ps[id*81+68]*3+2]&2)!=2)
                                                        continue;
						if(d_info[d_ps[id*81+68]*3+1]<d_info[d_ps[id*81+64]*3+1])
                                                {
                                                        d_ps[id*81+77]++;
                                                        continue;
                                                }
						if(d_info[d_ps[id*81+68]*3+1]-d_info[d_ps[id*81+64]*3+1]>300)
							break;
                                                for(d_ps[id*81+69]=d_ps[id*81+78];d_ps[id*81+69]<d_primer[turn[ID_thread*8+7]*10+4];d_ps[id*81+69]++)
                                                {
                                                        if(d_info[d_ps[id*81+69]*3]<d_ps[id*81+72])
							{
								d_ps[id*81+78]++;
                                                                continue;
							}
							if(d_info[d_ps[id*81+69]*3]>d_ps[id*81+72])
								break;
                                                        if((d_info[d_ps[id*81+69]*3+2]&2)!=2)
                                                                continue;
							if(d_info[d_ps[id*81+69]*3+1]<d_info[d_ps[id*81+64]*3+1])
                                                        {
                                                                d_ps[id*81+78]++;
                                                                continue;
                                                        }
							if(d_info[d_ps[id*81+69]*3+1]-d_info[d_ps[id*81+64]*3+1]>300)
								break;
                                                //F3-F2 
                                                        d_ps[id*81+71]=d_info[d_ps[id*81+65]*3+1]-(d_info[d_ps[id*81+64]*3+1]+d_primer[turn[ID_thread*8]*10+1]);
                                                        if(d_ps[id*81+71]<0)
                                                                continue;
                                                        if(d_ps[id*81+71]>20)
                                                                continue;
                                                //F2-F1c
                                                        d_ps[id*81+71]=d_info[d_ps[id*81+66]*3+1]-d_info[d_ps[id*81+65]*3+1]-1;
                                                        if(d_ps[id*81+71]<40)
                                                                continue;
                                                        if(d_ps[id*81+71]>60)
                                                                continue;
                                                //F1c-B1c
                                                        d_ps[id*81+71]=d_info[d_ps[id*81+67]*3+1]-(d_info[d_ps[id*81+66]*3+1]+d_primer[turn[ID_thread*8+3]*10+1]-1)-1;
                                                        if(d_ps[id*81+71]<0)
                                                                continue;
                                                //B1c-B2
                                                        d_ps[id*81+71]=(d_info[d_ps[id*81+68]*3+1]+d_primer[turn[ID_thread*8+6]*10+1]-1)-(d_info[d_ps[id*81+67]*3+1]+d_primer[turn[ID_thread*8+4]*10+1]-1)-1;
                                                        if(d_ps[id*81+71]<40)
                                                                continue;
                                                        if(d_ps[id*81+71]>60)
                                                                continue;
                                                //F2-B2
                                                        d_ps[id*81+71]=d_info[d_ps[id*81+68]*3+1]+d_primer[turn[ID_thread*8+6]*10+1]-1-d_info[d_ps[id*81+65]*3+1]-1;
                                                        if(d_ps[id*81+71]<120)
                                                                continue;
                                                        if(d_ps[id*81+71]>180)
                                                                continue;
                                                //B2-B3
                                                        d_ps[id*81+71]=d_info[d_ps[id*81+69]*3+1]-(d_info[d_ps[id*81+68]*3+1]+d_primer[turn[ID_thread*8+6]*10+1]-1)-1;
                                                        if(d_ps[id*81+71]<0)
                                                                continue;
                                                        if(d_ps[id*81+71]>20)
                                                                continue;
                                                //LF
                                                        if(turn[ID_thread*8+2]!=-1)
                                                        {
                                                                d_ps[id*81+71]=0;
                                                                for(d_ps[id*81+70]=d_ps[id*81+79];d_ps[id*81+70]<d_primer[turn[ID_thread*8+2]*10+4];d_ps[id*81+70]++)
                                                                {
                                                                        if(d_info[d_ps[id*81+70]*3]<d_ps[id*81+72])
									{
										d_ps[id*81+79]++;
                                                                                continue;
									}
									if(d_info[d_ps[id*81+70]*3]>d_ps[id*81+72])
										break;
                                                                        if((d_info[d_ps[id*81+70]*3+2]&2)!=2)
                                                                                continue;
									if(d_info[d_ps[id*81+70]*3+1]<d_info[d_ps[id*81+64]*3+1])
									{
										d_ps[id*81+79]++;
                                                                                continue;
                                                                        }
									if(d_info[d_ps[id*81+70]*3+1]-d_info[d_ps[id*81+64]*3+1]>300)
										break;
                                                                        if(d_info[d_ps[id*81+65]*3+1]+d_primer[turn[ID_thread*8+1]*10+1]>d_info[d_ps[id*81+70]*3+1])
                                                                                continue;
                                                                        if(d_info[d_ps[id*81+70]*3+1]+d_primer[turn[ID_thread*8+2]*10+1]>d_info[d_ps[id*81+66]*3+1])
                                                                                continue;
                                                                        d_ps[id*81+71]=1;
                                                                        break;
                                                                }
                                                                if(d_ps[id*81+71]==0)
                                                                        continue;
                                                        }
                                                //LB
                                                        if(turn[ID_thread*8+5]!=-1)
                                                        {
                                                                d_ps[id*81+71]=0;
                                                                for(d_ps[id*81+70]=d_ps[id*81+80];d_ps[id*81+70]=d_primer[turn[ID_thread*8+5]*10+4];d_ps[id*81+70]++)
                                                                {
                                                                        if(d_info[d_ps[id*81+70]*3]<d_ps[id*81+72])
									{
										d_ps[id*81+80]++;
                                                                                continue;
									}
									if(d_info[d_ps[id*81+70]*3]>d_ps[id*81+72])
										break;
                                                                        if((d_info[d_ps[id*81+70]*3+2]&1)!=1)
                                                                                continue;
									if(d_info[d_ps[id*81+70]*3+1]<d_info[d_ps[id*81+64]*3+1])
									{
                                                                                d_ps[id*81+80]++;
                                                                                continue;
                                                                        }
									if(d_info[d_ps[id*81+70]*3+1]-d_info[d_ps[id*81+64]*3+1]>300)
										break;
                                                                        if(d_info[d_ps[id*81+67]*3+1]+d_primer[turn[ID_thread*8+4]*10+1]>d_info[d_ps[id*81+70]*3+1])
                                                                                continue;
                                                                        if(d_info[d_ps[id*81+70]*3+1]+d_primer[turn[ID_thread*8+5]*10+1]>d_info[d_ps[id*81+68]*3+1])
                                                                                continue;
                                                                        d_ps[id*81+71]=1;
                                                                        break;
                                                                }
                                                                if(d_ps[id*81+71]==0)
                                                                        continue;
                                                        }
                                                        d_result[(8+const_int[7])*turn[ID_thread*8]+8+d_ps[id*81+72]]=1;
                                                }
                                        }
                                }
                        }
                }
        }
//minus
	d_ps[id*81+74]=d_primer[turn[ID_thread*8+1]*10+3]; //F2
        d_ps[id*81+75]=d_primer[turn[ID_thread*8+3]*10+3];//F1c
        d_ps[id*81+76]=d_primer[turn[ID_thread*8+4]*10+3];//B1c
        d_ps[id*81+77]=d_primer[turn[ID_thread*8+6]*10+3];//B2
        d_ps[id*81+78]=d_primer[turn[ID_thread*8+7]*10+3];//B3
        if(turn[ID_thread*8+2]!=-1)
                d_ps[id*81+79]=d_primer[turn[ID_thread*8+2]*10+3];//LF
        if(turn[ID_thread*8+5]!=-1)
                d_ps[id*81+80]=d_primer[turn[ID_thread*8+5]*10+3];
	for(d_ps[id*81+64]=d_primer[turn[ID_thread*8]*10+3];d_ps[id*81+64]<d_primer[turn[ID_thread*8]*10+4];d_ps[id*81+64]++) //F3
        {
                if((d_info[d_ps[id*81+64]*3+2]&2)!=2)
                        continue;
                d_ps[id*81+72]=d_info[d_ps[id*81+64]*3];
                if(d_result[(8+const_int[7])*turn[ID_thread*8]+8+d_ps[id*81+72]]!=0)
                        continue;
                for(d_ps[id*81+65]=d_ps[id*81+74];d_ps[id*81+65]<d_primer[turn[ID_thread*8+1]*10+4];d_ps[id*81+65]++)//F2
                {
			if(d_info[d_ps[id*81+65]*3]<d_ps[id*81+72])
                        {
                                d_ps[id*81+74]++;
                                continue;
                        }
                        if(d_info[d_ps[id*81+65]*3]>d_ps[id*81+72])
                                break;
                        if((d_info[d_ps[id*81+65]*3+2]&2)!=2)
                                continue;
			if(d_info[d_ps[id*81+65]*3+1]-d_info[d_ps[id*81+64]*3+1]<-300)
                        {
                                d_ps[id*81+74]++;
                                continue;
                        }
                        if(d_info[d_ps[id*81+65]*3+1]>d_info[d_ps[id*81+64]*3+1])
                                break;
                        for(d_ps[id*81+66]=d_ps[id*81+75];d_ps[id*81+66]<d_primer[turn[ID_thread*8+3]*10+4];d_ps[id*81+66]++)//F1c
                        {
				if(d_info[d_ps[id*81+66]*3]<d_ps[id*81+72])
                                {
                                        d_ps[id*81+75]++;
                                        continue;
                                }
                                if(d_info[d_ps[id*81+66]*3]>d_ps[id*81+72])
                                        break;
                                if((d_info[d_ps[id*81+66]*3+2]&1)!=1)
                                        continue;
				if(d_info[d_ps[id*81+66]*3+1]-d_info[d_ps[id*81+64]*3+1]<-300)
                                {
                                        d_ps[id*81+75]++;
                                        continue;
                                }
                                if(d_info[d_ps[id*81+66]*3+1]>d_info[d_ps[id*81+64]*3+1])
                                        break;
                                for(d_ps[id*81+67]=d_ps[id*81+76];d_ps[id*81+67]<d_primer[turn[ID_thread*8+4]*10+4];d_ps[id*81+67]++)//B1c
                                {
					if(d_info[d_ps[id*81+67]*3]<d_ps[id*81+72])
                                        {
                                                d_ps[id*81+76]++;
                                                continue;
                                        }
                                        if(d_info[d_ps[id*81+67]*3]>d_ps[id*81+72])
                                                break;
                                        if((d_info[d_ps[id*81+67]*3+2]&2)!=2)
                                                continue;
					if(d_info[d_ps[id*81+67]*3+1]-d_info[d_ps[id*81+64]*3+1]<-300)
                                        {
                                                d_ps[id*81+76]++;
                                                continue;
                                        }
                                        if(d_info[d_ps[id*81+67]*3+1]>d_info[d_ps[id*81+64]*3+1])
                                                break;
                                        for(d_ps[id*81+68]=d_ps[id*81+77];d_ps[id*81+68]<d_primer[turn[ID_thread*8+6]*10+4];d_ps[id*81+68]++)//B2
                                        {
						if(d_info[d_ps[id*81+68]*3]<d_ps[id*81+72])
                                                {
                                                        d_ps[id*81+77]++;
                                                        continue;
                                                }
                                                if(d_info[d_ps[id*81+68]*3]>d_ps[id*81+72])
                                                        break;
                                                if((d_info[d_ps[id*81+68]*3+2]&1)!=1)
                                                        continue;
						if(d_info[d_ps[id*81+68]*3+1]-d_info[d_ps[id*81+64]*3+1]<-300)
                                                {
                                                        d_ps[id*81+77]++;
                                                        continue;
                                                }
                                                if(d_info[d_ps[id*81+68]*3+1]>d_info[d_ps[id*81+64]*3+1])
                                                        break;
                                                for(d_ps[id*81+69]=d_ps[id*81+78];d_ps[id*81+69]<d_primer[turn[ID_thread*8+7]*10+4];d_ps[id*81+69]++)
                                                {
							if(d_info[d_ps[id*81+69]*3]<d_ps[id*81+72])
                                                        {
                                                                d_ps[id*81+78]++;
                                                                continue;
                                                        }
                                                        if(d_info[d_ps[id*81+69]*3]>d_ps[id*81+72])
                                                                break;
                                                        if((d_info[d_ps[id*81+69]*3+2]&1)!=1)
                                                                continue;
							if(d_info[d_ps[id*81+69]*3+1]-d_info[d_ps[id*81+64]*3+1]<-300)
                                                        {
                                                                d_ps[id*81+78]++;
                                                                continue;
                                                        }
                                                        if(d_info[d_ps[id*81+69]*3+1]>d_info[d_ps[id*81+64]*3+1])
                                                                break;
                                                //F3-F2 
                                                        d_ps[id*81+71]=d_info[d_ps[id*81+64]*3+1]-(d_info[d_ps[id*81+65]*3+1]+d_primer[turn[ID_thread*8+1]*10+1]-1)-1;
                                                        if(d_ps[id*81+71]<0)
                                                                continue;
                                                        if(d_ps[id*81+71]>20)
                                                                continue;
                                                //F2-F1c
                                                        d_ps[id*81+71]=(d_info[d_ps[id*81+65]*3+1]+d_primer[turn[ID_thread*8+1]*10+1]-1)-(d_info[d_ps[id*81+66]*3+1]+d_primer[turn[ID_thread*8+3]*10+1]-1)-1;
                                                        if(d_ps[id*81+71]<40)
                                                                continue;
                                                        if(d_ps[id*81+71]>60)
                                                                continue;
                                                //F1c-B1c
                                                        d_ps[id*81+71]=d_info[d_ps[id*81+66]*3+1]-(d_info[d_ps[id*81+67]*3+1]+d_primer[turn[ID_thread*8+4]*10+1]-1)-1;
                                                        if(d_ps[id*81+71]<0)
                                                                continue;
                                                //B1c-B2
                                                        d_ps[id*81+71]=d_info[d_ps[id*81+67]*3+1]-d_info[d_ps[id*81+68]*3+1]-1;
                                                        if(d_ps[id*81+71]<40)
                                                                continue;
                                                        if(d_ps[id*81+71]>60)
                                                                continue;
                                                //F2-B2
                                                        d_ps[id*81+71]=d_info[d_ps[id*81+65]*3+1]+d_primer[turn[ID_thread*8+1]*10+1]-1-d_info[d_ps[id*81+68]*3+1]-1;
                                                        if(d_ps[id*81+71]<120)
                                                                continue;
                                                        if(d_ps[id*81+71]>180)
                                                                continue;
                                                //B2-B3
                                                        d_ps[id*81+71]=d_info[d_ps[id*81+68]*3+1]-(d_info[d_ps[id*81+69]*3+1]+d_primer[turn[ID_thread*8+7]*10+1]-1)-1;
                                                        if(d_ps[id*81+71]<0)
                                                                continue;
                                                        if(d_ps[id*81+71]>20)
                                                                continue;
                                                //LF
                                                        if(turn[ID_thread*8+2]!=-1)
                                                        {
                                                                d_ps[id*81+71]=0;
                                                                for(d_ps[id*81+70]=d_ps[id*81+79];d_ps[id*81+70]<d_primer[turn[ID_thread*8+2]*10+4];d_ps[id*81+70]++)
                                                                {
									if(d_info[d_ps[id*81+70]*3]<d_ps[id*81+72])
                                                                        {
                                                                                d_ps[id*81+79]++;
                                                                                continue;
                                                                        }
                                                                        if(d_info[d_ps[id*81+70]*3]>d_ps[id*81+72])
                                                                                break;
                                                                        if((d_info[d_ps[id*81+70]*3+2]&1)!=1)
                                                                                continue;
									if(d_info[d_ps[id*81+70]*3+1]-d_info[d_ps[id*81+64]*3+1]<-300)
                                                                        {
                                                                                d_ps[id*81+79]++;
                                                                                continue;
                                                                        }
                                                                        if(d_info[d_ps[id*81+70]*3+1]>d_info[d_ps[id*81+64]*3+1])
                                                                                break;
                                                                        if(d_info[d_ps[id*81+66]*3+1]+d_primer[turn[ID_thread*8+3]*10+1]>d_info[d_ps[id*81+70]*3+1])
                                                                                continue;
                                                                        if(d_info[d_ps[id*81+70]*3+1]+d_primer[turn[ID_thread*8+2]*10+1]>d_info[d_ps[id*81+65]*3+1])
                                                                                continue;
                                                                        d_ps[id*81+71]=1;
                                                                        break;
                                                                }
                                                                if(d_ps[id*81+71]==0)
                                                                        continue;
                                                        }
                                                //LB
                                                        if(turn[ID_thread*8+5]!=-1)
                                                        {
                                                                d_ps[id*81+71]=0;
                                                                for(d_ps[id*81+70]=d_ps[id*81+80];d_ps[id*81+70]=d_primer[turn[ID_thread*8+5]*10+4];d_ps[id*81+70]++)
                                                                {
									if(d_info[d_ps[id*81+70]*3]<d_ps[id*81+72])
                                                                        {
                                                                                d_ps[id*81+80]++;
                                                                                continue;
                                                                        }
                                                                        if(d_info[d_ps[id*81+70]*3]>d_ps[id*81+72])
                                                                                break;
                                                                        if((d_info[d_ps[id*81+70]*3+2]&2)!=2)
                                                                                continue;
									if(d_info[d_ps[id*81+70]*3+1]-d_info[d_ps[id*81+64]*3+1]<-300)
                                                                        {
                                                                                d_ps[id*81+80]++;
                                                                                continue;
                                                                        }
                                                                        if(d_info[d_ps[id*81+70]*3+1]>d_info[d_ps[id*81+64]*3+1])
                                                                                break;
                                                                        if(d_info[d_ps[id*81+68]*3+1]+d_primer[turn[ID_thread*8+6]*10+1]>d_info[d_ps[id*81+70]*3+1])
                                                                                continue;
                                                                        if(d_info[d_ps[id*81+70]*3+1]+d_primer[turn[ID_thread*8+5]*10+1]>d_info[d_ps[id*81+67]*3+1])
                                                                                continue;
                                                                        d_ps[id*81+71]=1;
                                                                        break;
                                                                }
                                                                if(d_ps[id*81+71]==0)
                                                                        continue;
                                                        }
							d_result[(8+const_int[7])*turn[ID_thread*8]+8+d_ps[id*81+72]]=1;
                                                }
                                        }
                                }
                        }
                }
        }
	d_ps[id*81+71]=0;
        for(d_ps[id*81+72]=0;d_ps[id*81+72]<const_int[7];d_ps[id*81+72]++)
        {
                d_ps[id*81+71]=d_ps[id*81+71]+d_result[(8+const_int[7])*turn[ID_thread*8]+8+d_ps[id*81+72]];
        }
}

__device__ void design_loop(int *d_primer,char *d_seq,int *d_info,int turn[],int ID_thread,int *d_result,double *d_DPT,int id,int *d_ps,char *d_numSeq,int GC[])
{
//LF and LB 
        d_ps[id*81+73]=0;
	turn[ID_thread*8+2]=d_primer[turn[ID_thread*8+1]*10+9];
        while(turn[ID_thread*8+2]<const_int[2]+const_int[0]+const_int[1])
        {
		if(turn[ID_thread*8+2]==-1)
			break;
		if(((d_primer[turn[ID_thread*8+2]*10+2]/10)&GC[ID_thread])==0)
		{
			turn[ID_thread*8+2]++;
			continue;
		}
                if(d_primer[turn[ID_thread*8+2]*10]+18>d_primer[turn[ID_thread*8+3]*10])
                        break;
                turn[ID_thread*8+5]=d_primer[turn[ID_thread*8+3]*10+9];
		if(turn[ID_thread*8+5]==-1||d_primer[turn[ID_thread*8+5]*10]+18>d_primer[turn[ID_thread*8+6]*10])
			break;
                while(turn[ID_thread*8+5]<const_int[2]+const_int[0]+const_int[1])
                {
			if(((d_primer[turn[ID_thread*8+5]*10+2]%10)&GC[ID_thread])==0)
			{
				turn[ID_thread*8+5]++;
				continue;
			}
                        if(d_primer[turn[ID_thread*8+5]*10]+18>d_primer[turn[ID_thread*8+6]*10])
                                break;
                //check_common
                        if(const_int[3])
                        {
                                check_common(d_primer,d_info,turn,ID_thread,id,d_result,d_ps);
                                if(d_ps[id*81+71]<const_int[8])
                                {
                                        turn[ID_thread*8+5]++;
                                        continue;
                                }
                        }
                //check_structure
                        if(const_int[6])
                        {
                                check_structure(d_seq,d_primer,turn,ID_thread,id,d_DPT,d_ps,d_numSeq,GC);
                                if(d_ps[id*81+66]==0)
                                {
                                        turn[ID_thread*8+5]++;
                                        continue;
                                }
                        }
                        d_ps[id*81+73]=1;
                        break;
                }
                if(d_ps[id*81+73]==1)
                        break;
                else
                        turn[ID_thread*8+2]++;
        }
        if(d_ps[id*81+73]==1)
                return;
//only LF
        turn[ID_thread*8+2]=d_primer[turn[ID_thread*8+1]*10+9];
	turn[ID_thread*8+5]=-1;
        while(turn[ID_thread*8+2]<const_int[2]+const_int[1]+const_int[0])
        {
		if(turn[ID_thread*8+2]==-1)
			break;
                if(d_primer[turn[ID_thread*8+2]*10]+18>d_primer[turn[ID_thread*8+3]*10])
                        break;
		if(((d_primer[4*turn[ID_thread*8+2]+2]/10)&GC[ID_thread])==0)
		{
			turn[ID_thread*8+2]++;
			continue;
		}
        //check_common
                if(const_int[3])
                {
                        check_common(d_primer,d_info,turn,ID_thread,id,d_result,d_ps);
                        if(d_ps[id*81+71]<const_int[8])
                        {
                                turn[ID_thread*8+2]++;
                                continue;
                        }
                }
        //check_structure
                if(const_int[6])
                {
                        check_structure(d_seq,d_primer,turn,ID_thread,id,d_DPT,d_ps,d_numSeq,GC);
			if(d_ps[id*81+66]==0)
                        {
                                turn[ID_thread*8+2]++;
                                continue;
                        }
                }
                d_ps[id*81+73]=1;
                break;
        }
        if(d_ps[id*81+73]==1)
                return;
//only LB
        turn[ID_thread*8+5]=d_primer[turn[ID_thread*8+3]*10+9];
	turn[ID_thread*8+2]=-1;
        while(turn[ID_thread*8+5]<const_int[2]+const_int[0]+const_int[1])
        {
		if(turn[ID_thread*8+5]==-1)
			break;
                if(d_primer[turn[ID_thread*8+5]*10]+18>d_primer[turn[ID_thread*8+6]*10])
                        break;
		if(((d_primer[turn[ID_thread*8+5]*4+2]%10)&GC[ID_thread])==0)
		{
			turn[ID_thread*8+5]++;
			continue;
		}
        //check_common
                if(const_int[3])
                {
                        check_common(d_primer,d_info,turn,ID_thread,id,d_result,d_ps);
                        if(d_ps[id*81+71]<const_int[8])
                        {
                                turn[ID_thread*8+5]++;
                                continue;
                        }
                }
        //check_structure
                if(const_int[6])
                {
                        check_structure(d_seq,d_primer,turn,ID_thread,id,d_DPT,d_ps,d_numSeq,GC);
			if(d_ps[id*81+66]==0)
                        {
                                turn[ID_thread*8+5]++;
                                continue;
                        }
                }
                d_ps[id*81+73]=1;
                break;
        }
}

//caculate
__global__ void LAMP(char *d_seq,int *d_primer,int *d_info,int *d_result,double *d_DPT,int *d_ps,char *d_numSeq,float *d_Tm)
//const_int: 0:numS,1:numL,2:numLp,3:common_flag,4:special_flag,5:loop_flag,6:secondary_flag,7:common_num,8:this turn common_num,; 10:expect
{
	int id=blockDim.x*blockIdx.x+threadIdx.x;
	__shared__ int turn[8192];
	__shared__ int GC[1024];

	while(id<const_int[0])
	{
		d_result[id*(8+const_int[7])]=-1;//not LAMP, as a flag
//check add by F3'pos
		if(d_primer[id*10+2]%10==0)
		{
			id=id+blockDim.x*gridDim.x;
			continue;
		}
	//combine
		turn[threadIdx.x*8]=id; //one thread, one F3
		d_ps[id*81+73]=0;
		for(turn[threadIdx.x*8+1]=d_primer[id*10+7];turn[threadIdx.x*8+1]<const_int[0];turn[threadIdx.x*8+1]++) //F2
		{
			if(turn[threadIdx.x*8+1]==-1)
				break;
			d_result[id*(8+const_int[7])+1]=(d_primer[id*10+2]%10)&(d_primer[turn[threadIdx.x*8+1]*10+2]%10);
			if(d_result[id*(8+const_int[7])+1]==0)
				continue;
			if(d_primer[turn[threadIdx.x*8+1]*10]-(d_primer[turn[threadIdx.x*8]*10]+d_primer[turn[threadIdx.x*8]*10+1])>20)
				break;
			for(turn[threadIdx.x*8+3]=d_primer[turn[threadIdx.x*8+1]*10+8];turn[threadIdx.x*8+3]<const_int[1]+const_int[0];turn[threadIdx.x*8+3]++) //F1c
			{
				if(turn[threadIdx.x*8+3]==-1)
					break;
				d_result[id*(8+const_int[7])+2]=d_result[id*(8+const_int[7])+1]&(d_primer[turn[threadIdx.x*8+3]*10+2]/10);
				if(d_result[id*(8+const_int[7])+2]==0)
					continue;
			//Tm
				if(d_Tm[turn[threadIdx.x*8+3]]-d_Tm[id]<3)
					continue;
				if(d_Tm[turn[threadIdx.x*8+3]]-d_Tm[turn[threadIdx.x*8+1]]<3)
					continue;
				if(d_primer[turn[threadIdx.x*8+3]*10]-d_primer[turn[threadIdx.x*8+1]*10]-1<40)
					continue;
                                if(d_primer[turn[threadIdx.x*8+3]*10]-d_primer[turn[threadIdx.x*8+1]*10]-1>60)
                                	break;
                                for(turn[threadIdx.x*8+4]=d_primer[turn[threadIdx.x*8+3]*10+7];turn[threadIdx.x*8+4]<const_int[1]+const_int[0];turn[threadIdx.x*8+4]++)   //B1c
                                {
                                        if(turn[threadIdx.x*8+4]==-1)
                                        	break;
					d_result[id*(8+const_int[7])+3]=d_result[id*(8+const_int[7])+2]&(d_primer[turn[threadIdx.x*8+4]*10+2]%10);
					if(d_result[id*(8+const_int[7])+3]==0)
						continue;
				//Tm
					if(d_Tm[turn[threadIdx.x*8+4]]-d_Tm[id]<3)
						continue;
					if(d_Tm[turn[threadIdx.x*8+4]]-d_Tm[turn[threadIdx.x*8+1]]<3)
						continue;

                                        if(d_primer[turn[threadIdx.x*8+4]*10]-d_primer[turn[threadIdx.x*8+3]*10]>85)
                                        	break;
                                        for(turn[threadIdx.x*8+6]=d_primer[turn[threadIdx.x*8+4]*10+8];turn[threadIdx.x*8+6]<const_int[0];turn[threadIdx.x*8+6]++)   //B2
                                        {
                                                if(turn[threadIdx.x*8+6]==-1)
                                                	break;
						d_result[id*(8+const_int[7])+4]=d_result[id*(8+const_int[7])+3]&(d_primer[turn[threadIdx.x*8+6]*10+2]/10);
						if(d_result[id*(8+const_int[7])+4]==0)
							continue;
					//Tm
						if(d_Tm[turn[threadIdx.x*8+4]]-d_Tm[turn[threadIdx.x*8+6]]<3)
							continue;
						if(d_Tm[turn[threadIdx.x*8+3]]-d_Tm[turn[threadIdx.x*8+6]]<3)
							continue;
                                                if((d_primer[turn[threadIdx.x*8+6]*10]+d_primer[turn[threadIdx.x*8+6]*10+1]-1)-(d_primer[turn[threadIdx.x*8+4]*10]+d_primer[turn[threadIdx.x*8+4]*10+1])<40)
	                                                continue;
                                                if((d_primer[turn[threadIdx.x*8+6]*10]+d_primer[turn[threadIdx.x*8+6]*10+1]-1)-(d_primer[turn[threadIdx.x*8+4]*10]+d_primer[turn[threadIdx.x*8+4]*10+1])>60)
                                                	break;
                                                if(d_primer[turn[threadIdx.x*8+6]*10]+d_primer[turn[threadIdx.x*8+6]*10+1]-1-d_primer[turn[threadIdx.x*8+1]*10]-1<120)
                                                	continue;
                                                if(d_primer[turn[threadIdx.x*8+6]*10]+d_primer[turn[threadIdx.x*8+6]*10+1]-1-d_primer[turn[threadIdx.x*8+1]*10]-1>180)
                                                	break;
						if(const_int[5]&&(d_primer[turn[threadIdx.x*8+1]*10+9]==-1||(d_primer[d_primer[turn[threadIdx.x*8+1]*10+9]*10]+18>d_primer[turn[threadIdx.x*8+3]*10]))&&(d_primer[turn[threadIdx.x*8+4]*10+9]==-1||(d_primer[d_primer[turn[threadIdx.x*8+4]*10+9]*10]+18>d_primer[turn[threadIdx.x*8+6]*10])))
							continue;
						for(turn[threadIdx.x*8+7]=d_primer[turn[threadIdx.x*8+6]*10+7];turn[threadIdx.x*8+7]<const_int[0];turn[threadIdx.x*8+7]++)  //B3
						{
							if(turn[threadIdx.x*8+7]==-1)
								break;
							d_result[id*(8+const_int[7])+5]=d_result[id*(8+const_int[7])+4]&(d_primer[turn[threadIdx.x*8+7]*10+2]/10);
				                        if(d_result[id*(8+const_int[7])+5]==0)
				                                continue;
						//Tm
							if(d_Tm[turn[threadIdx.x*8+4]]-d_Tm[turn[threadIdx.x*8+7]]<3)
								continue;
							if(d_Tm[turn[threadIdx.x*8+3]]-d_Tm[turn[threadIdx.x*8+7]]<3)
								continue;

							if(d_primer[turn[threadIdx.x*8+7]*10]<d_primer[turn[threadIdx.x*8+6]*10]+d_primer[turn[threadIdx.x*8+6]*10+1])
								continue;
							if(d_primer[turn[threadIdx.x*8+7]*10]-(d_primer[turn[threadIdx.x*8+6]*10]+d_primer[turn[threadIdx.x*8+6]*10+1])>20)
								break;
							GC[threadIdx.x]=check_gc(d_seq,d_primer[turn[threadIdx.x*8]*10],(d_primer[turn[threadIdx.x*8+7]*10]+d_primer[turn[threadIdx.x*8+7]*10+1]),d_ps,id);
				                        if((GC[threadIdx.x]&d_result[id*(8+const_int[7])+5])==0)
				                                continue;

							if(const_int[4]!=0)
							{
								check_uniq(d_primer,d_info,turn,threadIdx.x,id,d_ps);
								if(d_ps[id*81+71]==0)
								{
									if(const_int[19])
									{
										d_ps[id*81+73]=1;
										break;
									}
									else
										continue;
								}
							}

							turn[threadIdx.x*8+2]=-1;
							turn[threadIdx.x*8+5]=-1; //loop
							if(const_int[3])
							{
								check_common(d_primer,d_info,turn,threadIdx.x,id,d_result,d_ps);
								if(d_ps[id*81+71]<const_int[8])
								{
                                                                        if(const_int[19])
                                                                        {
                                                                                d_ps[id*81+73]=1;
                                                                                break;
                                                                        }
                                                                        else
                                                                                continue;
                                                                }
							}
							if(const_int[6])
							{
								check_structure(d_seq,d_primer,turn,threadIdx.x,id,d_DPT,d_ps,d_numSeq,GC);
								if(d_ps[id*81+66]==0)
								{
                                                                        if(const_int[19])
                                                                        {
                                                                                d_ps[id*81+73]=1;
                                                                                break;
                                                                        }
                                                                        else
                                                                                continue;
                                                                }
							}
							if(const_int[5])
							{
								design_loop(d_primer,d_seq,d_info,turn,threadIdx.x,d_result,d_DPT,id,d_ps,d_numSeq,GC);
								if(d_ps[id*81+73]==0)
								{
                                                                        if(const_int[19])
                                                                        {
                                                                                d_ps[id*81+73]=1;
                                                                                break;
                                                                        }
                                                                        else
                                                                                continue;
                                                                }
							}

							d_result[id*(8+const_int[7])]=turn[threadIdx.x*8];
							d_result[id*(8+const_int[7])+1]=turn[threadIdx.x*8+1];
							d_result[id*(8+const_int[7])+2]=turn[threadIdx.x*8+2];
							d_result[id*(8+const_int[7])+3]=turn[threadIdx.x*8+3];
							d_result[id*(8+const_int[7])+4]=turn[threadIdx.x*8+4];
							d_result[id*(8+const_int[7])+5]=turn[threadIdx.x*8+5];
							d_result[id*(8+const_int[7])+6]=turn[threadIdx.x*8+6];
							d_result[id*(8+const_int[7])+7]=turn[threadIdx.x*8+7];
							d_ps[id*81+73]=1;
							break;
						}//B2
						if(d_ps[id*81+73]!=0)
                                                        break;
					}//B1c
					if(d_ps[id*81+73]!=0)
						break;
				}//F1c
				if(d_ps[id*81+73]!=0)
					break;
			}//F2
			if(d_ps[id*81+73]!=0)
				break;
		}//B3
		id=id+blockDim.x*gridDim.x;
	}
	__syncthreads();
}

void usage()
{
	printf("USAGE:\n");
        printf("  LAMP_GPU  -in <sinlge_primers_file>  -ref <ref_genome> -out <LAMP_primer_sets> [options]*\n\n");
        printf("ARGUMENTS:\n");
        printf("  -in <single_primers_file>\n");
        printf("    the file name of candidate single primer regions, files are generated from Single program\n");
        printf("  -ref <ref_genome>\n");
        printf("    reference genome, fasta formate\n");
        printf("  -dir <directory>\n");
        printf("    the directory for output file\n");
        printf("    default: current directory\n");
        printf("  -out <LAMP_primer_sets>\n");
        printf("    output successfully designed LAMP primer sets\n");
        printf("  -num <int>\n");
        printf("    the expected output number of LAMP primer sets\n");
        printf("    default: 10\n");
        printf("  -loop\n");
        printf("    design LAMP primer sets with loop primers\n");
        printf("  -common\n");
        printf("    design common LAMP primer sets those can amplify more than one target genomes\n");
        printf("  -specific\n");
        printf("    design specific LAMP primer sets those can't amplify any background genomes\n");
        printf("  -check <int>\n");
        printf("    check primers' tendency of binding to another in one LAMP primer set or not\n");
        printf("    0: don't check; other values: check\n");
        printf("    default: 1\n");
        printf("  -par <par_directory>\n");
        printf("    parameter files under the directory are used to check primers' binding tendency\n");
        printf("    default: GLAPD/Par/\n");
	printf("  -thread <int>\n");
	printf("    the number of threads in one block, max is 1024\n");
	printf("    default: 256\n");
	printf("  -threshold <int>\n");
	printf("    the number of designing LAMP primer sets simultaneously\n");
	printf("    default: 51200\n");
        printf("  -fast\n");
        printf("    fast mode to design LAMP primer sets, in this mode GLAPD may lost some right results\n");
        printf("  -h/-help\n");
        printf("    print usage\n");
}

struct INFO *read_list(char *path,int common_num[])  
{
        char *in,name[301];
        int turn,i,size;
        struct INFO *new_primer,*p_primer,*head;
        FILE *fp;

        i=strlen(path);
        in=(char *)malloc(i+20);              
        memset(in,'\0',i+20);
        strcpy(in,path);
        strcat(in,"-common_list.txt");
        if(access(in,0)==-1)  
        {
                printf("Error! Don't have the %s file!\n",in);
                exit(1);           
        }
        fp=fopen(in,"r");
        if(fp==NULL)
        {          
                printf("Error: can't open the %s file!\n",in);
                exit(1);
        }

        size=sizeof(struct INFO);
        i=0;
        memset(name,'\0',301);
        while(fscanf(fp,"%s\t%d\n",name,&turn)!=EOF)
        {
                new_primer=(struct INFO *)malloc(size);
                new_primer->turn=turn;
                strcpy(new_primer->name,name);
                new_primer->next=NULL;

                if(i==0)
                {
                        head=new_primer;
                        p_primer=new_primer;
                        i++;
                }
                else
                {
                        p_primer->next=new_primer;
                        p_primer=new_primer;
                }
                memset(name,'\0',301);
        }
        fclose(fp);
        common_num[0]=turn;
        free(in);
        return head;
}

int main(int argc,char **argv)
{
	int i,j,flag[12],expect,circle,have,common_num[1],num[11],max_loop,min_loop,count[3],block,thread,threshold,before;
	char *output,*prefix,*store_path,*path_fa,*inner,*outer,*loop,*par_path,*temp,*seq,*d_seq,primer[26],*d_numSeq;
	FILE *fp;
	struct Primer *headL,*headS,*headLoop,*tempL,*tempS,*tempLoop,*storeL,*storeS,*storeLoop; 
	struct Node *p_node,*p_temp;
	struct INFO *headList,*p_list;
	time_t start,end,begin;
	double *H_parameter;	
	cudaDeviceProp prop;
	int *d_primer,*d_info,*d_result;
	int *h_primer,*h_info,*h_result,h_int[20],*h_pos,*d_ps;
	double *d_DPT;
	float *h_Tm,*d_Tm;
	
	expect=10; //default output max 10 LAMP primers
	thread=256;
	threshold=51200;

	start=time(NULL);
	begin=start;
/////read the parameters
        for(i=0;i<12;i++)
                flag[i]=0;
	for(i=0;i<10;i++)
		h_int[i]=0;
	h_int[19]=0;
        flag[7]=1;
        for(i=1;i<argc;)
        {
                if(strcmp(argv[i],"-in")==0)
                {
                        flag[0]=1;
                        if(i+1==argc)
                        {
                                printf("Error! The \"-in\" parameter is not completed.\n");
                                usage();
                                exit(1);
                        }
                        j=strlen(argv[i+1]);
                        prefix=(char *)malloc(j+1);
                        memset(prefix,'\0',j+1);
                        strcpy(prefix,argv[i+1]);
                        i=i+2;
                }
                else if(strcmp(argv[i],"-out")==0)
                {
                        flag[1]=1;
                        if(i+1==argc)
                        {
                                printf("Error! The \"-out\" parameter is not completed.\n");
                                usage();
                                exit(1);
                        }
                        j=strlen(argv[i+1]);
                        output=(char *)malloc(j+1);
                        memset(output,'\0',j+1);
                        strcpy(output,argv[i+1]);
                        i=i+2;
                }
                else if(strcmp(argv[i],"-dir")==0)
                {
                        flag[2]=1;
                        if(i+1==argc)
                        {
                                printf("Error! The \"-dir\" parameter is not completed.\n");
                                usage();
                                exit(1);
                        }
                        j=strlen(argv[i+1]);
                        if(argv[i+1][j-1]=='/')
                        {
                                store_path=(char *)malloc(j+1);
                                memset(store_path,'\0',j+1);
                                strcpy(store_path,argv[i+1]);
                        }
                        else
                        {
                                store_path=(char *)malloc(j+2);
                                memset(store_path,'\0',j+2);
                                strcpy(store_path,argv[i+1]);
                                store_path[j]='/';
                        }
                        i=i+2;
                }
                else if(strcmp(argv[i],"-ref")==0)
                {
                        flag[3]=1;
                        if(i+1==argc)
                        {
                                printf("Error! The \"-ref\" parameter is not completed.\n");
                                usage();
                                exit(1);
                        }
                        j=strlen(argv[i+1]);
                        path_fa=(char *)malloc(j+1);
                        memset(path_fa,'\0',j+1);
                        strcpy(path_fa,argv[i+1]);
                        i=i+2;
                }
                else if(strcmp(argv[i],"-num")==0)
                {
                        flag[4]=1;
                        if(i+1==argc)
                        {
                                printf("Error! The \"-tm\" parameter is not completed.\n");
                                usage();
                                exit(1);
                        }
                        expect=atoi(argv[i+1]);
                        i=i+2;
                }
		else if(strcmp(argv[i],"-thread")==0)
                {
                        thread=atoi(argv[i+1]);
                        i=i+2;
                }
		else if(strcmp(argv[i],"-threshold")==0)
                {
                        threshold=atoi(argv[i+1]);
                        i=i+2;                 
                }
                else if(strcmp(argv[i],"-loop")==0) 
                {
                        flag[10]=1;
			h_int[5]=1;
                        i++;
                }
		else if(strcmp(argv[i],"-fast")==0)
		{
			h_int[19]=1;
			i++;
		}
                else if(strcmp(argv[i],"-h")==0 || strcmp(argv[i],"-help")==0)
                {
                        usage();
                        exit(1);
                }
                else if(strcmp(argv[i],"-check")==0)
                {
                        if(i+1==argc)
                        {
                                printf("Error! The \"-check\" parameter is not completed.\n");
                                usage();
                                exit(1);
                        }
                        flag[7]=atoi(argv[i+1]);
                        i=i+2;
                }
                else if(strcmp(argv[i],"-par")==0)
                {
                        flag[11]=1;
                        if(i+1==argc)
                        {
                                printf("Error! The \"-par\" parameter is not completed.\n");
                                usage();
                                exit(1);
                        }
                        j=strlen(argv[i+1]);
                        if(argv[i+1][j-1]=='/')
                        {
                                par_path=(char *)malloc(j+1);
                                strcpy(par_path,argv[i+1]);
                                par_path[j]='\0';
                        }
                        else
                        {
                                par_path=(char *)malloc(j+2);
                                strcpy(par_path,argv[i+1]);
                                par_path[j]='/';
                                par_path[j+1]='\0';
                        }
                        i=i+2;
                }
                else if(strcmp(argv[i],"-common")==0)
                {
                        flag[5]=1;
			h_int[3]=1;
                        i++;
                }
                else if(strcmp(argv[i],"-specific")==0)
                {
                        flag[6]=1;
			h_int[4]=1;
                        i++;
                }
                else
                {
                        printf("Warning! The parameter of %s is invalid.\n\n",argv[i]);
                        i++;
                }
        }
//check parameters
        if(flag[0]==0)
        {
                printf("Error! Users must supply the name of candidate single primers file with -in!\n");
                usage();
                exit(1);
        }
        if(flag[1]==0)
        {
                printf("Error! Users must supply the name of output file with -out!\n");
                usage();
                exit(1);
        }
        if(flag[3]==0)
        {
                printf("Error! Users must supply the reference sequence file with -ref!\n");
                usage();
                exit(1);
        }
//prepare
	if(flag[7]!=0)
		h_int[6]=1;
	else
		h_int[6]=0;
	h_int[9]=flag[8];
	h_int[10]=expect;
        if(flag[2]==0)
        {
                temp=(char *)malloc(4096);
                memset(temp,'\0',4096);
                getcwd(temp,4096);
                j=strlen(temp);
                store_path=(char *)malloc(j+2);
                memset(store_path,'\0',j+2);
                strcpy(store_path,temp);
                free(temp);
                store_path[j]='/';
        }
//secondary
        if(flag[7]&&flag[11]==0)
        {
                temp=(char *)malloc(4096);
                memset(temp,'\0',4096);
                getcwd(temp,4096);
                j=strlen(temp);
                par_path=(char *)malloc(j+10);
                memset(par_path,'\0',j+10);
                strcpy(par_path,temp);
                free(temp);

                j--;
                while(par_path[j]!='/'&&j>=0)
                {
                        par_path[j]='\0';
                        j--;
                }
                strcat(par_path,"Par/");
        }

        if(flag[7])
        {
                H_parameter=(double *)malloc(5730*sizeof(double));
                memset(H_parameter,'\0',5730*sizeof(double));

                getStack(par_path,H_parameter);
                getStackint2(par_path,H_parameter);
                getDangle(par_path,H_parameter);
                getLoop(par_path,H_parameter);
                getTstack(par_path,H_parameter);
                getTstack2(par_path,H_parameter);
                tableStartATS(6.9,H_parameter);
                tableStartATH(2200.0,H_parameter);
		cudaMemcpyToSymbol(parameter,H_parameter,5730*sizeof(double));
		free(H_parameter);

		h_int[11]=0; //F3,plus
		h_int[12]=0;
		h_int[13]=1; //LF,minus
		h_int[14]=1;//F1c
		h_int[15]=0;//B1c
		h_int[16]=0;
		h_int[17]=1;
		h_int[18]=1;
        }

//F3's pos 
	h_pos=(int *)malloc(expect*sizeof(int));
	for(i=0;i<expect;i++)
		h_pos[i]=-1;
//directory for single primers
        j=strlen(store_path)+strlen(prefix)+12;
        outer=(char *)malloc(j);
        memset(outer,'\0',j);
        strcpy(outer,store_path);

        inner=(char *)malloc(j);
        memset(inner,'\0',j);
        strcpy(inner,store_path);

        if(flag[10]==1)
        {
                loop=(char *)malloc(j);
                memset(loop,'\0',j);
                strcpy(loop,store_path);
        }
	strcat(outer,"Outer/");
	strcat(outer,prefix);
	strcat(inner,"Inner/");
	strcat(inner,prefix);
	if(flag[10]==1)
	{
		strcat(loop,"Loop/");
		strcat(loop,prefix);
        }

//reference sequence fa
        if(access(path_fa,0)==-1)
        {
                printf("Error! Don't have the %s file!\n",path_fa);
                exit(1);
        }
        i=file_size2(path_fa);
        i=i+100;
        temp=(char *)malloc(i*sizeof(char));
        memset(temp,'\0',i*sizeof(char));
        fp=fopen(path_fa,"r");
        if(fp==NULL)
        {
                printf("Error! Can't open the sequence file %s\n",path_fa);
                exit(1);
        }

        fread(temp,i*sizeof(char),1,fp);
        fclose(fp); 
        seq=(char *)malloc(i*sizeof(char));
        memset(seq,'\0',i*sizeof(char));
        
        j=0;
        i=0;
        while(temp[i]!='\n')
        {
                i++;
        }
        i++;
        while(temp[i]!='\0')
        {
                if(temp[i]=='\n')
                {
                        i++;
                        continue;
                }
                if(temp[i]=='a'||temp[i]=='A')
                        seq[j]='A';
                else if(temp[i]=='t'||temp[i]=='T')
                        seq[j]='T';
                else if(temp[i]=='c'||temp[i]=='C')
                        seq[j]='C';
                else if(temp[i]=='g'||temp[i]=='G')
                        seq[j]='G';
                else
                        seq[j]='N';
                i++;
                j++;
        }
        free(temp);

	num[0]=j; //the length of genome
	cudaMalloc((void **)&d_seq,num[0]);
	cudaMemset(d_seq,'\0',num[0]);
	cudaMemcpy(d_seq,seq,num[0],cudaMemcpyHostToDevice);
//common-list
        if(flag[5])
        {
                headList=read_list(inner,common_num);
                common_num[0]++;
        }
        else
                common_num[0]=1;
	h_int[7]=common_num[0];
//read parameters
        headS=read_par(outer,flag[5],flag[6]);
        headL=read_par(inner,flag[5],flag[6]);
        if(flag[10])
        {
                headLoop=read_par(loop,flag[5],0);
                tempLoop=headLoop;
                while(tempLoop->next!=NULL)
                        tempLoop=tempLoop->next;
                max_loop=tempLoop->pos;
		min_loop=headLoop->pos;
        }
//common statistics
        if(flag[5])
        {
                how_many(headL,common_num[0]);
		how_many(headS,common_num[0]);
                if(flag[10])
                        how_many(headLoop,common_num[0]);
        }
	cudaGetDeviceProperties(&prop,0); //read parameters
	if(thread>prop.maxThreadsPerBlock||thread>1024)
        {
                printf("Warning: the thread number is too big!\n");
                thread=prop.maxThreadsPerBlock;
                if(thread>1024)
                        thread=1024;
                printf("    Now the thread number is set to be %d.\n",thread);
        }
	fp=fopen(output,"w");
        if(fp==NULL)
        {
                printf("Error: can't create the %s file!\n",output);
                exit(1);
        }
	have=1;
	before=1;
	end=time(NULL);
        printf("It takes %0.1f seconds to prepare data.\n",difftime(end,start));
        start=time(NULL);
//LAMP-GPU
	for(circle=common_num[0];circle>=1;circle--)
	{
		if(have>expect)
			break;
		if(common_num[0]>1)
			printf("Running: amplify %d target genomes.\n",circle);
		storeL=headL;
		storeS=headS;
		while((storeL->pos<=storeS->pos+18)&&storeL!=NULL)
			storeL=storeL->next;
		if(flag[10])
		{
                        storeLoop=headLoop;
			while((storeLoop->pos<=storeS->pos+18)&&storeLoop!=NULL)
				storeLoop=storeLoop->next;
		}
		num[10]=0;
		while(storeS)
		{
			if(have>expect)
				break;
			if(num[10]==1)  //don't have enough primers
				break;
			for(i=1;i<9;i++)
			{
				num[i]=0;
			}
		//statistics	
			tempL=storeL;
			tempS=storeS;
			if(flag[10])
				tempLoop=storeLoop;
			while(tempS&&num[2]<threshold)
			{
				if(flag[10]&&(tempS->pos+200)<min_loop)
					continue;
				if(flag[10]&&(tempS->pos-200)>max_loop)
					break;
				if(tempS->total<circle)
				{
					tempS=tempS->next;
					continue;
				}
				while(tempL&&(tempL->pos<tempS->pos))
				{
					if(tempL->total<circle)
					{
						tempL=tempL->next;
						continue;
					}
					num[1]++;
					if(flag[5])
						num[3]=num[3]+tempL->total_common;
					if(flag[6])
						num[4]=num[4]+tempL->total_special;
					tempL=tempL->next;
				}
				
				while(flag[10]&&tempLoop&&(tempLoop->pos<tempS->pos))
                                {
                                        if(tempLoop->total<circle)
                                        {
                                                tempLoop=tempLoop->next;
                                                continue;
                                        }
                                        num[7]++;
                                        if(flag[5])
                                                num[8]=num[8]+tempLoop->total_common;
                                        tempLoop=tempLoop->next;
                                }

				num[2]++;
				if(flag[5])
					num[5]=num[5]+tempS->total_common;
				if(flag[6])
					num[6]=num[6]+tempS->total_special;
				tempS=tempS->next;
			}
			if(num[2]<4||num[1]<2||(flag[10]&&num[7]<1)) //don't have enough primers
			{
				num[10]=1;
				break;
			}
			if(tempS==NULL)  //check all primers
				num[10]=1;	
		//malloc
			h_primer=(int *)malloc(10*(num[2]+num[1]+num[7])*sizeof(int));
			cudaMalloc((void **)&d_primer,10*(num[2]+num[1]+num[7])*sizeof(int));
			h_Tm=(float *)malloc((num[2]+num[1]+num[7])*sizeof(float));
			cudaMalloc((void **)&d_Tm,(num[2]+num[1]+num[7])*sizeof(float));
			if(flag[5]||flag[6])
			{
				h_info=(int *)malloc(3*(num[5]+num[3]+num[8]+num[6]+num[4])*sizeof(int));
				cudaMalloc((void **)&d_info,3*(num[5]+num[3]+num[8]+num[6]+num[4])*sizeof(int));
			}
		
			tempS=storeS;
			for(i=0;i<2;i++)
				count[i]=0;
			while(count[0]<num[2])
			{
				if(tempS->total<circle)
				{
					tempS=tempS->next;
					continue;
				}
		//primer info
				h_primer[10*count[0]]=tempS->pos;
				h_primer[10*count[0]+1]=tempS->len;
				h_primer[10*count[0]+2]=tempS->strand;
				h_Tm[count[0]]=tempS->Tm;
		//common
				if(flag[5])
				{
					h_primer[10*count[0]+3]=count[1];
					if(tempS->total_common==0)
						h_primer[10*count[0]+4]=-1;
					else
					{
						p_node=tempS->common;
						while(p_node)
						{
							h_info[3*count[1]]=p_node->gi;
							h_info[3*count[1]+1]=p_node->pos;
							h_info[3*count[1]+2]=p_node->strand; 
							count[1]++;
							p_node=p_node->next;
						}
						h_primer[10*count[0]+4]=count[1];
					}
				}
			//special
				if(flag[6])
				{
					h_primer[10*count[0]+5]=count[1];
                	        	if(tempS->total_special==0)
                	        	        h_primer[10*count[0]+6]=-1;
                	        	else
                	        	{
                	        	        p_node=tempS->special;
                	        	        while(p_node)
                	        	        {
                	        	                h_info[3*count[1]]=p_node->gi;
                	        	                h_info[3*count[1]+1]=p_node->pos;
                	        	                h_info[3*count[1]+2]=p_node->strand;
                	        	                count[1]++;
                	        	                p_node=p_node->next;
                	        	        }
                	        	        h_primer[10*count[0]+6]=count[1];
                	        	}
				}
				count[0]++;
				tempS=tempS->next;
			}
			h_int[0]=num[2];

		//large primer
        	        tempL=storeL;
        	        while(count[0]<num[1]+num[2])
        	        {
				if(tempL->total<circle)
				{
					tempL=tempL->next;
					continue;
				}
                	//primer info
                	        h_primer[10*count[0]]=tempL->pos;
                	        h_primer[10*count[0]+1]=tempL->len;
                	        h_primer[10*count[0]+2]=tempL->strand;
				h_Tm[count[0]]=tempL->Tm;
                	//common
				if(flag[5])
				{
                	        	h_primer[10*count[0]+3]=count[1];
                	        	if(tempL->total_common==0)
                	        	        h_primer[10*count[0]+4]=-1;
                	        	else
                	        	{
                	        	        p_node=tempL->common;
                	        	        while(p_node)
                	        	        {
                	        	                h_info[3*count[1]]=p_node->gi;
                	        	                h_info[3*count[1]+1]=p_node->pos;
                	        	                h_info[3*count[1]+2]=p_node->strand; 
                        		                count[1]++;
                        		                p_node=p_node->next;
                        		        }
                        		        h_primer[10*count[0]+4]=count[1];
					}
                        	}
                	//special
				if(flag[6])
				{
                        		h_primer[10*count[0]+5]=count[1];
                        		if(tempL->total_special==0)
                        		        h_primer[10*count[0]+6]=-1;
                        		else
                        		{
                        		        p_node=tempL->special;
                        		        while(p_node)
                        		        {
                        		                h_info[3*count[1]]=p_node->gi;
                        		                h_info[3*count[1]+1]=p_node->pos;
                        		                h_info[3*count[1]+2]=p_node->strand;
                        		                count[1]++;
                        		                p_node=p_node->next;
                        		        }
                        		        h_primer[10*count[0]+6]=count[1];
                        		}
				}
                        	count[0]++;
                        	tempL=tempL->next;
                	}
			h_int[1]=num[1];

                //loop primer
			if(flag[10])
			{
	                        tempLoop=storeLoop;
	                        while(count[0]<num[7]+num[1]+num[2])
                        	{
                                	if(tempLoop->total<circle)
                                	{
                                        	tempLoop=tempLoop->next;
                                        	continue;
                                	}
                        	//primer info
                                	h_primer[10*count[0]]=tempLoop->pos;
                                	h_primer[10*count[0]+1]=tempLoop->len;
                                	h_primer[10*count[0]+2]=tempLoop->strand;
					h_Tm[count[0]]=tempLoop->Tm;
                        	//common
                                	if(flag[5])
                                	{
                                	        h_primer[10*count[0]+3]=count[1];
                                	        if(tempLoop->total_common==0)
                                	                h_primer[10*count[0]+4]=-1;
                                	        else
                                	        {
                                	                p_node=tempLoop->common;
                                	                while(p_node)
                                	                {
                                	                        h_info[3*count[1]]=p_node->gi;
                                	                        h_info[3*count[1]+1]=p_node->pos;
                                	                        h_info[3*count[1]+2]=p_node->strand; 
                                	                        count[1]++;
                                	                        p_node=p_node->next;
                                	                }
                                	                h_primer[10*count[0]+4]=count[1];
                                	        }
                                	}
	                                count[0]++;
	                                tempLoop=tempLoop->next;
	                        }
				h_int[2]=num[7];
                        }
	//run
			cudaMemcpy(d_primer,h_primer,10*count[0]*sizeof(int),cudaMemcpyHostToDevice);
			cudaMemcpy(d_Tm,h_Tm,count[0]*sizeof(float),cudaMemcpyHostToDevice);
			if(flag[5]||flag[6])
			{
				cudaMemcpy(d_info,h_info,3*count[1]*sizeof(int),cudaMemcpyHostToDevice);
				free(h_info);
			}
			if(count[0]%thread==0)
                                block=count[0]/thread;
                        else
                                block=(count[0]-count[0]%thread)/thread+1;

                        if(block>prop.maxGridSize[0])
                                block=prop.maxGridSize[0];
		//next primer
			if(num[2]%thread==0)
                                block=num[2]/thread;
                        else
                                block=(num[2]-num[2]%thread)/thread+1;

                        if(block>prop.maxGridSize[0])
                                block=prop.maxGridSize[0];
			printf("block is %d,thread is %d\n",block,thread);
			next_one<<<block,thread>>>(d_primer,0,num[2],0,num[2],7);//outer-self
			next_one<<<block,thread>>>(d_primer,num[2],(num[1]+num[2]),num[2],(num[1]+num[2]),7);//inner-self
			next_one<<<block,thread>>>(d_primer,0,num[2],num[2],(num[1]+num[2]),8);//outer_to_inner
        	        next_one<<<block,thread>>>(d_primer,num[2],(num[1]+num[2]),0,num[2],8);//inner_to_outer
			if(flag[10])
			{
				next_one<<<block,thread>>>(d_primer,(num[1]+num[2]),(num[1]+num[2]+num[7]),(num[1]+num[2]),(num[1]+num[2]+num[7]),7);//loop-self
                        	next_one<<<block,thread>>>(d_primer,0,num[2],(num[1]+num[2]),(num[1]+num[2]+num[7]),9);//outer_to_loop
                        	next_one<<<block,thread>>>(d_primer,num[2],(num[1]+num[2]),(num[1]+num[2]),(num[1]+num[2]+num[7]),9);//inner_to_loop
			}
		//calculate
			h_int[8]=circle;
			cudaMemcpyToSymbol(const_int,h_int,20*sizeof(int));
			cudaMalloc((void **)&d_result,num[2]*(8+common_num[0])*sizeof(int));
			cudaMalloc((void **)&d_DPT,num[2]*1264*sizeof(double));
			cudaMalloc((void **)&d_ps,num[2]*81*sizeof(int));
			cudaMalloc((void **)&d_numSeq,num[2]*54*sizeof(char));
			LAMP<<<block,thread>>>(d_seq,d_primer,d_info,d_result,d_DPT,d_ps,d_numSeq,d_Tm);
			cudaFree(d_DPT);
			cudaFree(d_ps);
			cudaFree(d_numSeq);
			h_result=(int *)malloc((8+common_num[0])*num[2]*sizeof(int));
			memset(h_result,'\0',(8+common_num[0])*num[2]*sizeof(int));
			cudaMemcpy(h_result,d_result,(8+common_num[0])*num[2]*sizeof(int),cudaMemcpyDeviceToHost);
			cudaFree(d_result);
	//free
			cudaFree(d_primer);
			cudaFree(d_Tm);
			free(h_Tm);
			if(flag[5]||flag[6])
				cudaFree(d_info);
		//LAMP primers, output
			for(i=0;i<num[2];i++)
			{
				if(have>expect)
					break;
				j=h_result[i*(8+common_num[0])];
				if(j==-1)
					continue;
				if(check_add(h_primer[j*10],h_pos,have)==0)
					continue;
				fprintf(fp,"The %d LAMP primers:\n",have);
		                generate_primer(seq,primer,h_primer[10*j],h_primer[j*10+1],0);
		                fprintf(fp,"  F3: pos:%d,length:%d bp, primer(5'-3'):%s\n",h_primer[10*j],h_primer[j*10+1],primer);
				j=h_result[i*(8+common_num[0])+1];
		                generate_primer(seq,primer,h_primer[j*10],h_primer[j*10+1],0);
		                fprintf(fp,"  F2: pos:%d,length:%d bp, primer(5'-3'):%s\n",h_primer[10*j],h_primer[j*10+1],primer);
				j=h_result[i*(8+common_num[0])+3];
		                generate_primer(seq,primer,h_primer[j*10],h_primer[j*10+1],1);
		                fprintf(fp,"  F1c: pos:%d,length:%d bp, primer(5'-3'):%s\n",h_primer[j*10],h_primer[j*10+1],primer);
				j=h_result[i*(8+common_num[0])+4];
		                generate_primer(seq,primer,h_primer[j*10],h_primer[j*10+1],0);
		                fprintf(fp,"  B1c: pos:%d,length:%d bp, primer(5'-3'):%s\n",h_primer[j*10],h_primer[j*10+1],primer);
				j=h_result[i*(8+common_num[0])+6];
		                generate_primer(seq,primer,h_primer[j*10],h_primer[j*10+1],1);
		                fprintf(fp,"  B2: pos:%d,length:%d bp, primer(5'-3'):%s\n",h_primer[j*10],h_primer[j*10+1],primer);
				j=h_result[i*(8+common_num[0])+7];
		                generate_primer(seq,primer,h_primer[j*10],h_primer[j*10+1],1);
		                fprintf(fp,"  B3: pos:%d,length:%d bp, primer(5'-3'):%s\n",h_primer[j*10],h_primer[j*10+1],primer);
                		if(flag[10])
                		{
					j=h_result[i*(8+common_num[0])+2];
                        		if(j==-1)
                        		        fprintf(fp,"  LF: NULL\n");
                        		else
                        		{
                        		        generate_primer(seq,primer,h_primer[j*10],h_primer[j*10+1],1);
                        		        fprintf(fp,"  LF: pos:%d,length:%d bp, primer(5'-3'):%s\n",h_primer[j*10],h_primer[j*10+1],primer);
                        		}

					j=h_result[i*(8+common_num[0])+5];
		                        if(j==-1)
		                                fprintf(fp,"  LB: NULL\n");
		                        else
		                        {
		                                generate_primer(seq,primer,h_primer[j*10],h_primer[j*10+1],0);
		                                fprintf(fp,"  LB: pos:%d,length:%d bp, primer(5'-3'):%s\n",h_primer[j*10],h_primer[j*10+1],primer);
		                        }
		                }
		                if(flag[5])
		                {
					j=0;
		                        fprintf(fp,"  This set of LAMP primers could be used in %d genomes, there are: ",circle);
		                        p_list=headList;
					while(p_list)
					{
						if(h_result[i*(8+common_num[0])+8+p_list->turn]==0)
						{
							p_list=p_list->next;
							continue;
						}
						if(j==0)
							fprintf(fp,"%s",p_list->name);
						else
							fprintf(fp,", %s",p_list->name);
						j++;
						p_list=p_list->next;
					}
					fprintf(fp,"\n");
				}
				h_pos[have-1]=h_primer[h_result[i*(8+common_num[0])]*10];
				have++;
			}
			free(h_result);
			free(h_primer);
			if(have>before)
                        {
                                end=time(NULL);
                                printf("It takes %0.1f seconds to design %d LAMP primer sets successfully.\n",difftime(end,start),(have-before));
                                before=have;
                                start=time(NULL);
                        }
			if(have>expect)
				break;
		//new primer start
			if(tempS==NULL)
				storeS=tempS;
			else
			{
				while(tempS->pos-storeS->pos>300)
				{
					storeS=storeS->next;
				}
			}
		}//one circle
	}
	cudaFree(d_seq);
	free(h_pos);
	free(seq);
	free(output);
        free(prefix);
        free(store_path);
        free(inner);
        free(outer);
        free(path_fa);
	fclose(fp);
//free struct list
        while(headL)
        {
                p_node=headL->common;
                while(p_node)
                {
                        p_temp=p_node->next;
                        free(p_node);
                        p_node=p_temp;
                }
                p_node=headL->special;
                while(p_node)  
                {
                        p_temp=p_node->next;  
                        free(p_node);  
                        p_node=p_temp;  
                }
                
                storeL=headL->next;
                free(headL);
                headL=storeL;
        }
        while(headS)
        {
                p_node=headS->common;  
                while(p_node)  
                {
                        p_temp=p_node->next;  
                        free(p_node);  
                        p_node=p_temp;  
                }
                p_node=headS->special;
                while(p_node)
                {               
                        p_temp=p_node->next;
                        free(p_node);
                        p_node=p_temp;
                }

                storeS=headS->next;
                free(headS);
                headS=storeS;
        }

        if(flag[5])
        {
                while(headList)
                {
                        p_list=headList->next;
                        free(headList);
                        headList=p_list;
                }
        }

        if(flag[7]||flag[11])
                free(par_path);

        if(flag[10])
        {
                free(loop);
                while(headLoop)
                {
                        p_node=headLoop->common;
                        while(p_node)
                        {
                                p_temp=p_node->next;
                                free(p_node);
                                p_node=p_temp;
                        }
                        p_node=headLoop->special;
                        while(p_node)
                        {
                                p_temp=p_node->next;
                                free(p_node);
                                p_node=p_temp;
                        }

                        storeLoop=headLoop->next;
                        free(headLoop);
                        headLoop=storeLoop;
                }
        }	
	end=time(NULL);
	printf("It takes %0.1f seconds to free memory.\n",difftime(end,start));
	printf("\nIt takes total %0.1f seconds to finish this design.\n",difftime(end,begin));
	return 1;
}
