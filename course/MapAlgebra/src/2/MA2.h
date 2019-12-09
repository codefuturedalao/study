#pragma once

#include <math.h>

#include "dis_template.hpp"

class Bmp;

bool DisTrans256(const char* srcBmpName, DistanceTemplate* pDisT, const char* disBmpName, const char* locBmpName);

bool DisTrans32bit(const char* srcBmpName, DistanceTemplate* pDisT, const char* disBmpName, const char* locBmpName);

bool getBoundaryFromBmp(const char* bmpInputName, const char* bmpOutputName);
bool getBoundary(const uchar* const * srcMtx, uint mtxWidth, uint mtxHeight, uchar*** boundaryMtx);

bool getBufferFromBmp(const char* disBmpName, const char* locBmpName, const char* bmpOutputBufName, float radius);
bool getBuffer(const float* const* disMtx, uint mtxWidth, uint mtxHeight, uchar*** bufferMtx, float radius);

bool scanSrcPointCoords(const char* srcBmpName, const char* coordsTableFile);

bool getTinPointPairs(const char* srcLocBmpName, const char* pointPairsFile);

bool linkPoints(const char* srcBmpName, const char* pointPairsFile, const char* coordsTableFile, const char* outBmpFile);


