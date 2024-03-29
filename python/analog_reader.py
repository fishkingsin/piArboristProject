#!/usr/bin/env python
import time
import os
import RPi.GPIO as GPIO
import threading
import socket
GPIO.setmode(GPIO.BCM)
DEBUG = 1

UDP_IP = "james.local"
UDP_PORT = 5005
MESSAGE = "Hello, World!"

# read SPI data from MCP3008 chip, 8 possible adc's (0 thru 7)
def readadc(adcnum, clockpin, mosipin, misopin, cspin):
		if ((adcnum > 7) or (adcnum < 0)):
				return -1
		GPIO.output(cspin, True)

		GPIO.output(clockpin, False)  # start clock low
		GPIO.output(cspin, False)     # bring CS low

		commandout = adcnum
		commandout |= 0x18  # start bit + single-ended bit
		commandout <<= 3    # we only need to send 5 bits here
		for i in range(5):
				if (commandout & 0x80):
						GPIO.output(mosipin, True)
				else:
						GPIO.output(mosipin, False)
				commandout <<= 1
				GPIO.output(clockpin, True)
				GPIO.output(clockpin, False)

		adcout = 0
		# read in one empty bit, one null bit and 10 ADC bits
		for i in range(12):
				GPIO.output(clockpin, True)
				GPIO.output(clockpin, False)
				adcout <<= 1
				if (GPIO.input(misopin)):
						adcout |= 0x1

		GPIO.output(cspin, True)
		
		adcout >>= 1       # first bit is 'null' so drop it
		return adcout

# change these as desired - they're the pins connected from the
# SPI port on the ADC to the Cobbler
SPICLK = 18
SPIMISO = 23
SPIMOSI = 24
SPICS = 25

# set up the SPI interface pins
GPIO.setup(SPIMOSI, GPIO.OUT)
GPIO.setup(SPIMISO, GPIO.IN)
GPIO.setup(SPICLK, GPIO.OUT)
GPIO.setup(SPICS, GPIO.OUT)

# 10k trim pot connected to adc #0
potentiometer_adc = 0;
num_sample = 20
sample_list=[0 for i in range(num_sample)]
for i in range(num_sample):
	sample_list[i] = 0;
sample_index = 0;
last_read = 0       # this keeps track of the last potentiometer value
tolerance = 5       # to keep from being jittery we'll only change
					# volume when the pot has moved more than 5 'counts'

#udp
sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP

def cal_average(_sample_list , _num_sample):
	sample_sum = 0;
	for i in range(_num_sample):
		sample_sum+=_sample_list[i];
	sample_average = sample_sum/_num_sample;
	if DEBUG:
		msg = ("sample_average : %s" % sample_average)
		print (msg);
		sock.sendto(msg, (UDP_IP, UDP_PORT))

while True:
	# we'll assume that the pot didn't move
	trim_pot_changed = False

	# read the analog pin
	trim_pot = readadc(potentiometer_adc, SPICLK, SPIMOSI, SPIMISO, SPICS)
	# how much has it changed since the last read?
	pot_adjust = abs(trim_pot - last_read)
	sample_list[sample_index] = trim_pot;
	sample_index+=1
	if(sample_index==num_sample):
		t = threading.Thread(target=cal_average, args = (sample_list , num_sample))
		t.daemon = True
		t.start()
		sample_index %= num_sample;
	if DEBUG:
		msg = ("trim_pot : %s" %trim_pot)
		sock.sendto(msg, (UDP_IP, UDP_PORT))
		print (msg);
		# print "pot_adjust:", pot_adjust
		# print "last_read", last_read
	if( pot_adjust > tolerance ):
		   trim_pot_changed = True
	if( trim_pot_changed ):
			last_read = trim_pot

	# hang out and do nothing for a half second
	time.sleep(0.01)
