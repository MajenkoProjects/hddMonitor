#include <Wire.h>
#include <TLC59116.h>

#pragma parameter upload.precmd=sudo /etc/init.d/hddmonitor-udpserver stop
#pragma parameter upload.postcmd=sudo /etc/init.d/hddmonitor-udpserver start

TLC59116 board1(0);

struct drive {
	uint32_t readValue;
	uint32_t writeValue; 
	uint8_t red_r;
	uint8_t green_r;
	uint8_t blue_r;
	uint8_t red_w;
	uint8_t green_w;
	uint8_t blue_w;
};

struct drive drives[2] = {
	{0, 0, 0, 1, 2, 15, 14, 13},
	{0, 0, 3, 4, 5, 12, 11, 10}
};

void setup() {
	board1.begin();
	Serial.begin(9600);
}

void loop() {
	static uint32_t readVal = 0;
	static uint32_t writeVal = 0;
	static char buffer[20];
	static int pos = 0;
	static uint32_t ts = millis();
	
	if (Serial.available()) {
		int c = Serial.read();
		if (c == '\n') {
			int dr = -1;
			pos = 0;
			char *tok;
			tok = strtok(buffer, " \n\r\t");
			dr = atoi(tok);
			if (dr != -1) {
				tok = strtok(NULL, " \n\r\t");			
				uint32_t rv = strtoul(tok, NULL, 10);
				tok = strtok(NULL, " \n\r\t");
				uint32_t wv = strtoul(tok, NULL, 10);
	
				if (rv > drives[dr].readValue) {
					drives[dr].readValue = rv;
				}
				if (wv > drives[dr].writeValue) {
					drives[dr].writeValue = wv;
				}
				if (drives[dr].readValue > 255*5) {
					drives[dr].readValue = 255*5;
				}
				if (drives[dr].writeValue > 255*5) {
					drives[dr].writeValue = 255*5;
				}
			}
		} else {
			buffer[pos++] = c;
			buffer[pos] = 0;
		}
	}

	if (millis() - ts > 0) {
		ts = millis();

		for (int i = 0; i < 2; i++) {
			setColor(drives[i].readValue, drives[i].red_r, drives[i].green_r, drives[i].blue_r);
			setColor(drives[i].writeValue, drives[i].red_w, drives[i].green_w, drives[i].blue_w);
	
			if (drives[i].readValue > 0) {
				drives[i].readValue/=1.03;
			}
	
			if (drives[i].writeValue > 0) {
				drives[i].writeValue/=1.03;
			}
		}
	}
}

void setColor(uint32_t val, uint8_t r, uint8_t g, uint8_t b) {
	uint8_t red = 0;
	uint8_t green = 0;
	uint8_t blue= 0;

	if (val > 255) {
		red = 255;
		val -= 255;
	} else {
		red = val;
		val = 0;
	}

	if (val > 255) {
		green = 255;
		val -= 255;
	} else {
		green = val;
		val = 0;
	}

	if (val > 255) {
		blue = 255;
		val -= 255;
	} else {
		blue = val;
		val = 0;
	}

	if (val > 255) {
		red = 0;
		val -= 255;
	} else {
		red -= val;
		val = 0;
	}

	if (val > 255) {
		green = 0;
		val -= 255;
	} else {
		green -= val;
		val = 0;
	}

	board1.analogWrite(r, red);
	board1.analogWrite(g, green);
	board1.analogWrite(b, blue);
}
