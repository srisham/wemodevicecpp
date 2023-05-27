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
 * @file Soap.h
 * 
 ***************************************************************************/

#pragma once
#include <iostream>

std::string createSoapRequest(const std::string& wMethod, const std::string& wServiceType, 
    const std::string& wName = "", const std::string& wState = "");

std::string parseSoapResponse(const std::string& response, const std::string& node);