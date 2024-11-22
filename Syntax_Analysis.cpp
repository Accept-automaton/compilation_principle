#include<iostream>
#include<fstream>
#include<sstream>
#include<iomanip>
#include<map>
#include<string>
#include<vector>
#include"Compilation_Principle.h"

std::vector<std::string> stack;
std::vector<int> reader;

// Production class
class Production {
  public:
    std::string l_str;
    std::vector<std::string> r_str;
    std::string prod;
    Production() = default; 

    Production(const std::string &l_str, const std::vector<std::string> &r_str,
               const std::string &prod)
        : l_str(l_str), r_str(r_str), prod(prod) {}
};

Production productions[150];
std::map<int, std::string> map_i2s;
std::map<std::string, int> map_s2i;

void init_syntax_analysis()
{
    map_s2i["int"] = 1;
    map_s2i["void"] = 2;
    map_s2i["return"] = 3;
    map_s2i["const"] = 4;
    map_s2i["main"] = 5;
    map_s2i["struct"] = 6;
    map_s2i["+"] = 7;
    map_s2i["-"] = 8;
    map_s2i["*"] = 9;
    map_s2i["/"] = 10;
    map_s2i["%"] = 11;
    map_s2i["="] = 12;
    map_s2i[">"] = 13;
    map_s2i["<"] = 14;
    map_s2i["=="] = 15;
    map_s2i["<="] = 16;
    map_s2i[">="] = 17;
    map_s2i["!="] = 18;
    map_s2i["&&"] = 19; // Not required to implement, but added in order
    map_s2i["||"] = 20; // Not required to implement, but added in order
    map_s2i["("] = 21;
    map_s2i[")"] = 22;
    map_s2i["{"] = 23;
    map_s2i["}"] = 24;
    map_s2i[";"] = 25;
    map_s2i[","] = 26;
    map_s2i["Ident"] = 27;
    map_s2i["INT"] = 28;
    map_s2i["#"] = 29;
    map_s2i["switch"] = 100;
    map_s2i["float"] = 101;
    map_s2i["char"] = 102;
    map_s2i["case"] = 103;
    map_s2i["default"] = 104;
    map_s2i[":"] = 105;

    map_i2s[1] = "int";
    map_i2s[2] = "void";
    map_i2s[3] = "return";
    map_i2s[4] = "const";
    map_i2s[5] = "main";
    map_i2s[6] = "struct";
    map_i2s[7] = "+";
    map_i2s[8] = "-";
    map_i2s[9] = "*";
    map_i2s[10] = "/";
    map_i2s[11] = "%";
    map_i2s[12] = "=";
    map_i2s[13] = ">";
    map_i2s[14] = "<";
    map_i2s[15] = "==";
    map_i2s[16] = "<=";
    map_i2s[17] = ">=";
    map_i2s[18] = "!=";
    map_i2s[19] = "&&";
    map_i2s[20] = "||";
    map_i2s[21] = "(";
    map_i2s[22] = ")";
    map_i2s[23] = "{";
    map_i2s[24] = "}";
    map_i2s[25] = ";";
    map_i2s[26] = ",";
    map_i2s[27] = "Ident";
    map_i2s[28] = "INT";
    map_i2s[29] = "#";

    map_i2s[100] = "switch";
    map_i2s[101] = "float";
    map_i2s[102] = "char";
    map_i2s[103] = "case";
    map_i2s[104] = "default";
    map_i2s[105] = ":";
    return;
}


