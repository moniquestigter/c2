#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <fstream>
#include "doctest.h"
#include "expr_ast.h"

#define AND(x, y) (new AndExpr((x), (y)))
#define OR(x, y) (new OrExpr((x), (y)))
#define GT(x, y) (new GTExpr((x), (y)))
#define GTE(x, y) (new GTEExpr((x), (y)))
#define LT(x, y) (new LTExpr((x), (y)))
#define LTE(x, y) (new LTEExpr((x), (y)))
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

std::string runCmd(const std::string& cmd) {
    FILE *stream = popen( cmd.c_str(), "r" );
    REQUIRE (stream != nullptr);

    std::ostringstream ssdata;
    char buffer[256] = {0};

    while(fgets(buffer, sizeof(buffer)-1, stream) != nullptr) {
        ssdata << buffer;
    }
    return ssdata.str();
}

void testCodeGen(const char *filename, ASTNode *n, const std::string& extraCode, int val, int expOffset) {
    resetCodegen();
    n->genCode();

    std::string code = n->code;
    REQUIRE( getOffset() <= expOffset );

    std::ofstream out(filename, std::ios::out|std::ios::trunc);

    REQUIRE (out.is_open());
    out << code << "\n" << extraCode;
    out.close();

    std::string cmd = std::string("nasm -felf32 -otest.o ") + filename + " 2>&1";
    std::string nasmOutput = runCmd(cmd.c_str());
    if (!nasmOutput.empty()) {
        std::cout << "\x1b[0;31mNASM Output:\x1b[0m" <<nasmOutput;
    }
    REQUIRE( nasmOutput.empty() );

    std::string gccOutput = runCmd("gcc -m32 -otest test.o 2>&1");
    if (!gccOutput.empty()) {
        std::cout << "\x1b[0;35mGCC Output:\x1b[0m" << gccOutput;
    }
    int gccError = gccOutput.find("error:");
    REQUIRE(gccError == static_cast<int>(std::string::npos));

    std::string output = runCmd("./test");
    REQUIRE(!output.empty());
    int result = std::stol(output);

    CHECK(result == val);
}

TEST_CASE("And expression") {
    ASTNode *e = AND(ID(x), ID(y));
    
    testCodeGen("and1.asm", new FuncNode(e), "section .data\nx dd 5\ny dd 9\n", 1, 4);
    testCodeGen("and2.asm", new FuncNode(e), "section .data\nx dd 5\ny dd 0\n", 0, 4);
}

TEST_CASE("Or expression") {
    ASTNode *e = OR(ID(x), ID(y));

    testCodeGen("or1.asm", new FuncNode(e), "section .data\nx dd 5\ny dd 0\n", 1, 4);
    testCodeGen("or2.asm", new FuncNode(e), "section .data\nx dd 0\ny dd 0\n", 0, 4);
}

TEST_CASE("Greater Than expression") {
    ASTNode *e = GT(ID(x), ID(y));
    
    testCodeGen("gt1.asm", new FuncNode(e), "section .data\nx dd 200\ny dd 100\n", 1, 4);
    testCodeGen("gt2.asm", new FuncNode(e), "section .data\nx dd -1\ny dd 100\n", 0, 4);
}

TEST_CASE("Greater Or Equal expression") {
    ASTNode *e = GTE(ID(x), ID(y));
    
    testCodeGen("gte1.asm", new FuncNode(e), "section .data\nx dd 200\ny dd 100\n", 1, 4);
    testCodeGen("gte2.asm", new FuncNode(e), "section .data\nx dd 200\ny dd 200\n", 1, 4);
    testCodeGen("gte3.asm", new FuncNode(e), "section .data\nx dd -1\ny dd 100\n", 0, 4);
}

TEST_CASE("Less Than expression") {
    ASTNode *e = LT(ID(x), ID(y));
    
    testCodeGen("lt1.asm", new FuncNode(e), "section .data\nx dd 100\ny dd 200\n", 1, 4);
    testCodeGen("lt2.asm", new FuncNode(e), "section .data\nx dd 0\ny dd -1\n", 0, 4);
}

TEST_CASE("Less Or Equal expression") {
    ASTNode *e = LTE(ID(x), ID(y));
    
    testCodeGen("lte1.asm", new FuncNode(e), "section .data\nx dd 100\ny dd 200\n", 1, 4);
    testCodeGen("lte2.asm", new FuncNode(e), "section .data\nx dd 200\ny dd 200\n", 1, 4);
    testCodeGen("lte3.asm", new FuncNode(e), "section .data\nx dd 10\ny dd -1\n", 0, 4);
}

TEST_CASE("Equal expression") {
    ASTNode *e = EQ(ID(x), ID(y));
    
    testCodeGen("eq1.asm", new FuncNode(e), "section .data\nx dd 100\ny dd 200\n", 0, 4);
    testCodeGen("eq2.asm", new FuncNode(e), "section .data\nx dd 100\ny dd 100\n", 1, 4);
}

TEST_CASE("Not Equal expression") {
    ASTNode *e = NEQ(ID(x), ID(y));
    
    testCodeGen("neq1.asm", new FuncNode(e), "section .data\nx dd 100\ny dd 200\n", 1, 4);
    testCodeGen("neq2.asm", new FuncNode(e), "section .data\nx dd 100\ny dd 100\n", 0, 4);
}

TEST_CASE("Add expression") {
    ASTNode *e = ADD(ADD(ID(x), ID(y)), NUM(300));
    
    testCodeGen("add.asm", new FuncNode(e), "section .data\nx dd 100\ny dd 200\n", 600, 12);
}

TEST_CASE("Sub expression") {
    ASTNode *e = SUB(SUB(ID(x), ID(y)), NUM(100));
    
    testCodeGen("sub.asm", new FuncNode(e), "section .data\nx dd 100\ny dd 200\n", -200, 12);
}

TEST_CASE("Mult expression") {
    ASTNode *e = MUL(MUL(ID(x), NUM(10)), ID(y));
    
    testCodeGen("mul.asm", new FuncNode(e), "section .data\nx dd 33\ny dd 5\n", 1650, 12);
}

TEST_CASE("Div expression") {
    ASTNode *e = DIV(DIV(ID(x), NUM(10)), ID(y));
    
    testCodeGen("div.asm", new FuncNode(e), "section .data\nx dd 330\ny dd 5\n", 6, 12);
}

TEST_CASE("Mod expression") {
    ASTNode *e = MOD(ID(x), NUM(5));
    
    testCodeGen("div.asm", new FuncNode(e), "section .data\nx dd 33\ny dd 5\n", 3, 8);
}

TEST_CASE("Combined expression (Add/Sub)") {
    ASTNode *e = MUL(ADD(NUM(10), ID(y)), SUB(ID(x), NUM(580)));
    
    testCodeGen("add_sub.asm", new FuncNode(e), "section .data\nx dd 33\ny dd 5\n", -8205, 20);
}

TEST_CASE("Combined expression (Div/Mod)") {
    ASTNode *e = MOD(DIV(ADD(ID(x), NUM(10)), ID(y)), NUM(13));
    
    testCodeGen("div_mod.asm", new FuncNode(e), "section .data\nx dd 3300\ny dd 5\n", 12, 20);
}
