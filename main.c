#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
    LIST,
    LIST_PASSWORDS,
} list_type;

typedef enum {
    ADD_BY_USER,
    ADD_BY_FILE,
} adding_service_way;

typedef struct Service {
    int id;
    char name[256];
    char password[256]; 
} Service;

typedef struct  list_of_service {
    Service *services;
    int capacity;
    int counter;
} list_of_service;

list_of_service *init() {
    list_of_service *services = malloc(sizeof(list_of_service));
    if(!services) {
        printf("[ERR] Failed to initialize the list\n");
        exit(EXIT_FAILURE);
    }

    services->counter = 0;
    services->capacity = 2;

    services->services = malloc(sizeof(Service)*services->capacity);
    if(!services->services) {
        printf("[ERR] Failed to initialize the list\n");
        free(services);
        exit(EXIT_FAILURE);
    }

    return services;
}

void free_services(list_of_service *service) {
    if(!service && !service->services) return;

    free(service->services);
    service->services = NULL;

    free(service);
    service = NULL;
}

void add_service(list_of_service *ptr,char *name,char *password,adding_service_way way) {
    char answear[200];
    char newanswear[200];
    char users_password[200];

    if(way == ADD_BY_USER) {
        while(1) {
            printf("\nAre you sure you want to add that? yes/no: ");
            fgets(answear,sizeof(answear),stdin);
            answear[strcspn(answear,"\n")] = 0;

            if(strcmp(answear,"yes")==0) {
                printf("Enter the service's password: ");
                fgets(users_password,sizeof(users_password),stdin);
                users_password[strcspn(users_password,"\n")] = 0;

                while(1) {
                    printf("Are you sure you want this password? yes/no: ");
                    fgets(newanswear,sizeof(newanswear),stdin);
                    newanswear[strcspn(newanswear,"\n")] = 0;

                    if(strcmp(newanswear,"yes")==0) {
                        if(ptr->counter >= ptr->capacity) {
                            ptr->capacity *= 2;
                            Service *temp = realloc(ptr->services,sizeof(Service)*ptr->capacity);
                            if(!temp) {
                                printf("[ERR] Failed to allocate memory\n");
                                return;
                            }

                            ptr->services = temp;
                        }

                        ptr->services[ptr->counter].id = ptr->counter+1;
                        strcpy(ptr->services[ptr->counter].name,name);
                        strcpy(ptr->services[ptr->counter].password,users_password);

                        ptr->counter++;
                        printf("[OK] Service added succesfully\n\n");
                        return;
                    } else if(strcmp(newanswear,"no")==0) {
                        printf("\n");
                        return;
                    } else {
                        printf("[ERR] Invalid answear: '%s'\n",newanswear);
                        continue;
                    }
                }
            } else if(strcmp(answear,"no")==0) {
                printf("\n");
                return;
            } else {
                printf("[ERR] Invalid answear '%s'\n",answear);
                continue;
            }
        }
    } else if(way == ADD_BY_FILE) {
        if(ptr->counter >= ptr->capacity) {
            ptr->capacity *= 2;
            Service *temp = realloc(ptr->services,sizeof(Service)*ptr->capacity);
            if(!temp) {
                printf("[ERR] Failed to load more services due to memory issues\n");
                return;
            }

            ptr->services = temp;
        }

        ptr->services[ptr->counter].id = ptr->counter+1;
        strcpy(ptr->services[ptr->counter].name,name);
        strcpy(ptr->services[ptr->counter].password,password);

        ptr->counter++;
    }
}

void remove_service(list_of_service *ptr,int id) {
    if(ptr->counter == 0) {
        printf("[ERR] Service list is empty\n");
        return;
    }

    if(ptr->counter < id) {
        printf("[ERR] This task does not exists\n");
        return;
    }

    char answear[200];
    while(1) {
        printf("\nAre you sure you want to delete this service? yes/no: ");
        fgets(answear,sizeof(answear),stdin);
        answear[strcspn(answear,"\n")] = 0;

        if(strcmp(answear,"yes")==0) {
            for(int i=id-1; i<ptr->counter-1; i++)
                ptr->services[i] = ptr->services[i+1];
            ptr->counter--;

            if(ptr->counter != 0)
                for(int i=0; i<ptr->counter; i++) ptr->services[i].id = i+1;

            printf("[OK] Task deleted succesfully\n\n");
            return;
        } else if(strcmp(answear,"no")==0) {
            printf("\n");
            return;
        } else {
            printf("[ERR] Invalid answear '%s'\n",answear);
            continue;
        }
    }
}

