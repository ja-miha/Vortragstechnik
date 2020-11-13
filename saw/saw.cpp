#include <vector>
#include <iostream>
#include <ctime>
#include <unordered_set>
#include <set>
#include <random>
#include <algorithm>
#include <boost/functional/hash.hpp>

struct vectorhash
{
    std::size_t operator()(const std::vector<int> &vec) const noexcept
    {
        return boost::hash_range(vec.begin(), vec.end());
    }
};

class SelfAvoidingWalk
{

    private:
        std::mt19937 gen;
        std::random_device seed;
        std::uniform_int_distribution<> dis;

        std::vector<std::vector<int> > first_walk(int N){
            std::vector<std::vector<int> > first;
            while (first.size()<N+1)
            {
                first.clear();
                std::vector<int> position(dimension, 0);
                first.push_back(position);
                std::unordered_set<std::vector<int>, vectorhash> history{position};
                //history.insert(position);
                for (int i = 0; i < N; i++)
                {
                    int randint = dis(gen);
                    position[(randint)/2] = position[(randint)/2] + 2*((randint)%2)-1;
                    if (history.count(position)==1) break;
                    history.insert(position);
                    first.push_back(position);
                }
            }
            std::cout << "found first walk" << std::endl;
            return first;
        }

        void permute(std::vector<int> &vec){
            std::mt19937 rn;
            std::uniform_int_distribution<> select(0, vec.size());
            for (int i = 0; i < 2*dimension; i++)
            {
                int pos = select(rn);
                int temp = vec[pos];
                vec[pos] = vec[0];
                vec[0] = temp;
            }
            
        }


    public:

        // the dimension

        int dimension;

        // the coordinates reached at each end of a walk are stored here

        std::vector<std::vector<int> > result;

        // initialize the object, set dimension

        SelfAvoidingWalk(int dim = 3){
            dimension = dim;
            std::random_device seed;
            gen = std::mt19937(seed());
            dis = std::uniform_int_distribution<>(0, 2*dim-1);
            result = std::vector<std::vector<int> >();
        }

        // find all possible self avoiding walks of length N (only works for small N, maybe up to about 13)

        void walk_everywhere(int N, std::vector<int> position = std::vector<int>(), const std::unordered_set<std::vector<int>, vectorhash> &history = std::unordered_set<std::vector<int>, vectorhash>(), int step = 0){
        if (step==N)
        {
            for (int i = 0; i < dimension*2; i++)
            {
                std::vector<int> next_position = position;
                next_position[i/2] = next_position[i/2] + 2*(i%2)-1;
                if (history.count(next_position)==0)
                {
                    result.push_back(next_position);
                }
            }
            
            return;
        }
        else if (step==0)
        {   
            result = std::vector<std::vector<int> >();
            std::vector<int> pos = std::vector<int>(dimension, 0);
            std::unordered_set<std::vector<int>, vectorhash> his = history;
            his.insert(pos);
            walk_everywhere(N, pos, his, step+1);
        }
        else
        {
            for (int i = 0; i < dimension*2; i++)
            {
                std::vector<int> next_position = position;
                next_position[i/2] = next_position[i/2] + 2*(i%2)-1;
                if (history.count(next_position)==0)
                {
                    std::unordered_set<std::vector<int>, vectorhash> his = history;
                    his.insert(next_position);
                    walk_everywhere(N, next_position, his, step+1);
                }
            }
        }
        }

        // make a sample of standard random walks until you have encountered sample_size self avoiding random walks of length N 

