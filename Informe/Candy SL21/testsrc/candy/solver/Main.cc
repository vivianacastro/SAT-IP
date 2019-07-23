#include <zlib.h>

#include <string>
#include <fstream>
#include <streambuf>

#include <gtest/gtest.h>

#include <candy/utils/CNFProblem.h>
#include <candy/core/Solver.h>

using namespace Candy;
using namespace Glucose;

TEST (CandyAddClauseTestPatterns, addClause) {
  CNFProblem formula;
  formula.readClause({mkLit(1), mkLit(2)});
  Solver<> solver;
  solver.addClauses(formula);
  ASSERT_EQ(solver.nClauses(), 1ul);
}

TEST (CandyAddClauseTestPatterns, rejectTautologies) {
  CNFProblem formula;
  formula.readClause(mkLit(1), mkLit(1, true));
  Solver<> solver;
  solver.addClauses(formula);
  ASSERT_EQ(solver.nClauses(), 0ul);
}

TEST (CandyAddClauseTestPatterns, rejectTautologies2) {
  CNFProblem formula;
  formula.readClause({ mkLit(1), mkLit(2), mkLit(1, true) });
  Solver<> solver;
  solver.addClauses(formula);
  ASSERT_EQ(solver.nClauses(), 0ul);
}

TEST (CandyAddClauseTestPatterns, rejectSatisfied) {
  CNFProblem formula;
  // assuming claues are added to solver in the given order
  formula.readClause(mkLit(1));
  formula.readClause(mkLit(1), mkLit(2));
  Solver<> solver;
  solver.addClauses(formula);
  ASSERT_EQ(solver.nClauses(), 0ul);
}

TEST (CandyAddClauseTestPatterns, removeDuplicates) {
  CNFProblem formula;
  formula.readClause({mkLit(1), mkLit(2), mkLit(1)});
  Solver<> solver;
  solver.addClauses(formula);
  Candy::Clause& clause = solver.getClause(0);
  ASSERT_EQ(clause.size(), 2ull);
}

TEST (CandyAddClauseTestPatterns, propagateEarly) {
  CNFProblem formula;
  formula.readClause({mkLit(1)});
  Solver<> solver;
  solver.addClauses(formula);
  ASSERT_EQ(solver.nClauses(), 0ul);
}

TEST (CandyAddClauseTestPatterns, propagateEarly2) {
  CNFProblem formula;
  formula.readClause(mkLit(1, true));
  formula.readClause(mkLit(1), mkLit(2));
  Solver<> solver;
  solver.addClauses(formula);
  ASSERT_EQ(solver.nClauses(), 0ul);;
}

TEST (CandyAddClauseTestPatterns, propagateEarly3) {
  CNFProblem formula;
  formula.readClause(mkLit(1, true));
  formula.readClause({mkLit(1), mkLit(2), mkLit(3)});
  Solver<> solver;
  solver.addClauses(formula);
  ASSERT_EQ(solver.nClauses(), 1ul);
  Candy::Clause& clause = solver.getClause(0);
  ASSERT_EQ(clause.size(), 2ul);
}
