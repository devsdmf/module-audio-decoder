#include <stdio.h>
#include <unistd.h>
#include <mikmod.h>

int main() 
{
	MODULE *module;

    MikMod_InitThreads();
	
	MikMod_RegisterAllDrivers();
	MikMod_RegisterAllLoaders();

	md_mode |= DMODE_SOFT_MUSIC | DMODE_NOISEREDUCTION | DMODE_INTERP;
	if (MikMod_Init("")) {
        fprintf(stderr, "Could not initialize sound, reason: %s\n",MikMod_strerror(MikMod_errno));
        return 1;
    }

    module = Player_Load("res/audio-embrace-1.xm", 128, 1);
    if (module) {
        module->wrap = 1;
        module->loop = 0;

        printf("Playing %s\n", module->songname);
        printf("Channels: %d\n",module->numchn);
        printf("Voices: %d\n",module->numvoices);
        printf("Patterns: %d\n",module->numpat);
        printf("Samples: %d\n",module->numsmp);
        printf("Time: %d\n",module->sngtime);
        printf("Wrap: %d\n",module->wrap);
        printf("Loop: %d\n",module->loop);
        printf("\n");

        for (int i=0;i<=module->numsmp;i++) {
            SAMPLE sample;
            sample = module->samples[i];
            printf("----> SAMPLE\n");
            printf("----> Length: %d\n",sample.length);
            printf("----> Loop start: %d\n",sample.loopstart);
            printf("----> Loop end: %d\n",sample.loopend);
            printf("\n");
        }

        Player_Start(module);
 
        while (Player_Active()) {
            usleep(100000);
            MikMod_Update();
        }

        Player_Stop();
        Player_Free(module);
    } else
        fprintf(stderr, "Could not load module, reason: %s\n", MikMod_strerror(MikMod_errno));

    MikMod_Exit();

    return 0;
}