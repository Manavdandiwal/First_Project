#include<iostream>
#include<string.h>
#include<vector>
#include<fstream>
#include<ctime>
#include <unistd.h>
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

std::string gen_random(const int len) {

	std::string tmp_s;
	static const char alphanum[] =
		"0123456789";

	srand((unsigned)time(NULL) * getpid());

	tmp_s.reserve(len);

	for (int i = 0; i < len; ++i)
		tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];


	return tmp_s;

}
vector<bank_data> users_info;
vector<last_three_trans> trans_detail;

void input_new_user(vector<bank_data>& users_info)
{
	bank_data new_user;
	string first_name,Last_name;
	cout << "Enter your Name : ";
	cin >> first_name >> Last_name;
	new_user.name += first_name + " " + Last_name;
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

void check_transactions(vector<bank_data>& users_info)
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

void balance_inquiry(vector<bank_data>& users_info)
{
	string name, acc_num;
	long long ph;
	string first_name, last_name;
	cout << "Enter your Name : ";
	cin >> first_name >> last_name;
	name = first_name + " " + last_name;
	fflush(stdin);
	cout << "Enter your Account number : ";
	cin >> acc_num;
	cout << "Enter Phone Number : ";
	cin >> ph;

	for (auto i : users_info)
	{
		if ((i.name == name) && (i.phone_number == ph) && (i.account_number == acc_num))
		{
			cout << "Your current balance is " << i.balance << endl;
			return;
		}
	}
	cout << "User not found. Please confirm your details." << endl;
}

void operation_withdraw(vector<bank_data>& users_info,vector<last_three_trans>& trans_detail)
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

void operation_deposit(vector<bank_data>& users_info, vector<last_three_trans>& trans_detail)
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
	for (auto i : users_info)
	{
		ofstream user("Bank_Data.txt");

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

int main()
{
	bank_data tmp;
	last_three_trans temp;
	fstream user_data,transactions;
	user_data.open("Bank_Data.txt");
	transactions.open("Transaction_details.txt");	

	string first_name,last_name;
	while (user_data >> first_name >> last_name >> tmp.account_number >> tmp.balance >> tmp.phone_number)
	{
		tmp.name = first_name + " " + last_name;
		users_info.push_back(tmp);
	}

	while (transactions >> temp.account_number >> temp.transaction_1.first >> temp.transaction_1.second >> temp.transaction_2.first >> temp.transaction_2.second >> temp.transaction_3.first >> temp.transaction_3.second)
	{
		trans_detail.push_back(temp);
	}

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
			input_new_user(users_info);
			break;

		case 2:
			balance_inquiry(users_info);
			break;

		case 3:
			check_transactions(users_info);
			break;

		case 4:
			operation_withdraw(users_info,trans_detail);
			break;

		case 5:
			operation_deposit(users_info, trans_detail);
			break;

		default:
			insering_detail();
			cout << "Thank you! Have a nice day" << endl;
			exit(0);
		}
	}
}