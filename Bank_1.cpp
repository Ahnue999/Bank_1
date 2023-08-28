#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string.h>
using namespace std;

const string FileName = "Client.txt";

enum Func { Show = 1, Add, Delete, Update, Find, Exit};

struct stData {
    string AccountNumber;
    string PINCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool ToDelete = false;
};

vector<string> Split(string TheString, string Delim = " ")
{
    vector<string> vWords;
    int count = 0;
    int pos = 0;
    string Word;
    while ((pos = TheString.find(Delim)) != std::string::npos) {
        Word = TheString.substr(0, pos);
        if (Word != " ") {
            vWords.push_back(Word);
        }
        TheString.erase(0, (pos + Delim.length()));
    }
    if (TheString != " ") {
        vWords.push_back(TheString);
    }
    return vWords;
}


stData LineToRecord(string Line, string Delim = "#//#") {
    vector<string> vData = Split(Line, "#//#");
    stData Data;
    Data.AccountNumber = vData[0];
    Data.PINCode = vData[1];
    Data.Name = vData[2];
    Data.Phone = vData[3];
    Data.AccountBalance = stod(vData[4]);
    return Data;
}

vector<stData> LoadFileToVector (string FileName) {
    fstream ClientsFile;
    vector<stData> vClientsData;

    ClientsFile.open(FileName, ios::in);
    if (ClientsFile.is_open()) {
        string Line;
        stData Client;

        while (getline(ClientsFile, Line)) {
            Client = LineToRecord(Line);
            vClientsData.push_back(Client);
        }

        ClientsFile.close();

    }
    return vClientsData;
}


void PrintMainMenu() {
    system ("cls");
    cout << "========================================================================\n";
    cout << "\t\t\t   Main Menu Screen \n";
    cout << "========================================================================\n";
    cout << "\t [1] Show Clients List.\n";
    cout << "\t [2] Add New Client.\n";
    cout << "\t [3] Delete Client.\n";
    cout << "\t [4] Update Client.\n";
    cout << "\t [5] Find Client.\n";
    cout << "\t [6] Exit.\n";
    cout << "========================================================================\n";
    cout << "Choose what do you want to do? [1 to 6] ?\n";
}

void PrintClients(vector<stData> Data) {
    system ("cls");
    cout << "                          Client list (" << Data.size() << ") Client(s): " << endl;
    cout << "________________________________________________________________________________________________\n";
    cout << "|" << setw(20) << left << "Account Number" << "|" << setw(12) << "Pin Code" << "|" << setw(34) << "Client Name" << "|" << setw(18) << "Phone Number" << "|" << setw(12) << "Balance" << endl;
    cout << "________________________________________________________________________________________________\n";

    for (stData &Record : Data) {
        cout <<  "|" << setw(20) << Record.AccountNumber <<  "|" << setw(12) << Record.PINCode << "|" << setw(34) << Record.Name << "|" << setw(18) << Record.Phone << "|" << setw(12) << Record.AccountBalance << endl;
    }
}

string ReadString(string Massage)
{
    string TheString;
    cout << Massage;
    getline(cin, TheString);
    return TheString;
}

stData FillClientData(string ID) {
    stData Data;
    Data.AccountNumber = ID;
    cout << "Enter Your PIN Code : ";
    getline(cin >> ws, Data.PINCode);
    Data.Name = ReadString("Enter Your Name : ");
    Data.Phone = ReadString("Enter Your Phone Number: ");
    Data.AccountBalance = stod(ReadString("Enter Your Account Balance : "));
    return Data;
}

string ConvertRecordToLine(stData Data, string Seperator = "#//#") {
    string Line = "";
    
    Line += Data.AccountNumber + Seperator; 
    Line += Data.PINCode + Seperator;
    Line += Data.Name + Seperator;
    Line += Data.Phone + Seperator;
    Line += to_string(Data.AccountBalance);

    return Line;
}

void ClientToFile(string FileName, string Line) {
    
    fstream ClientFile;

    ClientFile.open(FileName, ios::out | ios::app );
    if (ClientFile.is_open()) {
        ClientFile << Line << endl;
        ClientFile.close();
    }
    
}

bool SearchClient(string ID, vector<stData> Clients, stData& Client) {
    for (stData &C : Clients) {
        if (C.AccountNumber == ID) {
            Client = C;
            return true;
        }
    } 
    return false;
}

