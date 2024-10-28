#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include"Compilation_Principle.h"


std::map<std::string, std::string> kw_map, op_map;
std::map<char, int> state_next[1001];


std::pair<std::string, std::string> identify_kw_and_idn(std::string s)
{
    if (!kw_map.count(s))
    {
        return {"IDN", s};
    }
    else
    {
        return {"KW", kw_map[s]};
    }
    
}

std::pair<std::string, std::string> identify_op(std::string s)
{
    if (!op_map.count(s))
    {
        std::cerr << "Operate " << s << "not found !" << std::endl;
        exit(EXIT_FAILURE);
    }
    return {"OP", op_map[s]};
}

std::pair<std::string, std::string> identify_int(std::string s)
{
    return {"INT", s};
}

std::pair<std::string, std::string> identify_float(std::string s)
{
    return {"FLOAT", s};
}

std::pair<std::string, std::string> identify_char(std::string s)
{
    return {"CHAR", s};
}

std::pair<std::string, std::string> identify_string(std::string s)
{
    return {"STR", s};
}




void init_lexcial_analysis()
{
    //unfinish !!!!!
    //kw
    kw_map["int"] = "0";
    kw_map["float"] = "0";
    kw_map["char"] = "0";
    kw_map["void"] = "0";
    kw_map["return"] = "0";
    kw_map["const"] = "0";
    kw_map["main"] = "0";

    //op
    op_map["!"] = "0";
    op_map["+"] = "0";
    op_map["-"] = "0";
    op_map["*"] = "0";
    op_map["/"] = "0";
    op_map["%"] = "0";
    op_map["="] = "0";
    op_map[">"] = "0";
    op_map["<"] = "0";
    op_map["=="] = "0";
    op_map["<="] = "0";
    op_map[">="] = "0";
    op_map["!="] = "0";
    op_map["&&"] = "0";
    op_map["||"] = "0";


    //state_next

    return;
}


std::vector< std::pair<std::string, std::pair<std::string, std::string> > > lexcial_analysis(const char *input_path, const char *lexcial_output_path)
{
    std::vector< std::pair<std::string, std::pair<std::string, std::string> > > lexcial;

    // code

    int now_state = 0;
    std::ifstream input_file(input_path);
    if (!input_file)
    {
        std::cerr << "Error opening file: " << input_path << std::endl;
        exit(EXIT_FAILURE);
    }
    char c;
    std::string buffer = "";
    while (input_file >> c)
    {
        buffer += c;
        now_state = state_next[now_state][c];
        // DFA

    }
    input_file.close();


    std::ofstream lexcial_output_file(lexcial_output_path);
    if (!lexcial_output_file)
    {
        std::cerr << "Error opening file: " << lexcial_output_path << std::endl;
        exit(EXIT_FAILURE);
    }

    for (auto item: lexcial)
    {
        lexcial_output_file << item.first << "\t<" << item.second.first << "," << item.second.second << ">" << std::endl;
    }

    lexcial_output_file.close();
    return lexcial;
}























