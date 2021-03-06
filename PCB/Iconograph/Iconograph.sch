EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L RF_Module:E73-2G4M04S-52832 U1
U 1 1 609F8E5B
P 3750 4300
F 0 "U1" H 3850 2750 50  0000 C CNN
F 1 "E73-2G4M04S-52832 RF Module" H 4450 2650 50  0000 C CNN
F 2 "RF_Module:E73-2G4M04S" H 3750 4750 50  0001 C CNN
F 3 "http://www.cdebyte.com/en/downpdf.aspx?id=243" H 3750 4750 50  0001 C CNN
	1    3750 4300
	1    0    0    -1  
$EndComp
$Comp
L Connector:USB_B_Micro J2
U 1 1 609FB09E
P 2900 1650
F 0 "J2" H 2957 2117 50  0000 C CNN
F 1 "USB_B_Micro" H 2957 2026 50  0000 C CNN
F 2 "" H 3050 1600 50  0001 C CNN
F 3 "~" H 3050 1600 50  0001 C CNN
	1    2900 1650
	1    0    0    -1  
$EndComp
$Comp
L Motor:Motor_Servo M1
U 1 1 609FD77F
P 6350 2600
F 0 "M1" H 6682 2665 50  0000 L CNN
F 1 "Motor_Servo" H 6682 2574 50  0000 L CNN
F 2 "" H 6350 2410 50  0001 C CNN
F 3 "http://forums.parallax.com/uploads/attachments/46831/74481.png" H 6350 2410 50  0001 C CNN
	1    6350 2600
	1    0    0    -1  
$EndComp
$Comp
L Motor:Motor_Servo M2
U 1 1 609FF22D
P 6350 3150
F 0 "M2" H 6682 3215 50  0000 L CNN
F 1 "Motor_Servo" H 6682 3124 50  0000 L CNN
F 2 "" H 6350 2960 50  0001 C CNN
F 3 "http://forums.parallax.com/uploads/attachments/46831/74481.png" H 6350 2960 50  0001 C CNN
	1    6350 3150
	1    0    0    -1  
$EndComp
$Comp
L Motor:Motor_Servo M3
U 1 1 60A00243
P 6350 3750
F 0 "M3" H 6682 3815 50  0000 L CNN
F 1 "Motor_Servo" H 6682 3724 50  0000 L CNN
F 2 "" H 6350 3560 50  0001 C CNN
F 3 "http://forums.parallax.com/uploads/attachments/46831/74481.png" H 6350 3560 50  0001 C CNN
	1    6350 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 2700 5800 2700
Wire Wire Line
	5800 2700 5800 3250
Wire Wire Line
	6050 3250 5800 3250
Connection ~ 5800 3250
Wire Wire Line
	5800 3250 5800 3850
Wire Wire Line
	6050 3850 5800 3850
Connection ~ 5800 3850
Wire Wire Line
	5800 3850 5800 4250
$Comp
L power:GND #PWR08
U 1 1 60A03BEA
P 5800 4250
F 0 "#PWR08" H 5800 4000 50  0001 C CNN
F 1 "GND" H 5805 4077 50  0000 C CNN
F 2 "" H 5800 4250 50  0001 C CNN
F 3 "" H 5800 4250 50  0001 C CNN
	1    5800 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 3750 5950 3750
Wire Wire Line
	5950 3750 5950 3150
Wire Wire Line
	6050 2600 5950 2600
Connection ~ 5950 2600
Wire Wire Line
	5950 2600 5950 2200
Wire Wire Line
	6050 3150 5950 3150
Connection ~ 5950 3150
Wire Wire Line
	5950 3150 5950 2600
$Comp
L power:+5V #PWR09
U 1 1 60A06A9A
P 5950 2200
F 0 "#PWR09" H 5950 2050 50  0001 C CNN
F 1 "+5V" H 5965 2373 50  0000 C CNN
F 2 "" H 5950 2200 50  0001 C CNN
F 3 "" H 5950 2200 50  0001 C CNN
	1    5950 2200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 60A082EF
P 3750 6200
F 0 "#PWR05" H 3750 5950 50  0001 C CNN
F 1 "GND" H 3755 6027 50  0000 C CNN
F 2 "" H 3750 6200 50  0001 C CNN
F 3 "" H 3750 6200 50  0001 C CNN
	1    3750 6200
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR03
U 1 1 60A1A0C9
P 3300 1350
F 0 "#PWR03" H 3300 1200 50  0001 C CNN
F 1 "+5V" H 3315 1523 50  0000 C CNN
F 2 "" H 3300 1350 50  0001 C CNN
F 3 "" H 3300 1350 50  0001 C CNN
	1    3300 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 1450 3300 1450
Wire Wire Line
	3300 1450 3300 1350
Wire Wire Line
	3750 5900 3750 6200
Wire Wire Line
	4650 3100 5000 3100
Wire Wire Line
	5000 3100 5000 2500
Wire Wire Line
	5000 2500 6050 2500
Wire Wire Line
	4650 3200 5100 3200
Wire Wire Line
	5100 3200 5100 3050
Wire Wire Line
	5100 3050 6050 3050
Wire Wire Line
	4650 3300 5100 3300
Wire Wire Line
	5100 3300 5100 3650
