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

#include <gtest/gtest.h>

#include <gates/GateAnalyzer.h>

#include "TestGateStructure.h"
#include "TestUtils.h"

namespace Candy {
    
    TEST(RSTestMockGateStructureBuilding, GateStructureBuilderTest_mostSimple) {
        auto gateBuilder = createGateStructureBuilder();
        auto cnf = gateBuilder->build();
        auto& formula = cnf->getProblem();
        
        EXPECT_EQ(formula.size(), 1ul);
        EXPECT_TRUE(containsClause(formula, Cl({mkLit(0, 1)})));
    }
    
    TEST(RSTestMockGateStructureBuilding, GateStructureBuilderTest_andGate) {
        auto gateBuilder = createGateStructureBuilder();
        gateBuilder->withAnd({mkLit(1, 1), mkLit(2, 1)}, mkLit(0,1));
        auto cnf = gateBuilder->build();
        auto& formula = cnf->getProblem();
        
        EXPECT_EQ(formula.size(), 4ul);
        
        EXPECT_TRUE(containsClause(formula, Cl({mkLit(1, 0), mkLit(2, 0), mkLit(0, 1)})));
        EXPECT_TRUE(containsClause(formula, Cl({mkLit(1, 1), mkLit(0, 0)})));
        EXPECT_TRUE(containsClause(formula, Cl({mkLit(2, 1), mkLit(0, 0)})));
        EXPECT_TRUE(containsClause(formula, Cl({mkLit(0, 1)})));
    }
    
    TEST(RSTestMockGateStructureBuilding, GateStructureBuilderTest_orGate) {
        auto gateBuilder = createGateStructureBuilder();
        gateBuilder->withOr({mkLit(1, 1), mkLit(2, 1)}, mkLit(0,1));
        auto cnf = gateBuilder->build();
        auto& formula = cnf->getProblem();
        
        EXPECT_EQ(formula.size(), 4ul);
        
        EXPECT_TRUE(containsClause(formula, Cl({mkLit(1, 1), mkLit(2, 1), mkLit(0, 0)})));
        EXPECT_TRUE(containsClause(formula, Cl({mkLit(1, 0), mkLit(0, 1)})));
        EXPECT_TRUE(containsClause(formula, Cl({mkLit(2, 0), mkLit(0, 1)})));
        EXPECT_TRUE(containsClause(formula, Cl({mkLit(0, 1)})));
    }
    
    TEST(RSTestMockGateStructureBuilding, GateAnalyzerTest_simple) {
        auto gateBuilder = createGateStructureBuilder();
        gateBuilder->withOr({mkLit(1, 1), mkLit(2, 1)}, mkLit(0,1));
        auto formula = gateBuilder->build();
        
        GateAnalyzer ga(*formula);
        ga.analyze();
        EXPECT_EQ(ga.getGateCount(), 1);
    }
    
    TEST(RSTestMockGateStructureBuilding, GateAnalyzerTest_threeGates) {
        auto gateBuilder = createGateStructureBuilder();
        gateBuilder->withOr({mkLit(1, 1), mkLit(2, 1)}, mkLit(0,1));
        gateBuilder->withAnd({mkLit(3, 1), mkLit(4, 1), mkLit(5, 1)}, mkLit(1,1));
        gateBuilder->withAnd({mkLit(6, 1), mkLit(7, 1)}, mkLit(2,1));
        auto formula = gateBuilder->build();
        
        GateAnalyzer ga(*formula);
        ga.analyze();
        EXPECT_EQ(ga.getGateCount(), 3);
    }
}
