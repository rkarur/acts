#include <vector>
#include <iterator>
#include <numeric>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <string>
#include <Eigen/Dense>
#include <Eigen/StdVector>
#include <typeinfo>
// #include <boost/sort/spreadsort/spreadsort.hpp>
// #include <boost/tokenizer.hpp>
#include <chrono>
#include "annoy/src/annoylib.h"
#include "annoy/src/kissrandom.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TObjArray.h"


using namespace Eigen;
typedef float ANNOYTYPE;

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

// std::string met = "Euclidean";
// typedef AnnoyIndex<int, ANNOYTYPE, Euclidean, Kiss32Random,AnnoyIndexSingleThreadedBuildPolicy> MyAnnoyIndex;

std::string met = "Angular";
typedef AnnoyIndex<int, ANNOYTYPE, Angular, Kiss32Random,AnnoyIndexSingleThreadedBuildPolicy> MyAnnoyIndex;

// std::string met = "Manhattan";
// typedef AnnoyIndex<int, ANNOYTYPE, Manhattan, Kiss32Random,AnnoyIndexSingleThreadedBuildPolicy> MyAnnoyIndex;

// std::string met = "DotProduct";
// typedef AnnoyIndex<int, ANNOYTYPE, DotProduct, Kiss32Random,AnnoyIndexSingleThreadedBuildPolicy> MyAnnoyIndex;

   
    
class ReadCSV{
    public:
        std::vector<std::vector<ANNOYTYPE>> points;
        std::vector<long long> pids;
        ReadCSV(const std::string& path, std::string x_str,std::string y_str,std::string z_str);
        int get_freq(long long pid);
    private:
        void points_csv(const std::string& path, std::string x_str,std::string y_str,std::string z_str);
};

template<typename T>
void print_queue(T q) { // NB: pass by value so the print uses a copy
    while(!q.empty()) {
        std::cout << q.top() << ' ';
        q.pop();
    }
    std::cout << '\n';
}

ReadCSV::ReadCSV(const std::string& path, std::string x_str,std::string y_str,std::string z_str){
    points_csv(path, x_str,y_str,z_str);
}

void ReadCSV::points_csv(const std::string& path, std::string x_str,std::string y_str,std::string z_str){
    points.clear();
    pids.clear();
    auto ss = std::ostringstream{};
    std::ifstream input_file(path);
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file - '"
             << path << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
    ss << input_file.rdbuf();
    std::string file_contents = ss.str();
    char delimiter = ',';
    std::istringstream sstream(file_contents);
    std::string record;
    int counter = 0;
    int x,y,z, pid_idx;
    while (std::getline(sstream, record)) {
        std::istringstream line(record);
        std::vector<ANNOYTYPE> point(3);
        long long pid;
        int line_idx = 0;
        while (std::getline(line, record, delimiter)) {
            if (counter == 0){
                    if (record == x_str) x = line_idx;
                    if (record == y_str) y = line_idx;
                    if (record == z_str) z = line_idx;
                    if (record == "particle_id") pid_idx = line_idx;
                }
            if (counter > 0){
                if (line_idx == x) point[0] = stod(record);
                if (line_idx == y) point[1] = stod(record);
                if (line_idx == z) point[2] = stod(record);
                if (line_idx == pid_idx) pid = stoll(record);
            }
            line_idx += 1;
        }
        pids.push_back(pid);
        points.push_back(point);
        counter += 1;
    } 
}

int ReadCSV::get_freq(long long pid){
    int counter = 0;
    for (const auto& x : pids){
        if (pid == x) counter += 1;
    }
    return counter;
}



void freq_analyze(const MyAnnoyIndex& complete_obj_index,const ReadCSV& data_read, int kNN, std::vector<TH1F>& hist_list){
    std::string title = "freq_"+std::to_string(kNN);
    auto h1f = new TH1F("h1f",title.c_str(),kNN,0,kNN);
    TCanvas *c1 = new TCanvas("c1",title.c_str(),200,10,700,900);
    
    auto t1 = high_resolution_clock::now();
    for (int hit_id = 0; hit_id < data_read.points.size(); hit_id++){
        std::vector<int> nn_idx;
        std::vector<ANNOYTYPE> nn_dist;
        complete_obj_index.get_nns_by_item(hit_id, kNN+1, -1, &nn_idx, &nn_dist);
//         std::cout<<"ANNOY"<<std::endl;
        std::unordered_map<long long, int> bucket_id;



        for(int i=0; i < nn_dist.size(); i++){
//             std::cout<<nn_idx[i]<<std::endl; 
            if (!bucket_id.count(data_read.pids[nn_idx[i]])){
                bucket_id.insert(std::pair<long long,int>(data_read.pids[nn_idx[i]],1));
            }
            else {
                bucket_id[data_read.pids[nn_idx[i]]] += 1;
            }
//             std::cout<<data_read.pids[nn_idx[i]]<<std::endl;
        }

        int max_int = 0;
        long long max_key = 0;

        for (const auto& [key,value] : bucket_id){
    //         std::cout<<"key :"<<key<<" | value :"<<value<<std::endl;
            if (value > max_int) {
                max_int = value;
                max_key = key;
            }  
        }
        h1f->Fill(max_int);
    }
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << kNN << ": " << ms_double.count() << "ms \n";
    
    h1f->Draw("HIST");
    c1->Update();
    std::string filename = "freq_"+std::to_string(kNN)+".png";
    c1->Print(filename.c_str());
    hist_list.push_back(*h1f);
    delete c1; delete h1f;
//     return h1f;
}



int main(){
//     using std::chrono::high_resolution_clock;
//     using std::chrono::duration_cast;
//     using std::chrono::duration;
//     using std::chrono::milliseconds;
    
    std::string data("/global/cscratch1/sd/rkarur/data/sim/ttbar_14tev_mu140_fatras_generic/event000000099-hits.csv");
    ReadCSV data_read(data,"tx","ty","tz");
    TCanvas c1 = new TCanvas("c1","Default Hist Title",200,10,700,900);
    MyAnnoyIndex obj(3);
    for (size_t i = 0; i < data_read.points.size(); i++){
        obj.add_item(i,data_read.points[i].data());
    }
    
//SUPPOSE data_read.points is std::vector<SimSpacePoint>
//     for (size_t i = 0; i < data_read.points.size(); i++){
//         std::vector<float> s_points(3);
//         s_points[0] = data_read.points[i].x();
//         s_points[1] = data_read.points[i].y();
//         s_points[2] = data_read.points[i].z();
//         obj.add_item(i,s_points.data());
//     }

//SUPPOSE sim_space is std::vector<SimSpacePoint>
//     std::vector<std::vector<float>> s_points(sim_space.size());
//     for (size_t i = 0; i < s_points.size(); i++){
//         s_points[i][0] = sim_space[i].x();
//         s_points[i][1] = sim_space[i].y();
//         s_points[i][2] = sim_space[i].z();
//         obj.add_item(i, s_points[i].data());
//     }
    
    obj.build(5);

    std::vector<TH1F> hist_list;
    freq_analyze(obj,data_read,20,hist_list);
    freq_analyze(obj,data_read,50,hist_list);
    
    std::cout<<hist_list.size();
    TFile *MyFile = new TFile(("HistList_"+met+".root").c_str(),"RECREATE");
    for(int i =0;i<hist_list.size();i++){
        hist_list[i].Write((std::to_string(i)).c_str());
        
    }
    MyFile->Close();
    return 0;
}