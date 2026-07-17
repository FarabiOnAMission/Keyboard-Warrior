#include <raylib.h>
#include<vector>
#include<string>
#include<math.h>
#include <fstream>
using namespace std;

struct Bullet {
    Rectangle body;
    float speedX, speedY;
    bool active = true;
    
    int targetId; 

    void update() {
        body.x += speedX * GetFrameTime();
        body.y += speedY * GetFrameTime();
    }

    void draw() {
        DrawRectangleRec(body, YELLOW); 
    }
};

struct enemy {    
    string s;
    int currentIndex = 0; 
    
    Rectangle body;
    Rectangle text;
    float posX , posY;
    float speedX, speedY; 
    
    int health;          
    bool active = true;  
    int id; 

    void update(){
        body.x = body.x + speedX * GetFrameTime();
        body.y = body.y + speedY * GetFrameTime();
        text.x = text.x + speedX * GetFrameTime();
        text.y = text.y + speedY * GetFrameTime();
    }
    
    void draw(){
        DrawRectangleRec(body, WHITE);
        DrawRectangleRec(text, BLACK);
        
        string typedPart = s.substr(0, currentIndex);
        string untypedPart = s.substr(currentIndex);
        
        DrawText(typedPart.c_str(), text.x + 5, text.y + 2, 20, GRAY);
        
        int typedWidth = MeasureText(typedPart.c_str(), 20);
        DrawText(untypedPart.c_str(), text.x + 5 + typedWidth, text.y + 2, 20, WHITE);
    }
};


struct BattlePlayer{

    Rectangle body = {50, 500, 50, 50};

    void draw(){
        DrawRectangleRec(body, BLUE);
    }
};

void RunBattle(){

    vector<string>Words;

    ifstream wordfile("words.txt");
    if(wordfile.is_open()){
        string line;
        while(getline(wordfile, line)){
            Words.push_back(line);
        }
    }
    wordfile.close();

    bool battleIsActive = true;

    Color Background = {20 , 160, 193, 255};
    BattlePlayer player;
    vector<enemy> v;
    vector<Bullet> bullets;
    float bulletSpeed = 600.0f;

    bool gameOver=false;

    
    float time=1.90; 
    float enemyspawner = 2;
    bool locked = false;
    int locked_index = -1;
    int level = 1;

    while(battleIsActive && !WindowShouldClose()){

        time = time + GetFrameTime();
        if(time >= enemyspawner){
            time = 0;
            enemy NewEnemy;
            int i = GetRandomValue(0,Words.size()-1);
            NewEnemy.s = Words[i];
            NewEnemy.health = Words[i].length();
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
            for (int i = 0; i < (int)bullets.size(); i++) {
                bullets[i].update();

                for (int j = 0; j < (int)v.size(); j++) {
                    if (CheckCollisionRecs(bullets[i].body, v[j].body)) {
                        bullets[i].active = false; 
                        v[j].health--; 
                        
                        if (v[j].health <= 0) {
                            v[j].active = false; 
                        }
                        break; 
                    }
                }

                if (bullets[i].body.x > 800 || bullets[i].body.x < 0 ||
                    bullets[i].body.y > 800 || bullets[i].body.y < 0) {
                    bullets[i].active = false;
                }
            }

            for (int i = (int)bullets.size() - 1; i >= 0; i--) {
                if (!bullets[i].active) {
                    bullets.erase(bullets.begin() + i);
                }
            }

            for (int i = (int)v.size() - 1; i >= 0; i--) {
                if (!v[i].active) {
                    v.erase(v.begin() + i);
                    if (locked_index > i) {
                        locked_index--;
                    } else if (locked_index == i) {
                        locked = false;
                        locked_index = -1;
                    }
                }
            }
        }
        else{
            if (IsKeyPressed(KEY_ENTER)) {
                gameOver = false;
                v.clear();
                bullets.clear();
                time = 1.9f;
            }   
        }

        int typed = GetCharPressed();
        
        while(typed > 0){
            char letter = (char)typed;
            if(!locked){
                for(int i=0; i<(int)v.size(); i++){
                    // Check the letter at currentIndex instead of index 0
                    if(v[i].currentIndex < (int)v[i].s.length() && letter == v[i].s[v[i].currentIndex]){
                        locked_index  = i;
                        locked = true;
                        
                        // Spawn bullet
                        Bullet b;
                        b.targetId = v[i].id; 
                        b.body = {player.body.x + 25, player.body.y + 25, 10, 5};
                        float diffX = v[i].body.x - player.body.x;
                        float diffY = v[i].body.y - player.body.y;
                        float dist = sqrt((diffX * diffX) + (diffY * diffY));
                        b.speedX = (diffX / dist) * bulletSpeed;
                        b.speedY = (diffY / dist) * bulletSpeed;
                        bullets.push_back(b);

                        v[i].currentIndex++; // Move to the next letter!

                        // If we typed the whole word, unlock from this enemy
                        if(v[i].currentIndex >= (int)v[i].s.length()){
                            locked = false;
                            locked_index = -1;
                        }
                        break;        
                    }
                }
            }
            else{
                if(letter == v[locked_index].s[v[locked_index].currentIndex]){
                    // Spawn bullet
                    Bullet b;
                    b.targetId = v[locked_index].id;
                    b.body = {player.body.x + 25, player.body.y + 25, 10, 5};
                    float diffX = v[locked_index].body.x - player.body.x;
                    float diffY = v[locked_index].body.y - player.body.y;
                    float dist = sqrt((diffX * diffX) + (diffY * diffY));
                    b.speedX = (diffX / dist) * bulletSpeed;
                    b.speedY = (diffY / dist) * bulletSpeed;
                    bullets.push_back(b);

                    v[locked_index].currentIndex++; // Move forward
                }
                
                // If we typed the whole word, unlock from this enemy
                if(v[locked_index].currentIndex >= (int)v[locked_index].s.length()){
                    locked = false;
                    locked_index = -1;
                }
            }
            typed = GetCharPressed();
        }

        if(IsKeyPressed(KEY_ESCAPE)){
            battleIsActive = false;
        }

        BeginDrawing();
        ClearBackground(Background);
        
        if(!gameOver){
            player.draw(); 
            
            for(int i = 0; i < (int)bullets.size(); i++){
                bullets[i].draw(); 
            }

            
            for(int i = 0; i < (int)v.size(); i++){
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
    return;
}