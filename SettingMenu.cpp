#include<raylib.h>
#include<fstream>
#include<string>


void RunSettings(){
  int selectedOption = 0;
  const int totalOptions = 3;
  bool inSettings = true;
  bool canSelect = false;
  bool showingScorecard = false;
  int currentHighScore=0;

  while(inSettings && !WindowShouldClose()){

    if(IsKeyReleased(KEY_ENTER)) canSelect = true;
    
    if(showingScorecard){
      if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)){
        showingScorecard = false;
        canSelect = false;
      }
    } 
    else{
      if(IsKeyPressed(KEY_DOWN)) selectedOption = (selectedOption + 1 ) % totalOptions;
      if(IsKeyPressed(KEY_UP)) selectedOption = (selectedOption - 1 + totalOptions) % totalOptions;
      if(IsKeyPressed(KEY_ESCAPE)) inSettings = false;

      if(IsKeyPressed(KEY_ENTER) && canSelect){
        if(selectedOption==1){
          showingScorecard = true;
          std::ifstream file("highscore.save");
          if(file.is_open()){
              file >> currentHighScore;
              file.close();
          } else {
              currentHighScore = 0; 
          }
        }
      }
    }
    BeginDrawing();
    ClearBackground(SKYBLUE);
    if(showingScorecard){
      int title = MeasureText("SCORECARD",40);
      DrawText("SCORECARD", (800 - title) / 2, 100, 40, WHITE);

      string scoreText = "Highest Score: " + to_string(currentHighScore);
      int scoreWidth = MeasureText(scoreText.c_str(),40);
      DrawText(scoreText.c_str(), (800 - scoreWidth) / 2, 300, 40, YELLOW);

      int promptWidth = MeasureText("Press ESC or ENTER to return", 20);
      DrawText("Press ESC or ENTER to return", (800 - promptWidth) / 2, 500, 20, GRAY);
    }
    else{
    int title = MeasureText("SETTINGS",40);
    DrawText("SETTINGS",(800 - title) / 2, 100, 40, WHITE);

    Color color1 = (selectedOption == 0) ? YELLOW : LIGHTGRAY;
    Color color2 = (selectedOption == 1) ? YELLOW : LIGHTGRAY;
    Color color3 = (selectedOption == 2) ? YELLOW : LIGHTGRAY;

    DrawText("1. Sound Settings", 250, 250, 30, color1);
    DrawText("2. Scorecard Checker", 250, 320, 30, color2);
    DrawText("3. Credits", 250, 390, 30, color3);

    int promptWidth = MeasureText("Use UP/DOWN to select. Press ESC to leave.", 20);
    DrawText("Use UP/DOWN to select. Press ESC to leave.", (800 - promptWidth) / 2, 500, 20, GRAY);
    }
    EndDrawing();
  }
}