#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "expr_ast.h"
#include "expr_cfg.h"

#define AND(x, y) (new AndExpr((x), (y)))
#define OR(x, y) (new OrExpr((x), (y)))
#define NOT(x) (new NotExpr(x))
#define GT(x, y) (new GTExpr((x), (y)))
#define GE(x, y) (new GEExpr((x), (y)))
#define LT(x, y) (new LTExpr((x), (y)))
#define LE(x, y) (new LEExpr((x), (y)))
#define EQ(x, y) (new EQExpr((x), (y)))
#define NEQ(x, y) (new NEQExpr((x), (y)))
#define ADD(x, y) (new AddExpr((x), (y)))
#define SUB(x, y) (new SubExpr((x), (y)))
#define MUL(x, y) (new MulExpr((x), (y)))
#define DIV(x, y) (new DivExpr((x), (y)))
#define MOD(x, y) (new ModExpr((x), (y)))
#define NUM(n) (new NumExpr((n)))
#define ID(x) (new IdentExpr(#x))

#define ASSIGN(lhs, rhs) (new AssignStmt((lhs), (rhs)))
#define PRINT(e) (new PrintStmt((e)))
#define IF(c, t, f) (new IfStmt((c), (t), (f)))
#define WHILE(c, b) (new WhileStmt((c), (b)))
#define SEQ(s1, s2) (new SeqStmt((s1), (s2)))

TEST_CASE("And expression") {
    ASTNode *e = AND(GT(ID(x), ID(y)), GE(ID(y), ID(z)));
    ASTContext actx;

    CFGStmtPair sp = e->toCFG(actx);
    CFGStmt *stmt = CFGRemoveNops(sp.first);
    CFGSetLabels(stmt);

    CHECK(CFGToString(stmt) == "LC0: t2 = x > y : goto LC1\n"
                               "LC1: cbr(t2, LC2, LC3)\n"
                               "LC2: t1 = y >= z : goto LC4\n"
                               "LC3: t0 = 0 : goto LC5\n"
                               "LC4: cbr(t1, LC6, LC3)\n"
                               "LC5: nop : goto end\n"
                               "LC6: t0 = 1 : goto LC5\n"
                               "end:");
}

TEST_CASE("Or expression") {
    ASTNode *e = OR(LT(ID(x), ID(y)), LE(ID(y), ID(z)));
    ASTContext actx;

    CFGStmtPair sp = e->toCFG(actx);
    CFGStmt *stmt = CFGRemoveNops(sp.first);
    CFGSetLabels(stmt);

    CHECK(CFGToString(stmt) == "LC0: t2 = x < y : goto LC1\n"
                               "LC1: cbr(t2, LC2, LC3)\n"
                               "LC2: t0 = 1 : goto LC4\n"
                               "LC3: t1 = y < z : goto LC5\n"
                               "LC4: nop : goto end\n"
                               "LC5: cbr(t1, LC2, LC6)\n"
                               "LC6: t0 = 0 : goto LC4\n"
                               "end:");
}

TEST_CASE("Test expression (Add/Sub/Mul)") {
    ASTNode *e = MUL(ADD(NUM(10), ID(y)), SUB(ID(x), NUM(580)));
    ASTContext actx;
    
    CFGStmtPair sp = e->toCFG(actx);
    CFGStmt *stmt = CFGRemoveNops(sp.first);
    CFGSetLabels(stmt);

    CHECK(CFGToString(stmt) == "LC0: t0 = 10 + y : goto LC1\n"
                               "LC1: t1 = x - 580 : goto LC2\n"
                               "LC2: t2 = t0 * t1 : goto end\n"
                               "end:");
}

TEST_CASE("Test expression (Div/Mod/Add)") {
    ASTNode *e = MOD(DIV(ADD(ID(x), NUM(10)), ID(y)), NUM(13));
    ASTContext actx;
    
    CFGStmtPair sp = e->toCFG(actx);
    CFGStmt *stmt = CFGRemoveNops(sp.first);
    CFGSetLabels(stmt);

    CHECK(CFGToString(stmt) == "LC0: t0 = x + 10 : goto LC1\n"
                               "LC1: t1 = t0 / y : goto LC2\n"
                               "LC2: t2 = t1 % 13 : goto end\n"
                               "end:");
}

TEST_CASE("If Statement") {
    ASTNode *e = IF(AND(NOT(EQ(ID(x), ID(y))), NEQ(ID(y), ID(z))),
                    ASSIGN("x", ADD(ID(y), ID(z))),
                    ASSIGN("x", SUB(ID(y), NUM(10))));
    ASTContext actx;
    
    CFGStmtPair sp = e->toCFG(actx);
    CFGStmt *stmt = CFGRemoveNops(sp.first);
    CFGSetLabels(stmt);

    CHECK(CFGToString(stmt) == "LC0: t3 = x == y : goto LC1\n"
                               "LC1: cbr(t3, LC2, LC3)\n"
                               "LC2: t1 = y - 10 : goto LC4\n"
                               "LC3: t2 = y != z : goto LC5\n"
                               "LC4: x = t1 : goto LC6\n"
                               "LC5: cbr(t2, LC7, LC2)\n"
                               "LC6: nop : goto end\n"
                               "LC7: t0 = y + z : goto LC8\n"
                               "LC8: x = t0 : goto LC6\n"
                               "end:");
}

TEST_CASE("While Statement") {
    ASTNode *e = WHILE(LT(ID(x), ID(y)), ASSIGN("x", ADD(ID(x), NUM(1))));
    ASTContext actx;
    
    CFGStmtPair sp = e->toCFG(actx);
    CFGStmt *stmt = CFGRemoveNops(sp.first);
    CFGSetLabels(stmt);

    CHECK(CFGToString(stmt) == "LC0: t1 = x < y : goto LC1\n"
                               "LC1: cbr(t1, LC2, LC3)\n"
                               "LC2: t0 = x + 1 : goto LC4\n"
                               "LC3: nop : goto end\n"
                               "LC4: x = t0 : goto LC0\n"
                               "end:");
}

TEST_CASE("Sequence Statement") {
    ASTNode *e = SEQ(PRINT(ADD(ID(y), ID(z))), SEQ(ASSIGN("x", ADD(ID(x), NUM(1))),
                     ASSIGN("y", ADD(ID(x), NUM(10)))));
    ASTContext actx;
    
    CFGStmtPair sp = e->toCFG(actx);
    CFGStmt *stmt = CFGRemoveNops(sp.first);
    CFGSetLabels(stmt);

    CHECK(CFGToString(stmt) == "LC0: t0 = y + z : goto LC1\n"
                               "LC1: param t0 : goto LC2\n"
                               "LC2: call print, 1 : goto LC3\n"
                               "LC3: t1 = x + 1 : goto LC4\n"
                               "LC4: x = t1 : goto LC5\n"
                               "LC5: t2 = x + 10 : goto LC6\n"
                               "LC6: y = t2 : goto end\n"
                               "end:");
}