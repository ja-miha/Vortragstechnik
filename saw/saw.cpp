#include <vector>
#include <iostream>
#include <ctime>
#include <set>
#include <random>

class SelfAvoidingWalk
{

    private:
        std::mt19937 gen;
        std::random_device seed;
        std::uniform_int_distribution<> dis;

        // return all possible new positions in the range of one step from your old position, that are not included in the history of your walk

        std::vector<std::vector<int> > make_all_steps(std::vector<int> &position, std::set<std::vector<int> > &history){
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

        // make a random step from your old position and return the new position

        std::vector<int> random_step(std::vector<int> position){
            int randint = dis(gen);
            position[(randint-1)/2] = position[(randint-1)/2] + 2*((randint)%2)-1;
            return position;
        }

    public:

        // the coordinates reached at each end of a walk are stored here

        std::vector<std::vector<int> > result;

        // initialize the object

        SelfAvoidingWalk(){
            std::random_device seed;
            gen = std::mt19937(seed());
            dis = std::uniform_int_distribution<>(1, 6);
            std::vector<std::vector<int> > result = std::vector<std::vector<int> >();
        }

        // find all possible self avoiding walks of length N (only works for small N, maybe up to about 13)

        void walk_everywhere(int N, std::vector<int> position = {0, 0, 0}, std::set<std::vector<int> > history = std::set<std::vector<int> >(), int step = 1){
        if (step==N)
        {
            std::vector<std::vector<int> > next = make_all_steps(position, history);
            for (int i = 0; i < next.size(); i++)
            {
                result.push_back(next[i]);
            }
            
            return;
        }
        else if (step==1)
        {
            result = std::vector<std::vector<int> >();
            std::vector<std::vector<int> > next = make_all_steps(position, history);
            if (step==1)
            {
                history.insert(position);
            }
            for (int i = 0; i < next.size(); i++)
            {
                std::set<std::vector<int> > his = history;
                his.insert(next[i]);
                walk_everywhere(N, next[i], his, step+1);
            }
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
                std::set<std::vector<int> > his = history;
                his.insert(next[i]);
                walk_everywhere(N, next[i], his, step+1);
            }
            
        }
        }

        // make a sample of standard random walks until you have encountered sample_size self avoiding random walks of length N 

        void walk_sample(int N, int sample_size){
            result = std::vector<std::vector<int> >();
            while (result.size()<sample_size)
            {
                std::vector<int> position{0, 0, 0};
                std::set<std::vector<int> > history{position};
                //history.insert(position);
                bool avoidance_flag = false;
                for (int i = 0; i < N; i++)
                {
                    position = random_step(position);
                    if (history.count(position)==1)
                    {
                        avoidance_flag = true;
                        break;
                    }
                    history.insert(position);
                }
                if (avoidance_flag==false)
                {
                    result.push_back(position);
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

// how to use: first argument is length of walk N, second is size of your sample.
//If N is sufficiently small, the excact count is also returned after the sampling result.

int main(int argc, char** argv){
    SelfAvoidingWalk SAW;
    int N = atoi(argv[1]);
    int sample = atoi(argv[2]);
    SAW.walk_sample(N, sample);
    std::cout << SAW.square_disp() << std::endl;
    if (N<10)
    {
        SAW.walk_everywhere(N);
        std::cout << SAW.square_disp() << std::endl;
    }

    return 1;
}