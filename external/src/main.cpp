#include <cstdlib>
#include <thread>
#include <vector>
#include <math.h>
#include <emscripten.h>
#include "perlin.h"

typedef float dataType;

double timeCount = 0;
#define THREADS_COUNT 2
std::thread updateThread[THREADS_COUNT];

double infiniteSpace(double num, double min, double max) {
    if (num > max) return min;
    else if (num < min) return max;
    return num;
}

void UpdateFire(int data_length, dataType* data, double width, double height, double stepSize, double speed, dataType mouseX, dataType mouseY, dataType mousePower) 
{   
    timeCount += stepSize;
    const siv::PerlinNoise perlin(5);
    for (int i = 0; i < data_length; i+=4) {
        double noise = perlin.noise3D(10 * data[i] / width, 10 * data[i+1] / height, timeCount) * 6.2826852;
        dataType vx = 0;
        dataType vy = 0;
        dataType dotSpeed = speed;

        if (mouseX + mouseY > 0) {
            dataType dx = (mouseX-data[i]);
            dataType dy = (mouseY-data[i+1]);
            dataType dist = sqrt(dx*dx + dy*dy);
            if (dist < 40 + rand()%10) {
                dotSpeed += mousePower/(dist*dist);
                vx += (rand() % 200) / 100. - 1.;
                vy += (rand() % 200) / 100. - 1.;
            }
            //
            //
        }

        vx += cos(noise) * dotSpeed;
        vy += sin(noise) * dotSpeed;

        data[i+2] = (data[i+2] + vx) * 0.94;
        data[i+3] = (data[i+3] + vy) * 0.94;
        data[i] = infiniteSpace(data[i] + data[i+2], 0, width);
        data[i+1] = infiniteSpace(data[i+1] + data[i+3], 0, height);

    }
}

void UpdateLoop(int data_length, dataType* data, double width, double height, double stepSize, double speed) {
    while(true)
        UpdateFire(data_length/THREADS_COUNT, data, width, height, stepSize, speed, -1, -1, -1);
}
void StartUpdateFireThread(int data_length, dataType* data, double width, double height, double stepSize, double speed)  {
    for (int i = 0; i < THREADS_COUNT; i++)
        updateThread[i] = std::thread(UpdateLoop, data_length/THREADS_COUNT, &data[i*data_length/THREADS_COUNT], width, height, stepSize, speed);
}

extern "C" {
    void EMSCRIPTEN_KEEPALIVE cppStartThread(int data_length, dataType* data, int width, int height, double stepSize, double speed) {
        for (int i = 0; i < data_length; i+=4) {
            data[i] =   std::rand() % width;
            data[i+1] = std::rand() % height;
        }
        StartUpdateFireThread(data_length, data, width, height, stepSize, speed);
    }

    void EMSCRIPTEN_KEEPALIVE cppSetup(int data_length, dataType* data, int width, int height) {
        for (int i = 0; i < data_length; i+=4) {
            data[i] =   std::rand() % width;
            data[i+1] = std::rand() % height;
        }
    }

    void EMSCRIPTEN_KEEPALIVE cppUpdate(int data_length, dataType* data, double width, double height, double stepSize, double speed, double mouseX, double mouseY, double mousePower) {
        UpdateFire(data_length, data, width, height, stepSize, speed, mouseX, mouseY, mousePower);
    }
}