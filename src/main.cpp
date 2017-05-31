#ifdef __unix__
    #include <dirent.h>
#elif _WIN32
    #include <windows.h>
#endif
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <ctime>

#include "../includes/MLToolkit.hpp"

using namespace std;

bool sair = false, inva = false;
double max_time = 100.0f;
string data_folder = "DB/";
Data data;

//Menus utilities
void clear(void);
vector<string> list_datasets(bool list);
bool valid_file(string file);
void waitUserAction(void);
void exitProgram(void);

//Prints that are common in all menus
void header(void);
int selector(void);

//Functions to show menus options
void mainMenu(void);
void datasetMenu(void);
void dataMenu(void);

//Functions to call the execution of the menus options
void mainOption(int);
void classifiersOption(int);
void datasetOption(int);
void dataOption(int);

int main(){
    while (true) {
        if(sair) exitProgram();
        clear();
        mainMenu();
    }

    return 0;
}

bool valid_file(string file){
    int i;
    bool flag = false;
    string ext;

    if(file.empty() || file.size() < 4)
        return false;

    for(i = file.size()-1; i >= 0; i--){
        if(file[i] == '.') break;
        string f(1, file[i]);
        ext = f + ext;
    }

    for(string type : types){
        if(type == ext) flag = true;
    }

    return flag;
}

vector<string> list_datasets(bool list){
    vector<string> files;

    #ifdef __unix__
        DIR *dpdf;
        struct dirent *epdf;

        string temp = data_folder;
        temp.erase(temp.begin() + temp.size()-1);
        string path = "./" + data_folder;

        dpdf = opendir(path.c_str());
        if(dpdf != NULL){
            while((epdf = readdir(dpdf))){
                string file = string(epdf->d_name);
                if(valid_file(file) && !file.empty()){

                    if(list) cout << "[" << files.size() << "] " << file << endl;
                    files.push_back(file);
                }
            }
        }

        closedir(dpdf);
    #elif _WIN32
        HANDLE hFind;
        WIN32_FIND_DATA data;

        hFind = FindFirstFile(".\\DB\\*.*", &data);
        if (hFind != INVALID_HANDLE_VALUE) {
          do {
            string file_name(data.cFileName);
            if(valid_file(file_name) && !file_name.empty()){
                if(list) cout << "[" << files.size() << "] " << file_name << endl;
                files.push_back(file_name);
            }
          } while (FindNextFile(hFind, &data));
          FindClose(hFind);
        }
    #else
        cout << "This system is not supported for this function..." << endl;
    #endif

    return files;
}

void clear(void){
    #ifdef __unix__
        system("clear");
    #elif _WIN32
        system("CLS");
    #else
        int n;
        for (n = 0; n < 10; n++)
          printf( "\n\n\n\n\n\n\n\n\n\n" );
    #endif
}

void exitProgram(void){
    exit(0);
}

void waitUserAction(void){
    cout << "\nPress ENTER to continue..." << endl;
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    cin.get();
}

void header(void){
    cout << "         *----------------------------------------------------------* " << endl;
    cout << "         *                 Machine Learning Toolkit                 * " << endl;
    cout << "         *----------------------------------------------------------* " << endl;
    cout << endl;
    cout << "Select an option:\n" << endl;
}

int selector(void){
    int o;
    string opt;

    if(inva){
        cout << endl;
        cout << "Invalid option." << endl;
        inva = false;
        cout << endl;
    }
    cout << " > ";
    cin >> opt;
    istringstream iss(opt);
    iss >> o;
    clear();
    return o;
}

void mainMenu(){
    int option;

    clear();
    header();

    cout << "1 - Dataset" << endl;
    cout << "2 - Data" << endl;
    cout << "3 - Classifiers" << endl;
    cout << "4 - Emsembles" << endl;
    cout << "5 - Feature Selection" << endl;
    cout << "6 - Genomic" << endl;
    cout << "7 - Utils" << endl;
    cout << "0 - Exit" << endl;

    option = selector();
    mainOption(option);
}

void datasetMenu(void){
    int option;

    clear();
    header();

    cout << "1 - Load a dataset" << endl;
    cout << "2 - Dataset information " << endl;
    cout << "3 - Features names" << endl;
    cout << "4 - Set max time" << endl;
    cout << "5 - Divide dataset in Train/Test" << endl;
    cout << "6 - Save Train/Test dataset" << endl;
    cout << "0 - Back to the main menu" << endl;

    option = selector();
    datasetOption(option);
}

void dataMenu(void){
    int option;

    clear();
    header();

    cout << "1 - Insert features" << endl;
    cout << "2 - Remove features" << endl;
    cout << "3 - Remove point" << endl;
    cout << "4 - Data variance" << endl;
    cout << "5 - Data radius" << endl;
    cout << "6 - Distance from the center of the classes" << endl;
    cout << "7 - Normalize dataset" << endl;
    cout << "0 - Back to the main menu" << endl;

    option = selector();
    dataOption(option);
}


