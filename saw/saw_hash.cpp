#include <vector>
#include <iostream>
#include <ctime>
#include <unordered_set>
#include <boost/functional/hash.hpp>

// this file includes the excact counting method as in the other file, but using "std::unordered_set" and a hash function instead of "std::set".
//this uses boost for the hash function

// hash function for vectors
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

class SelfAvoidingWalk
{
    private:

        // return all possible new positions in the range of one step from your old position, that are not included in the history of your walk

        std::vector<std::vector<int> > make_all_steps(std::vector<int> &position, std::unordered_set<std::vector<int>, vectorhash > &history){
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

    public:

        // the coordinates reached at each end of a walk are stored here

        std::vector<std::vector<int> > result;

        // find all possible self avoiding walks of length N (only works for small N, maybe up to about 13)

        void walk_everywhere(int N, std::vector<int> position = {0, 0, 0}, std::unordered_set<std::vector<int>, vectorhash > history = std::unordered_set<std::vector<int>, vectorhash >(), int step = 1){
        if (step==N)
        {
            std::vector<std::vector<int> > next = make_all_steps(position, history);
            for (int i = 0; i < next.size(); i++)
            {
                result.push_back(next[i]);
            }
            
            return;
        }
        else
        {
            std::vector<std::vector<int> > next = make_all_steps(position, history);
            if (step==1)
            {
                history.insert(position);
            }
            for (int i = 0; i < next.size(); i++)
            {
                std::unordered_set<std::vector<int>, vectorhash > his = history;
                his.insert(next[i]);
                walk_everywhere(N, next[i], his, step+1);
            }
        }
        }

        // calculate the mean square displacement from result of either walk_sample or walk_everywhere 

        double square_disp(){
                double square_disp = 0;
                for (int i = 0; i < result.size(); i++)
                {
                    for (int j = 0; j < result[i].size(); j++)
                    {
                        square_disp = square_disp + (result[i][j]*result[i][j]);
                    }
                }
                square_disp = square_disp / result.size();
                return square_disp;
            }
};


// how to use: first argument is length of walk N

int main(int argc, char** argv){
    std::time_t stime = std::time(nullptr);
    int N = atoi(argv[1]);
    SelfAvoidingWalk SAW;
    SAW.walk_everywhere(N);
    std::cout << SAW.square_disp() << std::endl;
    return 1;
}