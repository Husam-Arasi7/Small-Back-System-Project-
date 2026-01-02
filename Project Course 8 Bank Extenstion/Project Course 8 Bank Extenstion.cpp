#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cctype>
#include <fstream>

using namespace std;

const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";

enum enMainMenueOptions {
	enListCliens = 1, enAddNewClient = 2, enDeleteClient = 3, enUpdateClient = 4,
	enFindClient = 5, enShoeTransactionsMenue = 6, enMangeUsers = 7, enEixt = 8
};

enum enTransactionsMenueOptions { enDepist = 1, enWithdrow = 2, enShowTotalBalace = 3, enShowMainMenue = 4 };

enum enMangeUsersMeuneOptions { enListUser = 1, enAddNewUser = 2, enDeleteUser = 3, enUpdateUser = 4, enFindUser = 5, enMainMenue = 6 };

enum enMainMenuePermissions {
	eAll = -1, pShowClientsList = 1, pAddNewClient = 2, pDeleteClient = 4, pUpdateClients = 8, pFindClient = 16
	, pTranactions = 32, pMangeUsers = 64
};


// ---------- Forward Declarations ----------
void ShowMainMenu();
void ShowTransactionsMenu();
void ShowMangeUsers();
void GoBackTOMainMenue();
void GoBackTOTransactionsMenure();
void GoBackTOMaangeUsersMenue();
void PerfromClientsMenueOption(enMainMenueOptions);
void PerfromTransactionsMenueOption(enTransactionsMenueOptions);
void PerfromMangeUsersMenueOption(enMangeUsersMeuneOptions);
void Login();
void ShowAddNewClientsScreen();



// Clients & Users 


struct stUser {
	string UserName;
	string Password;
	int  Permissions;
	bool MarkForDele = false;

};

stUser CurrentUser;



struct stClient {

	string AccountNumber;
	string PinCode;
	string Name;
	string PhoneNumber;
	double Balance;
	bool MarkForDele = false;

};

vector <string>  spiltString(string S1, string delimter) {

	vector <string > vCLients;

	short pos = 0;
	string StoringWords;


	while ((pos = S1.find(delimter)) != std::string::npos) {

		StoringWords = S1.substr(0, pos);

		if (StoringWords != "") {
			vCLients.push_back(StoringWords);

		}

		S1 = S1.erase(0, pos + delimter.length());

	}

	if (S1 != "") {
		vCLients.push_back(S1);
	}

	return vCLients;

}


vector <string>  spiltStringByUser(string S1, string delimter) {

	vector <string > vUser;

	short pos = 0;
	string StoringWords;


	while ((pos = S1.find(delimter)) != std::string::npos) {

		StoringWords = S1.substr(0, pos);

		if (StoringWords != "") {
			vUser.push_back(StoringWords);

		}

		S1 = S1.erase(0, pos + delimter.length());

	}

	if (S1 != "") {
		vUser.push_back(S1);
	}

	return vUser;

}



stClient CovertLineToRecored(string S1, string delimter = "#//#") {

	vector <string> vClients;
	stClient Client;

	vClients = spiltString(S1, delimter);

	Client.AccountNumber = vClients[0];
	Client.PinCode = vClients[1];
	Client.Name = vClients[2];
	Client.PhoneNumber = vClients[3];
	Client.Balance = stod(vClients[4]);


	return Client;

}

stUser CovertUserLineToRecored(string S1, string delimter = "#//#") {

	vector <string> vUser;
	stUser User;

	vUser = spiltStringByUser(S1, delimter);

	User.UserName = vUser[0];
	User.Password = vUser[1];
	User.Permissions = stoi(vUser[2]);



	return User;

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


string CovertUserRecordToLine(stUser User, string delimter = "#//#") {

	string Join = "";


	Join += User.UserName + delimter;
	Join += User.Password + delimter;
	Join += to_string(User.Permissions);



	return Join;


}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName) {
	vector <stClient> vClients;

	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open()) {

		string Line;
		stClient Client;

		while (getline(MyFile, Line)) {

			Client = CovertLineToRecored(Line);
			if (Client.AccountNumber == AccountNumber) {
				MyFile.close();
				return true;

			}

			vClients.push_back(Client);
		}
		MyFile.close();

	}
	return false;
}

