#include <iostream>
#include<string>
#include<vector>
#include<iomanip>
#include<fstream>

using namespace std;

void Login();
void ShowMainMenueScreen();
void ShowDepositScreen();
void ShowNormalWithdarwScreen();
void ShowQuichWithdrawScreen();

const string ClientsFileName = "Clients.txt";

struct stClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

enum enMainMenueOption
{
    eQuickWithdraw = 1, eNormalWithdraw = 2, eDeposit = 3, eCheckBalance = 4, eExit = 5
};

stClient CurrentClient;

vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord;

    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos);
        if (sWord != "")
        {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length());
    }
    if (S1 != "")
    {
        vString.push_back(S1);
    }

    return vString;
}

stClient ConvertLineToRecord(string Line, string Seprator = "#//#")
{
    stClient Client;
    vector<string> vClientData;

    vClientData = SplitString(Line, Seprator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);

    return Client;

}

string ConvertRecordToLine(stClient Client, string Separator = "#//#")
{
    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Separator;
    stClientRecord += Client.PinCode + Separator;
    stClientRecord += Client.Name + Separator;
    stClientRecord += Client.Phone + Separator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;
}

vector<stClient> LoadClientDataFromFile(string FileName)
{
    vector<stClient> vClient;
    fstream MyFile;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        stClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLineToRecord(Line);

            vClient.push_back(Client);
        }
        MyFile.close();
    }

    return vClient;
}

vector<stClient> SaveClientsDataToFile(string FileName, vector<stClient> vClient)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);

    string DataLine;

    if (MyFile.is_open())
    {
        for (stClient C : vClient)
        {
            if (C.MarkForDelete == false)
            {
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }

    return vClient;

}

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, stClient& Client)
{
    vector<stClient> vClients = LoadClientDataFromFile(ClientsFileName);

    for (stClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

bool LoadClientInfo(string Username, string Password)
{
    if (FindClientByAccountNumberAndPinCode(Username, Password, CurrentClient))
        return true;
    else
        return false;
}

//Deposite

double ReadDepositeAmount()
{
    double Amount;
    cout << "\nEnter a positive Deposite amount? ";
    cin >> Amount;

    while (Amount <= 0)
    {
        cout << "\nEnter a positive Deposite amount? ";
        cin >> Amount;
    }

    return Amount;
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector<stClient>& vClients)
{
    char Answer = 'n';
    cout << "\n\n Are you sure you want perform this transactions? y/n ? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        for (stClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveClientsDataToFile(ClientsFileName, vClients);
                cout << "\n\n Done Successfully. New balance is : " << C.AccountBalance;
                return true;
            }


        }
        return false;
    }

}

void PerformDepositeOption()
{
    double DepositAmount = ReadDepositeAmount();

    vector<stClient> vClient = LoadClientDataFromFile(ClientsFileName);

    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, DepositAmount, vClient);

    CurrentClient.AccountBalance += DepositAmount;
}

void ShowDepositScreen()
{
    cout << "\n===============================================\n";
    cout << "\t Deposit Screen ";
    cout << "\n===============================================\n";

    PerformDepositeOption();

}

//QuickWithdarw

short ReadQuickWithDraw()
{
    short Choice = 0;

    while (Choice < 1 || Choice>9)
    {
        cout << "\nChoose what to withdarw from [1] to [9]? ";
        cin >> Choice;
    }
    return Choice;
}

short GetQuickWithdrawAmount(short QuickWithdrawAmount)
{
    switch (QuickWithdrawAmount)
    {
    case 1:
        return 20;
    case 2:
        return 50;
    case 3:
        return 100;
    case 4:
        return 200;
    case 5:
        return 400;
    case 6:
        return 600;
    case 7:
        return 800;
    case 8:
        return 1000;
    default:
        return 0;
    }
}

void PerformQuickWithdrawOption(short QuickWithdrawAmount)
{
    if (QuickWithdrawAmount == 9)
        return;

    int WithdarwBalance = GetQuickWithdrawAmount(QuickWithdrawAmount);

    if (WithdarwBalance > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        cout << "Press Anykey to continue...";
        system("pause>0");
        system("cls");
        ShowQuichWithdrawScreen();

        return;
    }

    vector<stClient> vClient = LoadClientDataFromFile(ClientsFileName);

    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithdarwBalance * -1, vClient);
    CurrentClient.AccountBalance -= WithdarwBalance;

}