void init_Productions() {
    productions[0] = Production("program", {"compUnit"}, "program -> compUnit");
    productions[1] = Production("compUnit", {"decl", "compUnit"},
                                "compUnit -> decl compUnit");
    productions[2] = Production("compUnit", {"funcDef", "compUnit"},
                                "compUnit -> funcDef  compUnit");
    productions[3] = Production("compUnit", {"$"}, "compUnit -> $");
    productions[4] = Production("decl", {"constDecl"}, "decl -> constDecl");
    productions[5] = Production("decl", {"varDecl"}, "decl -> varDecl");
    productions[6] =
        Production("constDecl",
                   {std::to_string(map_s2i["const"]), "bType", "constDef",
                    "argConst", std::to_string(map_s2i[";"])},
                   "constDecl -> const bType constDef argConst ;");
    productions[7] = Production(
        "argConst", {std::to_string(map_s2i[","]), "constDef", "argConst"},
        "argConst -> , constDef argConst");
    productions[8] = Production("argConst", {"$"}, "argConst -> $");
    productions[9] = Production("constDef",
                                {std::to_string(map_s2i["Ident"]),
                                 std::to_string(map_s2i["="]), "constInitVal"},
                                "constDef -> Ident = constInitVal");
    productions[10] = Production("Q", {"constExp"}, "constInitVal -> constExp");
    productions[11] = Production(
        "varDecl",
        {"bType", "varDef", "argVarDecl", std::to_string(map_s2i[";"])},
        "varDecl -> bType varDef argVarDecl ;");
    productions[12] = Production(
        "argVarDecl", {std::to_string(map_s2i[","]), "varDef", "argVarDecl"},
        "argVarDecl -> , varDef argVarDecl");
    productions[13] = Production("argVarDecl", {"$"}, "argVarDecl -> $");
    productions[14] =
        Production("varDef", {std::to_string(map_s2i["Ident"]), "argVarDef"},
                   "varDef -> Ident argVarDef");
    productions[15] =
        Production("argVarDef", {std::to_string(map_s2i["="]), "initVal"},
                   "argVarDef -> = initVal");
    productions[16] = Production("argVarDef", {"$"}, "argVarDef -> $");
    productions[17] = Production("initVal", {"exp"}, "initVal -> exp");
    productions[18] =
        Production("bType", {std::to_string(map_s2i["int"])}, "bType -> int");
    productions[19] =
        Production("funcDef",
                   {"funcType", std::to_string(map_s2i["Ident"]),
                    std::to_string(map_s2i["("]), "funcFParams",
                    std::to_string(map_s2i[")"]), "block"},
                   "funcDef -> funcType Ident ( funcFParams ) block");
    productions[20] = Production("funcType", {std::to_string(map_s2i["void"])},
                                 "funcType -> void");
    productions[21] = Production("funcFParams", {"funcFParam", "argFunctionF"},
                                 "funcFParams -> funcFParam argFunctionF");
    productions[22] = Production("funcFParams", {"$"}, "funcFParams -> $");
    productions[23] =
        Production("argFunctionF",
                   {std::to_string(map_s2i[","]), "funcFParam", "argFunctionF"},
                   "argFunctionF -> , funcFParam argFunctionF");
    productions[24] = Production("argFunctionF", {"$"}, "argFunctionF -> $");
    productions[25] =
        Production("funcFParam", {"bType", std::to_string(map_s2i["Ident"])},
                   "funcFParam -> bType Ident");
    productions[26] = Production("block",
                                 {std::to_string(map_s2i["{"]), "blockItem",
                                  std::to_string(map_s2i["}"])},
                                 "block -> { blockItem }");
    productions[27] = Production("blockItem", {"decl", "blockItem"},
                                 "blockItem -> decl blockItem");
    productions[28] = Production("blockItem", {"stmt", "blockItem"},
                                 "blockItem -> stmt blockItem");
    productions[29] = Production("blockItem", {"$"}, "blockItem -> $");
    productions[30] = Production("stmt", {"exp", std::to_string(map_s2i[";"])},
                                 "stmt -> exp ;");
    productions[31] =
        Production("stmt", {std::to_string(map_s2i[";"])}, "stmt -> ;");
    productions[32] = Production("stmt", {"block"}, "stmt -> block");
    productions[33] = Production("stmt",
                                 {std::to_string(map_s2i["return"]), "argExp",
                                  std::to_string(map_s2i[";"])},
                                 "stmt -> return argExp ;");
    productions[34] = Production("argExp'", {"$"}, "argExp -> $");
    productions[35] = Production("argExp", {"exp"}, "argExp -> exp");
    productions[36] = Production("exp", {"assignExp"}, "exp -> assignExp");
    productions[37] =
        Production("lVal", {std::to_string(map_s2i["Ident"])}, "lVal -> Ident");
    productions[38] = Production(
        "primaryExp",
        {std::to_string(map_s2i["("]), "exp", std::to_string(map_s2i[")"])},
        "primaryExp -> ( exp )");
    productions[39] =
        Production("primaryExp", {"number"}, "primaryExp -> number");
    productions[40] =
        Production("number", {std::to_string(map_s2i["INT"])}, "number -> INT");
    productions[41] =
        Production("unaryOp", {std::to_string(map_s2i["+"])}, "unaryOp ->  +");
    productions[42] =
        Production("unaryOp", {std::to_string(map_s2i["-"])}, "unaryOp ->  -");
    productions[43] =
        Production("unaryOp", {std::to_string(map_s2i["!"])}, "unaryOp ->  !");
    productions[44] = Production("unaryExp", {"unaryOp", "unaryExp"},
                                 "unaryExp -> unaryOp unaryExp");
    productions[45] =
        Production("unaryExp", {std::to_string(map_s2i["Ident"]), "callFunc"},
                   "unaryExp -> Ident callFunc");
    productions[46] = Production("blockItem",
                                 {std::to_string(map_s2i["("]), "funcRParams",
                                  std::to_string(map_s2i[")"])},
                                 "callFunc -> ( funcRParams )");
    productions[47] = Production("callFunc", {"$"}, "callFunc -> $");
    productions[48] =
        Production("unaryExp", {"primaryExp"}, "unaryExp -> primaryExp");
    productions[49] = Production("funcRParams", {"funcRParam", "argFunctionR"},
                                 "funcRParams ->  funcRParam argFunctionR");
    productions[50] = Production("funcRParams", {"$"}, "funcRParams -> $");
    productions[51] =
        Production("argFunctionR",
                   {std::to_string(map_s2i[","]), "funcRParam", "argFunctionR"},
                   "argFunctionR -> , funcRParam argFunctionR");
    productions[52] = Production("argFunctionR", {"$"}, "argFunctionR -> $");
    productions[53] = Production("funcRParam", {"exp"}, "funcRParam ->  exp");
    productions[54] = Production("mulExp", {"unaryExp", "mulExpAtom"},
                                 "mulExp -> unaryExp mulExpAtom");
    productions[55] = Production(
        "mulExpAtom", {std::to_string(map_s2i["*"]), "unaryExp", "mulExpAtom"},
        "mulExpAtom -> * unaryExp mulExpAtom");
    productions[56] = Production(
        "mulExpAtom", {std::to_string(map_s2i["/"]), "unaryExp", "mulExpAtom"},
        "mulExpAtom -> / unaryExp mulExpAtom");
    productions[57] = Production(
        "mulExpAtom", {std::to_string(map_s2i["%"]), "unaryExp", "mulExpAtom"},
        "mulExpAtom -> % unaryExp mulExpAtom");
    productions[58] = Production("mulExpAtom", {"$"}, "mulExpAtom -> $");
    productions[59] = Production("addExp", {"mulExp", "addExpAtom"},
                                 "addExp -> mulExp addExpAtom");
    productions[60] = Production(
        "addExpAtom", {std::to_string(map_s2i["+"]), "mulExp", "addExpAtom"},
        "addExpAtom -> + mulExp addExpAtom");
    productions[61] = Production(
        "addExpAtom", {std::to_string(map_s2i["-"]), "mulExp", "addExpAtom"},
        "addExpAtom -> - mulExp addExpAtom");
    productions[62] = Production("addExpAtom", {"$"}, "addExpAtom -> $");
    productions[63] = Production("relExp", {"addExp", "relExpAtom"},
                                 "relExp -> addExp relExpAtom");
    productions[64] = Production(
        "relExpAtom", {std::to_string(map_s2i["<"]), "addExp", "relExpAtom"},
        "relExpAtom -> < addExp relExpAtom");
    productions[65] = Production(
        "relExpAtom", {std::to_string(map_s2i[">"]), "addExp", "relExpAtom"},
        "relExpAtom -> > addExp relExpAtom");
    productions[66] = Production(
        "relExpAtom", {std::to_string(map_s2i["<="]), "addExp", "relExpAtom"},
        "relExpAtom -> <= addExp relExpAtom");
    productions[67] = Production(
        "relExpAtom", {std::to_string(map_s2i[">="]), "addExp", "relExpAtom"},
        "relExpAtom -> >= addExp relExpAtom");
    productions[68] = Production("relExpAtom", {"$"}, "relExpAtom -> $");
    productions[69] = Production("eqExp", {"relExp", "eqExpAtom"},
                                 "eqExp -> relExp eqExpAtom");
    productions[70] = Production(
        "mulExpAtom", {std::to_string(map_s2i["%"]), "unaryExp", "mulExpAtom"},
        "mulExpAtom -> % unaryExp mulExpAtom");
    productions[71] = Production(
        "eqExpAtom", {std::to_string(map_s2i["!="]), "relExp", "eqExpAtom"},
        "eqExpAtom -> != relExp eqExpAtom");
    productions[72] = Production("eqExpAtom", {"$"}, "eqExpAtom -> $");
    productions[73] = Production("assignExp", {"eqExp", "assignExpAtom"},
                                 "assignExp -> eqExp assignExpAtom");
    productions[74] =
        Production("assignExpAtom",
                   {std::to_string(map_s2i["="]), "eqExp", "assignExpAtom"},
                   "assignExpAtom -> = eqExp assignExpAtom");
    productions[75] = Production("assignExpAtom", {"$"}, "assignExpAtom -> $");
    productions[76] =
        Production("constExp", {"assignExp"}, "constExp ->  assignExp");
    productions[77] = Production("decl", {"structDecl"}, "decl ->structDecl");
    productions[78] =
        Production("structDecl",
                   {std::to_string(map_s2i["struct"]), "structName", "opBlock"},
                   "structDecl -> struct structName opBlock");
    productions[79] =
        Production("structName", {std::to_string(map_s2i["Ident"])},
                   "structName -> Ident");
    productions[80] = Production("opBlock", {"structBlock", "opVar"},
                                 "opBlock -> structBlock opVar");
    productions[81] =
        Production("opBlock", {"structDef"}, "opBlock -> structDef");
    productions[82] = Production("opVar", {"structDef"}, "opVar -> structDef");
    productions[83] =
        Production("opVar", {std::to_string(map_s2i[";"])}, "opVar -> ;");
    productions[84] = Production("structDef",
                                 {std::to_string(map_s2i["Ident"]),
                                  "argStructDef", std::to_string(map_s2i[";"])},
                                 "structDef -> Ident argStructDef ;");
    productions[85] =
        Production("argStructDef",
                   {std::to_string(map_s2i[","]),
                    std::to_string(map_s2i["Ident"]), "argStructDef"},
                   "argStructDef -> Ident argStructDef");
    productions[86] = Production("argStructDef", {"$"}, "argStructDef -> $");
    productions[87] =
        Production("funcFParam",
                   {std::to_string(map_s2i["struct"]), "structName",
                    std::to_string(map_s2i["Ident"])},
                   "funcFParam -> struct structName Ident");
    productions[88] =
        Production("structBlock",
                   {std::to_string(map_s2i["{"]), "structBlockItem",
                    std::to_string(map_s2i["}"])},
                   "structBlock -> { structBlockItem }");
    productions[89] =
        Production("structBlockItem", {"funcDef", "structBlockItem"},
                   "structBlockItem -> funcDef structBlockItem");
    productions[90] = Production("structBlockItem", {"decl", "structBlockItem"},
                                 "structBlockItem -> decl structBlockItem");
    productions[91] = Production("structBlockItem", {"stmt", "structBlockItem"},
                                 "structBlockItem -> stmt structBlockItem");
    productions[92] =
        Production("structBlockItem", {"$"}, "structBlockItem -> $");
    productions[93] = 
        Production("switchDecl", 
                   {std::to_string(map_s2i["switch"]), std::to_string(map_s2i["("]), 
                    "exp", std::to_string(map_s2i[")"]), std::to_string(map_s2i["{"]), 
                    "caseDecl", "defaultCaseDecl", std::to_string(map_s2i["}"])},
                   "switchDecl -> switch ( exp ) { caseDecl defaultCaseDecl }");
    productions[94] = 
        Production("defaultCaseDecl", {"default", ":", std::to_string(map_s2i["{"]),
                    "switchBlockElem", "breakDecl", std::to_string(map_s2i["}"])},
                    "defaultCaseDecl -> default : { switchBlockElem breakDecl }");
    productions[95] = Production("defaultCaseDecl", {"$"}, "defaultCaseDecl -> $");
    productions[96] = Production("caseDecl", {"$"}, "caseDecl -> $ ");
    productions[97] = Production("caseDecl", {"case","constExp",":",std::to_string(map_s2i["{"]),"blockItem","breakDecl",std::to_string(map_s2i["}"]),"caseDecl"}, "caseDecl -> case constExp : { blockItem breakDecl } caseDecl");
    productions[98] = Production("breakDecl", {"$"}, "breakDecl -> $");
    productions[99] = Production("breakDecl", {"break", std::to_string(map_s2i[";"])}, "breakDecl -> break;");

    // need add something
    
}

