#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
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

stData FillClientData() {
    stData Data;
    cout << "Enter Your Account Number : ";
    getline(cin >> ws, Data.AccountNumber);
    Data.PINCode = ReadString("Enter Your PIN Code : ");
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

void AddNewClient() {
    stData ClientData;
    ClientData = FillClientData();
    ClientToFile(FileName, ConvertRecordToLine(ClientData));
}
void AddClients () {
    char Answer = 'Y';
    do {
        system ("cls");
        cout << "Adding new clients : \n\n";
        
        AddNewClient();

        cout << "Added succefully!!\nDo You Want to Add More Clients ? [y/n]  \n";
        cin >> Answer;
    } while (toupper(Answer) == 'Y');
    cout << "\n All clients were added to the file [" << FileName << "]";
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

void PrintRecord (stData Data) {
    cout << "Account Numer : " << Data.AccountNumber << endl;
    cout << "PIN Code : " << Data.PINCode << endl;
    cout << "Name : " << Data.Name << endl;
    cout << "Phone : " << Data.Phone << endl;
    cout << "Account Balance : " << Data.AccountBalance << endl;
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

bool DeleteClientByID(vector<stData> &Clients, string ID) {
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

bool UpdateClientByID(vector<stData> &Clients, string ID) {
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
void VersatileFunc(int n) {
    string ID;
    vector<stData> Vector = LoadFileToVector(FileName);
    switch (n) {
        case Delete: {
            cout << "What ID do you want to delete? \n";
            cin >> ID;
            DeleteClientByID(Vector, ID);
            break;
        }
        case Update: {
            cout << "What ID do you want to update? \n";
            cin >> ID;
            UpdateClientByID(Vector, ID);
            break;
        }
        case Find: {
            cout << "Enter the ID you want to find";
            cin >> ID;
            stData Client;
            if (SearchClient(ID, Vector, Client))
            {
            PrintRecord(Client);
            }
    else {
        cout << "This ID wasn't found\n";
    }
        }
        default: cout << "Miss!Miss!";
    }
}




void Functions() {
    int Ans;
    cin >> Ans;
    
    switch(Ans) {
        case Show: {
            PrintClients(LoadFileToVector(FileName));
            cout << "\nPress Enter To Continue...\n";
            system("pause>0");
            break;
        }
        case Add: {
            AddClients();
            cout << "\nPress Enter To Continue...\n";
            system("pause>0");
            break;
        }
        case Delete: {
            VersatileFunc(Delete);
            cout << "\nPress Enter To Continue...\n";
            system("pause>0");
            break;
        }
        case Update: {
            VersatileFunc(Update);
            cout << "\nPress Enter To Continue...\n";
            system("pause>0");
            break;
        }
        case Find: {
            VersatileFunc(Find);
            cout << "\nPress Enter To Continue...\n";
            system("pause>0");
            break;
        }
        case Exit: {

            break;
        }
        default: {

        }
    }
}
void StartBank() {

    PrintMainMenu();
    Functions();

}

int main() {
    StartBank();
}