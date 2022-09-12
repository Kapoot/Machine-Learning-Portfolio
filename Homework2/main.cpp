/*
 Dylan Kapustka (dlk190000)
 CS 4375.003
 Dr. Mazidi
 September 10, 2022
 Homework 2 - Data Exploration
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

int readCSV();
const int MAX_LEN = 1000;
vector<double> rm(MAX_LEN);
vector<double> medv(MAX_LEN);
double sum(vector<double> vec);
double range(vector<double> vec);
double mean(vector<double> vec, int length);
double median(vector<double> vec, int length);
double covariance(vector<double> vec1, vector<double> vec2, int length);
double correlation(vector<double> vec1, vector<double> vec2, int length);

int main() {

    int length = readCSV();

    cout << "rm:" << endl;
    cout << "The sum of rm is: " << sum(rm) << endl;
    cout << "The mean of rm is: " << mean(rm, length) << endl;
    cout << "The median of rm is: " << median(rm, length) << endl;
    cout << "The range of rm is: " << range(rm) << endl << endl;

    cout << "medv:" << endl;
    cout << "The sum of medv is: " << sum(medv) << endl;
    cout << "The mean of medv is: " << mean(medv, length) << endl;
    cout << "The median of medv is: " << median(medv, length) << endl;
    cout << "The range of medv is: " << range(medv) << endl << endl;

    cout <<"medv and rm:" << endl;
    cout << "The covariance of medv and rm is: " << covariance(rm,medv,length) << endl;
    cout << "The correlation of medv and rm is: " << correlation(rm,medv,length) << endl;

    return 0;
}

// Read file and return data count
int readCSV(){
    ifstream inFS;
    string line;
    string rm_in, medv_in;

    cout << "Opening File Boston.csv" << endl;
    inFS.open("Boston.csv");
    if (!inFS.is_open()) {
        cout << "Could not open file Boston.csv" <<endl;
        return 0;
    }

    cout << "Reading line 1" << endl;
    getline(inFS, line);

    cout << "heading: " << line << endl;

    int numObservations = 0;
    while (inFS.good()){
        getline(inFS, rm_in, ',');
        getline(inFS, medv_in, '\n');
        rm.at(numObservations) = stof(rm_in);
        medv.at(numObservations) = stof(medv_in);

        numObservations++;
    }

    rm.resize(numObservations);
    medv.resize(numObservations);

    cout << "new length: " << rm.size() << endl;

    cout << "Closing file Boston.csv" << endl <<endl;
    inFS.close();

    return numObservations;

}

// get the sum of the vector
double sum(vector<double> vec)
{
    double total = 0;
    for(double element : vec)
    {
        total += element;
    }
    return total;
}

//get mean of vector
double mean(vector<double> vec, int length)
{
    double sum = 0;
    for (int i = 0; i < length; i++)
        sum += vec[i];
    return sum / length;
}

//get median of vector
double median(vector<double> vec, int length)
{
    vector<double> temp(vec);

    sort(temp.begin(), temp.end());

    if (length % 2 != 0)
        return (double)temp[length/2];
    return (double)(temp[(length-1)/2] + temp[length/2])/2.0;

}
// get range of vector
double range(vector<double> vec)
{
    vector<double> temp(vec);
    sort(temp.begin(), temp.end());
    return temp[temp.size()-1] - temp[0];

}

//find covariance between vectors
double covariance(vector<double> vec1, vector<double> vec2, int length)
{
    double sum = 0;
    double rmMean = mean(vec1, length);
    double medvMean = mean(vec2, length);
    for (int i = 0; i < length; i++)
        sum = sum + (vec1[i] - rmMean) * (vec2[i] - medvMean);
    return sum / (length - 1);
}

//get correlation of vectors
double correlation(vector<double> vec1, vector<double> vec2, int length)
{
    double sigmaRm = sqrt(covariance(vec1,vec1,length));
    double sigmaMedv = sqrt(covariance(vec2,vec2,length));

    return covariance(vec1,vec2,length) / (sigmaRm * sigmaMedv);
}
