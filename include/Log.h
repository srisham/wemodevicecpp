/***************************************************************************
 *  Project                WEMODEVICECPP     
 *
 * Copyright (C) 2023 , Sri Balaji S.
 *
 * This software is licensed as described in the file LICENSE, which
 * you should have received as part of this distribution.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the LICENSE file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 * @file Log.h
 * 
 ***************************************************************************/

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <iostream>
#include <map>
using namespace std;

#define LOG_F(...)              L::log(L::f, ##__VA_ARGS__)
#define LOG_E(...)              L::log(L::e, ##__VA_ARGS__)
#define LOG_W(...)              L::log(L::w, ##__VA_ARGS__)
#define LOG_I(...)              L::log(L::i, ##__VA_ARGS__)
#define LOG_D(...)              L::log(L::d, ##__VA_ARGS__)

namespace L {

    enum LEVEL { f, e, w, i, d };

    void setLogLevel(LEVEL level);
    void log(LEVEL level, const char *format, ...);
    std::string getLogDetails(LEVEL level);
    std::string getTimestamp();

}
#endif