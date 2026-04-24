#include <raylib.h>
#include <iostream>
#include <cmath>
#include <string>
#include "player.cpp"

using namespace std;
int WIDTH = 1280;
int HEIGHT = 720;
int fps = 1200;
int weapon = 0;
Font sfont = LoadFont("./font.ttf");
player Player;
int object_count = 0;
Texture2D ntexture;



void update_rays(double r[100][150][2], double ang[100], player Player, int walls[142][2], Texture2D texture){


    for(int i = 0; i < Player.fov; i++){
        for(int x = 1; x < Player.ray_len; x++){
            double new_ang = Player.angle * M_PI / 180;
            r[i][x][0] = Player.x + x * cos(new_ang + ang[i]);
            r[i][x][1] = Player.y + x * sin(new_ang + ang[i]);
            r[i][0][0] = Player.x;
            r[i][0][1] = Player.y;
            double px1 = (int)r[i][x][0];
            double py1 = (int)r[i][x][1];
            int is = 0;
            for(int w = 0; w < object_count; w++){
                double xw1 = walls[w][0];
                double xw2 = walls[w][0] + 10;
                double yw1 = walls[w][1];
                double  yw2 = walls[w][1] + 10;
                if((px1 == xw1 || px1 == xw2) && (py1 >= yw1 && py1 <= yw2)){
                    double newx = i  * (WIDTH / (Player.fov));
                    int height = HEIGHT / (x) + 40 + (100 / (x));
                    double width = double(WIDTH) / (Player.fov);
                    double newy = Player.hor_y - (double(height) / 2);
                    Image temp = LoadImageFromTexture(texture);
                    ImageCrop(&temp, {(float)(py1 - yw1) * (float(texture.width) / 10), (float)0, (float)texture.width / 10, (float)texture.height});
                    DrawRectangle(newx, newy, width, height, ColorBrightness(GRAY, 1.0f - (x * 0.02f)));
                    ntexture = LoadTextureFromImage(temp);
                    UnloadImage(temp);
                    Rectangle source = { 0.0f, 0.0f, (float)ntexture.width, (float)ntexture.height };
                    Rectangle dest = { float(newx), float(newy), float(width), float(height)};
                    DrawTexturePro(ntexture, source, dest, {0.0f,0.0f}, 0.0f, ColorBrightness(GRAY, 1.0f - (x * 0.02f)));



                    is = 1;
                    break;
                }
                if((px1 >= xw1 && px1 <= xw2) && (py1 == yw1 || py1 == yw2)){
                    double newx = i  * (WIDTH / (Player.fov));
                    int height = HEIGHT / (x) + 40 + (100 / (x));
                    double width = double(WIDTH) / (Player.fov);
                    double newy = Player.hor_y - (double(height) / 2);
                    Image temp = LoadImageFromTexture(texture);
                    ImageCrop(&temp, {(float)(px1 - xw1) * (float(texture.width) / 10), (float)0, float(texture.width) / 10, (float)texture.height});
                    ntexture = LoadTextureFromImage(temp);
                    UnloadImage(temp);
                    Rectangle source = { 0.0f, 0.0f, (float)ntexture.width, (float)ntexture.height };
                    Rectangle dest = { float(newx), float(newy), float(width), float(height)};
                    DrawRectangle(newx, newy, width, height, ColorBrightness( GRAY, 1.0f - (x * 0.02f)));
                    DrawTexturePro(ntexture, source, dest, {0.0f,0.0f}, 0.0f, ColorBrightness(GRAY, 1.0f - (x * 0.02f)));


                    is = 1;
                    break;
                }
            }
            if(is == 1){
                break;
            }
        }
    }
}

void draw_sky(){
    DrawRectangle(0, Player.hor_y - HEIGHT, WIDTH, HEIGHT, BLUE);
    DrawRectangle(0, Player.hor_y, WIDTH, HEIGHT, GRAY);
}

void draw_hud(){
    DrawTextEx(sfont, to_string(GetFPS()).c_str(), {0,0}, 20, 2, GREEN);
}




void init_map(){
    int pivot = 0;
    int pivot_x = 0;
    int pivot_y = 0;
    for(int i = 0; i < 10; i++){
        for(int x = 0; x < 58; x++){
            if(mapw[i][x] == '1'){
                walls[pivot][0] = pivot_x;
                walls[pivot][1] = pivot_y;
                object_count += 1;
                pivot += 1;
                printf("x = %d, y = %d\n", walls[pivot - 1][0], walls[pivot-1][1]);
            }
            pivot_x += 11;
        }
        pivot_y += 11;
        pivot_x = 0;
    }
}

int main(){
    cout<<"Privet ot Vani Burko!\n";
    SetTraceLogLevel(LOG_NONE);

    init_map();
    InitWindow(WIDTH, HEIGHT, "Source-Burko binary");
    SetTargetFPS(fps);
    DisableCursor();

    Texture2D crosshair_text = LoadTexture("crosshair.png");
    Texture2D texture = LoadTexture("texture4.png");
    Texture2D weapons[2] = {LoadTexture("data/ak47/w.png"), LoadTexture("data/usp/w.png")};
    Vector2 mouse;
    while(!WindowShouldClose()){
        if(Player.angle == 360 || Player.angle == -360){
            Player.angle = 0;
        }
        double fpc = (60.0f / GetFPS());
        BeginDrawing();
        ClearBackground(BLACK);
        weapon += GetMouseWheelMoveV().y;
        if(weapon > 1){
            weapon = 0;
        }
        if(weapon < 0){
            weapon = 1;
        }


        draw_sky();
        update_rays(Player.rays, Player.angles, Player, walls, texture);

        draw_hud();
        Player.move(fpc);
        DrawTexture(weapons[weapon], 0, 0, WHITE);
        DrawTexture(crosshair_text, WIDTH / 2 - 100, HEIGHT / 2 - 100, WHITE);
        Player.moving=false;
        EndDrawing();
    }
    UnloadTexture(crosshair_text);

    return 0;
}
