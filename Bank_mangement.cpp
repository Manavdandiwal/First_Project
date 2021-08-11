#include<iostream>
#include<string.h>
#include<vector>
#include<fstream>
#include<ctime>
#include<string>
using namespace std;

struct bank_data
{
	string name;
	string account_number;
	long long balance = 0;
	long long phone_number = 0;
};

struct last_three_trans
{
	string account_number;
	pair<string, int> transaction_1;
	pair<string, int> transaction_2;
	pair<string, int> transaction_3;
};

string gen_random(const int len) {

	string tmp_s;
	static const char alphanum[] =
		"0123456789";

	srand(time(NULL));

	tmp_s.reserve(len);

	for (int i = 0; i < len; ++i)
		tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];


	return tmp_s;

}
vector<bank_data> users_info;
vector<last_three_trans> trans_detail;

void input_new_user()
{
	bank_data new_user;
	cout<< "Enter your Name : ";
	fflush(stdin);
	getline(cin,new_user.name);
	cout << "Enter amount to be deposited : ";
	cin >> new_user.balance;
	cout << "Enter your Phone Number : ";
	cin >> new_user.phone_number;

	for (auto i : users_info)
	{
		if ((i.name == new_user.name) && (i.phone_number == new_user.phone_number))
		{
			cout << "User already exists" << endl;
			return;
		}
	}
	new_user.account_number = gen_random(15);
	cout << "Account Created Successfully" << endl;
	cout << "Your Account Number is : " << new_user.account_number << endl;
	users_info.push_back(new_user);
}

void check_transactions()
{
	string user;
	cout << "Please enter your account number : ";
	cin >> user;

	for (auto i : trans_detail)
	{
		if (i.account_number == user)
		{
			cout << "Your Last three transactions are as follow : " << endl;
			cout << i.transaction_1.first << "  " << i.transaction_1.second << endl;
			cout << i.transaction_2.first << "  " << i.transaction_2.second << endl;
			cout << i.transaction_2.first << "  " << i.transaction_3.second << endl;

			return;
		}
	}

	cout << "No Transactions have been made yet" << endl;
}

void balance_inquiry()
{
	string acc_num;
	fflush(stdin);
	cout << "Enter your Account number : ";
	cin >> acc_num;
	
	for (auto i : users_info)
	{
		if (i.account_number == acc_num)
		{
			cout << "Welcome Mr. "<<i.name<<endl;
			cout << "Your current balance is " << i.balance << endl;
			return;
		}
	}
	cout << "User not found. Please confirm your details." << endl;
}

void operation_withdraw()
{
	string acc_no;
	int amount;
	cout << "Enter account number : ";
	cin >> acc_no;
	cout << "Enter Amount to be Withdrawn : ";
	cin >> amount;

	for (auto& i : users_info)
	{
		if (i.account_number == acc_no)
		{
			if (i.balance >= amount)
			{
				i.balance -= amount;
				cout << "Withdrawn successfully" << endl;
				break;
			}

			cout << "Insufficient Balance" << endl;
			break;
		}
	}

	for (auto& i : trans_detail)
	{
		if (i.account_number == acc_no)
		{
			i.transaction_3 = i.transaction_2;
			i.transaction_2 = i.transaction_1;
			i.transaction_1 = { "Withdrawn",amount };
			return;
		}
	}
	last_three_trans new_;
	new_.account_number = acc_no;
	new_.transaction_1 = { "Withdrawn",amount };
	new_.transaction_2 = { "None",0 };
	new_.transaction_3 = { "None",0 };

	trans_detail.push_back(new_);
}

void operation_deposit()
{
	string acc_no;
	int amount;
	cout << "Enter account number : ";
	cin >> acc_no;
	cout << "Enter Amount to be Deposited : ";
	cin >> amount;

	for (auto& i : users_info)
	{
		if (i.account_number == acc_no)
		{
			i.balance += amount;
			cout<<"Deposited Successfully"<<endl;
		}
	}

	for (auto& i : trans_detail)
	{
		if (i.account_number == acc_no)
		{
			i.transaction_3 = i.transaction_2;
			i.transaction_2 = i.transaction_1;
			i.transaction_1 = { "Deposited",amount };
			return;
		}
	}
	last_three_trans new_;
	new_.account_number = acc_no;
	new_.transaction_1 = { "Deposited",amount };
	new_.transaction_2 = { "None",0 };
	new_.transaction_3 = { "None",0 };

	trans_detail.push_back(new_);
}

void insering_detail()
{
	ofstream user("Bank_Data.txt");
	for (auto i : users_info)
	{
		for(int j=0;j<i.name.length();j++)
		{
			user<<i.name[j];
		}
		user<<endl;
		user << i.account_number << endl << i.balance << endl << i.phone_number << endl;
	}
	
	ofstream transactions("Transaction_details.txt");

	for (auto i : trans_detail)
	{
		transactions << i.account_number << endl << i.transaction_1.first << endl << i.transaction_1.second << endl << i.transaction_2.first << endl << i.transaction_2.second << endl << i.transaction_3.first << endl << i.transaction_3.second << endl;
	}
}

void get_info()
{
	bank_data tmp;
	last_three_trans temp;
	ifstream user_data,transactions;
	user_data.open("Bank_Data.txt");
	transactions.open("Transaction_details.txt");	

	while (getline(user_data,tmp.name) && user_data >> tmp.account_number >> tmp.balance >> tmp.phone_number)
	{
		users_info.push_back(tmp);
	}

	while (transactions >> temp.account_number >> temp.transaction_1.first >> temp.transaction_1.second >> temp.transaction_2.first >> temp.transaction_2.second >> temp.transaction_3.first >> temp.transaction_3.second)
	{
		trans_detail.push_back(temp);
	}

	user_data.close();
	transactions.close();
}
int main()
{
	get_info();
	system("cls");
	cout << "-------------------------------------------------" << endl;
	cout << "         Hello Sir! Welcome to the Bank." << endl;
	cout << "-------------------------------------------------" << endl;

	cout << "How can I help you Sir!" << endl;

	while (1)
	{
		cout << endl;
		int operation;
		cout << "Enter 1 for creating a new account" << endl;
		cout << "Enter 2 for balance Enquiry" << endl;
		cout << "Enter 3 to review your last three transactions" << endl;
		cout << "Enter 4 to withdraw money" << endl;
		cout << "Enter 5 to deposit money" << endl;
		cout << "Enter 0 to Exit" << endl;

		cin >> operation;

		switch (operation)
		{
		case 0:
			insering_detail();
			cout << "Thank you! Have a nice day" << endl;
			exit(0);

		case 1:
			input_new_user();
			break;

		case 2:
			balance_inquiry();
			break;

		case 3:
			check_transactions();
			break;

		case 4:
			operation_withdraw();
			break;

		case 5:
			operation_deposit();
			break;

		default:
			insering_detail();
			cout << "Thank you! Have a nice day" << endl;
			exit(0);
		}
	}
}
