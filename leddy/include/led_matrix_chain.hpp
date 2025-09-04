// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: Copyright 2007 Eberhard Fahle
// SPDX-FileCopyrightText: Copyright 2022-2025 Zeus WPI
/*
 *    LedMatrixChain.h - A library for controling Leds with a MAX7219/MAX7221
 *    Copyright (c) 2007 Eberhard Fahle
 *
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 *
 *    This permission notice shall be included in all copies or
 *    substantial portions of the Software.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <stdint.h>

#define foreach_row(body) for (size_t row = 0; row < 8; row++) body

#define row_reg(row) ((LedMatrixReg)(uint8_t)(1 + row))
#define foreach_dev(body) for (size_t dev = 0; dev < ct_deviceCount; dev++) body
#define foreach_dev_send(reg, payload) foreach_dev({spiStageAssignment(dev, reg, payload);}) spiSend();

// The registers for the MAX7221 and MAX7219.
enum struct LedMatrixReg : uint8_t
{
    NO_OP        = 0x00,
    SEGMENT_0    = 0x01,
    SEGMENT_1    = 0x02,
    SEGMENT_2    = 0x03,
    SEGMENT_3    = 0x04,
    SEGMENT_4    = 0x05,
    SEGMENT_5    = 0x06,
    SEGMENT_6    = 0x07,
    SEGMENT_7    = 0x08,
    DECODE_MODE  = 0x09,
    INTENSITY    = 0x0A,
    SCAN_LIMIT   = 0x0B,
    SHUTDOWN     = 0x0C,
    UNKNOWN_0x0D = 0x0D,
    UNKNOWN_0x0E = 0x0E,
    DISPLAY_TEST = 0x0F,
};

/**
 * Params:
 *     ct_deviceCount = Number of 8x8 matrices in this daisy chain.
 */
template <size_t ct_deviceCount>
struct LedMatrixChain
{
public:
    /**
     * Compile-time constant.
     * The number of 8x8 led matrices in this daisy chain.
     */
    static constexpr size_t deviceCount = ct_deviceCount;

private:
    uint8_t m_clkPin = 0;  // Active high.
    uint8_t m_loadPin = 0; // Active high.
    uint8_t m_mosiPin = 0; // Active high.
    uint8_t m_intensity = 15;
    uint8_t m_spiData[ct_deviceCount * 2] = {};

public:
    // Ctor, has no side effects.
    LedMatrixChain(uint8_t clkPin, uint8_t loadPin, uint8_t mosiPin)
    {
        m_clkPin = clkPin;
        m_loadPin = loadPin;
        m_mosiPin = mosiPin;
    }

private:
    /**
     * Stage the assignment of `payload` to register `reg` on the device with
     * index `deviceIndex`. All devices should have 1 staged assignment before
     * calling `spiSend()`.
     */
    inline void spiStageAssignment(size_t deviceIndex, LedMatrixReg reg, uint8_t payload)
    {
        // The matrix chain is one large shift register (FIFO). So device `0`
        // needs to be sent last, device `ct_deviceCount - 1` needs to be sent
        // first.
        constexpr size_t firstDevOffset = sizeof(m_spiData) - 2;
        const size_t offset = firstDevOffset - deviceIndex * 2;
        // Each matrix holds 16 bits: an 8-bit register address, followed by an
        // 8-bit payload.
        m_spiData[offset + 0] = (uint8_t)reg;
        m_spiData[offset + 1] = payload;
    }

