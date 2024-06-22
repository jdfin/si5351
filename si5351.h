#pragma once

class I2cDev;


class Si5351
{

    public:

        Si5351(I2cDev& i2c);

        ~Si5351();

        void show_reg(int n, bool zeros=true);

        void show(bool zeros=true);

    private:

        uint8_t read_reg(uint8_t address)
        {
            uint8_t value;
            _i2c.read_buf(address, &value, 1);
            return value;
        }

        void write_reg(uint8_t address, uint8_t value)
        {
            _i2c.write_buf(address, &value, 1);
        }

        I2cDev& _i2c;

}; // class Si5351
