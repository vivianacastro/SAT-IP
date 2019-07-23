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
#include <gmock/gmock.h>
#include <core/SolverTypes.h>
#include <randomsimulation/Conjectures.h>
#include <rsil/ImplicitLearningAdvice.h>
#include <rsar/Heuristics.h>
#include <rsil/RSARHeuristicsFilter.h>

#include "TestUtils.h"

#include <iostream>
#include <vector>

namespace Candy {
    namespace  {
        class MockHeuristic : public RefinementHeuristic {
        public:
            void beginRefinementStep() {} ;
            void markRemovals(EquivalenceImplications&) {};
            void markRemovals(Backbones&) {};
            MOCK_METHOD2(probe, bool(Var, bool));
        };
        
        void test_noRemovalWithNullHeuristic(bool filterBackbone) {
            Conjectures testDataSrc;
            testDataSrc.addEquivalence(EquivalenceConjecture{std::vector<Lit>{mkLit(5,1), mkLit(1,0)}});
            testDataSrc.addEquivalence(EquivalenceConjecture{std::vector<Lit>{mkLit(4,1), mkLit(2,0)}});
            ImplicitLearningAdvice<AdviceEntry<3>> testData(testDataSrc, 5);
            
            MockHeuristic nullHeuristic;
            EXPECT_CALL(nullHeuristic, probe(testing::_,testing::_)).WillRepeatedly(testing::Return(false));
            
            filterWithRSARHeuristics({&nullHeuristic}, testData, filterBackbone);
            
            EXPECT_TRUE(testData.hasPotentialAdvice(5));
            EXPECT_TRUE(isEquivalenceAdvised(testData, mkLit(5,1), mkLit(1,0)));
            EXPECT_TRUE(isEquivalenceAdvised(testData, mkLit(1,0), mkLit(5,1)));
            EXPECT_TRUE(isEquivalenceAdvised(testData, mkLit(4,1), mkLit(2,0)));
            EXPECT_TRUE(isEquivalenceAdvised(testData, mkLit(2,0), mkLit(4,1)));
        }
    }
    
    TEST(RSILRSARHeuristicsFilterTests, noRemovalWithNullHeuristic) {
        test_noRemovalWithNullHeuristic(false);
    }
    
    TEST(RSILRSARHeuristicsFilterTests, noRemovalWithNullHeuristic_onlyBackbones) {
        test_noRemovalWithNullHeuristic(true);
    }
    
    TEST(RSILRSARHeuristicsFilterTests, allRemovedWithProbePositiveHeuristic) {
        Conjectures testDataSrc;
        testDataSrc.addEquivalence(EquivalenceConjecture{std::vector<Lit>{mkLit(5,1), mkLit(1,0)}});
        testDataSrc.addEquivalence(EquivalenceConjecture{std::vector<Lit>{mkLit(4,1), mkLit(2,0)}});
        ImplicitLearningAdvice<AdviceEntry<3>> testData(testDataSrc, 5);
        
        MockHeuristic nullHeuristic;
        EXPECT_CALL(nullHeuristic, probe(testing::_,testing::_)).WillRepeatedly(testing::Return(true));
        
        filterWithRSARHeuristics({&nullHeuristic}, testData, false);
        
        EXPECT_TRUE(testData.hasPotentialAdvice(5));
        EXPECT_FALSE(isEquivalenceAdvised(testData, mkLit(5,1), mkLit(1,0)));
        EXPECT_FALSE(isEquivalenceAdvised(testData, mkLit(1,0), mkLit(5,1)));
        EXPECT_FALSE(isEquivalenceAdvised(testData, mkLit(4,1), mkLit(2,0)));
        EXPECT_FALSE(isEquivalenceAdvised(testData, mkLit(2,0), mkLit(4,1)));
    }
    
