#define LINELEN 128
#define GAME_NUM 5
#define letter_num_day 7
#define MIN_WORD_LEN 4
#define MAX_WORD_LEN 8
typedef struct 
{
    char date[LINELEN];
    char letter_group[LINELEN];
}single_day;
int count;//这个要全局
char letter_usedd[21]={'\0'};//后期改
single_day* all_day;
char core_letter[3]={'\0'};



