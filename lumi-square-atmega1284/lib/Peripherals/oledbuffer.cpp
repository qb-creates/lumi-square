#include "./oledbuffer.h"

OLEDBuffer::OLEDBuffer(uint8_t columnStartAddress, uint8_t columnEndAddress, uint8_t pageStartAddress, uint8_t pageEndAddress, uint8_t dataCount)
    : columnStartAddress(columnStartAddress),
      columnEndAddress(columnEndAddress),
      pageStartAddress(pageStartAddress),
      pageEndAddress(pageEndAddress),
      dataCount(dataCount)
{
}