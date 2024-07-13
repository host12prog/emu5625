#include "a5625.h"

const unsigned char volume_table[16] = {
	0, 2, 3, 5, 7, 11, 16, 27,
	32, 52, 74, 95, 126, 162, 206, 255
};

void snd_init(struct snd_chip *chip) {
	for (int i=0;i<2;i++) {
		chip->channel[i].out = chip->channel[i].final_out = chip->channel[i].volume = chip->channel[i].count = chip->channel[i].compare = chip->channel[i].noise = 0;
		chip->channel[i].lfsr = 0xae01;
	}
	chip->master_out = chip->master_vol = 0;
}

void snd_lfsr(struct snd_chip *chip) {
	for (int i=0;i<2;i++) {
		chip->channel[i].lfsr ^= chip->channel[i].lfsr >> 7;
		chip->channel[i].lfsr ^= chip->channel[i].lfsr << 9;
		chip->channel[i].lfsr ^= chip->channel[i].lfsr >> 13;
	}
}

void snd_clock(struct snd_chip *chip) {
	for (int i=0;i<2;i++) {
	/* generator */
	chip->channel[i].count++;
	if (!(chip->channel[i].noise)) {
		if (chip->channel[i].count >= chip->channel[i].compare) {
			chip->channel[i].count = 0;
			chip->channel[i].out ^= 0x01;
		}
		chip->channel[i].out &= 0x1; // ensure we remain a 1-bit output
	} else {
		if (chip->channel[i].count >= chip->channel[i].compare) {
			chip->channel[i].count = 0;
			snd_lfsr(chip);
		}
		chip->channel[i].out = (chip->channel[i].lfsr & 0x1);
	}
	/* output stage */
	chip->channel[i].final_out = (chip->channel[i].out * (volume_table[chip->channel[i].volume]/2));
	chip->master_out += chip->channel[i].final_out;
	}
	chip->master_out = chip->master_out >> 2;
}
