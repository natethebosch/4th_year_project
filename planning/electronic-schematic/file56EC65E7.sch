EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:capstone-lib
LIBS:electronic-schematic-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 3
Title "Motor Control"
Date "2016-03-01"
Rev "1"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MOTOR U?
U 1 1 56EC668E
P 3300 4175
F 0 "U?" H 3300 4075 60  0000 C CNN
F 1 "MOTOR" H 3300 4175 60  0000 C CNN
F 2 "" H 3300 4075 60  0000 C CNN
F 3 "" H 3300 4075 60  0000 C CNN
	1    3300 4175
	1    0    0    -1  
$EndComp
$Comp
L L298 L?
U 1 1 56EC66AB
P 5475 2900
F 0 "L?" H 5225 2800 60  0000 C CNN
F 1 "L298" H 5225 2900 60  0000 C CNN
F 2 "" H 5475 2900 60  0000 C CNN
F 3 "" H 5475 2900 60  0000 C CNN
	1    5475 2900
	0    1    1    0   
$EndComp
Wire Wire Line
	4775 3400 4775 4025
Wire Wire Line
	4775 4025 3750 4025
Wire Wire Line
	4875 3400 4875 4375
Wire Wire Line
	4875 4375 3750 4375
Text HLabel 7575 3800 2    60   Input ~ 0
GND
Text HLabel 7575 3950 2    60   Input ~ 0
Vs
Text HLabel 7575 4100 2    60   Input ~ 0
Vss
Text HLabel 7575 4250 2    60   Input ~ 0
SENSE
Text HLabel 7575 4400 2    60   Input ~ 0
V1
Text HLabel 7575 4525 2    60   Input ~ 0
V2
Wire Wire Line
	7575 3800 5375 3800
Wire Wire Line
	5375 3800 5375 3400
Wire Wire Line
	7575 3950 4975 3950
Wire Wire Line
	4975 3950 4975 3400
Wire Wire Line
	7575 4100 5475 4100
Wire Wire Line
	5475 4100 5475 3400
Wire Wire Line
	7575 4250 4675 4250
Wire Wire Line
	4675 4250 4675 3400
Wire Wire Line
	7575 4400 5075 4400
Wire Wire Line
	5075 4400 5075 3400
Wire Wire Line
	7575 4525 5175 4525
Wire Wire Line
	5175 4525 5175 3400
$EndSCHEMATC
