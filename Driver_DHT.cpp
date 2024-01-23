#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include "Ring_DHT.h"

using namespace std;

// to print main controler menu for distributed hash table
int Print_Menu_Get_Choice()
{
    int choice;

    do
    {
        cout << endl;
        cout << "_________________________________________________________________________\n";
        cout << "|                       DHT FUNCTIONALIY MENU                           |\n";
        cout << "|_______________________________________________________________________|\n";

        cout << "_________________________________________________________________________\n";
        cout << "| Main Menu:                                                            |\n";
        cout << "| Enter 1 to add file / data.                                           |\n";
        cout << "| Enter 2 to delete file / data.                                        |\n";
        cout << "| Enter 3 to search file / data.                                        |\n";
        cout << "| Enter 4 to search machine.                                            |\n";
        cout << "| Enter 5 to print routing table.                                       |\n";
        cout << "| Enter 6 to print B-Tree of a machine.                                 |\n";
        cout << "| Enter 7 to add a machine.                                             |\n"; // semi done
        cout << "| Enter 8 to delete a machine.                                          |\n"; // semi done
        cout << "| Enter 9 to Display All machines with Data and Routing Table.          |\n";
        cout << "| Enter 0 to exit.                                                      |\n";
        cout << "|_______________________________________________________________________|\n";
        cout << "Enter your choice: ";
        cin >> choice;
    } while (choice < 0 || choice > 9);

    return choice;
}
int main()
{
    // Print_Menu_Get_Choice();
    int bits_space = 0;
    unsigned long long int machines_count = 0;
    int choice = -1;

    do
    {
        cout << "Enter identifier bits (Bit space of DHT): ";
        cin >> bits_space;
    } while (bits_space < 1); // max bits check also IMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    cout << endl;

    unsigned long long int identifier = pow(2, bits_space);

    do
    {
        cout << "Enter number of machines: ";
        cin >> machines_count;
    } while (machines_count < 1 || machines_count > machines_count);

    int m;//order of btree
    cout << endl;
    do
    {
        cout << "Enter Order of btree: ";
        cin >> m;
    } while (m<0);

    cout << endl;
    do
    {
        cout << "Enter 1 to manually enter machines ids \nOtherwise enter 0 to automaticly generate machine ids\n";
        cout << "Please enter your choice: ";
        cin >> choice;
    } while (choice < 0 || choice > 1);
    Ring_DHT var(bits_space, machines_count,m, identifier);
    // cmpare choice to set machines
    unsigned long long int i = 0, key = -1;
    SHA1 main_sha;
    if (choice == 0)
    {
        var.add_machine_automatically(bits_space, machines_count);
    }

    else
    {
        choice = -1;
        cout << "\nDo you want to directly enter hash key that is to be assigned machine.     (press 0)  ";
        cout << "\nFor entring name or string of machine that is to be converted to hash key. (press 1)\n";
        do
        {
            cout << "\nEnter your choice:";
            cin >> choice;
        } while (choice < 0 || choice > 1);
        if (choice==0)
        {
            while (i != machines_count)
            {
                cout << endl;
                cout << "Enter machine number: ";
                unsigned long long int id_temp;
                cin >> id_temp;
                var.add_machine(id_temp);
                cout << endl;
                var.print_machine();
                cout << endl;

                i++;
            }
        }
        else
        {
            while (i != machines_count)
            {
                cout << endl;
                cout << "Enter machine name/string: ";
                string id_temp;
                getline(cin >> ws, id_temp);
                main_sha.update(id_temp);
                string hash = main_sha.final();
                hash = Hex_Dec(hash);
                cout << "\nHash key generated:";
                cout << hash << endl;
                bigint a = bit_extractor(hash, bits_space);
                unsigned long long int id_temp1 = stoi(a.str);//comment when changing unsigned long long int to big int
                cout << "\nFinal Hash Key:"<<id_temp1<<endl;
                var.add_machine(id_temp1);
                cout << endl;
                var.print_machine();
                cout << endl;
                i++;
                //D:\\SEm 03\\DATA_STRUCT\\Distributed-Hash-Table-System-main\\Distributed-Hash-Table-System-main\\hello.txt
            }
        }
        
    }


    choice = -1;
    i = 0;
    var.Set_Routing_Table_Machine();
    int choice1 = -1;
    do
    {
        choice = Print_Menu_Get_Choice();
        switch (choice)
        {
        case 1:
            var.Add_File();
            break;
        case 2:
             cout << "Enter key to delete: ";
             cin >> key;
             var.Delete_File(key);
            break;
        case 3:
            //search functionaliy
             cout << "Enter key to search: ";
             cin >> key;
             var.print_machine();
             cout << "\nEnter the node you want to satrt search from:";
             unsigned long long int node_start;
             cin >> node_start;
             cout << endl;
             //var.Set_Routing_Table_Machine();
             var.search_file(key,node_start);
            break;
        case 4:
            cout << "\nEnter node to start search from:";
            do
            {
                cin >> i;
            } while (i<0 || i>identifier);
            cout << "Enter key to be found:";
            do
            {
                cin >> key;
            } while (key<0 || key>identifier);
            cout << endl;
            //var.Set_Routing_Table_Machine();
            var.Routing(key, i);
            break;
        case 5:
            cout << "\nEnter node you want to print routing table of:";
            do
            {
                cin >> i;
            } while (i<0 || i>identifier);
            var.Set_Routing_Table_Machine();
            var.print_single_routing_table(i);
            break;
        case 6:
            //print b tree functionality
             cout << "Enter machine number to print B-Tree: ";
             do
             {
                 cin >> i;

             } while (i<0 || i>identifier);
             var.print_B_Tree(i);
            break;
        case 7:
            
            cout << "\nDo you want to directly enter hash key that is to be assigned machine.     (press 0)  ";
            cout << "\nFor entring name or string of machine that is to be converted to hash key. (press 1)\n";
            do
            {
                cout << "\nEnter your choice:";
                cin >> choice1;
            } while (choice1 < 0 || choice1 > 1);

            if (choice1 == 0)
            {
                    cout << endl;
                    cout << "Enter machine number: ";
                    unsigned long long int id_temp;
                    cin >> id_temp;
                    var.add_machine(id_temp);
                    cout << endl;
                    var.print_machine();
                    cout << endl;
            }
            else
            {
                    cout << endl;
                    cout << "Enter machine name/string: ";
                    string id_temp;
                    getline(cin >> ws, id_temp);

                    main_sha.update(id_temp);
                    string hash = main_sha.final();
                    hash = Hex_Dec(hash);

                    cout << "\nHash key generated:";
                    cout << hash << endl;
                    bigint a = bit_extractor(hash, bits_space);
                    unsigned long long int id_temp1 = stoi(a.str);//comment when changing unsigned long long int to big int
                    cout << "\nFinal Hash Key:" << id_temp1 << endl;
                    var.add_machine(id_temp1);
                    cout << endl;
                    var.print_machine();
                    cout << endl;
            }
            var.Set_Routing_Table_Machine();
            break;
        case 8:
            cout << "Enter machine number to delete: ";
            do
            {
                cin >> i;
            } while (i<0 || i>identifier);
            var.delete_machine(i);
            var.Set_Routing_Table_Machine();
            break;
        case 9:
            var.print_machine();
            var.print_complete_routing();
            break;
        
        default:
            break;
        }
    } while (choice != 0);

    /*var.print_machine();
    var.Set_Routing_Table_Machine();
    var.print_complete_routing();
    i = 0;
    cout << "Enter key:";
    cin >> key;
    cout << "\nEnter node to start search from:";
    cin >> i;
    cout << endl;
    var.Routing(key, i);*/
    return 0;
}