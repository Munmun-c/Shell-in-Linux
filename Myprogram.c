#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

void external();
void cwd();
void cd();
void echo_env();
void echo();
void export_env();
void history();
void fg();
void history_cmd();
void grep();

char *input;
char **cmdarg;
char **pipearg;
char *pwd;
char **hicm;

int count;
int cmdcount;
FILE *fp;
int hist_index=0;

int job=1;
int pid_array[6]={0};
char arr[1000][1000];
void exclamination(){ 
             //printf("%s",input);
	
	strcpy(input,"uniq history1 | tail -n 2 | head -1");
           //printf("%s",input);

external();
}
char *get_string(char *string){
 char *pname;
 int i,j,len;
     	len=strlen(string);  
               if(*(string+len-1)=='"')
        {
	pname=(char *)malloc(sizeof(char)*len);
                           j=0;
			for(i=1;i<len;i++,j++){
				*(pname+j)=*(string+i);
			}
			*(pname+j)='\0';
		len=strlen(pname);
		*(pname+len-1)='\0';
		return pname;
              
           }
             else 
      return string;
 }

void sigintHandler(int sig_num)
{

	signal(SIGINT, sigintHandler);
        if(pwd!=NULL)
        {
         printf("\n");
         
         printf("MUNMUN: %s $:",pwd);
         }
	fflush(stdout);
}

int main(){
	signal(SIGINT, sigintHandler);
	raise(SIGINT);
	
	input=malloc(sizeof(char)*1024);
	if(input==NULL){
		printf("Memory cannot be allocated");
		return 0;
	}

	hicm=malloc(sizeof(char*)*10);
	if(hicm==NULL){
		printf("Memory cannot be allocated");
		return 0;
	}
	for(hist_index=0;hist_index<10;hist_index++){
		hicm[hist_index]=malloc(sizeof(char)*50);

	}
	hist_index=0;

	pwd=malloc(sizeof(char)*50);
	if(pwd==NULL){
		printf("Memory cannot be allocated");
		exit(0);
	}

	cwd();
	printf("\n");

	fp=fopen("history1","a+");
	if(fp==NULL){
		printf("File cannot be opened\n");
		exit(0);
	}


	printf("MUNMUN: %s $:",pwd);
	scanf(" %[^\n]s",input);

	strcpy(hicm[hist_index],input);


	fputs(input,fp);
	fputs("\n",fp);
	fclose(fp);
           
           if ((strcmp(input,"exit"))==0)
		{
                 printf("Bye... ");
                 printf("\n");
                  return 0;
                   }

	while((strcmp(input,"exit"))!=0){
              
		if(strstr(input,"fg")!=NULL)
			fg();

		else if(strstr(input,"!")!=NULL)
			history_cmd();

		else
		external();
		cwd();
		printf("\n");
		printf("MUNMUN: %s $:",pwd);
		input=malloc(sizeof(char)*100);
		if(input==NULL){
			printf("memory cannot be allocated");
			exit(0);
		}

		scanf(" %[^\n]s",input);

		fp=fopen("history1","a+");
			if(fp==NULL){
				printf("File cannot be opened\n");
				exit(0);
			}

		fputs(input,fp);
		fputs("\n",fp);
		fclose(fp);

		hist_index++;
		if(hist_index==10)
		hist_index=0;
		strcpy(hicm[hist_index],input);
                 if ((strcmp(input,"exit"))==0)
		{
                 printf("Bye... ");
                 printf("\n");
                  return 0;
                   }



	}
	free(input); 
	free(pwd);
	return 0;
}

void history_cmd(){
	int i;
	char *temp;
	int flag=0;
	temp=malloc(sizeof(char)*50);
	if(temp==NULL){
		printf("memory cannot be allocated");
		exit(0);
	}
	
	strcpy(temp,input);
	temp++;
          
	for(i=hist_index-1; i>=0 ;i--){
              
		if((strstr(hicm[i],temp))!=NULL){
			flag=1;
			break;
		}
	}
	if(flag==0){
		printf("No previous records of the command are found\n");
		return;
	}
	else{
		
		strcpy(input,hicm[i]);
		
		external();

	}
exclamination();



}



