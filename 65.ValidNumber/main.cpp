#include <iostream>
#include <string>
#include <cctype>
#include <array>
#include <ranges>

class Utils
{
    static constexpr std::array<char, 2> exp_symbols = {{'e', 'E'}};
    static constexpr std::array<char, 2> signs = {{'-', '+'}};

public:
    static bool is_digit(char ch) { return std::isdigit(static_cast<unsigned char>(ch)); }
    static bool is_dot_symbol(char ch) { return '.' == ch; }

    static bool is_exp_symbol(char ch) 
    {
#if __cpp_lib_ranges_contains >= 202207L
        return std::ranges::contains(exp_symbols, ch);
#else
        return std::find(exp_symbols.begin(), exp_symbols.end(), ch) != exp_symbols.end();
#endif
    }

    static bool is_sign_symbol(char ch) 
    {
#if __cpp_lib_ranges_contains >= 202207L
        return std::ranges::contains(signs, ch);
#else
        return std::find(signs.begin(), signs.end(), ch) != signs.end();
#endif
    }
};

class NumberParser
{
    enum class SymbolType
    {
        NONE,
        SIGN,
        DOT,
        DIGIT,
        EXPONENT,
    };
    enum class NumberState
    {
        NONE,
        SIGNED,
        INTEGER,
        ZERO_FLOAT,
        FLOAT,
        ZERO_EXPONENT,
        SIGNED_ZERO_EXPONENT,
        EXPONENT,
        FAILED,
    };

public:
    NumberParser()
        : _number_state()
    {}

    bool is_number_valid() const
    {
        switch (_number_state)
        {
            case NumberState::INTEGER:
            case NumberState::FLOAT:
            case NumberState::EXPONENT:
                return true;

            case NumberState::NONE:
            case NumberState::SIGNED:
            case NumberState::ZERO_FLOAT:
            case NumberState::ZERO_EXPONENT:
            case NumberState::SIGNED_ZERO_EXPONENT:
            case NumberState::FAILED:
                break;
        }

        return false;
    }

    bool add(char ch)
    {
        const SymbolType sym_type = symbol_type(ch);            
        switch (_number_state)
        {
            case NumberState::FAILED:
                break;

            case NumberState::NONE: 
                _number_state = update_none_state(sym_type);
                break;

            case NumberState::SIGNED:
                _number_state = update_sign_state(sym_type);
                break;
            
            case NumberState::INTEGER:
                _number_state = update_integer_state(sym_type);
                break;

            case NumberState::ZERO_FLOAT:
                _number_state = update_zero_float_state(sym_type);
                break;
                     
            case NumberState::FLOAT:
                _number_state = update_float_state(sym_type);
                break;

            case NumberState::ZERO_EXPONENT:
                _number_state = update_zero_exponent_state(sym_type);
                break;

            case NumberState::SIGNED_ZERO_EXPONENT:
                _number_state = update_signed_zero_exponent_state(sym_type);
                break;

            case NumberState::EXPONENT:
                _number_state = update_exponent_state(sym_type);
                break;
        }

        return NumberState::FAILED == _number_state;
    }

private:
    static NumberState update_none_state(SymbolType symbol)
    {
        switch (symbol)
        {
            case SymbolType::NONE:
            case SymbolType::EXPONENT:
                break;

            case SymbolType::SIGN:  return NumberState::SIGNED;
            case SymbolType::DOT:   return NumberState::ZERO_FLOAT;
            case SymbolType::DIGIT: return NumberState::INTEGER;
        }

        return NumberState::FAILED;
    }

    static NumberState update_sign_state(SymbolType symbol)
    {
        switch (symbol)
        {
            case SymbolType::NONE:
            case SymbolType::SIGN:
            case SymbolType::EXPONENT:
                break;

            case SymbolType::DOT:       return NumberState::ZERO_FLOAT;
            case SymbolType::DIGIT:     return NumberState::INTEGER;
        }

        return NumberState::FAILED;
    }

    static NumberState update_integer_state(SymbolType symbol)
    {
        switch (symbol)
        {
            case SymbolType::NONE:
            case SymbolType::SIGN:
                break;

            case SymbolType::DOT:       return NumberState::FLOAT;
            case SymbolType::DIGIT:     return NumberState::INTEGER;
            case SymbolType::EXPONENT:  return NumberState::ZERO_EXPONENT;
        }

        return NumberState::FAILED;
    }

    static NumberState update_zero_float_state(SymbolType symbol)
    {
        switch (symbol)
        {
            case SymbolType::NONE:
            case SymbolType::SIGN:
            case SymbolType::DOT:
            case SymbolType::EXPONENT:
                break;

            case SymbolType::DIGIT:     return NumberState::FLOAT;
        }

        return NumberState::FAILED;
    }

    static NumberState update_float_state(SymbolType symbol)
    {
        switch (symbol)
        {
            case SymbolType::NONE:
            case SymbolType::SIGN:
            case SymbolType::DOT:
                break;

            case SymbolType::DIGIT:     return NumberState::FLOAT;
            case SymbolType::EXPONENT:  return NumberState::ZERO_EXPONENT;
        }

        return NumberState::FAILED;
    }

    static NumberState update_zero_exponent_state(SymbolType symbol)
    {
        switch (symbol)
        {
            case SymbolType::NONE:
            case SymbolType::DOT:
            case SymbolType::EXPONENT:
                break;

            case SymbolType::SIGN:      return NumberState::SIGNED_ZERO_EXPONENT;
            case SymbolType::DIGIT:     return NumberState::EXPONENT;
        }

        return NumberState::FAILED;
    }

    static NumberState update_signed_zero_exponent_state(SymbolType symbol)
    {
        switch (symbol)
        {
            case SymbolType::NONE:
            case SymbolType::SIGN:
            case SymbolType::DOT:
            case SymbolType::EXPONENT:
                break;

            case SymbolType::DIGIT:     return NumberState::EXPONENT;
        }

        return NumberState::FAILED;
    }

    static NumberState update_exponent_state(SymbolType symbol)
    {
        switch (symbol)
        {
            case SymbolType::NONE:
            case SymbolType::SIGN:
            case SymbolType::DOT:
            case SymbolType::EXPONENT:
                break;

            case SymbolType::DIGIT:     return NumberState::EXPONENT;
        }

        return NumberState::FAILED;
    }

    static SymbolType symbol_type(char ch)
    {
        if (Utils::is_sign_symbol(ch))      return SymbolType::SIGN;
        else if (Utils::is_dot_symbol(ch))  return SymbolType::DOT;
        else if (Utils::is_digit(ch))       return SymbolType::DIGIT;
        else if (Utils::is_exp_symbol(ch))  return SymbolType::EXPONENT;

        return SymbolType::NONE;
    }

    NumberState _number_state;
};

class Solution 
{
public:
    bool isNumber(std::string s) 
    {
        NumberParser parser;
        for(char& c : s) 
            parser.add(c);
        
        return parser.is_number_valid();
    }
};

int main()
{
    Solution solution;
    std::string valid[] = { "2", "0089", "-0.1", "+3.14", "4.", "-.9", "2e10", "-90E3", "3e+7", "+6e-1", "53.5e93", "-123.456e789" };
    std::string not_valid[] = { "abc", "1a", "1e", "e3", "99e2.5", "--6", "-+3", "95a54e53", "." };

    for(std::string& str : valid) 
        std::cout << solution.isNumber(str) << " ";

    std::cout << std::endl;

    for(std::string& str : not_valid) 
        std::cout << solution.isNumber(str) << " ";

    return 0;
}