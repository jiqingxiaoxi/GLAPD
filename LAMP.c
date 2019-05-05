#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include<time.h>
#include<sys/stat.h>
#include<ctype.h>

char str2int(char c)
{
        switch (c)
        {
                case 'A': case 'a': case '0':
                        return 0;
                case 'C': case 'c': case '1':
                        return 1;
                case 'G': case 'g': case '2':
                        return 2;
                case 'T': case 't': case '3':
                        return 3;
        }
        return 4;
}

void readLoop(FILE *file,double *v1,double *v2,double *v3)
{
        char *line,*p,*q;
        
        line=(char *)malloc(200);
        memset(line,'\0',200);
        fgets(line,200,file);

        p = line;
        while (isspace(*p)) 
                p++;
        while (isdigit(*p)) 
                p++;
        while (isspace(*p)) 
                p++;

        q = p;
        while (!isspace(*q)) 
                q++;
        *q = '\0';
        q++;
        if (!strcmp(p, "inf"))
                *v1 =1.0*INFINITY;
        else 
                sscanf(p, "%lf", v1);
        while (isspace(*q))
                q++;

        p = q;
        while (!isspace(*p))
                p++;
        *p = '\0';
        p++;
        if (!strcmp(q, "inf"))
                *v2 =1.0*INFINITY;
        else 
                sscanf(q, "%lf", v2);
        while (isspace(*p))
                p++;

        q = p;
        while (!isspace(*q) && (*q != '\0'))
                q++;
        *q = '\0';
        if (!strcmp(p, "inf"))
                *v3 =1.0*INFINITY;
        else 
                sscanf(p, "%lf", v3);
}

void getStack(double stackEntropies[],double stackEnthalpies[], char *path)
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
                                                stackEntropies[i*125+ii*25+j*5+jj] = -1.0;
                                                stackEnthalpies[i*125+ii*25+j*5+jj]=1.0*INFINITY;
                                        }
                                        else 
                                        {
                                                if(fgets(line,20,sFile)==NULL)
                                                {
                                                        printf("Error! When read parameters in getStack function!\n");
                                                        exit(1);
                                                }
                                                if(strncmp(line, "inf", 3)==0)
                                                        stackEntropies[i*125+ii*25+j*5+jj]=1.0*INFINITY;
                                                else
                                                        stackEntropies[i*125+ii*25+j*5+jj] = atof(line);

                                                if(fgets(line,20,hFile)==NULL)
                                                {
                                                        printf("Error! When read parameters in getStack function!\n");
                                                        exit(1);
                                                }
                                                if(strncmp(line, "inf", 3)==0)
                                                        stackEnthalpies[i*125+ii*25+j*5+jj]=1.0*INFINITY;
                                                else
                                                        stackEnthalpies[i*125+ii*25+j*5+jj] = atof(line);

                                                if (fabs(stackEntropies[i*125+ii*25+j*5+jj])>999999999 ||fabs(stackEnthalpies[i*125+ii*25+j*5+jj])>999999999) 
                                                {
                                                        stackEntropies[i*125+ii*25+j*5+jj] = -1.0;
                                                        stackEnthalpies[i*125+ii*25+j*5+jj] =1.0*INFINITY;
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

void getStackint2(double stackint2Entropies[],double stackint2Enthalpies[], char *path)
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
                                                stackint2Entropies[i*125+ii*25+j*5+jj] = -1.0;
                                                stackint2Enthalpies[i*125+ii*25+j*5+jj] =1.0*INFINITY;
                                        } 
                                        else 
                                        {
                                                if(fgets(line,20,sFile)==NULL)
                                                {
                                                        printf("Error! When read parameters in getStackint2 function!\n");
                                                        exit(1);
                                                }
                                                if(strncmp(line, "inf", 3)==0)
                                                        stackint2Entropies[i*125+ii*25+j*5+jj]=1.0*INFINITY;
                                                else
                                                        stackint2Entropies[i*125+ii*25+j*5+jj] = atof(line);

                                                if(fgets(line,20,hFile)==NULL)
                                                {
                                                        printf("Error! When read parameters in getStackint2 function!\n");
                                                        exit(1);
                                                }
                                                if(strncmp(line, "inf", 3)==0)
                                                        stackint2Enthalpies[i*125+ii*25+j*5+jj]=1.0*INFINITY;
                                                else
                                                        stackint2Enthalpies[i*125+ii*25+j*5+jj] = atof(line);

                                                if(fabs(stackint2Entropies[i*125+ii*25+j*5+jj])>999999999||fabs(stackint2Enthalpies[i*125+ii*25+j*5+jj])>999999999)
                                                {
                                                        stackint2Entropies[i*125+ii*25+j*5+jj] = -1.0;
                                                        stackint2Enthalpies[i*125+ii*25+j*5+jj] =1.0*INFINITY;
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

void getDangle(double dangleEntropies3[],double dangleEnthalpies3[],double dangleEntropies5[],double dangleEnthalpies5[],char *path)
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
                                        dangleEntropies3[i*25+k*5+j] = -1.0;
                                        dangleEnthalpies3[i*25+k*5+j] =1.0*INFINITY;
                                }
                                else if (k == 4)
                                {
                                        dangleEntropies3[i*25+k*5+j] = -1.0;
                                        dangleEnthalpies3[i*25+k*5+j] =1.0*INFINITY;
                                } 
                                else
                                {
                                        if(fgets(line,20,sFile)==NULL)
                                        {
                                                printf("Error! When read parameters in getDangle function!\n");
                                                exit(1);
                                        }
                                        if(strncmp(line, "inf", 3)==0)
                                                dangleEntropies3[i*25+k*5+j]=1.0*INFINITY;
                                        else
                                                dangleEntropies3[i*25+k*5+j]=atof(line);

                                        if(fgets(line,20,hFile)==NULL)
                                        {
                                                printf("Error! When read parameters in getDangle function!\n");        
                                                exit(1);        
                                        }
                                        if(strncmp(line, "inf", 3)==0)        
                                                dangleEnthalpies3[i*25+k*5+j]=1.0*INFINITY;           
                                        else        
                                                dangleEnthalpies3[i*25+k*5+j]=atof(line);

                                        if(fabs(dangleEntropies3[i*25+k*5+j])>999999999||fabs(dangleEnthalpies3[i*25+k*5+j])>999999999) 
                                        {
                                                dangleEntropies3[i*25+k*5+j] = -1.0;
                                                dangleEnthalpies3[i*25+k*5+j] =1.0*INFINITY;
                                        }
                                }
                        }

        for (i = 0; i < 5; ++i)
                for (j = 0; j < 5; ++j)
                        for (k = 0; k < 5; ++k) 
                        {
                                if (i == 4 || j == 4)
                                {
                                        dangleEntropies5[i*25+j*5+k] = -1.0;
                                        dangleEnthalpies5[i*25+j*5+k] =1.0*INFINITY;
                                } 
                                else if (k == 4) 
                                {
                                        dangleEntropies5[i*25+j*5+k] = -1.0;
                                        dangleEnthalpies5[i*25+j*5+k] =1.0*INFINITY;
                                }
                                else
                                {
                                        if(fgets(line,20,sFile)==NULL)
                                        {
                                                printf("Error! When read parameters in getDangle function!\n");
                                                exit(1);
                                        }
                                        if(strncmp(line, "inf", 3)==0)
                                                dangleEntropies5[i*25+j*5+k]=1.0*INFINITY;
                                        else
                                                dangleEntropies5[i*25+j*5+k]=atof(line);

                                        if(fgets(line,20,hFile)==NULL)
                                        {
                                                printf("Error! When read parameters in getDangle function!\n");        
                                                exit(1);        
                                        }
                                        if(strncmp(line, "inf", 3)==0)        
                                                dangleEnthalpies5[i*25+j*5+k]=1.0*INFINITY;           
                                        else        
                                                dangleEnthalpies5[i*25+j*5+k]=atof(line);

                                        if(fabs(dangleEntropies5[i*25+j*5+k])>999999999||fabs(dangleEnthalpies5[i*25+j*5+k])>999999999)
                                        {
                                                dangleEntropies5[i*25+j*5+k] = -1.0;
                                                dangleEnthalpies5[i*25+j*5+k] =1.0*INFINITY;
                                        }
                                }
                        }
        fclose(sFile);
        fclose(hFile);
        free(line);
}

void getLoop(double hairpinLoopEntropies[30],double interiorLoopEntropies[30],double bulgeLoopEntropies[30],double hairpinLoopEnthalpies[30],double interiorLoopEnthalpies[30],double bulgeLoopEnthalpies[30],char *path)
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
                readLoop(sFile, &interiorLoopEntropies[k], &bulgeLoopEntropies[k], &hairpinLoopEntropies[k]);
                readLoop(hFile, &interiorLoopEnthalpies[k], &bulgeLoopEnthalpies[k], &hairpinLoopEnthalpies[k]);
        }
        fclose(sFile);
        fclose(hFile);
}

void getTstack(double tstackEntropies[],double tstackEnthalpies[],char *path)
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
                                                tstackEnthalpies[i1*125+i2*25+j1*5+j2]=1.0*INFINITY;
                                                tstackEntropies[i1*125+i2*25+j1*5+j2] = -1.0;
                                        }
                                        else if (i2 == 4 || j2 == 4)
                                        {
                                                tstackEntropies[i1*125+i2*25+j1*5+j2] = 0.00000000001;
                                                tstackEnthalpies[i1*125+i2*25+j1*5+j2] = 0.0;
                                        }
                                        else
                                        {
                                                if(fgets(line,20,sFile)==NULL)
                                                {
                                                        printf("Error! When read parameters in getTstack function!\n");
                                                        exit(1);
                                                }
                                                if(strncmp(line, "inf", 3)==0)
                                                        tstackEntropies[i1*125+i2*25+j1*5+j2]=1.0*INFINITY;
                                                else
                                                        tstackEntropies[i1*125+i2*25+j1*5+j2]=atof(line);

                                                if(fgets(line,20,hFile)==NULL)
                                                {
                                                        printf("Error! When read parameters in getTstack function!\n");
                                                        exit(1);
                                                }
                                                if(strncmp(line, "inf", 3)==0)
                                                        tstackEnthalpies[i1*125+i2*25+j1*5+j2]=1.0*INFINITY;
                                                else
                                                        tstackEnthalpies[i1*125+i2*25+j1*5+j2]=atof(line);

                                                if (fabs(tstackEntropies[i1*125+i2*25+j1*5+j2])>999999999||fabs(tstackEnthalpies[i1*125+i2*25+j1*5+j2])>999999999)
                                                {
                                                        tstackEntropies[i1*125+i2*25+j1*5+j2] = -1.0;
                                                        tstackEnthalpies[i1*125+i2*25+j1*5+j2] =1.0*INFINITY;
                                                }
                                        }
        fclose(sFile);
        fclose(hFile);
        free(line);
}

void getTstack2(double tstack2Entropies[],double tstack2Enthalpies[],char *path)
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
                                                tstack2Enthalpies[i1*125+i2*25+j1*5+j2] =1.0*INFINITY;
                                                tstack2Entropies[i1*125+i2*25+j1*5+j2] = -1.0;
                                        }
                                        else if (i2 == 4 || j2 == 4)
                                        {
                                                tstack2Entropies[i1*125+i2*25+j1*5+j2] = 0.00000000001;
                                                tstack2Enthalpies[i1*125+i2*25+j1*5+j2] = 0.0;
                                        }
                                        else
                                        {
                                                if(fgets(line,20,sFile)==NULL)
                                                {
                                                        printf("Error! When read parameters in getTstack2 function!\n");
                                                        exit(1);
                                                }
                                                if(strncmp(line, "inf", 3)==0)
                                                        tstack2Entropies[i1*125+i2*25+j1*5+j2]=1.0*INFINITY;
                                                else
                                                        tstack2Entropies[i1*125+i2*25+j1*5+j2]=atof(line);

                                                if(fgets(line,20,hFile)==NULL)
                                                {
                                                        printf("Error! When read parameters in getTstack2 function!\n");
                                                        exit(1);
                                                }
                                                if(strncmp(line, "inf", 3)==0)
                                                        tstack2Enthalpies[i1*125+i2*25+j1*5+j2]=1.0*INFINITY;
                                                else
                                                        tstack2Enthalpies[i1*125+i2*25+j1*5+j2]=atof(line);


                                                if (fabs(tstack2Entropies[i1*125+i2*25+j1*5+j2])>999999999||fabs(tstack2Enthalpies[i1*125+i2*25+j1*5+j2])>999999999)
                                                {
                                                        tstack2Entropies[i1*125+i2*25+j1*5+j2] = -1.0;
                                                        tstack2Enthalpies[i1*125+i2*25+j1*5+j2] =1.0*INFINITY;
                                                }
                                        }
        fclose(sFile);
        fclose(hFile);
        free(line);
}

int get_num_line(char *path,int flag)
{
	FILE *fp;
	int i,size;
	char *line;

	i=strlen(path)+20;
        line=(char *)malloc(i);
        memset(line,'\0',i);
        strcpy(line,path);
	if(flag==0)
	        strcat(line,"triloop.ds");
	else
		strcat(line,"tetraloop.ds");

        if(access(line,0)==-1)
        {
                printf("Error! Don't have %s file!\n",line);
                exit(1);
        }
        fp=fopen(line,"r");
        if(fp==NULL)
        {
                printf("Error! Can't open the %s file!\n",line);
                exit(1);
        }

	size=0;
	while(fgets(line,i,fp)!=NULL)
		size++;
	return size;
}

void getTriloop(char *triloopEntropies1,char *triloopEnthalpies1,double *triloopEntropies2,double *triloopEnthalpies2,char *path)
{
        FILE *sFile, *hFile;
        int i,turn;
        char *line,seq[10],value[10];
        
        i=strlen(path)+20;
        line=(char *)malloc(i);
        memset(line,'\0',i);
        strcpy(line,path);
        strcat(line,"triloop.ds");
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
	
	turn=0;
        while(fscanf(sFile,"%s\t%s\n",seq,value)!=EOF)
        {
		for (i=0;i<5;i++)
			triloopEntropies1[5*turn+i]=str2int(seq[i]);
		if(value[0]=='i')
			triloopEntropies2[turn]=1.0*INFINITY;
		else
			triloopEntropies2[turn]=atof(value);
		turn++;
        }
        fclose(sFile);

	i=strlen(path)+20;
        memset(line,'\0',i);
        strcpy(line,path);
        strcat(line,"triloop.dh");
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

	turn=0;
        while(fscanf(hFile,"%s\t%s\n",seq,value)!=EOF)
        {
		for(i=0;i<5;i++)
			triloopEnthalpies1[turn*5+i]=str2int(seq[i]);
		if(value[0]=='i')
			triloopEnthalpies2[turn]=1.0*INFINITY;
		else
			triloopEnthalpies2[turn]=atof(value);
		turn++;
        }
        fclose(hFile);
}

void getTetraloop(char *tetraloopEntropies1,char *tetraloopEnthalpies1,double *tetraloopEntropies2,double *tetraloopEnthalpies2,char *path)
{
        FILE *sFile, *hFile;
        int i, turn;
        char *line,seq[10],value[10];

        i=strlen(path)+20;
        line=(char *)malloc(i);
        memset(line,'\0',i);
        strcpy(line,path);
        strcat(line,"tetraloop.ds");
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

	turn=0;
        while(fscanf(sFile,"%s\t%s\n",seq,value)!=EOF)
        {
		for(i=0;i<6;i++)
			tetraloopEntropies1[turn*6+i]=str2int(seq[i]);
		if(value[0]=='i')
			tetraloopEntropies2[turn]=1.0*INFINITY;
		else
			tetraloopEntropies2[turn]=atof(value);
		turn++;
        }
        fclose(sFile);

        memset(line,'\0',i);
        strcpy(line,path);
        strcat(line,"tetraloop.dh");
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
        
	turn=0;
        while(fscanf(hFile,"%s\t%s\n",seq,value)!=EOF)
        {
		for(i=0;i<6;i++)
			tetraloopEnthalpies1[6*turn+i]=str2int(seq[i]);
		if(value[0]=='i')
			tetraloopEnthalpies2[turn]=1.0*INFINITY;
		else
			tetraloopEnthalpies2[turn]=atof(value);
		turn++;
        }
        fclose(hFile);
}

void tableStartATS(double atp_value, double atpS[])
{
        int i, j;

        for (i = 0; i < 5; ++i)
                for (j = 0; j < 5; ++j)
                        atpS[i*5+j] = 0.00000000001;
        atpS[3] = atpS[15] = atp_value;
}

void tableStartATH(double atp_value,double atpH[])
{
        int i, j;

        for (i = 0; i < 5; ++i)
                for (j = 0; j < 5; ++j)
                        atpH[i*5+j] = 0.0;
        atpH[3] = atpH[15] = atp_value;
}

void initMatrix2(int Initint[],double enthalpyDPT[],double entropyDPT[],char numSeq1[])
{
	int i,j;
	for(i=1;i<=Initint[0];++i)
		for(j=i;j<=Initint[1];++j)
			if(j-i<4 || (numSeq1[i]+numSeq1[j]!=3))
			{
				enthalpyDPT[(i-1)*Initint[2]+j-1]=1.0*INFINITY;
				entropyDPT[(i-1)*Initint[2]+j-1]=-1.0;
			}
			else
			{
				enthalpyDPT[(i-1)*Initint[2]+j-1]=0.0;
				entropyDPT[(i-1)*Initint[2]+j-1]=-3224.0;
			}
}

double Ss(int i,int j,int k,double stackEntropies[],int Initint[],char numSeq1[],char numSeq2[])
{
	if(k==2)
	{
		if(i>=j)
			return -1.0;
		if(i==Initint[0]||j==Initint[1]+1)
			return -1.0;

		if(i>Initint[0])
			i-=Initint[0];
		if(j>Initint[1])
			j-=Initint[1];
		return stackEntropies[numSeq1[i]*125+numSeq1[i+1]*25+numSeq2[j]*5+numSeq2[j-1]];
	}
	else
		return stackEntropies[numSeq1[i]*125+numSeq1[i+1]*25+numSeq2[j]*5+numSeq2[j+1]];
}

double Hs(int i,int j,int k,double stackEnthalpies[],int Initint[],char numSeq1[],char numSeq2[])
{
	if(k==2)
	{
		if(i>= j)
			return 1.0*INFINITY;
		if(i==Initint[0]||j==Initint[1]+1)
			return 1.0*INFINITY;

		if(i>Initint[0])
			i-=Initint[0];
		if(j>Initint[1])
			j-=Initint[1];
		if(fabs(stackEnthalpies[numSeq1[i]*125+numSeq1[i+1]*25+numSeq2[j]*5+numSeq2[j-1]])<999999999)
			return stackEnthalpies[numSeq1[i]*125+numSeq1[i+1]*25+numSeq2[j]*5+numSeq2[j-1]];
		else
			return 1.0*INFINITY;
	}
	else
		return stackEnthalpies[numSeq1[i]*125+numSeq1[i+1]*25+numSeq2[j]*5+numSeq2[j+1]];
}

void maxTM2(int i,int j,double stackEntropies[],double stackEnthalpies[],double Initdouble[],int Initint[],double enthalpyDPT[],double entropyDPT[],char numSeq1[],char numSeq2[])
{
	double T0,T1,S0,S1,H0,H1;

	S0=entropyDPT[(i-1)*Initint[2]+j-1];
	H0=enthalpyDPT[(i-1)*Initint[2]+j-1];
	T0=(H0+Initdouble[0])/(S0+Initdouble[1]+Initdouble[2]);
	if(fabs(enthalpyDPT[(i-1)*Initint[2]+j-1])<999999999)
	{
		S1=(entropyDPT[i*Initint[2]+j-2]+Ss(i,j,2,stackEntropies,Initint,numSeq1,numSeq2));
		H1=(enthalpyDPT[i*Initint[2]+j-2]+Hs(i,j,2,stackEnthalpies,Initint,numSeq1,numSeq2));
	}
	else
	{
		S1=-1.0;
		H1=1.0*INFINITY;
	}
	T1=(H1+Initdouble[0])/(S1+Initdouble[1]+Initdouble[2]);
	if(S1<-2500.0)
	{
		S1=-3224.0;
		H1=0.0;
	}
	if(S0<-2500.0)
	{
		S0=-3224.0;
		H0=0.0;
 	}

	if(T1>T0)
	{
		entropyDPT[(i-1)*Initint[2]+j-1]=S1;
		enthalpyDPT[(i-1)*Initint[2]+j-1]= H1;
	}
	else
	{
		entropyDPT[(i-1)*Initint[2]+j-1]=S0;
		enthalpyDPT[(i-1)*Initint[2]+j-1]=H0;
	}
}

