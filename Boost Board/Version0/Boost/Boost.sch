EESchema Schematic File Version 4
LIBS:Boost-cache
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
L Device:C CIN2
U 1 1 5DB20FE7
P 1200 2150
F 0 "CIN2" H 1315 2196 50  0000 L CNN
F 1 "10uF" H 1315 2105 50  0000 L CNN
<<<<<<< HEAD
F 2 "Capacitor_SMD:CP_Elec_10x12.5" H 1238 2000 50  0001 C CNN
=======
F 2 "Capacitor_SMD:C_1210_3225Metric_Pad1.42x2.65mm_HandSolder" H 1238 2000 50  0001 C CNN
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
F 3 "~" H 1200 2150 50  0001 C CNN
	1    1200 2150
	1    0    0    -1  
$EndComp
$Comp
L Device:R Rt1
U 1 1 5DB22AEE
P 1800 2500
F 0 "Rt1" H 1870 2546 50  0000 L CNN
F 1 "140k" H 1870 2455 50  0000 L CNN
<<<<<<< HEAD
F 2 "Resistor_SMD:R_0603_1608Metric" V 1730 2500 50  0001 C CNN
=======
F 2 "Resistor_SMD:R_0402_1005Metric" V 1730 2500 50  0001 C CNN
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
F 3 "~" H 1800 2500 50  0001 C CNN
	1    1800 2500
	1    0    0    -1  
$EndComp
$Comp
L Device:R Rilim1
U 1 1 5DB23914
P 2150 2500
F 0 "Rilim1" H 2220 2546 50  0000 L CNN
F 1 "86.6k" H 2220 2455 50  0000 L CNN
<<<<<<< HEAD
F 2 "Resistor_SMD:R_0603_1608Metric" V 2080 2500 50  0001 C CNN
=======
F 2 "Resistor_SMD:R_0402_1005Metric" V 2080 2500 50  0001 C CNN
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
F 3 "~" H 2150 2500 50  0001 C CNN
	1    2150 2500
	1    0    0    -1  
$EndComp
$Comp
L Device:R Rcomp1
U 1 1 5DB242A7
P 2550 2600
F 0 "Rcomp1" H 2620 2646 50  0000 L CNN
F 1 "12.1k" H 2620 2555 50  0000 L CNN
<<<<<<< HEAD
F 2 "Resistor_SMD:R_0603_1608Metric" V 2480 2600 50  0001 C CNN
=======
F 2 "Resistor_SMD:R_0201_0603Metric" V 2480 2600 50  0001 C CNN
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
F 3 "~" H 2550 2600 50  0001 C CNN
	1    2550 2600
	1    0    0    -1  
$EndComp
$Comp
L Device:C Ccomp1
U 1 1 5DB249C6
P 2550 3100
F 0 "Ccomp1" H 2665 3146 50  0000 L CNN
F 1 "3.3nf" H 2665 3055 50  0000 L CNN
<<<<<<< HEAD
F 2 "Capacitor_SMD:CP_Elec_10x12.5" H 2588 2950 50  0001 C CNN
=======
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 2588 2950 50  0001 C CNN
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
F 3 "~" H 2550 3100 50  0001 C CNN
	1    2550 3100
	1    0    0    -1  
$EndComp
$Comp
L Device:L L1
U 1 1 5DB2501B
P 4000 700
F 0 "L1" V 4190 700 50  0000 C CNN
F 1 "2.7uH" V 4099 700 50  0000 C CNN
<<<<<<< HEAD
F 2 "Inductor_SMD:L_0805_2012Metric" H 4000 700 50  0001 C CNN
=======
F 2 "motherboard:SDR0403" H 4000 700 50  0001 C CNN
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
F 3 "~" H 4000 700 50  0001 C CNN
	1    4000 700 
	0    -1   -1   0   
$EndComp
$Comp
L Device:C Cboot1
U 1 1 5DB26BAF
P 4900 1300
F 0 "Cboot1" H 5015 1346 50  0000 L CNN
F 1 "100nF" H 5015 1255 50  0000 L CNN
<<<<<<< HEAD
F 2 "Capacitor_SMD:CP_Elec_10x12.5" H 4938 1150 50  0001 C CNN
=======
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 4938 1150 50  0001 C CNN
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
F 3 "~" H 4900 1300 50  0001 C CNN
	1    4900 1300
	1    0    0    -1  