bool UserExistsByUserName(string UserName, string FileName) {

	vector <stUser> vUser;

	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open()) {

		string Line;
		stUser User;

		while (getline(MyFile, Line)) {

			User = CovertUserLineToRecored(Line);
			if (User.UserName == UserName) {
				MyFile.close();
				return true;

			}

			vUser.push_back(User);
		}
		MyFile.close();

	}
	return false;
}




stClient ReadNewClient() {

	stClient Client;

	cout << "\nEnter Account Number : ";
	getline(cin >> ws, Client.AccountNumber);


	while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName)) {
		cout << "\nClient wiht [ " << Client.AccountNumber << "] already exists , enter another Accounter Number" << endl;
		getline(cin >> ws, Client.AccountNumber);
	}


	cout << "\nEnter Pin Code : ";
	getline(cin, Client.PinCode);

	cout << "\nEnter Name : ";
	getline(cin, Client.Name);

	cout << "\nEnter Phone Number : ";
	getline(cin, Client.PhoneNumber);

	cout << "\nEnter Balance : ";
	cin >> Client.Balance;


	return Client;

}




int ReadPermissionsToSet() {

	int Permissions = 0;
	char Answer = 'n';

	cout << "\nDo you want to give full access ? y/n ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y') {
		return -1;
	}

	cout << "\nDo you want to give access to : \n";

	cout << "\nShow Client List ? y/n ? ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y') {
		Permissions += enMainMenuePermissions::pShowClientsList;
	}


	cout << "\nAdd New Client ? y/n ? ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y') {
		Permissions += enMainMenuePermissions::pAddNewClient;
	}


	cout << "\nDelete Client ? y/n ? ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y') {
		Permissions += enMainMenuePermissions::pDeleteClient;


	}

	cout << "\nUpDate Client? y/n ? ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y') {
		Permissions += enMainMenuePermissions::pUpdateClients;
	}

	cout << "\nFind Client ? y/n ? ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y') {
		Permissions += enMainMenuePermissions::pFindClient;
	}


	cout << "\nTransactions  ? y/n ? ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y') {
		Permissions += enMainMenuePermissions::pTranactions;
	}



	cout << "\nMange Users ? y/n ? ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y') {
		Permissions += enMainMenuePermissions::pMangeUsers;
	}

	return Permissions;
}


stUser ReadNewUser() {

	stUser User;

	cout << "\nEnter UserName : ";
	getline(cin >> ws, User.UserName);


	while (UserExistsByUserName(User.UserName, UsersFileName)) {
		cout << "\nUser wiht [ " << User.UserName << "] already exists , enter another User Name " << endl;
		getline(cin >> ws, User.UserName);
	}


	cout << "\nEnter Password : ";
	getline(cin, User.Password);

	User.Permissions = ReadPermissionsToSet();

	return User;

}


vector <stClient> LoadClientsDateFromFile(string FileName) {
	fstream NewFile;
	vector <stClient> vClients;

	NewFile.open(FileName, ios::in);
	if (NewFile.is_open()) {
		string NewLine;
		stClient Client;

		while (getline(NewFile, NewLine)) {
			Client = CovertLineToRecored(NewLine);
			vClients.push_back(Client);
		}
		NewFile.close();
	}
	return vClients;
}

vector <stUser> LoadUsersDateFromFile(string UsersFile) {
	fstream NewFile;
	vector <stUser> vUser;

	NewFile.open(UsersFile, ios::in);
	if (NewFile.is_open()) {
		string NewLine;
		stUser User;

		while (getline(NewFile, NewLine)) {
			User = CovertUserLineToRecored(NewLine);
			vUser.push_back(User);
		}
		NewFile.close();
	}
	return vUser;
}


void PrintClientRecordLine(stClient Client) {

	cout << "|" << setw(15) << left << Client.AccountNumber;
	cout << "|" << setw(10) << left << Client.PinCode;
	cout << "|" << setw(40) << left << Client.Name;
	cout << "|" << setw(12) << left << Client.PhoneNumber;
	cout << "|" << setw(12) << left << Client.Balance;

}


