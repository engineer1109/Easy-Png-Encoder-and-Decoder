//
// Created by wjl on 2020/7/12.
//

#include "pngutils.h"

bool PngUtils::isBigEndian() {
    unsigned short usData = 0x1122;
    unsigned char *pucData = (unsigned char*)&usData;
    return (*pucData == 0x22);
}
