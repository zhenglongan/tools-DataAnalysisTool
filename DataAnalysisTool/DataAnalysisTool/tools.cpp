#include "stdafx.h"
#include "tools.h"


//4个字节转成INT32
INT32 bytes_to_int32(BYTE high,BYTE middle1,BYTE middle2,BYTE low)
{
	INT32 val=0;
	val = ((high & 0xff)<<24)|
                ((middle1 & 0xff)<<16)|
                ((middle2 & 0xff)<<8)|
                (low & 0xff);
    return val;

}
//2个字节转成INT16
INT16 bytes_to_int16(BYTE high,BYTE low)
{
	INT16 val=0;
	val = ((high & 0xff)<<8)|
                (low & 0xff);
    return val;
}



