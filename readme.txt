modes: cold, water(dehumidify - water droplet), fan(fan only), heat, smtn(the temp sign)
fan direction: 0(up), 1, 2, 3, 4(down), swing
fan speed: 0(auto?), 1, 3, 5


decoded:
binary:
byte 0: 11000100
byte 1: 11010011
byte 2: 01100100
byte 3: 10000000
byte 4: 00000000
byte 5:
	on:  00100100
	off: 00000100
byte 6: 
	hot:  10000000
	cold: 11000000
byte 7: 
	16C:   11110000
	31C:   00000000
	other: 00010000
byte 8: 
first 3 bits: fan speed:
	fan 0: 000
	fan 1: 010
	fan 3: 110
	fan 5: 101
next 3 bits: fan direction	
	dir 0: 100
	dir 1: 010
	dir 2: 110
	dir 3: 001
	dir 4: 101
	dir swing: 111
last 2 bits: dont know
	always: 00
byte 9-12: 00000000
byte 13: CRC: computed as: 
        reverse(big endian) all bytes before and add them together - cut the overflow away.
        then reverse the calculated CRC

