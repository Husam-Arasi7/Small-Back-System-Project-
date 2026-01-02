#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cctype>
#include <fstream>

using namespace std;

const string ClientsFileName = "Clients.txt";

enum enATMOptions { enQuickWithdraw = 1, enNormalWithdraw = 2, enDeposit = 3, enCheckBalance = 4, enLogout = 5 };

struct stClient {
    string AccountNumber;
    string PinCode;
    string Name;
    string PhoneNumber;
    double Balance;
    bool MarkForDelete = false;
};

stClient CurrentClient;

void ShowQuickWithdraw();
void PerformQuickWithdraw(stClient& Client);
void ShowATMMainMenue();
void GoBackTOMainMenue();
void Login();

// ---------------- Split String ----------------
vector<string> spiltString(string S1, string delimter) {
    vector<string> vClients;
    short pos = 0;
    string StoringWords;

    while ((pos = S1.find(delimter)) != string::npos) {
        StoringWords = S1.substr(0, pos);
        if (StoringWords != "")
            vClients.push_back(StoringWords);
        S1.erase(0, pos + delimter.length());
    }

    if (S1 != "")
        vClients.push_back(S1);

    return vClients;
}

// ---------------- Convert Line <-> Record ----------------
stClient CovertLineToRecored(string S1, string delimter = "#//#") {
    vector<string> vClients = spiltString(S1, delimter);
    stClient Client;
    Client.AccountNumber = vClients[0];
    Client.PinCode = vClients[1];
    Client.Name = vClients[2];
    Client.PhoneNumber = vClients[3];
    Client.Balance = stod(vClients[4]);
    return Client;
}

string CovertRecordToLine(stClient Client, string delimter = "#//#") {
    string Join = "";
    Join += Client.AccountNumber + delimter;
    Join += Client.PinCode + delimter;
    Join += Client.Name + delimter;
    Join += Client.PhoneNumber + delimter;
    Join += to_string(Client.Balance);
    return Join;
}

// ---------------- Load & Save ----------------
vector<stClient> LoadClientsDateFromFile(string FileName) {
    fstream NewFile;
    vector<stClient> vClients;

    NewFile.open(FileName, ios::in);
    if (NewFile.is_open()) {
        string NewLine;
        while (getline(NewFile, NewLine)) {
            vClients.push_back(CovertLineToRecored(NewLine));
        }
        NewFile.close();
    }
    return vClients;
}

vector<stClient> SaveClientDataToFile(string FileName, vector<stClient> vClient) {
    fstream NewFile(FileName, ios::out);
    if (NewFile.is_open()) {
        for (stClient& c : vClient) {
            if (!c.MarkForDelete) {
                NewFile << CovertRecordToLine(c) << endl;
            }
        }
        NewFile.close();
    }
    return vClient;
}

// ---------------- Quick Withdraw ----------------
void ShowQuickWithdraw() {
    system("cls");
    cout << "\n================= Quick Withdraw =================" << endl;
    cout << "[1] 20   " << "  [2] 50 " << endl;
    cout << "[3] 100  " << "  [4] 200 " << endl;
    cout << "[5] 400  " << "  [6] 600 " << endl;
    cout << "[7] 800  " << "  [8] 1000 " << endl;
    cout << "[9] Exit " << endl;
    cout << "==================================================" << endl;

    cout << "Your Balance is : " << CurrentClient.Balance << endl;
    cout << "Choose what to withdraw [1] to [9] ? ";
    PerformQuickWithdraw(CurrentClient);
}

void PerformQuickWithdraw(stClient& Client) {
    char Choose = ' ';
    int Amount = 0;

    cin >> Choose;

    switch (Choose) {
    case '1': Amount = 20; break;
    case '2': Amount = 50; break;
    case '3': Amount = 100; break;
    case '4': Amount = 200; break;
    case '5': Amount = 400; break;
    case '6': Amount = 600; break;
    case '7': Amount = 800; break;
    case '8': Amount = 1000; break;
    case '9': return;

    default:
        cout << "Invalid choice\n";
        return;
    }

    if (Amount > Client.Balance) {
        cout << "Insufficient balance\n";
        return;
    }

    Client.Balance -= Amount;
    cout << "Done Successflly , You have withdrawn : " << Amount << endl;
    cout << "\nNew Balance: " << Client.Balance << endl;

    vector<stClient> vClient = LoadClientsDateFromFile(ClientsFileName);
    for (stClient& c : vClient) {
        if (c.AccountNumber == Client.AccountNumber)
            c.Balance = Client.Balance;
    }
    SaveClientDataToFile(ClientsFileName, vClient);
}

