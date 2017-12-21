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
#include "../includes/Perceptron.hpp"
#include "../includes/IMA.hpp"

using namespace std;

bool sair = false, inva = false;
double max_time = 100.0f;
string data_folder = "DB/";
Data data;
Data test_sample;
Data train_sample;
Solution sol;
Visualisation plot(&data);

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
void visualisationMenu(void);
void classifiersMenu(void);

//Functions to call the execution of the menus options
void mainOption(int);
void datasetOption(int);
void dataOption(int);
void visualisationOption(int);
void classifiersOption(int);
void primalClassifiersOption(int);
void dualClassifiersOption(int);


int main(int argc, char* argv[]){
    if(argc > 1){
        data.load(string(argv[1]));
    }
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
        string path = ".\\" + data_folder + "\\*.*";

        hFind = FindFirstFile(path.c_str(), &data);
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
    cout << "3 - Data Visualisation" << endl;
    cout << "4 - Classifiers" << endl;
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
    cout << "8 - Print dataset" << endl;
    cout << "0 - Back to the main menu" << endl;

    option = selector();
    dataOption(option);
}

void visualisationMenu(void){
    int option;

    clear();
    header();

    cout << "1 - Plot features in 2D" << endl;
    cout << "2 - Plot features in 3D" << endl;
    cout << "3 - Plot features in 2D with hyperplane" << endl;
    cout << "4 - Plot features in 3D with hyperplane" << endl;
    cout << "0 - Back to the main menu" << endl;

    option = selector();
    visualisationOption(option);
}