$EndComp
$Comp
L Device:C Cout1
U 1 1 5DB27A87
P 5450 1750
F 0 "Cout1" H 5565 1796 50  0000 L CNN
F 1 "10u" H 5565 1705 50  0000 L CNN
<<<<<<< HEAD
F 2 "Capacitor_SMD:CP_Elec_10x12.5" H 5488 1600 50  0001 C CNN
=======
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 5488 1600 50  0001 C CNN
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
F 3 "~" H 5450 1750 50  0001 C CNN
	1    5450 1750
	1    0    0    -1  
$EndComp
$Comp
L Device:R Rgate1
U 1 1 5DB28F33
P 6050 1750
F 0 "Rgate1" H 6120 1796 50  0000 L CNN
F 1 "162k" H 6120 1705 50  0000 L CNN
<<<<<<< HEAD
F 2 "Resistor_SMD:R_0603_1608Metric" V 5980 1750 50  0001 C CNN
=======
F 2 "Resistor_SMD:R_0201_0603Metric" V 5980 1750 50  0001 C CNN
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
F 3 "~" H 6050 1750 50  0001 C CNN
	1    6050 1750
	1    0    0    -1  
$EndComp
$Comp
L Device:C Cgate1
U 1 1 5DB2955A
P 6650 1750
F 0 "Cgate1" H 6765 1796 50  0000 L CNN
F 1 "22n" H 6765 1705 50  0000 L CNN
<<<<<<< HEAD
F 2 "Capacitor_SMD:CP_Elec_10x12.5" H 6688 1600 50  0001 C CNN
=======
F 2 "Capacitor_SMD:C_0402_1005Metric" H 6688 1600 50  0001 C CNN
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
F 3 "~" H 6650 1750 50  0001 C CNN
	1    6650 1750
	1    0    0    -1  
$EndComp
$Comp
L Device:R Rfbt1
U 1 1 5DB2A227
P 7700 2200
F 0 "Rfbt1" H 7770 2246 50  0000 L CNN
F 1 "453k" H 7770 2155 50  0000 L CNN
<<<<<<< HEAD
F 2 "Resistor_SMD:R_0603_1608Metric" V 7630 2200 50  0001 C CNN
=======
F 2 "Resistor_SMD:R_0402_1005Metric" V 7630 2200 50  0001 C CNN
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
F 3 "~" H 7700 2200 50  0001 C CNN
	1    7700 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:R Rfbb1
U 1 1 5DB2B4C6
P 7700 3100
F 0 "Rfbb1" H 7770 3146 50  0000 L CNN
F 1 "86.6k" H 7770 3055 50  0000 L CNN
<<<<<<< HEAD
F 2 "Resistor_SMD:R_0603_1608Metric" V 7630 3100 50  0001 C CNN
=======
F 2 "Resistor_SMD:R_0201_0603Metric" V 7630 3100 50  0001 C CNN
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
F 3 "~" H 7700 3100 50  0001 C CNN
	1    7700 3100
	1    0    0    -1  
$EndComp
$Comp
L Device:C Coutx1
U 1 1 5DB2BF36
P 8400 2850
F 0 "Coutx1" H 8515 2896 50  0000 L CNN
F 1 "10u" H 8515 2805 50  0000 L CNN
<<<<<<< HEAD
F 2 "Capacitor_SMD:CP_Elec_10x12.5" H 8438 2700 50  0001 C CNN
=======
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 8438 2700 50  0001 C CNN
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
F 3 "~" H 8400 2850 50  0001 C CNN
	1    8400 2850
	1    0    0    -1  
$EndComp
$Comp
L Device:C Cout4
U 1 1 5DB2C65F
P 9200 2850
F 0 "Cout4" H 9315 2896 50  0000 L CNN
F 1 "10u" H 9315 2805 50  0000 L CNN
<<<<<<< HEAD
F 2 "Capacitor_SMD:CP_Elec_10x12.5" H 9238 2700 50  0001 C CNN
=======
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 9238 2700 50  0001 C CNN
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
F 3 "~" H 9200 2850 50  0001 C CNN
	1    9200 2850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5DB2EE9D