void PrintUserCard(stUser User) {

	cout << "\n\nThe Following is the User Name Info : \n";

	cout << "\nUser Name   : " << User.UserName;
	cout << "\nPassword    : " << User.Password;
	cout << "\nPermissions : " << User.Permissions;

}


void ShowAccessDeniedMessage() {
	cout << "\n----------------------------------------------------------\n";
	cout << "Access Denied, \n you don't Have Permission To Do this ,\n Please Contact Your Admin ";
	cout << "\n----------------------------------------------------------\n";
}


bool CheckAccessPermission(enMainMenuePermissions Permission) {
	if (CurrentUser.Permissions == enMainMenuePermissions::eAll)
		return true;
	if ((Permission & CurrentUser.Permissions) == Permission)
		return true;
	else
		return false;

}


void ShowAllClientsScreen() {

	if (!CheckAccessPermission(enMainMenuePermissions::pShowClientsList)) {
		ShowAccessDeniedMessage();
		return;
	}



	vector <stClient> vClients = LoadClientsDateFromFile(ClientsFileName);

	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
	cout << "\n--------------------------------------------------------------------------------------------------------\n" << endl;

	cout << "|" << setw(15) << left << "AccountNumber";
	cout << "|" << setw(10) << left << "Pincode";
	cout << "|" << setw(40) << left << "Name";
	cout << "|" << setw(12) << left << "PhoneNumber";
	cout << "|" << setw(12) << left << "Balance";

	cout << "\n--------------------------------------------------------------------------------------------------------\n" << endl;

	if (vClients.size() == 0)
		cout << "\t\t\t\tNo Clients Available In the System! ";

	else

		for (stClient& Client : vClients) {

			PrintClientRecordLine(Client);
			cout << endl;
		}
	cout << "\n--------------------------------------------------------------------------------------------------------\n" << endl;

}




void ViewTotalBalances() {

	vector<stClient> vClients = LoadClientsDateFromFile(ClientsFileName);

	double totalBalance = 0;

	cout << "\n-----------------------------------------------------------\n";
	cout << setw(15) << left << "AccountNumber" << setw(30) << left << "Name" << setw(12) << left << "Balance" << endl;
	cout << "-----------------------------------------------------------\n";



	for (stClient& c : vClients) {
		cout << "|" << setw(15) << left << c.AccountNumber;
		cout << "|" << setw(30) << left << c.Name;
		cout << "|" << setw(12) << left << c.Balance << endl;

		totalBalance += c.Balance;
	}

	cout << "-----------------------------------------------------------\n";
	cout << setw(48) << left << "Total Balance:    " << totalBalance << endl;
}


void PrintClientCard(stClient ClientInfo) {

	cout << "\n\nThe Following is the Client Info : \n";
	cout << "-----------------------------------------------------------\n";
	cout << "\nAccount Number : " << ClientInfo.AccountNumber;
	cout << "\nPin Code : " << ClientInfo.PinCode;
	cout << "\nName : " << ClientInfo.Name;
	cout << "\nPhone Number : " << ClientInfo.PhoneNumber;
	cout << "\nBalance : " << ClientInfo.Balance;
	cout << "\n-----------------------------------------------------------\n";

}

void PrintUserNameCard(stUser User) {

	cout << "\n\nThe Following is the User Name Info : \n";
	cout << "-----------------------------------------------------------\n";
	cout << "\nUser Name   : " << User.UserName;
	cout << "\nPassword    : " << User.Password;
	cout << "\nPermissions : " << User.Permissions;
	cout << "\n-----------------------------------------------------------\n";


}


bool FindClientByAccountNumber(string AccountNumber, vector<stClient> vClients, stClient& Client) {



	for (stClient& v : vClients) {

		if (v.AccountNumber == AccountNumber) {
			Client = v;
			return true;

		}
	}


	return false;

}


bool FindUserByUserName(string UserName, vector<stUser> vUser, stUser& User) {



	for (stUser& v : vUser) {

		if (v.UserName == UserName) {
			User = v;
			return true;

		}
	}


	return false;

}


bool FindUserByUserNameAndPassword(string UserName, string Password, stUser& User) {


	vector<stUser> vUser = LoadUsersDateFromFile(UsersFileName);


	for (stUser& v : vUser) {

		if (v.UserName == UserName && v.Password == Password) {
			User = v;
			return true;

		}
	}


	return false;

}


