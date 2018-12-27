import machine
import neopixel
import random

PIN = 16
LEDS = 20
COLOR = (42, 42, 42)


class LedControl(object):
    def __init__(self, pin, n, color):
        super(LedControl, self).__init__()
        self.pin = machine.Pin(pin)
        # XXX squash with pin definition?
        self.pwm = machine.PWM(self.pin)
        self.n = n
        self.leds = neopixel.NeoPixel(self.pin, self.n)
        self.color = color

    def random_color(self, color=None):
        color = color if color else self.color
        for i in range(0, self.n):
            self.leds[i] = tuple([int(c * random.random() * 10) % 255
                                  for c in color])
        self.leds.write()

    def fade(self, step=8, times=5, color=None):
        color = color if color else self.color
        for i in range(0, times * 256, step):
            for j in range(self.n):
                if (i // 256) % 2 == 0:
                    val = i % 256
                else:
                    val = 255 - (i % 256)
                old_color = self.leds[j]
                self.leds[j] = (val, old_color[1], old_color[2])
            self.leds.write()


# XXX use args?
def run(pin=PIN, n=LEDS, color=COLOR):
    lc = LedControl(pin, n, color)
    lc.random_color()
    return lc
