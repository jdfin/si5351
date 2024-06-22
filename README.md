# Using Si5351 from Raspberry Pi

This has been run with:
- [Adafruit Si5351 breakout board](https://www.adafruit.com/product/2045)
- Raspberry Pi 3a (bullseye)
- Skyworks [ClockBuilder Pro](https://www.skyworksinc.com/Application-Pages/Clockbuilder-Pro-Software) (4.13)

Use ClockBuilder Pro to generate the register settings, and replace the file
`Si5351A-RevB-Registers.h` with your new one.

Build and run it:
```
mkdir build
cd build
cmake ..
make
./si5351_test
```

You should see the clocks on the outputs after the program runs.