stClient UpdateClientInfo(string Account) {

	stClient Client;

	Client.AccountNumber = Account;

	cout << "\nEnter pin code : ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter your Name : ";
	getline(cin, Client.Name);

	cout << "Enter phone number : ";
	getline(cin, Client.PhoneNumber);

	cout << "Enter your Balance : ";
	cin >> Client.Balance;

	return Client;

}

stUser UpdateUserInfo(string UserName) {

	stUser User;

	User.UserName = UserName;

	cout << "\nEnter the Password: ";
	getline(cin >> ws, User.Password);

	User.Permissions = ReadPermissionsToSet();


	return User;

}


bool MarkClientDelForAccountNumber(string AccountNumber, vector <stClient>& vClient) {


	for (stClient& v : vClient) {

		if (v.AccountNumber == AccountNumber) {

			v.MarkForDele = true;
			return true;
		}

	}

	return false;

}


bool MarkUsertDelForAccountNumber(string UserName, vector <stUser>& vUser) {


	for (stUser& v : vUser) {

		if (v.UserName == UserName) {

			v.MarkForDele = true;
			return true;
		}

	}

	return false;

}


vector <stClient> SaveClientDataToFile(string FileName, vector <stClient> vClient) {

	fstream NewFlie;

	NewFlie.open(FileName, ios::out);   //over write

	string DataLine;

	if (NewFlie.is_open()) {

		for (stClient& c : vClient) {

			if (c.MarkForDele == false) {

				DataLine = CovertRecordToLine(c);
				NewFlie << DataLine << endl;

			}
		}
		NewFlie.close();
	}

	return vClient;


}


vector <stUser> SaveUsertDataToFile(string FileName, vector <stUser> vUser) {

	fstream NewFlie;

	NewFlie.open(FileName, ios::out);   //over write

	string DataLine;

	if (NewFlie.is_open()) {

		for (stUser& c : vUser) {

			if (c.MarkForDele == false) {

				DataLine = CovertUserRecordToLine(c);
				NewFlie << DataLine << endl;

			}
		}
		NewFlie.close();
	}

	return vUser;


}


void AddDateLineToFile(string FileName, string NewLine) {

	fstream AddClientToFile;

	AddClientToFile.open(FileName, ios::out | ios::app);

	if (AddClientToFile.is_open()) {
		AddClientToFile << NewLine << endl;

	}

	AddClientToFile.close();

}

void AddNewClient() {

	stClient Client;
	Client = ReadNewClient();


	AddDateLineToFile(ClientsFileName, CovertRecordToLine(Client));

}

void AddNewUsers() {

	stUser User;
	User = ReadNewUser();


	AddDateLineToFile(UsersFileName, CovertUserRecordToLine(User));

}



void AddNewClients() {

	char Add = ' ';


	do {

		system("cls");

		cout << "---------------------------------\n";
		cout << "\tAdding New Clients \n";
		cout << "---------------------------------\n";

		AddNewClient();

		cout << "\n\nClient Add successfully , Want to add more client  Y/N ? ";
		cin >> Add;


	} while (Add == 'y' || Add == 'Y');



}



void AddingNewUsers() {

	char Add = ' ';


	do {

		system("cls");



		cout << "---------------------------------\n";
		cout << "\tAdding New Users \n";
		cout << "---------------------------------\n";

		AddNewUsers();

		cout << "\n\nUser Add successfully , Want to add more User  Y/N ? ";
		cin >> Add;


	} while (Add == 'y' || Add == 'Y');



}


bool DeleAccountByAccountNumber(string AccountNumber, vector <stClient>& vCelints) {

	stClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vCelints, Client)) {

		PrintClientCard(Client);

		cout << "\n\nAre you sure you want to delet this Account Number  ? Y/N  \n";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y') {

			MarkClientDelForAccountNumber(AccountNumber, vCelints);
			SaveClientDataToFile(ClientsFileName, vCelints);


			vCelints = LoadClientsDateFromFile(ClientsFileName);

			cout << "\n\n Client deleted succussfully. ";
			return true;


		}

	}

	else
		cout << "\n\nClient Account Number (" << AccountNumber << ") is not found ";
	return false;

}



