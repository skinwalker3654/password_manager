#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
    PRINT,
    PRINT_PASSWORDS,
} list_type;

typedef struct Service {
    int id;
    char name[256];
    char password[256]; 
} Service;

typedef struct  list_of_service {
    Service services[200];
    int counter;
} list_of_service;

void add_service(list_of_service *ptr,char *name) {
    char enswear[200];
    char newenswear[200];
    char password[200];
    while(1) {
        printf("\nAre you sure? yes/no: ");
        fgets(enswear,sizeof(enswear),stdin);
        enswear[strcspn(enswear,"\n")] = 0;

        if(strcmp(enswear,"yes")==0) {
            printf("Enter service password: ");
            fgets(password,sizeof(password),stdin);
            password[strcspn(password,"\n")] = 0;

            while(1) {
                printf("Are you sure you want this password? yes/no: ");
                fgets(newenswear,sizeof(newenswear),stdin);
                newenswear[strcspn(newenswear,"\n")] = 0;

                if(strcmp(newenswear,"yes")==0) {
                    ptr->services[ptr->counter].id = ptr->counter+1;
                    strcpy(ptr->services[ptr->counter].name,name);
                    strcpy(ptr->services[ptr->counter].password,password);

                    ptr->counter++;
                    printf("[OK] service added succesfully\n\n");
                    return;
                } else if(strcmp(newenswear,"no")==0) {
                    return;
                } else {
                    printf("Error: Invalid enswear: '%s'\n",newenswear);
                    continue;
                }
            }
            ptr->services[ptr->counter].id = ptr->counter++;
            strcpy(ptr->services[ptr->counter].name,name);
        } else if(strcmp(enswear,"no")==0) {
            return;
        } else {
            printf("Error: Invalid enswear '%s'\n",enswear);
            continue;
        }
    }
}

void remove_service(list_of_service *ptr,int id) {
    if(ptr->counter == 0) {
        printf("[ERR] service list is empty\n");
        return;
    }

    if(ptr->counter < id) {
        printf("[ERR] this task does not exists\n");
        return;
    }

    char enswear[200];
    while(1) {
        printf("are you sure? yes/no: ");
        fgets(enswear,sizeof(enswear),stdin);
        enswear[strcspn(enswear,"\n")] = 0;

        if(strcmp(enswear,"yes")==0) {
            for(int i=id-1; i<ptr->counter-1; i++)
                ptr->services[i] = ptr->services[i+1];
            ptr->counter--;

            if(ptr->counter != 0)
                for(int i=0; i<ptr->counter; i++) ptr->services[i].id = i+1;

            printf("[OK] task deleted succesfully\n");
            return;
        } else if(strcmp(enswear,"no")==0) {
            return;
        } else {
            printf("Error: Invalid enswear '%s'\n",enswear);
            continue;
        }
    }
}

void list_services(list_of_service *ptr,list_type type) {
    if(ptr->counter == 0) {
        printf("[ERR] service list is empty\n");
        return;
    }

    if(type == PRINT) {
        printf("\n");
        for(int i=0; i<ptr->counter; i++) {
            printf("%d) %s\n"
                    ,ptr->services[i].id
                    ,ptr->services[i].name);
        }
        printf("\n");
    } else if(type == PRINT_PASSWORDS) {
        printf("\n");
        for(int i=0; i<ptr->counter; i++) {
            printf("%d) %s : %s\n"
                    ,ptr->services[i].id
                    ,ptr->services[i].name
                    ,ptr->services[i].password);
        }
        printf("\n");
    }
}

void save_to_file(list_of_service *ptr) {
    FILE *file = fopen(".passwords","w");
    if(!file) {
        printf("[ERR] failed to save the passwords\n");
        return;
    }

    if(fwrite(ptr,sizeof(*ptr),1,file)==-1) {
        printf("[ERR] failed to write content to file\n");
        fclose(file);
        return;
    }

    fclose(file);
}

void load_from_file(list_of_service *ptr) {
    FILE *file = fopen(".passwords","r");
    if(!file) return;

    if(fread(ptr,sizeof(*ptr),1,file)==-1) {
        printf("[ERR] failed to load the contnets\n");
        fclose(file);
        return;
    }

    fclose(file);
}

void print_help() {
    printf("\nCommands:\n");
    printf("  add \"service_name\"        - adds a service\n");
    printf("  remove <service_number>   - deletes a service\n");
    printf("  list / list passwords     - lists services / lists services and there passwords\n");
    printf("  exit                      - exits the program\n");
    printf("  help                      - prints this panel\n\n");
}
int main(void) {
    char input[256];
    char *args[10];

    list_of_service services = {0};
    load_from_file(&services);

    while(1) {
        printf(">> ");
        fgets(input,sizeof(input),stdin);
        input[strcspn(input,"\n")] = 0;

        if(strlen(input)==0) continue;
        
        int counter = 0;
        char *token = strtok(input," ");
        while(token != NULL) {
            args[counter++] = token;
            token = strtok(NULL," ");
        }

        if(counter == 2) {
            if(strcmp(args[0],"add")==0) {
                add_service(&services,args[1]);
                continue;
            } else if(strcmp(args[0],"list")==0) {
                if(strcmp(args[1],"passwords")!=0) {
                    printf("Error: Cannot list '%s'\n",args[1]);
                    printf("Correct usages: list | list passwords\n");
                    counter = 0;
                    continue;
                }

                list_services(&services,PRINT_PASSWORDS);
                continue;
            } else if(strcmp(args[0],"remove")==0) {
                char *endPtr;
                int id = strtol(args[1],&endPtr,10);
                if(*endPtr != '\0') {
                    printf("[ERR] invalid task picked '%s'\n",args[1]);
                    counter = 0;
                    continue;
                }

                remove_service(&services,id);
                continue;
            } else {
                printf("Error: Invalid command '%s'\n",input);
                continue;
            }
        } else if(counter == 1) {
            if(strcmp(args[0],"list")==0) {
                list_services(&services,PRINT);
                continue;
            } else if(strcmp(args[0],"exit")==0) {
                printf("[EXIT] exiting...\n");
                save_to_file(&services);
                return 0;
            } else if(strcmp(args[0],"help")==0) {
                print_help();
                continue;
            } else {
                printf("Error: Invalid command '%s'\n",input);
                continue;
            }
        } else {
            printf("Error: Invalid command '%s'\n",input);
            continue;
        }
    }

    return 0;
}