// Initialize input data function
std::string initLine(std::string line) {
    if (line.empty()) {
        return "";
    }

    size_t strStartIndex_0 = line.find("\t");
    if (strStartIndex_0 != std::string::npos) {
        line = line.substr(strStartIndex_0);
    }

    size_t strStartIndex_1 = line.find("<");
    size_t strStartIndex_2 = line.find(",");
    size_t strStartIndex_3 = line.find(">");

    if (strStartIndex_1 != std::string::npos &&
        strStartIndex_2 != std::string::npos) {
        std::string token = line.substr(strStartIndex_1 + 1,
                                        strStartIndex_2 - strStartIndex_1 - 1);
        if (token == "Ident") {
            return "27,Ident";
        } else if (token == "INT") {
            return "28,INT";
        }
    }

    if (strStartIndex_2 != std::string::npos &&
        strStartIndex_3 != std::string::npos) {
        std::string value = line.substr(strStartIndex_2 + 1,
                                        strStartIndex_3 - strStartIndex_2 - 1);
        int intValue = std::stoi(value);
        return value + "," + map_i2s[intValue];
    }

    return "";
}

static bool match(int stackTop, int readerTop) {
    try {
        // If no exception is thrown, it's a terminal symbol
        int stackTopVal = std::stoi(stack[stackTop]);
        if (stackTopVal == reader[0]) {
            stack.erase(stack.begin() + stackTop);
            reader.erase(reader.begin() + readerTop);
            return true;
        } else {
            return false;
        }
    } catch (const std::invalid_argument &e) {
        // If an exception is thrown, it's a non-terminal symbol
        return false;
    } catch (const std::out_of_range &e) {
        // Handle potential out_of_range exception
        return false;
    }
}

