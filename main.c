#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"
#define WHITE   "\033[1;37m"

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
        printf(RED "[ERR] Failed to initialize the list\n" RESET);
        exit(EXIT_FAILURE);
    }

    services->counter = 0;
    services->capacity = 2;

    services->services = malloc(sizeof(Service)*services->capacity);
    if(!services->services) {
        printf(RED "[ERR] Failed to initialize the list\n" RESET);
        free(services);
        exit(EXIT_FAILURE);
    }

    return services;
}

void free_services(list_of_service *service) {
    if(!service) return;
    if(service->services) {
        free(service->services);
        service->services = NULL;
    }

    free(service);
}

void add_service(list_of_service *ptr,char *name,char *password,adding_service_way way) {
    char answear[200];
    char newanswear[200];
    char users_password[200];

    if(way == ADD_BY_USER) {
        while(1) {
            printf(YELLOW "\nAre you sure you want to add that? yes/no: " RESET);
            fgets(answear,sizeof(answear),stdin);
            answear[strcspn(answear,"\n")] = 0;

            if(strcmp(answear,"yes")==0) {
                printf(CYAN "Enter the service's password: " RESET);
                fgets(users_password,sizeof(users_password),stdin);
                users_password[strcspn(users_password,"\n")] = 0;

                while(1) {
                    printf(YELLOW "Are you sure you want this password? yes/no: " RESET);
                    fgets(newanswear,sizeof(newanswear),stdin);
                    newanswear[strcspn(newanswear,"\n")] = 0;

                    if(strcmp(newanswear,"yes")==0) {
                        if(ptr->counter >= ptr->capacity) {
                            int new_capacity = ptr->capacity *= 2;
                            Service *temp = realloc(ptr->services,sizeof(Service)*new_capacity);
                            if(!temp) {
                                printf(RED "[ERR] Failed to allocate memory\n" RESET);
                                return;
                            }

                            ptr->capacity = new_capacity;
                            ptr->services = temp;
                        }

                        ptr->services[ptr->counter].id = ptr->counter+1;
                        strcpy(ptr->services[ptr->counter].name,name);
                        strcpy(ptr->services[ptr->counter].password,users_password);

                        ptr->counter++;
                        printf(GREEN "[OK] Service added succesfully\n\n" RESET);
                        return;
                    } else if(strcmp(newanswear,"no")==0) {
                        printf("\n");
                        return;
                    } else {
                        printf(RED "[ERR] Invalid answear: '%s'\n" RESET,newanswear);
                        continue;
                    }
                }
            } else if(strcmp(answear,"no")==0) {
                printf("\n");
                return;
            } else {
                printf(RED "[ERR] Invalid answear '%s'\n" RESET,answear);
                continue;
            }
        }
    } else if(way == ADD_BY_FILE) {
        if(ptr->counter >= ptr->capacity) {
            int new_capacity = ptr->capacity *= 2;
            Service *temp = realloc(ptr->services,sizeof(Service)*new_capacity);
            if(!temp) {
                printf(RED "[ERR] Failed to load more services due to memory issues\n" RESET);
                return;
            }

            ptr->capacity = new_capacity;
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
        printf(RED "[ERR] Service list is empty\n" RESET);
        return;
    }

    if(ptr->counter < id) {
        printf(RED "[ERR] This task does not exists\n" RESET);
        return;
    }

    char answear[200];
    while(1) {
        printf(YELLOW "\nAre you sure you want to delete this service? yes/no: " RESET);
        fgets(answear,sizeof(answear),stdin);
        answear[strcspn(answear,"\n")] = 0;

        if(strcmp(answear,"yes")==0) {
            for(int i=id-1; i<ptr->counter-1; i++)
                ptr->services[i] = ptr->services[i+1];
            ptr->counter--;

            if(ptr->counter != 0)
                for(int i=0; i<ptr->counter; i++) ptr->services[i].id = i+1;

            printf(GREEN "[OK] Task deleted succesfully\n\n" RESET);
            return;
        } else if(strcmp(answear,"no")==0) {
            printf("\n");
            return;
        } else {
            printf(RED "[ERR] Invalid answear '%s'\n" RESET,answear);
            continue;
        }
    }
}

void list_services(list_of_service *ptr,list_type type) {
    if(ptr->counter == 0) {
        printf(RED "[ERR] Service list is empty\n" RESET);
        return;
    }

    if(type == LIST) {
        printf(CYAN "\n=== Services List ===\n" RESET);
        for(int i=0; i<ptr->counter; i++) {
            printf(MAGENTA "%d) " CYAN "%s\n" RESET
                    ,ptr->services[i].id
                    ,ptr->services[i].name);
        }
        printf(CYAN "=====================\n\n" RESET);
    } else if(type == LIST_PASSWORDS) {
        printf(CYAN "\n=== Services with Passwords ===\n" RESET);
        for(int i=0; i<ptr->counter; i++) {
            printf(MAGENTA "%d) " CYAN "%s" YELLOW " : " GREEN "%s\n" RESET
                    ,ptr->services[i].id
                    ,ptr->services[i].name
                    ,ptr->services[i].password);
        }
        printf(CYAN "===============================\n\n" RESET);
    }
}

void save_to_file(list_of_service *ptr) {
    FILE *file = fopen(".passwords","w");
    if(!file) {
        printf(RED "[ERR] Failed to save the passwords\n" RESET);
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
    printf(CYAN "\n=== Available Commands ===\n" RESET);
    printf(YELLOW "  add \"service_name\"        " RESET "- adds a service\n");
    printf(YELLOW "  remove <service_number>   " RESET "- deletes a service\n");
    printf(YELLOW "  list                      " RESET "- lists services\n");
    printf(YELLOW "  list passwords            " RESET "- lists services with their passwords\n");
    printf(YELLOW "  help                      " RESET "- prints this panel\n");
    printf(YELLOW "  exit                      " RESET "- exits the program\n");
    printf(CYAN "==========================\n\n" RESET);
}

int main(void) {
    char input[256];
    char *args[10];

    printf(CYAN "\n=== Password Manager ===\n" RESET);
    printf(YELLOW "Type 'help' to see available commands\n\n" RESET);
    
    list_of_service *services = init();
    load_from_file(services);

    while(1) {
        printf(GREEN ">> " RESET);
        fflush(stdout);
        
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
                printf(RED "\n[ERR] Invalid usage '%s'\n" RESET,input);
                printf(YELLOW "[HELP] Correct usage: add \"service_name\"\n\n" RESET);
                continue;
            }

            add_service(services,args[1],0,ADD_BY_USER);
            continue;
        } else if(strcmp(args[0],"remove")==0) {
            if(counter != 2) {
                printf(RED "\n[ERR] Invalid usage '%s'\n" RESET,input);
                printf(YELLOW "[HELP] Correct usage: remove <service_number>\n\n" RESET);
                continue;
            }

            char *endPtr;
            int id = strtol(args[1],&endPtr,10);
            if(*endPtr != '\0') {
                printf(RED "\n[ERR] Invalid service number '%s'\n" RESET,args[1]);
                printf(YELLOW "[HELP] Service number must be an integer\n\n" RESET);
                continue;
            }

            remove_service(services,id);
            continue;
        } else if(strcmp(args[0],"list")==0) {
            if(counter != 2 && counter != 1) {
                printf(RED "\n[ERR] Invalid usage '%s'\n" RESET,input);
                printf(YELLOW "[HELP] Correct usage: list [OR] list passwords\n\n" RESET);
                continue;
            }

            if(counter == 1) {
                list_services(services,LIST);
                continue;
            }

            if(counter == 2 && strcmp(args[1],"passwords")!=0) {
                printf(RED "\n[ERR] Invalid list mode '%s'\n" RESET,args[1]);
                printf(YELLOW "[HELP] Available list modes: list [OR] list passwords\n\n" RESET);
                continue;
            }
    
            list_services(services,LIST_PASSWORDS);
            continue;
        } else if(strcmp(args[0],"help")==0) {
            if(counter != 1) {
                printf(RED "\n[ERR] Invalid usage '%s'\n" RESET,input);
                printf(YELLOW "[HELP] Correct usage: help\n\n" RESET);
                continue;
            }

            print_help();
            continue;
        } else if(strcmp(args[0],"exit")==0) {
            if(counter != 1) {
                printf(RED "\n[ERR] Invalid usage '%s'\n" RESET,input);
                printf(YELLOW "[HELP] Correct usage: exit\n\n" RESET);
                continue;
            }

            save_to_file(services);
            printf(CYAN "\n[EXIT] Exiting Password Manager...\n" RESET);
            free_services(services);
            return 0;
        } else {
            printf(RED "\n[ERR] Invalid command '%s'\n" RESET,input);
            printf(YELLOW "[HELP] Type 'help' for more details\n\n" RESET);
            continue;
        }
    }

    return 0;
}