P 5450 2050
F 0 "#PWR0101" H 5450 1800 50  0001 C CNN
F 1 "GND" H 5455 1877 50  0000 C CNN
F 2 "" H 5450 2050 50  0001 C CNN
F 3 "" H 5450 2050 50  0001 C CNN
	1    5450 2050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5DB2F4ED
P 4250 3450
F 0 "#PWR0102" H 4250 3200 50  0001 C CNN
F 1 "GND" H 4255 3277 50  0000 C CNN
F 2 "" H 4250 3450 50  0001 C CNN
F 3 "" H 4250 3450 50  0001 C CNN
	1    4250 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 1900 5450 2050
Wire Wire Line
	5450 1600 5450 1500
Wire Wire Line
	5450 1500 6050 1500
Wire Wire Line
	6050 1500 6050 1600
Wire Wire Line
	6650 1600 6650 1500
Wire Wire Line
	6650 1500 6050 1500
Connection ~ 6050 1500
Wire Wire Line
	6050 1900 6050 2400
Wire Wire Line
	6050 2400 6650 2400
Wire Wire Line
	6650 2400 6650 1900
Connection ~ 6050 2400
Wire Wire Line
	4900 1150 4900 950 
Wire Wire Line
	4900 950  4350 950 
Wire Wire Line
	4350 950  4350 700 
Wire Wire Line
	4350 700  4150 700 
Wire Wire Line
	3850 700  3650 700 
Wire Wire Line
<<<<<<< HEAD
	3650 700  3650 1200
Wire Wire Line
=======
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
	1200 2000 1200 1900
Wire Wire Line
	2550 2750 2550 2950
Wire Wire Line
	2550 3250 2550 3450
Wire Wire Line
	1200 2300 1200 2400
Connection ~ 4250 3450
Wire Wire Line
	4250 3450 4400 3450
Wire Wire Line
	7700 3250 7700 3450
Wire Wire Line
	10300 3450 9200 3450
Connection ~ 7700 3450
Wire Wire Line
	9200 2700 9200 2600
Connection ~ 9200 1750
Wire Wire Line
	9200 1750 10300 1750
Wire Wire Line
	9200 3000 9200 3450
Connection ~ 9200 3450
Wire Wire Line
	9200 3450 8400 3450
Wire Wire Line
	7700 2050 7700 1750
Wire Wire Line
	7700 1750 8400 1750
$Comp
L TPS611781RNWR:TPS611781RNWR U1
U 1 1 5DB2CDEE
P 2950 1500
F 0 "U1" H 3700 1765 50  0000 C CNN
F 1 "TPS611781RNWR" H 3700 1674 50  0000 C CNN
F 2 "TPS611781RNWR:RNW0013A" H 4300 1600 50  0001 L CNN
F 3 "http://www.ti.com/lit/gpn/tps61178" H 4300 1500 50  0001 L CNN
F 4 "20-V Fully Integrated Sync Boost with Load Disconnect " H 4300 1400 50  0001 L CNN "Description"
F 5 "" H 4300 1300 50  0001 L CNN "Height"
F 6 "Texas Instruments" H 4300 1200 50  0001 L CNN "Manufacturer_Name"
F 7 "TPS611781RNWR" H 4300 1100 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "595-TPS611781RNWR" H 4300 1000 50  0001 L CNN "Mouser Part Number"
F 9 "https://www.mouser.com/Search/Refine.aspx?Keyword=595-TPS611781RNWR" H 4300 900 50  0001 L CNN "Mouser Price/Stock"
F 10 "" H 4300 800 50  0001 L CNN "RS Part Number"
F 11 "" H 4300 700 50  0001 L CNN "RS Price/Stock"
	1    2950 1500
	1    0    0    -1  
$EndComp
Connection ~ 2550 3450
Wire Wire Line
	2550 2450 2550 1800
Wire Wire Line
	2550 1800 2950 1800
Wire Wire Line
	2150 2350 2150 1700
Wire Wire Line
	2150 1700 2950 1700
Wire Wire Line
	1800 2350 1800 1500
Wire Wire Line
	1800 1500 2950 1500
Wire Wire Line
	2950 1600 2900 1600
Wire Wire Line
	2900 1600 2900 2000
