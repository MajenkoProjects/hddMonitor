#include <Wire.h>
#include <TLC59116.h>

// These two lines will stop and then restart the UDP server on a linux machine
// if you are using the absolute latest version of UECIDE.
#pragma parameter upload.precmd=sudo /etc/init.d/hddmonitor stop
#pragma parameter upload.postcmd=sudo /etc/init.d/hddmonitor start

// Define more boards the same as this with a different address.
TLC59116 board0(0);

// Add the boards to this array
TLC59116 *boards[16] = {
	&board0, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL
};

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

// Each entry is defined as per the structure just above.  LED numbers (*_r and *_w) are
// comprised of two parts - the first nibble is the board number, and the second is the
// channel number on that board.
struct drive drives[] = {
	{0, 0, 0x03, 0x04, 0x05, 0x00, 0x01, 0x02},
	{0, 0, 0x0C, 0x0B, 0x0A, 0x0F, 0x0E, 0x0D},
	{0, 0, 0x06, 0x07, 0x08, 0xF9, 0xF9, 0xF9},
};

#define NDRIVES (sizeof(drives) / sizeof(struct drive))

void setup() {
	// Start any defined boards ...
	for (uint8_t i = 0; i < 16; i++) {
		if (boards[i] != NULL) {
			boards[i]->begin();
		}
	}
	
	// ... as well as the serial port
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
			if (dr < NDRIVES) {
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

		for (int i = 0; i < NDRIVES; i++) {
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

	uint8_t br = r >> 4;
	r &= 0x0F;

	uint8_t bg = g >> 4;
	g &= 0x0F;

	uint8_t bb = b >> 4;
	b &= 0x0F;

	if (boards[br] == NULL) return;
	if (boards[bg] == NULL) return;
	if (boards[bb] == NULL) return;

	boards[br]->analogWrite(r, red);
	boards[bg]->analogWrite(g, green);
	boards[bb]->analogWrite(b, blue);

}