void calc_bulge_internal2(int i,int j,int ii,int jj,double *EntropyEnthalpy,int traceback,double stackEntropies[],double stackEnthalpies[],double stackint2Entropies[],double stackint2Enthalpies[],double interiorLoopEntropies[],double bulgeLoopEntropies[],double interiorLoopEnthalpies[],double bulgeLoopEnthalpies[],double tstackEntropies[],double tstackEnthalpies[],double atpS[],double atpH[],double Initdouble[0],int Initint[],double enthalpyDPT[],double entropyDPT[],char numSeq1[],char numSeq2[])
{
	int loopSize1,loopSize2,loopSize;
	double T1,T2,S,H;

	S=-3224.0;
	H=0.0;
	loopSize1=ii-i-1;
	loopSize2=j-jj-1;
	if(loopSize1+loopSize2>30)
	{
		EntropyEnthalpy[0]=-1.0;
		EntropyEnthalpy[1]=1.0*INFINITY;
		return;
	}

	loopSize=loopSize1+loopSize2-1;
	if((loopSize1==0&&loopSize2>0)||(loopSize2==0&&loopSize1>0))
	{
		if(loopSize2==1||loopSize1==1)
		{ 
			if((loopSize2==1&&loopSize1==0)||(loopSize2==0&&loopSize1==1))
			{
				H=bulgeLoopEnthalpies[loopSize]+stackEnthalpies[numSeq1[i]*125+numSeq1[ii]*25+numSeq2[j]*5+numSeq2[jj]];
				S=bulgeLoopEntropies[loopSize]+stackEntropies[numSeq1[i]*125+numSeq1[ii]*25+numSeq2[j]*5+numSeq2[jj]];
 			}
			if(traceback!=1)
			{
				H+=enthalpyDPT[(ii-1)*Initint[2]+jj-1];
				S+=entropyDPT[(ii-1)*Initint[2]+jj-1];
			}

			if(fabs(H)>999999999)
			{
				H=1.0*INFINITY;
				S=-1.0;
			}
			T1=(H+Initdouble[0])/((S+Initdouble[1])+Initdouble[2]);
			T2=(enthalpyDPT[(i-1)*Initint[2]+j-1]+Initdouble[0])/((entropyDPT[(i-1)*Initint[2]+j-1])+Initdouble[1]+Initdouble[2]);
			if((T1>T2)||((traceback&&T1>=T2)||traceback==1))
			{
				EntropyEnthalpy[0]=S;
				EntropyEnthalpy[1]=H;
			}
		}
		else
		{
			H=bulgeLoopEnthalpies[loopSize]+atpH[numSeq1[i]*5+numSeq2[j]]+atpH[numSeq1[ii]*5+numSeq2[jj]];
			if(traceback!=1)
				H+=enthalpyDPT[(ii-1)*Initint[2]+jj-1];

			S=bulgeLoopEntropies[loopSize]+atpS[numSeq1[i]*5+numSeq2[j]]+atpS[numSeq1[ii]*5+numSeq2[jj]];
			if(traceback!=1)
				S+=entropyDPT[(ii-1)*Initint[2]+jj-1];
			if(fabs(H)>999999999)
			{
				H=1.0*INFINITY;
				S=-1.0;
			}
			T1=(H+Initdouble[0])/((S+Initdouble[1])+Initdouble[2]);
			T2=(enthalpyDPT[(i-1)*Initint[2]+j-1]+Initdouble[0])/(entropyDPT[(i-1)*Initint[2]+j-1]+Initdouble[1]+Initdouble[2]);
			if((T1>T2)||((traceback&&T1>=T2)||(traceback==1)))
			{
				EntropyEnthalpy[0]=S;
				EntropyEnthalpy[1]=H;
			}
		}
	}
	else if(loopSize1==1&&loopSize2==1)
	{
		S=stackint2Entropies[numSeq1[i]*125+numSeq1[i+1]*25+numSeq2[j]*5+numSeq2[j-1]]+stackint2Entropies[numSeq2[jj]*125+numSeq2[jj+1]*25+numSeq1[ii]*5+numSeq1[ii-1]];
		if(traceback!=1)
			S+=entropyDPT[(ii-1)*Initint[2]+jj-1];

		H=stackint2Enthalpies[numSeq1[i]*125+numSeq1[i+1]*25+numSeq2[j]*5+numSeq2[j-1]]+stackint2Enthalpies[numSeq2[jj]*125+numSeq2[jj+1]*25+numSeq1[ii]*5+numSeq1[ii-1]];
		if(traceback!=1)
			H+=enthalpyDPT[(ii-1)*Initint[2]+jj-1];
		if(fabs(H)>999999999)
		{
			H=1.0*INFINITY;
			S=-1.0;
		}
		T1=(H+Initdouble[0])/((S+Initdouble[1])+Initdouble[2]);
		T2=(enthalpyDPT[(i-1)*Initint[2]+j-1]+Initdouble[0])/(entropyDPT[(i-1)*Initint[2]+j-1]+Initdouble[1]+Initdouble[2]);
		if((T1-T2>=0.000001)||traceback)
		{
			if((T1>T2)||((traceback&&T1>= T2)||traceback==1))
			{
				EntropyEnthalpy[0]=S;
				EntropyEnthalpy[1]=H;
			}
		}
		return;
	}
	else
	{
		H=interiorLoopEnthalpies[loopSize]+tstackEnthalpies[numSeq1[i]*125+numSeq1[i+1]*25+numSeq2[j]*5+numSeq2[j-1]]+tstackEnthalpies[numSeq2[jj]*125+numSeq2[jj+1]*25+numSeq1[ii]*5+numSeq1[ii-1]];
		if(traceback!=1)
			H+=enthalpyDPT[(ii-1)*Initint[2]+jj-1];

		S=interiorLoopEntropies[loopSize]+tstackEntropies[numSeq1[i]*125+numSeq1[i+1]*25+numSeq2[j]*5+numSeq2[j-1]]+tstackEntropies[numSeq2[jj]*125+numSeq2[jj+1]*25+numSeq1[ii]*5+numSeq1[ii-1]]+(-300/310.15*abs(loopSize1-loopSize2));
		if(traceback!=1)
			S+=entropyDPT[(ii-1)*Initint[2]+jj-1];
		if(fabs(H)>999999999)
		{
			H=1.0*INFINITY;
			S=-1.0;
		}

		T1=(H+Initdouble[0])/((S+Initdouble[1])+Initdouble[2]);
		T2=(enthalpyDPT[(i-1)*Initint[2]+j-1]+Initdouble[0])/((entropyDPT[(i-1)*Initint[2]+j-1])+Initdouble[1]+Initdouble[2]);
		if((T1>T2)||((traceback&&T1>=T2)||(traceback==1)))
		{
			EntropyEnthalpy[0]=S;
			EntropyEnthalpy[1]=H;
		}
	}
	return;
}

void CBI(int i,int j,double* EntropyEnthalpy,int traceback,double stackEntropies[],double stackEnthalpies[],double stackint2Entropies[],double stackint2Enthalpies[],double interiorLoopEntropies[],double bulgeLoopEntropies[],double interiorLoopEnthalpies[],double bulgeLoopEnthalpies[],double tstackEntropies[],double tstackEnthalpies[],double atpS[],double atpH[],double Initdouble[],int Initint[],double enthalpyDPT[],double entropyDPT[],char numSeq1[],char numSeq2[])
{
	int d,ii,jj;

	for(d=j-i-3;d>=4&&d>=j-i-32;--d)
		for(ii=i+1;ii<j-d&&ii<=Initint[0];++ii)
		{
			jj=d+ii;
			if(traceback==0)
			{
				EntropyEnthalpy[0]=-1.0;
				EntropyEnthalpy[1]=1.0*INFINITY;
			}
			if(fabs(enthalpyDPT[(ii-1)*Initint[2]+jj-1])<999999999)
			{
				calc_bulge_internal2(i,j,ii,jj,EntropyEnthalpy,traceback,stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,interiorLoopEntropies,bulgeLoopEntropies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,numSeq1,numSeq2);
				if(fabs(EntropyEnthalpy[1])<999999999)
				{
					if(EntropyEnthalpy[0] <-2500.0)
					{
						EntropyEnthalpy[0]=-3224.0;
						EntropyEnthalpy[1]=0.0;
					}
					if(traceback==0)
					{
						enthalpyDPT[(i-1)*Initint[2]+j-1]=EntropyEnthalpy[1];
						entropyDPT[(i-1)*Initint[2]+j-1]=EntropyEnthalpy[0];
					}
				}
			}
		}
	return;
}

int find_pos(char *ref,int ref_start,char *source,int length,int num)
{
	int flag,i,j;

	for(i=0;i<num;i++)
	{
		flag=0;
		for(j=0;j<length;j++)
		{
			if(ref[ref_start+j]!=source[i*length+j])
			{
				flag++;
				break;
			}
		}
		if(flag==0)
			return i;
	}
	return -1;
}

void calc_hairpin(int i,int j,double *EntropyEnthalpy,int traceback,double hairpinLoopEntropies[],double hairpinLoopEnthalpies[],double tstack2Entropies[],double tstack2Enthalpies[],char *triloopEntropies1,char *triloopEnthalpies1,char *tetraloopEntropies1,char *tetraloopEnthalpies1,double *triloopEntropies2,double *triloopEnthalpies2,double *tetraloopEntropies2,double *tetraloopEnthalpies2,int numTriloops,int numTetraloops,double atpS[],double atpH[],double Initdouble[],int Initint[],double enthalpyDPT[],double entropyDPT[],char numSeq1[])
{
	int pos,loopSize=j-i-1;
	double T1,T2;
	
	if(loopSize < 3)
	{
		EntropyEnthalpy[0]=-1.0;
		EntropyEnthalpy[1]=1.0*INFINITY;
		return;
	}
	if(i<=Initint[0]&&Initint[1]<j)
	{
		EntropyEnthalpy[0]=-1.0;
		EntropyEnthalpy[1]=1.0*INFINITY;
		return;
	}
	else if(i>Initint[1])
	{
		i-= Initint[0];
		j-= Initint[1];
	}
	if(loopSize<=30)
	{
		EntropyEnthalpy[1]=hairpinLoopEnthalpies[loopSize-1];
		EntropyEnthalpy[0]=hairpinLoopEntropies[loopSize-1];
	}
	else
	{
		EntropyEnthalpy[1]=hairpinLoopEnthalpies[29];
		EntropyEnthalpy[0]=hairpinLoopEntropies[29];
	}

	if(loopSize>3) // for loops 4 bp and more in length, terminal mm are accounted
	{
		EntropyEnthalpy[1]+=tstack2Enthalpies[numSeq1[i]*125+numSeq1[i+1]*25+numSeq1[j]*5+numSeq1[j-1]];
		EntropyEnthalpy[0]+=tstack2Entropies[numSeq1[i]*125+numSeq1[i+1]*25+numSeq1[j]*5+numSeq1[j-1]];
	}
	else if(loopSize == 3) // for loops 3 bp in length at-penalty is considered
	{
		EntropyEnthalpy[1]+=atpH[numSeq1[i]*5+numSeq1[j]];
		EntropyEnthalpy[0]+=atpS[numSeq1[i]*5+numSeq1[j]];
	}

	if(loopSize==3) // closing AT-penalty (+), triloop bonus, hairpin of 3 (+) 
	{
		pos=find_pos(numSeq1,i,triloopEnthalpies1,5,numTriloops);
		if(pos!=-1)
			EntropyEnthalpy[1]+=triloopEnthalpies2[pos];

		pos=find_pos(numSeq1,i,triloopEntropies1,5,numTriloops);
		if(pos!=-1)
			EntropyEnthalpy[0]+=triloopEntropies2[pos];
	}
	else if (loopSize == 4) // terminal mismatch, tetraloop bonus, hairpin of 4
	{
		pos=find_pos(numSeq1,i,tetraloopEnthalpies1,6,numTetraloops);
		if(pos!=-1)
			EntropyEnthalpy[1]+=tetraloopEnthalpies2[pos];

		pos=find_pos(numSeq1,i,tetraloopEntropies1,6,numTetraloops);
		if(pos!=-1)
			EntropyEnthalpy[0]+=tetraloopEntropies2[pos];
	}
	if(fabs(EntropyEnthalpy[1])>999999999)
	{
		EntropyEnthalpy[1] =1.0*INFINITY;
		EntropyEnthalpy[0] = -1.0;
	}
	T1 = (EntropyEnthalpy[1] +Initdouble[0]) / ((EntropyEnthalpy[0] +Initdouble[1]+ Initdouble[2]));
	T2 = (enthalpyDPT[(i-1)*Initint[2]+j-1] +Initdouble[0]) / ((entropyDPT[(i-1)*Initint[2]+j-1]) +Initdouble[1]+ Initdouble[2]);
	if(T1 < T2 && traceback == 0)
	{
		EntropyEnthalpy[0] =entropyDPT[(i-1)*Initint[2]+j-1];
		EntropyEnthalpy[1] =enthalpyDPT[(i-1)*Initint[2]+j-1];
	}
	return;
}

void fillMatrix2(double stackEntropies[],double stackEnthalpies[],double stackint2Entropies[],double stackint2Enthalpies[],double hairpinLoopEntropies[],double interiorLoopEntropies[],double bulgeLoopEntropies[],double hairpinLoopEnthalpies[],double interiorLoopEnthalpies[],double bulgeLoopEnthalpies[],double tstackEntropies[],double tstackEnthalpies[],double tstack2Entropies[],double tstack2Enthalpies[],char *triloopEntropies1,char *triloopEnthalpies1,char *tetraloopEntropies1,char *tetraloopEnthalpies1,double *triloopEntropies2,double *triloopEnthalpies2,double *tetraloopEntropies2,double *tetraloopEnthalpies2,int numTriloops,int numTetraloops,double atpS[],double atpH[],double Initdouble[],int Initint[],double enthalpyDPT[],double entropyDPT[],char numSeq1[],char numSeq2[])
{
	int i, j;
	double SH[2];

	for (j = 2; j <= Initint[1]; ++j)
		for (i = j - 3 - 1; i >= 1; --i)
		{
			if (fabs(enthalpyDPT[(i-1)*Initint[2]+j-1])<999999999)
			{
				SH[0] = -1.0;
				SH[1] =1.0*INFINITY;
				maxTM2(i,j,stackEntropies,stackEnthalpies,Initdouble,Initint,enthalpyDPT,entropyDPT,numSeq1,numSeq2);
				CBI(i,j,SH,0,stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,interiorLoopEntropies,bulgeLoopEntropies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,numSeq1,numSeq2);

				SH[0] = -1.0;
				SH[1] =1.0*INFINITY;
				calc_hairpin(i, j, SH, 0,hairpinLoopEntropies,hairpinLoopEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,numSeq1);
				if(fabs(SH[1])<999999999)
				{
					if(SH[0] <-2500.0) /* to not give dH any value if dS is unreasonable */
					{
						SH[0] =-3224.0;
						SH[1] = 0.0;
					}
					entropyDPT[(i-1)*Initint[2]+j-1]= SH[0];
					enthalpyDPT[(i-1)*Initint[2]+j-1]= SH[1];
				}
			}
		}
}

int max5(double a,double b,double c,double d,double e)
{
	if(a>b&&a>c&&a>d&&a>e)
		return 1;
	else if(b>c&&b>d&&b>e)
		return 2;
	else if(c>d&&c>e)
		return 3;
	else if(d>e)
		return 4;
	else
		return 5;
}

double Sd5(int i,int j,double dangleEntropies5[],char numSeq1[])
{
	return dangleEntropies5[numSeq1[i]*25+numSeq1[j]*5+numSeq1[j-1]];
}

double Hd5(int i,int j,double dangleEnthalpies5[],char numSeq1[])
{
	return dangleEnthalpies5[numSeq1[i]*25+numSeq1[j]*5+numSeq1[j-1]];
}

double Sd3(int i,int j,double dangleEntropies3[],char numSeq1[])
{
	return dangleEntropies3[numSeq1[i]*25+numSeq1[i+1]*5+numSeq1[j]];
}

double Hd3(int i,int j,double dangleEnthalpies3[],char numSeq1[])
{
	return dangleEnthalpies3[numSeq1[i]*25+numSeq1[i+1]*5+numSeq1[j]];
}

double Ststack(int i,int j,double tstack2Entropies[],char numSeq1[])
{
	return tstack2Entropies[numSeq1[i]*125+numSeq1[i+1]*25+numSeq1[j]*5+numSeq1[j-1]];
}

double Htstack(int i,int j,double tstack2Enthalpies[],char numSeq1[])
{
	return tstack2Enthalpies[numSeq1[i]*125+numSeq1[i+1]*25+numSeq1[j]*5+numSeq1[j-1]];
}

double END5_1(int i,int hs,double atpS[],double atpH[],double Initdouble[],int Initint[],double enthalpyDPT[],double entropyDPT[],double send5[],double hend5[],char numSeq1[])
{
	int k;
	double max_tm,T1,T2,H,S,H_max,S_max;

	max_tm=-1.0*INFINITY;
	H_max=1.0*INFINITY;
	S_max=-1.0;
	for(k=0;k<=i-5;++k)
	{
		T1=(hend5[k]+Initdouble[0])/(send5[k]+Initdouble[1]+Initdouble[2]);
		T2=Initdouble[0]/(Initdouble[1]+Initdouble[2]);
		if(T1>=T2)
		{
			H=hend5[k]+atpH[numSeq1[k+1]*5+numSeq1[i]]+enthalpyDPT[k*Initint[2]+i-1];
			S=send5[k]+atpS[numSeq1[k+1]*5+numSeq1[i]]+entropyDPT[k*Initint[2]+i-1];
			if(fabs(H)>999999999||H>0||S>0)  // H and S must be greater than 0 to avoid BS
			{
				H=1.0*INFINITY;
				S=-1.0;
			}
			T1=(H+Initdouble[0])/(S+Initdouble[1]+Initdouble[2]);
		}
		else
		{
			H=atpH[numSeq1[k+1]*5+numSeq1[i]]+enthalpyDPT[k*Initint[2]+i-1];
			S=atpS[numSeq1[k+1]*5+numSeq1[i]]+entropyDPT[k*Initint[2]+i-1];
			if(fabs(H)>999999999||H>0||S>0)
			{
				H=1.0*INFINITY;
				S=-1.0;
			}
			T1=(H+Initdouble[0])/(S+Initdouble[1]+Initdouble[2]);
		}

		if(max_tm<T1)
		{
			if(S>-2500.0)
			{
				H_max=H;
				S_max=S;
				max_tm=T1;
			}
		}
	}
	if(hs==1)
		return H_max;
	return S_max;
}

