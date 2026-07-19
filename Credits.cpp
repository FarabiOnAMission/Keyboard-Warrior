#include<raylib.h>

void Credits(){
    bool inCredits = true;
    bool canExit = false;

    while(inCredits && !WindowShouldClose()){
        
        if(IsKeyReleased(KEY_ENTER)){
            canExit = true;
        }

        if(IsKeyPressed(KEY_ESCAPE) || (IsKeyPressed(KEY_ENTER) && canExit)){
            inCredits = false;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        int titleWidth = MeasureText("CREDITS",50);
        DrawText("CREDITS",(800 - titleWidth)/2,100,50,GOLD);

        int name1width = MeasureText("Mahir Faisal",30);
        DrawText("Mahir Faisal",(800-name1width)/2,250,30,WHITE);

        int name2width = MeasureText("Akib Alwan",30);
        DrawText("Akib Alwan",(800-name2width)/2,320,30,WHITE);

        int name3width = MeasureText("Kazi Farabi",30);
        DrawText("Kazi Farabi",(800-name3width)/2,390,30,WHITE);

        int promptWidth = MeasureText("Press ESC or ENTER to return", 20);
        DrawText("Press ESC or ENTER to return", (800 - promptWidth) / 2, 550, 20, DARKGRAY);

        EndDrawing();
    }

}