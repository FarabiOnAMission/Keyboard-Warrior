#include<raylib.h>
#include "main-map.h"
#include "PrototypeGame.cpp"
#include "SettingMenu.cpp"

#define IDLE_LAST_FRAME 3
#define IDLE_FIRST_FRAME 0

//Character Definition Section

//Movement Direction of The Character
enum SpriteDirection{
    Left = -1,
    Right = 1
};

//PlayerState
enum PlayerState{
  STATE_IDLE,
  STATE_RUN
};

typedef struct Sprite{
  Texture2D idle_texture; //Texture to Wrap around a object
  Texture2D run_texture;
  PlayerState state;
  Rectangle dest_rect; //Where that object will place itself in the Window
  Vector2 vel; //Vector to track velocity of a object
  SpriteDirection dir; //to track which position a object is facing
  bool isGrounded; //To check if the player is on ground or not
  //------ANIMATION----------
  int currentFrame;
  float frameTimer;
  float frameSpeed;
}Sprite;

typedef struct Tileset{
  Texture2D texture;
  int columns;
  int firstGID;
  int tileCount;
}Tileset;

//Defining the game state
enum GameState{
  HUB_WORLD, //Main Menu
  BATTLE_SCENE //Main Game
};

GameState currentState = HUB_WORLD; //Starting Position of the Game which is the Main Menu

void move_player(Sprite *player){
    PlayerState previousState = player->state;
    player->vel.x = 0.0; //initial velocity = 0.0
    player->state = STATE_IDLE;
    
    if(IsKeyDown(KEY_RIGHT)){  
        player->vel.x = 100.0f; //right key and becomes 100.0f constant
        player->dir = Right; //player is facing right
        player->state = STATE_RUN;
      }

    else if(IsKeyDown(KEY_LEFT)){
        player->vel.x = -100.0f; //left key and becomes -100.0f constant
        player->dir = Left; //player is facing left 
        player->state = STATE_RUN;
    }

    if(IsKeyPressed(KEY_SPACE) &&  player->isGrounded){
        player->vel.y = -600.0f; //go up 
        player->isGrounded = false;
    }

    if (player->state != previousState) {
        player->currentFrame = 0;
        player->frameTimer = 0.0f;
    }
}

void apply_gravity(Sprite *player){
  player->vel.y += 2000.0f * GetFrameTime(); //will always fall down as it does in concept of gravity untill they hit a wall of collision/ground

  if(player->vel.y >500.0f){
    player->vel.y = 500.0f; //gravity to fall down
  }
}

void apply_velocity(Sprite *player){
    player->dest_rect.x += player->vel.x * GetFrameTime();
    player->dest_rect.y += player->vel.y * GetFrameTime();
}

void AnimatePlayer(Sprite *player){
  player->frameTimer += GetFrameTime();

  if(player->frameTimer >= player->frameSpeed){
    player->frameTimer = 0.0f;
    player->currentFrame++;
  }

  int maxFrames = (player->state == STATE_RUN ? 8 : 4);

  if(player->currentFrame >= maxFrames ){
    player->currentFrame = 0;
  }
}


//Map for Drawing
void DrawMapLayer(const int MapArray[ROWS][COLS], Tileset tilesets[], int tilesetCount){
  for(int row=0;row<ROWS;row++){
    for(int col=0;col<COLS;col++){

      int tileID = MapArray[row][col];
      if(tileID == 0 ) continue;

      Tileset *matchedTileset = nullptr;
      for(int i=tilesetCount - 1;i>=0;i--){
        if(tileID >= tilesets[i].firstGID){
          matchedTileset = &tilesets[i];
          break;
        }
      }

      if(matchedTileset != nullptr){
        int localID = tileID - matchedTileset->firstGID;

        float srcX = (localID%matchedTileset->columns) * TILE_SIZE;
        float srcY = (localID/matchedTileset->columns) * TILE_SIZE;

        Rectangle sourceRect = {srcX,srcY, (float)TILE_SIZE,(float)TILE_SIZE};
        Vector2 destPosition = {(float)(col* TILE_SIZE),(float)(row* TILE_SIZE)};
        
        DrawTextureRec(matchedTileset->texture, sourceRect, destPosition, WHITE);
      }
    }
  }
}

