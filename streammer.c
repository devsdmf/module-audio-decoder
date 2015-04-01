#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <mikmod.h>
#include <stdlib.h>

int main() 
{
    pid_t pID = fork();

    if (pID == 0) {
        MODULE *module;
        FILE *fptr;

        MikMod_InitThreads();
        MikMod_RegisterAllDrivers();
        MikMod_RegisterAllLoaders();

        md_mode |= DMODE_SOFT_MUSIC | DMODE_NOISEREDUCTION | DMODE_INTERP;
        if (MikMod_Init("")) {
            fprintf(stderr, "Could not initialize sound, reason: %s\n",MikMod_strerror(MikMod_errno));
            return 1;
        }

        fptr = fopen("audio-embrace-1.xm", "rb");
        if (fptr == NULL) {
            perror("fopen");
            MikMod_Exit();
            return 1;
        }

        // module = Player_Load("audio-embrace-1.xm", 64, 1);
        module = Player_LoadFP(fptr, 64, 1);
        if (module) {
            module->wrap = 1;
            module->loop = 0;

            printf("Child process: %d\n",getpid());  
            Player_Start(module);
 
            while (Player_Active()) {
                usleep(10000);
                MikMod_Update();
            }
        } else
            fprintf(stderr, "Could not load module, reason: %s\n", MikMod_strerror(MikMod_errno));

        fclose(fptr);
        MikMod_Exit();
    } else if (pID < 0) {
        fprintf(stderr, "Failed to fork\n");
        exit(1);
    }

    return 0;
}