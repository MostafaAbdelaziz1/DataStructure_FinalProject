#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/// Client structure
struct Client {
    int id;
    string name;
    string currency;
    string password;
    double balance;
    Client* next;
    Client* previous;

    Client(int id, string name, string currency, string password, double balance) :
        id(id),
        name(name),
        currency(currency),
        password(password),
        balance(balance),
        next(nullptr),
        previous(nullptr)
    {}
};

/// Priority queue node
struct PQNode {
    string name;
    int rating;
    PQNode* next;

    PQNode(string name, int rating) :
        name(name),
        rating(rating),
        next(nullptr)
    {}
};

/// Bank class
class Bank {
private:
    Client* clients_head;
    int num_clients;
    PQNode* ratings_head;

public:
    Bank() {
        clients_head = nullptr;
        ratings_head = nullptr;
        num_clients = 0;
    }

    void run() {
        bool isRunning = true;
        while (isRunning) {
            system("cls");
            cout << "\n----------------------------------------------" << endl;
            cout << "   Welcome to the Banking Management System" << endl;
            cout << "----------------------------------------------" << endl;
            cout << "\n\t1. Admin Login" << endl;
            cout << "\t2. Client Login" << endl;
            cout << "\t3. Exit" << endl;
            cout << "\n\tEnter your choice: ";
            int choice;
            cin >> choice;

            switch (choice) {
                case 1:
                    admin_login();
                    break;
                case 2:
                    client_login();
                    break;
                case 3:
                    isRunning = false;
                    break;
                default:
                    cout << "Invalid choice." << endl;
                    break;
            }
        }
    }

/// Admin Login
    void admin_login() {
        system("cls");
        cout << "\n-------------------------" << endl;
        cout << "   Administration Page" << endl;
        cout << "-------------------------" << endl;

        int id;
        string password;

        cout << "\nEnter Admin ID: ";
        cin >> id;
        cout << "Enter password: ";
        cin >> password;

        if(id == 0 && password == "admin123") {
            admin_menu();
        } else {
            cout << "Invalid login.\n" << endl;
        }
    }

    void admin_menu() {
        system("cls");
        bool isLoggedIn = true;

        while (isLoggedIn) {
            cout << "\n-------------------------" << endl;
            cout << "   Administration Page" << endl;
            cout << "-------------------------" << endl;
            cout << "\n\t1. Add Client" << endl;
            cout << "\t2. Search Client" << endl;
            cout << "\t3. List Clients" << endl;
            cout << "\t4. Edit Client Information" << endl;
            cout << "\t5. Remove Client" << endl;
            cout << "\t6. Display Ratings" << endl;
            cout << "\t7. Logout" << endl;
            cout << "\n\t\tEnter choice: ";
            int choice;
            cin >> choice;

            switch (choice) {
                case 1:
                    add_client();
                    break;
                case 2:
                    search_client();
                    break;
                case 3:
                    list_clients();
                    break;
                case 4:
                    edit_client_info();
                    break;
                case 5:
                    remove_client_info();
                    break;
                case 6:
                    display_ratings();
                    break;
                case 7:
                    cout << "Logged out." << endl;
                    isLoggedIn = false;
                    break;
                default:
                    cout << "Invalid choice." << endl;
                    break;
            }
        }
    }

    void add_client() {

        int id;
        string name, password, currency;
        double balance;

        cout << " Enter client ID: ";
        cin >> id;

        if (find_client(id) != nullptr) {
            cout << "Client with ID " << id << " already exists." << endl;
            return;
        }
        cout << "Enter the client's name: ";
        cin >> name;
        cout << "Enter client's password: ";
        cin >> password;
        cout << "Enter initial client balance: ";
        cin >> balance;
        cout << "Enter currency type(USD/EGP): ";
        cin >> currency;

        bool validCurrency = false;
        for (const string& validCurrencyType : { "USD", "EGP" }) {
            if (currency == validCurrencyType) {
                validCurrency = true;
                break;
            }
        }

        if (!validCurrency) {
            cout << "Invalid currency, try again\n";
            return add_client();
        }

        Client* client = new Client(id, name, currency, password, balance);

        // Add the new client to the client list
        if (clients_head == nullptr) {
            clients_head = client;
        } else {
            Client* current = clients_head;
            while (current->next != nullptr) {
                current = current->next;
            }
            // Set the next node of the last node to the new client
            current->next = client;
            client->previous = current;
        }
        num_clients++;
        cout << "Client added successfully!\n" << endl;
    }

