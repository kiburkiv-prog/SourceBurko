#include <iostream>
#include <raylib.h>
#include <cmath>
#include "pl_methods.cpp"

using namespace std;

class player{
public:
    int width = 1280;
    int height = 720;
    double x = 62;
    double y = 62;
    int weapon = 0;
    int weapon_len = 2;
    double angle = 0;
    double rays[100][150][2];
    double angles[100];
    double hor_y = 360;
    bool moving = false;
    int fov = 100;
    int ray_len = 150;

    player(){
        for(int i = 0; i < fov; i++){
            for(int j = 0; j < ray_len; j++){
                rays[i][j][0] = x;
                rays[i][j][1] = y - x;
            }
        }
        for(int i = 0; i < fov; i++){
            angles[i] = double(i - double(fov)/2) * M_PI / 180;
            cout << angles[i] << "\n";
        }
    }

    int is_moving(double val1, double val2){
        for(int i = 0; i < 142; i++){
            int xw1 = walls[i][0];
            int xw2 = xw1 + 10;
            int yw1 = walls[i][1];
            int yw2 = yw1 + 10;
            int nx = val1 + x;
            int ny = val2 + y;
            if((nx >= xw1 && nx <= xw2) && (ny >= yw1 && ny <= yw2)){
                cout << 0;
                return 0;
            }
        }


        return 1;
    }



    void move(double fpc){
        Vector2 mouse = GetMouseDelta();


        angle += mouse.x / 2;
        if(!IsKeyUp(KEY_A)){
            if(is_moving(+ fpc * sin(angle * (M_PI / 180)), 0) == 1){
                x = x + fpc * sin(angle * (M_PI / 180)) * 0.25;
                moving = true;
            }
            if(is_moving(0,  - fpc * cos(angle * (M_PI / 180)) * 0.25) == 1){
                y = y - fpc * cos(angle * (M_PI / 180)) * 0.25;
                moving = true;
            }


        }
        if(!IsKeyUp(KEY_D)){
            if(is_moving( - fpc * sin(angle * (M_PI / 180)) , 0) == 1){
                x = x - fpc * sin(angle * (M_PI / 180)) * 0.25;
                moving = true;
            }
            if(is_moving(0,  + fpc * cos(angle * (M_PI / 180))) == 1){
                y = y + fpc * cos(angle * (M_PI / 180)) * 0.25;
                moving = true;
            }


        }
        if(!IsKeyUp(KEY_W)){
            if(is_moving( + fpc * cos(angle * (M_PI / 180)) , 0) == 1){
                x = x + fpc * cos(angle * (M_PI / 180)) * 0.25;
                moving = true;
            }
            if(is_moving(0,  + fpc * sin(angle * (M_PI / 180) )) == 1){
                y = y + fpc * sin(angle * (M_PI / 180)) * 0.25;
                moving = true;
            }



        }
        else if(!IsKeyUp(KEY_S)){
            if(is_moving( - fpc * cos(angle * (M_PI / 180)) , 0) == 1){
                x = x - fpc * cos(angle * (M_PI / 180)) * 0.25;
                moving = true;
            }
            if(is_moving(0,  - fpc * sin(angle * (M_PI / 180))) == 1){
                y = y - fpc * sin(angle * (M_PI / 180)) * 0.25;
                moving = true;
            }


        }
        if(hor_y  < (height) && hor_y > 0){
            hor_y -= mouse.y * 4;
            cout << hor_y << "\n";
        }
        if(hor_y >= height){
            hor_y  = height - 1;
        }
        if(hor_y <= 0){
            hor_y = 1;
        }
    }
};