Wire Wire Line
	2900 2250 4400 2250
Wire Wire Line
	4400 2250 4400 3450
Wire Wire Line
	2950 2000 2900 2000
Connection ~ 2900 2000
Wire Wire Line
	2900 2000 2900 2250
Wire Wire Line
<<<<<<< HEAD
	3500 1200 3500 1100
Wire Wire Line
	3500 1100 4550 1100
Wire Wire Line
	4550 1100 4550 1800
Wire Wire Line
	4550 1900 4450 1900
Connection ~ 3500 1200
Wire Wire Line
	3500 1200 3650 1200
Wire Wire Line
	4450 1800 4550 1800
Connection ~ 4550 1800
Wire Wire Line
	4550 1800 4550 1900
=======
	4550 1900 4450 1900
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
Wire Wire Line
	2800 1900 2950 1900
Wire Wire Line
	1800 2650 1800 3450
Connection ~ 1800 3450
Wire Wire Line
	1800 3450 2150 3450
Wire Wire Line
	2150 2650 2150 3450
Connection ~ 2150 3450
Wire Wire Line
	2150 3450 2550 3450
Wire Wire Line
	4900 1450 4900 2000
Wire Wire Line
	4900 2000 4450 2000
Wire Wire Line
	4600 1600 4450 1600
Wire Wire Line
	4600 2400 6050 2400
Wire Wire Line
	4450 1500 5450 1500
Connection ~ 5450 1500
Wire Wire Line
	4600 1600 4600 2400
Connection ~ 1200 1200
Wire Wire Line
<<<<<<< HEAD
	1200 1200 3500 1200
=======
	1200 1200 3650 1200
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
Wire Wire Line
	1200 3450 1550 3450
$Comp
L Device:C Cvcc1
U 1 1 5DB63E94
P 1550 3150
F 0 "Cvcc1" H 1665 3196 50  0000 L CNN
F 1 "4.7uf" H 1665 3105 50  0000 L CNN
<<<<<<< HEAD
F 2 "Capacitor_SMD:CP_Elec_10x12.5" H 1588 3000 50  0001 C CNN
=======
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 1588 3000 50  0001 C CNN
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
F 3 "~" H 1550 3150 50  0001 C CNN
	1    1550 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 3300 1550 3450
Connection ~ 1550 3450
Wire Wire Line
	1550 3450 1800 3450
Wire Wire Line
	1550 3000 1550 2800
Wire Wire Line
	1550 2800 4750 2800
Wire Wire Line
	4750 2800 4750 1700
Wire Wire Line
	4750 1700 4450 1700
Wire Wire Line
	7700 2350 7700 2650
Wire Wire Line
	2550 3450 4250 3450
<<<<<<< HEAD
Wire Wire Line
	10300 1750 10300 3450
=======
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
Connection ~ 7700 2650
Wire Wire Line
	7700 2650 7700 2950
Wire Wire Line
	3050 2650 3050 2350
Wire Wire Line
	3050 2350 2800 2350
Wire Wire Line
	2800 2350 2800 1900
Wire Wire Line
	4900 950  5250 950 
Wire Wire Line
	5250 950  5250 2550
Wire Wire Line
	5250 2550 2950 2550
Wire Wire Line
	2950 2550 2950 2100
Connection ~ 4900 950 
Wire Wire Line
	4400 3450 7700 3450
Connection ~ 4400 3450
Wire Wire Line
	3050 2650 7700 2650
Wire Wire Line
	7000 1500 6650 1500
Connection ~ 6650 1500
Wire Wire Line
	6650 2400 7200 2400
Wire Wire Line
	7200 2400 7200 1800
Connection ~ 6650 2400
Wire Wire Line
	7400 1500 7700 1500
Wire Wire Line
	7700 1500 7700 1750
Connection ~ 7700 1750
$Comp
L power:+5V #PWR0103
U 1 1 5DB98F57
P 950 1200
F 0 "#PWR0103" H 950 1050 50  0001 C CNN
F 1 "+5V" H 965 1373 50  0000 C CNN
F 2 "" H 950 1200 50  0001 C CNN
F 3 "" H 950 1200 50  0001 C CNN
	1    950  1200
	1    0    0    -1  
