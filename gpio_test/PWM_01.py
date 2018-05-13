# https://qiita.com/s417-lama/items/0ef64a7af3fcf6a56cc5


import pigpio
import time

gpio_pin0 = 21
gpio_pin1 = 19

#duty比 0~1の範囲で指定
duty_ratio = 0.5

pi = pigpio.pi()
pi.set_mode(gpio_pin0, pigpio.OUTPUT)
pi.set_mode(gpio_pin1, pigpio.OUTPUT)

# 飛行時duty比 duty_ratio
pi.hardware_PWM(gpio_pin0, 2, int(duty_ratio * 1000000))

time.sleep(10)

#duty比を線形に0まで落とす
while duty_ratio > 0:
    duty_ratio = duty_ratio - 0.01
    pi.hardware_PWM(gpio_pin0, 2,  int(duty_ratio * 1000000))
    time.sleep(0.1)

pi.set_mode(gpio_pin0, pigpio.INPUT)
pi.set_mode(gpio_pin1, pigpio.INPUT)
pi.stop()
