#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <mmsystem.h>
#include "console_draw2.h"
#include "show_image.h"
#include "kb_input2.h"
#include "audio.h"
#include <time.h>

#define map_width 633
#define map_height 195
#define ROWS 15
#define COLS 5
#define REFRESH_DURATION 10
#define KINDOFPLANTS 5
#define MAPSTART_X 10
#define MAPSTART_Y 45
#define MONSTERS 3

int mem[COLS][ROWS];
typedef struct object{
    int thing;
    //0 : nothing 1 : plant 2 : monster 3 : other object
    int HP;
    int tick;
    int bomb;
    int inshoot;
    //0 : nothing 1 :normal 2 :bomb
} Obj;

// x : [1,10] y : [1,5]
// monster have to move three times to go to the next matrix

typedef struct moveobject1{
    int x;
    int y;
    //this characteristic is for monster and bomb
    //0 : not monster 1 : at left 2 : in the middle 3 : at the right
    int tick;
    int show;
    int HP;
} Mon;

typedef struct moveobject2{
    int path;
    int state;
    int cd;
    int tick;
}Bul;




void delay(float sec);
void show_plant(int x, int y, int sort,int tick);
void show_monster(int x, int y, int tick);
void show_bullet(int x, int y, int bul);
void show_background(void);
void show_arrow(int x);
void clean_arrow(int x);
void refresh(int* tick);
void show_to(int x, int y);
void clean_to(int x, int y);
int menu(void);
void back_matrix(int x, int y);
void show_monster_damage(int x, int y);
void sound(int select);
void show_lose(void);
void show_start(void);
void show_graph(void);
void show_money(int a,int x);
void show_mon(void);
void show_last(void);
void show_win(void);
void show_pointer(int pointer);
void show_mpointer(int pointer);
void show_dpointer(int pointer);
void show_help();
void show_difficulty();
void show_music();

/*graph*/
//背景圖案
//商店位置 the first row
//植物圖案 OK
//怪物圖案 OK
//子彈圖案
//土地圖案
/*animation*/
//子彈發射動畫
//怪物移動動畫
/*判定*/
//子彈打到怪物
//地圖上有東西時不會洗掉
/*random*/
//怪物出現的時間
//怪物出現的地點
/**/
    int difficult_choose = 0;
/**/
void show_menu(){

    Image *temp;
    temp = read_image("zombie.pixel", "zombie.color");
    show_image(temp, 0, 0);
    temp = read_image("title2.pixel", "title2.color");
    show_image(temp, 0, 0);
    temp = read_image("start.pixel", "start.color");
    show_image(temp, 80,50 );
    temp = read_image("helptitle.pixel", "helptitle.color");
    show_image(temp, 80,80 );
    temp = read_image("music.pixel", "music.color");
    show_image(temp, 80,110 );
    temp = read_image("exit.pixel", "exit.color");
    show_image(temp, 80,140 );


}

void show_pointer(int pointer){
    Image *temp;
    if(pointer==3){
    temp = read_image("sstart.pixel", "sstart.color");
    show_image(temp, 80, 50);

    }

    if(pointer==2){
    temp = read_image("shelp.pixel", "shelp.color");
    show_image(temp, 80, 80);

    }

    if(pointer==1){
    temp = read_image("smusic.pixel", "smusic.color");
    show_image(temp, 80, 110);

    }

    if(pointer==0){
    temp = read_image("sexit.pixel", "sexit.color");
    show_image(temp, 80, 140);

    }

}

void show_mpointer(int pointer){
    Image *temp;

    if(pointer==2){
    temp = read_image("smusic1.pixel", "smusic1.color");
    show_image(temp, 80, 50);

    }

    if(pointer==1){
    temp = read_image("smusic2.pixel", "smusic2.color");
    show_image(temp, 80, 80);

    }

    if(pointer==0){
    temp = read_image("smusic3.pixel", "smusic3.color");
    show_image(temp, 80, 110);

    }

}

void show_dpointer(int pointer){
    Image *temp;

    if(pointer==1){
    temp = read_image("seasy.pixel", "seasy.color");
    show_image(temp, 80, 80);

    }

    if(pointer==0){
    temp = read_image("shard.pixel", "shard.color");
    show_image(temp, 80, 110);

    }

}

void show_help(){
//title
    Image *temp;
    temp = read_image("zombie.pixel", "zombie.color");
    show_image(temp, 0, 0);
    temp = read_image("help2.pixel", "help2.color");
    show_image(temp, 0, 0);

}

