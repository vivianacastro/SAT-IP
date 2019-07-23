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

#include <core/SolverTypes.h>
#include <utils/CNFProblem.h>


#include <gates/GateAnalyzer.h>
#include <randomsimulation/ClauseOrder.h>

#include <testutils/TestGateStructure.h>
#include "TestUtils.h"

#include <unordered_set>

namespace Candy {
    
    // TODO: these tests do not cover Plaistedt-Greenbaum-optimized gate encodings yet.
    
    // TODO: test ClauseOrder with gate filter


    // utility functions
    
    /// Returns true iff the given sets of clause pointers are equal modulo item order.
    static bool equals(std::vector<Cl*>& a, const std::vector<const Cl*>& b);
    
    /// Returns true iff the given sets of T are equal modulo item order.
    template<typename T> static bool equals(const std::vector<T>& a, const std::vector<T>& b);
    
    /// Returns true iff the given iterable object contains the given thing.
    template<typename X, typename Y> static bool contains(X* iterable, Y thing);
    
    /// Returns true iff the given gate analyzer contains exactly the given clauses containing the
    /// given output literal, for the rsp. gate with the given output literal.
    static bool containsClauses(GateAnalyzer& analyzer, Lit outputLiteral, const std::vector<const Cl*>& clauses);
    
    /// Returns true iff all given clauses contain the given literal.
    static bool allClausesContain(Lit literal, const std::vector<const Cl*>& clauses);
    
    /// Returns true iff literals contains firstVar and secondVar, and firstVar appears before secondVar in literals.
    static bool appearsOrdered(Var firstVar, Var secondVar, const std::vector<Lit> &literals);
    
    
    
    static void test_noGates(ClauseOrder& underTest) {
        auto gateBuilder = createGateStructureBuilder();
        auto formula = gateBuilder->build();
        
        GateAnalyzer ga(*formula);
        ga.analyze();
        ASSERT_EQ(ga.getGateCount(), 0);
        
        underTest.readGates(ga);
        EXPECT_EQ(underTest.getAmountOfVars(), 0ul);
        EXPECT_EQ(underTest.getGateOutputsOrdered().size(), 0ul);
        EXPECT_EQ(underTest.getInputVariables().size(), 0ul);
    }
    
    TEST(RSClauseOrderTest, noGates_recursiveImpl) {
        auto underTest = createRecursiveClauseOrder();
        test_noGates(*underTest);
    }
    
    TEST(RSClauseOrderTest, noGates_nonrecursiveImpl) {
        auto underTest = createNonrecursiveClauseOrder();
        test_noGates(*underTest);
    }
    
    
    
    static void test_singleGate(ClauseOrder& underTest) {
        auto gateBuilder = createGateStructureBuilder();
        gateBuilder->withOr({mkLit(1, 1), mkLit(2, 1)}, mkLit(0,1));
        auto formula = gateBuilder->build();
        
        GateAnalyzer ga(*formula);
        ga.analyze();
        
        ASSERT_EQ(ga.getGateCount(), 1);
        
        underTest.readGates(ga);
        
        EXPECT_EQ(underTest.getAmountOfVars(), 3ul);
        EXPECT_EQ(underTest.getGateOutputsOrdered().size(), 1ul);
        EXPECT_EQ(underTest.getInputVariables().size(), 2ul);
        
        EXPECT_TRUE(equals(underTest.getInputVariables(), std::vector<Var>({1,2})));
        
        for (auto lit : underTest.getGateOutputsOrdered()) {
            EXPECT_TRUE(allClausesContain(lit, underTest.getClauses(0)));
            EXPECT_TRUE(containsClauses(ga, lit, underTest.getClauses(0)));
        }
    }
    
    TEST(RSClauseOrderTest, singleGate_recursiveImpl) {
        auto underTest = createRecursiveClauseOrder();
        test_singleGate(*underTest);
    }
    
    TEST(RSClauseOrderTest, singleGate_nonrecursiveImpl) {
        auto underTest = createNonrecursiveClauseOrder();
        test_singleGate(*underTest);
    }
    
    
    
    static void test_fewGates(ClauseOrder& underTest) {
        auto gateBuilder = createGateStructureBuilder();
        gateBuilder->withOr({mkLit(1, 1), mkLit(2, 1)}, mkLit(0,1));
        gateBuilder->withAnd({mkLit(3, 1), mkLit(4, 1)}, mkLit(1,1));
        gateBuilder->withAnd({mkLit(5, 1), mkLit(4, 1)}, mkLit(3,1));
        
        auto formula = gateBuilder->build();
        
        GateAnalyzer ga(*formula);
        ga.analyze();
        
        ASSERT_EQ(ga.getGateCount(), 3);
        
        underTest.readGates(ga);
        
        EXPECT_EQ(underTest.getAmountOfVars(), 6ul);
        EXPECT_EQ(underTest.getGateOutputsOrdered().size(), 3ul);
        EXPECT_EQ(underTest.getInputVariables().size(), 3ul);
        
        EXPECT_TRUE(equals(underTest.getInputVariables(), std::vector<Var>({2,5,4})));

        
        EXPECT_TRUE(appearsOrdered(1, 0, underTest.getGateOutputsOrdered()));
        EXPECT_TRUE(appearsOrdered(3, 0, underTest.getGateOutputsOrdered()));
        EXPECT_TRUE(appearsOrdered(3, 1, underTest.getGateOutputsOrdered()));
        
        for (auto lit : underTest.getGateOutputsOrdered()) {
            EXPECT_TRUE(allClausesContain(lit, underTest.getClauses(var(lit))));
            EXPECT_TRUE(containsClauses(ga, lit, underTest.getClauses(var(lit))));
        }
    }
    
