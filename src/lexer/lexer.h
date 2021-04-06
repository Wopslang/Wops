#ifndef _WOPS_LEXER
#define <algorithm>

using Lexeme = vector<std::pair<int, std::string> >;

namespace wops {
	class Lexer {
		string codepath;
		vector<int>& tokens;
	public:
		Lexer(string);
	};
}

#endif // _WOPS_LEXER