// ---------------- Deposit ----------------
void DepositAmount(stClient& Client) {
    double amount = 0;
    char confirm = 'n';

    vector<stClient> vClient = LoadClientsDateFromFile(ClientsFileName);

    do {
        cout << "Enter deposit amount: ";
        cin >> amount;
        if (amount <= 0)
            cout << "Invalid amount. Try again.\n";
    } while (amount <= 0);

    cout << "Are you sure you want to comfire this transaction ? (Y/N): ";
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        Client.Balance += amount;
        cout << "\nDeposit successful.\n";
        cout << "New balance: " << Client.Balance << endl;
    }
    else {
        cout << "\nTransaction cancelled.\n";
    }

    for (stClient& c : vClient) {
        if (c.AccountNumber == Client.AccountNumber)
            c.Balance = Client.Balance;
    }
    SaveClientDataToFile(ClientsFileName, vClient);
}

void ShowDepositScreen() {
    system("cls");
    cout << "-------------------------------------------\n";
    cout << "\t\tDeposit Screen\n";
    cout << "-------------------------------------------\n";

    DepositAmount(CurrentClient);
}

// ---------------- Normal Withdraw ----------------
void CheckMultipleOf5sNormalWithdraw(stClient& Client) {
    int amount = 0;
    char confirm = ' ';
    vector<stClient> vClient = LoadClientsDateFromFile(ClientsFileName);

    do {
        cout << "Enter withdraw amount: ";
        cin >> amount;
        if (amount <= 0)
            cout << "Invalid amount.\n";
        else if (amount % 5 != 0)
            cout << "\nAmount must be multiple of 5.\n";
        else if (amount > Client.Balance)
            cout << "\nExceeds balance. Max: " << Client.Balance << endl;
    } while (amount <= 0 || amount % 5 != 0 || amount > Client.Balance);

    cout << "\n\nAre you sure you want to confirm this transaction ? (Y/N): ";
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        Client.Balance -= amount;
        cout << "\nTransaction successful.\n";
        cout << "Remaining balance: " << Client.Balance << endl;
    }
    else
        cout << "Transaction cancelled.\n";

    for (stClient& c : vClient) {
        if (c.AccountNumber == Client.AccountNumber)
            c.Balance = Client.Balance;
    }
    SaveClientDataToFile(ClientsFileName, vClient);
}

void ShowNormalWithdrawScreen() {
    system("cls");
    cout << "-------------------------------------------\n";
    cout << "\t\tWithdraw Screen\n";
    cout << "-------------------------------------------\n";
    CheckMultipleOf5sNormalWithdraw(CurrentClient);
}

// ---------------- Check Balance ----------------
void ShowCheckBalance() {
    system("cls");
    cout << "-------------------------------------------\n";
    cout << "\nCheck Balance\n";
    cout << "-------------------------------------------\n";
    cout << "\nYour Balance is: " << CurrentClient.Balance << endl;
}

// ---------------- ATM Menu ----------------
void PerformATMMainMenu(enATMOptions ATMOption) {
    switch (ATMOption) {
    case enQuickWithdraw:
        ShowQuickWithdraw();
        GoBackTOMainMenue();
        break;
    case enNormalWithdraw:
        ShowNormalWithdrawScreen();
        GoBackTOMainMenue();
        break;
    case enDeposit:
        ShowDepositScreen();
        GoBackTOMainMenue();
        break;
    case enCheckBalance:
        ShowCheckBalance();
        GoBackTOMainMenue();
        break;
    case enLogout:
        Login();
        break;
    }
}

void ShowATMMainMenue() {
    system("cls");
    cout << "\n==================== ATM Main Menu ====================" << endl;
    cout << "1. Quick Withdraw" << endl;
    cout << "2. Normal Withdraw" << endl;
    cout << "3. Deposit" << endl;
    cout << "4. Check Balance" << endl;
    cout << "5. Logout" << endl;
    cout << "======================================================" << endl;

    int choice;
    cin >> choice;
    PerformATMMainMenu((enATMOptions)choice);
}

void GoBackTOMainMenue() {
    cout << "\nPress any key to go back to Main Menu.. ";
    system("pause>0");
    ShowATMMainMenue();
}

// ---------------- Login ----------------
bool FindUserByUserNameAndPassword(string AccoutNumber, string PinCode, stClient& Client) {
    vector<stClient> vClient = LoadClientsDateFromFile(ClientsFileName);

    for (stClient& v : vClient) {
        if (v.AccountNumber == AccoutNumber && v.PinCode == PinCode) {
            Client = v;
            return true;
        }
    }
    return false;
}

bool LoadUserInfo(string UserName, string Password) {
    return FindUserByUserNameAndPassword(UserName, Password, CurrentClient);
}

void Login() {
    bool LoginFailed = false;
    string UserName, Password;

    do {
        system("cls");
        cout << "-------------------------------------------\n";
        cout << "\t\tLogin Screen \n";
        cout << "-------------------------------------------\n";

        if (LoginFailed)
            cout << "Invalid Account Number/PinCode!\n";

        cout << "Enter Account Number: ";
        cin >> UserName;
        cout << "Enter PinCode: ";
        cin >> Password;

        LoginFailed = !LoadUserInfo(UserName, Password);
    } while (LoginFailed);

    ShowATMMainMenue();
}


int main() {
    Login();
    return 0;
}
