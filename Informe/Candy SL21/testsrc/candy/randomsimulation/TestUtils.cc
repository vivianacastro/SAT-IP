/* Copyright (c) 2017 Felix Kutzner (github.com/fkutzner)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
 Except as contained in this notice, the name(s) of the above copyright holders
 shall not be used in advertising or otherwise to promote the sale, use or
 other dealings in this Software without prior written authorization.
 
 */

#include "TestUtils.h"
#include <utils/CNFProblem.h>
#include <randomsimulation/Conjectures.h>
#include <testutils/TestUtils.h>

namespace Candy {
    bool hasBackboneConj(const Conjectures &c, Lit lit) {
        bool found = false;
        for (auto bb : c.getBackbones()) {
            found |= (bb.getLit() == lit);
        }
        return found;
    }
    
    bool isEquivalenceConjEq(EquivalenceConjecture &conj, const std::vector<Glucose::Lit>& lits) {
        if (conj.size() != lits.size()) {
            return false;
        }
        else {
            for (auto lit : lits) {
                if (std::find(conj.begin(), conj.end(), lit) == conj.end()) {
                    return false;
                }
            }
        }
        
        return true;
    }
    
    bool hasEquivalenceConj(Conjectures &c, const std::vector<Glucose::Lit>& lits) {
        auto invertedLits = negatedLits(lits);
        for (auto eqconj : c.getEquivalences()) {
            if (isEquivalenceConjEq(eqconj, lits) || isEquivalenceConjEq(eqconj, invertedLits)) {
                return true;
            }
        }
        
        return false;
    }
}
