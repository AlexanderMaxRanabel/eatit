#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_SERVICES 10
#define MAX_RUNLEVELS 10

int runlevel = 0;

int main(int argc, char *argv[]) {
    printf("Starting init system...\n");
    int serviceCount = 0;
    char *services[MAX_SERVICES];
    int servicePIDs[MAX_SERVICES];
    int serviceRunlevels[MAX_SERVICES];
    
    // Read services from configuration file
    FILE *configFile = fopen("/etc/init.conf", "r");
    if(configFile != NULL) {
        char servicePath[256];
        int serviceRunlevel;
        while(fscanf(configFile, "%s %d", servicePath, &serviceRunlevel) != EOF) {
            services[serviceCount] = strdup(servicePath);
            serviceRunlevels[serviceCount] = serviceRunlevel;
            serviceCount++;
        }
        fclose(configFile);
    }

    // Start services for current runlevel
    for(int i = 0; i < serviceCount; i++) {
        if(serviceRunlevels[i] == runlevel) {
            servicePIDs[i] = fork();
            if(servicePIDs[i] == 0) {
                // Child process
                printf("Starting service: %s\n", services[i]);
                execl(services[i], services[i], NULL);
                // execl only returns if an error occurs
                printf("Error starting service: %s\n", services[i]);
                _exit(1);
            } else if(servicePIDs[i] < 0) {
                printf("Error starting service: %s\n", services[i]);
            }
        } else {
            servicePIDs[i] = 0;
        }
    }

    // Wait for services to finish
    int status;
    for(int i = 0; i < serviceCount; i++) {
        if(servicePIDs[i] > 0) {
            waitpid(servicePIDs[i], &status, 0);
            if(WIFEXITED(status)) {
                printf("Service %s exited with status %d\n", services[i], WEXITSTATUS(status));
            }
        }
    }

    // Service management
    while(1) {
        printf("Enter command (start <service>, stop <service>, runlevel <level>, status): ");
        char command[256];
        if(fgets(command, sizeof(command), stdin) != NULL) {
            command[strcspn(command, "\r\n")] = 0;
            if(strncmp(command, "start", 5) == 0) {
                char *serviceName = command + 6;
                int serviceIndex = -1;
                for(int i = 0; i < serviceCount; i++) {
                    if(strcmp(services[i], serviceName) ==
                if(serviceIndex != -1) {
                    if(servicePIDs[serviceIndex] != 0) {
                        printf("Service %s is already running\n", serviceName);
                    } else {
                        servicePIDs[serviceIndex] = fork();
                        if(servicePIDs[serviceIndex] == 0) {
                            // Child process
                            printf("Starting service: %s\n", serviceName);
execl(serviceName, serviceName, NULL);
// execl only returns if an error occurs
printf("Error starting service: %s\n", serviceName);
_exit(1);
} else if(servicePIDs[serviceIndex] < 0) {
    printf("Error starting service: %s\n", serviceName);
}
} else {
    printf("Service %s not found\n", serviceName);
}
} else if(strncmp(command, "stop", 4) == 0) {
    char *serviceName = command + 5;
    int serviceIndex = -1;
    for(int i = 0; i < serviceCount; i++) {
        if(strcmp(services[i], serviceName) == 0) {
            serviceIndex = i;
            break;
        }
    }
    if(serviceIndex != -1) {
        if(servicePIDs[serviceIndex] == 0) {
            printf("Service %s is not running\n", serviceName);
        } else {
            printf("Stopping service: %s\n", serviceName);
            kill(servicePIDs[serviceIndex], SIGTERM);
            servicePIDs[serviceIndex] = 0;
        }
    } else {
        printf("Service %s not found\n", serviceName);
    }
} else if(strncmp(command, "status", 6) == 0) {
    for(int i = 0; i < serviceCount; i++) {
        if(servicePIDs[i] == 0) {
            printf("Service %s is not running\n", services[i]);
        } else {
            printf("Service %s is running with PID %d\n", services[i], servicePIDs[i]);
        }
    }
} else if(strncmp(command, "runlevel", 8) == 0) {
    char *levelString = command + 9;
    int newRunlevel = atoi(levelString);
    if(newRunlevel > 0 && newRunlevel <= MAX_RUNLEVELS) {
        printf("Changing runlevel to %d\n", newRunlevel);
        runlevel = newRunlevel;
        // Start or stop services based on the new runlevel
    } else {
        printf("Invalid runlevel %s\n", levelString);
    }
}
    if(strcmp(services[i], serviceName) == 0) {
        serviceIndex = i;
        break;
    }
}
if(serviceIndex != -1) {
    if(servicePIDs[serviceIndex] != 0) {
        printf("Service %s is already running\n", serviceName);
    } else {
        servicePIDs[serviceIndex] = fork();
        if(servicePIDs[serviceIndex] == 0) {
            // Child process
            printf("Starting service: %s\n", serviceName);
            execl(serviceName, serviceName, NULL);
            // execl only returns if an error occurs
            printf("Error starting service: %s\n", serviceName);
            _exit(1);
        } else if(servicePIDs[serviceIndex] < 0) {
            printf("Error starting service: %s\n", serviceName);
        }
    }
} else {
    printf("Service %s not found\n", serviceName);
}
    char *serviceName = command + 5;
    int serviceIndex = -1;
    for(int i = 0; i < serviceCount; i++) {
        if(strcmp(services[i], serviceName) == 0) {
            serviceIndex = i;
            break;
        }
    }
    if(serviceIndex != -1) {
        if(servicePIDs[serviceIndex] == 0) {
            printf("Service %s is not running\n", serviceName);
        } else {
            printf("Stopping service: %s\n", serviceName);
            kill(servicePIDs[serviceIndex], SIGTERM);
            servicePIDs[serviceIndex] = 0;
        }
    } else {
        printf("Service %s not found\n", serviceName);
    }
}
else if(strncmp(command, "status", 6) == 0) {
    printf("Services status:\n");
    for(int i = 0; i < serviceCount; i++) {
        if(servicePIDs[i] != 0) {
            printf("%s is running with PID %d\n", services[i], servicePIDs[i]);
        } else {
            printf("%s is not running\n", services[i]);
        }
    }
}
else if(strncmp(command, "runlevel", 8) == 0) {
    char *levelString = command + 9;
    int newRunlevel = atoi(levelString);
    if(newRunlevel > 0 && newRunlevel <= MAX_RUNLEVELS) {
        printf("Changing runlevel to %d\n", newRunlevel);
        runlevel = newRunlevel;
        // Start or stop services based on the new runlevel
    } else {
        printf("Invalid runlevel %s\n", levelString);
   
    }
