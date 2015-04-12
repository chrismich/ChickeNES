/*
 * main.c
 *
 *  Created on: May 28, 2009
 *      Author: blanham
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include <getopt.h>

#include "cart.h"

#include "main.h"
#include "B6502.h"
#include "pad.h"
#include "PPU.h"
#include "APU.h"
#include "mapper.h"
#include "config.h"
#include "int2bin.h"

#define SCANLINES 1850
int NMI = 0;
int CPUcycles, PPUcycles, Mcycles, line, Scycles = 0;
int FirstREAD = 1;
int multiplier = 15;
int PRG_size, CHR_size;

SDL_Surface* screen = NULL;

static struct option longopts[] = {
	{ "scanlines", required_argument, NULL, 's'},
	{ NULL, 0, NULL, 0}
};


int sdl_init()
{   
    // initialize SDL video
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO)) {
        printf( "Unable to init SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("ChickeNES", 100, 100, 512, 480, SDL_WINDOW_SHOWN);

    // input_init();
    // make sure SDL cleans up before exit
    atexit(SDL_Quit);
  
    if (!window) {
        printf("Unable to set 256x240 video: %s\n", SDL_GetError());
        return 1;
    }
    
    return 0;
}

int main(int argc, char *argv[])
{
	int ch, scanlines = 0;

	while ((ch = getopt_long(argc, argv, "s:", longopts, NULL)) != -1) {
		switch (ch) {
			case 's':
				fprintf(stderr, "Scanlines: %s", optarg);
                scanlines = atoi(optarg);
				break;
		}
	}
    char *filename = argv[optind];

    if (filename == NULL) {
        //This should print usage instead of this error
        fprintf(stderr, "No file specified\n");
        return EXIT_FAILURE;
    }

    printf("ChickeNES v. 0.0.0.1a\n");
    if (openconfig()) exit(1);

    struct nes {
        struct cpu {} cpu;
        struct ppu {} ppu;
        struct apu {} apu;
        struct cart *cart;
    };

    if (sdl_init()) {
        return EXIT_FAILURE;
    }

    struct nes *nes = calloc(sizeof(*nes), 1);

	if ((nes->cart = cart_load(filename)) == NULL) {
		fprintf(stderr, "Error loading rom!\n");
		return EXIT_FAILURE;
	}


    //init cpu
    //init ppu
    //init apu

    //get number of lines/scanlines if specified
    //(use getopt), other options

    //pass cart and options to cpu

    //reset cpu

    //enter run loop

    bool running = true;
    SDL_Event e;
    while (running) {
        SDL_PollEvent(&e);
        if (e.type == SDL_QUIT)
            running = false;
    }

    return 0;
}

void printp()
{
    printf("P:");
    if (P & 0x80)printf("N");
    else printf("n");
    if (P & 0x40)printf("V");
    else printf("v");
    if (P & 0x20)printf("U");
    else printf("u");
    if (P & 0x10)printf("B");
    else printf("b");
    if (P & 0x08)printf("D");
    else printf("d");
    if (P & 0x04)printf("I");
    else printf("i");
    if (P & 0x02)printf("Z");
    else printf("z");
    if (P & 0x01)printf("C");
    else printf("c");
}

int Dcycles;


void RunEMU(int frames)
{
    //sets the number of cycles based on Regional multiplier
    //int Dcycles = cycles * multiplier;

    int running = 1;
    initCPU();
	initAPU();
    while (running)
    {

        RunCPU(0);
        //printf("PPU!\n");
		RunPPU(341);
	//	RunPPU2(0);
        PPUcycles=0;
        if (NMI)
            nmi();
        Mcycles=0;
        //if(running == 100) exit(0);
      //  if (ppu.frames == frames) running = 0;
    }

}

//FIXME Need to figure out what the fuck is going on with cycle counting and fix it

int RunCPU(int runto)
{
    CPUcycles = 114;

    while (CPUcycles > 0)
    {

        int help = DoOP(RAM[PC]);
	//	if(PC>0x9000)printf("PCL: %x\n",PC);
        PC++;
        Mcycles = 341;
        PPUcycles += (help * multiplier);
		//printf("PPU: %i\n",PPUcycles);
        CPUcycles -= help;
	//	RunAPU(341);
		

#ifdef PRNTSTAT
      //  printp();
        //printf(" A:%.2x, X:%.2x, Y:%.2x, SP:%.2x, S2:%.2x, S1:%.2x P:%.2x\n\n", A, X, Y, STACK, RAM[0x0100+STACK+1], RAM[0x0100+STACK],P);
     //   printf(" A:%.2x, X:%.2x, Y:%.2x, S:%.2x, S2:%.2x, S?:%.2x, Cycles:%i PPUCYCLES:%i line:%i\n\n", A, X, Y, STACK, RAM[0x0100+STACK+1], RAM[0x0100+STACK], CPUcycles, ppu.cycle, ppu.scanline);
#endif



    }

    return Dcycles;
}







