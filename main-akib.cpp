#include <raylib.h>
#include<vector>
#include<string>
#include<math.h>
#include <fstream>
using namespace std;


struct enemy{    
    string s;
    Rectangle body;
    Rectangle text;
    float posX , posY;
    float speedX, speedY; 
    void update(){
        body.x = body.x + speedX * GetFrameTime();
        body.y = body.y + speedY * GetFrameTime();
        text.x = text.x + speedX * GetFrameTime();
        text.y = text.y + speedY * GetFrameTime();

    }
    void draw(){
        DrawRectangleRec(body, WHITE);
        DrawRectangleRec(text, BLACK);
        DrawText(s.c_str(), text.x + 5, text.y + 2, 20, WHITE);
    }

};


struct BattlePlayer{

    Rectangle body = {50, 500, 50, 50};

    void draw(){
        DrawRectangleRec(body, BLUE);
    }
};

int main(){
    
    InitWindow(800 ,800 , "Keyboard Warrior boutta be CRAAZZYYYY");
    SetTargetFPS(60);

    vector<string>Words;

    ifstream wordfile("words.txt");
    if(wordfile.is_open()){
        string line;
        while(getline(wordfile, line)){
            Words.push_back(line);
        }
    }
    wordfile.close();


    Color Background = {20 , 160, 193, 255};
    BattlePlayer player;
    vector<enemy>v;

    bool gameOver=false;

    
    float time=1.90; 
    float enemyspawner = 2;
    bool locked = false;
    int locked_index = -1;
    int level = 1;

    while(!WindowShouldClose()){

        time = time + GetFrameTime();
        if(time >= enemyspawner){
            time = 0;
            enemy NewEnemy;
            int i = GetRandomValue(0,Words.size()-1);
            NewEnemy.s = Words[i];
            float valY = GetRandomValue(50,750);
            float textwidth = MeasureText(Words[i].c_str(), 20);
            NewEnemy.body = {800, valY, 30 , 30};
            NewEnemy.text = {800, valY-20, textwidth + 10 , 20};
            float distanceX = -750;
            float distanceY = 500 - valY;
            float distance  = sqrt((distanceX * distanceX)  + (distanceY * distanceY));
            NewEnemy.speedX = (distanceX / distance) * 100 ;
            NewEnemy.speedY = (distanceY / distance) * 100 ;
            v.push_back(NewEnemy);
        }


        if(!gameOver){
            for(int i = 0; i<(int)v.size() ; i++){
                v[i].update();
                    if(CheckCollisionRecs(v[i].body, player.body)){
                        gameOver = true;
                }
            }
        }
        else{
            if (IsKeyPressed(KEY_ENTER)) {
                gameOver = false;
                v.clear();
                time = 1.9f;
            }   
        }
        int typed = GetCharPressed();
        
        while(typed>0){

            char letter = (char)typed;
            if(!locked){
                for(int i=0;i<(int)v.size();i++){
                    if(letter == v[i].s[0]){
                        locked_index  = i;
                        locked = true;
                        v[i].s.erase(0,1);
                        if(v[i].s.empty()){
                            locked = false;
                            locked_index = -1;
                            v.erase(v.begin() + i);
                        }
                        break;        
                    }
                }
            }

            else{
                if(letter == v[locked_index].s[0] ){
                    v[locked_index].s.erase(0,1);
                }
                if(v[locked_index].s.empty()){
                    locked = false;
                    v.erase(v.begin() + locked_index);
                    locked_index = -1;
                }
            }

            typed = GetCharPressed();
        }


        BeginDrawing();
        ClearBackground(Background);
        if(!gameOver){
            player.draw();
            for(int i = 0; i<(int)v.size() ; i++){
                v[i].draw();
            }
            string leveltext = "LEVEL " + to_string(level);
            int levelWidth = MeasureText(leveltext.c_str(), 30);
            DrawText(leveltext.c_str() , (800-levelWidth)/2 , 60, 30, DARKBLUE);
        }
        else{
            int gameOverWidth = MeasureText("GAME OVER", 60);
            DrawText("GAME OVER" , (800-gameOverWidth)/2 , 300, 60, DARKBLUE);
            int restartWidth = MeasureText("Press ENTER to Restart", 20);
            DrawText("Press ENTER to Restart", (800 - restartWidth) / 2, 400, 20, DARKGRAY);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}