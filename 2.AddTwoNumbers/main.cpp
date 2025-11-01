#include <iostream>

class Sum
{
public:
    static int sum(int n1, int n2, bool& is_rem_exist)
    {
        static constexpr int REM_CONST = 10;
        const int result = n1 + n2;

        is_rem_exist = result >= REM_CONST;
        return is_rem_exist ? result - REM_CONST : result;
    }
};

struct ListNode
{
    int val;
    ListNode * next;

    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution 
{
public:
    ListNode * addTwoNumbers(ListNode* l1, ListNode* l2) 
    {
        ListNode * res = new(std::nothrow) ListNode(l1->val);
        if (nullptr == res)
            return nullptr;
        
        ListNode * node_ptr = res;
        ListNode * l1_ptr = l1->next;
        while (nullptr != l1_ptr)
        {
            node_ptr->next = new(std::nothrow) ListNode(l1_ptr->val);
            if (nullptr == node_ptr->next)
                return nullptr;

            node_ptr = node_ptr->next;
            l1_ptr = l1_ptr->next;
        }
        
        ListNode * res_end = node_ptr;
        node_ptr = res;

        ListNode * l2_ptr = l2;
        bool is_rem_exist = false;

        while (nullptr != l2_ptr)
        {
            if (nullptr == node_ptr)
            {
                res_end->next = new(std::nothrow) ListNode(is_rem_exist ? 1 : 0);
                if (nullptr == res_end->next)
                    return nullptr;

                is_rem_exist = false;
                res_end = res_end->next;
                node_ptr = res_end;
            }

            node_ptr->val = Sum::sum(is_rem_exist ? 1 + node_ptr->val : node_ptr->val, l2_ptr->val, is_rem_exist);

            node_ptr = node_ptr->next;
            l2_ptr = l2_ptr->next;
        }

        while (is_rem_exist)
        {
            if (nullptr == node_ptr)
            {
                res_end->next = new(std::nothrow) ListNode(1);
                if (nullptr == res_end->next)
                    return nullptr;

                break;
            }

            node_ptr->val = Sum::sum(node_ptr->val, 1, is_rem_exist);
            node_ptr = node_ptr->next;
        }
        
        return res;
    }
};

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

int main()
{
    {
        const int l1_vals[] = {2, 4, 9};
        const int l2_vals[] = {5, 6, 4, 9};

        ListNode * l1 = nullptr;
        ListNode * l2 = nullptr;

        for (int i = ARRAY_SIZE(l1_vals) - 1; i >= 0; --i)
            l1 = new ListNode(l1_vals[i], l1);

        for (int i = ARRAY_SIZE(l2_vals) - 1; i >= 0; --i)
            l2 = new ListNode(l2_vals[i], l2);

        Solution solution;
        ListNode * res = solution.addTwoNumbers(l1, l2);

        std::cout << "Res:";
        while (nullptr != res)
        {
            std::cout << " " << res->val;
            res = res->next;
        }
    }

    std::cout << std::endl;

    {
        const int l1_vals[] = {9, 9, 9, 9, 9, 9, 9};
        const int l2_vals[] = {9, 9, 9, 9};

        ListNode * l1 = nullptr;
        ListNode * l2 = nullptr;

        for (int i = ARRAY_SIZE(l1_vals) - 1; i >= 0; --i)
            l1 = new ListNode(l1_vals[i], l1);

        for (int i = ARRAY_SIZE(l2_vals) - 1; i >= 0; --i)
            l2 = new ListNode(l2_vals[i], l2);

        Solution solution;
        ListNode * res = solution.addTwoNumbers(l1, l2);

        std::cout << "Res:";
        while (nullptr != res)
        {
            std::cout << " " << res->val;
            res = res->next;
        }
    }

    return 0;
}