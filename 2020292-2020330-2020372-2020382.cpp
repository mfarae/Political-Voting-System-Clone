/****************************************************
Names(REG): Muhammad Farae(2020292), Muhammad Musa(2020330), Muhammad Umer Asif(2020372), Muhammed Hamza Malik(2020382)
Course/Section: CS221/B
Date: 9/January/2021
DSA PROJECT PRIMARY TASK-ELECTION MANAGEMENT SYSTEM
******************************************************/
/*****************************************************
 * In this project, instead of reading and writing individual data members to and from files, we used the method of writing and reading objects directly into the file
 * For this purpose we used the built in read and write functions in tandem with object-oriented class definitions for each object type
 * deletion was done by copying all the contents of the file into a new file except the particular record that had to be deleted, and deleting the original file
 *  renaming the new file with same name as the original file
 * updation was done through seek and tell functions to update the particular record that had to be changed. THey needed to be used since by the time the object to be 
 *  updated was discovered, the file traversal pointer had already moved further along to the next object, so the functions needed to be used move back the file stream
 * reading object by one object worth of size using the sizeof function and subtracting it from the current position of the file stream pointer.
 * **************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <stdio.h>
#include <conio.h>
using namespace std;

string election_commission_username = "p"; // login variables for election commission
string election_commission_password = "p";

void showmenu1() // showmenu function for the main procedure
{
    cout << "1: Election Commission Login" << endl;
    cout << "2: Political Party Login" << endl;
    cout << "3: Voter Login" << endl;
    cout << "4: Exit" << endl;
}

#define USER "p" // login information for starting the election management software application
#define PWD "p"

struct login
{
    string username, password;
    login(string u, string p)
    {
        username = u;
        password = p;
    }
};

void showmenu2()
{
    cout << "\n1: Register a National Assembly Constituency (e.g. Peshawar-1)" << endl;
    cout << "2: List all constituencies" << endl;
    cout << "3: Update/Delete Constituencies" << endl;
    cout << "4: Register Political Party" << endl;
    cout << "5: Register Candidate" << endl;
    cout << "6: Register Voter" << endl;
    cout << "7: Show Results" << endl;
    cout << "8: Display data of all files" << endl;
}
//------------------------------------------
class independent_candidate;
class party_candidate;
class constituency // class constituency that implements all the constituency registration, updation, deletion, etc
{
    private:
        char name[100];
    public:
        constituency(){}
        constituency(const constituency &c)
        {
            for (int i = 0; i < 100; i++)
            {
                name[i] = c.name[i];
            }
        }
        void set_con_data()
        {
            cout << "Enter name of constituency: ";
            cin >> name;
        }
        void store_con()
        {
            ofstream fout;
            fout.open("cons.txt", ios::app | ios::binary);
            fout.write((char*)this, sizeof(*this));
            fout.close();
        }
        void show_con_data()
        {
            cout << name;
        }
        void view_all_con()
        {
            ifstream fin;
            fin.open("cons.txt", ios::in | ios::binary);
            if(!fin)
            {
                cout << "File not found" << endl;
                return;
            }
            else
            {
                fin.read((char*)this, sizeof(*this));
                while(!fin.eof())
                {
                    cout << "\n" <<  name;
                    fin.read((char*)this, sizeof(*this));
                }
                fin.close();
                cout << endl;
            }
        }
        void delete_con(char* _name)
        {
            ifstream fin;
            ofstream fout;
            fin.open("cons.txt", ios::in | ios::binary);
            if (!fin)
            {
                cout << "File not found" << endl;
            }
            else
            {
                bool found = false;
                fout.open("tempfile.txt", ios::out | ios::binary);
                fin.read((char*)this, sizeof(*this));
                while(!fin.eof())
                {
                    if (strcmp(_name, name))
                    {
                        fout.write((char*)this, sizeof(*this));
                        found = true;
                    }
                    fin.read((char*)this, sizeof(*this));
                }
                fin.close();
                fout.close();
                remove("cons.txt");
                rename("tempfile.txt", "cons.txt");
                if (!found)
                    cout << "Constituency with the matching name was not found" << endl;
            }
        }
        void update_con(char *_name)
        {
	        fstream file;
	        file.open("cons.txt", ios::in | ios::out | ios::ate | ios::binary);
	        file.seekg(0); // starts the file pointer "0" bits from the start of the file. Required to start pointer from start of file since if the file already contains data, then you have to do this.
	        file.read((char*)this, sizeof(*this));
	        while(!file.eof())
	        {
	        	if (!strcmp(_name, name))
	        	{
	        		set_con_data();
	        		file.seekp(file.tellp()-sizeof(*this)); // seekp rearranges the file traversal pointer to a point before the object
                    cout << this->name << endl;
	        		file.write((char*)this, sizeof(*this));
	        	}
	        	file.read((char*)this, sizeof(*this));
	        }
	        file.close();
        }
        friend independent_candidate;
        friend party_candidate;     
};
//------------------------------------------------
//-------------------------------------------------

int* party_candidate_id = new int[0]; // globally declared dynamic array for sorting
int array_size = 0;
// class to define political parties such as PTI, PPP, Noon_League
class political_party
{
    private:
        char name[100], username[100], password[100], symbol_name[100];
        int symbol_id;

    public:
        void set_party_data()
        {
            char temp[100];
            cout << "Enter name of party: ";
            cin >> name;
            cout << "Enter symbol name of party: ";
            cin >> symbol_name;
            cout << "Enter three-digit symbol id (Please note that your symbol_id will be used as your username for login): ";
            cin >> temp;
            for (int i = 0; i < strlen(temp); i++)
                username[i] = temp[i];
            symbol_id = atoi(temp);
            cout << "Enter password: ";
            cin >> password;
        }
        void show_party_name()
        {
            cout << "\nName of Party: " << name;
            cout << "\nSymbol Name: " << symbol_name;
            cout << "\nSymbol ID: " << symbol_id;
            cout << "\nUsername: " << username;
            cout << "\nPassword: " << password << "\n";
        }
        void store_party()
        {
            ofstream fout;
            fout.open("party.txt", ios::app | ios::binary);
            fout.write((char*)this, sizeof(*this));
            fout.close();
        }
        void view_all_party()
        {
            ifstream fin;
            fin.open("party.txt", ios::in | ios::binary);
            if(!fin)
            {
                cout << "No political parties have been registered yet for the party candidate to choose from" << endl;
                return;
            }
            else
            {
                fin.read((char*)this, sizeof(*this));
                while(!fin.eof())
                {
                    show_party_name();
                    fin.read((char*)this, sizeof(*this));
                }
                fin.close();
            }
        }
        bool party_login(int symbolid)
        {
            ifstream fin;
            fin.open("party.txt", ios::in | ios::binary);
            if (!fin)
            {
                cout << "no party has been registered yet" << endl;
                return false;
            }
            else
            {             
                fin.read((char*)this, sizeof(*this));
                while(!fin.eof())
                {
                    if(symbolid == this->symbol_id)
                    {
                        cout << "Login Successful. Welcome " << this->name << " Party" <<endl;
                        fin.close();
                        return true;
                    }
                    fin.read((char*)this, sizeof(*this));
                }
                fin.close();
                return false;
            }
        }
};
//------------------------------------------------
//-----------------------------------------------
// voter class that deals with all voter login related information and also their registration
class voter
{
    private:
        long long int CNIC;
        char password[100], cons[100]; // only one constituency variable, so only one name has to be stored

        int vote_id = 0;

    public:
        void set_voter()
        {
            cout << "Enter 13-digit CNIC of voter: ";
            cin >> CNIC;
            cout << "Enter to set your account password: ";
            cin >> password;
            constituency temp;
            temp.view_all_con();
            cout << "\nSelect constituency from the list above: ";
            cin >> cons;
        }
        void store_voter()
        {
            ofstream fout;
            fout.open("voter.txt", ios::app | ios::binary);
            fout.write((char*)this, sizeof(*this));
            fout.close();
        }
        void show_voter()
        {
            cout << "\nVoter CNIC: " << CNIC;
            cout << "\nConstituency: " << cons;
            cout << "\nPassword: " << password;
            cout << "\nVoted for: " << vote_id << endl;
        }
        void view_all_voter()
        {
            ifstream fin;
            fin.open("voter.txt", ios::in | ios::binary);
            if(!fin)
            {
                cout << "No voters have been registered yet" << endl;
                return;
            }
            else
            {
                fin.read((char*)this, sizeof(*this));
                while(!fin.eof())
                {
                    show_voter();
                    fin.read((char*)this, sizeof(*this));
                }
                fin.close();
            }
        }
        bool voter_login(long long int cnic, char* pwd)
        {
            ifstream fin;
            fin.open("voter.txt", ios::in | ios::binary);
            if (!fin)
            {
                cout << "no voter has been registered yet" << endl;
                return false;
            }
            else
            {             
                fin.read((char*)this, sizeof(*this));
                while(!fin.eof())
                {
                    if((cnic == this->CNIC) && !strcmp(pwd, this->password))
                    {
                        cout << "Login Successful. Welcome " << this->CNIC <<endl;
                        fin.close();
                        return true;
                    }
                    fin.read((char*)this, sizeof(*this));
                }
                fin.close();
                return false;
            }
        }
        void cast_vote(long long int cnic) // long long int used for cnic as otherwise program crashes as it cannot handle 13-digit number with simple int
        {
            int voted;
            cout << "Using the candidate IDs that have been sorted specifically from your constituency, enter the ID that you would like to vote for: ";
            cin >> voted;
            fstream file;
	        file.open("voter.txt", ios::in | ios::out | ios::ate | ios::binary);
	        file.seekg(0);
	        file.read((char*)this, sizeof(*this));
	        while(!file.eof())
	        {
	        	if (cnic == this->CNIC)
	        	{
	        		this->vote_id = voted;
                    this->show_voter();
	        		file.seekp(file.tellp()-sizeof(*this));
	        		file.write((char*)this, sizeof(*this));
	        	}
	        	file.read((char*)this, sizeof(*this));
	        }
	        file.close();
        }
        bool id_checker(int vote_id)
        {
            bool id_match = false;
            for (int i = 0; i < array_size; i++)
            {
                if (vote_id == party_candidate_id[i])
                    id_match = true;
            }
            return id_match;
        }
        void display_votes()
        {
            int count = 0;
            ifstream fin;
            fin.open("voter.txt", ios::in | ios::binary);
            if (!fin)
            {
                cout << "no voter has been registered yet" << endl;
                return;
            }
            fin.read((char*)this, sizeof(*this));
            while(!fin.eof())
            {
                if(id_checker(this->vote_id))
                {
                    cout << "Voter with cnic: " << this->CNIC << " voted for " << this->vote_id << endl;
                    count++;
                }
                fin.read((char*)this, sizeof(*this));
            }
            fin.close();
            cout << "The selected political party received " << count << " number of votes if all the votes given to their candidates are added up" << endl;
        }
        void view_all_results()
        {
            ifstream fin;
            fin.open("voter.txt", ios::in | ios::binary);
            if (!fin)
            {
                cout << "no voter has been registered yet" << endl;
                return;
            }
            fin.read((char*)this, sizeof(*this));
            while(!fin.eof())
            {
                cout << "Voter with cnic: " << this->CNIC << " voted for " << this->vote_id << endl;
                fin.read((char*)this, sizeof(*this));
            }
            fin.close();
        }
        void view_constituency_results(char* selected_constituency)
        {
            ifstream fin;
            fin.open("voter.txt", ios::in | ios::binary);
            if (!fin)
            {
                cout << "no voter has been registered yet" << endl;
                return;
            }
            fin.read((char*)this, sizeof(*this));
            while(!fin.eof())
            {
                if (!strcmp(selected_constituency, this->cons))
                {
                    cout << "Voter with cnic: " << this->CNIC << " voted for " << this->vote_id << endl;
                }
                fin.read((char*)this, sizeof(*this));
            }
            fin.close();
        }
};
class party_candidate
{
    private:
        char name[100];
        int reg_approved = 0; // 0 if reg is pending, will be changed to 1 if party authorizes
        char party[100], party_symbol[100]; // add data member party_id
        char cons[100];
        int symbol_id, unique_id;

        int num_of_votes = 0;
        
    public:
        party_candidate(){} // copy constructors were defined to faciliatate name sorting
        party_candidate(const party_candidate &p)
        {
            reg_approved = p.reg_approved;
            unique_id = p.unique_id;
            symbol_id = p.symbol_id;
            for (int i = 0; i < strlen(p.name); i++)
                name[i] = p.name[i];
            for (int i = 0; i < strlen(p.party); i++)
                party[i] = p.party[i];
            for (int i = 0; i < strlen(p.party_symbol); i++)
                party_symbol[i] = p.party_symbol[i];
            for (int i = 0; i < strlen(p.cons); i++)
                cons[i] = p.cons[i];
        }
        void set_party_candidate_data()
        {
            cout << "Enter political party name from the list above: ";
            cin >> party;
            cout << "Enter their matching symbol_name: ";
            cin >> party_symbol;
            cout << "Enter their matching symbol_id: ";
            cin >> symbol_id;
            cout << "Enter your name: ";
            cin >> name;
            cout << "Enter unique candidate id: ";
            cin >> unique_id;

            constituency temp; // ------------>> call view all func of constituency and get all inputs, then try running and debug, they move on to independent candidates class
            temp.view_all_con();
            cout << "Select constituency from the list above: ";
            cin >> cons;
        }
        void store_party_candidate()
        {
            ofstream fout;
            fout.open("party_candidate.txt", ios::app | ios::binary);
            fout.write((char*)this, sizeof(*this));
            fout.close();
        }
        void show_party_candidate()
        {
            cout << "\nParty Candidate Name: " << name;
            cout << "\nConstituency: " << cons;
            cout << "\nMember of Party: " << party << " which has symbol " << party_symbol;
            cout << "\nSymbol ID: " << symbol_id;
            cout << "\nCandidate ID: " << unique_id;
            if (reg_approved == 1)
            {
                cout << "\nRegistraion status: Approved by party" << endl;
            }
            else
            {
                cout << "\nRegistration status: Pending approval by party" << endl;
            }
        }
        void view_all_party_candidate()
        {
            ifstream fin;
            fin.open("party_candidate.txt", ios::in | ios::binary);
            if(!fin)
            {
                cout << "No political party candidates have been registered yet" << endl;
                return;
            }
            else
            {
                fin.read((char*)this, sizeof(*this));
                while(!fin.eof())
                {
                    show_party_candidate();
                    fin.read((char*)this, sizeof(*this));
                }
                fin.close();
            }
        }
        void display_all_unapproved(char *_name) 
        {
            ifstream fin;
            fin.open("party_candidate.txt", ios::in | ios::binary);
            if(!fin)
            {
                cout << "File not found" << endl;
                return;
            }
            else                            
            {
                fin.read((char*)this, sizeof(*this));
                while(!fin.eof())
                {
                    if( (!strcmp(_name, party)) && !reg_approved)
                    {
                        this->show_party_candidate();
                    }
                    fin.read((char*)this, sizeof(*this));
                }
            }
            fin.close();
        }
        void approve_reg(char* name)
        {
            fstream file;
            file.open("party_candidate.txt", ios::in | ios::out | ios::ate | ios::binary);
            file.seekg(0);
            file.read((char*)this, sizeof(*this));
            while(!file.eof())
            {
                if (!strcmp(name, this->name))
                {
                    this->reg_approved = 1;
                    cout << this->symbol_id << endl;
                    cout << this->unique_id << endl;
                    cout << this->cons << endl;
                    cout << this->name << endl;
                    cout << this->party << endl;
                    cout << this->party_symbol << endl;
                    file.seekp(file.tellp()-sizeof(*this));
                    file.write((char*)this, sizeof(*this));
                }
                file.read((char*)this, sizeof(*this));
            }
            file.close();
        }
        void approve_request(char *party_name)
        {
            cout << "The following candidates are pending approval\n";
            while(1)
            {
                display_all_unapproved(party_name);
                char can_name[100];
                cout << "\nEnter the name of candidate you wish to approve: ";
                cin >> can_name;
                approve_reg(can_name);
                int choice;
                cout << "\nWould you like to approve another candidate? 1(yes), 2(no)\n";
                cin >> choice;
                if (choice == 2)
                    break;
            }
        }
        bool unique(char*);
        // void sortbyname(char* cons) // correct logic to sort the candidates by names, but faces some bugs as file reading in c++ through objects is highly unpredictable
        // {
        //     int i = 0;
        //     ifstream fin;
        //     fin.open("party_candidate.txt", ios::in | ios::binary);
        //     if (!fin)
        //     {
        //         cout << "No party candidates are registered" << endl;
        //         return;
        //     }
        //     party_candidate *dyn_arr = new party_candidate[0];
        //     fin.read((char*)this, sizeof(*this));
        //     while(!fin.eof())
        //     {
        //         if (this->reg_approved == 1 && !strcmp(this->cons, cons))
        //         {
        //             for (int a = 0; a < strlen(this->party_symbol); a++)
        //                 dyn_arr[i].party_symbol[a] = this->party_symbol[a];
        //             i++;
        //         }
        //         fin.read((char*)this, sizeof(*this));
        //     }
        //     fin.close();
        //     char temp[100];
        //     for (int j = 0; j <= i; j++)
        //     {
        //         for (int k = 1; k <= i; k++)
        //         {
        //             if (strcmp(dyn_arr[k].party_symbol, dyn_arr[k-1].party_symbol) < 0)
        //             {
        //                 for (int a = 0; a < strlen(dyn_arr[k].party_symbol); a++)
        //                     temp[a] = dyn_arr[k].party_symbol[a];
        //                 for (int a = 0; a < strlen(dyn_arr[k-1].party_symbol); a++)
        //                     dyn_arr[k].party_symbol[a] = dyn_arr[k-1].party_symbol[a];
        //                 for (int a = 0; a < strlen(temp); a++)
        //                     dyn_arr[k-1].party_symbol[a] = temp[a];
        //             }
        //         }
        //     }
        //     for (int j = 0; j <= i; j++)
        //     {
        //         cout << dyn_arr[j].party_symbol;
        //         cout << endl;
        //     }
        //     delete [] dyn_arr;
        //}
        // sorting by id
        void sort(char* cons)
        {
            int index = 0;
            ifstream fin;
            fin.open("party_candidate.txt", ios::in | ios::binary);
            if (!fin)
            {
                cout << "No party candidates are registered" << endl;
                return;
            }
            int* arr = new int[0];
            fin.read((char*)this, sizeof(*this));
            while(!fin.eof())
            {
                if (this->reg_approved == 1 && !strcmp(cons, this->cons))
                {
                    arr[index++] = (this->unique_id);
                }
                fin.read((char*)this, sizeof(*this));
            }
            fin.close();
            if (index != 1)
            {
                for (int i = 0; i <= index; i++)
                {
                    for (int j = 1; j <= index; j++)
                    {
                        if (arr[j] < arr[j-1])
                        {
                            int temp = arr[j];
                            arr[j] = arr[j-1];
                            arr[j-1] = temp;
                        }
                    }
                }
            }
            for (int i = 0; i < index; i++)
                cout << arr[i] << endl;
            delete [] arr;
        }

        void view_results()
        {
            char partyname[100];
            cout << "Enter the name of the political party whose voting results you would like to view: ";
            cin >> partyname;
            ifstream fin;
            fin.open("party_candidate.txt", ios::in | ios::binary);
            if (!fin)
            {
                cout << "no party candidates have been registered yet" << endl;
                return;
            }
            else
            {             
                int index = 0;
                fin.read((char*)this, sizeof(*this));
                while(!fin.eof())
                {
                    if(!strcmp(partyname, this->party))
                    {
                        party_candidate_id[index++] = this->unique_id;
                        array_size++;
                    }
                    fin.read((char*)this, sizeof(*this));
                }
                fin.close();

                voter temp;
                temp.display_votes();
                for (int i = 0; i < array_size; i++)
                {
                    party_candidate_id[i] = 0;
                }
                array_size = 0;
                return;
            }
        }
};
//---------------------------------------------------------------------------
bool party_candidate::unique(char* name)
{
    ifstream file;
    file.open("party_candidate.txt", ios::in | ios::binary);
    if(!file)
    {
        cout << "No party candidates have been registered yet" << endl;
        return true;
    }
    else
    {
        file.read((char*)this, sizeof(*this));
        while(!file.eof())
        {
            if (!strcmp(name, this->party_symbol))
            {
                file.close();
                return false;
            }
            file.read((char*)this, sizeof(*this));
        }
        file.close();
        return true;
    }
}
class independent_candidate
{
    private:
        char name[100], symbol_name[100];
        int symbol_id, id;
        char cons[100];

        int num_of_votes = 0;
    public:
        bool set_independent_candidate()
        {
            cout << "Enter the independent candidate's name: ";
            cin >> name;
            cout << "Enter name of independent candidate's symbol: ";
            cin >> symbol_name;
            party_candidate temp;
            if (!temp.unique(symbol_name))
            {
                cout << "Unique symbol name was not entered. Try again." << endl;
                return false;
            }
            cout << "Enter the corresponding symbol's id: ";
            cin >> symbol_id;
            cout << "Enter Candidate ID ";
            cin >> id;
            constituency tmp;
            tmp.view_all_con();
            cout << "Which constituency would you like to register for: ";
            cin >> cons;
            cout << "\n";
            return true;
        }
        void store_independent_candidate()
        {
            ofstream fout;
            fout.open("ind_candidate.txt", ios::app | ios::binary);
            fout.write((char*)this, sizeof(*this));
            fout.close();
        }
        void show_independent_candidate()
        {
            cout << "\nCandidate Name: " << name;
            cout << "\nSymbol name: " << symbol_name;
            cout << "\nRegistered for constituency: " << cons;
            cout << "\nSymbol id: " << symbol_id;
            cout << "\nCandidate id: " << id;
            cout << endl;
        }
        void view_all_independent_candidate()
        {
            ifstream fin;
            fin.open("ind_candidate.txt", ios::in | ios::binary);
            if(!fin)
            {
                cout << "No independent candidates have been registered yet" << endl;
                return;
            }
            else
            {
                fin.read((char*)this, sizeof(*this));
                while(!fin.eof())
                {
                    show_independent_candidate();
                    fin.read((char*)this, sizeof(*this));
                }
                fin.close();
            }
        }
        void sort(char* cons)
        {
            int index = 0;
            ifstream fin;
            fin.open("ind_candidate.txt", ios::in | ios::binary);
            if (!fin)
            {
                cout << "No independent candidates are registered" << endl;
                return;
            }
            int* arr = new int[0];
            fin.read((char*)this, sizeof(*this));
            while(!fin.eof())
            {
                if (!strcmp(cons, this->cons))
                {
                    arr[index++] = (this->symbol_id);
                }
                fin.read((char*)this, sizeof(*this));
            }
            fin.close();
            for (int i = 0; i <= index; i++) // bubblesort
            {
                for (int j = 1; j <= index; j++)
                {
                    if (arr[j] < arr[j-1])
                    {
                        int temp = arr[j];
                        arr[j] = arr[j-1];
                        arr[j-1] = temp;
                    }
                }
            }
            for (int i = 0; i < index; i++)
                cout << arr[i] << endl;
            delete [] arr;
        }
};
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// this function deals with all the user functionality required by the election commission
void election_commission()
{
    char name[100], name2[100];
    int choice;
    showmenu2();
    cin >> choice;
    //system("cls");
    constituency con;
    political_party pol;
    party_candidate party_can;
    independent_candidate ind_can;
    voter vot;
    switch (choice)
    {
        case 1:
        {
            con.set_con_data();
            con.store_con();
            cout << "Constituency saved" << endl;
            break;
        }
        case 2:
        {
            con.view_all_con();
            break;
        }
        case 3: // update and delete
        {
            int choice;
            cout << "Enter 1 for delete, 2 for update: ";
            cin >> choice;
            if (choice == 1)
            {
                cout << "Enter name of constituency to delete: ";
                cin.ignore();
                cin.getline(name, 99);
                con.delete_con(name);
                cout << "File deleted" << endl;
            }
            else if (choice == 2)
            {
                cout << "Enter name of constituency that will be updated: ";
                cin.ignore();
                cin.getline(name, 19);
                con.update_con(name);
            }
            else
            {
                cout << "Please enter a valid selection key" << endl;
                break;
            }
            break;
        }
        case 4:
        {
            pol.set_party_data();
            pol.store_party();
            pol.view_all_party();//del
            cout << "Party registered" << endl;
            break;
        }
        case 5:
        {
            int choice;
            cout << "Enter 1 to register party candidate, 2 for independent: ";
            cin >> choice;
            if (choice == 1)
            {
                pol.view_all_party();
                party_can.set_party_candidate_data();
                party_can.store_party_candidate();
                cout << "party candidate's information has been stored" << endl;
            }
            else if (choice == 2)
            {
                if(ind_can.set_independent_candidate())
                {
                    ind_can.store_independent_candidate();
                    cout << "independent candidate's information has been stored" << endl;
                }
            }
            else
            {
                cout << "Please enter a valid selection key" << endl;
                break;
            }
            break;
        }
        case 6:
        {
            vot.set_voter();
            vot.store_voter();
            break;
        }
        case 7:
        {
            int user_choice;
            cout << "View results of all constituencies (1) or a particular constituency (2)?: ";
            cin >> user_choice;
            if (user_choice == 1)
            {
                vot.view_all_results();
            }
            else if (user_choice == 2)
            {
                char selected_cons[100];
                cout << "Enter the name of the constituency you would like to display votes for: ";
                cin >> selected_cons;
                vot.view_constituency_results(selected_cons);
            }
            break;
        }
        case 8:
        {
            cout << "\n\nAll constituencies: " << endl;
            con.view_all_con();
            cout << "\n\nAll political parties: " << endl;
            pol.view_all_party();
            cout << "\n\nAll political party affiliated candidates: " << endl;
            party_can.view_all_party_candidate();
            cout << "\n\nAll independent candidates: " << endl;
            ind_can.view_all_independent_candidate();
            cout << "\n\nAll voters: " << endl;
            vot.view_all_voter();

            cout << "\n\n\n\nPress any key to return to the previous menus" << endl;
            getch();
            break;
        }
        default:
        {
            cout << "Invalid choice. Try Again" << endl;
            break;
        }
    }
}
// function to implement login functionality of election commission
bool election_commission_login()
{
    string user;
    string pwd;
    cout << "Enter your username which shouldn't contain any special characters and whitespaces: ";
    cin >> user;
    cout << "Enter your password which shouldn't contain any special characters and whitespaces: ";
    cin >> pwd;
    
    bool login = (user == election_commission_username && pwd == election_commission_password) ? true : false; 
    return login;
}
//-----------------------------------------------------------------------------------------------------------
void showmenu3()
{
    cout << "\n1: Approve/reject request of ticket" << endl;
    cout << "2: Check results after voting is done (won/lost)\n"; 
}
// party login procedure
void party_procedure()
{
    int choice;
    showmenu3();
    cin >> choice;
    switch(choice)
    {
        case 1:
        {
            party_candidate par_can;
            char name[100];
            cout << "Enter party name: ";
            cin >> name;
            par_can.approve_request(name);
            break;
        }
        case 2:
        {
            party_candidate temp;
            temp.view_results();
            break;
        }
        default:
        {
            cout << "Invalid choice, try again" << endl;
            break;
        }
    }
}
//----------------------------------------------------------------------------------------------------------
// voter login procedures are defined below
void showmenu4()
{
    cout << "\n1: Sort all party candidates with the same constituency as voter" << endl;
    cout << "2: Sort all independent candidates with the same constituency as voter" << endl;
    cout << "3: Cast Vote to candidates of your own constituency" << endl; 
}
void voter_procedure(long long int cnic)
{
    int choice;
    showmenu4();
    cin >> choice;
    constituency temp;
    char cons[100];
    switch(choice)
    {
        case 1:
        {
            temp.view_all_con();
            cout << "Enter your constituency: ";
            cin >> cons;
            party_candidate temp;
            temp.sort(cons);
            break;
        }
        case 2:
        {
            temp.view_all_con();
            cout << "Enter your constituency: ";
            cin >> cons;
            independent_candidate temp;
            temp.sort(cons);
            break;
        }
        case 3:
        {
            voter temp;
            temp.cast_vote(cnic);
            break;
        }
        default:
        {
            cout << "Invalid choice, try again" << endl;
            break;
        }
    }
}
// the main menu of the program implemented in int main()
int main()
{
    system("cls");
    string password, username;
    cout << "Enter user login credentials to access the software: ";
    cin >> username >> password;

    if ((username == USER) && (password == PWD))
    {
        cout << "Welcome to the Election Management System. Please select your login options from the menu below" << endl;
        int selection;
        system("cls");
        do
        {
            showmenu1();
            cin >> selection;
            switch (selection)
            {
                case 1:
                {
                    if (!election_commission_login())
                    {
                        cout << "The login information is incorrect, please select the option and try again" << endl;
                        break;
                    }
                    election_commission();
                    break;
                }
                case 2:
                {
                    int symbol_id;
                    cout << "Enter your party symbol id: ";
                    cin >> symbol_id;
                    political_party temp;
                    if(temp.party_login(symbol_id))
                    {
                        party_procedure();
                    }
                    break;
                }
                case 3:
                {
                    long long int cnic;
                    char pwd[100];
                    cout << "Enter your voter CNIC as username: ";
                    cin >> cnic;
                    cout << "Enter your password: ";
                    cin >> pwd;
                    voter temp;
                    if (temp.voter_login(cnic, pwd))
                    {
                        voter_procedure(cnic);
                    }
                    else{
                        cout << "Incorrect login info, try again" << endl;
                    }
                    break;
                }
                case 4:
                {
                    cout << "Thank you for using the election management system" << endl;
                    exit(-1);
                    break;
                }
                default:
                {
                    cout << "Please enter a correct selection option" << endl;
                }
            }
        }while(1);
    }
    return 0;
}
