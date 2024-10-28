#include<vector>
#include<string>




extern void init_lexcial_analysis();
extern void init_syntax_analysis();
extern std::vector< std::pair<std::string, std::pair<std::string, std::string> > >lexcial_analysis(const char *input_path, const char *lexcial_output_path);
extern void syntax_analysis(std::vector< std::pair<std::string, std::pair<std::string, std::string> > > lexcial, const char *syntax_output_path);