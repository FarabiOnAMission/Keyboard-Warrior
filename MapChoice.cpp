#include<raylib.h>

bool RunBattle(int level); 

void RunChoice(){
  int selectedOption = 0;
  const int totalOptions = 3;
  bool inSettings = true;

  bool KeyReleased = false;

  while(inSettings && !WindowShouldClose()){

    if(IsKeyReleased(KEY_ENTER)){
      KeyReleased=true;
    }

    if(IsKeyPressed(KEY_DOWN)){
      selectedOption = (selectedOption + 1 )% totalOptions;
    }

    if(IsKeyPressed(KEY_UP)){
      selectedOption = (selectedOption - 1 + totalOptions) % totalOptions;
    }

    if(IsKeyPressed(KEY_ESCAPE)){
      inSettings = false;
    }

    if(IsKeyPressed(KEY_ENTER) && KeyReleased){
      // 2. ADDED THE 'if' CHECKS TO CATCH THE TRUE SIGNAL
      if(selectedOption == 0){
        if (RunBattle(1)) return; 
      }
      else if(selectedOption == 1){
        if (RunBattle(2)) return;
      }
      else if(selectedOption == 2){
        if (RunBattle(3)) return;
      }
    }

    BeginDrawing();
    ClearBackground(SKYBLUE);

    int title = MeasureText("SETTINGS",40);
    DrawText("SETTINGS",(800 - title) / 2, 100, 40, WHITE);

    Color color1 = (selectedOption == 0) ? YELLOW : LIGHTGRAY;
    Color color2 = (selectedOption == 1) ? YELLOW : LIGHTGRAY;
    Color color3 = (selectedOption == 2) ? YELLOW : LIGHTGRAY;

    DrawText("1. MAP 1", 250, 250, 30, color1);
    DrawText("2. MAP 2", 250, 320, 30, color2);
    DrawText("3. MAP 3", 250, 390, 30, color3);

    int promptWidth = MeasureText("Use UP/DOWN to select. Press ESC to leave.", 20);
    DrawText("Use UP/DOWN to select. Press ESC to leave.", (800 - promptWidth) / 2, 500, 20, GRAY);

    EndDrawing();
  }
}