$EndComp
$Comp
L Device:C CIN1
U 1 1 5DBA2DF5
P 1200 1750
F 0 "CIN1" H 1315 1796 50  0000 L CNN
F 1 "10uF" H 1315 1705 50  0000 L CNN
<<<<<<< HEAD
F 2 "Capacitor_SMD:CP_Elec_10x12.5" H 1238 1600 50  0001 C CNN
=======
F 2 "Capacitor_SMD:C_1210_3225Metric_Pad1.42x2.65mm_HandSolder" H 1238 1600 50  0001 C CNN
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
F 3 "~" H 1200 1750 50  0001 C CNN
	1    1200 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 1600 1200 1200
$Comp
L Device:C CIN3
U 1 1 5DBA4B05
P 1200 2550
F 0 "CIN3" H 1315 2596 50  0000 L CNN
F 1 "10uF" H 1315 2505 50  0000 L CNN
<<<<<<< HEAD
F 2 "Capacitor_SMD:CP_Elec_10x12.5" H 1238 2400 50  0001 C CNN
=======
F 2 "Capacitor_SMD:C_1210_3225Metric_Pad1.42x2.65mm_HandSolder" H 1238 2400 50  0001 C CNN
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
F 3 "~" H 1200 2550 50  0001 C CNN
	1    1200 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 2700 1200 3450
$Comp
L Device:C Cout2
U 1 1 5DBA4FDE
P 9200 2050
F 0 "Cout2" H 9315 2096 50  0000 L CNN
F 1 "10u" H 9315 2005 50  0000 L CNN
<<<<<<< HEAD
F 2 "Capacitor_SMD:CP_Elec_10x12.5" H 9238 1900 50  0001 C CNN
=======
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 9238 1900 50  0001 C CNN
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
F 3 "~" H 9200 2050 50  0001 C CNN
	1    9200 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	9200 1900 9200 1750
$Comp
L Device:C Cout3
U 1 1 5DBA5426
P 9200 2450
F 0 "Cout3" H 9315 2496 50  0000 L CNN
F 1 "10u" H 9315 2405 50  0000 L CNN
<<<<<<< HEAD
F 2 "Capacitor_SMD:CP_Elec_10x12.5" H 9238 2300 50  0001 C CNN
=======
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 9238 2300 50  0001 C CNN
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
F 3 "~" H 9200 2450 50  0001 C CNN
	1    9200 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	9200 2300 9200 2200
Wire Wire Line
	8400 2700 8400 1750
Connection ~ 8400 1750
Wire Wire Line
	8400 1750 9200 1750
Wire Wire Line
	8400 3000 8400 3450
Connection ~ 8400 3450
Wire Wire Line
	8400 3450 7700 3450
$Comp
L Device:Q_PMOS_DGS Q1
U 1 1 5DBACDC6
P 7200 1600
F 0 "Q1" V 7543 1600 50  0000 C CNN
F 1 "Q_PMOS_DGS" V 7452 1600 50  0000 C CNN
<<<<<<< HEAD
F 2 "Package_DirectFET:DirectFET_ME" H 7400 1700 50  0001 C CNN
F 3 "~" H 7200 1600 50  0001 C CNN
	1    7200 1600
	0    -1   -1   0   
=======
F 2 "motherboard:Si6423DQ" H 7400 1700 50  0001 C CNN
F 3 "~" H 7200 1600 50  0001 C CNN
	1    7200 1600
	0    1    -1   0   
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
$EndComp
$Comp
L Header_Board-cache-2018-07-27-13-43-46:CONN_02X26 P2
U 1 1 5DBC06FD
P 3700 6350
F 0 "P2" H 3700 7815 50  0000 C CNN
F 1 "CONN_02X26" H 3700 7724 50  0000 C CNN
<<<<<<< HEAD
F 2 "" H 3700 5650 50  0001 C CNN
=======
F 2 "motherboard:CSK_Header" H 3700 5650 50  0001 C CNN
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
F 3 "" H 3700 5650 50  0000 C CNN
	1    3700 6350
	1    0    0    -1  