void mainOption(int option){
    switch(option){
        case 1:
            datasetMenu();
            break;
        case 2:
            dataMenu();
            break;
        case 3:
            //classifiersMenu();
            break;
        case 4:
           // emsemblesMenu();
            break;
        case 5:
            //featureSelectionMenu();
            break;
        case 6:
            //genomicMenu();
            break;
        case 7:
            //utilsMenu();
            break;
        case 0:
            exitProgram();
            break;
        default:
            inva = true;
        break;
    }
    mainMenu();
}

void datasetOption(int option){
    int i = 0;
    bool list = false;
    vector<string> files;

    switch(option){
        case 1:
            if(data.isEmpty()){
                string pos, neg;
                string sid, path;

                cout << "List all available datasets on DB folder?[y|n]" << endl;
                cout << " > ";
                cin >> sid;

                if(sid == "y"){
                    list = true;
                }

                files = list_datasets(list);
                cout << endl;
                cout << "Enter the number of the DB (must be in the DB folder): ";
                cin >> sid;
                cout << "Enter the positive class: ";
                cin >> pos;
                cout << "Enter the negative class: ";
                cin >> neg;

                path = data_folder + files[stoi(sid)];
                clock_t begin = clock();
                data.setClasses(pos, neg);
                cout << path << endl;
                data.load(path);
                clock_t end = clock();

                double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                cout << endl;
                cout << elapsed_secs << " seconds to load.\n";
            }else{
                char y;
                cout << "Which to load a new dataset?[y|n]" << endl;
                cout << " > ";
                cin >> y;

                if(y == 'y'){
                    data.clear();
                    /*if(test_sample){
                        delete test_sample;
                        test_sample = NULL;
                    }
                    _data = NULL;*/
                    cout << "\nOld dataset erased, select this option again."<< endl;
                }
            }

            waitUserAction();
            break;
        case 2:
            if(!data.isEmpty()){
                cout << "Number of features: ";
                cout << data.getDim() << endl;
                cout << "Number of samples: ";
                cout << data.getSize() << endl;
                cout << "Negative points: ";
                cout << data.getNumberNegativePoints() << endl;
                cout << "Positive points: ";
                cout << data.getNumberPositivePoints() << endl;

                /*if(test_sample != NULL){
                    cout << "\n\nTest sample information\n\n";
                    cout << "Number of features: ";
                    cout << test_sample->dim << endl;
                    cout << "Number of samples: ";
                    cout << test_sample->size << endl;
                    cout << "Negative points: ";
                    cout << test_sample->nneg_samples << endl;
                    cout << "Positive points: ";
                    cout << test_sample->npos_samples << endl;
                }*/
            }else cout << "Load a dataset first...\n\n";

            waitUserAction();
            break;

        case 3:
            if(!data.isEmpty()){
                vector<int> fnames = data.getFeaturesNames();

                for(int f : fnames){
                    cout << f << " : ";
                }
                cout << endl;
            }else cout << "Load a dataset first...\n\n";

            waitUserAction();
            break;
        case 4:
            cout << "Enter the max time: ";
            cin >> max_time;

            waitUserAction();
            break;
        case 5:
            /*if(_data != NULL && !_data->empty()){
                int fold;
                unsigned int seed;

                if(!test_sample){
                    Sample *sample = Sample::copy(_data->get_samples());
                    test_sample = new Sample;

                    cout << "K-Fold: ";
                    cin >> fold;
                    cout << "Seed for timestamps: ";
                    cin >> seed;

                    clock_t begin = clock();
                    test_sample = _data->part_train_test(sample, test_sample, fold, seed, verbose);
                    clock_t end = clock();

                    cout << "\nDone!" << endl;
                    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                    cout << "Size of the test sample: " << test_sample->size << endl;
                    cout << endl;
                    cout << elapsed_secs << " seconds to compute.\n";

                }else cout << "Test sample already divided...\n";
            }else cout << "Load a dataset first...\n\n";
            */
            waitUserAction();
            break;
        case 6:
            /*if(_data != NULL && !_data->empty()){
                if(!test_sample){
                    cerr << "Divide the train/test datasets first...\n" << endl;
                }else{
                    string outfile = _data->get_dataset_name();
                    Sample *sample = Sample::copy(_data->get_samples());

                    outfile = outfile + "_train";
                    _data->write(outfile, sample, 0);
                    outfile = _data->get_dataset_name();
                    outfile = outfile + "_test";
                    _data->write(outfile, test_sample, 0);

                    delete sample;
                }
            }else cout << "Load a dataset first...\n\n";
            */
            waitUserAction();
            break;
        case 0:
            mainMenu();
            break;
        default:
            inva = true;
        break;
    }
    datasetMenu();
}