    void search_client() {
        int id;
        cout << "Enter client ID: ";
        cin >> id;

        Client* client = find_client(id);
        if (client == nullptr) {
            cout << "Client not found.\n" << endl;
        } else {
            cout << "\n*Client found*" << endl;
            cout << "ID: " << client->id << endl;
            cout << "Name: " << client->name << endl;
            cout << "Balance: " << client->balance <<" "<< client->currency << endl;
        }
    }

    void list_clients() {
        if (num_clients == 0) {
            cout << "No clients found.\n" << endl;
        } else {
            cout << "List of clients:" << endl;
            Client* current = clients_head;
            while (current != nullptr) {
                cout << "-------------------------" << endl;
                cout << "ID: " << current->id << endl;
                cout << "Name: " << current->name << endl;
                cout << "Balance: " << current->balance <<" "<< current->currency << endl;
                cout << "-------------------------" << endl;
                current = current->next;
            }
        }
    }

    void edit_client_info() {
        int id;
        cout << "Enter client ID: ";
        cin >> id;

        Client* client = find_client(id);
        if (client == nullptr) {
            cout << "Client not found." << endl;
        } else {
            cout << "Enter new client name: ";
            cin >> client->name;
            cout << "Enter new client password: ";
            cin >> client->password;
            cout << "Client information updated!\n" << endl;
        }
    }

    void remove_client_info() {
        int id;
        cout << "Enter client ID: ";
        cin >> id;

        Client* client = find_client(id);
        if (client == nullptr) {
            cout << "Client not found." << endl;
        } else {
            remove_client_rating(client->name);

            // If the client to be removed is the head of the list
            if (client == clients_head) {
                clients_head = client->next;
                if (clients_head != nullptr) {
                    clients_head->previous = nullptr;
                }
            } else {
                // If the client is not the head, update the next and previous pointers of the adjacent nodes
                if (client->previous != nullptr) {
                    client->previous->next = client->next;
                }
                if (client->next != nullptr) {
                    client->next->previous = client->previous;
                }
            }
        delete client;
        num_clients--;
        cout << "Client removed successfully!\n" << endl;
        }
    }

    void remove_client_rating(const string& name) {
        // Traverse the ratings list to find the client
        PQNode* current = ratings_head;
        PQNode* previous = nullptr;

        // Search for the client in the ratings list
        while (current != nullptr) {
            if (current->name == name) {
                // If the node to be removed is the head of the list
                if (current == ratings_head) {
                    ratings_head = current->next;
                } else {
                    previous->next = current->next;
                }
                delete current;
                break;
            }

            previous = current;
            current = current->next;
        }

        // If the client is not found in the ratings list, display an error message
        if (current == nullptr) {
            cout << "Client '" << name << "' does not have a rating." << endl;
        }
    }



