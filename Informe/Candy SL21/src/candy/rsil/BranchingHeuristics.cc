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

#include "BranchingHeuristics.h"
#include <candy/core/Solver.h>

#include <algorithm>

namespace Candy {
    template <>
    Lit Solver<RSILBranchingHeuristic3>::pickBranchLit() {
        Lit rsilAdvice = pickBranchLitData.getAdvice(trail, trail_size, trail_lim, assigns, decision);
        if (rsilAdvice != lit_Undef) {
            return rsilAdvice;
        }
        
        Lit candidate = defaultPickBranchLit();
        
        if (rsilAdvice != lit_Undef || candidate == lit_Undef) {
            return candidate;
        }
        return pickBranchLitData.getSignAdvice(candidate);
    }
    
    template <>
    Lit Solver<RSILVanishingBranchingHeuristic3>::pickBranchLit() {
        Lit rsilAdvice = pickBranchLitData.getAdvice(trail, trail_size, trail_lim, assigns, decision);
        if (rsilAdvice != lit_Undef) {
            return rsilAdvice;
        }
        
        Lit candidate = defaultPickBranchLit();
        
        if (rsilAdvice != lit_Undef || candidate == lit_Undef) {
            return candidate;
        }
        return pickBranchLitData.getSignAdvice(candidate);
    }
    
    template <>
    Lit Solver<RSILBudgetBranchingHeuristic3>::pickBranchLit() {
        Lit rsilAdvice = pickBranchLitData.getAdvice(trail, trail_size, trail_lim, assigns, decision);
        if (rsilAdvice != lit_Undef) {
            return rsilAdvice;
        }
        
        Lit candidate = defaultPickBranchLit();
        
        if (rsilAdvice != lit_Undef || candidate == lit_Undef) {
            return candidate;
        }
        return pickBranchLitData.getSignAdvice(candidate);
    }
    
    template <>
    Lit Solver<RSILBranchingHeuristic2>::pickBranchLit() {
        Lit rsilAdvice = pickBranchLitData.getAdvice(trail, trail_size, trail_lim, assigns, decision);
        if (rsilAdvice != lit_Undef) {
            return rsilAdvice;
        }
        
        Lit candidate = defaultPickBranchLit();
        
        if (rsilAdvice != lit_Undef || candidate == lit_Undef) {
            return candidate;
        }
        return pickBranchLitData.getSignAdvice(candidate);
    }
    
    template <>
    Lit Solver<RSILVanishingBranchingHeuristic2>::pickBranchLit() {
        Lit rsilAdvice = pickBranchLitData.getAdvice(trail, trail_size, trail_lim, assigns, decision);
        if (rsilAdvice != lit_Undef) {
            return rsilAdvice;
        }
        
        Lit candidate = defaultPickBranchLit();
        
        if (rsilAdvice != lit_Undef || candidate == lit_Undef) {
            return candidate;
        }
        return pickBranchLitData.getSignAdvice(candidate);
    }
    
    template <>
    Lit Solver<RSILBudgetBranchingHeuristic2>::pickBranchLit() {
        Lit rsilAdvice = pickBranchLitData.getAdvice(trail, trail_size, trail_lim, assigns, decision);
        if (rsilAdvice != lit_Undef) {
            return rsilAdvice;
        }
        
        Lit candidate = defaultPickBranchLit();
        
        if (rsilAdvice != lit_Undef || candidate == lit_Undef) {
            return candidate;
        }
        return pickBranchLitData.getSignAdvice(candidate);
    }
}