    /**
     * Send payloads staged with `spiPrepare` to all devices.
     *
     * Note: The staged assignments are not cleared.
     */
    void spiSend()
    {
        //#define writePin(pin, value) (value ? *pin |= pin##Mask : *pin &= ~pin##Mask)
        #define writePin(pin, value) digitalWrite(m_##pin##Pin, value)
        //volatile uint8_t *load = portOutputRegister(digitalPinToPort(m_loadPin));
        //volatile uint8_t *clk  = portOutputRegister(digitalPinToPort(m_clkPin));
        //volatile uint8_t *mosi = portOutputRegister(digitalPinToPort(m_mosiPin));
        //const uint8_t loadMask = digitalPinToBitMask(m_loadPin);
        //const uint8_t clkMask  = digitalPinToBitMask(m_clkPin);
        //const uint8_t mosiMask = digitalPinToBitMask(m_mosiPin);

        //noInterrupts();

        for (size_t i = 0; i < sizeof(m_spiData); i++)
        {
            uint8_t b = m_spiData[i];

            for (size_t j = 0; j < 8; j++)
            {
                writePin(mosi, (b & 0x80) ? 1 : 0);
                b <<= 1;
                writePin(clk, 1);
                writePin(clk, 0);
            }
        }
        writePin(load, 0);
        writePin(load, 1);

        //interrupts();

        #undef writePin
    }

public:
    /**
     * (Re-)initialize all displays in the chain.
     * Should be called once in a while in case a row-write
     * actually overwrites another register.
     */
    void init()
    {
        pinMode(m_mosiPin, OUTPUT);
        pinMode(m_clkPin, OUTPUT);
        pinMode(m_loadPin, OUTPUT);

        foreach_dev_send(LedMatrixReg::NO_OP, 0);
        //setAllRows(0);
        foreach_dev_send(LedMatrixReg::DECODE_MODE, 0);
        foreach_dev_send(LedMatrixReg::INTENSITY, m_intensity);
        foreach_dev_send(LedMatrixReg::SCAN_LIMIT, 7);
        foreach_dev_send(LedMatrixReg::SHUTDOWN, 1);
        foreach_dev_send(LedMatrixReg::UNKNOWN_0x0D, 0);
        foreach_dev_send(LedMatrixReg::UNKNOWN_0x0E, 0);
        foreach_dev_send(LedMatrixReg::DISPLAY_TEST, 0);
    }

    /**
     * Set the intensity (brightness) of all displays.
     * Params:
     *     intensity = Value in the range `[0, 15]`.
     * Note: 14 and 15 are the same brightness.
     */
    void setIntensity(uint8_t intensity)
    {
        if (intensity > 15)
            intensity = 15;
        foreach_dev_send(LedMatrixReg::INTENSITY, intensity);
    }

    /**
     * Set all rows on all devices to the bits of the byte `value`.
     */
    void setAllRows(const uint8_t value)
    {
        foreach_row({
            foreach_dev_send(row_reg(row), value);
        });
    }

    /**
     * On each device, set the 8 leds in the row with index `row` to the bits of
     * the byte `arr[dev]`, with `dev` being the device index.
     * Params:
     *     row = The row which is to be set.
     *     arr = Array of `ct_deviceCount` bytes, holding the row values.
     */
    void setRows(uint8_t row, const uint8_t arr[ct_deviceCount])
    {
        if (row >= 8)
            return;
        foreach_dev_send(row_reg(row), arr[dev]);
    }

    /**
     * On each device, set the 8 leds in the row with index `row` to the bits of
     * the byte `arr[(start + dev * 8) % size]`, with `dev` being the device index.
     * Params:
     *     row = The row which is to be set.
     *     arr = Array of `size` bytes.
     *     size = `arr` size in bytes.
     *     start = The index of the byte representing row `0`. The incices for
     *         the other rows are obtained by adding `8`, wrapping around to the
     *         front of `arr` if needed.
     */
    void setRowsFromPixArr(uint8_t row, const uint8_t *arr, size_t size, size_t start)
    {
        if (row >= 8)
            return;
        if (size < 8)
            return;
        size_t i = start;
        foreach_dev({
            spiStageAssignment(dev, row_reg(row), arr[i]);
            i += 8;
            if (i >= size)
                i -= size;
        });
        spiSend();
    }
};

#undef row_reg
#undef foreach_dev
#undef foreach_dev_send