bool DeleUserByUserName(string UserName, vector <stUser>& vUser) {

	stUser User;
	char Answer = 'n';

	if (FindUserByUserName(UserName, vUser, User)) {

		PrintUserNameCard(User);

		cout << "\n\nAre you sure you want to delet this User Name  ? Y/N  \n";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y') {

			MarkUsertDelForAccountNumber(UserName, vUser);
			SaveUsertDataToFile(UsersFileName, vUser);


			vUser = LoadUsersDateFromFile(UsersFileName);

			cout << "\n\n User Name  deleted succussfully. ";
			return true;


		}

	}

	else
		cout << "\n\nUser Name (" << UserName << ") is not found ";
	return false;

}

bool UpdateAccountByAccountNumber(string AccountNumber, vector <stClient>& vCelints) {

	stClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vCelints, Client)) {

		PrintClientCard(Client);

		cout << "\n\nAre you sure you want to update this client ? Y/N  \n";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y') {


			for (stClient& v : vCelints) {
				if (v.AccountNumber == AccountNumber) {
					v = UpdateClientInfo(AccountNumber);
					break;
				}
			}


			SaveClientDataToFile(ClientsFileName, vCelints);



			cout << "\n\nClient updated successfully.\n";
			return true;
		}

	}
	else
		cout << "\n\nClient Account Number (" << AccountNumber << ") is not found ";

	return false;
}


bool UpdateUsertByUserName(string UserName, vector <stUser>& vUser) {

	stUser User;
	char Answer = 'n';

	if (FindUserByUserName(UserName, vUser, User)) {

		PrintUserNameCard(User);

		cout << "\n\nAre you sure you want to update this User Name ? Y/N  \n";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y') {


			for (stUser& v : vUser) {
				if (v.UserName == UserName) {
					v = UpdateUserInfo(UserName);
					break;
				}
			}


			SaveUsertDataToFile(UsersFileName, vUser);



			cout << "\n\nUser updated successfully.\n";
			return true;
		}

	}
	else
		cout << "\n\nUser Name (" << UserName << ") is not found ";

	return false;
}

void PrintUserRecordToLine(stUser User) {

	cout << "|" << setw(15) << left << User.UserName;
	cout << "|" << setw(10) << left << User.Password;
	cout << "|" << setw(40) << left << User.Permissions;


}


void ShowAllUsersScreen() {


	vector <stUser> vUser = LoadUsersDateFromFile(UsersFileName);


	cout << "\n\t\t\t\t\tUsers List  (" << vUser.size() << ") User(s).";
	cout << "\n--------------------------------------------------------------------------------------------------------\n" << endl;

	cout << "|" << setw(15) << left << "UserName";
	cout << "|" << setw(10) << left << "Password";
	cout << "|" << setw(40) << left << "Permissions";


	cout << "\n--------------------------------------------------------------------------------------------------------\n" << endl;

	if (vUser.size() == 0)
		cout << "\t\t\t\tNo Users Available In the System! ";
	else

		for (stUser& User : vUser) {

			PrintUserRecordToLine(User);
			cout << endl;
		}
	cout << "\n--------------------------------------------------------------------------------------------------------\n" << endl;

}




// ---------------- FIX 1: Deposit bug ----------------
bool DepositBalanceToClientByAccountNumber(string AccountNumber, vector<stClient>& vClients, double Amount) {
	char Answer = 'n';
	cout << "\n\nAre you sure you want to perfrom this transaction? y/n ? ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y') {
		for (stClient& C : vClients) {
			if (C.AccountNumber == AccountNumber) {
				C.Balance += Amount;
				SaveClientDataToFile(ClientsFileName, vClients);
				cout << "\n\nTransaction successful. New balance: " << C.Balance << endl;
				return true;
			}
		}
	}
	return false;
}



string ReadClientNumber() {

	string AccountNumber = "";

	cout << "Enter the account number  : ";
	cin >> AccountNumber;


	return AccountNumber;
}


string ReadUserName() {

	string UserName;

	cout << "Enter the User Name : ";
	cin >> UserName;


	return UserName;
}


void ShowListUsersScreen() {

	ShowAllUsersScreen();

}