void AddNewClient(string ID) {
    stData ClientData;
    ClientData = FillClientData(ID);
    ClientToFile(FileName, ConvertRecordToLine(ClientData));
}
void AddClients (vector<stData> Clients) {
    stData Client;
    char Answer = 'Y';
    string ID = "";
    do {
        cout << "enter the ID you want to add";
        cin >> ID;
        system ("cls");
        if (!SearchClient(ID, Clients, Client)) {
        cout << "Adding new clients : \n\n";
        
        AddNewClient(ID);

        cout << "Added succefully!!\n";
        }
        else cout << "This ID is taken\n";
        cout << "\nDo You Want to Add More Clients ? [y/n]  \n";
        cin >> Answer;
    } while (toupper(Answer) == 'Y');
    cout << "\n All clients were added to the file [" << FileName << "]";
}


void PrintRecord (stData Data) {
    cout << "\n-----------------------------------------\n";
    cout << "Account Numer : " << Data.AccountNumber << endl;
    cout << "PIN Code : " << Data.PINCode << endl;
    cout << "Name : " << Data.Name << endl;
    cout << "Phone : " << Data.Phone << endl;
    cout << "Account Balance : " << Data.AccountBalance << endl;
    cout << "\n-----------------------------------------\n";
}

void MarkClientForDeletion(string ID, vector<stData> &Clients) {
    for (stData &C : Clients) {
        if (C.AccountNumber == ID) {
            C.ToDelete = true;
        }
    }
}

void LoadVectorToFile(string FileName, vector<stData> Clients) {
    fstream MyFile;

    MyFile.open(FileName, ios::out);
    
    if (MyFile.is_open()) {
        for (stData &C : Clients) {
            if (!C.ToDelete) {
                MyFile << ConvertRecordToLine(C) << endl;
            }
        } 
    }
}

bool DeleteClientByID(vector<stData> &Clients) {
    string ID;
    cout << "What ID do you want to delete? \n";
    cin >> ID;
    char Answer = 'N';
    stData Client;

    if (SearchClient(ID, Clients, Client)) {
        PrintRecord(Client);

        cout << "Are you sure you want to delete this record\n";
        cin >> Answer;
        
        if (toupper(Answer) == 'Y') {
            MarkClientForDeletion(ID, Clients);
            LoadVectorToFile(FileName, Clients);

            cout << "Client was deleted successfully!\n";
            Clients = LoadFileToVector(FileName);
        }
    }
    else {
        cout << "Client was not found!\n";
    }
}

stData ChangeClientData(stData &Data) {
    cout << ("Enter Your PIN Code : ");
    getline(cin >> ws, Data.PINCode);
    Data.Name = ReadString("Enter Your Name : ");
    Data.Phone = ReadString("Enter Your Phone Number: ");
    Data.AccountBalance = stod(ReadString("Enter Your Account Balance : "));
    return Data;
}

bool UpdateClientByID(vector<stData> &Clients) {
    string ID;
    cout << "What ID do you want to update? \n";
    cin >> ID;
    char Answer = 'N';
    stData Client;

    if (SearchClient(ID, Clients, Client)) {
        PrintRecord(Client);

        cout << "Are you sure you want to update this record\n";
        cin >> Answer;
        
        if (toupper(Answer) == 'Y') {
            for (stData &C : Clients)
            {
                if (C.AccountNumber == ID)
                {
                    C = ChangeClientData(C);
                    break;
                }
            }
            LoadVectorToFile(FileName, Clients);

            cout << "Client was Updated successfully!\n";
            Clients = LoadFileToVector(FileName);
        }
    }
    else {
        cout << "Client was not found!\n";
    }
}

void PressToContinue() {
            cout << "\nPress Enter To Continue...\n";
            system("pause>0");
}

void FindClient(vector<stData> Clients) {
    string ID;
    cout << "Enter the ID you want to find? \n";
    cin >> ID;
    stData Client;
    if (SearchClient(ID, Clients, Client)) {
    PrintRecord(Client);
    }
    else {
        cout << "This ID wasn't found\n";
    }
}

void Functions(int &still) {
    int Ans;
    cin >> Ans;
    string ID;
    vector<stData> Clients = LoadFileToVector(FileName);
    
    switch(Ans) {
        case Show: {
            PrintClients(LoadFileToVector(FileName));
            PressToContinue();
            break;
        }
        case Add: {
            AddClients(Clients);
            PressToContinue();
            break;
        }
        case Delete: {
            DeleteClientByID(Clients);
            PressToContinue();
            break;
        }
        case Update: {
            UpdateClientByID(Clients);
            PressToContinue();
            break;
        }
        case Find: {
            FindClient(Clients);
            PressToContinue();
            break;
        }
        case Exit: {
            still = Exit;
            cout << "\n HAVE A NICE DAY~! \n";
            cout << "\n\n Exitting ... \n\n";
            break;
        }
        default: {

        }
    }
}
void StartBank() {

    int still;
    do {
        PrintMainMenu();
        Functions(still);
    }
    while (still != Exit);
}

int main() {
    StartBank();
}