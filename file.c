#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<string.h>
#include<dirent.h>
#include <unistd.h>

struct stat st;
struct dirent *de;

typedef struct file{
  char name[20];
  struct file *left,*right;
}file;

file *head=NULL;

void create_parent_directory(){
  char dirname[20];
  file *temp;
  temp=(file*)malloc(sizeof(file));
  printf("\nEnter directory name:-");
  scanf("%s",temp->name);
  temp->left=temp->right=NULL;
  if(stat(temp->name,&st)==-1){     //check dir present or not if not return -1
      mkdir(temp->name,0777);
      printf("\nDirectory is created\n");
    }
  else
      printf("\nDirectory already exist\n");
  head=temp;	// make head to that dir NO change in that
}

file* create_directory(file *head){
  char dirname[20];
  file *temp;
  temp=(file*)malloc(sizeof(file));
  printf("Enter dir name:-");
  scanf("%s",temp->name);
  temp->left=temp->right=NULL;
  if(stat(temp->name,&st)==-1){   //check dir present or not if not return -1
		 mkdir(temp->name,0777);
		 printf("\nDirectory is created\n");
		 
		 if(head==NULL){
			head=temp;
		}
		 else{
			  file *curr=head;
			  file *parent;
			  while(curr){		// In this loop BST system is use
				 parent=curr;	
				 if(strcmp(temp->name,curr->name)<0) 	// If dir is less than curr then make it left of curr
				curr=curr->left;
				 else if(strcmp(temp->name,curr->name)>0)	// If dir is gretter than curr then make it right of curr
				curr=curr->right;
				 else break;
			    }
			  if(strcmp(temp->name,parent->name)<0)		// If dir is less than parent then make it left of parent
			    parent->left=temp;
			  else if(strcmp(temp->name,parent->name)>0)	// If dir is right than parent then make it right of parent
			    parent->right=temp;
		}
    }
  else{
    		printf("\nDirectory already exist\n");
  		return head;
	}
}

file * create_file(file *head,char f_name[]){
	FILE *fp;
	file *temp;
	temp=(file*)malloc(sizeof(file));
	strcpy(temp->name,f_name);	// take file on temp pt* var
	temp->left=temp->right=NULL;
	fp=fopen(temp->name,"w");	// create file if nt exits
	if(fp==NULL)
	    printf("\nFile not created\n");
	else{
		 printf("\nFile is created\n");
		 if(head==NULL)
			head=temp;
		 else{
			  file *curr=head;
			  file *parent;
			  while(curr){
				 parent=curr;
				 if(strcmp(temp->name,curr->name)<0)	// same process as we cn do for dir
				curr=curr->left;
				 else if(strcmp(temp->name,curr->name)>0)
				curr=curr->right;
				 else break;
			  }
			  if(strcmp(temp->name,parent->name)<0)
			    parent->left=temp;
			  else if(strcmp(temp->name,parent->name)>0)
			    parent->right=temp;
			}
	    }
	  return head;
}

void write_file(){
  char file_name[20],buffer[100];
  printf("\nEnter file name:-");
  scanf("%s",file_name);
  FILE *fp;
  fp=fopen(file_name,"a");
  if(fp==NULL)
    printf("\nNo such file exist\n");
  else{
      printf("Enter text :-");
      scanf("%s",buffer);	// accept data in buff
      fputs(buffer,fp);		
      printf("\nData are inserted\n");
    }
}

void read_file(){
  char fname[20],line[100];
  FILE *fp;
  printf("\nEnter a file name:-");
  scanf("%s",fname);
  fp=fopen(fname,"r");
  if(fp==NULL)
    printf("\nDon't exist?\n");
  else{
      printf("\nFile content are..\n");
      while(fgets(line,100,fp)!=NULL)
	printf("%s",line);
      printf("\n");
    }
}


int search_directory(char name[]){
  if(strcmp(name,head->name)==0)
    return 1;
  file *temp;
  temp=head->left;
  while(temp)
    {
      if(strcmp(name,temp->name)==0)
	  return 1;
      else if(strcmp(name,temp->name)<0)
	temp=temp->left;	// temp is less
      else
	temp=temp->right;	//gretter
    }
  return 0;
}

int search_file(char name[]){
	if(strcmp(name,head->name)==0)
		return 1;
	file *temp;
	temp=head->right;
	while(temp){
		if(strcmp(name,temp->name)==0)
		return 1;
		else if(strcmp(name,temp->name)<0)
		temp=temp->left;
		else
		temp=temp->right;
	}
	return 0;
}


void delete_directory(char rm_dir_name[]){
  int check;
  check=rmdir(rm_dir_name);		
  if(!check)
    printf("Directory Deleted successfully\n");
  else
    printf("No such directory\n");
}

void delete_file(char rm_file[]){
  if(remove(rm_file) == 0) 
      printf("File Deleted successfully\n"); 
   else
      printf("No such file\n");
}