void show_difficulty(){
    //difficulty

    Image *temp;
    temp = read_image("zombie.pixel", "zombie.color");
    show_image(temp, 0, 0);
    temp = read_image("title2.pixel", "title2.color");
    show_image(temp, 0, 0);
    temp = read_image("difficulty.pixel", "difficulty.color");
    show_image(temp,80, 50);
    temp = read_image("easy.pixel", "easy.color");
    show_image(temp, 80, 80);
    temp = read_image("hard.pixel", "hard.color");
    show_image(temp, 80, 110);

}

void show_music(){
    //difficulty

    Image *temp;
    temp = read_image("zombie.pixel", "zombie.color");
    show_image(temp, 0, 0);
    temp = read_image("title2.pixel", "title2.color");
    show_image(temp, 0, 0);
    temp = read_image("music1.pixel", "music1.color");
    show_image(temp,80, 50);
    temp = read_image("music2.pixel", "music2.color");
    show_image(temp, 80, 80);
    temp = read_image("music3.pixel", "music3.color");
    show_image(temp, 80, 110);

}
/**/
void show_graph(void)
{
    int i, j, X, Y;
    for(i=0;i<COLS;i++){
        for(j=0;j<ROWS;j++){
            X = j*30+MAPSTART_X;
            Y = i*30+MAPSTART_Y;
            switch(mem[i][j]){
            case 1:
                clean_to(X, Y);
                break;
            case 2:
                show_to(X, Y);
                break;
            case 3:
                show_plant(X, Y, 0, 0);
                break;
            case 4:
                show_plant(X, Y, 1, 0);
                break;
            case 5:
                show_plant(X, Y, 1, 1);
                break;
            case 6:
                show_plant(X, Y, 2, 0);
                break;
            case 7:
                show_plant(X, Y, 2, 1);
                break;
            case 8:
                show_plant(X, Y, 2, 2);
                break;
            case 9:
                show_plant(X, Y, 3, 0);
                break;
            case 10:
                show_plant(X,Y,4,0);
                break;
            case 11:
                show_plant(X,Y,4,1);
                break;
            case 12:
                show_plant(X,Y,4,2);
                break;
            case 13:break;
            case 14:
                show_bullet(X, Y, 1);
                break;
            case 15:
                show_bullet(X, Y, 2);
                break;
            case 16:break;
            case 17:break;
            case 18:
                show_monster(X, Y, 1);
                break;
            case 19:
                show_monster_damage(X, Y);
                break;
            case 20:
                show_monster(X, Y, 2);
                break;
            case 21:
                show_monster(X, Y, 3);
                break;
            }
        }
    }
    drawCmdWindow();
}

void show_last()
{
    Image *temp;
    back_matrix(355,0);
    back_matrix(355,10);
    back_matrix(385,0);
    temp = read_image("monnum.pixel", "monnum.color");
    show_image(temp, 355, 0);
}

void show_mon()
{
    Image *temp;
    temp = read_image("money.pixel", "money.color");
    show_image(temp, 195, 0);
}
void show_money(int a,int x)
{
    Image *temp;
    switch(a){
    case 0:
        temp = read_image("0.pixel", "0.color");
        show_image(temp, x, 5);
        break;
    case 1:
        temp = read_image("1.pixel", "1.color");
        show_image(temp, x, 5);
        break;
    case 2:
        temp = read_image("2.pixel", "2.color");
        show_image(temp, x, 5);
        break;
    case 3:
        temp = read_image("3.pixel", "3.color");
        show_image(temp, x, 5);
        break;
    case 4:
        temp = read_image("4.pixel", "4.color");
        show_image(temp, x, 5);
        break;
    case 5:
        temp = read_image("5.pixel", "5.color");
        show_image(temp, x, 5);
        break;
    case 6:
        temp = read_image("6.pixel", "6.color");
        show_image(temp, x, 5);
        break;
    case 7:
        temp = read_image("7.pixel", "7.color");
        show_image(temp, x, 5);
        break;
    case 8:
        temp = read_image("8.pixel", "8.color");
        show_image(temp, x, 5);
        break;
    case 9:
        temp = read_image("9.pixel", "9.color");
        show_image(temp, x, 5);
        break;
    }
}

void sound(int select)
{

   if (select == 0) {
   /*
      用 PlaySound() 播放 wav 檔案
      假設 "recycle.wav" 聲音檔和程式碼放在相同目錄
      PlaySound("recycle.wav", NULL, SND_ASYNC|SND_FILENAME);
      會在背景開啟並播放 recycle.wav (播放時 其他鍵盤動作仍可以繼續)

      要使用 PlaySound() 這個 function
      必須要先到 [Project] -> [Project Options] 設定 (在 Dev-C++ 裡面按下  Alt+P)
      選到 Parameters 那一頁  然後在 Linker 裡面加入一行
       -lwinmm
 	  如果是 Code::Blocks 則要到   [Project]->[Build options]->[Linker settings] [other linker options]
	  加入參數 -lwinmm

      再 compile 就可以產生執行檔
   */
      PlaySound(TEXT("hit.wav"), NULL, SND_ASYNC|SND_FILENAME);
   }
   else if (select == 1) {
      PlaySound(TEXT("bomb.wav"), NULL, SND_ASYNC|SND_FILENAME);
   }
   else if (select == 2) {
      PlaySound(TEXT("bomb.wav"), NULL, SND_ASYNC|SND_FILENAME);
   }
   else if (select == 3) {
      PlaySound(TEXT("bomb.wav"), NULL, SND_ASYNC|SND_FILENAME);
   }

}