Wire Wire Line
	5100 3650 6050 3650
$Comp
L Regulator_Linear:MCP1703A-3302_SOT89 U2
U 1 1 60A255D1
P 4100 1450
F 0 "U2" H 4100 1692 50  0000 C CNN
F 1 "Generic 3.3V LDO" H 4100 1601 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-89-3" H 4100 1650 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/20005122B.pdf" H 4100 1400 50  0001 C CNN
	1    4100 1450
	1    0    0    -1  
$EndComp
Connection ~ 3300 1450
Wire Wire Line
	4400 1450 4550 1450
$Comp
L power:+3V3 #PWR07
U 1 1 60A27DAB
P 4750 1450
F 0 "#PWR07" H 4750 1300 50  0001 C CNN
F 1 "+3V3" H 4765 1623 50  0000 C CNN
F 2 "" H 4750 1450 50  0001 C CNN
F 3 "" H 4750 1450 50  0001 C CNN
	1    4750 1450
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR04
U 1 1 60A2A8AD
P 3750 2550
F 0 "#PWR04" H 3750 2400 50  0001 C CNN
F 1 "+3V3" H 3765 2723 50  0000 C CNN
F 2 "" H 3750 2550 50  0001 C CNN
F 3 "" H 3750 2550 50  0001 C CNN
	1    3750 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 2550 3750 2700
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J1
U 1 1 60A32551
P 1850 5400
F 0 "J1" H 1900 5817 50  0000 C CNN
F 1 "2x5 1.27mm" H 1900 5726 50  0000 C CNN
F 2 "" H 1850 5400 50  0001 C CNN
F 3 "~" H 1850 5400 50  0001 C CNN
	1    1850 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 5200 1400 5200
Wire Wire Line
	1400 5200 1400 5050
$Comp
L power:+3V3 #PWR01
U 1 1 60A514BE
P 1400 5050
F 0 "#PWR01" H 1400 4900 50  0001 C CNN
F 1 "+3V3" H 1415 5223 50  0000 C CNN
F 2 "" H 1400 5050 50  0001 C CNN
F 3 "" H 1400 5050 50  0001 C CNN
	1    1400 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 5300 1400 5300
Wire Wire Line
	1400 5300 1400 5400
Wire Wire Line
	1650 5400 1400 5400
Connection ~ 1400 5400
Wire Wire Line
	1400 5400 1400 5800
Wire Wire Line
	2150 5600 2700 5600
Wire Wire Line
	2700 5600 2700 4800
Wire Wire Line
	2700 4800 2850 4800
Wire Wire Line
	2150 5200 2300 5200
Wire Wire Line
	2300 5200 2300 5000
Wire Wire Line
	2300 5000 2850 5000
Wire Wire Line
	2150 5300 2400 5300
$Comp
L power:GND #PWR02
U 1 1 60A58BC4
P 1400 5800
F 0 "#PWR02" H 1400 5550 50  0001 C CNN
F 1 "GND" H 1405 5627 50  0000 C CNN
F 2 "" H 1400 5800 50  0001 C CNN
F 3 "" H 1400 5800 50  0001 C CNN
	1    1400 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 4900 2850 4900
Wire Wire Line
	2400 5300 2400 4900
$Comp
L power:GND #PWR06
U 1 1 60A61908
P 4550 2100
F 0 "#PWR06" H 4550 1850 50  0001 C CNN
F 1 "GND" H 4555 1927 50  0000 C CNN
F 2 "" H 4550 2100 50  0001 C CNN
F 3 "" H 4550 2100 50  0001 C CNN
	1    4550 2100
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C1
U 1 1 60A62127
P 3600 1700
F 0 "C1" H 3718 1746 50  0000 L CNN
F 1 "4.7uF" H 3718 1655 50  0000 L CNN
F 2 "" H 3638 1550 50  0001 C CNN
F 3 "~" H 3600 1700 50  0001 C CNN
	1    3600 1700
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C2
U 1 1 60A63286
P 4550 1700
F 0 "C2" H 4668 1746 50  0000 L CNN
F 1 "4.7uF" H 4668 1655 50  0000 L CNN
F 2 "" H 4588 1550 50  0001 C CNN
F 3 "~" H 4550 1700 50  0001 C CNN
	1    4550 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 1550 4550 1450
Connection ~ 4550 1450
Wire Wire Line
	4550 1450 4750 1450
Wire Wire Line
	3600 1550 3600 1450
Wire Wire Line
	3300 1450 3600 1450
Connection ~ 3600 1450
Wire Wire Line
	3600 1450 3800 1450
Wire Wire Line
	2900 2100 3600 2100
Wire Wire Line
	4550 2100 4550 1850
Connection ~ 4550 2100
Wire Wire Line
	2900 2100 2900 2050
Wire Wire Line
	3600 1850 3600 2100
Connection ~ 3600 2100
Wire Wire Line
	3600 2100 4100 2100
Wire Wire Line
	4100 1750 4100 2100
Connection ~ 4100 2100
Wire Wire Line
	4100 2100 4550 2100
Text Notes 7400 7500 0    71   ~ 0
Iconograph Controller by Adam Pierce <adam@siliconsparrow.com>
Text Notes 8150 7650 0    59   ~ 0
7-Jul-2021
$EndSCHEMATC
