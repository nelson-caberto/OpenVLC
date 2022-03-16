from queue import Queue
from threading import Thread
from time import sleep
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
	
	def __processRX(self):
        # chan = AnalogIn(ads, ADS.P0)
		pass
	
	def __processTX(self):
        # GPIO.output(RGBr,GPIO.HIGH) 
        # GPIO.output(RGBr,GPIO.LOW)
		if not self.q.empty():
			tx = self.q.get()
			print('TX ',tx)
			self.q.task_done()
			print('Remaining Queue Size: ',self.q.qsize())
	
	def __process(self):
		while True:
			self.__processRX()
			self.__processTX()
			sleep(4)