        void walk_sample(int N, int sample_size){
            result = std::vector<std::vector<int> >();
            while (result.size()<sample_size)
            {
                std::vector<int> position(dimension, 0);
                std::unordered_set<std::vector<int>, vectorhash> history{position};
                //history.insert(position);
                bool avoidance_flag = false;
                for (int i = 0; i < N; i++)
                {
                    int randint = dis(gen);
                    position[(randint)/2] = position[(randint)/2] + 2*((randint)%2)-1;
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

        void walk_metro(int N, int sample_size){
            std::random_device seed;
            std::mt19937 generator(seed());
            std::uniform_int_distribution<> piv(0, N-1);
            std::uniform_int_distribution<> sig(0, 1);
            std::vector<int> permutation;
            for (int i = 0; i < dimension; i++) permutation.push_back(i);
            std::vector<int> no_permutation = permutation;
            std::vector<int> ones(dimension, 1);
            result.clear();
            std::vector<std::vector<int> > walk(N+1, std::vector<int>(dimension, 0));
            for (int i = 0; i < N+1; i++) walk[i][0] = i;
            result.push_back(walk.back());
            std::unordered_set<std::vector<int>, vectorhash> avoidance_check;
            while (result.size()<sample_size+200000)
            {
                int avoidance_flag = 1;
                avoidance_check.clear();
                int pivot = piv(generator);
                avoidance_check.insert(walk[pivot]);
                std::shuffle(permutation.begin(), permutation.end(), generator);
                std::vector<std::vector<int> > new_walk = walk;
                std::vector<int> sigmas;
                for (int i = 0; i < dimension; i++) sigmas.push_back(2*sig(generator)-1);
                if ((permutation==no_permutation)&&(sigmas==ones)) continue;
                for (int i = pivot+1; i < walk.size(); i++)
                {
                    for (int j = 0; j < dimension; j++)
                    {
                        new_walk[i][permutation[j]] = walk[pivot][permutation[j]] + sigmas[j] * (walk[i][j]-walk[pivot][j]);
                    }
                    if (2*pivot-i>-1)
                    {
                        if (avoidance_check.count(new_walk[2*pivot-i]) == 1)
                        {
                            avoidance_flag = 0;
                            break;
                        }
                        avoidance_check.insert(new_walk[2*pivot-i]);
                    }
                    if (avoidance_check.count(new_walk[i]) == 1)
                        {
                            avoidance_flag = 0;
                            break;
                        }
                    avoidance_check.insert(new_walk[i]);
                }
                if (2*pivot-walk.size()>-1)
                {
                    for (int i = 2*pivot-walk.size(); i > -1; i--)
                    {
                        if (avoidance_check.count(new_walk[i]) == 1)
                        {
                            avoidance_flag = 0;
                            break;
                        }
                        avoidance_check.insert(new_walk[i]);
                    }
                }
                if (avoidance_flag==1)
                {
                    walk = new_walk;
                }
                result.push_back(walk.back());
            }
        }

        // calculate the mean square displacement from result of either walk_sample or walk_everywhere 

        double square_disp(int initialisation = 0){
            double square_disp = 0;
            for (int i = initialisation; i < result.size(); i++)
            {
                for (int j = 0; j < result[i].size(); j++)
                {
                    square_disp = square_disp + (result[i][j]*result[i][j]);
                }
            }
            square_disp = square_disp / (result.size()-initialisation);
            return square_disp;
        }
};

// how to use: first argument is mode(1=sample, else=count), second argument is length of walk N, third is size of your sample(only if mode==1),
// last (optional, default 3) is dimension

int main(int argc, char** argv){
    int mode = atoi(argv[1]);
    int N = atoi(argv[2]);
    int sample;
    if ((mode==1)||(mode==2))
    {
        sample = atoi(argv[3]);
    }
    int dim;
    if (((argc>4)&&(mode==2))||((argc>4)&&(mode==1))||((argc>3)&&(mode!=1)&&(mode!=2)))
    {
        dim = atoi(argv[argc-1]);
    }
    else
    {
        dim = 3;
    }
    SelfAvoidingWalk SAW(dim);
    if (mode==1)
    {
        SAW.walk_sample(N, sample);
        std::cout << SAW.result.size()<< " " << SAW.square_disp() << std::endl;
    }
    else if (mode==2)
    {
        SAW.walk_metro(N, sample);
        std::cout << SAW.result.size()-200000 << " " << SAW.square_disp(200000) << std::endl;
    }
    else
    {
        SAW.walk_everywhere(N);
        std::cout << SAW.result.size() << " " << SAW.square_disp() << std::endl;
    }

    return 1;
}