void delay(float sec)
{
    clock_t startc;
    startc = clock();
    for(;;){
        if((float)(clock()-startc)/CLOCKS_PER_SEC > sec)break;
    }
}

void show_plant(int x, int y, int sort, int tick)
{
    Image* temp;
    back_matrix(x, y);
    switch(sort){
    case 0:
            temp = read_image("sunflower.pixel", "sunflower.color");
            show_image(temp, x, y);
        break;
    case 1:
        if(tick == 1){
            //when the plant is going to shoot seeds, change the graph
            temp = read_image("f2.pixel", "f2.color");
            show_image(temp, x, y);
        }else{
            temp = read_image("f.pixel", "f.color");
            show_image(temp, x, y);
        }
        break;
    case 2:
        if(tick == 1){
            //when the plant is going to shoot seeds, change the graph
            temp = read_image("halfcutenut.pixel", "halfcutenut.color");
            show_image(temp, x, y);
        }else if(tick == 2){
            temp = read_image("halfcutenut2.pixel", "halfcutenut2.color");
            show_image(temp, x, y);
        }else{
            temp = read_image("cutenut.pixel", "cutenut.color");
            show_image(temp, x, y);
        }
        break;
    case 3:
            temp = read_image("bombplant.pixel", "bombplant.color");
            show_image(temp, x, y);
        break;
    case 4:
        if(tick == 1){
            //when the plant is going to shoot seeds, change the graph
            temp = read_image("epg.pixel", "epg.color");
            show_image(temp, x, y);
        }else if(tick == 2){
            temp = read_image("eating.pixel", "eating.color");
            show_image(temp, x, y);
        }else{
            temp = read_image("ef.pixel", "ef.color");
            show_image(temp, x, y);
        }
        break;
    }
}

void show_bullet(int x, int y, int bul)
{
    Image* temp;
    switch(bul){
        case 1:
                temp = read_image("b_left.pixel","b_left.color");
                show_image(temp, x, y);
                break;
        case 2:
                temp = read_image("bomb.pixel","bomb.color");
                show_image(temp, x, y);
                break;
    }
    //drawCmdWindow();
}

void show_monster(int x, int y, int tick)
{
    Image* temp;
    if(tick == 1){
        temp = read_image("mon.pixel","mon.color");
        show_image(temp, x, y);
    }else if(tick == 2){
        temp = read_image("deadbody.pixel","deadbody.color");
        show_image(temp, x, y);
    }else if(tick == 3){
        temp = read_image("head.pixel","head.color");
        show_image(temp, x, y);
    }
}

void show_monster_damage(int x, int y)
{
    Image* temp;
    temp = read_image("mon_damage.pixel","mon_damage.color");
    show_image(temp, x, y);
}

void show_lose(void)
{
    Image* temp;
    temp = read_image("lose.pixel","lose.color");
    show_image(temp, 0, 0);
}

void show_win(void)
{
    Image* temp;
    temp = read_image("win.pixel","win.color");
    show_image(temp, 0, 0);
}

void show_start(void)
{
    Image* temp;
    temp = read_image("start1.pixel","start1.color");
    show_image(temp, 0, 0);
    drawCmdWindow();
    delay(2);
    show_background();
    temp = read_image("GO.pixel", "GO.color");
    show_image(temp,0,0);
    drawCmdWindow();
    delay(2);
    show_background();

}

