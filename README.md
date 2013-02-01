RPiBJ.SPIADC
============

Raspberry Pi SPI ADC binding for Python

## Requirements
*  Uses [Microchip's MCP3008](http://www.microchip.com/wwwproducts/Devices.aspx?dDocName=en010530)
*  The SPI module must be enabled on your Raspberry Pi's kernel

## Enabling SPI on Raspbian
*  Remove spi from the blacklist file at /etc/modprobe.d/raspi-blacklist.conf
*  Reboot, If it worked you'll see spidev0.0 and spidev0.1 in /dev/

## Example Usage
```python
from RPiBJ import SPIADC as spi
# setup ADC on SPI channel 0 (Chip Enable 0) with speed 100,000hz
spi.setup(0, 100000)
# read channel 0-7
spi.read(0)
```
