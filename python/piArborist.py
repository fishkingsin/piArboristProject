#!/usr/bin/env python
import time
import os
import RPi.GPIO as GPIO
import threading,queue
import argparse
import socket

GPIO.setmode(GPIO.BCM)
DEBUG = 0
SOCKET = 0
DO_AVERAGE = 0
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

last_read = 0       # this keeps track of the last potentiometer value
tolerance = 10       # to keep from being jittery we'll only change
					# volume when the pot has moved more than 5 'counts'
#calculate average


def cal_average(_sample_list , _num_sample , _q ):
	sample_sum = 0;
	for i in range(_num_sample):
		sample_sum+=_sample_list[i];
	sample_average = sample_sum/_num_sample;
	_q.put(sample_average) 

def main():
	sleeptime = 0.02
	num_sample = 50
	sample_list=[0 for i in range(num_sample)]
	for i in range(num_sample):
		sample_list[i] = 0;
	sample_index = 0;
	parser = argparse.ArgumentParser()
	parser.add_argument('-i', '--ip', help="host ip", default="127.0.0.1", type=str)
	parser.add_argument('-p', '--port', help="host port", default=5005, type=int)
	
	args = parser.parse_args()
	sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
	UDP_IP=args.ip
	UDP_PORT=args.port
	__average = 0
	print ("UDP target IP:", UDP_IP)
	print ("UDP target port:", UDP_PORT)
	while True:
		# we'll assume that the pot didn't move
		trim_pot_changed = False

		# read the analog pin
		trim_pot = readadc(potentiometer_adc, SPICLK, SPIMOSI, SPIMISO, SPICS)
		# how much has it changed since the last read?
		pot_adjust = abs(trim_pot - last_read)
		if DO_AVERAGE:
			sample_list[sample_index] = trim_pot;
			sample_index+=1
			if(sample_index==num_sample):
				q = queue.Queue()
				t = threading.Thread(target=cal_average, args = (sample_list , num_sample, q))
				t.daemon = True
				t.start()
				__average=round(q.get())
			
			# msg =  "average:%d,"%result
			# msg+="samples:"
			# if SOCKET:
				# for i in range(num_sample):
					# msg+="%d,"%sample_list[i];
				# sock.sendto(msg.encode(), (UDP_IP, UDP_PORT))
			# print(msg);

			sample_index %= num_sample;
		msg =  "%d"%trim_pot
		my_bytes = bytearray()
		
		my_bytes.append(trim_pot & 0xFF)
		my_bytes.append((trim_pot >> 8 )& 0xFF)
		
		sock.sendto(my_bytes, (UDP_IP, UDP_PORT))
		if DEBUG:
			print(msg)
		#         print "pot_adjust:", pot_adjust
		#         print "last_read", last_read

		if ( pot_adjust > tolerance ):
			   trim_pot_changed = True

		# if DEBUG:
		#         print "trim_pot_changed", trim_pot_changed

		
		# 	trim_pot = 0;
		# else:
		# 	trim_pot = 1000;

		if ( trim_pot_changed ):
			set_volume = trim_pot / 10.24           # convert 10bit adc0 (0-1024) trim pot read into 0-100 volume level
			set_volume = round(set_volume)          # round out decimal value
			set_volume = int(set_volume)            # cast volume as integer
			# if DEBUG:
				# print ('Volume = {volume}%' .format(volume = set_volume))
			# set_vol_cmd = 'sudo amixer cset numid=1 -- {volume}% > /dev/null' .format(volume = set_volume)
			# os.system(set_vol_cmd)  # set volume
			if(trim_pot>500):
				if DEBUG:
					print ("touched")
		#         if DEBUG:
		#                 print "set_volume", set_volume
		#                 print "tri_pot_changed", set_volume

		#         # save the potentiometer reading for the next loop
		#         last_read = trim_pot

		# hang out and do nothing for a half second
		time.sleep(sleeptime)
if __name__ == "__main__":

	main()