void classifiersMenu(){
    int option;

    clear();
    header();

    cout << "1 - Primal Classifiers" << endl;
    cout << "2 - Dual Classifiers" << endl;
    cout << "0 - Back to the main menu" << endl;

    option = selector();
    classifiersOption(option);
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
            visualisationMenu();
            break;
        case 4:
            classifiersMenu();
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

                /*cout << "List all available datasets on DB folder?[y|n]" << endl;
                cout << " > ";
                cin >> sid;

                if(sid == "y"){
                    list = true;
                }*/

                files = list_datasets(true);
                cout << endl;
                cout << "Enter the number of the DB (must be in the DB folder): ";
                cin >> sid;
                /*cout << "Enter the positive class: ";
                cin >> pos;
                cout << "Enter the negative class: ";
                cin >> neg;*/

                path = data_folder + files[stoin(sid)];
                clock_t begin = clock();
                //data.setClasses(pos, neg);
                cout << "\n" << path << endl;
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
                    if(!test_sample.isEmpty()){
                        test_sample.clear();
                        train_sample.clear();
                    }
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

                if(!test_sample.isEmpty()){
                    cout << "\n\nTest sample information\n\n";
                    cout << "Number of features: ";
                    cout << test_sample.getDim() << endl;
                    cout << "Number of samples: ";
                    cout << test_sample.getSize() << endl;
                    cout << "Negative points: ";
                    cout << test_sample.getNumberNegativePoints() << endl;
                    cout << "Positive points: ";
                    cout << test_sample.getNumberPositivePoints() << endl;
                }
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
            if(!data.isEmpty()){
                int fold;
                unsigned int seed;
                Validation valid(&data);

                if(test_sample.isEmpty()){
                    cout << "K-Fold: ";
                    cin >> fold;
                    cout << "Seed for timestamps: ";
                    cin >> seed;

                    clock_t begin = clock();
                    valid.partTrainTest(fold, seed);
                    test_sample = valid.getTestSample();
                    train_sample = valid.getTrainSample();
                    clock_t end = clock();

                    cout << "\nDone!" << endl;
                    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                    cout << "Size of the test sample: " << test_sample.getSize() << endl;
                    cout << endl;
                    cout << elapsed_secs << " seconds to compute.\n";

                }else cout << "Test sample already divided...\n";
            }else cout << "Load a dataset first...\n\n";

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
            if(!data.isEmpty()){
                int totalFeat, flag_feat, f, fnamesize;
                vector<int> feats;
                vector<int> fnames = data.getFeaturesNames();

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
                Data temp = data.insertFeatures(feats);

                if(!temp.isEmpty()){
                    data = temp;
                }else{
                    cerr << "Something went wrong." << endl;
                }
            }else cout << "Load a dataset first...\n\n";

            waitUserAction();
            break;
        case 2:
            if(!data.isEmpty()){
                int totalFeat, flag_feat, f, fnamesize;
                vector<int> feats;
                vector<int> fnames = data.getFeaturesNames();

                fnamesize = fnames.size();

                cout << endl;
                while(true){
                    cout << "Remove how many features (-1 to cancel): ";
                    cin >> totalFeat;
                    if(totalFeat > fnamesize - 1){
                        clear();
                        cout << "Can't remove more features than exist." << endl;
                    }else if(totalFeat >= 0){
                        break;
                    }
                }

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
                data.removeFeatures(feats);

                fnames = data.getFeaturesNames();

            }else cout << "Load a dataset first...\n\n";

            waitUserAction();
            break;
        case 3:
            if(!data.isEmpty()){
                cout << "Point to remove index (-1 to cancel): ";
                cin >> i;

                if(i < 0){
                   break;
                }

                data.removePoint(i);
            }else cout << "Load a dataset first...\n\n";

            waitUserAction();
            break;
        case 4:
            if(!data.isEmpty()){
                int index;

                cout << "Feature to be ignored (-1 doesnt ignore any feature): ";
                cin >> index;
                clock_t begin = clock();
                cout << endl;
                cout << "The variance values is: " << Statistics::variance(data, index) << endl;
                clock_t end = clock();

                double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                cout << endl;
                cout << elapsed_secs << " seconds to compute.\n";
            }else cout << "Load a dataset first...\n\n";

            waitUserAction();
            break;
        case 5:
            if(!data.isEmpty()){
                int index, q;

                cout << "Feature to be ignored: ";
                cin >> index;
                cout << "Value of q [q = 1 | q = 2]: ";
                cin >> q;
                cout << endl;

                clock_t begin = clock();
                cout << "The value of the radius is: " << Statistics::getRadius(data, index, q) << endl;
                clock_t end = clock();

                double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                cout << endl;
                cout << elapsed_secs << " seconds to compute.\n";
            }else cout << "Load a dataset first...\n\n";

            waitUserAction();
            break;
        case 6:
            if(!data.isEmpty()){
                int index;

                cout << "Feature to be ignored: ";
                cin >> index;
                cout << endl;

                clock_t begin = clock();
                cout << "The value of the center of the classes are: " << Statistics::getDistCenters(data, index) << endl;
                cout << endl;
                clock_t end = clock();

                double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

                cout << elapsed_secs << " seconds to compute.\n";
            }else cout << "Load a dataset first...\n\n";

            waitUserAction();
            break;
        case 7:
            if(!data.isEmpty()){
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
                data.normalize(p);
                cout << "The dataset was normalized." << endl;
                clock_t end = clock();

                double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                cout << endl;
                cout << elapsed_secs << " seconds to compute.\n";
            }else cout << "Load a dataset first...\n\n";

            waitUserAction();
            break;
        case 8:
            if(!data.isEmpty()){
                cout << data << endl;
            }else cout << "Load a dataset first...\n\n";

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

void visualisationOption(int opt){
    int x, y, z;

    switch (opt) {
    case 1:
        cout << "Enter the feature to plot in the x-axis: ";
        cin >> x;
        cout << "Enter the feature to plot in the y-axis: ";
        cin >> y;

        plot.plot2D(x, y);
        break;
    case 2:
        cout << "Enter the feature to plot in the x-axis: ";
        cin >> x;
        cout << "Enter the feature to plot in the y-axis: ";
        cin >> y;
        cout << "Enter the feature to plot in the z-axis: ";
        cin >> z;
        plot.plot3D(x, y, z);
        break;
    case 3:
        if(sol.w.size() == 0){
            cout << "Run a classifier in the data first." << endl;
            waitUserAction();
            break;
        }
        cout << "Enter the feature to plot in the x-axis: ";
        cin >> x;
        cout << "Enter the feature to plot in the y-axis: ";
        cin >> y;

        plot.plot2DwithHyperplane(x, y, sol);
        break;
    case 4:
        if(sol.w.size() == 0){
            cout << "Run a classifier in the data first." << endl;
            waitUserAction();
            break;
        }
        cout << "Enter the feature to plot in the x-axis: ";
        cin >> x;
        cout << "Enter the feature to plot in the y-axis: ";
        cin >> y;
        cout << "Enter the feature to plot in the z-axis: ";
        cin >> z;

        plot.plot3DwithHyperplane(x, y, z, sol);
        break;
    case 0:
        mainMenu();
        break;
    default:
        inva = true;
        break;
    }
    visualisationMenu();
}

void classifiersOption(int option){
    int opt;

    switch (option) {
    case 1:
        clear();
        header();
        cout << "1 - Perceptron Primal" << endl;
        cout << "2 - Perceptron Primal with fixed margin" << endl;
        cout << "3 - Incremental Margin Algorithm Primal (IMAp)" << endl;
        cout << "0 - Back to classifiers menu" << endl;
        opt = selector();
        primalClassifiersOption(opt);
        break;
    case 2:
        clear();
        header();
        cout << "1 - Perceptron Dual" << endl;
        cout << "2 - Perceptron Dual with fixed margin" << endl;
        cout << "0 - Back to classifiers menu" << endl;
        opt = selector();
        dualClassifiersOption(opt);
        break;
    case 0:
        mainMenu();
        break;
    default:
        inva = true;
        break;
    }
    classifiersMenu();
}

void primalClassifiersOption(int option){
    int p, q, i, norm, flexible, svs;
    double rate, gamma, alpha_prox;

    switch (option) {
    case 1:
        if(!data.isEmpty()){
            cout << "Value of the learning rate: ";
            cin >> rate;
            cout << "Value of the q norm: ";
            cin >> q;
            cout << endl;

            PerceptronPrimal perc(&data, q, rate);

            perc.train();
            sol = perc.getSolution();

            cout << "Number of steps through data: " << perc.getSteps() << endl;
            cout << "Number of updates: " << perc.getUpdates() << endl;
            cout << "Weights vector:" << endl;
            cout << "[";
            for(i = 0; i < sol.w.size(); i++){
                cout << sol.w[i] << ", ";
            }
            cout << sol.bias <<  "]" << endl;
            cout << endl;
            waitUserAction();
        }else{
            cout << "Load a dataset first..." << endl;
        }
        break;
    case 2:
        if(!data.isEmpty()){
            cout << "Value of the learning rate: ";
            cin >> rate;
            cout << "Value of the q norm: ";
            cin >> q;
            cout << "Gamma value: ";
            cin >> gamma;
            cout << endl;

            PerceptronFixedMarginPrimal perc(&data, gamma, q, rate);

            perc.train();
            sol = perc.getSolution();

            cout << "Number of steps through data: " << perc.getSteps() << endl;
            cout << "Number of updates: " << perc.getUpdates() << endl;
            cout << "Weights vector:" << endl;
            cout << "[";
            for(i = 0; i < sol.w.size(); i++){
                cout << sol.w[i] << ", ";
            }
            cout << sol.bias <<  "]" << endl;
            cout << endl;
            waitUserAction();
        }else{
            cout << "Load a dataset first..." << endl;
        }
        break;
    case 3:
        if(!data.isEmpty()){
          cout << "[1]p or [2]q norm: ";
          cin >> norm;
          cout << endl;

          if(norm == 1){
            cout << "p-norm value: ";
            cin >> p;
            if(p == 1.0){
              q = -1.0;
            }else{
              q = p/(p-1.0);
            }
          }else{
            cout << "q-norm value: ";
            cin >> q;
            if(q == -1.0){
              p = 1.0;
            }else if(q == 1.0){
              p = 100.0;
            }else{
              p = q/(q-1.0);
            }
          }
          cout << endl;

          cout << "Flexibilization value [0 - no flexibilization]: ";
          cin >> flexible;
          cout << endl;

          cout << "Alpha aproximation value [1 - alpha]: ";
          cin >> alpha_prox;
          cout << endl;

          IMAp imap(&data);

          imap.setpNorm(p);
          imap.setqNorm(q);
          imap.setFlexible(flexible);
          imap.setAlphaAprox(alpha_prox);

          clock_t begin = clock();

          if(imap.train()){
            sol = imap.getSolution();
            cout << "Training successful..." << endl;
            cout << "\nMargin = " << sol.margin << ", Support Vectors = " << sol.svs << "\n" << endl;
          }

          clock_t end = clock();

          double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
          cout << endl;
          cout << elapsed_secs << " seconds to compute.\n";

          waitUserAction();
        }else{
            cout << "Load a dataset first..." << endl;
        }
        break;
    case 0:
        classifiersMenu();
        break;
    default:
        inva = true;
        break;
    }
    classifiersMenu();
}

void dualClassifiersOption(int option){
    int i, kernel_type, kernel_param;
    double rate;
    Kernel K;

    switch (option) {
    case 1:
        if(!data.isEmpty()){
            cout << "Learning rate: ";
            cin >> rate;
            cout << "Kernel [0]Inner Product [1]Polynomial [2]Gaussian: ";
            cin >> kernel_type;

            if(kernel_type != 0){
                if(kernel_type == 1){
                    cout << "Polynomial degree: ";
                }else{
                    cout << "Gaussian gamma: ";
                }
                cin >> kernel_param;
            }

            K.setType(kernel_type);
            K.setParam(kernel_param);
            K.compute(data);

            PerceptronDual perc_dual(&data, rate, &K);
            perc_dual.train();

            sol = perc_dual.getSolution();
            cout << endl;
            cout << "Alpha vector:" << endl;
            cout << "[";
            for(i = 0; i < sol.alpha.size(); i++){
                cout << sol.alpha[i] << ", ";
            }
            cout << sol.bias <<  "]" << endl;
            cout << endl;
            cout << "Weights vector:" << endl;
            cout << "[";
            for(i = 0; i < sol.w.size(); i++){
                cout << sol.w[i] << ", ";
            }
            cout << sol.bias <<  "]" << endl;
            cout << endl;
        }else{
            cout << "Load a dataset first..." << endl;
        }
        waitUserAction();
        break;
    case 2:
        if(!data.isEmpty()){
            double gamma;
            cout << "Learning rate: ";
            cin >> rate;
            cout << "Gamma value: ";
            cin >> gamma;
            cout << "Kernel [0]Inner Product [1]Polynomial [2]Gaussian: ";
            cin >> kernel_type;

            if(kernel_type != 0){
                if(kernel_type == 1){
                    cout << "Polynomial degree: ";
                }else{
                    cout << "Gaussian gamma: ";
                }
                cin >> kernel_param;
            }

            K.setType(kernel_type);
            K.setParam(kernel_param);
            K.compute(data);

            PerceptronFixedMarginDual perc_fixmargin_dual(&data, gamma, rate, &K);
            perc_fixmargin_dual.train();

            sol = perc_fixmargin_dual.getSolution();
            cout << endl;
            cout << "Alpha vector:" << endl;
            cout << "[";
            for(i = 0; i < sol.alpha.size(); i++){
                cout << sol.alpha[i] << ", ";
            }
            cout << sol.bias <<  "]" << endl;
            cout << endl;

            cout << "Weights vector:" << endl;
            cout << "[";
            for(i = 0; i < sol.w.size(); i++){
                cout << sol.w[i] << ", ";
            }
            cout << sol.bias <<  "]" << endl;
            cout << endl;
        }else{
            cout << "Load a dataset first..." << endl;
        }
        waitUserAction();
        break;
    case 0:
        classifiersMenu();
        break;
    default:
        break;
    }
    classifiersMenu();
}
