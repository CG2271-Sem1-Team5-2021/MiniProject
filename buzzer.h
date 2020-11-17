#define PTB0_Pin 0
#define PTB1_Pin 1

#define FREQ_2_MOD(x) (375000 / x)

#define START_SONG_LENGTH 8
#define RUNNING_SONG_LENGTH 35 //(number of Ich Liebe Dich notes)
#define END_SONG_LENGTH 33 //(number of Do Re Mi notes)

#define SEMIBREVE 2400*2
#define MINIM 1200*2
#define CROTCHET 600*2
#define CROTCHET2 600
#define QUAVER 300*2
#define SEMIQUAVER 150*2

#define Z4 262
#define Db4 277
#define D4 294
#define Eb4 311
#define E4 330
#define F4 349
#define Gb4 370
#define G4 392
#define Ab4 415
#define A4 440
#define Bb4 466
#define B4 494
#define Z5 523
#define Db5 554
#define D5 587
#define Eb5 622
#define E5 659
#define F5 699
#define G3 196

extern const unsigned long travellingNotes[][2];
extern const unsigned long DoReMi_notes[][2];
extern const unsigned long connectingSong[][2];

void initPWM(void);
void playNote(int);
void playConnectSong(void);
void playRunningSong(void);
void playEndSong(void);
