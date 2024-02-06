# 20W/50Ω μDummy RF Load with Wattmeter

This is the home of the design files and firmware for the μDummy RF Load project. See the description of the unit and the resources in this repo below.

## Description

This micro-size 50Ω RF dummy load with built-in wattmeter is well-suited for testing portable rigs. 
The glass LCD provides time-average power readings between 0.15 and 20W with 0.1W precision. 
Batteries are not required, as the unit draws power from the RF source, consuming a maximum of 3.0mA at the full rated input power. 
The board itself serves as a heatsink provided the operator limits the duration of the RF input. 
Improve the accuracy by loading a calibration table of up to 60 rows into EEPROM using an Arduino microcontroller.

## Organization of this Repo

- `2_Documentation/`: datasheet and documentation for the μDummy load unit
- `3_Mechanical/`: mechanical design files (primarily thermal analysis)
- `3_Electrical/`: KiCAD design files and LTspcice simulation files
- `4_Software/`: firmware files
  - `uDummyLoad_PIC18F24Q10.X/` firmware for the PIC microcontroller
  - `uDummyLoad_TranslatorArduino/`: Arduino sketch for uploading calibration table to the unit with an Arduino microcontroller

 ## License

Apache License, Version 2.0. See `LICENSE`.
