from multiprocessing.reduction import DupFd
from queue import Queue
from threading import Thread
from time import sleep
from math import floor
import RPi.GPIO as GPIO
import board
import busio
import adafruit_ads1x15.ads1115 as ADS
from adafruit_ads1x15.analog_in import AnalogIn

i2c = busio.I2C(board.SCL, board.SDA)
ads = ADS.ADS1115(i2c)
RGBr = 17
RGBg = 27
RGBb = 22

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

GPIO.setup(RGBr,GPIO.OUT) #RED
GPIO.setup(RGBg,GPIO.OUT) #GREEN
GPIO.setup(RGBb,GPIO.OUT) #BLUE

class txvr:
    def __init__(self):
        self.rxP = -1
        self.rxByte = '00000000'
        self.rxByteIter = 0
        self.txrBitLen = 10
        self.msgs = []
        self.q = Queue()
        self.process_t = Thread(target=self.__process)
        self.process_t.start()

    def trimMsgs(self):
        if len(self.msgs)>5:
            self.msgs.pop(0)

    def add(self,data):
        self.msgs.append(data)
        self.trimMsgs()
        self.q.put(data)
        print('Adding Message: ',data)
        print('New Queue Size: ',self.q.qsize())
        
    def __readBit(self):
        value = floor(AnalogIn(ads, ADS.P0).voltage*100)/100
        if value == 0.47: return 0  #G 3740 0.47
        if value == 0.46: return 1  #B 3610 0.46
        if value == 0.49: return 2  #R 3890 0.49
        if value == 0.52: return 3  #RGB 4120 0.51
        return -1

    def __keepBit(self):
        if self.rx == 0:
            self.rxByte[self.rxByteIter] = 0
            self.rxByte[self.rxByteIter+1] = 0
        elif self.rx == 1:
            self.rxByte[self.rxByteIter] = 0
            self.rxByte[self.rxByteIter+1] = 1
        elif self.rx == 2:
            self.rxByte[self.rxByteIter] = 1
            self.rxByte[self.rxByteIter+1] = 0
        elif self.rx == 3:
            self.rxByte[self.rxByteIter] = 1
            self.rxByte[self.rxByteIter+1] = 1
        
        if self.rxByteIter == self.txrBitLen-2:
            self.msgs.append(int(f'0b{self.rxByte}',2))
            self.trimMsgs()
        self.rxByteIter += 2

    def __processRX(self):
        self.rx = self.__readBit()
        if self.rx != self.rxP:
            sleep(50/1000)
            self.rx = self.__readBit()
        if self.rx != self.rxP:
            if self.rx != -1:
                print('RX ',self.rx)
                self.__keepBit()
                # self.msgs.append(self.rx)
                # self.trimMsgs()
            self.rxP = self.rx
	
    def __ledOn(self):
        if   self.tx == 0: GPIO.output(RGBg,GPIO.HIGH)
        elif self.tx == 1: GPIO.output(RGBb,GPIO.HIGH)
        elif self.tx == 2: GPIO.output(RGBr,GPIO.HIGH)
        elif self.tx == 3:
            GPIO.output(RGBr,GPIO.HIGH)
            GPIO.output(RGBg,GPIO.HIGH)
            GPIO.output(RGBb,GPIO.HIGH)

    def __ledOff(self):
        if   self.tx == 0: GPIO.output(RGBg,GPIO.LOW)
        elif self.tx == 1: GPIO.output(RGBb,GPIO.LOW)
        elif self.tx == 2: GPIO.output(RGBr,GPIO.LOW)
        elif self.tx == 3:
            GPIO.output(RGBr,GPIO.LOW)
            GPIO.output(RGBg,GPIO.LOW)
            GPIO.output(RGBb,GPIO.LOW)

    def __sendBit(self):
        self.__ledOn()
        sleep(400/1000)
        self.__ledOff()
        sleep(400/1000)

    def __sendByte(self):
        msg = self.tx
        parity = 0
        for char in msg:
            bitS = '{0:08b}'.format(ord(char))
            for i in range(0,8,2):
                self.tx = int(bitS[i])+int(bitS[+1])
                parity += self.tx
                self.__sendBit()
            self.tx = parity % 2
            self.__sendBit()
            print("TX Data:",bitS,self.tx)

    def __processTX(self):
        # GPIO.output(RGBr,GPIO.HIGH) 
        # GPIO.output(RGBr,GPIO.LOW)
        if not self.q.empty():
            self.tx = self.q.get()
            self.__sendByte()
            print('TX ',self.tx)
            self.q.task_done()
            print('Remaining Queue Size: ',self.q.qsize())

    def __process(self):
        while True:
            self.__processRX()
            self.__processTX()