void cwd(){
	pwd=getcwd(pwd,50);
	if(pwd==NULL){
	
	printf("PWD OUTPUT CANNOT BE DEFINED\n");
}

}


void cd(){
	int res;
	res=chdir(cmdarg[1]);
	if(res<0){
		printf("cd : No such File or Directory\n");

	}
}


void echo(){
	int i, k=2;
	char* strstrt;
	char* prtstr;
	char* str;
	str=malloc(sizeof(char)*strlen(cmdarg[1]));
	strcpy(str,cmdarg[1]);
	
	while(1){
		if(cmdarg[k]==NULL)
			break;
		str = (char *) realloc(str,strlen(str)+strlen(cmdarg[k]));
		if(str==NULL){
			printf("memory cannot be allocated\n");
			exit(0);
		}
		strcat(strcat(str," "),cmdarg[k]);
		k++;
	}

	prtstr=malloc(sizeof(char)*strlen(str));
	if(prtstr==NULL){
		printf("memory cannot be allocated\n");
		exit(0);
	}
	strstrt = prtstr;
	
	for(i = 0; i<strlen(str); i++){
		if(str[i] == '"' && str[i-1] != '\\'){
	    	continue;
	    }
	    *prtstr = str[i];
	    prtstr++;	    
	}
	printf("%s\n",strstrt);
	free(strstrt);
}

void echo_env(){
	char *result;
	char *value;

	result=malloc(sizeof(char)*strlen(cmdarg[1]));
	if(result==NULL){
		printf("memory cannot be allocated\n");
		exit(0);
	}
	value=malloc(sizeof(char)*100);
	if(value==NULL){
		printf("memory cannot be allocated\n");
		exit(0);
	}

	strcpy(result,cmdarg[1]);
	result++;
       if((*result=='P')&&(*(result+1)=='W')&&(*(result+2)=='D'))
         {
         printf("%s",pwd);
         }
         else
        {
	value=getenv(result);
	if(value==NULL){
		printf("No such environment variable\n");
	}
	else
	printf("%s\n",value);
          }
         
        
	result--;
	free(result);
         
}


void export_env(){
	char *name;
	char *value;
	char *temp;
	int ret;
	temp=malloc(sizeof(char)*100);
	if(temp==NULL){
		exit(0);
	}
	name=malloc(sizeof(char)*100);
	if(name==NULL){
		exit(0);
	}
	value=malloc(sizeof(char)*100);
	if(value==NULL){
		exit(0);
	}
	temp=strstr(cmdarg[1],"=");
	temp++;
	strcpy(value,temp);
	temp--;
	*temp='\0';
	strcpy(name,cmdarg[1]);
	
	ret=setenv(name,value,1);
	if(ret<0){
		printf("Environment variable not set\n");
	}
	free(name);
	free(value);
}

void history(){
	char line[1];
	if(cmdarg[1]){
		strcpy(line,cmdarg[1]);
		strcpy(input,"uniq history1 | cat -n  | tail -n ");
		strcat(input,line);
		external();
	}
	else{
		strcpy(input,"uniq history1 | cat -n");
		external();
	}	
	
}

void fg(){
	char *jobno;
	int i;
	int status;
	jobno=malloc(sizeof(char)*10);
	if(jobno==NULL){
		printf("Memory cannot be allocated\n");
		return;
	}
	jobno=strstr(input," ");
	jobno++;
	i=atoi(jobno);
	
	if(i<1 || i>5){
		printf("No such job\n");
		return;
	}
	if(pid_array[i]!=0)
	waitpid(pid_array[i],&status,0);
	pid_array[i]=0;
	printf("Background process has been executed\n");
}



void grep(){
	int fd,r,j=0; 
    char temp,line[100];
    

    if((fd=open(cmdarg[2],O_RDONLY)) != -1) 
	    { 
	        while((r=read(fd,&temp,sizeof(char)))!= 0) 
	        { 
	            if(temp!='\n') 
	            { 
	                line[j]=temp; 
	                j++; 
	            } 
	            else 
	            { 
	                if(strstr(line,cmdarg[1])!=NULL) 
	                    printf("%s\n",line); 
	                memset(line,0,sizeof(line)); 
	                j=0; 
	            } 

	        } 
	    } 
	else
		printf("No such file exist");  		   
}

