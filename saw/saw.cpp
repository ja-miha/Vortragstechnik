#include <vector>
#include <iostream>
#include <ctime>
#include <unordered_set>
#include <boost/functional/hash.hpp>

struct vectorhash
{
    std::size_t operator()(std::vector<int> vec) const noexcept
    {
        std::size_t h1 = 3;
        for (int i = 0; i < vec.size(); i++)
        {
            boost::hash_combine(h1, vec[i]);
        }
    return h1;
    }
};

class saw
{
public:
    std::vector<std::vector<int> > result;

    std::vector<std::vector<int> > make_step(std::vector<int> &position, std::unordered_set<std::vector<int>, vectorhash > &history){
        std::vector<std::vector<int> > next_positions;
        std::vector<int> trial_position;
        for (int i = 0; i < 3; i++)
        {
            trial_position = position;
            trial_position[i] = trial_position[i]+1;
            if (history.count(trial_position)==0)
            {
                next_positions.push_back(trial_position);
            }
            trial_position = position;
            trial_position[i] = trial_position[i]-1;
            if (history.count(trial_position)==0)
            {
                next_positions.push_back(trial_position);
            }
        }
        return next_positions;
    }


    void walk(int N, std::vector<int> position = {0, 0, 0}, std::unordered_set<std::vector<int>, vectorhash > history = std::unordered_set<std::vector<int>, vectorhash >(), int step = 1){
    if (step==N)
    {
        std::vector<std::vector<int> > next = make_step(position, history);
        for (int i = 0; i < next.size(); i++)
        {
            result.push_back(next[i]);
        }
        
        return;
    }
    else
    {
        std::vector<std::vector<int> > next = make_step(position, history);
        if (step==1)
        {
            history.insert(position);
        }
        for (int i = 0; i < next.size(); i++)
        {
            std::unordered_set<std::vector<int>, vectorhash > his = history;
            his.insert(next[i]);
            walk(N, next[i], his, step+1);
        }
        
    }
    }

};



int main(int argc, char** argv){
    std::time_t stime = std::time(nullptr);
    int N = atoi(argv[1]);
    while (std::time(nullptr)-stime<3600)
    {
    saw w;
    w.walk(N);
    std::vector<std::vector<int> > res = w.result;
    long int square_disp = 0;
    for (int i = 0; i < res.size(); i++)
    {
        for (int j = 0; j < res[i].size(); j++)
        {
            square_disp = square_disp + (res[i][j]*res[i][j]);
        }
        
    }
    std::cout << res.size() << " , " << square_disp << std::endl;
    N = N+1;
    }
    return 1;
}