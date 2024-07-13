#ifndef A5625_H
#define A5625_H
#ifdef __cplusplus
extern "C" {
#endif

struct snd_ch {
	int out;
	int final_out;
	unsigned char volume;
	int count;
	int compare;
	int lfsr;
	int noise; // 0 for off, 1 for on
};

struct snd_chip {
	struct snd_ch channel[2];
	int master_out;
};

void snd_init(struct snd_chip *chip);
void snd_lfsr(struct snd_chip *chip);
void snd_clock(struct snd_chip *chip);
#ifdef __cplusplus
}
#endif
#endif
