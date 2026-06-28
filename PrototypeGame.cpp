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
    
    int lev = 3;

    Texture2D* tex;
    int frame = 0;
    float frameTimer = 0;
    float frameSpeed = 0.1f;
    int totalFrames = 16;
    int cols = 4;           
    int frameW = 200;       
    int frameH = 200; 


    void update(){
        body.x = body.x + speedX * GetFrameTime();
        body.y = body.y + speedY * GetFrameTime();
        text.x = text.x + speedX * GetFrameTime();
        text.y = text.y + speedY * GetFrameTime();


        frameTimer += GetFrameTime();
        if (frameTimer >= frameSpeed) {
            frameTimer = 0;
            frame = (frame + 1) % totalFrames;
        }

    }
    void draw(Color C){

        int col = frame % cols;
        int row = frame / cols;

        Rectangle source = {
            (float)(col * frameW),
            (float)(row * frameH),
            (float)frameW,
            (float)frameH
        };

        DrawTexturePro(*tex, source, body, {0, 0}, 0.0f, WHITE);
    
        // DrawRectangleRec(body, WHITE);
        DrawRectangleRec(text, BLACK);
        DrawText(s.c_str(), text.x + 5, text.y + 2, 20, C);
    }

};


struct Player{

    Rectangle body = {50, 500, 50, 50};

    void draw(){
        DrawRectangleRec(body, BLUE);
    }
};

int main(){

    int level = 1;

    float time=1.90; 
    bool locked = false;
    int locked_index = -1;

    InitWindow(800 ,800 , "Keyboard Warrior boutta be CRAAZZYYYY");
    SetTargetFPS(60);
    
    
    float enemyspawner;
    float enemyBodySize;
    int enemyValue;
    Texture2D enemyTex;
    Texture2D bgTex;
    Color C;

    if(level==1){
        enemyTex = enemyTex = LoadTexture("resource/GreenDemonFinal.png");
        bgTex = LoadTexture("resource/GreenBGFinal.png");
        enemyspawner = 1.5;
        enemyBodySize = 80;
        enemyValue = 2;
        C = GREEN;
    } 
    else if(level==2){
        enemyTex = enemyTex = LoadTexture("resource/BlueDemonFinal.png");
        bgTex = LoadTexture("resource/BlueBGFinal.png");
        enemyspawner = 1.3;
        enemyBodySize = 80;
        enemyValue = 3;
        C = BLUE;
    }
    else if(level==3){
        enemyTex = enemyTex = LoadTexture("resource/RedDemonFinal.png");
        bgTex = LoadTexture("resource/RedBGFinal.png");
        enemyspawner = 1.1;
        enemyBodySize = 120;
        enemyValue = 5;
        C = RED;
    }
    
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
    Player player;
    vector<enemy>v;

    bool gameOver=false;

    int score = 0;

    while(!WindowShouldClose()){

        time = time + GetFrameTime();
        if(time >= enemyspawner){
            time = 0;
            enemy NewEnemy;
            NewEnemy.tex = &enemyTex;

            if(level==1){
                NewEnemy.frame = 0;
                NewEnemy.frameTimer = 0;
                NewEnemy.frameSpeed = 0.1f;
                NewEnemy.totalFrames = 16;
                NewEnemy.cols = 4;           
                NewEnemy.frameW = 200;       
                NewEnemy.frameH = 200;             
            }
            else if(level==3){
                NewEnemy.frame = 0;
                NewEnemy.frameTimer = 0;
                NewEnemy.frameSpeed = 0.1f;
                NewEnemy.totalFrames = 4;
                NewEnemy.cols = 2;           
                NewEnemy.frameW = 100;       
                NewEnemy.frameH = 100;                
            }

            int i = GetRandomValue(0,Words.size()-1);
            NewEnemy.s = Words[i];
            float valY = GetRandomValue(50,750);
            float textwidth = MeasureText(Words[i].c_str(), 20);
            NewEnemy.body = {800, valY, enemyBodySize, enemyBodySize};
            NewEnemy.text = {800, valY-20, textwidth+10 , 20};
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
                score = 0;
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
                            score += enemyValue;
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
                    score += enemyValue;
                    locked_index = -1;
                }
            }

            typed = GetCharPressed();
        }


        BeginDrawing();
        ClearBackground(Background);
        if(!gameOver){

            DrawTexturePro(bgTex, {0,0,(float)bgTex.width,(float)bgTex.height}, {0,0,800,800}, {0,0}, 0.0f, WHITE);

            player.draw();
            for(int i = 0; i<(int)v.size() ; i++){
                v[i].draw(C);
            }
            string leveltext = "LEVEL " + to_string(level);
            int levelWidth = MeasureText(leveltext.c_str(), 30);
            DrawText(leveltext.c_str() , (800-levelWidth)/2 , 60, 30, DARKBLUE);
        }
        else{
            int gameOverWidth = MeasureText("GAME OVER", 60);
            DrawText("GAME OVER" , (800-gameOverWidth)/2 , 300, 60, DARKBLUE);

            string scoretext = "Score: " + to_string(score);
            int scoreWidth = MeasureText(scoretext.c_str(), 30);
            DrawText(scoretext.c_str(), (800-scoreWidth)/2, 380, 30, DARKBLUE);

            int restartWidth = MeasureText("Press ENTER to Restart", 20);
            DrawText("Press ENTER to Restart", (800 - restartWidth) / 2, 430, 20, DARKBLUE);
        }
        EndDrawing();
    }
    UnloadTexture(enemyTex);
    UnloadTexture(bgTex);


    CloseWindow();
    return 0;
}


// g++ PrototypeGame.cpp -o PrototypeGame -lraylib -lm -lpthread -ldl && ./PrototypeGame