$EndComp
Text Label 3050 5100 0    50   ~ 0
SGPIO0
Text Label 3050 5200 0    50   ~ 0
SGPIO2
Text Label 3050 5300 0    50   ~ 0
I2C3_SDA
Text Label 3050 5400 0    50   ~ 0
SGPIO4
Text Label 4100 5100 0    50   ~ 0
SGPIO1
Text Label 4100 5200 0    50   ~ 0
SGPIO3
Text Label 4100 5300 0    50   ~ 0
I2C3_SCL
$Comp
L Header_Board-cache-2018-07-27-13-43-46:GND #PWR0104
U 1 1 5DCB9648
P 2900 5600
F 0 "#PWR0104" H 2900 5350 50  0001 C CNN
F 1 "GND" H 2905 5427 50  0000 C CNN
F 2 "" H 2900 5600 50  0000 C CNN
F 3 "" H 2900 5600 50  0000 C CNN
	1    2900 5600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 5100 3450 5100
Wire Wire Line
	3450 5200 3050 5200
Wire Wire Line
	3450 5300 3050 5300
Wire Wire Line
	3450 5400 3050 5400
Wire Wire Line
	2900 5600 2900 5500
Wire Wire Line
	2900 5500 3300 5500
Wire Wire Line
	3300 5500 3300 5900
Wire Wire Line
	3300 5900 3450 5900
Connection ~ 3300 5500
Wire Wire Line
	3300 5500 3450 5500
Wire Wire Line
	3300 5900 3300 6100
Wire Wire Line
	3300 6100 3450 6100
Connection ~ 3300 5900
Wire Wire Line
	3300 6100 3300 6500
Wire Wire Line
	3300 6500 3450 6500
Connection ~ 3300 6100
Wire Wire Line
	3300 6500 3300 7400
Wire Wire Line
	3300 7400 3450 7400
Connection ~ 3300 6500
$Comp
L Header_Board-cache-2018-07-27-13-43-46:GND #PWR0105
U 1 1 5DCF4B1C
P 4550 5750
F 0 "#PWR0105" H 4550 5500 50  0001 C CNN
F 1 "GND" H 4555 5577 50  0000 C CNN
F 2 "" H 4550 5750 50  0000 C CNN
F 3 "" H 4550 5750 50  0000 C CNN
	1    4550 5750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 5750 4550 5700
Wire Wire Line
	3950 5700 4100 5700
Wire Wire Line
	4100 5700 4100 6100
Wire Wire Line
	4100 6100 3950 6100
Connection ~ 4100 5700
Wire Wire Line
	4100 5700 4550 5700
Wire Wire Line
	4100 6100 4100 6500
Wire Wire Line
	4100 6500 3950 6500
Connection ~ 4100 6100
Wire Wire Line
	4100 6500 4100 6600
Wire Wire Line
	4100 6600 3950 6600
Connection ~ 4100 6500
Wire Wire Line
	4100 6600 4100 7400
Wire Wire Line
	4100 7400 3950 7400
Connection ~ 4100 6600
Text Label 3050 7500 0    50   ~ 0
SPI2_NSS
Text Label 3000 7600 0    50   ~ 0
SPI2_MISO
Text Label 4100 7500 0    50   ~ 0
SPI2_SCK
Text Label 4100 7600 0    50   ~ 0
SPI2_MOSI
$Comp
L Header_Board-cache-2018-07-27-13-43-46:+5V #PWR0106
U 1 1 5DD24A87
P 4450 6300
F 0 "#PWR0106" H 4450 6150 50  0001 C CNN
F 1 "+5V" H 4465 6473 50  0000 C CNN
F 2 "" H 4450 6300 50  0000 C CNN
F 3 "" H 4450 6300 50  0000 C CNN
	1    4450 6300
	1    0    0    -1  
$EndComp
$Comp
L Header_Board-cache-2018-07-27-13-43-46:+5V #PWR0107
U 1 1 5DD25478
P 3000 6300
F 0 "#PWR0107" H 3000 6150 50  0001 C CNN
F 1 "+5V" H 3015 6473 50  0000 C CNN
F 2 "" H 3000 6300 50  0000 C CNN
F 3 "" H 3000 6300 50  0000 C CNN
	1    3000 6300
	1    0    0    -1  