double END5_2(int i,int hs,double dangleEntropies5[],double dangleEnthalpies5[],double atpS[],double atpH[],double Initdouble[],int Initint[],double enthalpyDPT[],double entropyDPT[],double send5[],double hend5[],char numSeq1[])
{
	int k;
	double max_tm,T1,T2,H,S,H_max,S_max;

	H_max=1.0*INFINITY;
	max_tm=-1.0*INFINITY;
	S_max=-1.0;
	for(k=0;k<=i-6;++k)
	{
		T1=(hend5[k]+Initdouble[0])/(send5[k]+Initdouble[1]+Initdouble[2]);
		T2=Initdouble[0]/(Initdouble[1]+Initdouble[2]);
		if(T1>=T2)
		{
			H=hend5[k]+atpH[numSeq1[k+2]*5+numSeq1[i]]+Hd5(i,k+2,dangleEnthalpies5,numSeq1)+enthalpyDPT[(k+1)*Initint[2]+i-1];
			S=send5[k]+atpS[numSeq1[k+2]*5+numSeq1[i]]+Sd5(i,k+2,dangleEntropies5,numSeq1)+entropyDPT[(k+1)*Initint[2]+i-1];
			if(fabs(H)>999999999||H>0||S>0)
			{
				H=1.0*INFINITY;
				S=-1.0;
			}
			T1=(H+Initdouble[0])/(S+Initdouble[1]+Initdouble[2]);
		}
		else
		{
			H=atpH[numSeq1[k+2]*5+numSeq1[i]]+Hd5(i,k+2,dangleEnthalpies5,numSeq1)+enthalpyDPT[(k+1)*Initint[2]+i-1];
			S=atpS[numSeq1[k+2]*5+numSeq1[i]]+Sd5(i,k+2,dangleEntropies5,numSeq1)+entropyDPT[(k+1)*Initint[2]+i-1];
			if(fabs(H)>999999999||H>0||S>0)
			{
				H=1.0*INFINITY;
				S=-1.0;
			}
			T1=(H+Initdouble[0])/(S+Initdouble[1]+Initdouble[2]);
		}

		if(max_tm<T1)
		{
			if(S>-2500.0)
			{
				H_max=H;
				S_max=S;
				max_tm=T1;
			}
		}
	}
	if(hs==1)
		return H_max;
	return S_max;
}

double END5_3(int i,int hs,double dangleEntropies3[],double dangleEnthalpies3[],double atpS[],double atpH[],double Initdouble[],int Initint[],double enthalpyDPT[],double entropyDPT[],double send5[],double hend5[],char numSeq1[])
{
	int k;
	double max_tm,T1,T2,H,S,H_max,S_max;

	H_max=1.0*INFINITY;
	max_tm=-1.0*INFINITY;
	S_max=-1.0;
	for(k=0;k<=i-6;++k)
	{
		T1=(hend5[k]+Initdouble[0])/(send5[k]+Initdouble[1]+Initdouble[2]);
		T2=Initdouble[0]/(Initdouble[1]+Initdouble[2]);
		if(T1>=T2)
		{
			H=hend5[k]+atpH[numSeq1[k+1]*5+numSeq1[i-1]]+Hd3(i-1,k+1,dangleEnthalpies3,numSeq1)+enthalpyDPT[k*Initint[2]+i-2];
			S=send5[k]+atpS[numSeq1[k+1]*5+numSeq1[i-1]]+Sd3(i-1,k+1,dangleEntropies3,numSeq1)+entropyDPT[k*Initint[2]+i-2];
			if(fabs(H)>999999999||H>0||S>0)
			{
				H=1.0*INFINITY;
				S=-1.0;
			}
			T1=(H+Initdouble[0])/(S+Initdouble[1]+Initdouble[2]);
		}
		else
		{
			H=atpH[numSeq1[k+1]*5+numSeq1[i-1]]+Hd3(i-1,k+1,dangleEnthalpies3,numSeq1)+enthalpyDPT[k*Initint[2]+i-2];
			S=atpS[numSeq1[k+1]*5+numSeq1[i-1]]+Sd3(i-1,k+1,dangleEntropies3,numSeq1)+entropyDPT[k*Initint[2]+i-2];
			if(fabs(H)>999999999||H>0||S>0)
			{
				H=1.0*INFINITY;
				S=-1.0;
			}
			T1=(H+Initdouble[0])/(S+Initdouble[1]+Initdouble[2]);
		}

		if(max_tm<T1)
		{
			if(S>-2500.0)
			{
				H_max=H;
				S_max=S;
				max_tm=T1;
			}
		}
	}
	if(hs==1)
		return H_max;
	return S_max;
}

double END5_4(int i,int hs,double tstack2Entropies[],double tstack2Enthalpies[],double atpS[],double atpH[],double Initdouble[],int Initint[],double enthalpyDPT[],double entropyDPT[],double send5[],double hend5[],char numSeq1[])
{
	int k;
	double max_tm,T1,T2,H,S,H_max,S_max;

	H_max=1.0*INFINITY;
	max_tm=-1.0*INFINITY;
	S_max=-1.0;
	for(k=0;k<=i-7;++k)
	{
		T1=(hend5[k]+Initdouble[0])/(send5[k]+Initdouble[1]+Initdouble[2]);
		T2=Initdouble[0]/(Initdouble[1]+Initdouble[2]);
		if(T1>=T2)
		{
			H=hend5[k]+atpH[numSeq1[k+2]*5+numSeq1[i-1]]+Htstack(i-1,k+2,tstack2Enthalpies,numSeq1)+enthalpyDPT[(k+1)*Initint[2]+i-2];
			S=send5[k]+atpS[numSeq1[k+2]*5+numSeq1[i-1]]+Ststack(i-1,k+2,tstack2Entropies,numSeq1)+entropyDPT[(k+1)*Initint[2]+i-2];
			if(fabs(H)>999999999||H>0||S>0)
			{
				H=1.0*INFINITY;
				S=-1.0;
			}
			T1=(H+Initdouble[0])/(S+Initdouble[1]+Initdouble[2]);
		}
		else
		{
			H=atpH[numSeq1[k+2]*5+numSeq1[i-1]]+Htstack(i-1,k+2,tstack2Enthalpies,numSeq1)+enthalpyDPT[(k+1)*Initint[2]+i-2];
			S=atpS[numSeq1[k+2]*5+numSeq1[i-1]]+Ststack(i-1,k+2,tstack2Entropies,numSeq1)+entropyDPT[(k+1)*Initint[2]+i-2];
			if(fabs(H)>999999999||H>0||S>0)
			{
				H=1.0*INFINITY;
				S=-1.0;
			}
			T1=(H+Initdouble[0])/(S+Initdouble[1]+Initdouble[2]);
 		}

		if(max_tm<T1)
		{
			if(S>-2500.0)
			{
				H_max=H;
				S_max=S;
				max_tm=T1;
			}
		}
	}
	if(hs==1)
		return H_max;
	return S_max;
}

void calc_terminal_bp(double temp,double dangleEntropies3[],double dangleEnthalpies3[],double dangleEntropies5[],double dangleEnthalpies5[],double tstack2Entropies[],double tstack2Enthalpies[],double atpS[],double atpH[],double Initdouble[],int Initint[],double enthalpyDPT[],double entropyDPT[],double send5[],double hend5[],char numSeq1[])
{
	int i,max;
	double T1,T2,T3,T4,T5,G,end5_11,end5_12,end5_21,end5_22,end5_31,end5_32,end5_41,end5_42;
	
	send5[0]=send5[1]= -1.0;
	hend5[0]=hend5[1]=1.0*INFINITY;

	for(i=2;i<=Initint[0];i++)
	{
		send5[i]=-3224.0;
		hend5[i]=0;
	}

// adding terminal penalties to 3' end and to 5' end 
	for(i=2;i<=Initint[0];++i)
	{
		max=0;
		T1=(hend5[i-1]+Initdouble[0])/(send5[i-1]+Initdouble[1]+Initdouble[2]);
		end5_11=END5_1(i,1,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,send5,hend5,numSeq1);
		end5_12=END5_1(i,2,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,send5,hend5,numSeq1);
		T2=(end5_11+Initdouble[0])/(end5_12+Initdouble[1]+Initdouble[2]);
		end5_21=END5_2(i,1,dangleEntropies5,dangleEnthalpies5,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,send5,hend5,numSeq1);
		end5_22=END5_2(i,2,dangleEntropies5,dangleEnthalpies5,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,send5,hend5,numSeq1);
		T3=(end5_21+Initdouble[0])/(end5_22+Initdouble[1]+Initdouble[2]);
		end5_31=END5_3(i,1,dangleEntropies3,dangleEnthalpies3,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,send5,hend5,numSeq1);
		end5_32=END5_3(i,2,dangleEntropies3,dangleEnthalpies3,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,send5,hend5,numSeq1);
		T4=(end5_31+Initdouble[0])/(end5_32+Initdouble[1]+Initdouble[2]);
		end5_41=END5_4(i,1,tstack2Entropies,tstack2Enthalpies,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,send5,hend5,numSeq1);
		end5_42=END5_4(i,2,tstack2Entropies,tstack2Enthalpies,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,send5,hend5,numSeq1);
		T5=(end5_41+Initdouble[0])/(end5_42+Initdouble[1]+Initdouble[2]);

		max=max5(T1,T2,T3,T4,T5);
		switch(max)
		{
			case 1:
				send5[i]=send5[i-1];
				hend5[i]=hend5[i-1];
				break;
			case 2:
				G=end5_11-temp*end5_12;
				if(G<0.0)
				{
					send5[i]=end5_12;
					hend5[i]=end5_11;
				}
				else
				{
					send5[i]=send5[i-1];
					hend5[i]=hend5[i-1];
				}
				break;
			case 3:
				G=end5_21-temp*end5_22;
				if(G<0.0)
				{
					send5[i]=end5_22;
					hend5[i]=end5_21;
				}
				else
				{
					send5[i]=send5[i-1];
					hend5[i]=hend5[i-1];
				}
				break;
			case 4:
				G=end5_31-temp*end5_32;
				if(G<0.0)
				{
					send5[i]=end5_32;
					hend5[i]=end5_31;
				}
				else
				{
					send5[i]=send5[i-1];
					hend5[i]=hend5[i-1];
				}
				break;
			case 5:
				G=end5_41-temp*end5_42;
				if(G<0.0)
				{
					send5[i]=end5_42;
					hend5[i]=end5_41;
				}
				else
				{
					send5[i]=send5[i-1];
					hend5[i]=hend5[i-1];
				}
				break;
			default:
				break;
		}
	}
}

int newpush(int store[],int i,int j,int mtrx,int total,int next)
{
        int k;
        for(k=total-1;k>=next;k--)
        {
                store[(k+1)*3]=store[k*3];
                store[(k+1)*3+1]=store[k*3+1];
                store[(k+1)*3+2]=store[k*3+2];
        }
        store[next*3]=i;                  
        store[next*3+1]=j;
        store[next*3+2]=mtrx;

        return total+1;           
}

int equal(double a,double b)
{
	if(fabs(a)>999999999||fabs(b)>999999999)
		return 0;
	return fabs(a-b)<1e-5;
}

void tracebacku(int bp[],double stackEntropies[],double stackEnthalpies[],double stackint2Entropies[],double stackint2Enthalpies[],double dangleEntropies3[],double dangleEnthalpies3[],double dangleEntropies5[],double dangleEnthalpies5[],double hairpinLoopEntropies[],double interiorLoopEntropies[],double bulgeLoopEntropies[],double hairpinLoopEnthalpies[],double interiorLoopEnthalpies[],double bulgeLoopEnthalpies[],double tstackEntropies[],double tstackEnthalpies[],double tstack2Entropies[],double tstack2Enthalpies[],char *triloopEntropies1,char *triloopEnthalpies1,char *tetraloopEntropies1,char *tetraloopEnthalpies1,double *triloopEntropies2,double *triloopEnthalpies2,double *tetraloopEntropies2,double *tetraloopEnthalpies2,int numTriloops,int numTetraloops,double atpS[],double atpH[],double Initdouble[],int Initint[],double enthalpyDPT[],double entropyDPT[],double send5[],double hend5[],char numSeq1[],char numSeq2[])
{
	int i,j,store[50],total,now,ii,jj,k,d,done;
	double SH1[2],SH2[2],EntropyEnthalpy[2];

        total=newpush(store,Initint[0],0,1,0,0);
        now=0;
        while(now<total)
        {
                i=store[3*now]; // top->i;
                j=store[3*now+1]; // top->j;
                if(store[now*3+2]==1)
                {
                        while(equal(send5[i],send5[i-1])&&equal(hend5[i],hend5[i-1])) // if previous structure is the same as this one
                                --i;
                        if(i==0)
                                continue;
                        if(equal(send5[i],END5_1(i,2,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,send5,hend5,numSeq1))&&equal(hend5[i],END5_1(i,1,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,send5,hend5,numSeq1)))
                        {
                                for(k=0;k<=i-5;++k)
                                        if(equal(send5[i],atpS[numSeq1[k+1]*5+numSeq1[i]]+entropyDPT[k*Initint[2]+i-1])&&equal(hend5[i],atpH[numSeq1[k+1]*5+numSeq1[i]]+enthalpyDPT[k*Initint[2]+i-1]))
                                        {
                                                total=newpush(store,k+1,i,0,total,now+1);                    
                                                break;
                                        }
                                        else if(equal(send5[i],send5[k]+atpS[numSeq1[k+1]*5+numSeq1[i]]+entropyDPT[k*Initint[2]+i-1])&&equal(hend5[i],hend5[k]+atpH[numSeq1[k+1]*5+numSeq1[i]]+enthalpyDPT[k*Initint[2]+i-1]))
                                        {
                                                total=newpush(store,k+1,i,0,total,now+1);
                                                total=newpush(store,k,0,1,total,now+1);
                                                break;
                                        }
                        }
                        else if(equal(send5[i],END5_2(i,2,dangleEntropies5,dangleEnthalpies5,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,send5,hend5,numSeq1))&&equal(hend5[i],END5_2(i,1,dangleEntropies5,dangleEnthalpies5,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,send5,hend5,numSeq1)))
                        {
                                for (k=0;k<=i-6;++k)
                                        if(equal(send5[i],atpS[numSeq1[k+2]*5+numSeq1[i]]+Sd5(i,k+2,dangleEntropies5,numSeq1)+entropyDPT[(k+1)*Initint[2]+i-1])&&equal(hend5[i],atpH[numSeq1[k+2]*5+numSeq1[i]]+Hd5(i,k+2,dangleEnthalpies5,numSeq1)+enthalpyDPT[(k+1)*Initint[2]+i-1]))
                                        {
                                                total=newpush(store,k+2,i,0,total,now+1);
                                                break;
                                        }
                                        else if(equal(send5[i],send5[k]+atpS[numSeq1[k+2]*5+numSeq1[i]]+Sd5(i,k+2,dangleEntropies5,numSeq1)+entropyDPT[(k+1)*Initint[2]+i-1])&&equal(hend5[i],hend5[k]+atpH[numSeq1[k+2]*5+numSeq1[i]]+Hd5(i,k+2,dangleEnthalpies5,numSeq1)+enthalpyDPT[(k+1)*Initint[2]+i-1]))
                                        {
                                                total=newpush(store,k+2,i,0,total,now+1);
                                                total=newpush(store,k,0,1,total,now+1);
                                                break;
                                        }
                        }
                        else if(equal(send5[i],END5_3(i,2,dangleEntropies3,dangleEnthalpies3,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,send5,hend5,numSeq1))&&equal(hend5[i],END5_3(i,1,dangleEntropies3,dangleEnthalpies3,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,send5,hend5,numSeq1)))
                        {
                                for (k=0;k<=i-6;++k)
                                        if(equal(send5[i],atpS[numSeq1[k+1]*5+numSeq1[i-1]]+Sd3(i-1,k+1,dangleEntropies3,numSeq1)+entropyDPT[k*Initint[2]+i-2])&&equal(hend5[i],atpH[numSeq1[k+1]*5+numSeq1[i-1]]+Hd3(i-1,k+1,dangleEnthalpies3,numSeq1)+enthalpyDPT[k*Initint[2]+i-2]))
                                        {
                                                total=newpush(store,k+1,i-1,0,total,now+1);
                                                break;
                                        }
                                        else if(equal(send5[i],send5[k]+atpS[numSeq1[k+1]*5+numSeq1[i-1]]+Sd3(i-1,k+1,dangleEntropies3,numSeq1)+entropyDPT[k*Initint[2]+i-2])&&equal(hend5[i],hend5[k]+atpH[numSeq1[k+1]*5+numSeq1[i-1]]+Hd3(i-1,k+1,dangleEnthalpies3,numSeq1)+enthalpyDPT[k*Initint[2]+i-2]))
                                        {
                                                total=newpush(store,k+1,i-1,0,total,now+1);
                                                total=newpush(store,k,0,1,total,now+1);
                                                break;
                                        }
                        }
                        else if(equal(send5[i],END5_4(i,2,tstack2Entropies,tstack2Enthalpies,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,send5,hend5,numSeq1))&&equal(hend5[i],END5_4(i,1,tstack2Entropies,tstack2Enthalpies,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,send5,hend5,numSeq1)))
                        {
                                for (k=0;k<=i-7;++k)
                                        if(equal(send5[i],atpS[numSeq1[k+2]*5+numSeq1[i-1]]+Ststack(i-1,k+2,tstack2Entropies,numSeq1)+entropyDPT[(k+1)*Initint[2]+i-2])&&equal(hend5[i],atpH[numSeq1[k+2]*5+numSeq1[i-1]]+Htstack(i-1,k+2,tstack2Enthalpies,numSeq1)+enthalpyDPT[(k+1)*Initint[2]+i-2]))
                                        {
                                                total=newpush(store,k+2,i-1,0,total,now+1);
                                                break;
                                        }
                                        else if(equal(send5[i],send5[k]+atpS[numSeq1[k+2]*5+numSeq1[i-1]]+Ststack(i-1,k+2,tstack2Entropies,numSeq1)+entropyDPT[(k+1)*Initint[2]+i-2])&&equal(hend5[i],hend5[k]+atpH[numSeq1[k+2]*5+numSeq1[i-1]]+Htstack(i-1,k+2,tstack2Enthalpies,numSeq1)+enthalpyDPT[(k+1)*Initint[2]+i-2]))
                                        {
                                                total=newpush(store,k+2,i-1,0,total,now+1);
                                                total=newpush(store,k,0,1,total,now+1);
                                                break;
                                        }
                        }
                }
                else if(store[3*now+2]==0)
                {
                        bp[i-1]=j;
                        bp[j-1]=i;
                        SH1[0]=-1.0;
                        SH1[1]=1.0*INFINITY;
                        calc_hairpin(i,j,SH1,1,hairpinLoopEntropies,hairpinLoopEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,numSeq1);

                        SH2[0]=-1.0;
                        SH2[1]=1.0*INFINITY;
                        CBI(i,j,SH2,2,stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,interiorLoopEntropies,bulgeLoopEntropies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,numSeq1,numSeq2);

                        if (equal(entropyDPT[(i-1)*Initint[2]+j-1],Ss(i,j,2,stackEntropies,Initint,numSeq1,numSeq2)+entropyDPT[i*Initint[2]+j-2])&&equal(enthalpyDPT[(i-1)*Initint[2]+j-1],Hs(i,j,2,stackEnthalpies,Initint,numSeq1,numSeq2)+enthalpyDPT[i*Initint[2]+j-2]))
                                total=newpush(store,i+1,j-1,0,total,now+1);
                        else if(equal(entropyDPT[(i-1)*Initint[2]+j-1],SH1[0])&&equal(enthalpyDPT[(i-1)*Initint[2]+j-1],SH1[1]));
                        else if(equal(entropyDPT[(i-1)*Initint[2]+j-1],SH2[0])&&equal(enthalpyDPT[(i-1)*Initint[2]+j-1],SH2[1]))
                        {
                                for (done=0,d=j-i-3;d>=4&&d>=j-i-32&&!done;--d)
                                        for (ii=i+1;ii<j-d;++ii)
                                        {
                                                jj=d+ii;
                                                EntropyEnthalpy[0]=-1.0;
                                                EntropyEnthalpy[1]=1.0*INFINITY;
                                                calc_bulge_internal2(i,j,ii,jj,EntropyEnthalpy,1,stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,interiorLoopEntropies,bulgeLoopEntropies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,numSeq1,numSeq2);

                                                if (equal(entropyDPT[(i-1)*Initint[2]+j-1],EntropyEnthalpy[0]+entropyDPT[(ii-1)*Initint[2]+jj-1])&&equal(enthalpyDPT[(i-1)*Initint[2]+j-1],EntropyEnthalpy[1]+enthalpyDPT[(ii-1)*Initint[2]+jj-1]))
                                                {
                                                        total=newpush(store,ii,jj,0,total,now+1);
                                                        ++done;
                                                        break;
                                                }
                                        }
                        }
                }
                now++;
        }
}