void ShowAddNewUsersScreen() {


	cout << "---------------------------------\n";
	cout << "\tAdd New User Screen \n";
	cout << "---------------------------------\n";

	AddingNewUsers();
}


void ShowDeleteUsers() {


	cout << "---------------------------------\n";
	cout << "\tDelete Users Screen \n";
	cout << "---------------------------------\n";

	vector<stUser> vUsers = LoadUsersDateFromFile(UsersFileName);

	string UserName = ReadUserName();


	DeleUserByUserName(UserName, vUsers);
}


void ShowUpdateUserScreen() {


	cout << "---------------------------------\n";
	cout << "\tUpdate Users Screen \n";
	cout << "---------------------------------\n";

	vector<stUser> vUsers = LoadUsersDateFromFile(UsersFileName);

	string UserName = ReadUserName();


	UpdateUsertByUserName(UserName, vUsers);

}




void showDeleteClientScreen() {

	if (!CheckAccessPermission(enMainMenuePermissions::pDeleteClient)) {

		ShowAccessDeniedMessage();
		return;
	}


	cout << "---------------------------------\n";
	cout << "\tDelete Client Screen \n";
	cout << "---------------------------------\n";

	vector<stClient> vClients = LoadClientsDateFromFile(ClientsFileName);

	string AccountNumber = ReadClientNumber();
	DeleAccountByAccountNumber(AccountNumber, vClients);



}


void ShowUpdateClientScreen() {

	if (!CheckAccessPermission(enMainMenuePermissions::pUpdateClients)) {

		ShowAccessDeniedMessage();
		return;
	}



	cout << "---------------------------------\n";
	cout << "\nUpdate Client Screen \n";
	cout << "---------------------------------\n";

	vector<stClient> vClients = LoadClientsDateFromFile(ClientsFileName);

	string AccountNumber = ReadClientNumber();
	UpdateAccountByAccountNumber(AccountNumber, vClients);



}


void ShowAddNewClientsScreen() {

	if (!CheckAccessPermission(enMainMenuePermissions::pAddNewClient)) {

		ShowAccessDeniedMessage();
		return;

	}



	cout << "---------------------------------\n";
	cout << "\nAdd Client Screen \n";
	cout << "---------------------------------\n";

	vector<stClient> vClients = LoadClientsDateFromFile(ClientsFileName);

	AddNewClients();


}



void ShowFindClientScreen() {

	if (!CheckAccessPermission(enMainMenuePermissions::pShowClientsList)) {

		ShowAccessDeniedMessage();
		return;
	}



	cout << "---------------------------------\n";
	cout << "\nFind Client Screen \n";
	cout << "---------------------------------\n";

	vector<stClient> vClients = LoadClientsDateFromFile(ClientsFileName);
	stClient Client;

	string AccountNumber = ReadClientNumber();

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
		PrintClientCard(Client);

	else

		cout << "\nClient with Account Number [ " << AccountNumber << "] is not Found! ";



}


void ShowFindUserScreen() {




	cout << "---------------------------------\n";
	cout << "\nFind User Screen \n";
	cout << "---------------------------------\n";

	vector<stUser> vUsers = LoadUsersDateFromFile(UsersFileName);
	stUser User;

	string UserName = ReadUserName();

	if (FindUserByUserName(UserName, vUsers, User))
		PrintUserNameCard(User);

	else

		cout << "\nUser with User Name [ " << UserName << "] is not Found! ";

}



void ShowEndScreen() {

	cout << "-------------------------------------------\n";
	cout << "\nProgram Ends  \n";
	cout << "-------------------------------------------\n";

}

void ShowDepositScreen() {


	cout << "-------------------------------------------\n";
	cout << "\nDeposit Screen   \n";
	cout << "-------------------------------------------\n";

	stClient Client;

	vector<stClient> vClient = LoadClientsDateFromFile(ClientsFileName);

	string AccountNumber = ReadClientNumber();

	while (!FindClientByAccountNumber(AccountNumber, vClient, Client)) {
		cout << "\nClient with [ " << AccountNumber << "] does not exist. \n ";

		AccountNumber = ReadClientNumber();
	}
	PrintClientCard(Client);

	double Amount = 0;
	cout << "\nPlease enter deposit amount ? ";
	cin >> Amount;


	DepositBalanceToClientByAccountNumber(AccountNumber, vClient, Amount);


}


