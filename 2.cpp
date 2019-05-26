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
void replaceData(std::string **, std::string *, std::string *, int, int);
int partition(std::string *, std::string *, int, int);
void quickSort(std::string *, std::string *, int, int);
void swap(std::string *, std::string *);
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
    quickSort(empNo, empName, 0, rows - 1);
    replaceData(dataHolder, empName, empNo, rows, cols);
    //calling .csv file creator function
    createCSV(dataHolder, rows, cols);

    return 0;
}
void swap(std::string *a, std::string *b)
{
    std::string temp = *a;
    *a = *b;
    *b = temp;
}
int partition(std::string *arr, std::string *arr_1, int low, int high)
{
    int pivot = atoi(arr[high].c_str()); // pivot
    int i = (low - 1);                   // Index of smaller element

    for (int j = low; j <= high - 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (atoi(arr[j].c_str()) <= pivot)
        {
            i++; // increment index of smaller element
            swap(&arr[i], &arr[j]);
            swap(&arr_1[i], &arr_1[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    swap(&arr_1[i + 1], &arr_1[high]);
    return (i + 1);
}

/* The main function that implements QuickSort 
 arr[] --> Array to be sorted, 
  low  --> Starting index, 
  high  --> Ending index */
void quickSort(std::string *arr, std::string *arr_1, int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now 
           at right place */
        int pi = partition(arr, arr_1, low, high);

        // Separately sort elements before
        // partition and after partition
        quickSort(arr, arr_1, low, pi - 1);
        quickSort(arr, arr_1, pi + 1, high);
    }
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
    std::ofstream fptr("HRDataset_sortbyempno.csv"); //Creates a file

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