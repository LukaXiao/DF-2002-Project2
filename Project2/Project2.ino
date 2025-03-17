#include "TinyFilmFestival.h"
#include "Normal.h"   // Normal动画
#include "Happy.h"    // Happy动画

#define TRIG_PIN 6  // HC-SR04 触发引脚
#define ECHO_PIN 7  // HC-SR04 回响引脚

TinyFilmFestival film;
Animation normalAnimation = Normal;   // 默认动画
Animation happyAnimation = Happy;     // 开心动画

int playSpeed = 50;  // 动画播放速度（毫秒）
long duration;
int distance;
bool isHappy = false; // 记录当前是否是开心动画

void setup() {
    Serial.begin(9600);

    // 初始化 LED 矩阵
    film.begin();

    // 默认播放 Normal 动画（循环）
    film.startAnimation(normalAnimation, LOOP);
    
    // 设置动画速度
    film.setSpeed(playSpeed);

    // 初始化超声波传感器
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

void loop() {
    // 更新动画
    film.update();

    // 读取超声波传感器数据
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    duration = pulseIn(ECHO_PIN, HIGH);
    distance = duration * 0.034 / 2;  // 计算距离 (cm)

    Serial.print("Distance: ");
    Serial.println(distance);

    // 如果手靠近（小于 10cm），播放 Happy 动画
    if (distance < 10 && !isHappy) {
        film.startAnimation(happyAnimation, LOOP);  // 播放 Happy
        isHappy = true;  // 记录当前状态
    }
    // 如果手远离（大于等于 10cm），播放 Normal 动画
    else if (distance >= 10 && isHappy) {
        film.startAnimation(normalAnimation, LOOP);  // 切换回 Normal
        isHappy = false;
    }

    delay(100); // 避免传感器抖动
}
