#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <chrono>

/*
 Team: Abed Ahmed (ASA190005)  and Dylan Kapustka (DLK190000)
 10/02/2022
 HomeWork 4 - ML From Scratch
 */

using namespace std;
using namespace std::chrono;


const int MAX_LEN = 2000;
vector<double> id(MAX_LEN);
vector<double> pClass(MAX_LEN);
vector<double> survived(MAX_LEN);
vector<double> sex(MAX_LEN);
vector<double> age(MAX_LEN);

bool custom_sort(double a, double b);
void logisticRegression();
void naiveBayes();
int readCSV();

int main(){
    readCSV();
    logisticRegression();
    naiveBayes();
}

void logisticRegression(){

    auto start = high_resolution_clock::now();

    double  e = 2.71828;
    double b0 = 0;
    double b1 = 0;
    double lr = 0.01; //  the learning rate that controls how much the coefficients change and learn each time
    double err;    // for calculating error on each stage
    vector<double> errorValues; // storing error values


    /* The idea here is the following
       1 - Make a prediction using the sigmoid and current independent variable
       2 - Calculate new coefficient based on the error in the prediction
       3 - Train the algorithm for 10 epochs
    */

    for(int i = 0; i < 10; i++){  // 10 is the number of epochs
        for(int j = 0; j <= 800; j++){  // There are about 1000 observations, 800 for train, the rest for test
            int index = j;
            double p = -(b0 + b1 * sex[index]);
            double prediction  = 1/(1+ pow(e,p));
            err = survived[index]-prediction;
            b0 = b0 - lr * err*prediction *(1-prediction)* 1.0;
            b1 = b1 + lr * err * prediction*(1-prediction) * sex[index];
            errorValues.push_back(err);
        }
    }
    sort(errorValues.begin(),errorValues.end(),custom_sort); //custom sort based on absolute error difference
    cout<<"Coefficients for Logistic Regression are:\n" << "B0 (Target value when x is 0): "<<b0<< "\n"<<"B1 (expected change per unit x): "<<b1<<"\n"<<"Error: "<<errorValues[0] << endl;


    double truePositive = 0;
    double trueNegative = 0;
    double falsePositive = 0;
    double falseNegative = 0;

    for(int i = 801; i < sex.size(); i++){
        int currSex = sex[i];
        double prediction =b0+b1*currSex; // Predicting
        if(prediction>0.5)
            prediction=1;
        else {
            prediction = 0;
        }
        if(prediction == survived[i] && prediction == 1) truePositive++;
        if(prediction == survived[i] && prediction == 0) trueNegative++;
        if(prediction != survived[i] && prediction == 1) falseNegative++;
        if(prediction != survived[i] && prediction == 0) falsePositive++;
    }

    // Calculating Accuracy, specificity, and sensitivity
    double accuracy = (truePositive + trueNegative) / (truePositive + trueNegative + falsePositive + falseNegative);
    double sensitivity = truePositive / (truePositive + falseNegative);
    double specificity = trueNegative / + (trueNegative + falsePositive);

    cout << "Accuracy of the model is: " << accuracy << endl;
    cout << "Sensitivity of the model is: " << sensitivity << endl;
    cout << "Specificity of the model is: " << specificity << endl;

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Duration of runtime for Logistic Regression was: " << duration.count() << " ms" << endl << endl;



}