void show_background(void)
{
    int i, j;
    Image* temp;
    //show the background picture
    temp = read_image("back2.pixel", "back2.color");
    show_image(temp, 0, 0);
    //show the shop
    temp = read_image("shop.pixel", "shop.color");
    show_image(temp, 45, 0);
    back_matrix(45,0);
    /**/
    temp = read_image("sunflower.pixel", "sunflower.color");
    show_image(temp, 45, 0);
    temp = read_image("200.pixel", "200.color");
    show_image(temp, 45, 20);
    /**/
    back_matrix(75,0);
    temp = read_image("f.pixel", "f.color");
    show_image(temp, 75, 0);
    temp = read_image("300.pixel", "300.color");
    show_image(temp, 75, 20);
    /**/
    back_matrix(105,0);
    temp = read_image("cutenut.pixel", "cutenut.color");
    show_image(temp, 105, 0);
    temp = read_image("100.pixel", "100.color");
    show_image(temp, 105, 20);
    /**/
    back_matrix(135,0);
    temp = read_image("bombplant.pixel", "bombplant.color");
    show_image(temp, 135, 0);
    clean_to(165,0);
        temp = read_image("500.pixel", "500.color");
    show_image(temp, 135, 20);
    /**/
    temp = read_image("ef.pixel", "ef.color");
    show_image(temp, 165, 0);
        temp = read_image("800.pixel", "800.color");
    show_image(temp, 165, 20);
    //show the money
    show_mon();
    //show the number of monsters
    show_last();
    //show the map
    temp = read_image("land.pixel", "land.color");
    for(i=0;i<ROWS;i++){
        for(j=0;j<COLS;j++){
            show_image(temp, i*30+MAPSTART_X, j*30+MAPSTART_Y);
        }
    }
    temp = read_image("tower.pixel", "tower.color");
    for(i=0;i<5;i++){
        show_image(temp, MAPSTART_X, i*30+MAPSTART_Y);
    }
}

void show_arrow(int x)
{
    Image* temp;
    temp = read_image("arrow.pixel","arrow.color");
    show_image(temp, x, 32);
}

void clean_arrow(int x)
{
    Image* temp;
    temp = read_image("cleanarrow.pixel","cleanarrow.color");
    show_image(temp, x, 32);
    /*int i, j;
    for(i=0;i<30;i++){
        for(j=0;j<10;j++){
                putASCII2(x+i, 32+j, ' ', 0, 0);
        }
    }*/
}

void show_to(int x, int y)
{
    Image* temp;
    temp = read_image("to.pixel","to.color");
    show_image(temp, x, y);
}

void clean_to(int x, int y)
{
    Image* temp;
    temp = read_image("land.pixel","land.color");
    show_image(temp, x, y);
}

void back_matrix(int x, int y)
{
    int i, j;
    for(i=0;i<30;i++){
        for(j=0;j<30;j++){
            putASCII2(x+i,y+j, ' ', 0, 0);
        }
    }
}

