#include <iostream>
#include <string>
#include <algorithm>

class Solution 
{
    static constexpr size_t SYMBOLS_COUNT = UINT8_MAX + 1;
    static constexpr int ZERO_SYMBOL_CODE = -1;

public:
    Solution() { std::fill(_symbol_pos, _symbol_pos + SYMBOLS_COUNT, ZERO_SYMBOL_CODE); }

    int lengthOfLongestSubstring(std::string s) 
    {
        int substr_start = 0;
        int substr_size = 0;

        for (int i = 0; i < static_cast<int>(s.size()); i++)
        {
            auto symbol = static_cast<unsigned char>(s[i]);

            if (_symbol_pos[symbol] >= substr_start)
                substr_start = _symbol_pos[symbol] + 1;

            _symbol_pos[symbol] = i;
            substr_size = std::max(substr_size, i - substr_start + 1);
        }

        return substr_size;
    }

private:
    int _symbol_pos[SYMBOLS_COUNT];
};

int main()
{
    Solution solution;
    std::string s = "pwwkew";

    std::cout << solution.lengthOfLongestSubstring(s);

    return 0;
}