void naiveBayes(){
    auto start = high_resolution_clock::now();

    double survivedCount = 0,perishedCount = 0;
    double pClass1CountSurvived = 0, pClass1CountPerished = 0;
    double pClass2CountSurvived = 0, pClass2CountPerished = 0;
    double pClass3CountSurvived = 0, pClass3CountPerished = 0;
    double maleCountSurvived = 0, maleCountPerished = 0;
    double femaleCountSurvived = 0, femaleCountPerished = 0;
    double childrenCountSurvived = 0, childrenCountPerished;
    double adultCountSurvived = 0, adultCountPerished = 0;
    double elderlyCountSurvived = 0, elderlyCountPerished = 0;
    double probabilitySurvived;
    double probabilityPerished;
    double probabilityMaleSurvived, probabilityMalePerished;
    double probabilityFemaleSurvived, probabilityFemalePerished;
    double probabilityPClass1Survived, probabilityPClass1Perished;
    double probabilityPClass2Survived, probabilityPClass2Perished;
    double probabilityPClass3Survived, probabilityPClass3Perished;
    double probabilityChildSurvived, probabilityChildPerished;
    double probabilityAdultSurvived, probabilityAdultPerished;
    double probabilityElderlySurvived, probabilityElderlyPerished;

    // Training the model and calculating probabilities of every attribute given it survived and given it perished
    for(int i = 0; i <= 800; i++){
        int currPClass = pClass[i];
        int currSurvived = survived[i];
        int currSex = sex[i];
        int currAge = age[i];

        if(currSurvived == 1) {
            if (currPClass == 1) pClass1CountSurvived++;
            else if (currPClass == 2) pClass2CountSurvived++;
            else if (currPClass == 3) pClass3CountSurvived++;
            else cout << "Unknown pClass" << endl;

            survivedCount++;

            if (currSex == 1) maleCountSurvived++;
            else femaleCountSurvived++;

            if (currAge < 18) childrenCountSurvived++;
            else if (currAge >= 18 && currAge < 55) adultCountSurvived++;
            else elderlyCountSurvived++;


        }
        else{
            if (currPClass == 1) pClass1CountPerished++;
            else if (currPClass == 2) pClass2CountPerished++;
            else if (currPClass == 3) pClass3CountPerished++;
            else cout << "Unknown pClass" << endl;

            perishedCount++;

            if (currSex == 1) maleCountPerished++;
            else femaleCountPerished++;

            if (currAge < 18) childrenCountPerished++;
            else if (currAge >= 18 && currAge < 55) adultCountPerished++;
            else elderlyCountPerished++;
        }
    }

     probabilitySurvived = survivedCount / 801;
     probabilityPerished = perishedCount / 801;
     probabilityMaleSurvived = maleCountSurvived / 801;
     probabilityFemaleSurvived = femaleCountSurvived / 801;
     probabilityMalePerished = maleCountPerished/ 801;
     probabilityFemalePerished = femaleCountPerished / 801;

     probabilityPClass1Survived = pClass1CountSurvived / 801;
     probabilityPClass2Survived = pClass2CountSurvived / 801;
     probabilityPClass3Survived = pClass3CountSurvived / 801;
     probabilityPClass1Perished = pClass1CountPerished / 801;
     probabilityPClass2Perished = pClass2CountPerished / 801;
     probabilityPClass3Perished = pClass3CountPerished / 801;
     probabilityChildSurvived = childrenCountSurvived / 801;
     probabilityAdultSurvived = adultCountSurvived / 801;
     probabilityElderlySurvived = elderlyCountSurvived / 801;
     probabilityChildPerished = childrenCountPerished / 801;
     probabilityAdultPerished = adultCountPerished / 801;
     probabilityElderlyPerished = elderlyCountPerished / 801;

    double truePositive = 0;
    double trueNegative = 0;
    double falsePositive = 0;
    double falseNegative = 0;

    // Testing the Model
    for(int i = 801; i < age.size(); i++){
        int currPClass = pClass[i];
        int currSex = sex[i];
        int currAge = age[i];

        double probSexSurvived = currSex == 0? probabilityFemaleSurvived: probabilityMaleSurvived;
        double probSexPerished = currSex == 0? probabilityFemalePerished: probabilityMalePerished;

        double probAgeSurvived;
        double probAgePerished;

        double probPClassSurvived;
        double probPClassPerished;


        if(currAge < 18) {
            probAgeSurvived = probabilityChildSurvived;
            probAgePerished = probabilityChildPerished;
        }
        else if(currAge >= 55) {
            probAgeSurvived = probabilityElderlySurvived;
            probAgePerished = probabilityElderlyPerished;
        }
        else {
            probAgeSurvived = probabilityAdultSurvived;
            probAgePerished = probabilityAdultPerished;
        }

        if(currPClass == 1) {
            probPClassSurvived = probabilityPClass1Survived;
            probPClassPerished = probabilityPClass1Perished;

        }
        else if(currPClass == 2){
            probPClassSurvived = probabilityPClass2Survived;
            probPClassPerished = probabilityPClass2Perished;
        }
        else {
            probPClassSurvived = probabilityPClass3Survived;
            probPClassPerished = probabilityPClass3Perished;
        }

        double probSurvived = probAgeSurvived * probSexSurvived * probPClassSurvived ;
        double probPerished = probAgePerished * probSexPerished * probPClassPerished ;
        int prediction = probSurvived >= probPerished? 1: 0;


        if(prediction == survived[i] && prediction == 1) truePositive++;
        if(prediction == survived[i] && prediction == 0) trueNegative++;
        if(prediction != survived[i] && prediction == 1) falseNegative++;
        if(prediction != survived[i] && prediction == 0) falsePositive++;

    }


    // Calculating Accuracy, specificity, and sensitivity
    double accuracy = (truePositive + trueNegative) / (truePositive + trueNegative + falsePositive + falseNegative);
    double sensitivity = truePositive / (truePositive + falseNegative);
    double specificity = trueNegative / + (trueNegative + falsePositive);

    cout << "The Naive Bayes Algorithm produced the following: " << endl;
    cout << "Accuracy of the model is: " << accuracy << endl;
    cout << "Sensitivity of the model is: " << sensitivity << endl;
    cout << "Specificity of the model is: " << specificity << endl;

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Duration of runtime for Naive Bayes was: " << duration.count() << " ms" << endl;


}


int readCSV(){
    ifstream inFS;
    string line;
    string id_in,pClass_in, survived_in, sex_in, age_in;
    cout << "Opening File titanic_project.csv" << endl;
    inFS.open("titanic_project.csv");
    if (!inFS.is_open()) {
        cout << "Could not open file titanic csv" <<endl;
        return 1;
    }

    cout << "Reading line 1" << endl;
    getline(inFS, line);

    cout << "heading: " << line << endl << endl;

    int numObservations = 0;
    while (inFS.good()){
        getline(inFS, id_in, ',');
        getline(inFS, pClass_in, ',');
        getline(inFS, survived_in, ',');
        getline(inFS, sex_in, ',');
        getline(inFS, age_in, '\n');

        pClass.at(numObservations) = stof(pClass_in);
        survived.at(numObservations) = stof(survived_in);
        sex.at(numObservations) = stof(sex_in);
        age.at(numObservations) = stof(age_in);

        numObservations++;
    }
    id.resize(numObservations);
    pClass.resize(numObservations);
    survived.resize(numObservations);
    sex.resize(numObservations);
    age.resize(numObservations);


    cout << "Closing file titanic_project.csv\n" << endl;
    inFS.close();
}

bool custom_sort(double a, double b) /* this custom sort function is defined to
                                     sort on basis of min absolute value or error*/
{
    double  a1=abs(a-0);
    double  b1=abs(b-0);
    return a1<b1;
}
