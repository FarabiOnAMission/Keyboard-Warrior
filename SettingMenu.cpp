#include<raylib.h>

void RunSettings(){
  int selectedOption = 0;
  const int totalOptions = 3;
  bool inSettings = true;

  while(inSettings && !WindowShouldClose()){

    if(IsKeyPressed(KEY_DOWN)){
      selectedOption = (selectedOption + 1 )% totalOptions;
    }

    if(IsKeyPressed(KEY_UP)){
      selectedOption = (selectedOption - 1 + totalOptions) % totalOptions;
    }

    if(IsKeyPressed(KEY_ESCAPE)){
      inSettings = false;
    }

    if(IsKeyPressed(KEY_ENTER)){

    }

    BeginDrawing();
    ClearBackground(SKYBLUE);

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

    EndDrawing();
  }
}