void ShowQuichWithdrawScreen()
{
    cout << "\n===============================================\n";
    cout << "\t\t Quick Withdraw";
    cout << "\n===============================================\n";

    cout << "\t [1] 20 \t\t[2] 50.\n";
    cout << "\t [3] 100 \t\t[4] 200.\n";
    cout << "\t [5] 400 \t\t[6] 600.\n";
    cout << "\t [7] 800 \t\t[8] 1000.\n";
    cout << "\t [9] Exit\n";

    cout << "===============================================\n";

    cout << " Your Balance is " << CurrentClient.AccountBalance << endl;

    PerformQuickWithdrawOption(ReadQuickWithDraw());
}

//Normal Withdarw

int ReadWithdarwAmount()
{
    int Amount = 0;
    cout << "\nEnter an amount multiple of 5' s ? ";
    cin >> Amount;

    while (Amount % 5 != 0)
    {
        cout << "\nEnter an amount multiple of 5' s ? ";
        cin >> Amount;
    }
    return Amount;
}

void PerformNormalWithdrawOption()
{
    int WithdarwBalance = ReadWithdarwAmount();

    if (WithdarwBalance > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        cout << "Press Anykey to continue...";
        system("pause>0");

        ShowNormalWithdarwScreen();
        return;
    }

    vector<stClient> vClient = LoadClientDataFromFile(ClientsFileName);

    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithdarwBalance * -1, vClient);
    CurrentClient.AccountBalance -= WithdarwBalance;

}

void ShowNormalWithdarwScreen()
{
    system("cls");
    cout << "\n===============================================\n";
    cout << "\t\tNormal Withdraw Screen";
    cout << "\n===============================================\n";

    PerformNormalWithdrawOption();
}


void ShowCheckBalancesScreen()
{
    system("cls");
    cout << "\n===============================================\n";
    cout << "\t\t Check Balance Screen";
    cout << "\n===============================================\n";

    cout << " Your Balance is " << CurrentClient.AccountBalance;

}

void GoBackToMainMenue()
{
    cout << " \n\nPress any key to go back to Main Menue...\n";
    system("pause>0");

}

short ReadMainMenueOption()
{
    short Choice;
    cout << "\n Choose what do you want to do? [1 to 5]? ";
    cin >> Choice;
    return Choice;
}

void PerformMainMenueOption(enMainMenueOption MainMeueOption)
{
    switch (MainMeueOption)
    {
    case enMainMenueOption::eQuickWithdraw:

        system("cls");
        ShowQuichWithdrawScreen();
        GoBackToMainMenue();
        ShowMainMenueScreen();
        break;

    case enMainMenueOption::eNormalWithdraw:

        system("cls");
        ShowNormalWithdarwScreen();
        GoBackToMainMenue();
        ShowMainMenueScreen();
        break;

    case enMainMenueOption::eDeposit:

        system("cls");
        ShowDepositScreen();
        GoBackToMainMenue();
        ShowMainMenueScreen();
        break;


    case enMainMenueOption::eCheckBalance:

        system("cls");
        ShowCheckBalancesScreen();
        GoBackToMainMenue();
        ShowMainMenueScreen();
        break;


    case enMainMenueOption::eExit:

        system("cls");
        Login();
        break;

    }

}

void ShowMainMenueScreen()
{
    system("cls");

    cout << "\n===============================================\n";
    cout << "\t\t ATM Main Menue Screen ";
    cout << "\n===============================================";

    cout << "\n\t [1] Quick Withdraw.";
    cout << "\n\t [2] Normal Withdraw.";
    cout << "\n\t [3] Deposit.";
    cout << "\n\t [4] Check Balance.";
    cout << "\n\t [5] Logout.";

    cout << "\n===============================================\n";

    PerformMainMenueOption(enMainMenueOption(ReadMainMenueOption()));
}

void Login()
{
    bool LoginFailed = false;
    string PinCode, AccountNumber;

    do
    {
        system("cls");
        cout << "\n------------------------------------------\n";
        cout << "\t Login Screen";
        cout << "\n------------------------------------------\n";

        if (LoginFailed)
        {
            cout << "Invalide AccountNumber / PinCode!\n";
        }

        cout << "Enter AccountNumber? ";
        cin >> AccountNumber;
        cout << "Enter PinCode? ";
        cin >> PinCode;

        LoginFailed = !LoadClientInfo(AccountNumber, PinCode);

    } while (LoginFailed);

    ShowMainMenueScreen();

}


int main()
{
    Login();




    system("pause>0");
    return 0;
}
