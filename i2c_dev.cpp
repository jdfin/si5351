#include <cstdint>
#include <cstring>
#include <string>
#include <stdexcept>

#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "i2c_dev.h"

using std::runtime_error;
using std::string;


I2cDev::I2cDev(const char *i2c_dev, uint8_t i2c_adr) :
    _fd(open(i2c_dev, O_RDWR)),
    _adr(i2c_adr)
{
    if (_fd < 0)
        throw runtime_error("I2cDev::I2cDev: _fd < 0");
}


I2cDev::~I2cDev()
{
    if (_fd >= 0) {
        close(_fd);
        _fd = -1;
    }
}


// Read buf_len bytes from reg_adr into buf
//
// always succeeds or throws
void I2cDev::read_buf(uint8_t reg_adr, uint8_t *buf, int buf_len)
{
    if (_fd < 0)
        throw runtime_error("I2cDev::read_buf: _fd < 0");

    // send the reg_adr, repeated start, receive the data
    i2c_msg msgs[2] = {
        { _adr, 0, 1, &reg_adr },
        { _adr, I2C_M_RD, uint16_t(buf_len), buf }
    };
    i2c_rdwr_ioctl_data rdwr = { msgs, 2 };

    if (buf == nullptr || buf_len == 0)
        rdwr.nmsgs = 1; // just sending the register address, ok

    if (ioctl(_fd, I2C_RDWR, &rdwr) < 0)
        throw runtime_error(string("I2cDev::read_buf: ioctl: ")
                            + strerror(errno));
}


// Write buf_len bytes from buf to reg_adr
//
// always succeeds or throws
void I2cDev::write_buf(uint8_t reg_adr, uint8_t *buf, int buf_len)
{
    if (_fd < 0)
        throw runtime_error("I2cDev::write_buf: _fd < 0");

    // Si5351 does not work if we: send reg_adr, restart, send data
    // (in two i2c_msg)
    // Many devices are happy with that sequence for writes.
    //
    // Si5351 only works if we: send reg_adr, send data
    // (no restart, one i2c_msg)
    //
    // Too bad; using two msgs is nice because the temp buffer is not needed.
    // Raspberry Pi doesn't have I2C_FUNC_NOSTART (so no I2C_M_NOSTART).

    uint8_t *data = new uint8_t[1 + buf_len];

    data[0] = reg_adr;
    memcpy(&data[1], buf, buf_len);
    i2c_msg msg = { _adr, 0, uint16_t(1 + buf_len), data };

    i2c_rdwr_ioctl_data rdwr = { &msg, 1 };

    if (ioctl(_fd, I2C_RDWR, &rdwr) < 0)
        throw runtime_error(string("I2cDev::write_buf: ioctl: ")
                            + strerror(errno));

    delete[] data;
}