void delete_directories(char name[]){
  int found;
  found=search_directory(name);		// goto s_d() func
  if(found==1)
	    {
		 file *curr ,*parent;
		 curr=head->left;
		 parent=head->left;
		 while(strcmp(curr->name,name)!=0){	// cmp h->left is same as new entered
			  parent=curr;
			  if(strcmp(name,curr->name)<0)	// if curr->lft is same as new thn make ( curr->lft is curr)
			    curr=curr->left;
			  else
			    curr=curr->right;
		}
		 
		 delete_directory(name);			// goto d_d() func
	
		 if(curr->left==NULL && curr->right==NULL){				//.................both NULL
			  if(curr==parent->left)
				 parent->left=NULL;
			  else
			    parent->right=NULL;
			  free(curr);
		}else if(curr->left!=NULL && curr->right==NULL){			//................left NULL
		  if(curr==parent){		// if new is parent
			 file *temp;
			 temp=curr->left;
			 strcpy(parent->name,temp->name);	// curr->lft to parent
			 parent->left=temp->left;
			 free(temp);
			 return;
		    }
		  else if(curr==parent->left)	// if new is p->lft
			 parent->left=curr->left;
		  else
				 parent->right=curr->left;
			  	 curr->left == NULL;
		  	free(curr);
		}
		 
		 else if(curr->left==NULL && curr->right!=NULL){			// ................right NULL
			  if(curr==parent){
				 file *temp;
				 temp=curr->right;
				 strcpy(parent->name,temp->name);
				 parent->right=temp->right;
				 free(temp);
				 return;
			  }
			  
			  if(curr==parent->left)
				 parent->left=curr->right;
			  else
			    	  	parent->right=curr->right;
			  		curr->right==NULL;
				  free(curr);
		} 
		 
		else if(curr->left!=NULL && curr->right!=NULL){			//...................both NOT NULL
			  file *temp1, *temp2;
			  temp1=curr->right;
			  while(temp1->left!=NULL){
				 temp2=temp1;
				 temp1=temp1->left;
			  }
			  
			  strcpy(curr->name,temp1->name);
			  if(temp1->left==NULL && temp1->right!=NULL)
				 temp2->left=temp1->right;
			  else
			  	  temp2->left=NULL;
			  free(temp1);
		}
	}
	else{
  	  printf("\nNo such directory exist\n\n");
  	}  
}

void delete_files(char name[]){	// as to dir
		int found;
		found=search_file(name);
		 if(found==1){
			 file *curr,*parent;
			 curr=head->right;
			 parent=head->right;
			 
			 while(strcmp(curr->name,name)!=0){
				  parent=curr;
				  if(strcmp(name,curr->name)<0)
				    curr=curr->left;
				  else
				    curr=curr->right;
			 }
			 
			 delete_file(name);
	
			 if(curr->left==NULL && curr->right==NULL){
				  if(curr==parent->left)
					 parent->left=NULL;
				  else
				    parent->right=NULL;
				  free(curr);
			}
			 
			 else if(curr->left!=NULL && curr->right==NULL){
				  if(curr==parent){
					 struct file *s;
					 s=curr->left;
					 strcpy(parent->name,s->name);
					 parent->left=s->left;
					 free(s);
					 return;
				  }
				  else if(curr==parent->left)
					 parent->left=curr->left;
				  else
						 parent->right=curr->left;
				  		 curr->left==NULL;
					  free(curr);
			}
			 
			 else if(curr->left==NULL && curr->right!=NULL)	{
				  if(curr==parent){
					 file *temp;
					 temp=curr->right;
					 strcpy(parent->name,temp->name);
					 parent->right=temp->right;
					 free(temp);
					 return;
				   }
				  if(curr==parent->left)
					 parent->left=curr->right;
				  else
						  parent->right=curr->right;
						  curr->right==NULL;
					  free(curr);
			}
			 
			else if(curr->left!=NULL && curr->right!=NULL){
				  file *temp1, *temp2;
				  temp1=curr->right;
				  *temp2;
				  while(temp1->left!=NULL){
					 temp2=temp1;
					 temp1=temp1->left;
				  }
				  
				  strcpy(curr->name,temp1->name);
				  if(temp1->left==NULL && temp1->right!=NULL){
					 temp2->left=temp1->right;
				  }	 
				  else{
				  	  temp2->left=NULL;
				  	  free(temp1);
				  } 	  
			}
	  }
	  else{
	   	 printf("\nNo such file exist\n\n");
	  }  
}
/*    printf("\nFile content are..\n");
      while(fgets(line,100,fp)!=NULL)
	printf("%s",line);
      printf("\n");
*/
void get_cont(){
	
	char fname[15], line[50]	;
	FILE *fp;
	scanf("%s",fname);
	fp=fopen(fname,"r");
	if(fp==NULL){
		exit(0);
	}else{
		while(1){
						
		}
	}
	
}

void dis_cont(){
	
}

void main(int argc, char **argv){
  int ch;
  char file_name[20];
  create_parent_directory();
  do{
      printf("\n1.Create File");
      printf("\n2.Open File for Writting");
      printf("\n3.Open File for Reading");
      printf("\n4.Delete File");
      printf("\n5.Exit");
      printf("\nEnter your choice:-");
      scanf("%d",&ch);
      switch(ch){
		case 1:
		  printf("\nEnter file name:-");
		  scanf("%s",file_name);
		  head->right=create_file(head->right,file_name);
		  break;
		  
		case 2:
		  write_file();
		  break;

		case 3:
		  read_file();
		  break;

		case 4:
		  printf("\nEnter file name:-");
		  scanf("%s",file_name);
		  delete_files(file_name);
		  break;

		case 5:
		  exit(0);
		  break;

		default:printf("\nInvalid choice \n");
		  break;
		}
	}while(ch!=5);
	get_cont(file_name);
	dis_cont();	
}
