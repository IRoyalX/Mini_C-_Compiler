#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

enum class TokenType {
  UNKNOWN,KEYWORD,ID,E_DIV,DIV,E_MUL,MUL,E_REM,REM,INC,E_PLS,PLS,DEC,E_DEC,PTR,
  MNS,NE,N,AND,B_AND_ASG,B_AND,OR,B_OR_ASG,B_OR,E_GT,GT,E_LT,LT,
  SCP_RES,CLN,B_XOR_ASG,B_XOR,STR,R_BRC,L_BRC,L_FN,R_FN,L_BRK,
  R_BRK,COMMA,SEMI,Q,B_NOT,NO,R_ASG,RST,L_ASG,LST,CHR,DOT,HASH,ASG,EQ,COMMENT
};
struct Token {
    int no;
    int row;
    int col;
    int type;
    string value;
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

vector<Token> lexer(string);
void out(vector<Token>);

int main() {
    string filename , file = "" , line;
    vector<Token> output;
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
        infile.close();
        output = lexer(file);
    }
    out(output);
    return 0;
}


bool Digit(char ch){
    return (ch >= '0' && ch <= '9');
}
bool Letter(char ch){
    return ((ch >= 'a' && ch <= 'z')||(ch >= 'A' && ch <= 'Z'));
}
bool Keyword(string str){
    vector<string> wrds = {"asm","auto","bool","break","case","catch","char","class","const","constexpr","continue","decltype","default","delete","do","double",
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
                cout << "\nErr: Line " << row << ", Column " << col << ". EXP = " << lex;
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

void out(vector<Token> output){

    vector<string> TokenTypeNames = {
        "UNKNOWN", "KEYWORD", "ID", "E_DIV", "DIV", "E_MUL", "MUL", "E_REM", "REM", "INC",
        "E_PLS", "PLS", "DEC", "E_DEC", "PTR", "MNS", "NE", "N", "AND", "B_AND_ASG", "B_AND",
        "OR", "B_OR_ASG", "B_OR", "E_GT", "GT", "E_LT", "LT", "SCP_RES", "CLN", "B_XOR_ASG",
        "B_XOR", "STR", "R_BRC", "L_BRC", "L_FN", "R_FN", "L_BRK", "R_BRK", "COMMA", "SEMI",
        "Q", "B_NOT", "NO", "R_ASG", "RST", "L_ASG", "LST", "CHR", "DOT", "HASH", "ASG", "EQ" , "COMMENT"};

    cout << "\n\n" << setw(7) << "NO" << setw(5) << "|" << "\t" << "[  ROW   ,   COL  ]" << setw(5) << "|" << "  " << setw(9) << "TYPE" << "  " << setw(6) << "|" <<"\t" << "NAME" << endl;
    cout << "--------------------------------------------------------------------------" << endl;
    for(int i=0; i < output.size(); i++){
        Token token = output[i];
        cout << setw(7) << token.no << setw(5) << "|" << "\t" << "[ " << setw(3) << token.row << setw(5) << "," << setw(5) << token.col << setw(4) << " ]" << setw(5) << "|";
        cout << setw(3) << token.type << " _ " << TokenTypeNames[token.type] << "\t" << setw(3) << "|" << "\t" << token.value << endl;
    }
    cout << "--------------------------------------------------------------------------" << endl;
}
