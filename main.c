#include <stdio.h>
#include <string.h>
//#include <stdbool.h>
#include "hash.h"
#include "password.h"
#include "database_operations.h"
#include "encrypt_decrypt.h"

#define max_length 15



void create_master();
void view_password();
void add_password();
void reset_master();

int main(){
  
  
  int loop_condition = 1;
  int choice;
  while(loop_condition){
    printf("\n=================MENU=================\n");
    printf("1.Create Master Password\n");
    printf("2.View saved passwords\n");
    printf("3.Add new password\n");
    printf("4.Reset master password\n");
    printf("5.Exit\n");
    for(int i=0; i<38; i++){
      printf("-");
    }
    printf("\nEnter choice: ");
    scanf("%d",&choice);
    getchar();
    fflush(stdin);
    //printf("\n");
    for(int i=0; i<38; i++){
      printf("-");
    }


    

    switch(choice){
      case 1:
        create_master();
        break;
      case 2:
        view_password();
        break;
      case 3:
        add_password();
        break;
      case 4:
        reset_master();
        break;
      case 5:
        printf("\nExiting........\n");
        loop_condition =0;
        break;
      default:
        printf("Invalid option\n");
    }


  }
  

  
}

void create_master(){
  char master [max_length];
  char hashed_master[32];
  char master_username[20];
  char *master_website = "None";
  printf("\n\n---------CREATE MASTER PASSWORD-----------------\n");
  printf("\nNote: Creating a new master password creates a new database\n");

  if(database_exists() == 0){
    printf("\nEnter master password: ");
    //fgets(master, sizeof(master),stdin);
    //master[strcspn(master, "\n")]=0;
    mask_password(master, max_length);
    printf("\nEnter username: ");
    fgets(master_username,sizeof(master_username),stdin);
    master_username[strcspn(master_username, "\n")]=0;

    hash_sha256(master,hashed_master);
    strcpy(hashed_master_password,hashed_master);
    db_add_entry(master_username, master_website, hashed_master);

    printf("\nMatster Password Created!!\n");
  }
  else{
    printf("\nUser present....reset master password if you want to change user!\n");
  }
}

void view_password(){
  printf("\n\n-----------------SAVED PASSWORDS-------------------\n");
  int number = db_read_all();
  for(int i=0; i<50; i++){
    printf("-");
  }
  printf("\nNumber of records found: %d",number);
  printf("\n");
  for(int i=0; i<50; i++){
    printf("-");
  }
  printf("\n");
}

void add_password(){
  int max_len = 32;
  char username[20];
  char website[20];
  char password[max_len];
  char master [max_len];
  char hashed_master [max_len];
  char *encrypted_password =NULL;

  printf("\n\n--------------ADD PASSWORD------------------------");
  printf("\nEnter Your Master password: ");
  mask_password(master,max_len);
  hash_sha256(master,hashed_master);
  int result = db_check_password(hashed_master);
  if(result == 1){
    printf("\nEnter your Username: ");
    fgets(username, sizeof(username),stdin);
    username[strcspn(username,"\n")]=0;
    printf("\nEnter website: ");
    fgets(website,sizeof(website),stdin);
    website[strcspn(website,"\n")]=0;
    printf("\nEnter password to be stored: ");
    mask_password(password,max_len);
    if(encrypt_password(password, master, &encrypted_password) == 1 && db_add_entry(username,website,encrypted_password) == 1){
      printf("\n");
      for(int i=0; i< 30; i++){
        printf("-");
      }
      printf("\nPassword saved succesfully!!\n");
      for(int i=0; i<30; i++){
        printf("-");
      }

    }
    else{
      printf("\nPassword failed to save!!\n");
    }
  }
  else{
    printf("\nMaster password incorrect...Correct Master needed to add new passwords to existing database!!\n");
  }
  
  
}


void reset_master(){
  char choice;
  int exist = database_exists();
  printf("\n------------------RESET MASTER----------------------\n");
  printf("\nResetting master will delete the existing database\n");
  printf("Are you sure you want to carry on?\n");
  printf("Enter 'Y' or 'N': ");
  scanf("%s",&choice);
  getchar();
  fflush(stdin);
  if(choice == 'Y' || choice == 'y'){
    if(database_exists() == 1){
      delete_db();
      printf("\nCreate a new master password using the create master option!");
    }
    else{
      printf("\nDatabase does not Exist!");
    }
  }
  else if(choice == 'N' || choice == 'n'){
    printf("\nReset Aborted");
  }
}