    Client* find_client(int id) {
        Client* current = clients_head;
        while (current != nullptr) {
            if (current->id == id) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

// Client Login
    void client_login() {
        int id;
        string password;
        cout << "Enter client ID: ";
        cin >> id;
        cout << "Enter password: ";
        cin >> password;

        Client* client = find_client(id);
        if (client != nullptr && client->password == password) {
            client_menu(client);
        } else {
            cout << "Invalid client ID or password." << endl;
        }
    }

    void client_menu(Client* client) {
        bool isLoggedIn = true;
        while (isLoggedIn) {

            cout << endl;
            cout << "   *" << client->name <<"*  " << endl;
            cout << "----------------" << endl;
            cout << "1. Deposit" << endl;
            cout << "2. Withdraw" << endl;
            cout << "3. Check balance" << endl;
            cout << "4. Transfer money" << endl;
            cout << "5. Write Feedback" << endl;
            cout << "6. Logout" << endl;
            cout << "Enter choice: ";
            int choice;
            cin >> choice;

            switch (choice) {
                case 1:
                    deposit(client);
                    break;
                case 2:
                    withdraw(client);
                    break;
                case 3:
                    check_balance(client);
                    break;
                case 4:
                    transfer_money(client);
                    break;
                case 5:
                    take_feedback(client);
                    break;
                case 6:
                    cout << "Logged out." << endl;
                    isLoggedIn = false;
                    break;
                default:
                    cout << "Invalid choice." << endl;
                    break;
            }
        }
    }

    void deposit(Client* client) {
        double amount;
        cout << "Enter amount to deposit: ";
        cin >> amount;
        client->balance += amount;
        cout << "Deposit successful! \nNew balance: " << client->balance <<" "<< client->currency << endl;
    }

    void withdraw(Client* client) {
        double amount;
        cout << "Enter amount to withdraw: ";
        cin >> amount;
        if (amount > client->balance) {
            cout << "Insufficient balance." << endl;
        } else {
            client->balance -= amount;
            cout << "Withdrawal successful! \nNew balance: " << client->balance <<" "<< client->currency << endl;
        }
    }

     void check_balance(Client* client) {
            char charYN;
            int choise, eur, kwd, omr;
            cout << "YOUR BALANCE IS:" << client->balance <<" "<< client->currency << endl;
            cout << "\nDo you want to view your balance by other currencies? (y/n)";
            cin >> charYN;
            string user_currency = client->currency;
            if (charYN == 'y')
            {
                cout << "Choose the currency \n";
                cout << "1.EURO (EUR)\n";
                cout << "2.Kuwaiti dinar (KWD) \n";
                cout << "3.Omani rial (OMR)\n";
                cout << "Enter choice: ";
                cin >> choise;
                switch (choise)
                {
                case 1:
                    if (user_currency == "USD") {
                        eur = client->balance * 0.93;
                        cout << "Your balance by EURO is: " << eur;
                        break;
                    }
                    else {
                        eur = client->balance * 0.033;
                        cout << "Your balance by EURO is " << eur;
                        break;
                    }

                case 2:
                    if (user_currency == "USD") {
                        kwd = client->balance * 0.31;
                        cout << "Your balance in Kuwaiti dinar is: " << kwd;
                        break;
                    }
                    else {
                        kwd = client->balance * 0.01;
                        cout << "Your balance in Kuwaiti dinar is " << kwd;
                        break;
                    }

                case 3:
                    if (user_currency == "USD") {
                        omr = client->balance * 0.39;
                        cout << "Your balance in Omani rial is: " << omr;
                        break;
                    }
                    else {
                        omr = client->balance * 0.012;
                        cout << "Your balance by Omani rial is " << omr;
                        break;
                    }

                default:
                    cout << "Wrong choose please try again";
                }
            }

            else if (charYN == 'n') {
                cout << "Glad to help you";
            }
            else {
                cout << "Wrong choice try again";
            }
        }

    void transfer_money(Client* client) {
        int receive_id;
        double namount, bamount;

        cout << "Enter recipient ID: ";
        cin >> receive_id;
        Client* recipient = find_client(receive_id);
        if (recipient == nullptr) {
            cout << "Invalid recipient ID." << endl;
            return;
        }

        cout << "Enter amount to transfer: ";
        cin >> bamount;
        if (bamount > client->balance) {
            cout << "Not enough balance" << endl;
            return;
        }

        string user_currency = client->currency;
        string recipient_currency = recipient->currency;

        if (user_currency != recipient_currency) {
            double conversion_rate = 1;
            if (user_currency == "USD") {
                conversion_rate = 31.90;
            }
            else if (recipient_currency == "USD") {
                conversion_rate = 1 / 31.90;
            }

            namount = bamount * conversion_rate;
            client->balance -= bamount;
            recipient->balance += namount;
        }

        else {
            client->balance -= bamount;
            recipient->balance += bamount;
        }
        cout << "Transfer successful! \nCurrent balance: " << client->balance <<" "<< client->currency << endl;
    }

    // Function to take client feedback and rating
    void take_feedback(Client* client) {
        string feedback;
        int rating;

        cout << "Enter your feedback: ";
        cin.ignore();
        getline(cin, feedback);

        cout << "Rate your experience (out of 10): ";
        cin >> rating;

        // Store the feedback and rating in a file
        ofstream file("feedback.txt", ios::app);
        file << "Client ID: " << client->id << endl;
        file << "Name: " << client->name << endl;
        file << "Feedback: " << feedback << endl;
        file << "Rating: " << rating << endl;
        file << "--------------------------" << endl;
        file.close();

        // Store the client's name and rating in the queue if rating is under 6
        if (rating < 6) {
        insert_rating(client->name, rating);
        }
    }

    void insert_rating(string name, int rating) {
        PQNode* newRating = new PQNode(name, rating);

        if (ratings_head == nullptr || rating < ratings_head->rating) {
            newRating->next = ratings_head;
            ratings_head = newRating;
        } else {
            PQNode* current = ratings_head;
            while (current->next != nullptr && current->next->rating <= rating) {
                current = current->next;
            }
            newRating->next = current->next;
            current->next = newRating;
        }
    }

    // Function to display ratings with ascending order from the queue
    void display_ratings() {
        if (ratings_head == nullptr) {
            cout << "No ratings available." << endl;
        } else {
            cout << "Ratings with ascending order:" << endl;
            PQNode* current = ratings_head;
            while (current != nullptr) {
                cout << "-------------------------" << endl;
                cout << "Name: " << current->name << endl;
                cout << "Rating: " << current->rating << endl;
                cout << "-------------------------" << endl;
                current = current->next;
            }
        }
    }
};

int main() {
    Bank bank;
    bank.run();

    return 0;
}
