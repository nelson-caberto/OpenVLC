from flask import Flask, jsonify, render_template
from queue import Queue
from threading import Thread
from time import sleep

app = Flask(__name__)

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
		pass
	
	def __processTX(self):
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
			
msgs = txvr()

@app.route('/')
def root():
	return render_template('index.html')
	
@app.route('/add/<msg>')
def add(msg):
	msgs.add(msg)
	return ''

@app.route('/msgs')
def getMsgs():
	return jsonify(msgs.msgs)
	
@app.route('/favicon.ico')
def nothing():
	return ''

if __name__ == "__main__":
	app.run(debug=True,host="0.0.0.0")