    TEST(RSILRSARHeuristicsFilterTests, singleEquivalenceRemoval) {
        Conjectures testDataSrc;
        testDataSrc.addEquivalence(EquivalenceConjecture{std::vector<Lit>{mkLit(5,1), mkLit(1,0)}});
        testDataSrc.addEquivalence(EquivalenceConjecture{std::vector<Lit>{mkLit(4,1), mkLit(2,0)}});
        ImplicitLearningAdvice<AdviceEntry<3>> testData(testDataSrc, 5);
        
        MockHeuristic nullHeuristic;
        
        EXPECT_CALL(nullHeuristic, probe(5, testing::_)).WillRepeatedly(testing::Return(true));
        EXPECT_CALL(nullHeuristic, probe(testing::Lt(5), testing::_)).WillRepeatedly(testing::Return(false));
        
        filterWithRSARHeuristics({&nullHeuristic}, testData, false);
        
        EXPECT_TRUE(testData.hasPotentialAdvice(5));
        EXPECT_FALSE(isEquivalenceAdvised(testData, mkLit(5,1), mkLit(1,0)));
        EXPECT_FALSE(isEquivalenceAdvised(testData, mkLit(1,0), mkLit(5,1)));
        EXPECT_TRUE(isEquivalenceAdvised(testData, mkLit(4,1), mkLit(2,0)));
        EXPECT_TRUE(isEquivalenceAdvised(testData, mkLit(2,0), mkLit(4,1)));
    }
    
    TEST(RSILRSARHeuristicsFilterTests, equivalenceDoesNotGetRemovedWithOnlyBackbone) {
        Conjectures testDataSrc;
        testDataSrc.addEquivalence(EquivalenceConjecture{std::vector<Lit>{mkLit(5,1), mkLit(1,0)}});
        testDataSrc.addEquivalence(EquivalenceConjecture{std::vector<Lit>{mkLit(4,1), mkLit(2,0)}});
        ImplicitLearningAdvice<AdviceEntry<3>> testData(testDataSrc, 5);
        
        MockHeuristic nullHeuristic;
        
        EXPECT_CALL(nullHeuristic, probe(5, testing::_)).WillRepeatedly(testing::Return(true));
        EXPECT_CALL(nullHeuristic, probe(testing::Lt(5), testing::_)).WillRepeatedly(testing::Return(false));
        
        filterWithRSARHeuristics({&nullHeuristic}, testData, true);
        
        EXPECT_TRUE(testData.hasPotentialAdvice(5));
        EXPECT_TRUE(isEquivalenceAdvised(testData, mkLit(5,1), mkLit(1,0)));
        EXPECT_TRUE(isEquivalenceAdvised(testData, mkLit(1,0), mkLit(5,1)));
        EXPECT_TRUE(isEquivalenceAdvised(testData, mkLit(4,1), mkLit(2,0)));
        EXPECT_TRUE(isEquivalenceAdvised(testData, mkLit(2,0), mkLit(4,1)));
    }
    
    TEST(RSILRSARHeuristicsFilterTests, backboneGetsRemovedWithOnlyBackbone) {
        Conjectures testDataSrc;
        testDataSrc.addEquivalence(EquivalenceConjecture{std::vector<Lit>{mkLit(5,1), mkLit(1,0)}});
        testDataSrc.addEquivalence(EquivalenceConjecture{std::vector<Lit>{mkLit(4,1), mkLit(2,0)}});
        testDataSrc.addBackbone(BackboneConjecture{mkLit(6,1)});
        ImplicitLearningAdvice<AdviceEntry<3>> testData(testDataSrc, 6);
        
        MockHeuristic nullHeuristic;
        
        EXPECT_CALL(nullHeuristic, probe(5, testing::_)).WillRepeatedly(testing::Return(true));
        EXPECT_CALL(nullHeuristic, probe(6, testing::_)).WillRepeatedly(testing::Return(true));
        EXPECT_CALL(nullHeuristic, probe(testing::Lt(5), testing::_)).WillRepeatedly(testing::Return(false));
        
        filterWithRSARHeuristics({&nullHeuristic}, testData, true);
        
        EXPECT_TRUE(testData.hasPotentialAdvice(5));
        EXPECT_TRUE(testData.hasPotentialAdvice(6));
        EXPECT_EQ(testData.getAdvice(6).getSize(), 0ull);
        EXPECT_TRUE(isEquivalenceAdvised(testData, mkLit(5,1), mkLit(1,0)));
        EXPECT_TRUE(isEquivalenceAdvised(testData, mkLit(1,0), mkLit(5,1)));
        EXPECT_TRUE(isEquivalenceAdvised(testData, mkLit(4,1), mkLit(2,0)));
        EXPECT_TRUE(isEquivalenceAdvised(testData, mkLit(2,0), mkLit(4,1)));
    }
}
