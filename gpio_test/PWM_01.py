# https://qiita.com/s417-lama/items/0ef64a7af3fcf6a56cc5


import pigpio
import time

gpio_pin0 = 10

#duty比 0~1の範囲で指定
duty_ratio = 0.5

pi = pigpio.pi()
pi.set_mode(gpio_pin0, pigpio.OUTPUT)

i = 0
# 飛行時duty比 duty_ratio
while i < 100000:
    pi.hardware_PWM(gpio_pin0, 8, int(duty_ratio * 1000000))
    time.sleep(0.001)
    i = i+ 1

#duty比を線形に0まで落とす
k = 0
while duty_ratio > 0:
    while k < 100: 
        pi.hardware_PWM(gpio_pin0, 8,  int(duty_ratio * 1000000))
        time.sleep(0.001)
    k = 0
    duty_ratio = duty_ratio - 0.01

pi.set_mode(gpio_pin0, pigpio.INPUT)
pi.stop()
