#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_SERVICES 10

int main(int argc, char *argv[]) {
    printf("Starting init system...\n");
    int serviceCount = 0;
    char *services[MAX_SERVICES];
    int servicePIDs[MAX_SERVICES];

    // Read services from configuration file
    FILE *configFile = fopen("/etc/init.conf", "r");
    if(configFile != NULL) {
        char servicePath[256];
        while(fgets(servicePath, sizeof(servicePath), configFile) != NULL) {
            servicePath[strcspn(servicePath, "\r\n")] = 0;
            services[serviceCount] = strdup(servicePath);
            serviceCount++;
        }
        fclose(configFile);
    }

    // Start services
    for(int i = 0; i < serviceCount; i++) {
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
    }

    // Wait for services to finish
    int status;
    for(int i = 0; i < serviceCount; i++) {
        waitpid(servicePIDs[i], &status, 0);
        if(WIFEXITED(status)) {
            printf("Service %s exited with status %d\n", services[i], WEXITSTATUS(status));
        }
    }

    // Service management
    while(1) {
        printf("Enter command (start <service>, stop <service>, status): ");
        char command[256];
        if(fgets(command, sizeof(command), stdin) != NULL) {
            command[strcspn(command, "\r\n")] = 0;
            if(strncmp(command, "start", 5) == 0) {
                char *serviceName = command + 6;
                int serviceIndex = -1;
                for(int i = 0; i < serviceCount; i++) {
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
            waitpid(servicePIDs[serviceIndex], &status, 0);
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
            printf("Service %s is running\n", services[i]);
        }
    }
}
}
}
printf("Init system exiting...\n");
return 0;

