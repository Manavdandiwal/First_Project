#include<iostream>
#include<fstream>
#include<chrono>
#include<string>
#include<vector>
using namespace std;

#define TAXI_FARE 500

struct bank_data
{
	string name;
	string account_number;
	long long balance = 0;
	long long phone_number = 0;
};

int no_of_taxis_available = 15;

struct taxi_stand
{
	string name;
	string Phone_number;
	time_t init;
};

vector<taxi_stand> users;

vector<bank_data> users_bank_info;
void get_info()
{
	bank_data tmp;
	ifstream user_data;
	user_data.open("Bank_Data.txt");	

	while (getline(user_data,tmp.name) && user_data >> tmp.account_number >> tmp.balance >> tmp.phone_number)
	{
		users_bank_info.push_back(tmp);
	}

	user_data.close();
}

int payment(float pay)
{
	char ch;
	cout<<"Do you have a bank account?(y/n) : ";
	cin>>ch;

	if(ch == 'n' || ch == 'N')
	{
		cout<<"Please create an account to pay you bill"<<endl;
		return 0;
	}

	string account;
	cout<<"Please enter your account number : ";
	cin>>account;

	for(auto& i : users_bank_info)
	{
		if(i.account_number==account)
		{
			if(i.balance>=pay)
			{
				i.balance -= pay;
				cout<<"Payment successful"<<endl;
				return 1;
			}

			else
			cout<<"Insufficient balance"<<endl;
			return 0;
		}
	}

	cout<<"Invalid account"<<endl;
}

void book()
{
	if (!no_of_taxis_available)
	{
		cout << "Sorry, no taxi is available for booking" << endl;
		return;
	}
	
	taxi_stand new_user;
	cout << "Enter your Name : " ;
	fflush(stdin);
	getline(cin, new_user.name);
	cout << "Enter your Phone number : " ;
	cin >> new_user.Phone_number;
	new_user.init = time(NULL);

	users.push_back(new_user);

	no_of_taxis_available--;

	cout << "Taxi booked successfully! You will be charged "<< TAXI_FARE << " rupees per hour starting from now" << endl;
}

void pay()
{
	time_t final = time(NULL);
	float used;
	string name, phone;
	cout << "Enter your name : ";
	fflush(stdin);
	getline(cin, name);
	cout << "Enter your Registered phone number : ";
	cin >> phone;

	for (int i = 0; i < users.size(); i++)
	{
		if (users[i].name == name && users[i].Phone_number == phone)
		{
			used = final - users[i].init;
			used /= 3600*1.0;
			float bill = TAXI_FARE * used;

			cout << "Your billing amount is " << bill << "rupees" << endl;

			if(payment(bill))
			{
				users.erase(users.begin() + i);
				no_of_taxis_available++;
			}
			return;
		}
	}

	cout<<"No booking found"<<endl;
}

void inserting_detail()
{
	ofstream user("Bank_Data.txt");
	for (auto i : users_bank_info)
	{
		user<<i.name;
		user<<endl;
		user << i.account_number << endl << i.balance << endl << i.phone_number << endl;
	}
}

int main()
{
	get_info();
    fstream user_data;
	user_data.open("User_Data.txt");
	taxi_stand user;
	
    while (getline(user_data,user.name) && user_data>>user.Phone_number>>user.init)
    {
        users.push_back(user);
    }

    no_of_taxis_available -= users.size();
    
	user_data.close();
    system("cls");
	cout << "-------------------------------------------" << endl;
	cout << "         Welcome to the Taxi Stand         " << endl;
	cout << "-------------------------------------------" << endl;

	int operation;

	cout << "Taxi Fare will be "<< TAXI_FARE << " rupees per hour" << endl;

	repeat:
		cout << "To book a taxi enter 1" << endl;
		cout << "To pay your bill enter 2" << endl;
		cout << "To Exit enter 3" << endl;

		cin >> operation;

		switch (operation)
		{
		case 1:
			book();
			goto repeat;
		case 2:
			pay();
			goto repeat;
		default:
			inserting_detail();
			remove("User_Data.txt");
            ofstream user_data;
			user_data.open("User_Data.txt");
			for (int i = 0; i < users.size(); i++)
			{
				user_data << users[i].name << endl << users[i].Phone_number << endl << users[i].init << endl;
			}
			break;
		}
}