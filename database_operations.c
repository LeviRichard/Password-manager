// database.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>
#include "database_operations.h"
#include "encrypt_decrypt.h"
#include "password.h"
#include "hash.h"

char hashed_master_password[32];

int db_add_entry(char *username, char *website, char *hashed_password) {
    struct json_object *root_array;

    
    FILE *fp = fopen(DB_FILE, "r");
    if (fp) {
        fseek(fp, 0, SEEK_END);
        long len = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        char *data = malloc(len + 1);
        fread(data, 1, len, fp);
        data[len] = '\0';
        fclose(fp);

        root_array = json_tokener_parse(data);
        free(data);
    } else {
        root_array = json_object_new_array();  
    }

    
    struct json_object *entry = json_object_new_object();
    json_object_object_add(entry, "username", json_object_new_string(username));
    json_object_object_add(entry, "website", json_object_new_string(website));
    json_object_object_add(entry, "password", json_object_new_string(hashed_password));

    
    json_object_array_add(root_array, entry);

    
    fp = fopen(DB_FILE, "w");
    if (!fp) return 0;
    fputs(json_object_to_json_string_ext(root_array, JSON_C_TO_STRING_PRETTY), fp);
    fclose(fp);

    json_object_put(root_array);  
    return 1;
}

int db_read_all() {
    FILE *fp = fopen(DB_FILE, "r");
    if (!fp) {
        printf("\nNo database found.\n");
        return 0;
    }

    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *data = malloc(len + 1);
    fread(data, 1, len, fp);
    data[len] = '\0';
    fclose(fp);

    struct json_object *root_array = json_tokener_parse(data);
    free(data);

    int length = json_object_array_length(root_array);
    int found_entries = 0;
    char master[32]; 
    
    char hashed_master[32];  

    printf("\nEnter your master password: ");
    mask_password(master, sizeof(master));
    hash_sha256(master, hashed_master); 
    int result = db_check_password(hashed_master);

    if(result == 1){
      printf("\nMaster Password Correct!!\n");
      printf("\nUSERNAME         WEBSITE          PASSWORD           \n");
      for(int i =0; i<50; i++){
        printf("-");
      }
      
      for(int i =0; i<length; i++){
          struct json_object *entry = json_object_array_get_idx(root_array,i);
          const char *username = json_object_get_string(json_object_object_get(entry, "username"));
          const char *website = json_object_get_string(json_object_object_get(entry, "website"));
          const char *password = json_object_get_string(json_object_object_get(entry, "password"));
          char *decrypted_password = NULL;
          
          if(memcmp(password,hashed_master,32)==0){
            continue;
          }
          found_entries++;

          if(decrypt_password(password,master,&decrypted_password) == 1){
            
            printf("\n%d: %s  |  %s  |  %s\n",found_entries,username,website,decrypted_password);
          }
          else{
            printf("\nFailed to decrypt password!\n");
          }
          
          free (decrypted_password);
      }
    }
    else{
      printf("\nMaster Password Inorrect!!\n");
    }
    
  
   

    if (found_entries == 0) {
        printf("\nNo saved password was found!\n");
    }

    json_object_put(root_array);
    return found_entries;
}

int db_check_password(char *hashed_master) {
    FILE *fp = fopen(DB_FILE, "r");
    if (!fp) {
        printf("No database found.\n");
        return 0;
    }

    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *data = malloc(len + 1);
    fread(data, 1, len, fp);
    data[len] = '\0';
    fclose(fp);

    struct json_object *root_array = json_tokener_parse(data);
    free(data);

    int length = json_object_array_length(root_array);
    for (int i = 0; i < length; i++) {
        struct json_object *entry = json_object_array_get_idx(root_array, i);
        const char *password = json_object_get_string(json_object_object_get(entry, "password"));

        if (memcmp(password, hashed_master,32) == 0) {
            json_object_put(root_array);
            return 1;  
        }
    }

    json_object_put(root_array);
    return 0;
    
}


int database_exists(){
  FILE * fp = fopen(DB_FILE, "r");
  if(fp){
    fclose(fp);
    return 1;
  }
  else{
    return 0;
  }

}

void delete_db(){
  if(remove(DB_FILE) == 0){
    printf("\nDatabase Deleted successfully");
    
  }
  else{
    printf("\nFailed to delete database!");
    
  }
}
