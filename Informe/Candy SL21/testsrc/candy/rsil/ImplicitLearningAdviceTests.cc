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

#include <gtest/gtest.h>
#include <core/SolverTypes.h>
#include <rsil/ImplicitLearningAdvice.h>

#include <iostream>

namespace Candy {
    namespace {
        template<class AdviceEntryType>
        void emptyConjectures_test() {
            static_assert(AdviceEntryType::maxSize == 3, "This test requires advice entries of size 3");
            
            Conjectures testData;
            ImplicitLearningAdvice<AdviceEntryType> underTest(testData, 9);
            
            for (Var i = 0; i < 10; ++i) {
                EXPECT_FALSE(underTest.getAdvice(i).isBackbone());
                EXPECT_EQ(underTest.getAdvice(i).getSize(), 0ull);
            }
        }
        
        template<class AdviceEntryType>
        void singleBackboneConjecture_test() {
            static_assert(AdviceEntryType::maxSize == 3, "This test requires advice entries of size 3");
            
            Conjectures testData;
            Lit backboneLit = mkLit(5,0);
            testData.addBackbone(BackboneConjecture{backboneLit});
            ImplicitLearningAdvice<AdviceEntryType> underTest(testData, 9);
            
            for (Var i = 0; i < 10; ++i) {
                if (i != var(backboneLit)) {
                    EXPECT_FALSE(underTest.getAdvice(i).isBackbone());
                    EXPECT_EQ(underTest.getAdvice(i).getSize(), 0ull);
                }
            }
            
            EXPECT_TRUE(underTest.hasPotentialAdvice(var(backboneLit)));
            EXPECT_TRUE(isBackboneAdvised(underTest, backboneLit));
        }
        
        template<class AdviceEntryType>
        void singleEquivalenceConjecture_test() {
            static_assert(AdviceEntryType::maxSize == 3, "This test requires advice entries of size 3");
            
            Conjectures testData;
            
            testData.addEquivalence(EquivalenceConjecture{std::vector<Lit>{mkLit(5,1), mkLit(1,0)}});
            ImplicitLearningAdvice<AdviceEntryType> underTest(testData, 5);
            
            EXPECT_TRUE(underTest.hasPotentialAdvice(5));
            EXPECT_TRUE(isEquivalenceAdvised(underTest, mkLit(5,1), mkLit(1,0)));
            EXPECT_TRUE(isEquivalenceAdvised(underTest, mkLit(1,1), mkLit(5,0)));
            EXPECT_FALSE(isEquivalenceAdvised(underTest, mkLit(5,1), mkLit(1,1)));
            EXPECT_FALSE(isEquivalenceAdvised(underTest, mkLit(1,1), mkLit(5,1)));
            EXPECT_FALSE(isEquivalenceAdvised(underTest, mkLit(1,1), mkLit(4,1)));
        }
        
        template<class AdviceEntryType>
        void oversizedEquivalenceConjectureIsntAdded_test() {
            static_assert(AdviceEntryType::maxSize == 2, "This test requires advice entries of size 2");
            
            Conjectures testData;
            
            testData.addEquivalence(EquivalenceConjecture{std::vector<Lit>{mkLit(5,1), mkLit(1,0), mkLit(2,1)}});
            ImplicitLearningAdvice<AdviceEntryType> underTest(testData, 5);
            
            EXPECT_FALSE(isEquivalenceAdvised(underTest, mkLit(5,1), mkLit(1,0)));
            EXPECT_FALSE(isEquivalenceAdvised(underTest, mkLit(1,1), mkLit(5,0)));
        }
        
    }
    
    TEST(RSILImplicitLearningAdviceTests, emptyConjectures_AdviceEntry) {
        emptyConjectures_test<AdviceEntry<3>>();
    }
    
    TEST(RSILImplicitLearningAdviceTests, emptyConjectures_BudgetAdviceEntry) {
        emptyConjectures_test<BudgetAdviceEntry<3>>();
    }
    
    TEST(RSILImplicitLearningAdviceTests, singleBackboneConjecture_AdviceEntry) {
        singleBackboneConjecture_test<AdviceEntry<3>>();
    }
    
    TEST(RSILImplicitLearningAdviceTests, singleBackboneConjecture_BudgetAdviceEntry) {
        singleBackboneConjecture_test<BudgetAdviceEntry<3>>();
    }
    
    TEST(RSILImplicitLearningAdviceTests, singleEquivalenceConjecture_AdviceEntry) {
        singleEquivalenceConjecture_test<AdviceEntry<3>>();
    }
    
    TEST(RSILImplicitLearningAdviceTests, singleEquivalenceConjecture_BudgetAdviceEntry) {
        singleEquivalenceConjecture_test<BudgetAdviceEntry<3>>();
    }
    
    TEST(RSILImplicitLearningAdviceTests, oversizedEquivalenceConjectureIsntAdded_AdviceEntry) {
        oversizedEquivalenceConjectureIsntAdded_test<AdviceEntry<2>>();
    }
    
    TEST(RSILImplicitLearningAdviceTests, oversizedEquivalenceConjectureIsntAdded_BudgetAdviceEntry) {
        oversizedEquivalenceConjectureIsntAdded_test<BudgetAdviceEntry<2>>();
    }
}