double drawHairpin(int bp[],double mh,double ms,int Initint[])
{
        int i,N;

        N=0;
        if(fabs(ms)>999999999||fabs(mh)>999999999)
        {
		return 0.0;
        }
        else
        {
		for(i=1;i<Initint[0];++i)
		{
			if(bp[i-1]>0)
				N++;
                }
                return mh/(ms+(((N/2)-1)*-0.51986))-273.15;
        }
}

void initMatrix(int Initint[],double enthalpyDPT[],double entropyDPT[],char numSeq1[],char numSeq2[])
{
	int i,j;

	for(i=1;i<=Initint[0];++i)
	{
		for(j=1;j<=Initint[1];++j)
		{
			if(numSeq1[i]+numSeq2[j]!=3)
			{
				enthalpyDPT[(i-1)*Initint[2]+j-1]=1.0*INFINITY;
				entropyDPT[(i-1)*Initint[2]+j-1]=-1.0;
			}
			else
			{
				enthalpyDPT[(i-1)*Initint[2]+j-1]=0.0;
				entropyDPT[(i-1)*Initint[2]+j-1]=-3224.0;
			}
		}
	}
}

void LSH(int i,int j,double *EntropyEnthalpy,double dangleEntropies3[],double dangleEnthalpies3[],double dangleEntropies5[],double dangleEnthalpies5[],double tstack2Entropies[],double tstack2Enthalpies[],double atpS[],double atpH[],double Initdouble[],int Initint[],double enthalpyDPT[],double entropyDPT[],char numSeq1[],char numSeq2[])
{
	double S1,H1,T1,S2,H2,T2;

	if(numSeq1[i]+numSeq2[j]!=3)
	{
		entropyDPT[(i-1)*Initint[2]+j-1]=-1.0;
		enthalpyDPT[(i-1)*Initint[2]+j-1]=1.0*INFINITY;
		return;
	}

	S1=atpS[numSeq1[i]*5+numSeq2[j]]+tstack2Entropies[numSeq2[j]*125+numSeq2[j-1]*25+numSeq1[i]*5+numSeq1[i-1]];
	H1=atpH[numSeq1[i]*5+numSeq2[j]]+tstack2Enthalpies[numSeq2[j]*125+numSeq2[j-1]*25+numSeq1[i]*5+numSeq1[i-1]];
	if(fabs(H1)>999999999)
	{
		H1=1.0*INFINITY;
		S1=-1.0;
	}
// If there is two dangling ends at the same end of duplex
	if(fabs(dangleEnthalpies3[numSeq2[j]*25+numSeq2[j-1]*5+numSeq1[i]])<999999999&&fabs(dangleEnthalpies5[numSeq2[j]*25+numSeq1[i]*5+numSeq1[i-1]])<999999999)
	{
		S2=atpS[numSeq1[i]*5+numSeq2[j]]+dangleEntropies3[numSeq2[j]*25+numSeq2[j-1]*5+numSeq1[i]]+dangleEntropies5[numSeq2[j]*25+numSeq1[i]*5+numSeq1[i-1]];
		H2=atpH[numSeq1[i]*5+numSeq2[j]]+dangleEnthalpies3[numSeq2[j]*25+numSeq2[j-1]*5+numSeq1[i]]+dangleEnthalpies5[numSeq2[j]*25+numSeq1[i]*5+numSeq1[i-1]];
		if(fabs(H2)>999999999)
		{
			H2=1.0*INFINITY;
			S2=-1.0;
		}
		T2=(H2+Initdouble[0])/(S2+Initdouble[1]+Initdouble[2]);
		if(fabs(H1)<999999999)
		{
			T1=(H1+Initdouble[0])/(S1+Initdouble[1]+Initdouble[2]);
			if(T1<T2)
			{
				S1=S2;
				H1=H2;
				T1=T2;
			}
		}
		else
		{
			S1=S2;
			H1=H2;
			T1=T2;
		}
	}
	else if(fabs(dangleEnthalpies3[numSeq2[j]*25+numSeq2[j-1]*5+numSeq1[i]])<999999999)
	{
		S2=atpS[numSeq1[i]*5+numSeq2[j]]+dangleEntropies3[numSeq2[j]*25+numSeq2[j-1]*5+numSeq1[i]];
		H2=atpH[numSeq1[i]*5+numSeq2[j]]+dangleEnthalpies3[numSeq2[j]*25+numSeq2[j-1]*5+numSeq1[i]];
		if(fabs(H2)>999999999)
		{
			H2=1.0*INFINITY;
			S2=-1.0;
		}
		T2=(H2+Initdouble[0])/(S2+Initdouble[1]+Initdouble[2]);
		if(fabs(H1)<999999999)
		{
			T1=(H1+Initdouble[0])/(S1+Initdouble[1]+Initdouble[2]);
			if(T1<T2)
			{
				S1=S2;
				H1=H2;
				T1=T2;
			}
		}
		else
		{
			S1=S2;
			H1=H2;
			T1=T2;
		}
	}
	else if(fabs(dangleEnthalpies5[numSeq2[j]*25+numSeq1[i]*5+numSeq1[i-1]])<999999999)
	{
		S2=atpS[numSeq1[i]*5+numSeq2[j]]+dangleEntropies5[numSeq2[j]*25+numSeq1[i]*5+numSeq1[i-1]];
		H2=atpH[numSeq1[i]*5+numSeq2[j]]+dangleEnthalpies5[numSeq2[j]*25+numSeq1[i]*5+numSeq1[i-1]];
		if(fabs(H2)>999999999)
		{
			H2=1.0*INFINITY;
			S2=-1.0;
		}
		T2=(H2+Initdouble[0])/(S2+Initdouble[1]+Initdouble[2]);
		if(fabs(H1)<999999999)
		{
			T1=(H1+Initdouble[0])/(S1+Initdouble[1]+Initdouble[2]);
			if(T1<T2)
			{
				S1=S2;
				H1=H2;
				T1=T2;
			}
		}
		else
		{
			S1=S2;
			H1=H2;
			T1=T2;
		}
	}

	S2=atpS[numSeq1[i]*5+numSeq2[j]];
	H2=atpH[numSeq1[i]*5+numSeq2[j]];
	T2=(H2+Initdouble[0])/(S2+Initdouble[1]+Initdouble[2]);
	if(fabs(H1)<999999999)
	{
		if(T1<T2)
		{
			EntropyEnthalpy[0]=S2;
			EntropyEnthalpy[1]=H2;
		}
		else
		{
			EntropyEnthalpy[0]=S1;
			EntropyEnthalpy[1]=H1;
		}
	}
	else
	{
		EntropyEnthalpy[0]=S2;
		EntropyEnthalpy[1]=H2;
	}
	return;
}

void maxTM(int i,int j,double stackEntropies[],double stackEnthalpies[],double Initdouble[],int Initint[],double enthalpyDPT[],double entropyDPT[],char numSeq1[],char numSeq2[])
{
	double T0,T1,S0,S1,H0,H1;

	S0=entropyDPT[(i-1)*Initint[2]+j-1];
	H0=enthalpyDPT[(i-1)*Initint[2]+j-1];
	T0=(H0+Initdouble[0])/(S0+Initdouble[1]+Initdouble[2]); // at current position 
	if(fabs(enthalpyDPT[(i-2)*Initint[2]+j-2])<999999999&&fabs(Hs(i-1,j-1,1,stackEnthalpies,Initint,numSeq1,numSeq2))<999999999)
	{
		S1=(entropyDPT[(i-2)*Initint[2]+j-2]+Ss(i-1,j-1,1,stackEntropies,Initint,numSeq1,numSeq2));
		H1=(enthalpyDPT[(i-2)*Initint[2]+j-2]+Hs(i-1,j-1,1,stackEnthalpies,Initint,numSeq1,numSeq2));
	}
	else
	{
		S1=-1.0;
		H1=1.0*INFINITY;
	}
	T1=(H1+Initdouble[0])/(S1+Initdouble[1]+Initdouble[2]);

	if(S1<-2500.0)
	{
// to not give dH any value if dS is unreasonable
		S1=-3224.0;
		H1=0.0;
	}
	if(S0<-2500.0)
	{
// to not give dH any value if dS is unreasonable
		S0=-3224.0;
		H0=0.0;
	}
	if((T1>T0)||(S0>0&&H0>0)) // T1 on suurem 
	{
		entropyDPT[(i-1)*Initint[2]+j-1]=S1;
		enthalpyDPT[(i-1)*Initint[2]+j-1]=H1;
	}
	else if(T0>=T1)
	{
		entropyDPT[(i-1)*Initint[2]+j-1]=S0;
		enthalpyDPT[(i-1)*Initint[2]+j-1]=H0;
	}
}

void calc_bulge_internal(int i,int j,int ii,int jj,double* EntropyEnthalpy,int traceback,double stackEntropies[],double stackEnthalpies[],double stackint2Entropies[],double stackint2Enthalpies[],double interiorLoopEntropies[],double bulgeLoopEntropies[],double interiorLoopEnthalpies[],double bulgeLoopEnthalpies[],double tstackEntropies[],double tstackEnthalpies[],double atpS[],double atpH[],double Initdouble[],int Initint[],double enthalpyDPT[],double entropyDPT[],char numSeq1[],char numSeq2[])
{
	int loopSize1,loopSize2,loopSize,N,N_loop;
	double T1,T2,S,H;

	S=-3224.0;
	H=0;
	loopSize1=ii-i-1;
	loopSize2=jj-j-1;
	if(ii<jj)
	{
		N=i;
		N_loop=N;
		if(loopSize1>2)
			N_loop-=(loopSize1-2);
		if(loopSize2>2)
			N_loop-=(loopSize2-2);
	}
	else
	{
		N=j;
		N_loop=2*jj;
		if(loopSize1>2)
			N_loop-=(loopSize1-2);
		if(loopSize2>2)
			N_loop-=(loopSize2-2);
		N_loop=(N_loop/2)-1;
	}

	loopSize=loopSize1+loopSize2-1;
	if((loopSize1==0&&loopSize2>0)||(loopSize2==0&&loopSize1>0))// only bulges have to be considered
	{
		if(loopSize2==1||loopSize1==1) // bulge loop of size one is treated differently the intervening nn-pair must be added
		{
			if((loopSize2==1&&loopSize1==0)||(loopSize2==0&&loopSize1==1))
			{
				H=bulgeLoopEnthalpies[loopSize]+stackEnthalpies[numSeq1[i]*125+numSeq1[ii]*25+numSeq2[j]*5+numSeq2[jj]];
				S=bulgeLoopEntropies[loopSize]+stackEntropies[numSeq1[i]*125+numSeq1[ii]*25+numSeq2[j]*5+numSeq2[jj]];
			}
			H+=enthalpyDPT[(i-1)*Initint[2]+j-1];
			S+=entropyDPT[(i-1)*Initint[2]+j-1];
			if(fabs(H)>999999999)
			{
				H=1.0*INFINITY;
				S=-1.0;
			}

			T1=(H+Initdouble[0])/((S+Initdouble[1])+Initdouble[2]);
			T2=(enthalpyDPT[(ii-1)*Initint[2]+jj-1]+Initdouble[0])/((entropyDPT[(ii-1)*Initint[2]+jj-1])+Initdouble[1]+Initdouble[2]);
			if((T1>T2)||((traceback&&T1>=T2)||(traceback==1)))
			{
				EntropyEnthalpy[0]=S;
				EntropyEnthalpy[1]=H;
			}
		}
		else // we have _not_ implemented Jacobson-Stockaymayer equation; the maximum bulgeloop size is 30
		{
			H=bulgeLoopEnthalpies[loopSize]+atpH[numSeq1[i]*5+numSeq2[j]]+atpH[numSeq1[ii]*5+numSeq2[jj]];
			H+=enthalpyDPT[(i-1)*Initint[2]+j-1];

			S=bulgeLoopEntropies[loopSize]+atpS[numSeq1[i]*5+numSeq2[j]]+atpS[numSeq1[ii]*5+numSeq2[jj]];
			S+=entropyDPT[(i-1)*Initint[2]+j-1];
			if(fabs(H)>999999999)
			{
				H=1.0*INFINITY;
				S=-1.0;
			}
			T1=(H+Initdouble[0])/((S+Initdouble[1])+Initdouble[2]);
			T2=(enthalpyDPT[(ii-1)*Initint[2]+jj-1]+Initdouble[0])/(entropyDPT[(ii-1)*Initint[2]+jj-1]+Initdouble[1]+Initdouble[2]);
			if((T1>T2)||((traceback&&T1>=T2)||(traceback==1)))
			{
				EntropyEnthalpy[0]=S;
				EntropyEnthalpy[1]=H;
			}
		}
	}
	else if(loopSize1==1&&loopSize2==1)
	{
		S=stackint2Entropies[numSeq1[i]*125+numSeq1[i+1]*25+numSeq2[j]*5+numSeq2[j+1]]+stackint2Entropies[numSeq2[jj]*125+numSeq2[jj-1]*25+numSeq1[ii]*5+numSeq1[ii-1]];
		S+=entropyDPT[(i-1)*Initint[2]+j-1];

		H=stackint2Enthalpies[numSeq1[i]*125+numSeq1[i+1]*25+numSeq2[j]*5+numSeq2[j+1]]+stackint2Enthalpies[numSeq2[jj]*125+numSeq2[jj-1]*25+numSeq1[ii]*5+numSeq1[ii-1]];
		H+=enthalpyDPT[(i-1)*Initint[2]+j-1];
		if(fabs(H)>999999999)
		{
			H=1.0*INFINITY;
			S=-1.0;
		}
		T1=(H+Initdouble[0])/((S+Initdouble[1])+Initdouble[2]);
		T2=(enthalpyDPT[(ii-1)*Initint[2]+jj-1]+Initdouble[0])/(entropyDPT[(ii-1)*Initint[2]+jj-1]+Initdouble[1]+Initdouble[2]);
		if((T1-T2>=0.000001)||traceback==1)
		{
			if((T1>T2)||(traceback&&T1>=T2))
			{
				EntropyEnthalpy[0]=S;
				EntropyEnthalpy[1]=H;
			}
		}
		return;
	}
	else // only internal loops
	{
		H=interiorLoopEnthalpies[loopSize]+tstackEnthalpies[numSeq1[i]*125+numSeq1[i+1]*25+numSeq2[j]*5+numSeq2[j+1]]+tstackEnthalpies[numSeq2[jj]*125+numSeq2[jj-1]*25+numSeq1[ii]*5+numSeq1[ii-1]];
		H+=enthalpyDPT[(i-1)*Initint[2]+j-1];

		S=interiorLoopEntropies[loopSize]+tstackEntropies[numSeq1[i]*125+numSeq1[i+1]*25+numSeq2[j]*5+numSeq2[j+1]]+tstackEntropies[numSeq2[jj]*125+numSeq2[jj-1]*25+numSeq1[ii]*5+numSeq1[ii-1]]+(-300/310.15*abs(loopSize1-loopSize2));
		S+=entropyDPT[(i-1)*Initint[2]+j-1];
		if(fabs(H)>999999999)
		{
			H=1.0*INFINITY;
			S=-1.0;
		}
		T1=(H+Initdouble[0])/((S+Initdouble[1])+Initdouble[2]);
		T2=(enthalpyDPT[(ii-1)*Initint[2]+jj-1]+Initdouble[0])/((entropyDPT[(ii-1)*Initint[2]+jj-1])+Initdouble[1]+Initdouble[2]);
		if((T1>T2)||((traceback&&T1>=T2)||(traceback==1)))
		{
			EntropyEnthalpy[0]=S;
			EntropyEnthalpy[1]=H;
		}
	}
	return;
}

void fillMatrix(double stackEntropies[],double stackEnthalpies[],double stackint2Entropies[],double stackint2Enthalpies[],double dangleEntropies3[],double dangleEnthalpies3[],double dangleEntropies5[],double dangleEnthalpies5[],double interiorLoopEntropies[],double bulgeLoopEntropies[],double interiorLoopEnthalpies[],double bulgeLoopEnthalpies[],double tstackEntropies[],double tstackEnthalpies[],double tstack2Entropies[],double tstack2Enthalpies[],double atpS[],double atpH[],double Initdouble[],int Initint[],double enthalpyDPT[],double entropyDPT[],char numSeq1[],char numSeq2[])
{
	int d,i,j,ii,jj;
	double SH[2];

	for(i=1;i<=Initint[0];++i)
	{
		for(j=1;j<=Initint[1];++j)
		{
			if(fabs(enthalpyDPT[(i-1)*Initint[2]+j-1])<999999999)
			{
				SH[0]=-1.0;
				SH[1]=1.0*INFINITY;
				LSH(i,j,SH,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,tstack2Entropies,tstack2Enthalpies,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,numSeq1,numSeq2);

				if(fabs(SH[1])<999999999)
				{
					entropyDPT[(i-1)*Initint[2]+j-1]=SH[0];
					enthalpyDPT[(i-1)*Initint[2]+j-1]=SH[1];
				}
				if(i>1&&j>1)
				{
					maxTM(i,j,stackEntropies,stackEnthalpies,Initdouble,Initint,enthalpyDPT,entropyDPT,numSeq1,numSeq2);
					for(d=3;d<=32;d++)
					{
						ii=i-1;
						jj=-ii-d+(j+i);
						if(jj<1)
						{
							ii-=abs(jj-1);
							jj=1;
						}
						for(;ii>0&&jj<j;--ii,++jj)
						{
							if(fabs(enthalpyDPT[(ii-1)*Initint[2]+jj-1])<999999999)
							{
								SH[0]=-1.0;
								SH[1]=1.0*INFINITY;
								calc_bulge_internal(ii,jj,i,j,SH,0,stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,interiorLoopEntropies,bulgeLoopEntropies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,numSeq1,numSeq2);

								if(SH[0]<-2500.0)
								{
									SH[0] =-3224.0;
									SH[1] = 0.0;
								}
								if(fabs(SH[1])<999999999)
								{
									enthalpyDPT[(i-1)*Initint[2]+j-1]=SH[1];
									entropyDPT[(i-1)*Initint[2]+j-1]=SH[0];
								}
							}
						}
					}
				} // if 
			}
		} // for 
	} //for
}

