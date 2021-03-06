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
Sheet 1 3
Title "TOP"
Date "2016-03-02"
Rev "3"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L udoo U?
U 1 1 56E6D858
P 8625 3500
F 0 "U?" H 8625 3400 60  0000 C CNN
F 1 "udoo" H 8625 3500 60  0000 C CNN
F 2 "" H 8625 3500 60  0000 C CNN
F 3 "" H 8625 3500 60  0000 C CNN
	1    8625 3500
	1    0    0    -1  
$EndComp
$Sheet
S 2125 2650 1600 1200
U 56EC17F9
F0 "DIODE_ACCESS" 60
F1 "file56EC17F8.sch" 60
F2 "Vss" I R 3725 2850 60 
F3 "GND" I R 3725 3025 60 
F4 "A0" I R 3725 3175 60 
F5 "A1" I R 3725 3300 60 
F6 "A2" I R 3725 3425 60 
F7 "A3" I R 3725 3550 60 
F8 "A4" I R 3725 3675 60 
F9 "D" O R 3725 3800 60 
$EndSheet
$Sheet
S 2100 4225 1625 1000
U 56EC65E8
F0 "MOTOR_ACCESS" 60
F1 "file56EC65E7.sch" 60
F2 "GND" I R 3725 4375 60 
F3 "Vs" I R 3725 4550 60 
F4 "Vss" I R 3725 4675 60 
F5 "SENSE" I R 3725 4825 60 
F6 "V1" I R 3725 4950 60 
F7 "V2" I R 3725 5075 60 
$EndSheet
$Comp
L BREAK_BEAM_DIODE U?
U 1 1 56EC6B3A
P 2825 6225
F 0 "U?" H 2825 6125 60  0000 C CNN
F 1 "BREAK_BEAM_DIODE" H 2825 6225 60  0000 C CNN
F 2 "" H 2825 6225 60  0000 C CNN
F 3 "" H 2825 6225 60  0000 C CNN
	1    2825 6225
	1    0    0    -1  
$EndComp
$Comp
L END_OF_LINE_SENSOR U?
U 1 1 56EC95BF
P 2775 1425
F 0 "U?" H 2775 1275 60  0000 C CNN
F 1 "END_OF_LINE_SENSOR" H 2775 1425 60  0000 C CNN
F 2 "" H 2775 1425 60  0000 C CNN
F 3 "" H 2775 1425 60  0000 C CNN
	1    2775 1425
	1    0    0    -1  
$EndComp
$Comp
L END_OF_LINE_SENSOR U?
U 1 1 56EC963D
P 2775 2225
F 0 "U?" H 2775 2075 60  0000 C CNN
F 1 "END_OF_LINE_SENSOR" H 2775 2225 60  0000 C CNN
F 2 "" H 2775 2225 60  0000 C CNN
F 3 "" H 2775 2225 60  0000 C CNN
	1    2775 2225
	1    0    0    -1  
$EndComp
$Comp
L POWER_SUPPLY U?
U 1 1 56ECB71D
P 10375 2450
F 0 "U?" H 10375 2300 60  0000 C CNN
F 1 "POWER_SUPPLY" H 10375 2450 60  0000 C CNN
F 2 "" H 10375 2450 60  0000 C CNN
F 3 "" H 10375 2450 60  0000 C CNN
	1    10375 2450
	1    0    0    -1  
$EndComp
$Comp
L SPST SW?
U 1 1 56ECB8B0
P 10275 1325
F 0 "SW?" H 10275 1425 50  0000 C CNN
F 1 "SPST" H 10275 1225 50  0000 C CNN
F 2 "" H 10275 1325 50  0000 C CNN
F 3 "" H 10275 1325 50  0000 C CNN
	1    10275 1325
	1    0    0    -1  
$EndComp
Wire Wire Line
	7525 2500 4600 2500
Wire Wire Line
	4600 2500 4600 3175
Wire Wire Line
	4600 3175 3725 3175
Wire Wire Line
	3725 3300 4650 3300
Wire Wire Line
	4650 3300 4650 2600
Wire Wire Line
	4650 2600 7525 2600
Wire Wire Line
	7525 2700 4700 2700
Wire Wire Line
	4700 2700 4700 3425
Wire Wire Line
	4700 3425 3725 3425
Wire Wire Line
	3725 3550 4750 3550
Wire Wire Line
	4750 3550 4750 2800
Wire Wire Line
	4750 2800 7525 2800
Wire Wire Line
	7525 2900 4800 2900
Wire Wire Line
	4800 2900 4800 3675
Wire Wire Line
	4800 3675 3725 3675
Wire Wire Line
	3750 3825 4850 3825
Wire Wire Line
	4850 3825 4850 3000
Wire Wire Line
	4850 3000 7525 3000
Wire Wire Line
	3725 4850 6275 4850
Wire Wire Line
	6275 4850 6275 3550
Wire Wire Line
	6275 3550 7525 3550
Wire Wire Line
	3725 4950 6325 4950
Wire Wire Line
	6325 4950 6325 3650
Wire Wire Line
	6325 3650 7525 3650
Wire Wire Line
	3725 5075 6375 5075
Wire Wire Line
	6375 5075 6375 3750
Wire Wire Line
	6375 3750 7525 3750
Wire Wire Line
	6425 6325 3725 6325
Wire Wire Line
	6425 3100 6425 6325
Wire Wire Line
	6425 3850 7525 3850
Wire Wire Line
	6425 3100 7525 3100
Connection ~ 6425 3850
Wire Wire Line
	3725 3025 4225 3025
Wire Wire Line
	4225 2000 4225 6225
Wire Wire Line
	4225 2000 7525 2000
Wire Wire Line
	4225 4375 3725 4375
Connection ~ 4225 3025
Wire Wire Line
	4225 6225 3725 6225
Connection ~ 4225 4375
Wire Wire Line
	4175 6125 3725 6125
Wire Wire Line
	4175 2300 4175 6125
Wire Wire Line
	4175 4675 3725 4675
Wire Wire Line
	4175 2850 3725 2850
Connection ~ 4175 4675
Wire Wire Line
	4175 2300 7525 2300
Connection ~ 4175 2850
Wire Wire Line
	3725 4550 4125 4550
Wire Wire Line
	4125 4550 4125 2150
Wire Wire Line
	4125 2150 7525 2150
Wire Wire Line
	2175 1175 2175 2475
Wire Wire Line
	2175 2475 4175 2475
Connection ~ 4175 2475
Connection ~ 2175 1975
Wire Wire Line
	3325 1975 3475 1975
Wire Wire Line
	3475 1975 3475 1350
Wire Wire Line
	3475 1350 6900 1350
Wire Wire Line
	6900 1350 6900 3950
Wire Wire Line
	6900 3950 7525 3950
Wire Wire Line
	7525 4050 6950 4050
Wire Wire Line
	6950 4050 6950 1175
Wire Wire Line
	6950 1175 3325 1175
Wire Wire Line
	9525 2550 9375 2550
Wire Wire Line
	9375 2550 9375 1275
Wire Wire Line
	9375 1275 8075 1275
Wire Wire Line
	8075 1275 8075 1500
Wire Wire Line
	9525 2100 9425 2100
Wire Wire Line
	9425 2100 9425 1325
Wire Wire Line
	8225 1325 9775 1325
Wire Wire Line
	8225 1325 8225 1500
Connection ~ 9425 1325
Wire Wire Line
	10775 1325 10975 1325
Wire Wire Line
	10975 1325 10975 1500
Wire Wire Line
	10975 1500 9525 1500
Wire Wire Line
	9525 1500 9525 1950
$EndSCHEMATC
