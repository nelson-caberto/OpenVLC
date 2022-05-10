from flask import Flask, jsonify, render_template
from txvr import txvr

app = Flask(__name__)
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