void RSH(int i,int j,double EntropyEnthalpy[],double dangleEntropies3[],double dangleEnthalpies3[],double dangleEntropies5[],double dangleEnthalpies5[],double tstack2Entropies[],double tstack2Enthalpies[],double atpS[],double atpH[],double Initdouble[],char numSeq1[],char numSeq2[])
{
	double S1,S2,H1,H2,T1,T2;

	if(numSeq1[i]+numSeq2[j]!=3)
	{
		EntropyEnthalpy[0]=-1.0;
		EntropyEnthalpy[1]=1.0*INFINITY;
		return;
	}
	S1=atpS[numSeq1[i]*5+numSeq2[j]]+tstack2Entropies[numSeq1[i]*125+numSeq1[i+1]*25+numSeq2[j]*5+numSeq2[j+1]];
	H1=atpH[numSeq1[i]*5+numSeq2[j]]+tstack2Enthalpies[numSeq1[i]*125+numSeq1[i+1]*25+numSeq2[j]*5+numSeq2[j+1]];
	if(fabs(H1)>999999999)
	{
		H1=1.0*INFINITY;
		S1=-1.0;
	}
	if(fabs(dangleEnthalpies3[numSeq1[i]*25+numSeq1[i+1]*5+numSeq2[j]])<999999999&&fabs(dangleEnthalpies5[numSeq1[i]*25+numSeq2[j]*5+numSeq2[j+1]])<999999999)
	{
		S2=atpS[numSeq1[i]*5+numSeq2[j]]+dangleEntropies3[numSeq1[i]*25+numSeq1[i+1]*5+numSeq2[j]]+dangleEntropies5[numSeq1[i]*25+numSeq2[j]*5+numSeq2[j+1]];
		H2=atpH[numSeq1[i]*5+numSeq2[j]]+dangleEnthalpies3[numSeq1[i]*25+numSeq1[i+1]*5+numSeq2[j]]+dangleEnthalpies5[numSeq1[i]*25+numSeq2[j]*5+numSeq2[j+1]];
		if(fabs(H2)>999999999)
		{
			H2=1.0*INFINITY;
			S2=-1.0;
		}
		T2=(H2+Initdouble[0])/(S2+Initdouble[1]+Initdouble[2]);
		if(fabs(H1)<999999999)
		{
			T1=(H1+Initdouble[0])/(S1+Initdouble[1]+Initdouble[2]);
			if(T1<T2)
			{
				S1=S2;
				H1=H2;
				T1=T2;
			}
		}
		else
		{
			S1=S2;
			H1=H2;
			T1=T2;
		}
	}

	if(fabs(dangleEnthalpies3[numSeq1[i]*25+numSeq1[i+1]*5+numSeq2[j]])<999999999)
	{
		S2=atpS[numSeq1[i]*5+numSeq2[j]]+dangleEntropies3[numSeq1[i]*25+numSeq1[i+1]*5+numSeq2[j]];
		H2=atpH[numSeq1[i]*5+numSeq2[j]]+dangleEnthalpies3[numSeq1[i]*25+numSeq1[i+1]*5+numSeq2[j]];
		if(fabs(H2)>999999999)
		{
			H2=1.0*INFINITY;
			S2=-1.0;
		}
		T2=(H2+Initdouble[0])/(S2+Initdouble[1]+Initdouble[2]);
		if(fabs(H1)<999999999)
		{
			T1=(H1+Initdouble[0])/(S1+Initdouble[1]+Initdouble[2]);
			if(T1<T2)
			{
				S1=S2;
				H1=H2;
				T1=T2;
			}
		}
		else
		{
			S1=S2;
			H1=H2;
			T1=T2;
		}
	}

	if(fabs(dangleEnthalpies5[numSeq1[i]*25+numSeq2[j]*5+numSeq2[j+1]])<999999999)
	{
		S2=atpS[numSeq1[i]*5+numSeq2[j]]+dangleEntropies5[numSeq1[i]*25+numSeq2[j]*5+numSeq2[j+1]];
		H2=atpH[numSeq1[i]*5+numSeq2[j]]+dangleEnthalpies5[numSeq1[i]*25+numSeq2[j]*5+numSeq2[j+1]];
		if(fabs(H2)>999999999)
		{
			H2=1.0*INFINITY;
			S2=-1.0;
		}
		T2=(H2+Initdouble[0])/(S2+Initdouble[1]+Initdouble[2]);
		if(fabs(H1)<999999999)
		{
			T1=(H1+Initdouble[0])/(S1+Initdouble[1]+Initdouble[2]);
			if(T1<T2)
			{
				S1=S2;
				H1=H2;
				T1=T2;
			}
		}
		else
		{
			S1=S2;
			H1=H2;
			T1=T2;
		}
	}
	S2=atpS[numSeq1[i]*5+numSeq2[j]];
	H2=atpH[numSeq1[i]*5+numSeq2[j]];
	T2=(H2+Initdouble[0])/(S2+Initdouble[1]+Initdouble[2]);
	if(fabs(H1)<999999999)
	{
		if(T1<T2)
		{
			EntropyEnthalpy[0]=S2;
			EntropyEnthalpy[1]=H2;
		}
		else
		{
			EntropyEnthalpy[0]=S1;
			EntropyEnthalpy[1]=H1;
		}
	}
	else
	{
		EntropyEnthalpy[0]=S2;
		EntropyEnthalpy[1]=H2;
	}
	return;
}

void traceback(int i,int j,int* ps1,int* ps2,double stackEntropies[],double stackEnthalpies[],double stackint2Entropies[],double stackint2Enthalpies[],double dangleEntropies3[],double dangleEnthalpies3[],double dangleEntropies5[],double dangleEnthalpies5[],double interiorLoopEntropies[],double bulgeLoopEntropies[],double interiorLoopEnthalpies[],double bulgeLoopEnthalpies[],double tstackEntropies[],double tstackEnthalpies[],double tstack2Entropies[],double tstack2Enthalpies[],double atpS[],double atpH[],double Initdouble[],int Initint[],double enthalpyDPT[],double entropyDPT[],char numSeq1[],char numSeq2[])
{
	int d,ii,jj,done;
	double SH[2];

	ps1[i-1]=j;
	ps2[j-1]=i;
	while(1)
	{
		SH[0]=-1.0;
		SH[1]=1.0*INFINITY;
		LSH(i,j,SH,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,tstack2Entropies,tstack2Enthalpies,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,numSeq1,numSeq2);
		if(equal(entropyDPT[(i-1)*Initint[2]+j-1],SH[0])&&equal(enthalpyDPT[(i-1)*Initint[2]+j-1],SH[1]))
			break;

		done = 0;
		if(i>1&&j>1&&equal(entropyDPT[(i-1)*Initint[2]+j-1],Ss(i-1,j-1,1,stackEntropies,Initint,numSeq1,numSeq2)+entropyDPT[(i-2)*Initint[2]+j-2]))
		{
			i=i-1;
			j=j-1;
			ps1[i-1]=j;
			ps2[j-1]=i;
			done=1;
		}
		for(d=3;!done&&d<=32;++d)
		{
			ii=i-1;
			jj=-ii-d+(j+i);
			if(jj<1)
			{
				ii-=abs(jj-1);
				jj=1;
			}
			for(;!done&&ii>0&&jj<j;--ii,++jj)
			{
				SH[0]=-1.0;
				SH[1]=1.0*INFINITY;
				calc_bulge_internal(ii,jj,i,j,SH,1,stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,interiorLoopEntropies,bulgeLoopEntropies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,numSeq1,numSeq2);
				if(equal(entropyDPT[(i-1)*Initint[2]+j-1],SH[0])&&equal(enthalpyDPT[(i-1)*Initint[2]+j-1],SH[1]))
				{
					i=ii;
					j=jj;
					ps1[i-1]=j;
					ps2[j-1]=i;
					done=1;
					break;
				}
			}
		}
	}
}

double drawDimer(int *ps1,int *ps2,double H,double S,double Initdouble[],int Initint[])
{
        int i,N;

        if(fabs(Initdouble[3])>999999999)
                return 0.0;
        else
        {
                N=0;
                for(i=0;i<Initint[0];i++)
                {
                        if(ps1[i]>0)
                                ++N;
                }
                for(i=0;i<Initint[1];i++)
                {
                        if(ps2[i]>0)
                                ++N;
                }
                N=(N/2)-1;
                return (H/(S+(N*-0.51986)+Initdouble[2]))-273.15;
        }
}

int symmetry_thermo(char seq[])
{
        int i = 0;
        int seq_len=strlen(seq);
        int mp = seq_len/2;
        if(seq_len%2==1)
                return 0;          

        while(i<mp) 
        {
                if((seq[i]=='A'&&seq[seq_len-1-i]!='T')||(seq[i]=='T'&&seq[seq_len-1-i]!='A')||(seq[seq_len-1-i]=='A'&&seq[i]!='T')||(seq[seq_len-1-i]=='T'&&seq[i]!='A'))
                        return 0;   
                if((seq[i]=='C'&&seq[seq_len-1-i]!='G')||(seq[i]=='G'&&seq[seq_len-1-i]!='C')||(seq[seq_len-1-i]=='C'&&seq[i]!='G')||(seq[seq_len-1-i]=='G'&&seq[i]!='C'))
                        return 0;
		i++;
        }
        return 1;
}

double thal(char oligo_f[],char oligo_r[],double stackEntropies[],double stackEnthalpies[],double stackint2Entropies[],double stackint2Enthalpies[],double dangleEntropies3[],double dangleEnthalpies3[],double dangleEntropies5[],double dangleEnthalpies5[],double hairpinLoopEntropies[],double interiorLoopEntropies[],double bulgeLoopEntropies[],double hairpinLoopEnthalpies[],double interiorLoopEnthalpies[],double bulgeLoopEnthalpies[],double tstackEntropies[],double tstackEnthalpies[],double tstack2Entropies[],double tstack2Enthalpies[],char *triloopEntropies1,char *triloopEnthalpies1,char *tetraloopEntropies1,char *tetraloopEnthalpies1,double *triloopEntropies2,double *triloopEnthalpies2,double *tetraloopEntropies2,double *tetraloopEnthalpies2,int numTriloops,int numTetraloops,double atpS[],double atpH[],int type)
{
	double SH[2],Initdouble[4];//0 is dplx_init_H, 1 is dplx_init_S, 2 is RC, 3 is SHleft
	int Initint[5]; //0 is len1, 1 is len2, 2 is len3, 3 is bestI, 4 is bestJ
	int i, j;
	double T1,enthalpyDPT[625],entropyDPT[625],send5[26],hend5[26],result_TH;
	int ps1[25],ps2[25];
	char numSeq1[27],numSeq2[27];
	double mh, ms;

/*** INIT values for unimolecular and bimolecular structures ***/
	if (type==4) /* unimolecular folding */
	{
		Initdouble[0]= 0.0;
		Initdouble[1] = -0.00000000001;
		Initdouble[2]=0;
	}
	else /* hybridization of two oligos */
	{
		Initdouble[0]= 200;
		Initdouble[1]= -5.7;
		if(symmetry_thermo(oligo_f) && symmetry_thermo(oligo_r))
			Initdouble[2]=1.9872* log(38/1000000000.0);
		else
			Initdouble[2]=1.9872* log(38/4000000000.0);
	}
/* convert nucleotides to numbers */
	if(type==1 || type==2)
	{
		Initint[0]=strlen(oligo_f);
		Initint[1]=strlen(oligo_r);
	 	for(i=1;i<=Initint[0];++i)
			numSeq1[i]=str2int(oligo_f[i-1]);
		for(i=1;i<=Initint[1];++i)
			numSeq2[i]=str2int(oligo_r[Initint[1]-i]);
	}
	else if(type==3)
	{
		Initint[0]=strlen(oligo_r);
		Initint[1]=strlen(oligo_f);
		for(i=1;i<=Initint[0];++i)
			numSeq1[i]=str2int(oligo_r[i-1]);
		for(i=1;i<=Initint[1];++i)
			numSeq2[i]=str2int(oligo_f[Initint[1]-i]);
	}
	else
	{
		Initint[0]=strlen(oligo_f);
                Initint[1]=strlen(oligo_r);
		Initint[2]=Initint[1]-1;
                for(i=1;i<=Initint[0];++i)      
                        numSeq1[i]=str2int(oligo_f[i-1]);   
                for(i=1;i<=Initint[1];++i)      
                        numSeq2[i]=str2int(oligo_r[i-1]);
	}
	numSeq1[0]=numSeq1[Initint[0]+1]=numSeq2[0]=numSeq2[Initint[1]+1]=4; /* mark as N-s */

	result_TH=0;
	if (type==4) /* calculate structure of monomer */
	{
		initMatrix2(Initint,enthalpyDPT,entropyDPT,numSeq1);
		fillMatrix2(stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,numSeq1,numSeq2);
		calc_terminal_bp(310.15,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,tstack2Entropies,tstack2Enthalpies,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,send5,hend5,numSeq1);
		mh=hend5[Initint[0]];
		ms=send5[Initint[0]];
		for (i=0;i<Initint[0];i++)
			ps1[i]=0;
		if(fabs(mh)<999999999)
		{
			tracebacku(ps1,stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,send5,hend5,numSeq1,numSeq2);
			result_TH=drawHairpin(ps1,mh,ms,Initint);
			result_TH=(int)(result_TH*100+0.5)/100.0;
		}
	}
	else if(type!=4) /* Hybridization of two moleculs */
	{
		Initint[2]=Initint[1];
		initMatrix(Initint,enthalpyDPT,entropyDPT,numSeq1,numSeq2);
		fillMatrix(stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,interiorLoopEntropies,bulgeLoopEntropies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,numSeq1,numSeq2);

		Initdouble[3]=-1.0*INFINITY;
	/* calculate terminal basepairs */
		Initint[3]=Initint[4]=0;
		if(type==1)
			for (i=1;i<=Initint[0];i++)
			{
				for (j=1;j<=Initint[1];j++)
				{
					RSH(i,j,SH,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,tstack2Entropies,tstack2Enthalpies,atpS,atpH,Initdouble,numSeq1,numSeq2);
					SH[0]=SH[0]+0.000001; /* this adding is done for compiler, optimization -O2 vs -O0 */
					SH[1]=SH[1]+0.000001;
					T1=((enthalpyDPT[(i-1)*Initint[2]+j-1]+ SH[1] +Initdouble[0]) / ((entropyDPT[(i-1)*Initint[2]+j-1]) + SH[0] +Initdouble[1] + Initdouble[2])) -273.15;
					if(T1>Initdouble[3]&&((entropyDPT[(i-1)*Initint[2]+j-1]+SH[0])<0&&(SH[1]+enthalpyDPT[(i-1)*Initint[2]+j-1])<0))
					{
						Initdouble[3]=T1;
						Initint[3]=i;
						Initint[4]=j;
					}
				}
			}
		if(type==2||type==3)
		{
		 //THAL_END1
			Initint[4]=0;
			Initint[3]=Initint[0];
			i=Initint[0];
			Initdouble[3]=-1.0*INFINITY;
			for (j=1;j<=Initint[1];++j)
			{
				RSH(i,j,SH,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,tstack2Entropies,tstack2Enthalpies,atpS,atpH,Initdouble,numSeq1,numSeq2);
				SH[0]=SH[0]+0.000001; // this adding is done for compiler, optimization -O2 vs -O0,that compiler could understand that SH is changed in this cycle 
				SH[1]=SH[1]+0.000001;
				T1=((enthalpyDPT[(i-1)*Initint[2]+j-1]+SH[1]+Initdouble[0])/((entropyDPT[(i-1)*Initint[2]+j-1])+SH[0]+Initdouble[1]+Initdouble[2]))-273.15;
				if (T1>Initdouble[3]&&((SH[0]+entropyDPT[(i-1)*Initint[2]+j-1])<0&&(SH[1]+enthalpyDPT[(i-1)*Initint[2]+j-1])<0))
				{
					Initdouble[3]=T1;
					Initint[4]=j;
				}
			}
		}
		if(fabs(Initdouble[3])>999999999)
			Initint[3]=Initint[4]=1;
		RSH(Initint[3], Initint[4], SH,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,tstack2Entropies,tstack2Enthalpies,atpS,atpH,Initdouble,numSeq1,numSeq2);
	 // tracebacking 
		for (i=0;i<Initint[0];++i)
			ps1[i]=0;
		for (j=0;j<Initint[1];++j)
			ps2[j] = 0;
		if(fabs(enthalpyDPT[(Initint[3]-1)*Initint[2]+Initint[4]-1])<999999999)
		{
			traceback(Initint[3],Initint[4],ps1,ps2,stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,interiorLoopEntropies,bulgeLoopEntropies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,atpS,atpH,Initdouble,Initint,enthalpyDPT,entropyDPT,numSeq1,numSeq2);
			result_TH=drawDimer(ps1,ps2,(enthalpyDPT[(Initint[3]-1)*Initint[2]+Initint[4]-1]+SH[1]+Initdouble[0]),(entropyDPT[(Initint[3]-1)*Initint[2]+Initint[4]-1]+SH[0]+Initdouble[1]),Initdouble,Initint);
			result_TH=(int)(result_TH*100+0.5)/100.0;
		}
	}
	return result_TH;
}

struct Node
{
	int pos;
	int gi;
	int plus;  //as a flag, 1 is OK, 0 is no
	int minus; //as a flag
	struct Node *next;
};

struct INFO
{
	char name[301];
	int turn;
	struct INFO *next;
};

struct Primer
{
	int pos;
	int len;
	int plus;
	int minus;
	int total;  //how many GIs can be used
	float Tm;
	struct Primer *self;
	struct Primer *loop;
	struct Primer *notloop;
	struct Primer *next;
	struct Node *common;
	struct Node *special;
};

//get the file size
int file_size2(char *filename)
{
	int size;
        struct stat statbuf;

        stat(filename,&statbuf);
        size=statbuf.st_size;
        return size;
}

void usage()
{
        printf("USAGE:\n");
        printf("  LAMP_CPU  -in <sinlge_primers_file>  -ref <ref_genome> -out <LAMP_primer_sets> [options]*\n\n");
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
	printf("  -fast\n");
	printf("    fast mode to design LAMP primer sets, in this mode GLAPD may lost some right results\n");
        printf("  -h/-help\n");
	printf("    print usage\n");
}

void generate_primer(char *seq,char primer[],int start,int length,int flag) //flag=0:plus
{
        int i;
        
        if(flag==0)
        {
                for(i=0;i<length;i++)
                        primer[i]=seq[start+i];
                primer[i]='\0';
        }
        else
        {
                for(i=0;i<length;i++)
                {
                        if((seq[start+length-1-i]=='A')||(seq[start+length-1-i]=='a'))
                                primer[i]='T';
                        else if((seq[start+length-1-i]=='T')||(seq[start+length-1-i]=='t'))
                                primer[i]='A';
                        else if((seq[start+length-1-i]=='C')||(seq[start+length-1-i]=='c'))
                                primer[i]='G';
                        else 
                                primer[i]='C';
                }
                primer[length]='\0';
        }
}

int check_same(struct Primer *one[],struct Primer *two[],struct Primer *p_F3,struct Primer *p_F2,struct Primer *p_F1c,struct Primer *p_B1c,struct Primer *p_B2,struct Primer *p_B3)
{
	int i;
	for(i=0;i<10;i++)
	{
		if(one[i]==NULL)
			return 0;
		if(one[i]==p_F3)
		{
			if(two[i]==p_F2)
				return 1;
			if(two[i]==p_F1c)
				return 1;
			if(two[i]==p_B1c)
				return 1;
			if(two[i]==p_B2)
				return 1;
			if(two[i]==p_B3)
				return 1;
		}
		else if(one[i]==p_F2)
		{
			if(two[i]==p_F1c)
                                return 1;
                        if(two[i]==p_B1c)
                                return 1;
                        if(two[i]==p_B2)
                                return 1;
                        if(two[i]==p_B3)
                                return 1;
		}
		else if(one[i]==p_F1c)
		{
			if(two[i]==p_B1c)
                                return 1;
                        if(two[i]==p_B2)
                                return 1;
                        if(two[i]==p_B3)
                                return 1;
		}
		else if(one[i]==p_B1c)
		{
			if(two[i]==p_B2) 
                                return 1;
                        if(two[i]==p_B3)
                                return 1;
		}
		else if(one[i]==p_B2)
		{
			if(two[i]==p_B3)
                                return 1;
		}
	}
	return 0;
}

int add_same(struct Primer *one[],struct Primer *two[],struct Primer *p_F3,struct Primer *p_F2,struct Primer *p_F1c,struct Primer *p_B1c,struct Primer *p_B2,struct Primer *p_B3,int error[],int replace)
{
	if(error[0]==0)
	{
		one[replace]=p_F3;
		if(error[1]==1)
			two[replace]=p_F2;
		else if(error[1]==2)
			two[replace]=p_F1c;
		else if(error[1]==3)
			two[replace]=p_B1c;
		else if(error[1]==4)
			two[replace]=p_B2;
		else
			two[replace]=p_B3;
	}
	else if(error[0]==1)
	{
		one[replace]=p_F2;
		if(error[1]==2)
                        two[replace]=p_F1c;
                else if(error[1]==3)
                        two[replace]=p_B1c;
                else if(error[1]==4)
                        two[replace]=p_B2;
                else
                        two[replace]=p_B3;
	}
	else if(error[0]==2)
        {
                one[replace]=p_F1c;
		if(error[1]==3)
                        two[replace]=p_B1c;
                else if(error[1]==4)
                        two[replace]=p_B2;
                else
                        two[replace]=p_B3;
        }
	else if(error[0]==3)
        {
		one[replace]=p_B1c;
		if(error[1]==4)
                        two[replace]=p_B2;
                else
                        two[replace]=p_B3;
        }
	else
        {
                one[replace]=p_B2;
		two[replace]=p_B3;
        }
	replace++;
	if(replace==10)
		replace=0;
	return replace;
}

