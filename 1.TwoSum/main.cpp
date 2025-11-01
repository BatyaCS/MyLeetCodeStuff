#include <iostream>
#include <unordered_map>

using namespace std;

class Solution 
{
public:
    vector<int> twoSum(vector<int>& nums, int target)
    {
        unordered_map<unsigned int, int> tmp;

        for (unsigned int i = 0; i < nums.size(); i++)
        {
            int to_find = target - nums[i];

            if (tmp.count(to_find))
                return { static_cast<int>(tmp[to_find]), static_cast<int>(i) };

            tmp[nums[i]] = static_cast<unsigned int>(i);
        }

        return {};
    }
};

int main()
{
    Solution solution;
    vector<int> nums = {3,2,4};
    vector<int> res = solution.twoSum(nums, 6);

    cout << res[0] << " " << res[1] << endl;
    return 0;
}