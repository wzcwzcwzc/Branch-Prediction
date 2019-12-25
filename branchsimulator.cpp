#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <bitset>
#include <map>

using namespace std;

//hex->binary
bitset<32> toBin(unsigned long pc){
    bitset<32> bin = bitset<32>(pc);
    return bin;
}

//string binary -> decimal unsigned long
unsigned long toUnsignedLong(string str){
    unsigned long ans = stoul(str, 0, 2);
    return ans;
}


int toInt(string str){
    int ans = stoi(str, 0, 2);
    return ans;
}

//get col num from BHR
int getValueOfBHR(vector<int> vec){
    string str;
    for(int i = 0; i < vec.size(); i++){
        str += to_string(vec.at(i));
    }
    int ans = stoi(str, 0, 2);
    return ans;
}

int getPredictResult(int sc){
    if(sc == 11) {
        return 1;
    }else if(sc == 10){
        return 1;
    }else if(sc == 1){
        return 0;
    }else if(sc == 0){
        return 0;
    }

    return -1;
}

int updatePHT(int old_saturate_counter, int taken){

    //build up finite state machine
    if(old_saturate_counter == 11 && taken == 1){
        return 11;
    }else if(old_saturate_counter == 10 && taken == 1){
        return 11;
    }else if(old_saturate_counter == 11 && taken == 0){
        return 10;
    }else if(old_saturate_counter == 10 && taken == 0){
        return 0;
    }else if(old_saturate_counter == 0 && taken == 1){
        return 1;
    }else if(old_saturate_counter == 0 && taken == 0){
        return 0;
    }else if(old_saturate_counter == 1 && taken == 1){
        return 11;
    }else if(old_saturate_counter == 1 && taken == 0){
        return 0;
    }

    return -1;
}


int main (int argc, char** argv) {
    ifstream config;
    config.open(argv[1]);

    int m, k;
    config >> m >> k;

    config.close();

    ofstream out;
    string out_file_name = string(argv[2]) + ".out";
    out.open(out_file_name.c_str());

    ifstream trace;
    trace.open(argv[2]);
    unsigned long pc; bool taken;
    trace >> std::hex >> pc >> taken;

    //initial PHT
    map<int, vector<int> > PHT;
    int pht_col_size = pow(2, k);
    int pht_row_size = pow(2, m);
    vector<int> saturating_vec(pht_row_size);
    for(int i = 0; i < saturating_vec.size(); i++){
        saturating_vec.at(i) = 11;
    }

    for(int i = 0; i < pht_col_size; i++){
        PHT.insert(pair<int, vector<int> >(i, saturating_vec));
    }

    vector<int> BHR(k);
    for(int i = 0; i < BHR.size(); i++) {
        BHR.at(i) = 1;
    }

    while (!trace.eof()) {

        int row = toInt(toBin(pc).to_string().substr(32-m, m));
        int col = getValueOfBHR(BHR);

        int saturate_counter = PHT.at(col).at(row);
        int saturate_predict = getPredictResult(saturate_counter);
        out << saturate_predict << endl;

        int new_sc = updatePHT(saturate_counter, (int)taken);
        PHT.at(col).at(row) = new_sc;

        BHR.insert(BHR.begin(), (int)taken);
        BHR.pop_back();

        trace >> std::hex >> pc >> taken;
    }
    trace.close();
    out.close();
}

