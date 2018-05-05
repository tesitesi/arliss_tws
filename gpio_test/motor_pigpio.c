// 参考URL
// http://www.geocities.jp/spacelab/archives/4061/

//motor pigpio.c v1.00 20150810
//コンパイルは以下のコマンドを実行
//gcc motor_pigpio.c -o motor_pigpio -lpigpiod_if -lpthread -lrt

#include <stdio.h>
#include <pigpiod_if.h>
 
 
#define MOTOR1 22           // GPIO22
#define MOTOR2 23           // GPIO23
#define MOTOR3 24           // GPIO24
#define MOTOR4 25           // GPIO25
#define RANGE 255           //PWM最大値
#define LEFT_STR "l"
#define RIGHT_STR "r"
#define FWRD_STR "f"
#define BACK_STR "b"
#define STOP_STR "s"
 
int main(int argc, char **argv)
{
    int pwm,status;
 
    // 速度を設定
    if ( argc < 3 ){ pwm = 0; } else { pwm = atoi(argv[2]); } if ( pwm > RANGE ){ pwm = RANGE; }
 
    // pigpiod接続
    status = pigpio_start(0, 0);
    //if (status < 0){ return 1; }
 
    // softPWM設定
    set_PWM_range(MOTOR1, RANGE);
    set_PWM_range(MOTOR2, RANGE);
    set_PWM_range(MOTOR3, RANGE);
    set_PWM_range(MOTOR4, RANGE);
 
    //停止
    set_PWM_dutycycle(MOTOR1, 0);
    set_PWM_dutycycle(MOTOR4, 0);
    set_PWM_dutycycle(MOTOR2, 0);
    set_PWM_dutycycle(MOTOR3, 0);
 
    if ( strcmp( argv[1], FWRD_STR ) == 0 ){
        //前進
        set_PWM_dutycycle(MOTOR2,pwm);
        set_PWM_dutycycle(MOTOR4,pwm);
        return 0;
    } else if ( strcmp( argv[1], BACK_STR ) == 0 ){
        //後退
        set_PWM_dutycycle(MOTOR1,pwm);
        set_PWM_dutycycle(MOTOR3,pwm);
        return 0;
    } else if ( strcmp( argv[1], LEFT_STR ) == 0 ){
        //左ターン
        set_PWM_dutycycle(MOTOR1,pwm);
        set_PWM_dutycycle(MOTOR4,pwm);
        return 0;
    } else if ( strcmp( argv[1], RIGHT_STR ) == 0 ){
        //右ターン
        set_PWM_dutycycle(MOTOR2,pwm);
        set_PWM_dutycycle(MOTOR3,pwm);
        return 0;
    }
 
    //ブレーキ
    set_PWM_dutycycle(MOTOR1, RANGE);
    set_PWM_dutycycle(MOTOR4, RANGE);
    set_PWM_dutycycle(MOTOR2, RANGE);
    set_PWM_dutycycle(MOTOR3, RANGE);
    time_sleep(0.5);
    //停止
    set_PWM_dutycycle(MOTOR1, 0);
    set_PWM_dutycycle(MOTOR4, 0);
    set_PWM_dutycycle(MOTOR2, 0);
    set_PWM_dutycycle(MOTOR3, 0);
 
    return 0;
}
