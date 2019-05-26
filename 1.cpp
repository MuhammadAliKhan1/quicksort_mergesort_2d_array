//Included libraries
#include <iostream> //I/O stream
#include <string>   //For using string functions
#include <fstream>  //For using file functions
#include <vector>
//Function Declaration
std::string **createMatrix(int, int);
std::string **readFile(std::string, std::string **, int, int);
void createCSV(std::string **, int, int);
void splitVec(std::string *, std::string *, std::string **, int);
void merge(std::string *, std::string *, int, int, int);
void mergeSort(std::string *, std::string *, int, int);
void replaceData(std::string **, std::string *, std::string *, int, int);
//Main Function
int main()
{
    //Variable Declaration
    int rows{0}, cols{0};
    std::string fileName;
    std::cout << "Input number of rows in the .csv file: ";
    std::cin >> rows;
    std::cout << "Input number of cols in the .csv file: ";
    std::cin >> cols;
    //Calling createMatrix
    std::string **dataHolder = createMatrix(rows, cols);
    std::string *empName = new std::string[rows];
    std::string *empNo = new std::string[rows];
    std::cin.ignore(); //To consume new line character
    std::cout << "Input .csv file name(Without extension): ";
    getline(std::cin, fileName);
    fileName += ".csv";
    //calling readFile
    dataHolder = readFile(fileName, dataHolder, rows, cols);
    splitVec(empName, empNo, dataHolder, rows);
    mergeSort(empName, empNo, 0, rows - 1);
    replaceData(dataHolder, empName, empNo, rows, cols);
    //calling .csv file creator function
    createCSV(dataHolder, rows, cols);

    return 0;
}
void replaceData(std::string **dataHolder, std::string *empName, std::string *empNo, int rows, int cols)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if (j == 0)
                dataHolder[i][j] = empName[i];
            else
                dataHolder[i][j] = empNo[i];
        }
    }
}
void merge(std::string *arr, std::string *arr_1, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    /* create temp arrays */
    std::string L[n1];
    std::string R[n2];
    std::string L_1[n1];
    std::string R_1[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
    {
        L[i] = arr[l + i];
        L_1[i] = arr_1[l + i];
    }
    for (j = 0; j < n2; j++)
    {
        R[j] = arr[m + 1 + j];
        R_1[j] = arr_1[m + 1 + j];
    }

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        std::string s_1 = L[i];
        std::string s_2 = R[j];
        int n_1 = s_1[0];
        int n_2 = s_2[0];
        if (n_1 <= n_2)
        {
            arr[k] = L[i];
            arr_1[k] = L_1[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            arr_1[k] = R_1[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there 
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
        arr_1[k] = L_1[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there 
       are any */
    while (j < n2)
    {
        arr_1[k] = R_1[j];
        j++;
        k++;
    }
}

/* l is for left index and r is right index of the 
   sub-array of arr to be sorted */
void mergeSort(std::string *arr, std::string *arr_1, int l, int r)
{
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l + (r - l) / 2;

        // Sort first and second halves
        mergeSort(arr, arr_1, l, m);
        mergeSort(arr, arr_1, m + 1, r);

        merge(arr, arr_1, l, m, r);
    }
}

//Array creator
std::string **createMatrix(int rows, int cols)
{
    //Creates a matrix and returns its address
    std::string **dataHolder = new std::string *[rows];
    for (int i = 0; i < rows; ++i)
        dataHolder[i] = new std::string[cols];
    return dataHolder;
}
void splitVec(std::string *empName, std::string *empNo, std::string **dataHolder, int rows)
{

    for (int i = 0; i < rows; ++i)
    {
        empName[i] = dataHolder[i][0];
        empNo[i] = dataHolder[i][1];
    }
}
//File Reader
std::string **readFile(std::string fileName, std::string **dataHolder, int rows, int cols)
{
    //Reads a .csv file and store its data in a matrix
    std::fstream fptr;   //File type pointer creation
    fptr.open(fileName); //opening given file name
    std::string discard; //variable to get and discard a text

    if (!fptr.is_open()) //File opened or not check
        std::cout << "ERROR! FILE DID NOT OPENED.";

    for (int i = 0; i < rows && fptr.good(); ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if (i > 0)
            {
                //Reading rest of the document except first row
                if (j > 0)
                {
                    //Reading rest of the column except first one
                    if (j < (cols - 1))
                        getline(fptr, dataHolder[i][j], ',');
                    else
                        getline(fptr, dataHolder[i][j], '\n');
                }
                else
                {
                    //First column reader
                    getline(fptr, discard, '"');
                    getline(fptr, dataHolder[i][j], '"');
                    getline(fptr, discard, ',');
                }
            }
            else
            {
                //First row reader
                if (j < (cols - 1))
                    getline(fptr, discard, ',');
                else
                    getline(fptr, discard, '\n');
            }
        }
    }

    return dataHolder;
}

//Sorting Algorithm
//.csv file creator from a matrix array
void createCSV(std::string **dataHolder, int rows, int cols)
{
    std::ofstream fptr("HRDataset_sortbyname.csv"); //Creates a file

    //Logic for writing the matrix into a .csv file
    fptr << "Employee Name,Employee Number\n";
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if (i > 0)
            {
                //For writing rows except first one
                if (j > 0)
                {
                    //For writing columns except first one
                    fptr << dataHolder[i][j];
                    if (j < (cols - 1))
                        fptr << ',';
                }
                else
                {
                    //For writing first column
                    fptr << '"';
                    fptr << dataHolder[i][j];
                    fptr << '"';
                    fptr << ',';
                }
            }
        }
        fptr << '\n';
    }
}