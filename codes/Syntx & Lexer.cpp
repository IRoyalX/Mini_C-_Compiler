#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct Token {
    int no;
    int row;
    int col;
    int type;
    string value;
};
//---------------------------------------------------------Function Def---------------------------------------------------------
bool Digit(char);
bool Letter(char);
bool Keyword(string);
vector<Token> lexer(string);
void match(string);
void Program();
void Headers();
void Keyword1();
void HeadNames();
void FunctionList();
void Function();
void Params();
void HVParams();
void FBody();
void StatementList();
void Statement();
void IFst();
void EPart();
void ElseIfPart();
void ElsePart();
void FORst();
void SCN();
void WHILEst();
void DOWHILEst();
void SWITCHst();
void CaseList();
void CASEst();
void DefCase();
void ASSIGNst();
void CALLst();
void Var();
void VDef1();
void VDef2();
void IN();
void CL();
void OUT();
void CO();
void PR();
void Condition();
void Body();
void Exp();
void Term();
void Factor();
string OOP();
string COP();
string SOP();
string Val();
string TYPEst();
void prout();
//-----------------------------------------------------global variables-------------------------------------------------------------
bool otp = 0;
int currentTokenIndex = 0;
vector<string> tokens;
vector<int> rows;
vector<string> errors;
//-----------------------------------------------------------Main-------------------------------------------------------------------
int main() {
    vector<string> tokens;
    vector<int> rows;
    string filename , file = "" , line;
    cout << "Enter your file name: ";
    getline(cin, filename);
    ifstream infile(filename);
    if(!infile){
        cout << "\nERR: Can't open file";
        return 0;
    }
    else {
        while (getline(infile, line)){
            file += line + "\n";
        }
        vector<Token> tkn = lexer(file);
        vector<string> TokenTypeNames = {
            "UNKNOWN", "KEYWORD", "ID", "E_DIV", "DIV", "E_MUL", "MUL", "E_REM", "REM", "INC",
            "E_PLS", "PLS", "DEC", "E_DEC", "PTR", "MNS", "NE", "N", "AND", "B_AND_ASG", "B_AND",
            "OR", "B_OR_ASG", "B_OR", "E_GT", "GT", "E_LT", "LT", "SCP_RES", "CLN", "B_XOR_ASG",
            "B_XOR", "STR", "R_BRC", "L_BRC", "L_FN", "R_FN", "L_BRK", "R_BRK", "COMMA", "SEMI",
            "Q", "B_NOT", "NO", "R_ASG", "RST", "L_ASG", "LST", "CHR", "DOT", "HASH", "ASG", "EQ" , "COMMENT"};
        for(int i = 0;i < tkn.size();i++) {
            Token out = tkn[i];
            if (out.type == 1) {
                vector<string> wrds = {"cin" , "cout" , "include", "define","asm","auto","bool","break","case","catch","char","class","const","constexpr","continue","decltype","default","delete","do","double",
                                        "dynamic_cast","else","enum","explicit","export","extern","false","final","float","for","friend",
                                        "goto","if","inline","int","long","mutable","namespace","new","noexcept","nullptr","operator","override","private","protected",
                                        "public","register","reinterpret_cast","return","short","signed","sizeof","static","static_assert","static_cast","struct","switch","template","this",
                                        "thread_local","throw","true","try","typedef","typeid","typename","union","unsigned","using","virtual","void","volatile","wchar_t","while"};
                for (string wrd : wrds)
                    if(out.value == wrd)
                        ::tokens.push_back(wrd);
            }
            else
                ::tokens.push_back(TokenTypeNames[out.type]);
            ::rows.push_back(out.row);
        }
    }

    Program();
    prout();
    return 0;
}
//-----------------------------------------------------Lexical Analyzer--------------------------------------------------------------
enum class TokenType {
  UNKNOWN,KEYWORD,ID,E_DIV,DIV,E_MUL,MUL,E_REM,REM,INC,E_PLS,PLS,DEC,E_DEC,PTR,
  MNS,NE,N,AND,B_AND_ASG,B_AND,OR,B_OR_ASG,B_OR,E_GT,GT,E_LT,LT,
  SCP_RES,CLN,B_XOR_ASG,B_XOR,STR,R_BRC,L_BRC,L_FN,R_FN,L_BRK,
  R_BRK,COMMA,SEMI,Q,B_NOT,NO,R_ASG,RST,L_ASG,LST,CHR,DOT,HASH,ASG,EQ,COMMENT
};
Token createToken(int no = -1, int row = -1, int col = -1, TokenType type = TokenType::UNKNOWN, string value = "ERR:NULL") {
    Token token;
    token.no = no;
    token.row = row;
    token.col = col;
    token.type = static_cast<int>(type);
    token.value = value;
    return token;
}
bool Digit(char ch){
    return (ch >= '0' && ch <= '9');
}
bool Letter(char ch){
    return ((ch >= 'a' && ch <= 'z')||(ch >= 'A' && ch <= 'Z'));
}
bool Keyword(string str){
    vector<string> wrds = {"cin" , "cout", "include","define","asm","auto","bool","break","case","catch","char","class","const","constexpr","continue","decltype","default","delete","do","double",
    "dynamic_cast","else","enum","explicit","export","extern","false","final","float","for","friend",
    "goto","if","inline","int","long","mutable","namespace","new","noexcept","nullptr","operator","override","private","protected",
    "public","register","reinterpret_cast","return","short","signed","sizeof","static","static_assert","static_cast","struct","switch","template","this",
    "thread_local","throw","true","try","typedef","typeid","typename","union","unsigned","using","virtual","void","volatile","wchar_t","while"};
    str.pop_back();
    for (string wrd : wrds)
        if(str == wrd)
            return true;
    return false;
}
vector<Token> lexer(string input) {
    bool err;
    int row = 1,col = 0,no = 0;
    char r;
    string lex = "";
    TokenType type = TokenType::UNKNOWN;
    vector<Token> tokens;

    int i = 0, State = 0;
    while (i < input.length()) {

        r = input[i++];
        lex += r;
        switch(State) {
        case 0:
            State = -1;
            if(r == '\n'){row++, col = 0, lex = "", State = 0;}
            else if(r == '\t' || r == ' '){lex = "", State = 0;}
            else if(Letter(r))State = 1;
            else if(Digit(r))State = 2;
            else if(r == '/')State = 3;
            else if(r == '*')State = 4;
            else if(r == '%')State = 5;
            else if(r == '+')State = 6;
            else if(r == '-')State = 7;
            else if(r == '!')State = 8;
            else if(r == '&')State = 9;
            else if(r == '|')State = 10;
            else if(r == '>')State = 11;
            else if(r == '<')State = 12;
            else if(r == ':')State = 13;
            else if(r == '^')State = 14;
            else if(r == '"')State = 15;
            else if(r == '\'')State = 16;
            else if(r == '}')State = 17;
            else if(r == '{')State = 18;
            else if(r == '(')State = 19;
            else if(r == ')')State = 20;
            else if(r == '[')State = 21;
            else if(r == ']')State = 22;
            else if(r == ',')State = 23;
            else if(r == ';')State = 24;
            else if(r == '?')State = 25;
            else if(r == '~')State = 26;
            else if(r == '.')State = 36;
            else if(r == '#')State = 37;
            else if(r == '=')State = 38;
            break;
        case -1:
            if(r == '\n' || r == '\t' || r == ' '){
                if(r == '\n')lex.pop_back();
                cout << "\nLexical Error: Line " << row << ", Column " << col << ". EXP = " << lex;
                lex = '\0';
                State = 0;
                i--;
            }
            err = true;
            break;
        case 1:
            if(Letter(r)||Digit(r)||r == '_');
            else {
                State = -3;
                type = TokenType::ID;
                if(Keyword(lex))type = TokenType::KEYWORD;
            }
            break;
        case 2:
            if(Digit(r));
            else if(r == '.')State = 27;
            else if (Letter(r) || r == '_')State = -1;
            else type = TokenType::NO, State = -3;
            break;
        case 3:
            if(r == '*')State = 29;
            else if(r == '/')State = 31;
            else if(r == '=')type = TokenType::E_DIV, State = -2;
            else type = TokenType::DIV, State = -3;
            break;
        case 4:
            if(r == '=')type = TokenType::E_MUL, State = -2;
            else type = TokenType::MUL, State = -3;
            break;
        case 5:
            if(r == '=')type = TokenType::E_REM, State = -2;
            else type = TokenType::REM, State = -3;
            break;
        case 6:
            if(r == '+')type = TokenType::INC, State = -2;
            else if(r == '=')type = TokenType::E_PLS, State = -2;
            else type = TokenType::PLS, State = -3;
            break;
        case 7:
            if(r == '-')type = TokenType::DEC, State = -2;
            else if(r == '=')type = TokenType::E_DEC, State = -2;
            else if(r == '>')type = TokenType::PTR, State = -2;
            else type = TokenType::MNS, State = -3;
            break;
        case 8:
            if(r == '=')type = TokenType::NE, State = -2;
            else type = TokenType::N, State = -3;
            break;
        case 9:
            if(r == '&')type = TokenType::AND, State = -2;
            else if(r == '=')type = TokenType::B_AND_ASG, State = -2;
            else type = TokenType::B_AND, State = -3;
            break;
        case 10:
            if(r == '|')type = TokenType::OR, State = -2;
            else if(r == '=')type = TokenType::B_OR_ASG, State = -2;
            else type = TokenType::B_OR, State = i-3;
            break;
        case 11:
            if(r == '>')State = 32;
            else if(r == '=')type = TokenType::E_GT, State = -2;
            else type = TokenType::GT, State = -3;
            break;
        case 12:
            if(r == '<')State = 33;
            else if(r == '=')type = TokenType::E_LT, State = -2;
            else type = TokenType::LT, State = -3;
            break;
        case 13:
            if(r == ':')type = TokenType::SCP_RES, State = -2;
            else type = TokenType::CLN, State = -3;
            break;
        case 14:
            if(r == '=')type = TokenType::B_XOR_ASG, State = -2;
            else type = TokenType::B_XOR, State = -3;
            break;
        case 15:
            if(r == '\n'){lex.pop_back() , lex += "\\n";}
            if(r == '"')type = TokenType::STR, State = -2;
            else if(r == '\\')State = 34;
            break;
        case 16:
            State = 35;
            if(r == '\\') State = 39;
            break;
        case 17:
            type = TokenType::R_BRC, State = -3;
            break;
        case 18:
            type = TokenType::L_BRC, State = -3;
            break;
        case 19:
            type = TokenType::L_FN, State = -3;
            break;
        case 20:
            type = TokenType::R_FN, State = -3;
            break;
        case 21:
            type = TokenType::L_BRK, State = -3;
            break;
        case 22:
            type = TokenType::R_BRK, State = -3;
            break;
        case 23:
            type = TokenType::COMMA, State = -3;
            break;
        case 24:
            type = TokenType::SEMI, State = -3;
            break;
        case 25:
            type = TokenType::Q, State = -3;
            break;
        case 26:
            type = TokenType::B_NOT, State = -3;
            break;
        case 27:
            State = -1;
            if(Digit(r))State = 28;
            break;
        case 28:
            if(Digit(r));
            else if (Letter(r) || r == '_')State = -1;
            else type = TokenType::NO, State = -3;
            break;
        case 29:
            if(r == '\n'){lex.pop_back() , lex += "\\n";}
            if(r == '*')State = 30;
            break;
        case 30:
            if(r == '\n'){lex.pop_back() , lex += "\\n";}
            if(r == '*');
            else if(r == '/'){type = TokenType::COMMENT, State = -2;}
            else State = 29;
            break;
        case 31:
            if(r == '\n'){type = TokenType::COMMENT, State = -2;}
            break;
        case 32:
            if(r == '=')type = TokenType::R_ASG, State = -2;
            else type = TokenType::RST, State = -3;
            break;
        case 33:
            if(r == '=')type = TokenType::L_ASG, State = -2;
            else type = TokenType::LST, State = -3;
            break;
        case 34:
            State = 15;
            break;
        case 35:
            State = -1;
            if(r == '\'')type = TokenType::CHR, State = -2;
            break;
        case 36:
            type = TokenType::DOT, State = -3;
            break;
        case 37:
            type = TokenType::HASH, State = -3;
            break;
        case 38:
            if(r == '=')type = TokenType::EQ, State = -2;
            else type = TokenType::ASG, State = -3;
            break;
        case 39:
            State = 35;
            break;
        }
        if(State == -2 || State == -3) {
            if(State == -3) {--i, lex.pop_back();}
            tokens.push_back(createToken(++no,row,++col,type,lex));
            lex = "";
            State = 0;
            }
    }
    if(State != 0){err = true , cout << "\nErr: Line " << row << ", Column " << col << ". EXP = " << lex;}
    if(err)
        exit(0);
    return tokens;
}
//-----------------------------------------------------Syntax Analyzer---------------------------------------------------------------
    void match(string expectedToken) {
        //if(!otp)cout << tokens[currentTokenIndex] + " ";
        if (currentTokenIndex < tokens.size() && tokens[currentTokenIndex] != expectedToken) {
                errors.push_back("Line" + to_string(rows[currentTokenIndex]) + ": Syntax Error: Expected '" + expectedToken + "', but found '" + tokens[currentTokenIndex] + "'");
            while(currentTokenIndex < tokens.size() && expectedToken != tokens[currentTokenIndex])
                currentTokenIndex++;
        }
       // if(currentTokenIndex >= tokens.size())
            //prout();
        currentTokenIndex++;
    }

    void Program() {
        if(otp)cout << "Program ";
        if(tokens[currentTokenIndex] == "HASH")
            Headers();
        else
            FunctionList();
        while(currentTokenIndex < tokens.size())
            Program();
    }

    void Headers() {
        if(otp)cout << "Headers ";
        match("HASH");
        Keyword1();
        match("LT");
        HeadNames();
        match("GT");
    }
    void Keyword1() {
        if(otp)cout << "Keyword1 ";
        if (tokens[currentTokenIndex] == "include" || tokens[currentTokenIndex] == "define")
            match(tokens[currentTokenIndex]);
        else {
            errors.push_back("Line" + to_string(rows[currentTokenIndex]) + ": Syntax Error: Expected 'include' or 'define', but found '" + tokens[currentTokenIndex] + "'");
            currentTokenIndex++;
        }
    }
    void HeadNames() {
        if(otp)cout << "HeadNames ";
        if (tokens[currentTokenIndex] == "ID" || tokens[currentTokenIndex] == "STR")
            match(tokens[currentTokenIndex]);
        else {
            errors.push_back("Line" + to_string(rows[currentTokenIndex]) + ": Syntax Error: Expected 'Head Name', but found '" + tokens[currentTokenIndex] + "'");
            currentTokenIndex++;
        }
    }

    void FunctionList() {
        if(otp)cout << "FunctionList ";
        Function();
        while(currentTokenIndex < tokens.size())
            FunctionList();
    }
    void Function() {
        if(otp)cout << "Function ";
        match(TYPEst());
        match("ID");
        match("L_FN");
        Params();
        match("R_FN");
        FBody();
    }

    void Params() {
        if(otp)cout << "Params ";
        if(tokens[currentTokenIndex] != "R_FN")
            HVParams();
    }
    void HVParams() {
        if(otp)cout << "HVParams ";
        Exp();
        if(tokens[currentTokenIndex] == "COMMA") {
            match("COMMA");
            HVParams();
        }
    }
    void FBody() {
        if(otp)cout << "FBody ";
        match("L_BRC");
        StatementList();
        if(tokens[currentTokenIndex] == "return") {
            match("return");
            match(Val());
            match("SEMI");
        }
        match("R_BRC");
    }

    void StatementList() {
        if(otp)cout << "StatementList ";
        Statement();
        if(tokens[currentTokenIndex] != "R_BRC" && tokens[currentTokenIndex] != "break" && currentTokenIndex < tokens.size())
            StatementList();
    }
    void Statement() {
        if(otp)cout << "Statement ";
        if(tokens[currentTokenIndex] == "if")IFst();
        else if(tokens[currentTokenIndex] == "for")FORst();
        else if(tokens[currentTokenIndex] == "while")WHILEst();
        else if(tokens[currentTokenIndex] == "do")DOWHILEst();
        else if(tokens[currentTokenIndex] == "switch")SWITCHst();
        else if(tokens[currentTokenIndex] == "ID") {
            if(tokens[currentTokenIndex+1] == "L_FN")
                CALLst();
            else
                ASSIGNst();
        }
        else if(tokens[currentTokenIndex] == "cin")IN();
        else if(tokens[currentTokenIndex] == "cout")OUT();
        else if(tokens[currentTokenIndex] == TYPEst())Var();
        else {
            errors.push_back("Line" + to_string(rows[currentTokenIndex]) + ": Syntax Error: Expected valid 'STATEMENT', but found '" + tokens[currentTokenIndex] + "'");
            currentTokenIndex++;
        }
    }

    void IFst() {
        if(otp)cout << "IFst ";
        match("if");
        match("L_FN");
        Condition();
        match("R_FN");
        Body();
        EPart();
    }
    void EPart() {
        if(otp)cout << "EPart ";
        ElseIfPart();
        ElsePart();
    }
    void ElseIfPart() {
        if(otp)cout << "ElseIfPart ";
        if(tokens[currentTokenIndex] == "else" && tokens[currentTokenIndex+1] == "if") {
            match("else");
            match("if");
            match("L_FN");
            Condition();
            match("R_FN");
            Body();
            ElseIfPart();
        }
    }
    void ElsePart() {
        if(otp)cout << "ElsePart ";
        if(tokens[currentTokenIndex] == "else") {
            match("else");
            Body();
        }
    }

    void FORst() {
        if(otp)cout << "FORst ";
        match("for");
        match("L_FN");
        SCN();
        match("SEMI");
        Condition();
        match("SEMI");
        Exp();
        match("R_FN");
        Body();
    }
    void SCN() {
        if(otp)cout << "SCN ";
        if(tokens[currentTokenIndex] != "ID")
            match(TYPEst());
        match("ID");
        match("ASG");
        match(Val());
    }

    void WHILEst() {
        if(otp)cout << "WHILEst ";
        match("while");
        match("L_FN");
        Condition();
        match("R_FN");
        Body();
    }
    void DOWHILEst() {
        if(otp)cout << "DOWHILEst ";
        match("do");
        match("L_BRC");
        StatementList();
        match("R_BRC");
        match("while");
        match("L_FN");
        Condition();
        match("R_FN");
        match("SEMI");
    }

    void SWITCHst() {
        if(otp)cout << "SWITCHst ";
        match("switch");
        match("L_FN");
        Exp();
        match("R_FN");
        match("L_BRC");
        CaseList();
        DefCase();
        match("R_BRC");
    }
    void CaseList() {
        if(otp)cout << "CaseList ";
        CASEst();
        if(tokens[currentTokenIndex] == "case")
            CaseList();
    }
    void CASEst() {
        if(otp)cout << "CASEst ";
        match("case");
        match(Val());
        match("CLN");
        StatementList();
        match("break");
        match("SEMI");
    }
    void DefCase() {
        if(otp)cout << "DefCase ";
        if(tokens[currentTokenIndex] == "default")
            match("default");
            match("CLN");
            StatementList();
    }

    void ASSIGNst() {
        if(otp)cout << "ASSIGNst ";
        match("ID");
        match(SOP());
        Exp();
        match("SEMI");
    }
    void CALLst() {
        if(otp)cout << "CALLst ";
        match("ID");
        match("L_FN");
        Params();
        match("R_FN");
        match("SEMI");
    }

    void Var() {
        if(otp)cout << "Var ";
        match(TYPEst());
        VDef1();
        match("SEMI");
    }
    void VDef1() {
        if(otp)cout << "VDef1 ";
        VDef2();
        if(tokens[currentTokenIndex] == "COMMA" || tokens[currentTokenIndex] == "ASG") {
            match(tokens[currentTokenIndex]);
            VDef1();
        }
    }
    void VDef2() {
        if(otp)cout << "VDef2 ";
        match("ID");
        if(tokens[currentTokenIndex] == "ASG") {
            match("ASG");
            match(Val());
        }
    }

    void IN() {
        if(otp)cout << "IN ";
        match("cin");
        match("RST");
        CL();
        match("SEMI");
    }
    void CL() {
        if(otp)cout << "CL ";
        match("ID");
        if(tokens[currentTokenIndex] == "RST") {
            match("RST");
            CL();
        }
    }

    void OUT() {
        if(otp)cout << "OUT ";
        match("cout");
        match("LST");
        CO();
        match("SEMI");
    }
    void CO() {
        if(otp)cout << "CO ";
        PR();
        if(tokens[currentTokenIndex] == "LST") {
           match("LST");
           CO();
        }
    }
    void PR() {
        if(otp)cout << "PR ";
        if(tokens[currentTokenIndex] == Val() && (tokens[currentTokenIndex+1] == "LST" || tokens[currentTokenIndex+1] == "SEMI"))
            match(tokens[currentTokenIndex]);
        else
            Exp();
    }

    void Condition() {
        if(otp)cout << "Condition ";
        Exp();
        match(COP());
        Exp();
        if(tokens[currentTokenIndex] == "AND" || tokens[currentTokenIndex] == "OR") {
            match(tokens[currentTokenIndex]);
            Condition();
        }
    }
    void Body() {
        if(otp)cout << "Body ";
        if(tokens[currentTokenIndex] == "L_BRC") {
            match("L_BRC");
            StatementList();
            match("R_BRC");
        }
        else
            Statement();
    }
    void Exp() {
        if(otp)cout << "Exp ";
        Term();
        if(tokens[currentTokenIndex] == "MNS" || tokens[currentTokenIndex] == "PLS") {
            match(tokens[currentTokenIndex]);
            Exp();
        }
    }
    void Term() {
        if(otp)cout << "Term ";
        Factor();
        if(tokens[currentTokenIndex] == "REM" || tokens[currentTokenIndex] == "MUL" || tokens[currentTokenIndex] == "DIV") {
            match(tokens[currentTokenIndex]);
            Factor();
        }
    }
    void Factor() {
        if(otp)cout << "Factor ";
        if(tokens[currentTokenIndex] == "ID") {
            match("ID");
            if(tokens[currentTokenIndex] == OOP())
                match(OOP());
        }
        else if(tokens[currentTokenIndex] == OOP()) {
            match(OOP());
            match("ID");
        }
        else if(tokens[currentTokenIndex] == "NO")
            match("NO");
        else if(tokens[currentTokenIndex] == "N") {
            match("N");
            Factor();
        }
        else if(tokens[currentTokenIndex] == "L_FN"){
            match("L_FN");
            Exp();
            match("R_FN");
        }
         else {
            errors.push_back("Line" + to_string(rows[currentTokenIndex]) + ": Syntax Error: Expected valid 'EXPRESSION', but found '" + tokens[currentTokenIndex] + "'");
            currentTokenIndex++;
        }
    }

    string OOP() {
        if(otp)cout << "OOP ";
        if(tokens[currentTokenIndex] == "INC")
            return "INC";
        else if (tokens[currentTokenIndex] == "DEC")
            return "DEC";
        else return "OOP";
    }
    string COP() {
        if(otp)cout << "COP ";
        if (tokens[currentTokenIndex] == "GT")return "GT";
        else if (tokens[currentTokenIndex] == "LT")return "LT";
        else if (tokens[currentTokenIndex] == "E_GT")return "E_GT";
        else if (tokens[currentTokenIndex] == "E_LT")return "E_LT";
        else if (tokens[currentTokenIndex] == "NE")return "NE";
        else if (tokens[currentTokenIndex] == "EQ")return "EQ";
        else return "COP";
    }
    string SOP() {
        if(otp)cout << "SOP ";
        if (tokens[currentTokenIndex] == "ASG") return "ASG";
        else if (tokens[currentTokenIndex] == "E_PLS") return "E_PLS";
        else if (tokens[currentTokenIndex] == "E_DEC") return "E_DEC";
        else if (tokens[currentTokenIndex] == "E_DIV") return "E_DIV";
        else if (tokens[currentTokenIndex] == "E_MUL") return "E_MUL";
        else if (tokens[currentTokenIndex] == "E_REM") return "E_REM";
        else return "SOP";
    }
    string Val() {
        if(otp)cout << "Val ";
        if (tokens[currentTokenIndex] == "NO") return "NO";
        else if (tokens[currentTokenIndex] == "CHR") return "CHR";
        else if (tokens[currentTokenIndex] == "STR") return "STR";
        else if (tokens[currentTokenIndex] == "true") return "true";
        else if (tokens[currentTokenIndex] == "false") return "false";
        else return "Val";
    }
    string TYPEst() {
        if(otp)cout << "TYPEst ";
        if (tokens[currentTokenIndex] == "bool") return "bool";
        else if (tokens[currentTokenIndex] == "int") return "int";
        else if (tokens[currentTokenIndex] == "char") return "char";
        else if (tokens[currentTokenIndex] == "double") return "double";
        else if (tokens[currentTokenIndex] == "float") return "float";
        else if (tokens[currentTokenIndex] == "void") return "void";
        else return "TYPEst";
    }
    //------------------------------------------------------output-------------------------------------------------------------------
    void prout() {
        cout << "\nImported TOKENS from lexer (There is no lexical error):\n\n";
        for (string wrd : ::tokens)
            cout << wrd << " ";
        cout << endl << endl;

        if(errors.size() == 0)
            cout << ">> This Program is Acceptable (There is no Syntax error)\n\n";
        else {
            cout << ">> This Program is not Accepable.\n-----------------------------------------------------\n";
            for (string wrd : ::errors)
                cout << wrd << endl;
        }
        exit(0);
    }