void dataOption(int option){
    int i = 0, j = 0;

    switch(option){
            case 1:
                /*if(_data != NULL && !_data->empty()){
                    int totalFeat, flag_feat, f, fnamesize;
                    vector<int> feats;
                    vector<int> fnames = _data->get_fnames();

                    fnamesize = fnames.size();
                    cout << "Insert how many features: ";
                    cin >> totalFeat;
                    feats.resize(totalFeat);

                    for(i = 0; i < totalFeat; i++){
                        cout << "Feature " << i + 1 << ": ";
                        cin >> f;
                        feats[i] = f;

                        for(flag_feat = 0, j = 0; j < fnamesize; j++){
                            if(feats[i] == fnames[j]){
                                flag_feat = 1;
                            }
                        }

                        if(!flag_feat){
                            cout << "Feature " << feats[i] << " does not belongs to the set.\n";
                            i--;
                            cin.clear();
                        }
                    }

                    Sample *samples = Sample::copy(_data->get_samples());
                    Sample *sample_temp = _data->insert_features(samples, feats, totalFeat, verbose);
                    _data->set_samples(sample_temp);
                }else cout << "Load a dataset first...\n\n";
                */
                waitUserAction();
                break;
        case 2:
            /*if(_data != NULL && !_data->empty()){
                int totalFeat, flag_feat, f, fnamesize;
                vector<int> feats;
                vector<int> fnames = _data->get_fnames();

                fnamesize = fnames.size();

                cout << "Remove how many features: ";
                cin >> totalFeat;

                for(i = 0; i < totalFeat; i++){
                    cout << "Feature " << i + 1 << ": ";
                    cin >> f;
                    feats.push_back(f);
                    for(flag_feat = 0, j = 0; j < fnamesize; j++){
                        if(feats[i] == fnames[j]){
                            flag_feat = 1;
                        }
                    }

                    if(!flag_feat){
                        cout << "Feature " << feats[i] << " does not belongs to the set.\n";
                        i--;
                    }
                }
                cout << endl;
                Sample *samples = _data->get_samples();
                _data->remove_features(samples, feats);
            }else cout << "Load a dataset first...\n\n";
            */
            waitUserAction();
            break;
        case 3:
            /*if(_data != NULL && !_data->empty()){
                Sample* samp = _data->get_samples();

                cout << "Point to remove index: ";
                cin >> i;

                _data->remove_point(samp, i);
            }else cout << "Load a dataset first...\n\n";
            */
            waitUserAction();
            break;
        case 4:
            /*if(_data != NULL && !_data->empty()){
                int index;

                cout << "Feature to be ignored: ";
                cin >> index;
                clock_t begin = clock();
                cout << endl;
                cout << "The variance values is: " << _data->get_variance(index) << endl;
                clock_t end = clock();

                double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                cout << endl;
                cout << elapsed_secs << " seconds to compute.\n";
            }else cout << "Load a dataset first...\n\n";
            */
            waitUserAction();
            break;
        case 5:
           /* if(_data != NULL && !_data->empty()){
                int index, q;

                cout << "Feature to be ignored: ";
                cin >> index;
                cout << "Value of q [q = 1 | q = 2]: ";
                cin >> q;
                cout << endl;

                clock_t begin = clock();
                cout << "The value of the radius is: " << _data->get_radius(index, q) << endl;
                clock_t end = clock();

                double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                cout << endl;
                cout << elapsed_secs << " seconds to compute.\n";
            }else cout << "Load a dataset first...\n\n";
            */
            waitUserAction();
            break;
        case 6:
            /*if(_data != NULL && !_data->empty()){
                int index;

                cout << "Feature to be ignored: ";
                cin >> index;
                cout << endl;

                clock_t begin = clock();
                cout << "The value of the center of the classes are: " << _data->get_dist_centers(index) << endl;
                cout << endl;
                clock_t end = clock();

                double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

                cout << elapsed_secs << " seconds to compute.\n";
            }else cout << "Load a dataset first...\n\n";
            */
            waitUserAction();
            break;
        case 7:
            /*if(_data != NULL && !_data->empty()){
                int q, p;

                cout << "Value of q: ";
                cin >> q;
                cout << endl;

                if(q == -1.0){
                    p = 1.0;
                }else if(q == 1.0){
                    p = 100.0;
                }else{
                    p = q/(q-1.0);
                }

                clock_t begin = clock();
                _data->set_norm(p, q);
                _data->normalize_dataset();
                cout << "The dataset was normalized." << endl;
                clock_t end = clock();

                double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                cout << endl;
                cout << elapsed_secs << " seconds to compute.\n";
            }else cout << "Load a dataset first...\n\n";
            */
            waitUserAction();
            break;
        case 0:
            mainMenu();
            break;
        default:
            inva = true;
        break;
    }
    dataMenu();
}