// ---------------- FIX 2: Withdraw logic ----------------
void ShowWithDrawScreen() {

	cout << "-------------------------------------------\n";
	cout << "\nWhithDraw Screen  \n";
	cout << "-------------------------------------------\n";

	stClient Client;
	vector<stClient> vClient = LoadClientsDateFromFile(ClientsFileName);
	string AccountNumber = ReadClientNumber();

	while (!FindClientByAccountNumber(AccountNumber, vClient, Client)) {
		cout << "\nClient with [ " << AccountNumber << "] does not exist. \n ";
		AccountNumber = ReadClientNumber();
	}

	PrintClientCard(Client);

	double Amount = 0;
	cout << "\nPlease enter withdraw amount ? ";
	cin >> Amount;

	while (Amount > Client.Balance) {
		cout << "\nAmount Exceeds the balance, You can withdraw up to : " << Client.Balance << endl;
		cout << "Please enter another amount ? ";
		cin >> Amount;
	}

	DepositBalanceToClientByAccountNumber(AccountNumber, vClient, -Amount);
}




void ShowTotalBalacesScreen() {

	ViewTotalBalances();
}




// Clients Meneu

short ReadMainMenueOption() {
	short Choice = 0;
	cout << "Choose what do you want to do ?  [1 to 8] ? ";
	cin >> Choice;

	return Choice;
}

void ShowMainMenu() {
	system("cls");

	cout << "\n==================== Bank System Menu ====================" << endl;
	cout << "1. Show all clients" << endl;
	cout << "2. Add new client" << endl;
	cout << "3. Delete a client" << endl;
	cout << "4. Update a client" << endl;
	cout << "5. Search client by account number" << endl;
	cout << "6. Transactions " << endl;
	cout << "7. Mange Users" << endl;
	cout << "8. Logout " << endl;
	cout << "==========================================================" << endl;

	PerfromClientsMenueOption((enMainMenueOptions)ReadMainMenueOption());
}

void PerfromClientsMenueOption(enMainMenueOptions ClientsMainMenueOptions) {

	switch (ClientsMainMenueOptions) {
	case enMainMenueOptions::enListCliens: {

		system("cls");
		ShowAllClientsScreen();
		GoBackTOMainMenue();
		break;
	}

	case enMainMenueOptions::enAddNewClient: {

		system("cls");
		ShowAddNewClientsScreen();
		GoBackTOMainMenue();
		break;
	}

	case enMainMenueOptions::enDeleteClient: {

		system("cls");
		showDeleteClientScreen();
		GoBackTOMainMenue();
		break;
	}

	case enMainMenueOptions::enUpdateClient: {

		system("cls");
		ShowUpdateClientScreen();
		GoBackTOMainMenue();
		break;
	}

	case enMainMenueOptions::enFindClient: {

		system("cls");
		ShowFindClientScreen();
		GoBackTOMainMenue();
		break;
	}

	case enMainMenueOptions::enShoeTransactionsMenue: {

		system("cls");
		ShowTransactionsMenu();
		break;   // ✅ let Transactions menu control flow
	}



	case enMainMenueOptions::enMangeUsers: {

		system("cls");
		ShowMangeUsers();
		GoBackTOMainMenue();
		break;
	}

	case enMainMenueOptions::enEixt: {
		system("cls");
		ShowEndScreen();
		Login();

	}

	}

}




void GoBackTOMainMenue() {
	cout << "\n\nPree any key to go back to Main Menue.. ";
	system("pause > 0");
	ShowMainMenu();

}

void GoBackTOTransactionsMenure() {
	cout << "\n\nPree any key to go back to Transactions Menue.. ";
	system("pause > 0");
	ShowTransactionsMenu();

}

void GoBackTOMaangeUsersMenue() {
	cout << "\n\nPree any key to go back to Mange Users Menue.. ";
	system("pause > 0");
	ShowMangeUsers();

}



// Mange Users Menue
short ReadMangeUserMenueOption() {
	short Choice = 0;
	cout << "Choose what do you want to do ?  [1 to 6] ? ";
	cin >> Choice;

	return Choice;
}




