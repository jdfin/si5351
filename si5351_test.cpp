
#include "i2c_dev.h"
#include "si5351.h"


int main(int argc, char *argv[])
{
    I2cDev i2c("/dev/i2c-1", 0x60);
    Si5351 si5351(i2c);

    si5351.show();

    return 0;
}
