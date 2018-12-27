import machine
import neopixel
import random
import time

PIN = 16
LEDS = 20
COLOR = (42, 42, 42)
YELLOW_BUTTON_PIN = 13
BLUE_BUTTON_PIN = 12


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
        print("random color!")
        color = color if color else self.color
        for i in range(0, self.n):
            self.leds[i] = tuple([int(c * random.random() * 10) % 255
                                  for c in color])
        self.leds.write()

    def fade(self, step=8, times=5, color=None):
        color = color if color else self.color
        print("color is %s" % str(color))
        for i in range(0, times * 256, step):
            for j in range(self.n):
                if (i // 256) % 2 == 0:
                    val = i % 256
                else:
                    val = 255 - (i % 256)
                old_color = self.leds[j]
                self.leds[j] = (val, old_color[1], old_color[2])
            self.leds.write()


class Button(object):
    def __init__(self, pin, callback):
        self._pressed = False
        self.callback = callback
        self.pin = machine.Pin(pin, machine.Pin.OUT)

    def is_pressed(self):
        return self.pin.value == 1

    def action(self):
        if self.pin.value() == 1:
            # no action but register a click
            self._pressed = True
        elif self.pin.value() == 0 and self._pressed:
            self._pressed = False
            self.callback()


# XXX use args?
def run(pin=PIN, n=LEDS, color=COLOR):
    lc = LedControl(pin, n, color)
    lc.random_color()
    return lc


def listen(lc):
    button_yellow = Button(YELLOW_BUTTON_PIN, lc.random_color)
    button_blue = Button(BLUE_BUTTON_PIN, lc.fade)
    try:
        while True:
            button_yellow.action()
            button_blue.action()
            time.sleep(0.5)
    except KeyboardInterrupt:
        print("Finished listening")
        return