void ShowMangeUsers() {


	if (!CheckAccessPermission(enMainMenuePermissions::pMangeUsers)) {

		ShowAccessDeniedMessage();
		GoBackTOMainMenue();
		return;
	}

	system("cls");

	cout << "\n==================== Mange Users Screen ====================" << endl;
	cout << "1. List Users" << endl;
	cout << "2. Add New User" << endl;
	cout << "3. Delet User" << endl;
	cout << "4. Update User" << endl;
	cout << "5. Find User" << endl;
	cout << "6. Main Menue " << endl;
	cout << "==========================================================" << endl;


	PerfromMangeUsersMenueOption((enMangeUsersMeuneOptions)ReadMangeUserMenueOption());

}

void PerfromMangeUsersMenueOption(enMangeUsersMeuneOptions MangeUsersMenueOption) {

	switch (MangeUsersMenueOption) {
	case enMangeUsersMeuneOptions::enListUser: {

		system("cls");
		ShowListUsersScreen();
		GoBackTOMaangeUsersMenue();
		break;
	}

	case enMangeUsersMeuneOptions::enAddNewUser: {

		system("cls");
		ShowAddNewUsersScreen();
		GoBackTOMaangeUsersMenue();
		break;
	}

	case enMangeUsersMeuneOptions::enDeleteUser: {

		system("cls");
		ShowDeleteUsers();
		GoBackTOMaangeUsersMenue();
		break;
	}

	case enMangeUsersMeuneOptions::enUpdateUser: {

		system("cls");
		ShowUpdateUserScreen();
		GoBackTOMaangeUsersMenue();
		break;
	}

	case enMangeUsersMeuneOptions::enFindUser: {

		system("cls");
		ShowFindUserScreen();
		GoBackTOMaangeUsersMenue();
		break;
	}
	case enMangeUsersMeuneOptions::enMainMenue: {

		ShowMainMenu();
	}

	}

}


// Transactions Menue
void PerfromTransactionsMenueOption(enTransactionsMenueOptions TransactionsMenueOption) {




	switch (TransactionsMenueOption) {
	case enTransactionsMenueOptions::enDepist: {

		system("cls");
		ShowDepositScreen();
		GoBackTOMainMenue();
		break;
	}

	case enTransactionsMenueOptions::enWithdrow: {

		system("cls");
		ShowWithDrawScreen();
		GoBackTOMainMenue();
		break;
	}
	case enTransactionsMenueOptions::enShowTotalBalace: {

		system("cls");
		ShowTotalBalacesScreen();
		GoBackTOMainMenue();
		break;
	}

	case enTransactionsMenueOptions::enShowMainMenue: {

		ShowMainMenu();
	}

	}

}

short ReadTransactionsMenueOption() {

	short Choice = 0;
	cout << "Choose what do you want to do ?  [1 to 4] ? ";
	cin >> Choice;

	return Choice;
}

void ShowTransactionsMenu() {

	if (!CheckAccessPermission(enMainMenuePermissions::pTranactions)) {

		ShowAccessDeniedMessage();
		GoBackTOMainMenue();
		return;
	}

	system("cls");

	cout << "\n================= Transactions Menu =================" << endl;
	cout << "1. Deposit" << endl;
	cout << "2. Withdraw" << endl;
	cout << "3. View All Balances" << endl;
	cout << "4. Back to Main Menu" << endl;
	cout << "====================================================" << endl;

	PerfromTransactionsMenueOption((enTransactionsMenueOptions)ReadTransactionsMenueOption());

}


bool LoadUserInfo(string UserName, string Password) {

	if (FindUserByUserNameAndPassword(UserName, Password, CurrentUser))
		return true;
	else
		return false;
}


void Login() {

	bool LoginFalid = false;

	string UserName, Password;

	do {
		system("cls");

		cout << "-------------------------------------------\n";
		cout << "\tLogin Screen \n";
		cout << "-------------------------------------------\n";


		if (LoginFalid) {
			cout << "Invalid Username/Password !  \n";

		}

		cout << "Enter UserName ? ";
		cin >> UserName;

		cout << "Enter Password ? ";
		cin >> Password;

		LoginFalid = !LoadUserInfo(UserName, Password);


	} while (LoginFalid);

	ShowMainMenu();


}

int main() {
	Login();
	system("pause > 0");
	return 0;
}  