void main(int argc,char **argv)
{
	double stackEntropies[625],stackEnthalpies[625],stackint2Entropies[625],stackint2Enthalpies[625],dangleEntropies3[125],dangleEnthalpies3[125],dangleEntropies5[125],dangleEnthalpies5[125];
        double hairpinLoopEntropies[30],interiorLoopEntropies[30],bulgeLoopEntropies[30],hairpinLoopEnthalpies[30],interiorLoopEnthalpies[30],bulgeLoopEnthalpies[30],tstackEntropies[625],tstackEnthalpies[625],tstack2Entropies[625],tstack2Enthalpies[625];
        double *triloopEntropies2,*triloopEnthalpies2,*tetraloopEntropies2,*tetraloopEnthalpies2,atpS[25],atpH[25];
	int i,j,*result,new,*best_par,expect,flag[13],common_num[1],turn,success,numTriloops,numTetraloops,max_loop,min_loop,error[2],replace,GC;
	char *output,*prefix,*store_path,*path_fa,*inner,*outer,*loop,*par_path;
	char *temp,*seq,F3[26],F2[26],F1c[26],B1c[26],B2[26],B3[26],LF[26],LB[26],*triloopEntropies1,*triloopEnthalpies1,*tetraloopEntropies1,*tetraloopEnthalpies1;
	FILE *fp;
	struct Primer *headL,*headS,*headLoop,*p_F3,*p_F2,*p_F1c,*p_B1c,*p_B2,*p_B3,*result_Loop[2],*one[10],*two[10];
	struct Node *p_node,*p_temp;
	struct INFO *headList,*p_list;
	time_t start,end,begin;
	
	struct Primer *read_par();
	struct INFO *read_list();
	void next_one();
	int check_common();
	void how_many();
	void add();
	int check_add();
	int check_gc();
	int check_uniq();
	int check_structure();
	int design_loop();

	start=time(NULL);
	begin=start;
        for(i=0;i<13;i++)
        {
                flag[i]=0;
        }
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
                else if(strcmp(argv[i],"-loop")==0) 
                {
                        flag[10]=1;
                        i++;
                }
		else if(strcmp(argv[i],"-fast")==0)
		{
			flag[12]=1;
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
			i++;
		}
		else if(strcmp(argv[i],"-specific")==0)
		{
			flag[6]=1;
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
	if(flag[4]==0)
		expect=10;

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
		if(par_path[j]!='/')
                {
                        par_path[j+1]='/';
			par_path[j+2]='\0';
                }
                strcat(par_path,"Par/");
        }
	if(flag[7])
	{
		getStack(stackEntropies,stackEnthalpies,par_path);
                getStackint2(stackint2Entropies,stackint2Enthalpies,par_path);
                getDangle(dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,par_path);
                getLoop(hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,par_path);
                getTstack(tstackEntropies,tstackEnthalpies,par_path);
                getTstack2(tstack2Entropies,tstack2Enthalpies,par_path);

                numTriloops=get_num_line(par_path,0);
                triloopEntropies1=(char *)malloc(numTriloops*5);
                triloopEnthalpies1=(char *)malloc(numTriloops*5);
                triloopEntropies2=(double *)malloc(numTriloops*sizeof(double));
                triloopEnthalpies2=(double *)malloc(numTriloops*sizeof(double));
                getTriloop(triloopEntropies1,triloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,par_path);
        
                numTetraloops=get_num_line(par_path,1);
                tetraloopEntropies1=(char *)malloc(numTetraloops*6);
                tetraloopEnthalpies1=(char *)malloc(numTetraloops*6);
                tetraloopEntropies2=(double *)malloc(numTetraloops*sizeof(double));
                tetraloopEnthalpies2=(double *)malloc(numTetraloops*sizeof(double));
                getTetraloop(tetraloopEntropies1,tetraloopEnthalpies1,tetraloopEntropies2,tetraloopEnthalpies2,par_path);
                tableStartATS(6.9,atpS);
                tableStartATH(2200.0,atpH);
        }

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

//common-list
	if(flag[5])
	{
		headList=read_list(inner,common_num);
		common_num[0]++;
	}
	else
		common_num[0]=1;

//read parameters
	headS=read_par(outer,flag[5],flag[6]);
	headL=read_par(inner,flag[5],flag[6]);
	if(flag[10])
	{
		headLoop=read_par(loop,flag[5],0);//don't use special info
		p_F3=headLoop;
		while(p_F3->next!=NULL)
			p_F3=p_F3->next;
		max_loop=p_F3->pos;
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

//the next one 
	next_one(headL,headL,0);
	next_one(headS,headS,0);
	next_one(headS,headL,1);
	next_one(headL,headS,1);
	if(flag[10]==1)
	{
		next_one(headS,headLoop,2);
		next_one(headL,headLoop,2);
	}

	if(flag[5])
		result=(int *)malloc(common_num[0]*sizeof(int));
	best_par=(int *)malloc(expect*sizeof(int)); //include LF/LB
	for(i=0;i<expect;i++)
                best_par[i]=-1;
	fp=fopen(output,"w");
        if(fp==NULL)
        {
                printf("Error: can't create the %s file!\n",output);
                exit(1);
        }
	end=time(NULL);
	printf("It takes %0.0f seconds to prepare data.\n",difftime(end,start));

//design LAMP primers
	start=time(NULL);
	if(flag[12]==0)
	{
		for(i=0;i<10;i++)
		{
			one[i]=NULL;
			two[i]=NULL;
		}
		replace=0;
	}
	turn=0;
	for(j=common_num[0];j>=1;j--)
	{
		if(common_num[0]>1)
			printf("Running: amplify %d target genome.\n",j);
        	for(p_F3=headS;p_F3;p_F3=p_F3->next)   //F3
        	{
			if(flag[10]&&(p_F3->pos+200)<min_loop)
				continue;
			if(flag[10]&&(p_F3->pos-200)>max_loop)
				break;
			if(p_F3->total<j)
				continue;
			if(p_F3->plus==0)
				continue;
			success=check_add(p_F3->pos,best_par,expect); //in best_par are sorted primers by common
			if(success==0)
				continue;
			new=0;  //whether find a new primer, if find, adjust F3 position
                	for(p_F2=p_F3->self;p_F2;p_F2=p_F2->next)  //F2
               		{
				flag[0]=p_F2->plus&p_F3->plus;
				if(flag[0]==0)
					continue;
				if(p_F2->total<j)
                        		continue;
                	        if(p_F2->pos-(p_F3->pos+p_F3->len)>20)
                	                break;
                	        for(p_F1c=p_F2->notloop;p_F1c;p_F1c=p_F1c->next)   //F1c
                       		{
					flag[1]=flag[0]&p_F1c->minus;
					if(flag[1]==0)
						continue;
					if(p_F1c->total<j)
						continue;
				//Tm
					if(p_F1c->Tm-p_F3->Tm<3)
						continue;
					if(p_F1c->Tm-p_F2->Tm<3)
						continue;

					if(p_F1c->pos-p_F2->pos-1<40)
						continue;
                	                if(p_F1c->pos-p_F2->pos-1>60)
                       	                	break;
                       	        	for(p_B1c=p_F1c->self;p_B1c;p_B1c=p_B1c->next)   //B1c
                       	        	{
						flag[2]=flag[1]&p_B1c->plus;
						if(flag[2]==0)
							continue;
                       	                        if(p_B1c->total<j)
                       	                                continue;
					//Tm
						if(p_B1c->Tm-p_F3->Tm<3)
							continue;
						if(p_B1c->Tm-p_F2->Tm<3)
							continue;

                	                        if(p_B1c->pos-p_F1c->pos>85)
                       	                        	break;
                       	                	for(p_B2=p_B1c->notloop;p_B2;p_B2=p_B2->next)   //B2
                       	                	{
							flag[3]=flag[2]&p_B2->minus;
							if(flag[3]==0)
								continue;
							if(p_B2->total<j)
								continue;
						//Tm
							if(p_F1c->Tm-p_B2->Tm<3)
								continue;
							if(p_B1c->Tm-p_B2->Tm<3)
								continue;

							if((p_B2->pos+p_B2->len-1)-(p_B1c->pos+p_B1c->len-1)-1<40)
								continue;
	               	                                if((p_B2->pos+p_B2->len-1)-(p_B1c->pos+p_B1c->len-1)-1>60)
                       	                                	break;
                       	                        	if(p_B2->pos+p_B2->len-1-p_F2->pos-1<120)
                       	                                	continue;
                               	                	if(p_B2->pos+p_B2->len-1-p_F2->pos-1>180)
                               	                        	break;
					//check whether has enough positions for loop
							if(flag[10]&&((p_F1c->pos-p_F2->pos-p_F2->len)<18)&&(p_B2->pos-p_B1c->pos-p_B1c->len<18))
								continue;
							for(p_B3=p_B2->self;p_B3;p_B3=p_B3->next)  //B3
                        				{
								if(p_B3->pos<p_B2->pos+p_B2->len)
                                                                        continue;
                                                                if(p_B3->pos-(p_B2->pos+p_B2->len)>20)
                                                                        break;
				                                if(p_B3->total<j)
				                                        continue;
								flag[4]=flag[3]&p_B3->minus;
								if(flag[4]==0)
									continue;
								GC=check_gc(seq,p_F3->pos,(p_B3->pos+p_B3->len));
								if((GC&flag[4])!=GC)
									continue;
							//Tm
								if(p_F1c->Tm-p_B3->Tm<3)
									continue;
								if(p_B1c->Tm-p_B3->Tm<3)
									continue;

                                                                if(flag[12])
                                                                        new=1;
								else
								{
									success=check_same(one,two,p_F3,p_F2,p_F1c,p_B1c,p_B2,p_B3);
									if(success)
									{
										if(new)
											break;
										else
											continue;
									}
								}
                                                        //check uniq and common
								if(flag[6])
								{
									success=check_uniq(p_F3,p_F2,p_F1c,p_B1c,p_B2,p_B3);
									if(success==0)
									{
										if(new)
											break;
										else
											continue;
									}
								}
								if(flag[5])
								{
									success=check_common(p_F3,p_F2,p_F1c,p_B1c,p_B2,p_B3,result,common_num[0]);
									if(success!=j)
									{
										if(new)
											break;
										else
											continue;
									}
								}
							//check second structure
								generate_primer(seq,F3,p_F3->pos,p_F3->len,0);
								generate_primer(seq,F2,p_F2->pos,p_F2->len,0);
								generate_primer(seq,F1c,p_F1c->pos,p_F1c->len,1);
								generate_primer(seq,B1c,p_B1c->pos,p_B1c->len,0);
								generate_primer(seq,B2,p_B2->pos,p_B2->len,1);
								generate_primer(seq,B3,p_B3->pos,p_B3->len,1);
								if(flag[7])
								{
									success=check_structure(F3,F2,F1c,B1c,B2,B3,GC,stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,error);
									if(success==0)
									{
										replace=add_same(one,two,p_F3,p_F2,p_F1c,p_B1c,p_B2,p_B3,error,replace);
										if(new)
											break;
										else
											continue;
									}
								}
							//design loop
								if(flag[10])
								{
									success=design_loop(p_F3,p_F2,p_F2->loop,p_F1c,p_B1c,p_B1c->loop,p_B2,p_B3,result_Loop,result,flag,common_num[0],seq,stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,GC);
									if(success==0)
									{
										if(new)
											break;
										else
											continue;
									}
								}
								best_par[turn]=p_F3->pos;

							//output
								turn++;
								fprintf(fp,"The %d LAMP primers:\n",turn);
								fprintf(fp,"  F3: pos:%d,length:%d bp, primer(5'-3'):%s\n",p_F3->pos,p_F3->len,F3);
								fprintf(fp,"  F2: pos:%d,length:%d bp, primer(5'-3'):%s\n",p_F2->pos,p_F2->len,F2);
								fprintf(fp,"  F1c: pos:%d,length:%d bp, primer(5'-3'):%s\n",p_F1c->pos,p_F1c->len,F1c);
								fprintf(fp,"  B1c: pos:%d,length:%d bp, primer(5'-3'):%s\n",p_B1c->pos,p_B1c->len,B1c);
								fprintf(fp,"  B2: pos:%d,length:%d bp, primer(5'-3'):%s\n",p_B2->pos,p_B2->len,B2);
								fprintf(fp,"  B3: pos:%d,length:%d bp, primer(5'-3'):%s\n",p_B3->pos,p_B3->len,B3);
								if(flag[10])
								{
									if(result_Loop[0]==NULL)
										fprintf(fp,"  LF: NULL\n");
									else
									{
										generate_primer(seq,LF,result_Loop[0]->pos,result_Loop[0]->len,1);
										fprintf(fp,"  LF: pos:%d,length:%d bp, primer(5'-3'):%s\n",result_Loop[0]->pos,result_Loop[0]->len,LF);
									}
									if(result_Loop[1]==NULL)
										fprintf(fp,"  LB: NULL\n");
									else
									{
										generate_primer(seq,LB,result_Loop[1]->pos,result_Loop[1]->len,0);
										fprintf(fp,"  LB: pos:%d,length:%d bp, primer(5'-3'):%s\n",result_Loop[1]->pos,result_Loop[1]->len,LB);
									}
								}
                						if(flag[5])
								{
									fprintf(fp,"  This set of LAMP primers could be used in %d genomes, there are: ",j);
									p_list=headList;
									success=0;
									for(i=0;i<common_num[0];i++)
									{
										if(result[i]==0)
											continue;
										while(p_list)
										{
											if(p_list->turn==i)
												break;
											else
												p_list=p_list->next;
										}
										if(success==0)
											fprintf(fp,"%s",p_list->name);
										else
											fprintf(fp,", %s",p_list->name);
										success++;
									}
									fprintf(fp,"\n");
								}
								end=time(NULL);
								printf("It takes %0.0f seconds to design the %d-th LAMP primer set successfully.\n",difftime(end,start),turn);
								start=time(NULL);
								new=1;
								break;
							} //B2
							if(turn>=expect||new)
								break;
                                                }  //B1c
						if(turn>=expect||new)
							break;
                                        } //F1c
					if(turn>=expect||new)
						break;
                                } //F2
				if(turn>=expect||new)
					break;
                        }  //B3
			if(turn>=expect)
				break;
                }  //F3
		if(turn>=expect)
			break;
        }
	fclose(fp);
	free(best_par);
	free(seq);
	free(output);
        free(prefix);
        free(store_path);
        free(inner);
        free(outer);
        free(path_fa);

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
		
		p_F3=headL->next;
		free(headL);
		headL=p_F3;
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

                p_F3=headS->next;
                free(headS);
                headS=p_F3;
        }

	if(flag[5])
	{
		while(headList)
		{
			p_list=headList->next;
			free(headList);
			headList=p_list;
		}
		free(result);
	}

	if(flag[7])
	{
		free(triloopEntropies1);
	        free(triloopEnthalpies1);
	        free(tetraloopEntropies1);
	        free(tetraloopEnthalpies1);
	        free(triloopEntropies2);
	        free(triloopEnthalpies2);
	        free(tetraloopEntropies2);
	        free(tetraloopEnthalpies2);
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

                	p_F3=headLoop->next;
                	free(headLoop);
                	headLoop=p_F3;
        	}
	}
	end=time(NULL);
	printf("It takes %0.0f seconds to free memory.\n",difftime(end,start));
        printf("\nIt takes total %0.0f seconds to finish this design.\n",difftime(end,begin));
}