    TEST(RSClauseOrderTest, fewGates_recursiveImpl) {
        auto underTest = createRecursiveClauseOrder();
        test_fewGates(*underTest);
    }
    
    TEST(RSClauseOrderTest, fewGates_nonrecursiveImpl) {
        auto underTest = createNonrecursiveClauseOrder();
        test_fewGates(*underTest);
    }
    
    
    
    static void test_manyGates(ClauseOrder& underTest) {
        auto gateBuilder = createGateStructureBuilder();
        gateBuilder->withOr({mkLit(1, 1), mkLit(2, 1)}, mkLit(0,1));
        gateBuilder->withAnd({mkLit(3, 1), mkLit(4, 1)}, mkLit(1,1));
        
        gateBuilder->withAnd({mkLit(5, 1), mkLit(4, 1), mkLit(6, 1)}, mkLit(3,0));
        gateBuilder->withAnd({mkLit(7, 0), mkLit(6, 0)}, mkLit(4,1));
        
        
        gateBuilder->withOr({mkLit(8, 1), mkLit(9,0), mkLit(5,0)}, mkLit(7,0));
        gateBuilder->withAnd({mkLit(10, 1), mkLit(9,0), mkLit(6,0)}, mkLit(8,0));
        gateBuilder->withAnd({mkLit(11, 1), mkLit(12,0), mkLit(13,0)}, mkLit(5,0));
        

        
        auto formula = gateBuilder->build();
        
        GateAnalyzer ga(*formula, 0, true, false, false, false);
        ga.analyze();
        
        ASSERT_EQ(ga.getGateCount(), 7);
        ASSERT_EQ(ga.getRoots().size(), 1ul);
        ASSERT_EQ(ga.getRoots()[0]->size(), 1ul);
        
        underTest.readGates(ga);
        
        EXPECT_EQ(underTest.getAmountOfVars(), 14ul);
        EXPECT_EQ(underTest.getGateOutputsOrdered().size(), 7ul);
        EXPECT_EQ(underTest.getInputVariables().size(), 7ul);
        
        EXPECT_TRUE(equals(underTest.getInputVariables(), std::vector<Var>({2,6,9,10,11,12,13})));
        
        
        EXPECT_TRUE(appearsOrdered(1, 0, underTest.getGateOutputsOrdered()));
        EXPECT_TRUE(appearsOrdered(3, 0, underTest.getGateOutputsOrdered()));
        EXPECT_TRUE(appearsOrdered(3, 1, underTest.getGateOutputsOrdered()));
        EXPECT_TRUE(appearsOrdered(4, 3, underTest.getGateOutputsOrdered()));
        EXPECT_TRUE(appearsOrdered(7, 4, underTest.getGateOutputsOrdered()));
        EXPECT_TRUE(appearsOrdered(8, 7, underTest.getGateOutputsOrdered()));
        EXPECT_TRUE(appearsOrdered(5, 3, underTest.getGateOutputsOrdered()));
        EXPECT_TRUE(appearsOrdered(5, 7, underTest.getGateOutputsOrdered()));
        
        for (auto lit : underTest.getGateOutputsOrdered()) {
            EXPECT_TRUE(allClausesContain(lit, underTest.getClauses(var(lit))));
            EXPECT_TRUE(containsClauses(ga, lit, underTest.getClauses(var(lit))));
        }
    }
    
    TEST(RSClauseOrderTest, manyGates_recursiveImpl) {
        auto underTest = createRecursiveClauseOrder();
        test_manyGates(*underTest);
    }
    
    TEST(RSClauseOrderTest, manyGates_nonrecursiveImpl) {
        auto underTest = createNonrecursiveClauseOrder();
        test_manyGates(*underTest);
    }

    
    
    
    
    
    static bool equals(std::vector<Cl*>& a, const std::vector<const Cl*>& b) {
        if (a.size() != b.size()) {
            return false;
        }
        
        std::unordered_set<const Cl*> set_a;
        std::unordered_set<const Cl*> set_b;
        
        set_a.insert(a.begin(), a.end());
        set_b.insert(b.begin(), b.end());
        
        return set_a == set_b;
    }
    
    template<typename T>
    static bool equals(const std::vector<T>& a, const std::vector<T>& b) {
        if (a.size() != b.size()) {
            return false;
        }
        
        std::unordered_set<T> set_a;
        std::unordered_set<T> set_b;
        
        set_a.insert(a.begin(), a.end());
        set_b.insert(b.begin(), b.end());
        
        return set_a == set_b;
    }
    
    template<typename X, typename Y>
    static bool contains(X* iterable, Y thing) {
        return std::find(iterable->begin(), iterable->end(), thing) != iterable->end();
    }
    
    static bool containsClauses(GateAnalyzer& analyzer, Lit outputLiteral, const std::vector<const Cl*>& clauses) {
        auto &gate = analyzer.getGate(outputLiteral);
        return gate.isDefined() && (equals(gate.getForwardClauses(), clauses) || equals(gate.getBackwardClauses(), clauses));
    }
    
    static bool allClausesContain(Lit literal, const std::vector<const Cl*>& clauses) {
        for (auto clause : clauses) {
            if (!contains(clause, literal)) {
                return false;
            }
        }
        return true;
    }
    
    static bool appearsOrdered(Var firstVar, Var secondVar, const std::vector<Lit> &literals) {
        bool foundFirstLit = false;
        bool foundSecondLit = false;
        for (auto lit : literals) {
            foundFirstLit |= var(lit) == firstVar;
            foundSecondLit |= var(lit) == secondVar;
            if (foundSecondLit && !foundFirstLit) {
                return false;
            }
        }
        return foundFirstLit && foundSecondLit;
    }
}