void list_services(list_of_service *ptr,list_type type) {
    if(ptr->counter == 0) {
        printf("[ERR] Service list is empty\n");
        return;
    }

    if(type == LIST) {
        printf("\n");
        for(int i=0; i<ptr->counter; i++) {
            printf("%d) %s\n"
                    ,ptr->services[i].id
                    ,ptr->services[i].name);
        }
        printf("\n");
    } else if(type == LIST_PASSWORDS) {
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
        printf("[ERR] Failed to save the passwords\n");
        return;
    }
    
    for(int i=0; i<ptr->counter; i++) {
        fprintf(file,"%s|%s|\n" ,
                ptr->services[i].name,
                ptr->services[i].password);
    }

    fclose(file);
}

void load_from_file(list_of_service *ptr) {
    FILE *file = fopen(".passwords","r");
    if(!file) return;

    char line[256];
    char name[256];
    char password[256];

    while(fgets(line,sizeof(line),file)) {
        if(sscanf(line,"%[^|]|%[^|]|",name,password)==2) {
            add_service(ptr,name,password,ADD_BY_FILE);
        }
    }

    fclose(file);
}

void print_help() {
    printf("\nCommands:\n");
    printf("  add \"service_name\"        - adds a service\n");
    printf("  remove <service_number>   - deletes a service\n");
    printf("  list / list passwords     - lists services / lists services and there passwords\n");
    printf("  help                      - prints this panel\n");
    printf("  exit                      - exits the program\n\n");
}

int main(void) {
    char input[256];
    char *args[10];

    list_of_service *services = init();
    load_from_file(services);

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

        if(strcmp(args[0],"add")==0) {
            if(counter != 2) {
                printf("\n[ERR] Invalid usage '%s'\n",input);
                printf("[ERR] Correct usage: add \"service_name\"\n\n");
                continue;
            }

            add_service(services,args[1],0,ADD_BY_USER);
            continue;
        } else if(strcmp(args[0],"remove")==0) {
            if(counter != 2) {
                printf("\n[ERR] Invalid usage '%s'\n",input);
                printf("[ERR] Correct usage: remove <service_number>\n\n");
            }

            char *endPtr;
            int id = strtol(args[1],&endPtr,10);
            if(*endPtr != '\0') {
                printf("[ERR] Invalid service number '%s'\n",args[1]);
                printf("[ERR] Service number must be an integer\n");
                continue;
            }

            remove_service(services,id);
            continue;
        } else if(strcmp(args[0],"list")==0) {
            if(counter != 2 && counter != 1) {
                printf("\n[ERR] Invalid usage '%s'\n",input);
                printf("[ERR] Correct usage: list [OR] list passwords\n\n");
                continue;
            }

            if(counter == 1) {
                list_services(services,LIST);
                continue;
            }

            if(counter == 2 && strcmp(args[1],"passwords")!=0) {
                printf("\n[ERR] Invalid list mode '%s'\n",args[1]);
                printf("[ERR] Available list modes: list [OR] list passwords\n\n");
                continue;
            }
    
            list_services(services,LIST_PASSWORDS);
            continue;
        } else if(strcmp(args[0],"help")==0) {
            if(counter != 1) {
                printf("\n[ERR] Invalid usage '%s'\n",input);
                printf("[ERR] Correct usage: help\n\n");
                continue;
            }

            print_help();
            continue;
        } else if(strcmp(args[0],"exit")==0) {
            if(counter != 1) {
                printf("\n[ERR] Invalid usage '%s'\n",input);
                printf("[ERR] Correct usage: exit\n\n");
                continue;
            }

            save_to_file(services);
            printf("[EXIT] Exiting...\n");
            free_services(services);
            return 0;
        } else {
            printf("[ERR] Invalid command '%s'\n",input);
            printf("[ERR] Type 'help' for more details\n");
            continue;
        }
    }

    return 0;
}