$EndComp
$Comp
L Header_Board-cache-2018-07-27-13-43-46:+3.3V #PWR0108
U 1 1 5DD25CC8
P 4650 6400
F 0 "#PWR0108" H 4650 6250 50  0001 C CNN
F 1 "+3.3V" H 4665 6573 50  0000 C CNN
F 2 "" H 4650 6400 50  0000 C CNN
F 3 "" H 4650 6400 50  0000 C CNN
	1    4650 6400
	1    0    0    -1  
$EndComp
$Comp
L Header_Board-cache-2018-07-27-13-43-46:+3.3V #PWR0109
U 1 1 5DD26BE9
P 2800 6400
F 0 "#PWR0109" H 2800 6250 50  0001 C CNN
F 1 "+3.3V" H 2815 6573 50  0000 C CNN
F 2 "" H 2800 6400 50  0000 C CNN
F 3 "" H 2800 6400 50  0000 C CNN
	1    2800 6400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 5100 3950 5100
Wire Wire Line
	4100 5200 3950 5200
Wire Wire Line
	4100 5300 3950 5300
Wire Wire Line
	4450 6300 3950 6300
Wire Wire Line
	4650 6400 3950 6400
Wire Wire Line
	3000 6300 3450 6300
Wire Wire Line
	2800 6400 3450 6400
Wire Wire Line
	3050 7500 3450 7500
Wire Wire Line
	3450 7600 3000 7600
Wire Wire Line
	3950 7500 4100 7500
Wire Wire Line
	3950 7600 4100 7600
Wire Wire Line
	950  1200 1200 1200
Wire Wire Line
	1650 7500 1750 7500
Wire Wire Line
	1650 7400 1750 7400
Wire Wire Line
	1650 7300 1750 7300
Wire Wire Line
	1650 6900 1750 6900
Wire Wire Line
	1650 6800 1750 6800
Wire Wire Line
	1650 6700 1750 6700
Wire Wire Line
	1650 6600 1750 6600
Wire Wire Line
	1650 6100 1750 6100
Wire Wire Line
	1650 6000 1750 6000
Wire Wire Line
	1650 5900 1750 5900
Wire Wire Line
	1650 5800 1750 5800
Wire Wire Line
	1650 5700 1750 5700
Wire Wire Line
	1650 5600 1750 5600
Wire Wire Line
<<<<<<< HEAD
	1650 5500 1750 5500
Wire Wire Line
=======
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
	1650 5400 1750 5400
Wire Wire Line
	1650 5300 1750 5300
Wire Wire Line
	1650 5200 1750 5200
Wire Wire Line
	1650 5100 1750 5100
Wire Wire Line
	1650 5000 1750 5000
Wire Wire Line
	1150 7300 650  7300
Wire Wire Line
	1150 7100 800  7100
Wire Wire Line
	1150 7000 800  7000
Wire Wire Line
	1150 6900 800  6900
Wire Wire Line
	1150 6800 800  6800
Wire Wire Line
	1150 6700 850  6700
Wire Wire Line
	1150 6600 850  6600
Wire Wire Line
	1150 6400 700  6400
Wire Wire Line
	1150 6100 600  6100
Wire Wire Line
	1150 6000 650  6000
Wire Wire Line
	1150 5900 750  5900
Wire Wire Line
	1150 5800 750  5800
Wire Wire Line
	1150 5700 850  5700
Wire Wire Line
	1150 5600 850  5600
Wire Wire Line
	1150 5500 850  5500
Wire Wire Line
	1150 5400 850  5400
Wire Wire Line
	1150 5300 850  5300
Wire Wire Line
	1150 5200 750  5200
Wire Wire Line
	1150 5100 850  5100
Wire Wire Line
	1150 5000 850  5000
