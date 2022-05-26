#include <iostream>

using namespace std;

// Scalable Variables
const int totalFiles = 20;
const int diskSize = 200;
const int fileSize = 50;
const int fileNameSize = 32;
const int extensionSize = 5;



// Necessary Structure Definitions
struct fileAllocation
{
    char fileName[fileNameSize];
    char fileExt[extensionSize];
    int firstIndex;
    int availability;
};
struct storage
{
    char data;
    int nextIndex;
};



// Class Definition for files
class File
{
    private:
        char fileName[fileNameSize];
        char fileExtension[extensionSize];
        char data[fileSize];
    
    public:
        void readName()
        {
            cin >> fileName;
        }

        void readExt()
        {
            cin >> fileExtension;
        }

        void readData()
        {
            cin >> data;
        }

        char* getName()
        {
            return fileName;
        }

        char* getExt()
        {
            return fileExtension;
        }

        char* getData()
        {
            return data;
        }
};



// Class Definition for the actual hard drive simulation
class HddSimulation
{
    private:
        fileAllocation FAT[totalFiles];
        storage Disk[diskSize];
        int p1End;
        int p2End;
        int p1Count;
        int p2Count;

    public:
        HddSimulation(int p1End, int p2End)
        {
            this->p1End = p1End;
            this->p2End = p2End;
            p1Count = 0;
            p2Count = 0;
            this->format();
        }

        void format()
        {
            for(int i=0; i<totalFiles; i++)
            {
                FAT[i].availability = -1;
            }

            for(int i=0; i<diskSize; i++)
            {
                Disk[i].nextIndex = -1;
            }

            // Partition End Codes
            Disk[p1End].nextIndex = -3;
            Disk[p2End].nextIndex = -3;
            Disk[diskSize-1].nextIndex = -3;
        }
};



// Driver Code
int main()
{
    int opt = 0;    // Option Selection Sentry
    int p1End = 0;  // Partition 1 End
    int p2End = 0;  // Partition 2 End
    File tp;

    cout << " --------------------------------" << endl;
    cout << " Welcome to Hard Drive Simulation" << endl;
    cout << " --------------------------------" << endl;

    while(true)
    {
        cout << "\n\n Please Enter End of Partition 1 (0-" << diskSize-1 << "): ";
        cin >> p1End;

        if(p1End > 0 && p1End < diskSize)
        {
            cout << " Please Enter End of Partition 2 (" << p1End+1 << "-" << diskSize-1 << "): ";
            cin >> p2End;

            if(p2End > p1End && p2End < diskSize)
            {
                break;
            }
            else
            {
                cout << " Invalid Partition 2 Size, Try again.";
            }
        }
        else
        {
            cout << " Invalid Partition 1 Size, Try again.";
        }
    }

    HddSimulation drive(p1End, p2End);
    while(opt != 6)
    {
        cout << "\n\n\n Please select an option";
        cout << "\n 1: Create a File";
        cout << "\n 2: Read a File";
        cout << "\n 3: Delete a File";
        cout << "\n 4: Format Disk";
        cout << "\n 5: Defragment Disk";
        cout << "\n 6: Exit Program";
        cout << "\n\n Enter Option: ";
        cin >> opt;

        if(opt == 1)
        {
            File temp;
            int opt2 = 0;

            cout << "\n\n\n\t Please Enter File Name: ";
            temp.getName();
            cout << "\t Please Enter File Extension: ";
            temp.getExt();
            cout << "\t File Data: ";
            temp.getData();

            while(opt!=3)
            {
                cout << "\n\n\t\t Please Enter which partition to store file";
                cout << "\n\t\t 1: Drive C";
                cout << "\n\t\t 2: Drive D";
                cout << "\n\t\t 3: Cancel Operation";
            }
        }
    }
    return 0;
}