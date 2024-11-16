#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include"Compilation_Principle.h"


std::map<std::string, std::string> kw_map, op_map, se_map;
//DFA
int next[12][9] = 
   {{0, 2, -1, 1, 4, 5, 6, 7, 12},
    {0, 1, 11, 1, 4, 5, -1, -1, -1},
    {0, 2, 3, -1, 4, 5, -1, -1, -1},
    {0, 3, -1, -1, 4, 5, -1, -1, -1},
    {0, 2, -1, 1, 4, 5, 6, 9, -1},
    {0, 2, 0, 1, 0, 5, 6, 9, 12},
    {-1, 7, -1, 7, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, 8, -1, -1},
    {0, -1, -1, -1, 4, 5, -1, -1, -1},
    {-1, 9, 9, 9, -1, -1, -1, 10, -1},
    {0, -1, -1, -1, 4, 5, -1, -1, -1},
    {-1, -1, -1, 1, -1, -1, -1, -1, -1}};
int if_identify[12][9] = 
   {{0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 1, 0, 1, 1, 0, 0, 0},
    {1, 0, 0, 0, 1, 1, 0, 0, 0},
    {1, 0, 0, 0, 1, 1, 0, 0, 0},
    {1, 1, 0, 1, 0, 1, 1, 1, 0},
    {1, 1, 0, 1, 0, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 0}};


std::pair<std::string, std::string> identify_kw_and_idn(std::string s)
{
    if (!kw_map.count(s))
    {
        return {"Ident", s};
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

std::pair<std::string, std::string> identify_se(std::string s)
{
    if (!se_map.count(s))
    {
        std::cerr << "SE $" << s << "$ not found !" << std::endl;
        exit(EXIT_FAILURE);
    }
    return {"SE", se_map[s]};
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
    //need to add union !!!!!
    //kw
    kw_map["int"] = "1";
    kw_map["void"] = "2";
    kw_map["return"] = "3";
    kw_map["const"] = "4";
    kw_map["main"] = "5";
    kw_map["struct"] = "6";
    // kw_map["float"] = "0";
    // kw_map["char"] = "0";
    
    //op
    op_map["+"] = "7";
    op_map["-"] = "8";
    op_map["*"] = "9";
    op_map["/"] = "10";
    op_map["%"] = "11";
    op_map["="] = "12";
    op_map[">"] = "13";
    op_map["<"] = "14";
    op_map["=="] = "15";
    op_map["<="] = "16";
    op_map[">="] = "17";
    op_map["!="] = "18";
    op_map["&&"] = "19";
    op_map["||"] = "20";


    //se
    se_map["("] = "21";
    se_map[")"] = "22";
    se_map["{"] = "23";
    se_map["}"] = "24";
    se_map[";"] = "25";
    se_map[","] = "26";


    return;
}


int get_char_id(char c)
{
    if (c == ' ' || c == '\n' || c == '\t') return 0;
    if ('0' <= c && c <= '9') return 1;
    if (c == '.') return 2;
    if (c == '_' || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')) return 3;
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '!' || c == '=' || c == '<' || c == '>' || c == '&' || c == '|') return 4;
    if (c == '{' || c == '}' || c == '(' || c == ')' || c == ';' || c == ',') return 5;
    if (c == '\'') return 6;
    if (c == '\"') return 7;
    if (c == '$') return 8;
    exit(EXIT_FAILURE);
}


std::vector< std::pair<std::string, std::pair<std::string, std::string> > > lexcial_analysis(const char *input_path, const char *lexcial_output_path)
{
    std::vector< std::pair<std::string, std::pair<std::string, std::string> > > lexcial;

    // code
    init_lexcial_analysis();

    int now_state = 0;
    std::ifstream input_file(input_path);
    if (!input_file)
    {
        std::cerr << "Error opening file: " << input_path << std::endl;
        exit(EXIT_FAILURE);
    }

    char c;
    std::string buffer = "";
    std::cout << "Read program below: " << std::endl;
    while (1)
    {
        if (!input_file.get(c))
        {
            c = '$';
        }
        // else
        // {
        //     std::cout<<c<<std::endl;
        // }
        // --------------------------- DFA ---------------------------------
        int char_id = get_char_id(c);

        if (if_identify[now_state][char_id])
        {   
            if (now_state == 1)
            {
                lexcial.push_back({buffer, identify_kw_and_idn(buffer)});
                buffer = "";
            }
            else if (now_state == 2)
            {
                lexcial.push_back({buffer, identify_int(buffer)});
                buffer = "";
            }
            else if (now_state == 3)
            {
                lexcial.push_back({buffer, identify_float(buffer)});
                buffer = "";
            }
            else if (now_state == 4)
            {
                lexcial.push_back({buffer, identify_op(buffer)});
                buffer = "";
            }
            else if (now_state == 5)
            {
                lexcial.push_back({buffer, identify_se(buffer)});
                buffer = "";
            }
            else if (now_state == 8)
            {
                lexcial.push_back({buffer, identify_char(buffer)});
                buffer = "";
            }
            else if (now_state == 10)
            {
                lexcial.push_back({buffer, identify_string(buffer)});
                buffer = "";
            }
            else if (now_state == 11)
            {
                lexcial.push_back({".", {"DOT", "-"}});
                buffer = "";
            }
                
        }

        now_state = next[now_state][char_id];
        if (now_state == 12) break;
        if (c != '\'' && c != '\"' && c != ' ' && c != '\n' && c != '\t')
            buffer += c;
        std::cout<< "Debug : " << c << ' ' << buffer << std::endl;
    }
    std::cout<< std::endl;
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





