int menu(void)
{
    Audio audio[7];
    openAudioFile("bgmusic1.wav",&audio[0]);
    openAudioFile("bgmusic2.wav",&audio[5]);
    openAudioFile("bgmusic3.wav",&audio[6]);
    openAudioFile("hit.wav",&audio[1]);
    openAudioFile("eat.wav",&audio[2]);
    openAudioFile("lose.wav",&audio[3]);
    openAudioFile("win.wav",&audio[4]);
    audio[0].loop=1;
    playAudio(&audio[0]);

    int menu_pointer = 3;

    int vk;

    int music_pointer=2;

    int difficult_pointer = 1;

    int choose = 0;

    int music_choose = 0;
    /*****background music*****/
    show_menu();
    show_pointer(menu_pointer);

    int end = 0;

    /***menu***/
    while(!end){
        if(choose == 0){
            initializeKeyInput();
            show_menu();
            show_pointer(menu_pointer);
            drawCmdWindow();
            vk=getKeyEventVirtual();
            switch(vk){
                case VK_DOWN:

                    if(menu_pointer > 0)

                        menu_pointer --;

                        show_pointer(menu_pointer);

                        break;

                case VK_UP:

                    if(menu_pointer < 3)

                        menu_pointer ++;

                        show_pointer(menu_pointer);

                        break;

                case VK_RETURN:

                    if(menu_pointer == 0){

                        clearScreen();

                        return 0;

                    }

                else if(menu_pointer == 1){

                    clearScreen();

                    choose = 3;

                }

                else if(menu_pointer == 2){

                    clearScreen();

                    choose = 2;

                }

                else if(menu_pointer == 3){

                    clearScreen();

                    choose = 1;

                }

                break;
            }

        }

        if(choose == 2){
            initializeKeyInput();
            show_help();
            drawCmdWindow();
            vk=getKeyEventVirtual();
            switch(vk){
                case VK_ESCAPE:

                    clearScreen();

                    choose = 0;

                    break;

            }

        }

        if(choose == 1){
            initializeKeyInput();
            show_difficulty();
            show_dpointer(difficult_pointer);
            drawCmdWindow();
            vk=getKeyEventVirtual();
            switch(vk){

                case VK_DOWN:

                    if(difficult_pointer > 0)

                        difficult_pointer --;

                        show_difficulty();

                        show_dpointer(difficult_pointer);

                        break;

                case VK_UP:

                    if(difficult_pointer < 1)

                        difficult_pointer ++;

                        show_difficulty();

                        show_dpointer(difficult_pointer);

                        break;

                case VK_RETURN:

                    if(difficult_pointer == 0){

                        clearScreen();

                        difficult_choose = 1;

                        end=1;

                        return 0;

                    }

                else if(difficult_pointer == 1){

                    clearScreen();

                    difficult_choose = 0;

                    end=1;

                    return 0;

                    }


                    break;

                case VK_ESCAPE:

                    clearScreen();

                    choose = 0;

                    break;

            }

        }

        if(choose == 3){
            initializeKeyInput();
            show_music();
            show_mpointer(music_pointer);
            drawCmdWindow();
            vk=getKeyEventVirtual();
            switch(vk){

                case VK_DOWN:

                    if(music_pointer > 0)

                        music_pointer --;

                        show_music();

                        show_mpointer(music_pointer);

                        break;

                case VK_UP:

                    if(music_pointer < 2)

                        music_pointer ++;

                        show_music();

                        show_mpointer(music_pointer);

                        break;

                case VK_RETURN:

                    if(music_pointer == 0){
                        pauseAudio(&audio[music_choose]);
                        audio[music_choose].loop=0;
                        music_choose = music_pointer;
                        audio[music_choose].loop=1;
                        playAudio(&audio[music_choose]);
                        restartAudio(&audio[music_choose]);
                    }

                    else if(music_pointer == 1){
                        pauseAudio(&audio[music_choose]);
                        audio[music_choose].loop=0;
                        music_choose = music_pointer+4;
                        audio[music_choose].loop=1;
                        playAudio(&audio[music_choose]);
                        restartAudio(&audio[music_choose]);

                    }else if(music_pointer == 2){
                        pauseAudio(&audio[music_choose]);
                        audio[music_choose].loop=0;
                        music_choose = music_pointer+4;
                        audio[music_choose].loop=1;
                        playAudio(&audio[music_choose]);
                        restartAudio(&audio[music_choose]);
                    }


                    break;

                case VK_ESCAPE:

                    clearScreen();

                    choose = 0;

                    break;

            }

        }

    }

}
int main(void)
{
    Audio audio[7];
    openAudioFile("bgmusic1.wav",&audio[0]);
    openAudioFile("bgmusic2.wav",&audio[5]);
    openAudioFile("bgmusic3.wav",&audio[6]);
    openAudioFile("hit.wav",&audio[1]);
    openAudioFile("eat.wav",&audio[2]);
    openAudioFile("lose.wav",&audio[3]);
    openAudioFile("win.wav",&audio[4]);
        menu();
    //music
    /*definition*/
    clock_t startc;
    startc = clock();
    show_background();
    /*local variables in main*/
    int i, j, k, m, n, l;
    int x,y;
    Obj map[COLS][ROWS];
    int isEnding = 0, shopping = 0, planting = 0, fighting = 0;
    int vk;
    int arrow = 45;
    int seed[KINDOFPLANTS];
    int sun_num = 0, money = 1200, number, pow;
    int last = MONSTERS;
    x = MAPSTART_X;
    y = MAPSTART_Y;
    fighting = 1;
    int moneytick;
    clock_t clockc;
    clock_t planttick[COLS][ROWS] = {0};
    clock_t monclock[MONSTERS] = {0};
    float speed = 0.05;
    /*difficulty*/
        if(difficult_choose == 1){
          money = 500;
          speed = 0.01;
        }
    /*memory the map*/
    //1 land
    //2 arrow
    //3 sun
    //4 f
    //5 f1
    //6 nut1
    //7 nut2
    //8 nut3
    //9 bomb plant
    //10 eat f1
    //11 eat f2
    //12 eat f3
    //13 frozen p
    //14 bul
    //15 bomb
    //16 whirl bul
    //17 ice bul
    //18 mon
    //19 hurt mon
    /*initialization of bullet*/
    Bul bul[COLS][ROWS];
    for(i=0;i<COLS;i++){
        for(j=0;j<ROWS;j++){
            bul[i][j].path = 0;
            bul[i][j].state = 0;
            bul[i][j].tick = 0;
            bul[i][j].cd = 0;
        }
    }
    /*initialization of monster*/
    Mon mon[MONSTERS];
    srand(time(NULL));
    for(i=0;i<MONSTERS;i++){
        mon[i].x = ROWS;
        mon[i].y = rand() % 5 ;
        mon[i].tick = 0;
        mon[i].show = (rand() % 30)*5000 ;
        mon[i].HP = 10;
        for(j=0;j<i;j++){
            if(mon[j].y == mon[i].y && mon[j].show== mon[i].show){
                i--;
                break;
            }
        }
    }
    /*initialization of map*/
    for(i=0;i<COLS;i++){
        for(j=0;j<ROWS;j++){
            map[i][j].bomb = 0;
            map[i][j].HP = 0;
            map[i][j].thing = 0;
            map[i][j].tick = 0;
            map[i][j].inshoot = 0;
        }
    }

    show_start();
    while(!isEnding)
    {
        //control refresh time
        show_graph();
        //shop
        if(shopping){
            show_arrow(arrow);
            if(waitForKeyDown(0.01)){
                vk = getKeyEventVirtual();
                //the arrow's range is from 45~105
                switch(vk) {
                    case VK_LEFT:
                        clean_arrow(arrow);
                        if(arrow >45)
                        arrow -= 30;
                        break;
                    case VK_RIGHT:
                        clean_arrow(arrow);
                        if(arrow <165)
                        arrow += 30;
                        break;
                    case VK_RETURN:
                    //choose the seed
                    //don't break because it should go back to plant
                        for(i=0;i<KINDOFPLANTS;i++){
                            seed[i] = 0;
                            if(arrow == 45 + i * 30)
                                seed[i] = 1;
                        }
                    case VK_CONTROL:
                    //go back to plant
                        clean_arrow(arrow);
                        x = MAPSTART_X+30;
                        y = MAPSTART_Y;
                    shopping = 0;
                    planting = 1;
                    case VK_SHIFT:
                    clean_arrow(arrow);
                    shopping = 0;
                    fighting = 1;
                    break;
                }
            }
        }
        // planing
        else if(planting){
            if(waitForKeyDown(0.01)) {
            vk=getKeyEventVirtual();
            if(map[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30].thing == 0)
            mem[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30] = 2;
            show_graph();
            drawCmdWindow();
            switch(vk) {
                case VK_ESCAPE:
                    isEnding = 1;
                    break;
                case VK_LEFT:
                    if(map[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30].thing == 0)
                    mem[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30] = 1;
                    if(x > MAPSTART_X+30)
                    x -= 30;
                    break;
                case VK_RIGHT:
                    if(map[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30].thing == 0)
                    mem[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30] = 1;
                    if(x < MAPSTART_X + 30 * (ROWS-1))
                    x += 30;
                    break;
                case VK_DOWN:
                    if(map[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30].thing == 0)
                    mem[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30] = 1;
                    if(y < MAPSTART_Y + 30 * (COLS-1))
                    y += 30;
                    break;
                case VK_UP:
                    if(map[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30].thing == 0)
                    mem[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30] = 1;
                    if(y > MAPSTART_Y)
                    y -= 30;
                    break;
                case VK_RETURN://press enter can plant
                    for(i=0;i<KINDOFPLANTS;i++){
                        if(seed[i] == 1 && map[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30].thing == 0){
                            switch(i){
                            case 0:
                                if(money >= 200){
                                    money -= 200;
                                    mem[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30] = 3;
                                    map[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30].thing = 1;
                                    map[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30].HP = 5;
                                    sun_num++;
                                }
                                break;
                            case 1:
                                if(money >= 300){
                                    money -= 300;
                                    mem[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30] = 4;
                                    map[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30].thing = 1;
                                    map[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30].bomb = 1;
                                    map[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30].HP = 20;
                                    map[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30].inshoot = 0;
                                }
                                break;
                            case 2:
                                if(money >= 100){
                                    money -= 100;
                                    mem[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30] = 6;
                                    map[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30].thing = 1;
                                    map[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30].HP = 20;
                                }
                                break;
                            case 3:
                                if(money >=500){
                                    money -= 500;
                                    mem[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30] = 9;
                                    map[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30].thing = 1;
                                    map[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30].HP = 20;
                                    map[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30].bomb = 2;
                                    map[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30].inshoot = 0;
                                }
                                break;
                            case 4:
                                if(money >= 800){
                                    money -= 800;
                                    mem[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30] = 10;
                                    map[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30].thing = 1;
                                    map[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30].HP = 20;
                                }
                                break;

                            }
                            show_mon();
                            for(i=0;i<4;i++){
                                    pow = 1;
                                for(j=0;j<i;j++){
                                    pow = pow * 10;
                                }
                                number = money / pow;
                                number = number % 10;
                                show_money(number, 315-i*30);
                            }
                        }
                    }
                    break;
                case VK_CONTROL:
                    if(map[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30].thing == 0)
                    mem[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30] = 1;
                    arrow = 45;
                    shopping = 1;
                    planting = 0;
                    break;
                case VK_SHIFT:
                    if(map[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30].thing == 0)
                    mem[(y-MAPSTART_Y)/30][(x-MAPSTART_X)/30] = 1;
                    planting = 0;
                    fighting = 1;
                    break;
                }
            }
        }
        //fighting
        else if(fighting){
            for(i=0;i<MONSTERS;i++){
                if(mon[i].show <= clock() && mon[i].HP > 0){
                    if(mon[i].tick < 2){
                        if((clock()-monclock[i])/CLOCKS_PER_SEC > speed){
                            if(map[mon[i].y][mon[i].x-1].thing < 2)
                            mon[i].tick++;
                            monclock[i] = clock();
                        }
                    }
                    if(mon[i].tick >= 2 && mon[i].x > 0 && map[mon[i].y][mon[i].x-1].thing == 0){
                        if(mon[i].x != ROWS)
                        mem[mon[i].y][mon[i].x] = 1;
                        map[mon[i].y][mon[i].x].thing = 0;
                        mon[i].x -= 1;
                        map[mon[i].y][mon[i].x].thing = 2;
                        if(mon[i].HP <= 5) mem[mon[i].y][mon[i].x] = 20;
                        else  mem[mon[i].y][mon[i].x] = 18;
                        mon[i].tick = 0;
                    }else if(mon[i].tick >= 2 && map[mon[i].y][mon[i].x-1].thing == 1){
                        if(mem[mon[i].y][mon[i].x-1] == 10){
                            mem[mon[i].y][mon[i].x-1] = 11;
                            mem[mon[i].y][mon[i].x] = 1;
                            mon[i].HP = 0;
                            money += 200;
                            last--;
                            map[mon[i].y][mon[i].x].thing = 0;
                        }
                        map[mon[i].y][mon[i].x-1].HP--;
                        playAudio(&audio[2]);
                        mon[i].tick = 0;
                        if(map[mon[i].y][mon[i].x-1].HP == 0 ){
                            if(mem[mon[i].y][mon[i].x-1] == 3) sun_num--;
                            mem[mon[i].y][mon[i].x-1] = 1;
                            map[mon[i].y][mon[i].x-1].thing = 0;
                        }/*else if(map[mon[i].y][mon[i].x-1].HP <= 30 ){
                            if(map[mon[i].y][mon[i].x-1].HP <= 10)
                            mem[mon[i].y][mon[i].x-1] = 8;
                            else
                            mem[mon[i].y][mon[i].x-1] = 7;
                        }*/
                    }/*else if(map[mon[i].y][mon[i].x-1].thing >1 ){
                    }*/else if(mon[i].x == 0){
                      //  clearScreen();
                        clearScreen();
                        show_lose();
                        pauseAudio(&audio[0]);
                        playAudio(&audio[3]);
                        drawCmdWindow();
                        delay(3);
                        isEnding = 1;
                        break;
                    }
                }
            }
            if(isEnding) break;
            for(i=0;i<COLS;i++){
                for(j=0;j<ROWS;j++){
                    //1->determine the shooting plant
                    //2a->determine the kind
                    //2b->determine the bomb area and damage
                    if(mem[i][j] == 11 || mem[i][j] == 12){
                        if(map[i][j].inshoot == 1)
                        mem[i][j] = 12;
                        map[i][j].tick++;
                        map[i][j].inshoot = 1;
                        if(map[i][j].tick == 100){
                            mem[i][j] = 10;
                            map[i][j].tick = 0;
                            map[i][j].inshoot = 0;
                        }
                    }

                    if(map[i][j].bomb > 0 && map[i][j].HP > 0){
                            switch(map[i][j].bomb){
                            case 1:
                                if(map[i][j].tick >= 2)
                                    mem[i][j] = 5;
                                if(map[i][j].inshoot == 1){
                                   mem[i][j] = 4;
                                   if(bul[i][j].path >= ROWS-1){
                                        if(bul[i][j].path != j && map[i][bul[i][j].path].thing == 0)
                                        mem[i][bul[i][j].path] = 1;
                                        map[i][j].inshoot = 0;
                                   }else{
                                        if(bul[i][j].path != j  && map[i][bul[i][j].path].thing == 0)
                                        mem[i][bul[i][j].path] = 1;
                                        bul[i][j].path += 1;
                                        if(map[i][bul[i][j].path].thing == 0 && map[i][bul[i][j].path-1].thing != 2)
                                            mem[i][bul[i][j].path] = 14;
                                   }
                                }
                                break;
                            case 2:
                                mem[i][j] = 9;
                                if(map[i][j].inshoot == 1){
                                   //show_plant(0,j*30+MAPSTART_X, i*30+MAPSTART_Y, 3);
                                   if(bul[i][j].path >= ROWS-1){
                                        if(bul[i][j].path != j && map[i][bul[i][j].path].thing == 0)
                                        mem[i][bul[i][j].path] = 1;
                                        map[i][j].inshoot = 0;
                                   }else{
                                        if(bul[i][j].path != j && map[i][bul[i][j].path].thing == 0 ) //no sure if this command valid
                                        mem[i][bul[i][j].path] = 1;
                                        bul[i][j].path += 1;
                                        if(map[i][bul[i][j].path].thing == 0 && map[i][bul[i][j].path-1].thing != 2)
                                            mem[i][bul[i][j].path] = 15;
                                   }
                                }
                                break;
                            }

                            if(map[i][j].inshoot != 1){
                                if(map[i][j].tick >= 2){
                                    bul[i][j].path = j;
                                    map[i][j].inshoot = 1;
                                    map[i][j].tick = 0;
                                }else{
                                    if((clock()-planttick[i][j])/CLOCKS_PER_SEC > 0.2){
                                        map[i][j].tick++;
                                        planttick[i][j] = clock();
                                    }
                                }
                            }else{
                                for(k=0;k<MONSTERS;k++){
                                    if(mon[k].y == i && mon[k].HP > 0){
                                        //touch the monster
                                        if(bul[i][j].path >= mon[k].x){
                                            //each type of bullet has different effect
                                            switch(map[i][j].bomb){
                                            case 1:
                                                playAudio(&audio[1]);
                                                mon[k].HP--;
                                                //drawCmdWindow();
                                                //show_monster(mon[k].x, mon[k].y);
                                                if(mon[k].HP == 0){
                                                        money += 200;
                                                        last--;
                                                    mem[mon[k].y][mon[k].x] = 1;
                                                    map[mon[k].y][mon[k].x].thing = 0;
                                                }else if(mon[k].HP == 5){
                                                    mem[mon[k].y][mon[k].x] = 20;
                                                    if(map[mon[k].y][mon[k].x+1].thing == 0)
                                                    mem[mon[k].y][mon[k].x+1] = 21;
                                                }
                                                break;
                                            case 2:
                                                sound(1);
                                                for(m=mon[k].x-1;m<=mon[k].x+1;m++){
                                                    for(n=mon[k].y-1;n<=mon[k].y+1;n++){
                                                        for(l=0;l<MONSTERS;l++){
                                                            if(mon[l].x == m && mon[l].y == n && mon[l].x < ROWS && mon[l].HP > 0){
/*music*/                                                       mon[l].HP--;
                                                                if(mon[l].HP == 0){
                                                                    last--;
                                                                    money += 200;
                                                                    mem[mon[l].y][mon[l].x] = 1;
                                                                    map[mon[l].y][mon[l].x].thing = 0;
                                                                }else if(mon[l].HP == 5){
                                                                    mem[mon[l].y][mon[l].x] = 20;
                                                                    if(map[mon[l].y][mon[l].x+1].thing == 0)
                                                                    mem[mon[l].y][mon[l].x+1] = 21;
                                                                }
                                                            }
                                                        }
                                                    }
                                                //drawCmdWindow();
                                                /*for(m=mon[k].x-30;m<=mon[k].x+30;m += 30){
                                                    for(n=mon[k].y-30;n<=mon[k].y+30;n += 30){
                                                        for(l=0;l<MONSTERS;l++){
                                                            if(mon[l].x == m && mon[l].y == n && mon[l].x < MAPSTART_X + 30*ROWS){
                                                                //show_monster(mon[l].x, mon[l].y);
                                                            }
                                                            }
                                                        }
                                                    }*/
                                                }
                                                break;

                                            }

                                            map[i][j].inshoot = 0;
                                        }
                                        //death determination
                                    }
                                }
                            }
                    }
                }
            }
            //bullet fly
            //monster walk
            if(waitForKeyDown(0.01)) {
                vk=getKeyEventVirtual();
                if(vk == VK_SHIFT){
                    x = MAPSTART_X+30;
                    y = MAPSTART_Y;
                    shopping = 1;
                    fighting = 0;
                }
            }
            show_mon();
            if(moneytick >= 1){
                money = money + sun_num * 5;
                moneytick = 0;
            }else{
                if((clock()-clockc)/CLOCKS_PER_SEC > 0.03){
                    moneytick++;
                    clockc = clock();
                }
            }
            for(i=0;i<4;i++){
                    pow = 1;
                for(j=0;j<i;j++){
                    pow = pow * 10;
                }
                number = money / pow;
                number = number % 10;
                show_money(number, 315-i*30);
            }

            show_last();
            for(i=0;i<2;i++){
                    pow = 1;
                for(j=0;j<i;j++){
                    pow = pow * 10;
                }
                number = last / pow;
                number = number % 10;
                show_money(number, 418-i*30);
            }
            if(last == 0){
                clearScreen();
                show_win();
                pauseAudio(&audio[0]);
                playAudio(&audio[4]);
                drawCmdWindow();
                delay(3);
                isEnding = 1;
            }
        }
    }
    return 0;
}