void merge(int *store,int *temp,int *data,int start,int end,int middle)
{
        int i,j,k;
        i=start;
        j=middle+1;
        k=start;
        while(i<=middle&&j<=end)
        {
                if(data[store[i]*6]<data[store[j]*6])
                {
                        temp[k]=store[i];
                        k++;
                        i++;
                        continue;
                }
                if(data[store[j]*6]<data[store[i]*6])
                {
                        temp[k]=store[j];
                        k++;
                        j++;
                        continue;
                }
        //len
                if(data[store[i]*6+1]<data[store[j]*6+1])
                {
                        temp[k]=store[i];
                        k++;
                        i++;
                        continue;
                }
                if(data[store[j]*6+1]<data[store[i]*6+1])
                {
                        temp[k]=store[j];
                        k++;
                        j++;
                        continue;
                }
        //gi
                if(data[store[i]*6+2]<data[store[j]*6+2])
                {
                        temp[k]=store[i];
                        k++;
                        i++;
                        continue;
                }
                if(data[store[j]*6+2]<data[store[i]*6+2])
                {
                        temp[k]=store[j];
                        k++;
                        j++;
                        continue;
                }
        //position
                if(data[store[i]*6+3]<data[store[j]*6+3])
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
	int pos,len,gi,position,plus,minus,size,i,flag,total,*store,*temp,*data;
	float Tm;
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
		new_primer->total=1;
		new_primer->plus=plus;
		new_primer->minus=minus;
		new_primer->Tm=Tm;
		new_primer->next=NULL;
		new_primer->self=NULL;
		new_primer->loop=NULL;
		new_primer->notloop=NULL;
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
		data=(int *)malloc(6*total*sizeof(int));
		total=0;
		while(fscanf(fp,"%d\t%d\t%d\t%d\t%d\t%d\n",&pos,&len,&gi,&position,&plus,&minus)!=EOF)
		{
			data[6*total]=pos;
			data[6*total+1]=len;
			data[6*total+2]=gi;
			data[6*total+3]=position;
			data[6*total+4]=plus;
			data[6*total+5]=minus;
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
			if(data[store[i]*6]<p_primer->pos)
			{
				i++;
				continue;
			}
			if(data[store[i]*6]>p_primer->pos)
			{
				p_primer=p_primer->next;
				flag=0;
				continue;
			}
		//len
			if(data[store[i]*6+1]<p_primer->len)
                        {
                                i++;
                                continue;
                        }
                        if(data[store[i]*6+1]>p_primer->len)
                        {
                                p_primer=p_primer->next;
				flag=0;
                                continue;
                        }
			new_node=(struct Node *)malloc(size);
			new_node->gi=data[store[i]*6+2];
			new_node->pos=data[store[i]*6+3];
			new_node->plus=data[store[i]*6+4];
			new_node->minus=data[store[i]*6+5];
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
                data=(int *)malloc(6*total*sizeof(int));
                total=0;
                while(fscanf(fp,"%d\t%d\t%d\t%d\t%d\t%d\n",&pos,&len,&gi,&position,&plus,&minus)!=EOF)
                {
                        data[6*total]=pos;
                        data[6*total+1]=len;
                        data[6*total+2]=gi;
                        data[6*total+3]=position;
                        data[6*total+4]=plus;
                        data[6*total+5]=minus;
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
                        if(data[store[i]*6]<p_primer->pos)
                        {
                                i++;
                                continue;
                        }
                        if(data[store[i]*6]>p_primer->pos)
                        {
                                p_primer=p_primer->next;
				flag=0;
                                continue;
                        }
                //len
                        if(data[store[i]*6+1]<p_primer->len)
                        {
                                i++;
                                continue;
                        }
                        if(data[store[i]*6+1]>p_primer->len)
                        {
                                p_primer=p_primer->next;
				flag=0;
                                continue;
                        }
                        new_node=(struct Node *)malloc(size);
                        new_node->gi=data[store[i]*6+2];
                        new_node->pos=data[store[i]*6+3];
                        new_node->plus=data[store[i]*6+4];
                        new_node->minus=data[store[i]*6+5];
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
                        i++;
                }
		free(in);
                free(data);
                free(store);
                free(temp);
	}
	return head;
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

//function: the next one
void next_one(struct Primer *first, struct Primer *second,int flag) //0:self,1:notloop;2:loop
{
	struct Primer *one,*two,*start;
	int pos=-1;

	one=first;
	start=second;
	two=start;

	while(one)
	{
		if(pos!=one->pos)
		{
			while(start)
			{
				if(start->pos+18<one->pos)
					start=start->next;
				else
					break;
			}
			pos=one->pos;
		}
		//move second
		two=start;
		while(two)
		{
			if(two->pos<one->pos+one->len)
				two=two->next;
			else
			{
				if(flag==0)
					one->self=two;
				else if(flag==1)
					one->notloop=two;
				else
					one->loop=two;
				break;
			}
		}
		one=one->next;
	}			
}

//function: check how many GIs this primer can be used for
int check_common(struct Primer *F3,struct Primer *F2,struct Primer *F1c,struct Primer *B1c,struct Primer *B2,struct Primer *B3,int *result,int common)
{
        int dis,num,i;
	struct Node *c_F3,*c_F2,*c_F1c,*c_B1c,*c_B2,*c_B3;
	struct Node *b_F2,*b_F1c,*b_B1c,*b_B2,*b_B3;

	for(i=0;i<common;i++)
		result[i]=0;
//plus
	b_F2=F2->common;
        b_F1c=F1c->common;
        b_B1c=B1c->common;
        b_B2=B2->common;
        b_B3=B3->common;
	for(c_F3=F3->common;c_F3;c_F3=c_F3->next)
	{
		if(c_F3->plus!=1)
			continue;
		if(result[c_F3->gi])
			continue;
		for(c_F2=b_F2;c_F2;c_F2=c_F2->next)
		{
			if(c_F2->gi<c_F3->gi)
			{
				b_F2=c_F2->next;
				continue;
			}
			if(c_F2->gi>c_F3->gi)
				break;
			if(c_F2->plus!=1)
				continue;
			if(c_F2->pos<c_F3->pos)
			{
				b_F2=c_F2->next;
				continue;
			}
			if(c_F2->pos-c_F3->pos>300)
				break;
			for(c_F1c=b_F1c;c_F1c;c_F1c=c_F1c->next)
			{
				if(c_F1c->gi<c_F3->gi)
				{
					b_F1c=c_F1c->next;
					continue;
				}
				if(c_F1c->gi>c_F3->gi)
					break;
				if(c_F1c->minus!=1)
					continue;
				if(c_F1c->pos<c_F3->pos)
				{
					b_F1c=c_F1c->next;
					continue;
				}
				if(c_F1c->pos-c_F3->pos>300)
					break;
				for(c_B1c=b_B1c;c_B1c;c_B1c=c_B1c->next)
				{
					if(c_B1c->gi<c_F3->gi)
					{
						b_B1c=c_B1c->next;
						continue;
					}
					if(c_B1c->gi>c_F3->gi)
						break;
					if(c_B1c->plus!=1)
						continue;
					if(c_B1c->pos<c_F3->pos)
					{
						b_B1c=c_B1c->next;
						continue;
					}
					if(c_B1c->pos-c_F3->pos>300)
						break;
					for(c_B2=b_B2;c_B2;c_B2=c_B2->next)
					{
						if(c_B2->gi<c_F3->gi)
						{
							b_B2=c_B2->next;
							continue;
						}
						if(c_B2->gi>c_F3->gi)
							break;
						if(c_B2->minus!=1)
							continue;
						if(c_B2->pos<c_F3->pos)
						{
							b_B2=c_B2->next;
							continue;
						}
						if(c_B2->pos-c_F3->pos>300)
							break;
						for(c_B3=b_B3;c_B3;c_B3=c_B3->next)
						{
							if(c_B3->gi<c_F3->gi)
							{
								b_B3=c_B3->next;
								continue;
							}
							if(c_B3->gi>c_F3->gi)
								break;
							if(c_B3->minus!=1)
								continue;
							if(c_B3->pos<c_F3->pos)
							{
								b_B3=c_B3->next;
								continue;
							}
							if(c_B3->pos-c_F3->pos>300)
								break;
						//F3-F2 
        						dis=c_F2->pos-(c_F3->pos+F3->len-1)-1;
						        if(dis<0)
                						continue;
        						if(dis>20)
                						continue;
						//F2-F1c
						        dis=c_F1c->pos-c_F2->pos-1;
        						if(dis<40)
                						continue;
        						if(dis>60)
                						continue;
						//F1c-B1c
        						dis=c_B1c->pos-(c_F1c->pos+F1c->len-1)-1;
        						if(dis<0)
                						continue;
						//B1c-B2
        						dis=(c_B2->pos+B2->len-1)-(c_B1c->pos+B1c->len-1)-1;
        						if(dis<40)
								continue;
						        if(dis>60)
                						continue;
						//F2-B2
        						dis=c_B2->pos+B2->len-1-c_F2->pos-1;
        						if(dis<120)
                						continue;
						        if(dis>180)
                						continue;
						//B2-B3
        						dis=c_B3->pos-(c_B2->pos+B2->len-1)-1;
        						if(dis<0)
                						continue;
						        if(dis>20)
                						continue;
							result[c_F3->gi]=1;
						}
					}
				}
			}
		}
	}
//minus
	b_F2=F2->common;
        b_F1c=F1c->common;
        b_B1c=B1c->common;
        b_B2=B2->common;
        b_B3=B3->common;
        for(c_F3=F3->common;c_F3;c_F3=c_F3->next)
        {
                if(c_F3->minus!=1)
                        continue;
                if(result[c_F3->gi])
                        continue;  //this GI can common

                for(c_F2=b_F2;c_F2;c_F2=c_F2->next)
                {
			if(c_F2->gi<c_F3->gi)
                        {
                                b_F2=c_F2->next;
                                continue;
                        }
                        if(c_F2->gi>c_F3->gi)
                                break;
                        if(c_F2->minus!=1)
                                continue;
			if(c_F2->pos>c_F3->pos)
				break;
			if(c_F2->pos-c_F3->pos<-300)
			{
				b_F2=c_F2->next;
				continue;
			}
                        for(c_F1c=b_F1c;c_F1c;c_F1c=c_F1c->next)
                        {
				if(c_F1c->gi<c_F3->gi)
                                {
                                        b_F1c=c_F1c->next;
                                        continue;
                                }
                                if(c_F1c->gi>c_F3->gi)
                                        break;
                                if(c_F1c->plus!=1)
                                        continue;
				if(c_F1c->pos>c_F3->pos)
					break;
				if(c_F1c->pos-c_F3->pos<-300)
				{
					b_F1c=c_F1c->next;
					continue;
				}
                                for(c_B1c=b_B1c;c_B1c;c_B1c=c_B1c->next)
                                {
					if(c_B1c->gi<c_F3->gi)
                                        {
                                                b_B1c=c_B1c->next;
                                                continue;
                                        }
                                        if(c_B1c->gi>c_F3->gi)
                                                break;
                                        if(c_B1c->minus!=1)
                                                continue;
					if(c_B1c->pos>c_F3->pos)
						break;
					if(c_B1c->pos-c_F3->pos<-300)
					{
                                                b_B1c=c_B1c->next;
                                                continue;
                                        }
                                        for(c_B2=b_B2;c_B2;c_B2=c_B2->next)
                                        {
						if(c_B2->gi<c_F3->gi)
                                                {
                                                        b_B2=c_B2->next;
                                                        continue;
                                                }
                                                if(c_B2->gi>c_F3->gi)
                                                        break;
                                                if(c_B2->plus!=1)
                                                        continue;
						if(c_B2->pos>c_F3->pos)
							break;
						if(c_B2->pos-c_F3->pos<-300)
						{
                                                        b_B2=c_B2->next;
                                                        continue;
                                                }
                                                for(c_B3=b_B3;c_B3;c_B3=c_B3->next)
                                                {
							if(c_B3->gi<c_F3->gi)
                                                        {
                                                                b_B3=c_B3->next;
                                                                continue;
                                                        }
                                                        if(c_B3->gi>c_F3->gi)
                                                                break;
                                                        if(c_B3->plus!=1)
                                                                continue;
							if(c_B3->pos>c_F3->pos)
								break;
							if(c_B3->pos-c_F3->pos<-300)
							{
                                                                b_B3=c_B3->next;
                                                                continue;
                                                        }
                                                //F3-F2 
                                                        dis=c_F3->pos-(c_F2->pos+F2->len-1)-1;
                                                        if(dis<0)
                                                                continue;
                                                        if(dis>20)
                                                                continue;
                                                //F2-F1c
                                                        dis=(c_F2->pos+F2->len-1)-(c_F1c->pos+F1c->len-1)-1;
                                                        if(dis<40)
                                                                continue;
                                                        if(dis>60)
                                                                continue;
                                                //F1c-B1c
                                                        dis=c_F1c->pos-(c_B1c->pos+B1c->len-1)-1;
                                                        if(dis<0)
                                                                continue;
                                                //B1c-B2
                                                        dis=c_B1c->pos-c_B2->pos-1;
                                                        if(dis<40)
                                                                continue;
                                                        if(dis>60)
                                                                continue;
                                                //F2-B2
                                                        dis=c_F2->pos+F2->len-1-c_B2->pos-1;
                                                        if(dis<120)
                                                                continue;
                                                        if(dis>180)
                                                                continue;
                                                //B2-B3
                                                        dis=c_B2->pos-(c_B3->pos+B3->len-1)-1;
                                                        if(dis<0)
                                                                continue;
                                                        if(dis>20)
                                                                continue;
                                                        result[c_F3->gi]=1;
                                                }
                                        }
                                }
                        }
                }
        }
	num=0;
	for(i=0;i<common;i++)
	{
		num=num+result[i];
	}
	return num;
}

void reverse(char seq[],char rev[],int length)
{
        int i;
        
        for(i=0;i<length;i++)
        {
                if(seq[length-1-i]=='A')
                {
                        rev[i]='T';
                        continue;
                }
                if(seq[length-1-i]=='T')
                {
                        rev[i]='A';
                        continue;
                }
                if(seq[length-1-i]=='C')
                {
                        rev[i]='G';
                        continue;
                }
                rev[i]='C';
        }
        rev[i]='\0';
}

int check_structure(char F3[],char F2[],char F1c[],char B1c[],char B2[],char B3[],int GC,double stackEntropies[],double stackEnthalpies[],double stackint2Entropies[],double stackint2Enthalpies[],double dangleEntropies3[],double dangleEnthalpies3[],double dangleEntropies5[],double dangleEnthalpies5[],double hairpinLoopEntropies[],double interiorLoopEntropies[],double bulgeLoopEntropies[],double hairpinLoopEnthalpies[],double interiorLoopEnthalpies[],double bulgeLoopEnthalpies[],double tstackEntropies[],double tstackEnthalpies[],double tstack2Entropies[],double tstack2Enthalpies[],char *triloopEntropies1,char *triloopEnthalpies1,char *tetraloopEntropies1,char *tetraloopEnthalpies1,double *triloopEntropies2,double *triloopEnthalpies2,double *tetraloopEntropies2,double *tetraloopEnthalpies2,int numTriloops,int numTetraloops,double atpS[],double atpH[],int error[])
{
	int i,j,threshold;
	double TH;
	char *list[6],rev1[26],rev2[26];

//prepare
	if(GC==1||GC==4)
		threshold=49;
	else
		threshold=45;
	list[0]=F3;
	list[1]=F2;
	list[2]=F1c;
	list[3]=B1c;
	list[4]=B2;
	list[5]=B3;
	for(i=0;i<6;i++)
	{
		for(j=i+1;j<6;j++)
		{
			TH=thal(list[i],list[j],stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,1);
			if(TH>threshold)
			{
				error[0]=i;
				error[1]=j;
				return 0;
			}

			TH=thal(list[i],list[j],stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,2);
			if(TH>threshold)
			{
				error[0]=i;
				error[1]=j;
                                return 0;
			}
			TH=thal(list[i],list[j],stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,3);
			if(TH>threshold)
			{
				error[0]=i;
				error[1]=j;
                                return 0;
			}

			reverse(list[j],rev1,strlen(list[j]));
			reverse(list[i],rev2,strlen(list[i]));
			TH=thal(rev1,rev2,stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,2);
			if(TH>threshold)
			{
				error[0]=i;
				error[1]=j;
                                return 0;
			}
			TH=thal(rev1,rev2,stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,3);
			if(TH>threshold)
			{
				error[0]=i;
				error[1]=j;
                                return 0;
			}
		}
	}
	return 1;
}			

int check_structure_loop(char F3[],char F2[],char F1c[],char B1c[],char B2[],char B3[],char LF[],char LB[],int GC,double stackEntropies[],double stackEnthalpies[],double stackint2Entropies[],double stackint2Enthalpies[],double dangleEntropies3[],double dangleEnthalpies3[],double dangleEntropies5[],double dangleEnthalpies5[],double hairpinLoopEntropies[],double interiorLoopEntropies[],double bulgeLoopEntropies[],double hairpinLoopEnthalpies[],double interiorLoopEnthalpies[],double bulgeLoopEnthalpies[],double tstackEntropies[],double tstackEnthalpies[],double tstack2Entropies[],double tstack2Enthalpies[],char *triloopEntropies1,char *triloopEnthalpies1,char *tetraloopEntropies1,char *tetraloopEnthalpies1,double *triloopEntropies2,double *triloopEnthalpies2,double *tetraloopEntropies2,double *tetraloopEnthalpies2,int numTriloops,int numTetraloops,double atpS[],double atpH[])
{
        int i,threshold;
        double TH;
        char *list[8],rev1[26],rev2[26];

//prepare
	if(GC==1||GC==4)
		threshold=49;
	else
		threshold=45;
        list[0]=F3;
        list[1]=F2;
        list[2]=LF;
        list[3]=F1c;
        list[4]=B1c;
	list[5]=LB;
	list[6]=B2;
	list[7]=B3;

	if(list[2]!=NULL)
	{
		reverse(list[2],rev1,strlen(list[2]));
        	for(i=0;i<=1;i++)
		{
			TH=thal(list[i],list[2],stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,1);
			if(TH>threshold)
				return 0;
			TH=thal(list[i],list[2],stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,2);
			if(TH>threshold)
                                return 0;
                        TH=thal(list[i],list[2],stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,3);
			if(TH>threshold)
                                return 0;

			reverse(list[i],rev2,strlen(list[i]));
			TH=thal(rev1,rev2,stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,2);
			if(TH>threshold)
                                return 0;
			TH=thal(rev1,rev2,stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,3);
			if(TH>44+threshold)
                                return 0;
		}

		for(i=3;i<8;i++)
		{
			if(i==5)
				continue;
			TH=thal(list[2],list[i],stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,1);
			if(TH>threshold)
                                return 0;
			TH=thal(list[2],list[i],stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,2);
			if(TH>threshold)
                                return 0;
                        TH=thal(list[2],list[i],stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,3);
			if(TH>threshold)
                                return 0;

			reverse(list[i],rev2,strlen(list[i]));
			TH=thal(rev2,rev1,stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,2);
			if(TH>threshold) 
                                return 0;
			TH=thal(rev2,rev1,stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,3);
			if(TH>threshold)
                                return 0;
		}
        }
	if(list[5]!=NULL)
	{
		reverse(list[5],rev1,strlen(list[5]));
		for(i=0;i<5;i++)
		{
			if(i==2)
				continue;
			TH=thal(list[i],list[5],stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,1);
			if(TH>threshold)
                                return 0;
			TH=thal(list[i],list[5],stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,2);
			if(TH>threshold)
                                return 0;
                        TH=thal(list[i],list[5],stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,3);
			if(TH>threshold)
                                return 0;

			reverse(list[i],rev2,strlen(list[i]));
			TH=thal(rev1,rev2,stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,2);
			if(TH>threshold)
                                return 0;
			TH=thal(rev1,rev2,stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,3);
			if(TH>threshold)
				return 0;
		}
		for(i=6;i<8;i++)
		{
			TH=thal(list[5],list[i],stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,1);
			if(TH>threshold)
                                return 0;
			TH=thal(list[5],list[i],stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,2);
			if(TH>threshold)
                                return 0;
			TH=thal(list[5],list[i],stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,3);
			if(TH>threshold)
                                return 0;

			reverse(list[i],rev2,strlen(list[i]));
			TH=thal(rev2,rev1,stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,2);
			if(TH>threshold)
                                return 0;
			TH=thal(rev2,rev1,stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,3);
			if(TH>threshold)
                                return 0;
		}
	}
	if(list[2]!=NULL&&list[5]!=NULL)
	{
		TH=thal(list[2],list[5],stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,1);
		if(TH>threshold)
			return 0;
		TH=thal(list[2],list[5],stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,2);
		if(TH>threshold)
                        return 0;
                TH=thal(list[2],list[5],stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,3);
		if(TH>threshold)
                        return 0;

		reverse(list[2],rev1,strlen(list[2]));
		reverse(list[5],rev2,strlen(list[5]));
		TH=thal(rev2,rev1,stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,2);
		if(TH>threshold)
                        return 0;
		TH=thal(rev2,rev1,stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH,3);
		if(TH>threshold)
                        return 0;
	}
	return 1;
}

int check_common_loop(struct Primer *F3,struct Primer *F2,struct Primer *LF,struct Primer *F1c,struct Primer *B1c,struct Primer *LB,struct Primer *B2,struct Primer *B3,int *result,int common_num)
{
        int dis,i,*temp,success;
        struct Node *c_F3,*c_F2,*c_LF,*c_F1c,*c_B1c,*c_LB,*c_B2,*c_B3;

	temp=(int *)malloc(common_num*sizeof(int));
	for(i=0;i<common_num;i++)
		temp[i]=0;
//plus
        for(c_F3=F3->common;c_F3;c_F3=c_F3->next)
        {
                if(c_F3->plus!=1)
                        continue;
                if(result[c_F3->gi]==0)
                        continue;
		if(temp[c_F3->gi])
			continue;
                for(c_F2=F2->common;c_F2;c_F2=c_F2->next)
                {
                        if(c_F2->gi!=c_F3->gi)
                                continue;
                        if(c_F2->plus!=1)
                                continue;
                        for(c_F1c=F1c->common;c_F1c;c_F1c=c_F1c->next)
                        {
                                if(c_F1c->gi!=c_F3->gi)
                                        continue;
                                if(c_F1c->minus!=1)
                                        continue;
                                for(c_B1c=B1c->common;c_B1c;c_B1c=c_B1c->next)
                                {
                                        if(c_B1c->gi!=c_F3->gi)
                                                continue;
                                        if(c_B1c->plus!=1)
                                                continue;
                                        for(c_B2=B2->common;c_B2;c_B2=c_B2->next)
                                        {
                                                if(c_B2->gi!=c_F3->gi)
                                                        continue;
                                                if(c_B2->minus!=1)
                                                        continue;
                                                for(c_B3=B3->common;c_B3;c_B3=c_B3->next)
                                                {
                                                        if(c_B3->gi!=c_F3->gi)
                                                                continue;
                                                        if(c_B3->minus!=1)
                                                                continue;
                                                //F3-F2 
                                                        dis=c_F2->pos-(c_F3->pos+F3->len-1)-1;
                                                        if(dis<0)
                                                                continue;
                                                        if(dis>20)
                                                                continue;
                                                //F2-F1c
                                                        dis=c_F1c->pos-c_F2->pos-1;
                                                        if(dis<40)
                                                                continue;
                                                        if(dis>60)
                                                                continue;
                                                //F1c-B1c
                                                        dis=c_B1c->pos-(c_F1c->pos+F1c->len-1)-1;
                                                        if(dis<0)
                                                                continue;
                                                //B1c-B2
                                                        dis=(c_B2->pos+B2->len-1)-(c_B1c->pos+B1c->len-1)-1;
                                                        if(dis<40)
                                                                continue;
                                                        if(dis>60)
                                                                continue;
                                                //F2-B2
                                                        dis=c_B2->pos+B2->len-1-c_F2->pos-1;
                                                        if(dis<120)
                                                                continue;
                                                        if(dis>180)
                                                                continue;
                                                //B2-B3
                                                        dis=c_B3->pos-(c_B2->pos+B2->len-1)-1;
                                                        if(dis<0)
                                                                continue;
                                                        if(dis>20)
                                                                continue;
						//LF
							if(LF)
							{
								success=0;
								for(c_LF=LF->common;c_LF;c_LF=c_LF->next)
								{
									if(c_LF->gi!=c_F3->gi)
										continue;
									if(c_LF->minus!=1)
										continue;
									if(c_F2->pos+F2->len>c_LF->pos)
										continue;
									if(c_LF->pos+LF->len>c_F1c->pos)
										continue;
									success=1;
									break;
								}
								if(success==0)
									continue;
							}
						//LB
							if(LB)
                                                        {
                                                                success=0;
                                                                for(c_LB=LB->common;c_LB;c_LB=c_LB->next)
                                                                {
                                                                        if(c_LB->gi!=c_F3->gi)
                                                                                continue;
                                                                        if(c_LB->plus!=1)
                                                                                continue;
                                                                        if(c_B1c->pos+B1c->len>c_LB->pos)
                                                                                continue;
                                                                        if(c_LB->pos+LB->len>c_B2->pos)
                                                                                continue;
                                                                        success=1;
                                                                        break;
                                                                }
                                                                if(success==0)
                                                                        continue;
                                                        }
							temp[c_F3->gi]=1;
                                                }
                                        }
                                }
                        }
                }
        }
//minus
        for(c_F3=F3->common;c_F3;c_F3=c_F3->next)
        {
                if(c_F3->minus!=1)
                        continue;
                if(result[c_F3->gi]==0)
                        continue;  
		if(temp[c_F3->gi])
			continue;
                for(c_F2=F2->common;c_F2;c_F2=c_F2->next)
                {
                        if(c_F2->gi!=c_F3->gi)
                                continue;
                        if(c_F2->minus!=1)
                                continue;
                        for(c_F1c=F1c->common;c_F1c;c_F1c=c_F1c->next)
                        {
                                if(c_F1c->gi!=c_F3->gi)
                                        continue;
                                if(c_F1c->plus!=1)
                                        continue;
                                for(c_B1c=B1c->common;c_B1c;c_B1c=c_B1c->next)
                                {
                                        if(c_B1c->gi!=c_F3->gi)
                                                continue;
                                        if(c_B1c->minus!=1)
                                                continue;
                                        for(c_B2=B2->common;c_B2;c_B2=c_B2->next)
                                        {
                                                if(c_B2->gi!=c_F3->gi)
                                                        continue;
                                                if(c_B2->plus!=1)
                                                        continue;
                                                for(c_B3=B3->common;c_B3;c_B3=c_B3->next)
                                                {
                                                        if(c_B3->gi!=c_F3->gi)
                                                                continue;
                                                        if(c_B3->plus!=1)
                                                                continue;
                                                //F3-F2 
                                                        dis=c_F3->pos-(c_F2->pos+F2->len-1)-1;
                                                        if(dis<0)
                                                                continue;
                                                        if(dis>20)
                                                                continue;
                                                //F2-F1c
                                                        dis=(c_F2->pos+F2->len-1)-(c_F1c->pos+F1c->len-1)-1;
                                                        if(dis<40)
                                                                continue;
                                                        if(dis>60)
                                                                continue;
                                                //F1c-B1c
                                                        dis=c_F1c->pos-(c_B1c->pos+B1c->len-1)-1;
                                                        if(dis<0)
                                                                continue;
                                                //B1c-B2
                                                        dis=c_B1c->pos-c_B2->pos-1;
                                                        if(dis<40)
                                                                continue;
                                                        if(dis>60)
                                                                continue;
                                                //F2-B2
                                                        dis=c_F2->pos+F2->len-1-c_B2->pos-1;
                                                        if(dis<120)
                                                                continue;
                                                        if(dis>180)
                                                                continue;
                                                //B2-B3
                                                        dis=c_B2->pos-(c_B3->pos+B3->len-1)-1;
                                                        if(dis<0)
                                                                continue;
                                                        if(dis>20)
                                                                continue;
						//LF
                                                        if(LF)
                                                        {
                                                                success=0;
                                                                for(c_LF=LF->common;c_LF;c_LF=c_LF->next)
                                                                {
                                                                        if(c_LF->gi!=c_F3->gi)
                                                                                continue;
                                                                        if(c_LF->plus!=1)
                                                                                continue;
                                                                        if(c_F1c->pos+F1c->len>c_LF->pos)
                                                                                continue;
                                                                        if(c_LF->pos+LF->len>c_F2->pos)
                                                                                continue;
                                                                        success=1;
                                                                        break;
                                                                }
                                                                if(success==0)
                                                                        continue;
                                                        }
                                                //LB
                                                        if(LB)
                                                        {
                                                                success=0;
                                                                for(c_LB=LB->common;c_LB;c_LB=c_LB->next)
                                                                {
                                                                        if(c_LB->gi!=c_F3->gi)
                                                                                continue;
                                                                        if(c_LB->minus!=1)
                                                                                continue;
                                                                        if(c_B2->pos+B2->len>c_LB->pos)
                                                                                continue;
                                                                        if(c_LB->pos+LB->len>c_B1c->pos)
                                                                                continue;
                                                                        success=1;
                                                                        break;
                                                                }
                                                                if(success==0)
                                                                        continue;
                                                        }
                                                        temp[c_F3->gi]=1;
                                                }
                                        }
                                }
                        }
                }
        }
        for(i=0;i<common_num;i++)
        {
                if(result[i]&&temp[i]==0)
			return 0;
        }
        return 1;
}

int design_loop(struct Primer *p_F3,struct Primer *p_F2,struct Primer *p_LF,struct Primer *p_F1c,struct Primer *p_B1c,struct Primer *p_LB,struct Primer *p_B2,struct Primer *p_B3,struct Primer *result_Loop[], int *result,int flag[],int common_num,char *seq,double stackEntropies[],double stackEnthalpies[],double stackint2Entropies[],double stackint2Enthalpies[],double dangleEntropies3[],double dangleEnthalpies3[],double dangleEntropies5[],double dangleEnthalpies5[],double hairpinLoopEntropies[],double interiorLoopEntropies[],double bulgeLoopEntropies[],double hairpinLoopEnthalpies[],double interiorLoopEnthalpies[],double bulgeLoopEnthalpies[],double tstackEntropies[],double tstackEnthalpies[],double tstack2Entropies[],double tstack2Enthalpies[],char *triloopEntropies1,char *triloopEnthalpies1,char *tetraloopEntropies1,char *tetraloopEnthalpies1,double *triloopEntropies2,double *triloopEnthalpies2,double *tetraloopEntropies2,double *tetraloopEnthalpies2,int numTriloops,int numTetraloops,double atpS[],double atpH[],int GC)
{
	int success;
	struct Primer *LF,*LB;
	struct Node *c_LF,*c_LB,*s_LF,*s_LB;
	char primer_F3[26],primer_F2[26],primer_F1c[26],primer_B1c[26],primer_B2[26],primer_B3[26],primer_LF[26],primer_LB[26];

	if(flag[7])
	{
		generate_primer(seq,primer_F3,p_F3->pos,p_F3->len,0);
		generate_primer(seq,primer_F2,p_F2->pos,p_F2->len,0);
		generate_primer(seq,primer_F1c,p_F1c->pos,p_F1c->len,1);
		generate_primer(seq,primer_B1c,p_B1c->pos,p_B1c->len,0);
		generate_primer(seq,primer_B2,p_B2->pos,p_B2->len,1);
		generate_primer(seq,primer_B3,p_B3->pos,p_B3->len,1);
	}
//LF and LB 
	success=0;
	LF=p_LF;
	while(LF)
	{
		if(LF->pos+LF->len>p_F1c->pos)
			break;
		if(LF->minus==0)
		{
			LF=LF->next;
			continue;
		}
		if((LF->minus&GC)!=GC)
		{
			LF=LF->next;
			continue;
		}

		LB=p_LB;
		if(flag[7])
			generate_primer(seq,primer_LF,LF->pos,LF->len,1);
		while(LB)
		{
			if(LB->pos+LB->len>p_B2->pos)
				break;
			if(LB->plus==0)
			{
				LB=LB->next;
				continue;
			}
			if((LB->plus&GC)!=GC)
			{
				LB=LB->next;
				continue;
			}
		//check_common
			if(flag[5])
			{
				success=check_common_loop(p_F3,p_F2,LF,p_F1c,p_B1c,LB,p_B2,p_B3,result,common_num);
				if(success==0)
				{
					LB=LB->next;
					continue;
				}
			}
		//check_structure
			if(flag[7])
			{
				generate_primer(seq,primer_LB,LB->pos,LB->len,0);
				success=check_structure_loop(primer_F3,primer_F2,primer_F1c,primer_B1c,primer_B2,primer_B3,primer_LF,primer_LB,GC,stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH);
				if(success==0)
                        	{
                        	        LB=LB->next;
                        	        continue;
                        	}
			}
			result_Loop[0]=LF;
			result_Loop[1]=LB;
			success=1;
			break;
		}
		if(success==1)
			break;
		else
			LF=LF->next;
	}
	if(success==1)
		return success;
//only LF
	LF=p_LF;
	result_Loop[1]=NULL;
        while(LF)
        {
                if(LF->pos+LF->len>p_F1c->pos)
                        break;
		if(LF->minus==0)
		{
			LF=LF->next;
			continue;
		}
		if((LF->minus&GC)!=GC)
		{
			LF=LF->next;
			continue;
		}
	//check_common
		if(flag[5])
		{
			success=check_common_loop(p_F3,p_F2,LF,p_F1c,p_B1c,NULL,p_B2,p_B3,result,common_num);
			if(success==0)
			{
				LF=LF->next;
				continue;
			}
		}
	//check_structure
		if(flag[7])
		{
			generate_primer(seq,primer_LF,LF->pos,LF->len,1);
			success=check_structure_loop(primer_F3,primer_F2,primer_F1c,primer_B1c,primer_B2,primer_B3,primer_LF,NULL,GC,stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH);
			if(success==0)
			{
				LF=LF->next;
				continue;
			}
		}
		result_Loop[0]=LF;
		success=1;
		break;
        }
        if(success==1)
                return success;
//only LB
	LB=p_LB;
	result_Loop[0]=NULL;
        while(LB)
        {
                if(LB->pos+LB->len>p_B2->pos)
                        break;
		if(LB->plus==0)
		{
			LB=LB->next;
			continue;
		}
		if((LB->plus&GC)!=GC)
		{
			LB=LB->next;
			continue;
		}
	//check_common
		if(flag[5])
		{
			success=check_common_loop(p_F3,p_F2,NULL,p_F1c,p_B1c,LB,p_B2,p_B3,result,common_num);
			if(success==0)
			{
				LB=LB->next;
				continue;
			}
		}
	//check_structure
		if(flag[7])
		{
			generate_primer(seq,primer_LB,LB->pos,LB->len,0);
			success=check_structure_loop(primer_F3,primer_F2,primer_F1c,primer_B1c,primer_B2,primer_B3,NULL,primer_LB,GC,stackEntropies,stackEnthalpies,stackint2Entropies,stackint2Enthalpies,dangleEntropies3,dangleEnthalpies3,dangleEntropies5,dangleEnthalpies5,hairpinLoopEntropies,interiorLoopEntropies,bulgeLoopEntropies,hairpinLoopEnthalpies,interiorLoopEnthalpies,bulgeLoopEnthalpies,tstackEntropies,tstackEnthalpies,tstack2Entropies,tstack2Enthalpies,triloopEntropies1,triloopEnthalpies1,tetraloopEntropies1,tetraloopEnthalpies1,triloopEntropies2,triloopEnthalpies2,tetraloopEntropies2,tetraloopEnthalpies2,numTriloops,numTetraloops,atpS,atpH);
			if(success==0)
			{
				LB=LB->next;
				continue;
			}
		}
		result_Loop[1]=LB;
		success=1;
		break;
        }
	return success;
}			
		
int check_add(int F3_pos,int *best_par,int expect)
{
        int i,dis;

	for(i=0;i<expect;i++)
	{
		if(best_par[i]==-1) //the empty record
			return 1;
		dis=best_par[i]-F3_pos;
		if(abs(dis)<300)
			return 0;
	}
        return 1;
}

int check_gc(char *seq,int start,int end)//p_F3->pos,(p_B3->pos+p_B3->len))
{
	int i,total=0;
	float gc;

	for(i=start;i<end;i++)
	{
		if(seq[i]=='C'||seq[i]=='G'||seq[i]=='c'||seq[i]=='g')
			total++;
	}
	gc=total*100.0/(end-start);
	if(gc>=60)
		return 1;
	if(gc<=45)
		return 2;
	return 4;
}
	
///how many GIs this primer can be used in, return the biggest common number
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
			list[i]=0;
		while(p_node)
		{
			list[p_node->gi]=1;
			p_node=p_node->next;
		}
		num=0;
                for(i=0;i<common;i++)
                        num=num+list[i];
		p_primer->total=num;
		p_primer=p_primer->next;
	}
	free(list);
}

