/*
 * Certificate.cc
 *
 *  Created on: Mar 7, 2017
 *      Author: markus
 */

#include "candy/core/Certificate.h"
#include "candy/utils/MemUtils.h"

namespace Candy {

Certificate::Certificate(const char* _out, const bool _active) :
                active(_out != nullptr && _active)
{
    if (active) {
        out = backported_std::make_unique<std::ofstream>(_out, std::ofstream::out);
        if (!out->is_open()) {
            active = false;
        }
    }
    else {
        out = backported_std::make_unique<std::ofstream>();
    }
}

Certificate::~Certificate() {
    if (active) {
        out->close();
    }
}

} /* namespace Candy */
