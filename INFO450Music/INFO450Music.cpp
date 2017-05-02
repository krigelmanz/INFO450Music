// Zach Krigelman
// INFO 450 Music

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int READERROR = 100;
const int WRITEERROR = 200;
const int ARRAYSIZE = 100;

class Inventory
{
	string ArtistName;
	string ItemName;
	Inventory *next;
public:
	Inventory();
	Inventory(string num, string name);
	void CaptureInventoryItem();
	void ShowInventoryItem();
	int SaveInventoryItem(ofstream &outfile);
	friend class InventoryList;
};

class InventoryList
{

	int numrecords;
	Inventory *head;
	Inventory *tail;

public:
	InventoryList();
	~InventoryList();
	void GetUserInput();
	void AddNodeToEnd(Inventory *newnode);
	void ShowInventoryList();
	void DeleteItem(Inventory *ptr);
	void PlaySong();
	int SaveInventoryList(string filename);
	int ReadInventoryList(string filename);
};

Inventory::Inventory()
{
	ArtistName = ""; ItemName = "";
	next = NULL;
}

Inventory::Inventory(string num, string name)
{
	ArtistName = num;
	ItemName = name;
	next = NULL;
}

void Inventory::CaptureInventoryItem()
{
	cout << "Artist Name?  -->";
	getline(cin, ArtistName);
	cout << "Song Name?  -->";
	getline(cin, ItemName);
}

void Inventory::ShowInventoryItem()
{
	cout << "Artist : " << ArtistName << " Song : " << ItemName << endl;
}

int Inventory::SaveInventoryItem(ofstream& outfile)
{
	if (outfile.is_open())
	{
		outfile << ArtistName << "|" << ItemName << endl;
		return 0;
	}
	else
		return WRITEERROR;
}

InventoryList::InventoryList()
{

	numrecords = 0;
	head = NULL;
	tail = NULL;

}

InventoryList::~InventoryList()
{
	Inventory *ptr = head;
	while (ptr != NULL)
	{
		head = ptr->next;
		delete ptr;
		ptr = head;
	}
}

void InventoryList::AddNodeToEnd(Inventory *newnode)
{
	if (head == NULL)
	{
		head = newnode;
		tail = newnode;
	}
	else
	{
		tail->next = newnode;
		tail = newnode;
	}
}

void InventoryList::GetUserInput()
{
	string answer = "Y";
	cout << "enter new song Y/N?" << endl;
	getline(cin, answer);
	while (answer == "Y" || answer == "y")
	{
		Inventory *nptr = new Inventory();
		nptr->CaptureInventoryItem();
		AddNodeToEnd(nptr);

		numrecords++;
		cout << "enter another song Y/N?" << endl;
		getline(cin, answer);
	}
}

void InventoryList::DeleteItem(Inventory *s) {
	Inventory *ptr = head;
	if (ptr->ItemName == s->ItemName && ptr->ArtistName == s->ArtistName) {
		head = ptr->next;
		delete ptr;
		return;
	}
	while (ptr != NULL) {
		if (ptr->next->ItemName == s->ItemName && ptr->next->ArtistName == s->ArtistName) {
			ptr->next = ptr->next->next;
			delete ptr;
			return;
		}
	}
}

void InventoryList::PlaySong()
{
	string Input;
	int quant;
	Inventory *ptr = head;
	Inventory *tbd = NULL;
	if (head == NULL)
		return;

	while (ptr != NULL)
	{
		ptr->ShowInventoryItem();
		cout << "What next? (P)lay (S)kip (D)elete (A)dd or (Q)uit" << endl;
		getline(cin, Input);
		if (Input == "P" || Input == "p")
		{
			cout << "Playing Song: " << endl;
		}
		else if (Input == "S" || Input == "s")
		{
			cout << "How many songs would you like to skip?" << endl;
			cin >> quant;
			for (int i = 0; i < quant; i++)
			{
				ptr = ptr->next;
				if (ptr == NULL)
				{
					ptr = head;
				}
			}
			cin.ignore();
			cin.clear();
		}
		else if (Input == "D" || Input == "d")
		{
			tbd = ptr->next;
			DeleteItem(ptr);
			ptr = tbd;
		}
		else if (Input == "Q" || Input == "q")
		{
			cout << "Exiting playback" << endl;
			break;
		}
		else if (ptr == NULL)
		{
			ptr = head;
		}
		else if (Input == "A" || Input == "a")
		{
			Inventory *nptr = new Inventory();
			nptr->CaptureInventoryItem();
			{
				nptr->next = ptr->next;
				ptr->next = nptr;
			}

			numrecords++;
		}
	}
}

void InventoryList::ShowInventoryList()
{
	Inventory *ptr = head;
	if (head == NULL)
		return;

	while (ptr != NULL)
	{
		ptr->ShowInventoryItem();
		ptr = ptr->next;
	}
}

int InventoryList::SaveInventoryList(string filename)
{
	ofstream output(filename, ios::trunc);
	if (output)
	{
		Inventory *ptr = head;
		while (ptr != NULL)
		{
			ptr->SaveInventoryItem(output);
			ptr = ptr->next;
		}

		output.close();
	}
	else
	{
		return WRITEERROR;
	}
	return 0;
}

int InventoryList::ReadInventoryList(string filename)
{
	string inum, iname;
	ifstream infile(filename, ios::in);
	if (!infile)
	{
		cout << "File could not be opened for reading" << endl;
		return READERROR;
	}


	while (!infile.eof())
	{
		getline(infile, inum, '|');
		if (!inum.empty())
		{
			getline(infile, iname);
			Inventory *newnode = new Inventory(inum, iname);
			AddNodeToEnd(newnode);
			numrecords++;
		}
	}

	infile.close();
	return 0;
}


int main()
{
	string filename; //= "d:\\My Documents\\School\\Spring 2017\\INFO 450\\inventory2.txt";
	cout << "Please enter the filepath for your file: " << endl;
	getline(cin, filename);
	InventoryList myInventory;
	string answer;
	int error;
	error = myInventory.ReadInventoryList(filename);
	if (error)
	{
		cout << "Cannot read inventory - continue creating new list? Y/N -->";
		getline(cin, answer);
		if (answer != "Y" && answer != "y")
		{
			return 1;
		}
	}
	else
	{
		myInventory.ShowInventoryList();
	}
	myInventory.GetUserInput();
	myInventory.PlaySong();
	myInventory.SaveInventoryList(filename);
	myInventory.ShowInventoryList();

	return 0;
}