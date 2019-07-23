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

#include "SolverMock.h"
#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>

namespace Candy {
    

    lbool SolverMock::solve(const std::vector<Lit> &assumptions, bool doSimp, bool turnOffSimp) {
        /*for (auto lit : assumptions) {
            std::cout << var(lit) << std::endl;
        }*/
        EXPECT_TRUE(std::all_of(assumptions.begin(), assumptions.end(),
                                [this](Lit l) { return var(l) >= m_minAssumptionVar; }));
        EXPECT_TRUE(std::all_of(assumptions.begin(), assumptions.end(),
                                [this](Lit l) { return !this->isEliminated(var(l)); }));
        m_lastAssumptionLits = assumptions;
        return solve();
    }
    
    lbool SolverMock::solve() {
        // TODO: clear assumption lits if appropriate
        m_nClausesAddedSinceLastSolve = 0;
        bool result = m_defaultSolveResult;
        if (m_solveResultInInvocationN.find(m_nInvocations) != m_solveResultInInvocationN.end()) {
            result = m_solveResultInInvocationN[m_nInvocations];
        }
        
        m_eventLog.push_back(SolverMockEvent::SOLVE);
        
        if (m_callOnSolve) {
            m_callOnSolve(m_nInvocations);
        }
        
        ++m_nInvocations;
        
        return lbool(result);
    }
    
    bool SolverMock::addClause(const Cl &clause) {
        EXPECT_TRUE(std::all_of(clause.begin(), clause.end(),
                                [this](Lit l) { return var(l) <= m_maxCreatedVar; }));
        EXPECT_TRUE(std::all_of(clause.begin(), clause.end(),
                                [this](Lit l) { return !this->isEliminated(var(l)); }));
        m_addedClauses.push_back(clause);
        ++m_nClausesAddedSinceLastSolve;
        m_eventLog.push_back(SolverMockEvent::ADD_CLAUSE);
        return true;
    }
    
    void SolverMock::insertClauses(const CNFProblem &problem) {
        // TODO: fix the unscrupolously non-const CNFProblem interface to update
        // m_nClausesAddedSinceLastSolve
        m_eventLog.push_back(SolverMockEvent::ADD_PROBLEM);
        for (int i = 0; i < problem.nVars(); ++i) {
            newVar();
        }
    }
    
    void SolverMock::setFrozen(Var variable, bool frozen) {
        if (frozen) {
            m_frozenVars.insert(variable);
        }
        else {
            m_frozenVars.erase(variable);
        }
    }
    
    bool SolverMock::simplify(const std::vector<Lit>& assumptions) {
        m_eventLog.push_back(SolverMockEvent::SIMPLIFY);
        if (m_callOnSimplify) {
            m_callOnSimplify(m_nInvocations);
        }
        return true;
    }
    
    bool SolverMock::isEliminated(Var var) {
        return m_eliminatedVars.find(var) != m_eliminatedVars.end();
    }
        
    void SolverMock::setIncrementalMode() {
        m_isIncrementalSet = true;
    }
    
    void SolverMock::initNbInitialVars(int n) {
        //std::cout << "initnb:" << n << std::endl;
        m_minAssumptionVar = n;
    }
    
    const std::vector<Lit>& SolverMock::getConflict() {
        return m_conflictLits;
    }
    
    Var SolverMock::newVar() {
        return ++m_maxCreatedVar;
    }
    
    int SolverMock::getNVars() const {
        return m_maxCreatedVar+1;
    }
    
    bool SolverMock::isInConflictingState() const {
        return false;
    };
    
    void SolverMock::mockctrl_setConflictLits(const std::vector<Lit> & conflictLits) {
        m_conflictLits = conflictLits;
    }
    
    bool SolverMock::mockctrl_isIncrementalSet() const noexcept {
        return m_isIncrementalSet;
    }
    
    bool SolverMock::mockctrl_isParsingSet() const noexcept {
        return m_isParsingSet;
    }
    
    bool SolverMock::mockctrl_isAssumptionVar(Var v) const noexcept {
        return m_isIncrementalSet && v >= m_minAssumptionVar;
    }
    
    void SolverMock::mockctrl_setEliminated(Var v, bool elim) {
        if (elim) {
            m_eliminatedVars.insert(v);
        }
        else {
            m_eliminatedVars.erase(v);
        }
    }
    
    void SolverMock::mockctrl_setResultInInvocation(int n, bool result) {
        m_solveResultInInvocationN[n] = result;
    }
    
    void SolverMock::mockctrl_setDefaultSolveResult(bool result) noexcept {
        m_defaultSolveResult = result;
    }
    
    int SolverMock::mockctrl_getAmountOfInvocations() const noexcept {
        return m_nInvocations;
    }
    
    const std::vector<SolverMockEvent>& SolverMock::mockctrl_getEventLog() const noexcept {
        return m_eventLog;
    }
    
    void SolverMock::mockctrl_callOnSolve(std::function<void (int)> func) {
        m_callOnSolve = func;
    }
    
    void SolverMock::mockctrl_callOnSimplify(std::function<void (int)> func) {
        m_callOnSimplify = func;
    }
    
    int SolverMock::mockctrl_getAmountOfClausesAddedSinceLastSolve() const noexcept {
        return m_nClausesAddedSinceLastSolve;
    }
    
    const std::vector<Lit>& SolverMock::mockctrl_getLastAssumptionLits() const noexcept {
        return m_lastAssumptionLits;
    }
    
    
    const std::vector<Cl> &SolverMock::mockctrl_getAddedClauses() const noexcept{
        return m_addedClauses;
    }
    
    std::pair<Var, Var> SolverMock::mockctrl_getNonAssumptionVars() const noexcept {
        if (!m_isIncrementalSet) {
            return {0, m_maxCreatedVar};
        }
        else {
            return {0, m_minAssumptionVar-1};
        }
    }
    
    std::pair<Var, Var> SolverMock::mockctrl_getAssumptionVars() const noexcept {
        if (!m_isIncrementalSet) {
            return {-1, -1};
        }
        else {
            return {m_minAssumptionVar, m_maxCreatedVar};
        }
    }

    
    SolverMock::SolverMock() noexcept : SolverAdapter(),
    m_nInvocations(0),
    m_maxCreatedVar(-1),
    m_conflictLits(),
    m_minAssumptionVar(0),
    m_eliminatedVars(),
    m_frozenVars(),
    m_solveResultInInvocationN(),
    m_defaultSolveResult(false),
    m_callOnSolve(),
    m_callOnSimplify(),
    m_nClausesAddedSinceLastSolve(0),
    m_lastAssumptionLits(),
    m_isIncrementalSet(false),
    m_isParsingSet(false),
    m_eventLog() {
        
    }
    
    SolverMock::~SolverMock() {
        
    }
    
    std::unique_ptr<SolverMock> createSolverMock() {
        return std::unique_ptr<SolverMock>(new SolverMock());
    }
}