Text Label 1750 7500 0    50   ~ 0
SGPIO6
Text Label 1750 7400 0    50   ~ 0
SGPIO5
Text Label 650  7300 0    50   ~ 0
NBUFEN_PRI
Text Label 1750 7300 0    50   ~ 0
NBUFEN_SEC
Text Label 800  7100 0    50   ~ 0
SYS_SCL
Text Label 800  7000 0    50   ~ 0
SYS_SDA
Text Label 1750 6900 0    50   ~ 0
SGPIO14
Text Label 1750 6800 0    50   ~ 0
SGPIO12
Text Label 1750 6700 0    50   ~ 0
SGPIO10
Text Label 1750 6600 0    50   ~ 0
SGPIO8
Text Label 800  6900 0    50   ~ 0
SGPIO13
Text Label 800  6800 0    50   ~ 0
SGPIO11
Text Label 850  6700 0    50   ~ 0
SGPIO9
Text Label 850  6600 0    50   ~ 0
SGPIO7
Text Label 700  6400 0    50   ~ 0
SYS_RESET
Text Label 600  6100 0    50   ~ 0
SYS_SPI_MOSI
Text Label 1750 6000 0    50   ~ 0
SYS_SPI_MISO
Text Label 1750 6100 0    50   ~ 0
SYS_SPI_NSS
Text Label 1750 5900 0    50   ~ 0
I2C2_SCL
Text Label 1750 5800 0    50   ~ 0
RADIO_TX
Text Label 1750 5700 0    50   ~ 0
GPIO13
Text Label 1750 5600 0    50   ~ 0
GPIO11
Text Label 1750 5500 0    50   ~ 0
<<<<<<< HEAD
GPIO9
=======
EN
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
Text Label 1750 5400 0    50   ~ 0
GPIO7
Text Label 1750 5300 0    50   ~ 0
GPIO5
Text Label 1750 5200 0    50   ~ 0
RADIO_RR
Text Label 1750 5100 0    50   ~ 0
GPIO3
Text Label 1750 5000 0    50   ~ 0
GPIO1
Text Label 650  6000 0    50   ~ 0
SYS_SPI_SCK
Text Label 750  5900 0    50   ~ 0
12C2_SDA
Text Label 750  5800 0    50   ~ 0
RADIO_RX
Text Label 850  5700 0    50   ~ 0
GPIO12
Text Label 850  5600 0    50   ~ 0
GPIO10
Text Label 850  5500 0    50   ~ 0
GPIO8
Text Label 850  5400 0    50   ~ 0
GPIO6
Text Label 850  5300 0    50   ~ 0
GPIO4
Text Label 750  5200 0    50   ~ 0
RADIO_TR
Text Label 850  5100 0    50   ~ 0
GPIO2
Text Label 850  5000 0    50   ~ 0
GPIO0
$Comp
L Header_Board-cache-2018-07-27-13-43-46:CONN_02X26 P1
U 1 1 5DBC40DD
P 1400 6250
F 0 "P1" H 1400 7715 50  0000 C CNN
F 1 "CONN_02X26" H 1400 7624 50  0000 C CNN
<<<<<<< HEAD
F 2 "" H 1400 5550 50  0001 C CNN
=======
F 2 "motherboard:CSK_Header" H 1400 5550 50  0001 C CNN
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
F 3 "" H 1400 5550 50  0000 C CNN
	1    1400 6250
	1    0    0    -1  
$EndComp
<<<<<<< HEAD
=======
Wire Wire Line
	1650 5500 2150 5500
$Comp
L Header_Board-cache-2018-07-27-13-43-46:+5V #PWR0110
U 1 1 5DC41B70
P 2150 5500
F 0 "#PWR0110" H 2150 5350 50  0001 C CNN
F 1 "+5V" H 2165 5673 50  0000 C CNN
F 2 "" H 2150 5500 50  0000 C CNN
F 3 "" H 2150 5500 50  0000 C CNN
	1    2150 5500
	1    0    0    -1  
$EndComp
Text Label 6400 1500 0    50   ~ 0
+8V
Text Label 10300 1750 0    50   ~ 0
vout
Text Label 4350 7300 0    50   ~ 0
vout
Wire Wire Line
	4350 7300 3950 7300
Text Label 2800 6800 0    50   ~ 0
Boost_EN
Wire Wire Line
	3450 6800 2800 6800
Wire Wire Line
	3650 1200 3650 1100
Wire Wire Line
	3650 700  3650 1100
Connection ~ 3650 1100
Wire Wire Line
	3650 1100 4550 1100
Wire Wire Line
	4550 1100 4550 1900
Text Label 4800 1800 0    50   ~ 0
Boost_EN
Wire Wire Line
	4800 1800 4450 1800
>>>>>>> a9eea6c4935e61f6a1c1986cf38f297d1483b495
$EndSCHEMATC