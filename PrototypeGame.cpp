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
    Texture2D *tex;

    void update() {
        body.x += speedX * GetFrameTime();
        body.y += speedY * GetFrameTime();
    }

    void draw() {
        float angle = atan2f(speedY, speedX) * (180.0f/3.1415926);
        DrawTexturePro(*tex,{0, 0, (float)tex->width, (float)tex->height},body, {body.width / 2, body.height / 2},angle,WHITE);
    }
};


struct enemy {    
    string s;
    int currentIndex = 0;  

    Rectangle body;
    Rectangle text;
    float posX, posY;
    float speedX, speedY;

    int lev = 3;
    int id;
    int health;
    bool active = true;

    Texture2D* tex;
    int frame = 0;
    float frameTimer = 0;
    float frameSpeed = 0.1f;
    int totalFrames = 16;
    int cols = 4;
    int frameW = 200;
    int frameH = 200;

    void update(){
        body.x += speedX * GetFrameTime();
        body.y += speedY * GetFrameTime();
        text.x += speedX * GetFrameTime();
        text.y += speedY * GetFrameTime();

        frameTimer += GetFrameTime();
        if(frameTimer >= frameSpeed){
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
        DrawRectangleRec(text, BLACK);
        string typedPart   = s.substr(0, currentIndex);
        string untypedPart = s.substr(currentIndex);
        DrawText(typedPart.c_str(), text.x + 5, text.y + 2, 20, GRAY);
        int typedWidth = MeasureText(typedPart.c_str(), 20);
        DrawText(untypedPart.c_str(), text.x + 5 + typedWidth, text.y + 2, 20, C);
    }
};


struct BattlePlayer {
    Rectangle body = {50, 500, 50, 50};

