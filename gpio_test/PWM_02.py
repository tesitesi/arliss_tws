#参考url:https://qiita.com/undo0530/items/51cc446eefec6c91c26d

import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)

#GPIO4を制御パルスの出力に設定
gp_out =  21
GPIO.setup(gp_out, GPIO.OUT)

#デューティー比:0~100
duty_ratio = 50

#「GPIO4出力」でPWMインスタンスを作成する。
#GPIO.PWM( [ピン番号] , [周波数Hz] )
#SG92RはPWMサイクル:20ms(=50Hz), 制御パルス:0.5ms〜2.4ms, (=2.5%〜12%)。
servo = GPIO.PWM(gp_out, 50)

#パルス出力開始。　servo.start( [デューティサイクル 0~100%] )
#とりあえずゼロ指定だとサイクルが生まれないので特に動かないっぽい？
servo.start(0)
#time.sleep(1)

i = 0

while i < 10000:
    servo.ChangeDutyCycle(duty_ratio)
    time.sleep(0.001)
    i = i + 1

k = 0
print(duty_ratio)

while duty_ratio > 0:
    while k < 100:
        servo.ChangeDutyCycle(duty_ratio)
        time.sleep(0.001)
        k = k + 1
    k = 0
    print(duty_ratio)
    duty_ratio = duty_ratio - 1
    
servo.stop()
GPIO.cleanup()
