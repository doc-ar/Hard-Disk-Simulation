#include <iostream>
#include <string.h>
#include <limits>

using namespace std;

// Scalable Variables
const int totalFiles = 20; // Total entries in File allocation Table
const int diskSize = 500; // Total Disk size
const int fileSize = 50; // Size limit of data in one File
const int fileNameSize = 32; // Size limit of file names
const int extensionSize = 5; // size limit of file extensions



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



// Class Definition for the actual hard drive simulation
class HddSimulation
{
    private:
        fileAllocation FAT[totalFiles];
        storage Disk[diskSize];
        int fatCount;
        int p1End;
        int p2End;
        int p1Count;
        int p2Count;

    public:
        HddSimulation(int p1End, int p2End)
        {
            this->p1End = p1End;
            this->p2End = p2End;
            fatCount = 0;
            p1Count = 0;
            p2Count = 0;
            this->format();
        }

        bool FATisFull()
        {
            if(fatCount >= totalFiles)
            {
                return true;
            }
            return false;
        }

        bool p1HasSpace()
        {
            if(p1End-p1Count > fileSize)
            {
                return true;
            }
            return false;
        }

        bool p2HasSpace()
        {
            if((p2End-p1End-p2Count) > fileSize)
            {
                return true;
            }
            return false;
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

        void createFileP1(char *name, char *ext, char *data)
        {
            for(int i=0; i<totalFiles; i++)
            {
                if(FAT[i].availability == -1)
                {
                    int sentry = 0;
                    strcpy(FAT[i].fileName, name);
                    strcpy(FAT[i].fileExt, ext);
                    FAT[i].availability = 1;

                    while(Disk[sentry].nextIndex != -3)
                    {
                        if(Disk[sentry].nextIndex == -1)
                        {
                            FAT[i].firstIndex = sentry;
                            break;
                        }
                        sentry++;
                    }

                    int previousIndex = -1;
                    for(int i=0; data[i] != '\0'; i++)
                    {
                        while(Disk[sentry].nextIndex != -3)
                        {
                            if(Disk[sentry].nextIndex == -1)
                            {
                                if(previousIndex >= 0)
                                {
                                    Disk[previousIndex].nextIndex = sentry;
                                }

                                Disk[sentry].data = data[i];
                                previousIndex = sentry;
                                Disk[sentry].nextIndex = -2;
                                break;
                            }
                            sentry++;
                        }
                    }
                    break;
                }
            }
        }

        void createFileP2(char *name, char *ext, char *data)
        {
            for(int i=0; i<totalFiles; i++)
            {
                if(FAT[i].availability == -1)
                {
                    int sentry = p1End+1;
                    strcpy(FAT[i].fileName, name);
                    strcpy(FAT[i].fileExt, ext);
                    FAT[i].availability = 1;

                    while(Disk[sentry].nextIndex != -3)
                    {
                        if(Disk[sentry].nextIndex == -1)
                        {
                            FAT[i].firstIndex = sentry;
                            break;
                        }
                        sentry++;
                    }

                    int previousIndex = -1;
                    for(int i=0; data[i] != '\0'; i++)
                    {
                        while(Disk[sentry].nextIndex != -3)
                        {
                            if(Disk[sentry].nextIndex == -1)
                            {
                                if(previousIndex >= 0)
                                {
                                    Disk[previousIndex].nextIndex = sentry;
                                }

                                Disk[sentry].data = data[i];
                                previousIndex = sentry;
                                Disk[sentry].nextIndex -2;
                                break;
                            }
                            sentry++;
                        }
                    }
                    break;
                }
            }
        }

        void ListFiles()
        {
            for(int i=0; i<totalFiles; i++)
            {
                if(FAT[i].availability == 1)
                {
                    cout << "\n\n\t File index: " << i;
                    cout << "\n\t File name: " << FAT[i].fileName << "." << FAT[i].fileExt;
                    cout << "\n\t File Data: ";

                    int sentry = FAT[i].firstIndex;
                    while(sentry != -2)
                    {
                        cout << Disk[sentry].data;
                        sentry = Disk[sentry].nextIndex;
                    }
                }
            }
        }

        bool DeleteFile(int index)
        {
            if(FAT[index].availability == -1)
            {
                return false;
            }

            int sentry = FAT[index].firstIndex;
            while(Disk[sentry].nextIndex != -2)
            {
                int temp = Disk[sentry].nextIndex;
                Disk[sentry].nextIndex = -1;
                sentry = temp;
            }
            FAT[index].availability = -1;
            return true;
        }
};



// Driver Code
int main()
{
    int opt = 0;    // Option Selection Sentry
    int p1End = 0;  // Partition 1 End
    int p2End = 0;  // Partition 2 End

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
    while(opt != 4)
    {
        cout << "\n\n\n Please select an option";
        cout << "\n 1: Create a File";
        cout << "\n 2: Delete a File";
        cout << "\n 3: Format Disk";
        cout << "\n 4: Exit Program";
        cout << "\n\n Enter Option: ";
        cin >> opt;

        if(opt == 1) // Option 1 from main menu
        {
            int opt2 = 0;
            char tempName[fileNameSize];
            char tempExt[extensionSize];
            char tempDat[fileSize];

            cout << "\n\n\n\t Please Enter File Name: ";
            cin >> tempName;
            cout << "\t Please Enter File Extension: ";
            cin >> tempExt;
            cout << "\t File Data: ";
            cin >> tempDat;

            while(opt2 != 3) 
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "\n\n\n\t\t Please Enter which partition to store file";
                cout << "\n\t\t 1: Partition 1";
                cout << "\n\t\t 2: Partition 2";
                cout << "\n\t\t 3: Cancel Operation";
                cout << "\n\t\t Option is: ";
                cin >> opt2;

                if(opt2 == 1)
                {
                    drive.createFileP1(tempName, tempExt, tempDat);
                    break;
                }
                else if(opt2 == 2)
                {
                    drive.createFileP2(tempName, tempExt, tempDat);
                    break;
                }
            } // End while

        } // End If

        else if(opt == 2)
        {
            int tempIndex = 0;
            cout << "\n\n\n\t The files are: ";
            drive.ListFiles();

            cout << "\n\n\t Enter the file index to delete: ";
            cin >> tempIndex;

            if(drive.DeleteFile(tempIndex))
            {
                cout << "\n\t File Deleted Successfully";
            }
            else
            {
                cout << "\n\t File Deleteion Failed";
            }
        } // End If

        else if(opt == 3)
        {
            char temp;
            cout << "\n\n\n\t Are your sure you want to format?";
            cout << "\n\t (Y)es or (N)o: ";
            cin >> temp;

            if(temp == 'Y' || temp == 'y')
            {
                drive.format();
            }
            else if(temp == 'N' || temp == 'n')
            {
                cout << "\n\t Operation Cancelled.";
            }
        } // End If
    } // End While
    return 0;
}