//check this LAMP primers are uniq or not
//return=0: stop and return=1: go on
int check_uniq(struct Primer *F3,struct Primer *F2,struct Primer *F1c,struct Primer *B1c,struct Primer *B2,struct Primer *B3)
{
        struct Node *s_F3,*s_F2,*s_F1c,*s_B1c,*s_B2,*s_B3;
	struct Node *b_F2,*b_F1c,*b_B1c,*b_B2,*b_B3;

//plus
	b_F2=F2->special;
	b_F1c=F1c->special;
	b_B1c=B1c->special;
	b_B2=B2->special;
	b_B3=B3->special;
        for(s_F3=F3->special;s_F3;s_F3=s_F3->next)
        {
                if(s_F3->plus!=1)
                        continue;
                for(s_F2=b_F2;s_F2;s_F2=s_F2->next)
                {
			if(s_F2->gi<s_F3->gi)
			{
				b_F2=b_F2->next;
				continue;
			}
                        if(s_F2->gi>s_F3->gi)
                                break;
			if(s_F2->pos<s_F3->pos)
			{
				b_F2=b_F2->next;
                                continue;
                        }
			if(s_F2->pos-s_F3->pos>1000)
				break;
                        if(s_F2->plus!=1)
                                continue;
                        for(s_F1c=b_F1c;s_F1c;s_F1c=s_F1c->next)
                        {
                                if(s_F1c->gi<s_F3->gi)
				{
					b_F1c=b_F1c->next;
                                        continue;
				}
				if(s_F1c->gi>s_F3->gi)
					break;
				if(s_F1c->pos<s_F3->pos)
				{
                                        b_F1c=b_F1c->next;
                                        continue;
                                }
				if(s_F1c->pos-s_F3->pos>1000)
					break;
                                if(s_F1c->minus!=1)
                                        continue;
                                for(s_B1c=b_B1c;s_B1c;s_B1c=s_B1c->next)
                                {
                                        if(s_B1c->gi<s_F3->gi)
					{
						b_B1c=b_B1c->next;
                                                continue;
					}
					if(s_B1c->gi>s_F3->gi)
						break;
					if(s_B1c->pos<s_F3->pos)
					{
						b_B1c=b_B1c->next;
                                                continue;
                                        }
					if(s_B1c->pos-s_F3->pos>1000)
						break;
                                        if(s_B1c->plus!=1)
                                                continue;
                                        for(s_B2=b_B2;s_B2;s_B2=s_B2->next)
                                        {
                                                if(s_B2->gi<s_F3->gi)
						{
							b_B2=b_B2->next;
                                                        continue;
						}
						if(s_B2->gi>s_F3->gi)
							break;
						if(s_B2->pos<s_F3->pos)
						{
							b_B2=b_B2->next;
                                                        continue;
                                                }
						if(s_B2->pos-s_F3->pos>1000)
							break;
                                                if(s_B2->minus!=1)
                                                        continue;
                                                for(s_B3=b_B3;s_B3;s_B3=s_B3->next)
                                                {
                                                        if(s_B3->gi<s_F3->gi)
							{
								b_B3=b_B3->next;
                                                                continue;
							}
							if(s_B3->gi>s_F3->gi)
								break;
							if(s_B3->pos<s_F3->pos)
							{
                                                                b_B3=b_B3->next;
                                                                continue;
                                                        }
							if(s_B3->pos-s_F3->pos>1000)
								break;
                                                        if(s_B3->minus!=1)
                                                                continue;
                                                //F3-F2 
                                                        if(s_F2->pos<s_F3->pos)
                                                                continue;
                                                //F2-F1c
                                                        if(s_F1c->pos<s_F2->pos+F2->len)
                                                                continue;
                                                //F1c-B1c
                                                        if(s_B1c->pos<s_F1c->pos+F1c->len)
                                                                continue;
                                                //B1c-B2
                                                        if(s_B2->pos<s_B1c->pos+B1c->len)
                                                                continue;
                                                //B2-B3
                                                        if(s_B3->pos<s_B2->pos)
                                                                continue;
						//whole
							if(s_B3->pos-s_F3->pos>1000)
								continue;
							return 0;
                                                }
                                        }
                                }
                        }
                }
        }
//minus
	b_F2=F2->special;
	b_F1c=F1c->special;
	b_B1c=B1c->special;
	b_B2=B2->special;
	b_B3=B3->special;
        for(s_F3=F3->special;s_F3;s_F3=s_F3->next)
        {
                if(s_F3->minus!=1)
                        continue;
                for(s_F2=b_F2;s_F2;s_F2=s_F2->next)
                {
                        if(s_F2->gi<s_F3->gi)
			{
				b_F2=b_F2->next;
                                continue;
			}
			if(s_F2->gi>s_F3->gi)
				break;
			if(s_F2->pos>s_F3->pos)
				break;
			if(s_F2->pos-s_F3->pos<-1000)
			{
                                b_F2=b_F2->next;
                                continue;
                        }
                        if(s_F2->minus!=1)
                                continue;
                        for(s_F1c=b_F1c;s_F1c;s_F1c=s_F1c->next)
                        {
                                if(s_F1c->gi<s_F3->gi)
				{
					b_F1c=b_F1c->next;
                                        continue;
				}
				if(s_F1c->gi>s_F3->gi)
					break;
				if(s_F1c->pos>s_F3->pos)
					break;
				if(s_F1c->pos-s_F3->pos<-1000)
				{
                                        b_F1c=b_F1c->next;
                                        continue;
                                }
                                if(s_F1c->plus!=1)
                                        continue;
                                for(s_B1c=b_B1c;s_B1c;s_B1c=s_B1c->next)
                                {
                                        if(s_B1c->gi<s_F3->gi)
					{
						b_B1c=b_B1c->next;
                                                continue;
					}
					if(s_B1c->gi>s_F3->gi)
						break;
					if(s_B1c->pos>s_F3->pos)
						break;
					if(s_B1c->pos-s_F3->pos<-1000)
					{
                                                b_B1c=b_B1c->next;
                                                continue;
                                        }
                                        if(s_B1c->minus!=1)
                                                continue;
                                        for(s_B2=b_B2;s_B2;s_B2=s_B2->next)
                                        {
                                                if(s_B2->gi<s_F3->gi)
						{
							b_B2=b_B2->next;
                                                        continue;
						}
						if(s_B2->gi>s_F3->gi)
							break;
						if(s_B2->pos>s_F3->pos)
							break;
						if(s_B2->pos-s_F3->pos<-1000)
						{
                                                        b_B2=b_B2->next;
                                                        continue;
                                                }
                                                if(s_B2->plus!=1)
                                                        continue;
                                                for(s_B3=b_B3;s_B3;s_B3=s_B3->next)
                                                {
                                                        if(s_B3->gi<s_F3->gi)
							{
								b_B3=b_B3->next;
                                                                continue;
							}
							if(s_B3->gi>s_F3->gi)
								break;
							if(s_B3->pos>s_F3->pos)
								break;
							if(s_B3->pos-s_F3->pos<-1000)
							{
                                                                b_B3=b_B3->next;
                                                                continue;
                                                        }
                                                        if(s_B3->plus!=1)
                                                                continue;
                                                //F3-F2 
                                                        if(s_F3->pos<s_F2->pos)
                                                                continue;
                                                //F2-F1c
                                                        if(s_F2->pos<s_F1c->pos+F1c->len)
                                                                continue;
                                                //F1c-B1c
                                                        if(s_F1c->pos<s_B1c->pos+B1c->len)
                                                                continue;
                                                //B1c-B2
                                                        if(s_B1c->pos<s_B2->pos+B2->len)
                                                                continue;
                                                //B2-B3
                                                        if(s_B2->pos<s_B3->pos)
                                                                continue;
						//whole
							if(s_F3->pos-s_B3->pos>1000)
								continue;
							return 0;
                                                }
                                        }
                                }
                        }
                }
        }
	return 1;
}