// Push into stack
int stackPush(int stackTop, const Production &production) {
    int len = production.r_str.size();
    stack.pop_back();

    if (production.r_str[0] != "$") {
        for (int i = len - 1; i >= 0; i--) {
            stack.push_back(production.r_str[i]);
        }
        return len - 1;
    }

    return -1;
}

int ll1_table(int stackTop, int readerTop) {
    // need add something
    if (stack[stackTop] == "addExp") {
        if (map_i2s[reader[readerTop]] == "!") {
            return 59;
        } else if (map_i2s[reader[readerTop]] == "(") {
            return 59;
        } else if (map_i2s[reader[readerTop]] == "+") {
            return 59;
        } else if (map_i2s[reader[readerTop]] == "-") {
            return 59;
        } else if (map_i2s[reader[readerTop]] == "INT") {
            return 59;
        } else if (map_i2s[reader[readerTop]] == "Ident") {
            return 59;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "addExpAtom") {
        if (map_i2s[reader[readerTop]] == "!=") {
            return 62;
        } else if (map_i2s[reader[readerTop]] == ")") {
            return 62;
        } else if (map_i2s[reader[readerTop]] == "+") {
            return 60;
        } else if (map_i2s[reader[readerTop]] == ",") {
            return 62;
        } else if (map_i2s[reader[readerTop]] == "-") {
            return 61;
        } else if (map_i2s[reader[readerTop]] == ";") {
            return 62;
        } else if (map_i2s[reader[readerTop]] == "<") {
            return 62;
        } else if (map_i2s[reader[readerTop]] == "<=") {
            return 62;
        } else if (map_i2s[reader[readerTop]] == "=") {
            return 62;
        } else if (map_i2s[reader[readerTop]] == "==") {
            return 62;
        } else if (map_i2s[reader[readerTop]] == ">") {
            return 62;
        } else if (map_i2s[reader[readerTop]] == ">=") {
            return 62;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "argConst") {
        if (map_i2s[reader[readerTop]] == ",") {
            return 7;
        } else if (map_i2s[reader[readerTop]] == ";") {
            return 8;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "argExp") {
        if (map_i2s[reader[readerTop]] == "!") {
            return 35;
        } else if (map_i2s[reader[readerTop]] == "(") {
            return 35;
        } else if (map_i2s[reader[readerTop]] == "+") {
            return 35;
        } else if (map_i2s[reader[readerTop]] == "-") {
            return 35;
        } else if (map_i2s[reader[readerTop]] == ";") {
            return 34;
        } else if (map_i2s[reader[readerTop]] == "INT") {
            return 35;
        } else if (map_i2s[reader[readerTop]] == "Ident") {
            return 35;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "argFunctionF") {
        if (map_i2s[reader[readerTop]] == ")") {
            return 24;
        } else if (map_i2s[reader[readerTop]] == ",") {
            return 23;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "argFunctionR") {
        if (map_i2s[reader[readerTop]] == ")") {
            return 52;
        } else if (map_i2s[reader[readerTop]] == ",") {
            return 51;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "argStructDef") {
        if (map_i2s[reader[readerTop]] == ",") {
            return 85;
        } else if (map_i2s[reader[readerTop]] == ";") {
            return 86;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "argVarDecl") {
        if (map_i2s[reader[readerTop]] == ",") {
            return 12;
        } else if (map_i2s[reader[readerTop]] == ";") {
            return 13;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "argVarDef") {
        if (map_i2s[reader[readerTop]] == ",") {
            return 16;
        } else if (map_i2s[reader[readerTop]] == ";") {
            return 16;
        } else if (map_i2s[reader[readerTop]] == "=") {
            return 15;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "assignExp") {
        if (map_i2s[reader[readerTop]] == "!") {
            return 73;
        } else if (map_i2s[reader[readerTop]] == "(") {
            return 73;
        } else if (map_i2s[reader[readerTop]] == "+") {
            return 73;
        } else if (map_i2s[reader[readerTop]] == "-") {
            return 73;
        } else if (map_i2s[reader[readerTop]] == "INT") {
            return 73;
        } else if (map_i2s[reader[readerTop]] == "Ident") {
            return 73;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "assignExpAtom") {
        if (map_i2s[reader[readerTop]] == ")") {
            return 75;
        } else if (map_i2s[reader[readerTop]] == ",") {
            return 75;
        } else if (map_i2s[reader[readerTop]] == ";") {
            return 75;
        } else if (map_i2s[reader[readerTop]] == "=") {
            return 74;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "bType") {
        if (map_i2s[reader[readerTop]] == "int") {
            return 18;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "block") {
        if (map_i2s[reader[readerTop]] == "{") {
            return 26;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "blockItem") {
        if (map_i2s[reader[readerTop]] == "!") {
            return 28;
        } else if (map_i2s[reader[readerTop]] == "(") {
            return 28;
        } else if (map_i2s[reader[readerTop]] == "+") {
            return 28;
        } else if (map_i2s[reader[readerTop]] == "-") {
            return 28;
        } else if (map_i2s[reader[readerTop]] == ";") {
            return 28;
        } else if (map_i2s[reader[readerTop]] == "+") {
            return 28;
        } else if (map_i2s[reader[readerTop]] == "INT") {
            return 28;
        } else if (map_i2s[reader[readerTop]] == "Ident") {
            return 28;
        } else if (map_i2s[reader[readerTop]] == "const") {
            return 27;
        } else if (map_i2s[reader[readerTop]] == "int") {
            return 27;
        } else if (map_i2s[reader[readerTop]] == "return") {
            return 28;
        } else if (map_i2s[reader[readerTop]] == "{") {
            return 28;
        } else if (map_i2s[reader[readerTop]] == "}") {
            return 29;
        } else if (map_i2s[reader[readerTop]] == "struct") {
            return 27;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "callFunc") {
        if (map_i2s[reader[readerTop]] == "!=") {
            return 47;
        } else if (map_i2s[reader[readerTop]] == "%") {
            return 47;
        } else if (map_i2s[reader[readerTop]] == "(") {
            return 46;
        } else if (map_i2s[reader[readerTop]] == ")") {
            return 47;
        } else if (map_i2s[reader[readerTop]] == "*") {
            return 47;
        } else if (map_i2s[reader[readerTop]] == "+") {
            return 47;
        } else if (map_i2s[reader[readerTop]] == ",") {
            return 47;
        } else if (map_i2s[reader[readerTop]] == "-") {
            return 47;
        } else if (map_i2s[reader[readerTop]] == "/") {
            return 47;
        } else if (map_i2s[reader[readerTop]] == ";") {
            return 47;
        } else if (map_i2s[reader[readerTop]] == "<") {
            return 47;
        } else if (map_i2s[reader[readerTop]] == "<=") {
            return 47;
        } else if (map_i2s[reader[readerTop]] == "=") {
            return 47;
        } else if (map_i2s[reader[readerTop]] == "==") {
            return 47;
        } else if (map_i2s[reader[readerTop]] == ">") {
            return 47;
        } else if (map_i2s[reader[readerTop]] == ">=") {
            return 47;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "compUnit") {
        if (map_i2s[reader[readerTop]] == "#") {
            return 3;
        } else if (map_i2s[reader[readerTop]] == "const") {
            return 1;
        } else if (map_i2s[reader[readerTop]] == "int") {
            return 1;
        } else if (map_i2s[reader[readerTop]] == "void") {
            return 2;
        } else if (map_i2s[reader[readerTop]] == "struct") {
            return 1;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "constDecl") {
        if (map_i2s[reader[readerTop]] == "const") {
            return 6;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "constDef") {
        if (map_i2s[reader[readerTop]] == "Ident") {
            return 9;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "constExp") {
        if (map_i2s[reader[readerTop]] == "!") {
            return 76;
        } else if (map_i2s[reader[readerTop]] == "(") {
            return 76;
        } else if (map_i2s[reader[readerTop]] == "+") {
            return 76;
        } else if (map_i2s[reader[readerTop]] == "-") {
            return 76;
        } else if (map_i2s[reader[readerTop]] == "INT") {
            return 76;
        } else if (map_i2s[reader[readerTop]] == "Ident") {
            return 76;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "constInitVal") {
        if (map_i2s[reader[readerTop]] == "!") {
            return 10;
        } else if (map_i2s[reader[readerTop]] == "(") {
            return 10;
        } else if (map_i2s[reader[readerTop]] == "+") {
            return 10;
        } else if (map_i2s[reader[readerTop]] == "-") {
            return 10;
        } else if (map_i2s[reader[readerTop]] == "INT") {
            return 10;
        } else if (map_i2s[reader[readerTop]] == "Ident") {
            return 10;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "decl") {
        if (map_i2s[reader[readerTop]] == "const") {
            return 4;
        } else if (map_i2s[reader[readerTop]] == "int") {
            return 5;
        } else if (map_i2s[reader[readerTop]] == "struct") {
            return 77;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "eqExp") {
        if (map_i2s[reader[readerTop]] == "!") {
            return 69;
        } else if (map_i2s[reader[readerTop]] == "(") {
            return 69;
        } else if (map_i2s[reader[readerTop]] == "+") {
            return 69;
        } else if (map_i2s[reader[readerTop]] == "-") {
            return 69;
        } else if (map_i2s[reader[readerTop]] == "INT") {
            return 69;
        } else if (map_i2s[reader[readerTop]] == "Ident") {
            return 69;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "eqExpAtom") {
        if (map_i2s[reader[readerTop]] == "!=") {
            return 71;
        } else if (map_i2s[reader[readerTop]] == ")") {
            return 72;
        } else if (map_i2s[reader[readerTop]] == ",") {
            return 72;
        } else if (map_i2s[reader[readerTop]] == ";") {
            return 72;
        } else if (map_i2s[reader[readerTop]] == "=") {
            return 72;
        } else if (map_i2s[reader[readerTop]] == "==") {
            return 70;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "exp") {
        if (map_i2s[reader[readerTop]] == "!") {
            return 36;
        } else if (map_i2s[reader[readerTop]] == "(") {
            return 36;
        } else if (map_i2s[reader[readerTop]] == "+") {
            return 36;
        } else if (map_i2s[reader[readerTop]] == "-") {
            return 36;
        } else if (map_i2s[reader[readerTop]] == "INT") {
            return 36;
        } else if (map_i2s[reader[readerTop]] == "Ident") {
            return 36;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "funcDef") {
        if (map_i2s[reader[readerTop]] == "void") {
            return 19;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "funcFParam") {
        if (map_i2s[reader[readerTop]] == "int") {
            return 25;
        } else if (map_i2s[reader[readerTop]] == "struct") {
            return 87;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "funcFParams") {
        if (map_i2s[reader[readerTop]] == ")") {
            return 22;
        } else if (map_i2s[reader[readerTop]] == "int") {
            return 21;
        } else if (map_i2s[reader[readerTop]] == "struct") {
            return 21;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "funcRParam") {
        if (map_i2s[reader[readerTop]] == "!") {
            return 53;
        } else if (map_i2s[reader[readerTop]] == "(") {
            return 53;
        } else if (map_i2s[reader[readerTop]] == "+") {
            return 53;
        } else if (map_i2s[reader[readerTop]] == "-") {
            return 53;
        } else if (map_i2s[reader[readerTop]] == "INT") {
            return 53;
        } else if (map_i2s[reader[readerTop]] == "Ident") {
            return 53;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "funcRParams") {
        if (map_i2s[reader[readerTop]] == "!") {
            return 49;
        } else if (map_i2s[reader[readerTop]] == "(") {
            return 49;
        } else if (map_i2s[reader[readerTop]] == "+") {
            return 49;
        } else if (map_i2s[reader[readerTop]] == "-") {
            return 49;
        } else if (map_i2s[reader[readerTop]] == "INT") {
            return 49;
        } else if (map_i2s[reader[readerTop]] == "Ident") {
            return 49;
        } else if (map_i2s[reader[readerTop]] == ")") {
            return 50;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "funcType") {
        if (map_i2s[reader[readerTop]] == "void") {
            return 20;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "initVal") {
        if (map_i2s[reader[readerTop]] == "!") {
            return 17;
        } else if (map_i2s[reader[readerTop]] == "(") {
            return 17;
        } else if (map_i2s[reader[readerTop]] == "+") {
            return 17;
        } else if (map_i2s[reader[readerTop]] == "-") {
            return 17;
        } else if (map_i2s[reader[readerTop]] == "INT") {
            return 17;
        } else if (map_i2s[reader[readerTop]] == "Ident") {
            return 17;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "lVal") {
        if (map_i2s[reader[readerTop]] == "Ident") {
            return 37;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "mulExp") {
        if (map_i2s[reader[readerTop]] == "!") {
            return 54;
        } else if (map_i2s[reader[readerTop]] == "(") {
            return 54;
        } else if (map_i2s[reader[readerTop]] == "+") {
            return 54;
        } else if (map_i2s[reader[readerTop]] == "-") {
            return 54;
        } else if (map_i2s[reader[readerTop]] == "INT") {
            return 54;
        } else if (map_i2s[reader[readerTop]] == "Ident") {
            return 54;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "mulExpAtom") {
        if (map_i2s[reader[readerTop]] == "!") {
            return 58;
        } else if (map_i2s[reader[readerTop]] == "%") {
            return 57;
        } else if (map_i2s[reader[readerTop]] == ")") {
            return 58;
        } else if (map_i2s[reader[readerTop]] == "*") {
            return 55;
        } else if (map_i2s[reader[readerTop]] == "+") {
            return 58;
        } else if (map_i2s[reader[readerTop]] == ",") {
            return 58;
        } else if (map_i2s[reader[readerTop]] == "-") {
            return 58;
        } else if (map_i2s[reader[readerTop]] == "/") {
            return 56;
        } else if (map_i2s[reader[readerTop]] == ";") {
            return 58;
        } else if (map_i2s[reader[readerTop]] == "<") {
            return 58;
        } else if (map_i2s[reader[readerTop]] == "<=") {
            return 58;
        } else if (map_i2s[reader[readerTop]] == "=") {
            return 58;
        } else if (map_i2s[reader[readerTop]] == "==") {
            return 58;
        } else if (map_i2s[reader[readerTop]] == ">=") {
            return 58;
        } else if (map_i2s[reader[readerTop]] == ">") {
            return 58;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "number") {
        if (map_i2s[reader[readerTop]] == "INT") {
            return 40;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "opBlock") {
        if (map_i2s[reader[readerTop]] == "Ident") {
            return 81;
        } else if (map_i2s[reader[readerTop]] == "{") {
            return 80;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "opVar") {
        if (map_i2s[reader[readerTop]] == ";") {
            return 83;
        } else if (map_i2s[reader[readerTop]] == "Ident") {
            return 82;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "primaryExp") {
        if (map_i2s[reader[readerTop]] == "(") {
            return 38;
        } else if (map_i2s[reader[readerTop]] == "INT") {
            return 39;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "program") {
        if (map_i2s[reader[readerTop]] == "#") {
            return 0;
        } else if (map_i2s[reader[readerTop]] == "const") {
            return 0;
        } else if (map_i2s[reader[readerTop]] == "int") {
            return 0;
        } else if (map_i2s[reader[readerTop]] == "void") {
            return 0;
        } else if (map_i2s[reader[readerTop]] == "struct") {
            return 0;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "relExp") {
        if (map_i2s[reader[readerTop]] == "!") {
            return 63;
        } else if (map_i2s[reader[readerTop]] == "(") {
            return 63;
        } else if (map_i2s[reader[readerTop]] == "+") {
            return 63;
        } else if (map_i2s[reader[readerTop]] == "-") {
            return 63;
        } else if (map_i2s[reader[readerTop]] == "INT") {
            return 63;
        } else if (map_i2s[reader[readerTop]] == "Ident") {
            return 63;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "relExpAtom") {
        if (map_i2s[reader[readerTop]] == "!=") {
            return 68;
        } else if (map_i2s[reader[readerTop]] == ")") {
            return 68;
        } else if (map_i2s[reader[readerTop]] == ",") {
            return 68;
        } else if (map_i2s[reader[readerTop]] == ";") {
            return 68;
        } else if (map_i2s[reader[readerTop]] == "<") {
            return 64;
        } else if (map_i2s[reader[readerTop]] == "<=") {
            return 66;
        } else if (map_i2s[reader[readerTop]] == "==") {
            return 68;
        } else if (map_i2s[reader[readerTop]] == "=") {
            return 68;
        } else if (map_i2s[reader[readerTop]] == ">") {
            return 65;
        } else if (map_i2s[reader[readerTop]] == ">=") {
            return 67;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "stmt") {
        if (map_i2s[reader[readerTop]] == "!") {
            return 30;
        } else if (map_i2s[reader[readerTop]] == "(") {
            return 30;
        } else if (map_i2s[reader[readerTop]] == "+") {
            return 30;
        } else if (map_i2s[reader[readerTop]] == "-") {
            return 30;
        } else if (map_i2s[reader[readerTop]] == "INT") {
            return 30;
        } else if (map_i2s[reader[readerTop]] == "Ident") {
            return 30;
        } else if (map_i2s[reader[readerTop]] == ";") {
            return 31;
        } else if (map_i2s[reader[readerTop]] == "return") {
            return 33;
        } else if (map_i2s[reader[readerTop]] == "{") {
            return 32;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "structDecl") {
        if (map_i2s[reader[readerTop]] == "struct") {
            return 78;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "structDef") {
        if (map_i2s[reader[readerTop]] == "Ident") {
            return 84;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "structName") {
        if (map_i2s[reader[readerTop]] == "Ident") {
            return 79;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "unaryExp") {
        if (map_i2s[reader[readerTop]] == "!") {
            return 44;
        } else if (map_i2s[reader[readerTop]] == "(") {
            return 44;
        } else if (map_i2s[reader[readerTop]] == "+") {
            return 44;
        } else if (map_i2s[reader[readerTop]] == "(") {
            return 48;
        } else if (map_i2s[reader[readerTop]] == "INT") {
            return 48;
        } else if (map_i2s[reader[readerTop]] == "Ident") {
            return 45;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "unaryOp") {
        if (map_i2s[reader[readerTop]] == "!") {
            return 43;
        } else if (map_i2s[reader[readerTop]] == "+") {
            return 41;
        } else if (map_i2s[reader[readerTop]] == "-") {
            return 42;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "varDecl") {
        if (map_i2s[reader[readerTop]] == "int") {
            return 11;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "varDef") {
        if (map_i2s[reader[readerTop]] == "Ident") {
            return 14;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "structBlock") {
        if (map_i2s[reader[readerTop]] == "{") {
            return 88;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "structBlockItem") {
        if (map_i2s[reader[readerTop]] == "!") {
            return 91;
        } else if (map_i2s[reader[readerTop]] == "(") {
            return 91;
        } else if (map_i2s[reader[readerTop]] == "+") {
            return 91;
        } else if (map_i2s[reader[readerTop]] == "-") {
            return 91;
        } else if (map_i2s[reader[readerTop]] == ";") {
            return 91;
        } else if (map_i2s[reader[readerTop]] == "INT") {
            return 91;
        } else if (map_i2s[reader[readerTop]] == "const") {
            return 90;
        } else if (map_i2s[reader[readerTop]] == "int") {
            return 90;
        } else if (map_i2s[reader[readerTop]] == "Ident") {
            return 91;
        } else if (map_i2s[reader[readerTop]] == "return") {
            return 91;
        } else if (map_i2s[reader[readerTop]] == "void") {
            return 89;
        } else if (map_i2s[reader[readerTop]] == "{") {
            return 91;
        } else if (map_i2s[reader[readerTop]] == "}") {
            return 92;
        } else if (map_i2s[reader[readerTop]] == "struct") {
            return 90;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "switchDecl") {
        if (map_i2s[reader[readerTop]] == "switch") {
            return 94;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "defaultCaseDecl") {
        if (map_i2s[reader[readerTop]] == "}") {
            return 96;
        } else if (map_i2s[reader[readerTop]] == "default") {
            return 95;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "caseDecl") {
        if (map_i2s[reader[readerTop]] == "}") {
            return 97;
        } else if (map_i2s[reader[readerTop]] == "default") {
            return 97;
        } else if (map_i2s[reader[readerTop]] == "case") {
            return 98;
        } else {
            return -1;
        }
    } else if (stack[stackTop] == "breakDecl") {
        if (map_i2s[reader[readerTop]] == "}") {
            return 99;
        } else if (map_i2s[reader[readerTop]] == "break") {
            return 100;
        } else {
            return -1;
        }
    } else {
        std::cout << "语法错误" << std::endl;
    }
    return -1;
}



void syntax_analysis(std::vector< std::pair<std::string, std::pair<std::string, std::string> > > lexcial, const char *syntax_output_path)
{
    std::vector< std::pair<std::string, std::pair<std::string, std::string> > > syntax;
    
    //code
    int stackTop = 1;
    int readerTop = 0;
    int index = 0;
    
    init_syntax_analysis();

    init_Productions();
    
    stack.insert(stack.begin(), std::to_string(map_s2i["#"]));
    stack.push_back("program");

    //input to stack
    for (auto item:lexcial)
    {   
        if (item.second.first == "Ident")
        {
            reader.push_back(27);
        }
        else if (item.second.first == "INT")
        {
            reader.push_back(28);
        }
        else
        {
            reader.push_back(std::stoi(item.second.second));
        }
        
    }


    reader.push_back(map_s2i["#"]);

    std::stringstream outputBuffer;
    //syntax analysis

    while (stackTop >= 0) {
        std::cout << std::left << std::setw(5) << ++index;
        std::cout << std::left << std::setw(10) << "Current stack:";
        outputBuffer << index << "\t";

        std::string str;
        try {
            str = map_i2s[std::stoi(stack[stackTop])];
            if (!str.empty()) {
                std::cout << std::left << std::setw(30) << str;
                outputBuffer << str;
            }
        } catch (const std::invalid_argument &) {
            std::cout << std::left << std::setw(30) << stack[stackTop];
            outputBuffer << stack[stackTop];
        }

        std::cout << std::left << std::setw(50)
                  << "#" + map_i2s[reader[0]] + " ";
        outputBuffer << "#" << map_i2s[reader[0]] << " ";

        if (match(stackTop, readerTop)) {
            stackTop--;
            std::cout << "move" << std::endl;
            outputBuffer << "\tmove\n";
        } else {
            int i = ll1_table(stackTop, readerTop);
            if (i == -1 && (!stack.empty() || !reader.empty())) {
                std::cout << "Error" << std::endl;
                outputBuffer << "Error";
                exit(EXIT_FAILURE);
            } else {
                stackTop += stackPush(stackTop, productions[i]);
                std::cout << std::left << std::setw(30) << "reduction"
                          << std::endl;
                outputBuffer << "\treduction\n";
            }
        }
    }

    if (stackTop == -1) {
        std::cout << "Accept" << std::endl;
        outputBuffer << "Accept";
    }


    std::ofstream syntax_output_file(syntax_output_path);
    if (!syntax_output_file)
    {
        std::cerr << "Error opening file: " << syntax_output_path << std::endl;
        exit(EXIT_FAILURE);
    }
    syntax_output_file << outputBuffer.str();
    syntax_output_file.close();
    return;
}