int main(){
    InitWindow(800,600,"game");
    InitAudioDevice();
    SetTargetFPS(60);

    Texture2D texGround     = LoadTexture("resource/Floor Tiles1.png");
    Texture2D texSky        = LoadTexture("resource/GandalfHardcore Background layers_layer 5.png");
    Texture2D texTent       = LoadTexture("resource/Large Tent.png");
    Texture2D texDecor      = LoadTexture("resource/Decor.png");
    Texture2D texWillow     = LoadTexture("resource/Weeping Willow1.png");
    Texture2D texTree2      = LoadTexture("resource/Tree2.png");
    Texture2D texPine       = LoadTexture("resource/Large Pine Tree.png");
    Texture2D texBirch      = LoadTexture("resource/Birch2.png");
    Texture2D texBuildings  = LoadTexture("resource/Buildings.png");
    Texture2D texBgTrees    = LoadTexture("resource/GandalfHardcore Background layers_layer 1.png");
    Texture2D texClouds     = LoadTexture("resource/cloud5.png");
    Texture2D texSun        = LoadTexture("resource/sun.png");

    Texture2D player_idle_texture = LoadTexture("resource/Idle-Sheet.png");
    Texture2D player_run_texture = LoadTexture("resource/Run-Sheet.png");

    Music bgMusic = LoadMusicStream("resource/theme.ogg");
    PlayMusicStream(bgMusic);
    SetMusicVolume(bgMusic, 1.0f);
  
  float frameWidth = (float)player_idle_texture.width/4.0f; //Because my original frame has 4 characters so i need to start it from the beginning
  float frameHeight = (float)player_idle_texture.height;
  float scaleMultiplier = 1.5f;
  int frameIndex = 1;

  Rectangle destRect = {
    150.0f,
    200.0f,
    frameWidth * scaleMultiplier,
    frameHeight * scaleMultiplier
  };

  Vector2 origin = {0.0f, 0.0f};
  float rotation = 0.0f;

    Sprite Player = (Sprite){
        .idle_texture = player_idle_texture,
        .run_texture = player_run_texture,
        .state = STATE_IDLE,
        .dest_rect = destRect,
        .dir = Right,
        .isGrounded = false,
        .currentFrame = 0, //start at frame 0
        .frameTimer = 0.0f,
        .frameSpeed = 0.1f //Change every 0.15 seconds
    };
    
    const int TOTAL_TILESETS = 12;
    Tileset gameTilesets[TOTAL_TILESETS]={
      { texGround,    9, 1, 162      },
      { texSky,       32, 163, 320   },
      { texTent,      3, 483, 12     },
      { texDecor,     13, 495, 221   },
      { texWillow,    7,  716,  42   },
      { texTree2,     8,  758,  48   },
      { texPine,      4,  806,  20   },
      { texBirch,     2,  826,  6    },
      { texBuildings, 12, 832,  144  },
      { texBgTrees,   32, 976,  320  },
      { texClouds,    3,  1296, 3    },
      { texSun,       1,  1299, 1    }
    };

    Camera2D camera = {0};
    camera.offset = (Vector2){800.0f/2, 600.0f/2};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    

    SetExitKey(KEY_NULL);
    bool exitGame = false;
    Rectangle tentTrigger = { 8.0f * TILE_SIZE, 11.0f * TILE_SIZE, 1.0f * TILE_SIZE, 3.0f * TILE_SIZE };
    Rectangle settingsTrigger = { 27.0f * TILE_SIZE, 11.0f * TILE_SIZE, 2.0f * TILE_SIZE, 3.0f * TILE_SIZE };
    Rectangle exitTrigger = { 46.0f * TILE_SIZE, 11.0f * TILE_SIZE, 2.0f * TILE_SIZE, 3.0f * TILE_SIZE };

    while(!exitGame && !WindowShouldClose()){

      move_player(&Player);
      apply_gravity(&Player);
      UpdateMusicStream(bgMusic);
      apply_velocity(&Player);
      AnimatePlayer(&Player);

      int feetOffset = 0.0f;

      int playerBottomTileY = (Player.dest_rect.y + Player.dest_rect.height - feetOffset)/ TILE_SIZE;
      int playerCenterTileX = (Player.dest_rect.x + (Player.dest_rect.width/2.0f)) / TILE_SIZE;

      if(playerBottomTileY >=0 && playerBottomTileY < ROWS
      && playerCenterTileX >=0 && playerCenterTileX < COLS){

        Player.isGrounded = false;

        if(groundLayer[playerBottomTileY][playerCenterTileX]!=0){
          if(Player.vel.y>=0){
                Player.dest_rect.y = (playerBottomTileY * TILE_SIZE) - Player.dest_rect.height + feetOffset;
                Player.vel.y = 0.0f;
                Player.isGrounded = true;
            }
        }
      }

        camera.target.x = (Player.dest_rect.x + (Player.dest_rect.width/ 2.0f));
        camera.target.y = (Player.dest_rect.y + (Player.dest_rect.height/ 2.0f));

        float screenWidth = 800.0f;
        float screenHeight = 600.0f;
        float mapWidth = COLS * TILE_SIZE;
        float mapHeight = ROWS * TILE_SIZE;

        if(camera.target.x < camera.offset.x){
          camera.target.x = camera.offset.x;
        }
        if(camera.target.x > mapWidth - (screenWidth - camera.offset.x)){
          camera.target.x =  mapWidth - (screenWidth - camera.offset.x);
        }

        if(camera.target.y < camera.offset.y){
          camera.target.y = camera.offset.y;
        }
        if(camera.target.y > mapHeight - (screenHeight - camera.offset.y)){
          camera.target.y =  mapHeight - (screenHeight - camera.offset.y);
        }

        //Collision to check if the player entered the house where the main game should begin
        if(CheckCollisionRecs(Player.dest_rect, tentTrigger)){
          if(IsKeyPressed(KEY_ENTER)){
            RunBattle();
          }
        }

        if(CheckCollisionRecs(Player.dest_rect, settingsTrigger)){
          if(IsKeyPressed(KEY_ENTER)){
            RunSettings(); 
          }
        }

        if(CheckCollisionRecs(Player.dest_rect, exitTrigger)){
          if(IsKeyPressed(KEY_ENTER)){
            exitGame = true;
          }
        }

        BeginDrawing();
        ClearBackground(SKYBLUE);

        BeginMode2D(camera);

        DrawMapLayer(skyLayer, gameTilesets, TOTAL_TILESETS);
        DrawMapLayer(bgTreesLayer, gameTilesets, TOTAL_TILESETS);
        DrawMapLayer(decorsLayer, gameTilesets, TOTAL_TILESETS);
        DrawMapLayer(houseLayer, gameTilesets, TOTAL_TILESETS);
        DrawMapLayer(groundLayer, gameTilesets, TOTAL_TILESETS);

        Texture2D currentTexture = (Player.state == STATE_RUN) ? Player.run_texture : Player.idle_texture;
        float frame_width = (Player.state == STATE_RUN) ? 8.0f : 4.0f;
        float currentFrameWidth = (float)currentTexture.width / frame_width;
        float currentFrameHeight = (float)currentTexture.height;

        Rectangle drawRect;
        drawRect.width = currentFrameWidth * scaleMultiplier;
        drawRect.height = currentFrameHeight * scaleMultiplier;

        drawRect.x = Player.dest_rect.x - (drawRect.width - Player.dest_rect.width) / 2.0f;
        float visualYOffset = (Player.state == STATE_RUN) ? 22.0f : 0.0f;
        drawRect.y = (Player.dest_rect.y + Player.dest_rect.height) - drawRect.height + visualYOffset;



        DrawTexturePro(currentTexture,
            {Player.currentFrame * currentFrameWidth, 0.0f, currentFrameWidth * static_cast<float>(Player.dir), currentFrameHeight},
            drawRect, origin, rotation, WHITE);

            EndMode2D();
            //Text to enter main game

            if(CheckCollisionRecs(Player.dest_rect, tentTrigger)){
              int textWidth = MeasureText("Press ENTER to start Typing Battle", 20);
              DrawText("Press ENTER to start Typing Battle", (screenWidth - textWidth) / 2, 50, 20, BLACK);
            }

            if(CheckCollisionRecs(Player.dest_rect, settingsTrigger)){
              int textWidth = MeasureText("Press ENTER to go to Settings", 20);
              DrawText("Press ENTER to go to Settings", (screenWidth - textWidth) / 2, 50, 20, BLACK);
            }

            if(CheckCollisionRecs(Player.dest_rect, exitTrigger)){
              int textWidth = MeasureText("Press ENTER to exit the game", 20);
              DrawText("Press ENTER to exit the game", (screenWidth - textWidth) / 2, 50, 20, BLACK);
            }
            
            EndDrawing();
    }
    UnloadMusicStream(bgMusic);
    CloseAudioDevice();
    UnloadTexture(texGround);
    UnloadTexture(texSky);
    UnloadTexture(texTent);
    UnloadTexture(texDecor);
    UnloadTexture(texWillow);
    UnloadTexture(texTree2);
    UnloadTexture(texPine);
    UnloadTexture(texBirch);
    UnloadTexture(texBuildings);
    UnloadTexture(texBgTrees);
    UnloadTexture(texClouds);
    UnloadTexture(player_idle_texture);
    UnloadTexture(player_run_texture);
    CloseWindow();

    return 0;
  }