void external(){

	int *pipes;
	int count=0,i,j,k,status;
	int ret;

	int inputflag=0;
	FILE *inputfile=NULL;
	int fd;

	int outputflag=0;
	FILE *outputfile=NULL;

	int outputflag_a=0;
	FILE *appendfile=NULL;

	int inputflag_d=0;
	FILE *inputfile_d=NULL;

	int backgroundflag=0;
	int pid;

	for(i=0;i<strlen(input);i++){
		if(input[i]=='|')
		count++;
	}
	pipearg=malloc(sizeof(char *)*(count+2));
	if(pipearg==NULL){
		printf("memory cannot be allocated\n");
		exit(0);
	}
	for(i=0;i<(count + 2);i++)
	pipearg[i]=malloc(sizeof(char)*10);

	i=0;
	pipearg[i]=strtok(input,"|");
	i++;
	while(1){
		pipearg[i]=strtok(NULL,"|");
		if(pipearg[i]==NULL )
			break;
		i++;
	}

	if(count>0){
		pipes=malloc(sizeof(int)*2*count);
		if(pipes==NULL){
			printf("Memory cannot be allocated\n");
			exit(0);
		}

		for(i=0;i<2*count;i=i+2){
			ret=pipe(pipes+i);
			if(ret<0)
			printf("pipe not created");
		}
	}

	for(i=0;i<=(count) && pipearg[i]!=NULL ;i++)
	{

		inputflag=0;
		inputfile=NULL;

		outputflag=0;
		outputfile=NULL;

		outputflag_a=0;
		appendfile=NULL;

		inputflag_d=0;
		inputfile_d=NULL;

		backgroundflag=0;

		cmdcount=0;
		for(j=0;j<strlen(pipearg[i]);j++){
			if(pipearg[i][j]==' ')
			cmdcount++;
		}
		cmdarg=malloc(sizeof(char *)*(cmdcount+2));
		for(k=0;k<(cmdcount+2);k++)
			cmdarg[k]=malloc(sizeof(char)*10);
		k=0;
		cmdarg[k]=strtok(pipearg[i]," ");
		k++;
		while(1){
			cmdarg[k]=strtok(NULL," ");
			if(cmdarg[k]==NULL)
				break;

			if(strcmp(cmdarg[k],"<")==0)
				inputflag=k;

			if(strcmp(cmdarg[k],">")==0)
				outputflag=k;

			if(strcmp(cmdarg[k],">>")==0)
				outputflag_a=k;

			if(strcmp(cmdarg[k],"<<")==0)
				inputflag_d=k;

                         if (cmdarg[k]!=NULL)
                         {
			
                         char *pname=get_string(cmdarg[k]);
                          cmdarg[k]=pname;
               
                           }
			k++;
			}
		
		if((cmdcount>1) && (strcmp(cmdarg[k-1],"&")==0)){
			backgroundflag=1;
			cmdarg[cmdcount]=NULL;
			

		}


		printf("\n");
		pid=fork();
		if(pid==0){

				if(i==0 && count > 0)
				{
					
					ret=dup2(pipes[1],1);
					if(ret>0)
					{
						//perror("pipe1c");
					}
					for(j=0;j<2*count;j++)
						close(pipes[j]);
				}
				else if(i>0 && i<count && count>0)
				{
					ret=0;
					ret=dup2(pipes[(i-1)*2],0);
					if(ret>0)
					{
						//perror("pipe2c");

					}
					ret=0;
					ret=dup2(pipes[(i*2)+1],1);
					if(ret>0)
					{
						//perror("pipe3c");

					}

					for(j=0;j<2*count;j++)
					{
						close(pipes[j]);
					}

				}
				else if(i==count && count>0)
				{
					
					ret=0;
					ret=dup2(pipes[(i-1)*2],0);
					if(ret>0)
						{//perror("pipe4c");
						
						}
					for(j=0;j<2*count;j++)
						close(pipes[j]);

				}
				else {

					}



				if((cmdcount>=2) && (inputflag>0))
				{
					inputfile=fopen(cmdarg[inputflag+1],"r+");


					if(inputfile==NULL)
					{
						printf("File cannot be opened\n");
						exit(0);
					}

					fd=fileno(inputfile);
					ret=dup2(fd,0);
					if(ret<0)
						printf("dup2 failed\n");


					close(fd);
					fclose(inputfile);
					for(k=inputflag;k<(cmdcount+1);k++)
					{

						if(cmdarg[k+2]==NULL)
							break;
						strcpy(cmdarg[k],cmdarg[k+2]);
					}
					cmdarg[k]=NULL;
					inputflag=0;
					if(outputflag>0)
						outputflag-=2;
				}

				if((cmdcount>=2) && (inputflag_d>0))
				{
					char *line=malloc(sizeof(char)*100);
					if(line==NULL)
					{
						printf("Memory cannot be allocated\n");
					}
					
					inputfile_d=fopen("temp","w+");
					if(inputfile_d==NULL)
					{
						printf("File cannot be opened\n");
						exit(0);
					}
					scanf(" %[^\n]s",line);
					while(strcmp(line,cmdarg[inputflag_d+1])!=0)
					{
						fputs(line,inputfile_d);
						fputs("\n",inputfile_d);

						scanf(" %[^\n]s",line);
					}
					fclose(inputfile_d);

					strcpy(cmdarg[inputflag_d],"temp");
					cmdarg[inputflag_d+1]=NULL;

					inputflag_d=0;
				


				}




			if((cmdcount>=2) && (outputflag>0)){
			
				outputfile=fopen(cmdarg[outputflag+1],"w+");

				

				if(outputfile==NULL){
					printf("File cannot be opened\n");
					exit(0);
				}
				fd=fileno(outputfile);
				cmdarg[outputflag]=NULL;
				ret=dup2(fd,1);

				if(ret<0)
					printf("dup2 failed\n");
				close(fd);
				fclose(outputfile);

				
				outputflag=0;
			}

			if((cmdcount>=2) && (outputflag_a>0)){
				//printf("entered here to redirect and append output\n");
				appendfile=fopen(cmdarg[outputflag_a+1],"a+");

				if(appendfile==NULL){
					printf("File cannot be opened\n");
					exit(0);
				}
				fd=fileno(appendfile);
				ret=dup2(fd,1);
				if(ret<0)
					printf("dup2 failed\n");
				close(fd);
				fclose(appendfile);
				cmdarg[outputflag_a]=NULL;
				
				outputflag_a=0;

			}




			if(strcmp(cmdarg[0],"pwd")==0){
				cwd();
				printf("%s\n",pwd);
				exit(4);

			}

			else if((strcmp(cmdarg[0],"echo")==0) && (cmdcount==1) && (cmdarg[1][0]=='$')){
				echo_env();
				exit(3);
			}

			else if(strcmp(cmdarg[0],"echo")==0){
				echo();
				exit(6);
			}

			else if((strcmp(cmdarg[0],"export")==0) && (cmdcount==1)){
				export_env();
				exit(5);
			}

			else if(strcmp(cmdarg[0],"grep")==0 && count == 0){
				grep();
				exit(7);
			}	

			else if(strcmp(cmdarg[0],"cd")==0){
				cd();
				exit(2);
			}

			else if(strcmp(cmdarg[0],"history")==0){
				history();
				exit(8);
			}

			else if(cmdarg[0][0] == '!'){
				exclamination();
				exit(9);
			}

			else{
				ret=execvp(cmdarg[0],cmdarg);
				if(ret<0){
					printf("Command not Found\n");
					exit(1);
				}

			}


		}

			if(count==0){
				if(backgroundflag==1){
					pid_array[job]=pid;
					
					printf("[%d]   %d\n",job,pid);
                                        waitpid(pid,&status,0);
					job=(job+1)%6;
					if(job==0)
					job=1;


				}
				else
				wait(&status);
			}

			if((strcmp(cmdarg[0],"export")==0) && (cmdcount==1) && (count==0)){
				export_env();

			}

			if((strcmp(cmdarg[0],"cd")==0) && (count==0)){
				cd();

			}
			
			free(cmdarg);


	}
	if(count>0){
		for(j=0;j<2*count;j++)
			close(pipes[j]);

		for(j=0;j<count+1;j++)
			wait(&status);
	}




	
	free(input);
	free(pipearg);




}