    void draw(){
        DrawRectangleRec(body, BLUE);
    }
};

int HighScoreTrack(){
    int highscore = 0;
    std::ifstream HighScores("highscore.save");
    if(HighScores.is_open()){
        HighScores>>highscore;
        HighScores.close();
    }
    return highscore;
}
void SaveHighScore(int newScore){
    std::ofstream HighScore("highscore.save");
    if (HighScore.is_open()){
        HighScore<<newScore;
        HighScore.close();
    }
}

void RunBattle(int level){
    float time = 1.90f;
    bool locked = false;
    int locked_index = -1;

    SetWindowSize(800, 800); 
    SetWindowTitle("Keyboard Warrior boutta be CRAAZZYYYY");
    SetTargetFPS(60);

    float enemyspawner;
    float enemyBodySize;
    int enemyValue;
    Texture2D enemyTex;
    Texture2D bgTex;
    Texture2D arrowTex;
    Color C;

    if(level == 1){
        enemyTex = LoadTexture("resource/GreenDemonFinal.png");
        bgTex    = LoadTexture("resource/GreenBGFinal.png");
        arrowTex = LoadTexture("resource/YellowArrow.png");
        enemyspawner  = 1.5f;
        enemyBodySize = 80;
        enemyValue    = 2;
        C = GREEN;
    }
    else if(level == 2){
        enemyTex = LoadTexture("resource/BlueDemonFinal.png");
        bgTex    = LoadTexture("resource/BlueBGFinal.png");
        arrowTex = LoadTexture("resource/BlueArrow.png");
        enemyspawner  = 1.3f;
        enemyBodySize = 80;
        enemyValue    = 3;
        C = BLUE;
    }
    else if(level == 3){
        enemyTex = LoadTexture("resource/RedDemonFinal.png");
        bgTex    = LoadTexture("resource/RedBGFinal.png");
        arrowTex = LoadTexture("resource/RedArrow.png");
        enemyspawner  = 1.1f;
        enemyBodySize = 120;
        enemyValue    = 5;
        C = RED;
    }

    vector<string> Words;
    ifstream wordfile("words.txt");
    if(wordfile.is_open()){
        string line;
        while(getline(wordfile, line)){
            Words.push_back(line);
        }
    }
    wordfile.close();

    Color Background = {20, 160, 193, 255};
    BattlePlayer player;
    vector<enemy> v;
    vector<Bullet> bullets;
    float bulletSpeed = 600.0f;

    bool gameOver = false;
    int score = 0;
    int enemyIdCounter = 0;

    while(!WindowShouldClose()){

        time += GetFrameTime();
        if(time >= enemyspawner){
            time = 0;
            enemy NewEnemy;
            NewEnemy.tex = &enemyTex;
            NewEnemy.id  = enemyIdCounter++;

            if(level == 1){
                NewEnemy.frameSpeed   = 0.1f;
                NewEnemy.totalFrames  = 16;
                NewEnemy.cols         = 4;
                NewEnemy.frameW       = 200;
                NewEnemy.frameH       = 200;
            }
            else if(level == 2){
                NewEnemy.frameSpeed   = 0.1f;
                NewEnemy.totalFrames  = 16;
                NewEnemy.cols         = 4;
                NewEnemy.frameW       = 200;
                NewEnemy.frameH       = 200;
            }
            else if(level == 3){
                NewEnemy.frameSpeed   = 0.1f;
                NewEnemy.totalFrames  = 4;
                NewEnemy.cols         = 2;
                NewEnemy.frameW       = 100;
                NewEnemy.frameH       = 100;
            }

            int i = GetRandomValue(0, Words.size()-1);
            NewEnemy.s      = Words[i];
            NewEnemy.health = Words[i].length();
            float valY      = GetRandomValue(50, 750);
            float textwidth = MeasureText(Words[i].c_str(), 20);
            NewEnemy.body   = {800, valY, enemyBodySize, enemyBodySize};
            NewEnemy.text   = {800, valY - 20, textwidth + 10, 20};
            float distanceX = -750;
            float distanceY = 500 - valY;
            float distance  = sqrt((distanceX * distanceX) + (distanceY * distanceY));
            NewEnemy.speedX = (distanceX / distance) * 100;
            NewEnemy.speedY = (distanceY / distance) * 100;
            v.push_back(NewEnemy);
        }

        if(!gameOver){

            for(int i = 0; i < (int)v.size(); i++){
                v[i].update();
                if(CheckCollisionRecs(v[i].body, player.body)){
                    gameOver = true;
                    int currentHighScore = HighScoreTrack();
                    if (score > currentHighScore) {
                        currentHighScore = score;
                        SaveHighScore(currentHighScore);
                    }
                    break;
                }
            }

            for(int i = 0; i < (int)bullets.size(); i++){
                bullets[i].update();

                for(int j = 0; j < (int)v.size(); j++){
                    if(CheckCollisionRecs(bullets[i].body, v[j].body)){
                        bullets[i].active = false;
                        v[j].health--;
                        if(v[j].health <= 0){
                            v[j].active = false;
                        }
                        break;
                    }
                }

                if(bullets[i].body.x > 800 || bullets[i].body.x < 0 ||
                    bullets[i].body.y > 800 || bullets[i].body.y < 0){
                    bullets[i].active = false;
                }
            }

            for(int i = (int)bullets.size()-1; i >= 0; i--){
                if(!bullets[i].active) bullets.erase(bullets.begin() + i);
            }

            for(int i = (int)v.size()-1; i >= 0; i--){
                if(!v[i].active){
                    v.erase(v.begin() + i);
                    score += enemyValue;
                    if(locked_index == i){
                        locked = false;
                        locked_index = -1;
                    } else if(locked_index > i){
                        locked_index--;
                    }
                }
            }
        }
        else{
            if(IsKeyPressed(KEY_ENTER)){
                gameOver     = false;
                score        = 0;
                locked       = false;
                locked_index = -1;
                v.clear();
                bullets.clear();
                time = 1.9f;
            }
        }

        int typed = GetCharPressed();
        while(typed > 0){
            char letter = (char)typed;

            if(!locked){
                for(int i = 0; i < (int)v.size(); i++){
                    if(v[i].currentIndex < (int)v[i].s.length() && letter == v[i].s[v[i].currentIndex]){
                        locked_index = i;
                        locked = true;

                        Bullet b;
                        b.targetId = v[i].id;
                        b.tex = &arrowTex;
                        b.body = {player.body.x + 60, player.body.y + 25, 80, 40};
                        float diffX = v[i].body.x - player.body.x;
                        float diffY = v[i].body.y - player.body.y;
                        float dist  = sqrt((diffX * diffX) + (diffY * diffY));
                        b.speedX = (diffX / dist) * bulletSpeed;
                        b.speedY = (diffY / dist) * bulletSpeed;
                        bullets.push_back(b);

                        v[i].currentIndex++;
                        if(v[i].currentIndex >= (int)v[i].s.length()){
                            locked = false;
                            locked_index = -1;
                        }
                        break;
                    }
                }
            }
            else{
                if(v[locked_index].currentIndex < (int)v[locked_index].s.length() &&
                    letter == v[locked_index].s[v[locked_index].currentIndex]){

                    Bullet b;
                    b.targetId = v[locked_index].id;
                    b.tex = &arrowTex;
                    b.body = {player.body.x + 60, player.body.y + 25, 80, 40};
                    float diffX = v[locked_index].body.x - player.body.x;
                    float diffY = v[locked_index].body.y - player.body.y;
                    float dist  = sqrt((diffX * diffX) + (diffY * diffY));
                    b.speedX = (diffX / dist) * bulletSpeed;
                    b.speedY = (diffY / dist) * bulletSpeed;
                    bullets.push_back(b);

                    v[locked_index].currentIndex++;
                }
                if(v[locked_index].currentIndex >= (int)v[locked_index].s.length()){
                    locked = false;
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

            for(int i = 0; i < (int)bullets.size(); i++) bullets[i].draw();
            for(int i = 0; i < (int)v.size(); i++)       v[i].draw(C);

            string leveltext = "LEVEL " + to_string(level);
            int levelWidth   = MeasureText(leveltext.c_str(), 30);
            DrawText(leveltext.c_str(), (800-levelWidth)/2, 60, 30, DARKBLUE);

            string scoretext = "Score: " + to_string(score);
            DrawText(scoretext.c_str(), 10, 10, 25, WHITE);
        }
        else{
            int gameOverWidth = MeasureText("GAME OVER", 60);
            DrawText("GAME OVER", (800-gameOverWidth)/2, 300, 60, DARKBLUE);

            string scoretext = "Score: " + to_string(score);
            int scoreWidth   = MeasureText(scoretext.c_str(), 30);
            DrawText(scoretext.c_str(), (800-scoreWidth)/2, 380, 30, DARKBLUE);

            int restartWidth = MeasureText("Press ENTER to Restart", 20);
            DrawText("Press ENTER to Restart", (800-restartWidth)/2, 430, 20, DARKBLUE);
        }

        EndDrawing();
    }

    UnloadTexture(arrowTex);
    UnloadTexture(enemyTex);
    UnloadTexture(bgTex);

    SetWindowSize(800, 600);
    SetWindowTitle("game");

    CloseWindow();
}

// g++ PrototypeGame.cpp -o PrototypeGame -lraylib -lm -lpthread -ldl && ./PrototypeGame
