#pragma once
#include <iostream>
#include <string>
#include <stack>
#include <sstream>
#include <cmath>
#include <fstream>
#include <filesystem>
#include "bigint.h"
#include "Btree.h"
#include "SHA.h"

#include "Routing_Table.h"
using namespace std;
namespace fs = std::filesystem;


class Node_Machine
{
public:
    unsigned long long int machine_id;
    // string file_address;
    string path;
    Node_Machine* next;
    // variable for btree
    BTree tree;
    // variable for routing_table
    Routing_Table* Machine_Routing_Table;
    Node_Machine()
    {
        machine_id = -1;
        path = "Machine_";
        next = NULL;
        tree = NULL;
        // variable for btree initialize to null
        // variable for routing_table initialize to null
    }
    Node_Machine(int id)
    {
        machine_id = id;
        // file_address = address;
        path = "Machine_";

        next = NULL;
        // variable for btree initialize to null
        // variable for routing_table initialize to null
        Machine_Routing_Table = NULL;
    }
};
class Ring_DHT
{
private:
    int bit_space;
    unsigned long long int max_machine;
    unsigned long long int identifier_space;
    Node_Machine* head;
    unsigned long long int machine_count;
    SHA1 sha;
    int order = 0;
public:
    Ring_DHT()
    {
        bit_space = 0;
        max_machine = 0;
        head = NULL;
        machine_count = 0;
        identifier_space = 0;
    }
    Ring_DHT(int bits_space, unsigned long long int machines_count,int m,unsigned long long int max) // plus add functionality
    {
        order = m;
        bit_space = bits_space;
        max_machine = machines_count;
        identifier_space = max;
        head = NULL;
        machine_count = 0;
        identifier_space = pow(2, bit_space);
    }
    // hash_function SHA1 goes here
    // IMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // shift btree functionality in code and integrate in below code
    void add_machine_automatically(int bits,unsigned long long int number)
    {
        unsigned long long int factor = identifier_space / number;
        unsigned long long int var = 0;
        while (number != 0)
        {
            number--;
            add_machine(var);
            var += factor;
        }
    }

    void adjust_files_to_next(Node_Machine* temp,Node_Machine* priv, Node_Machine* new_machine)//for add functionality
    {
        //Node_Machine* next_machine = temp->next;
        if (temp->tree.root == NULL)
        {
            return;
        }
        unsigned long long int val=new_machine->machine_id;
        while (val > priv->machine_id && temp->tree.root!=NULL)
        {
            TreeNode* ret_node = temp->tree.search(val);
            if (ret_node!=NULL)
            {
                string str = ret_node->keys->second;
                string str2 = new_machine->path + "\\" + fs::path(str).filename().string();
                new_machine->tree.insert(val,str2);
                copy_in_folder(str, str2);//str= source & str2=dest
                temp->tree.deletion(val);
            }
            val--;
        }
    }
    void copy_in_folder(string source, string dest)
    {
        if (copyFile(source, dest))
        {
            cout << "File copied successfully to the new folder.\n";
        }
        else
        {
            cout << "Error copying file to the new folder.\n";
        }
        try {
            // Check if the file exists before attempting to delete
            if (fs::exists(source)) {
                // Delete the file
                fs::remove(source);
                std::cout << "File deleted successfully from source." << std::endl;
            }
            else {
                std::cerr << "Error: The specified file does not exist." << std::endl;
            }
        }
        catch (const std::filesystem::filesystem_error& ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
        }
    }
    void add_machine(unsigned long long int id)
    {
        string str = to_string(id);
        if (head == NULL)
        {
            head = new Node_Machine(id);
            head->path += str;
            head->tree.set_order(order);
            machine_count++;
            head->next = head;
        }
        else
        {
            Node_Machine* temp = head;
            Node_Machine* add_new = new Node_Machine(id);
            add_new->tree.set_order(order);

            add_new->path += str;
            
            
            add_new->tree.set_order(order);
            Node_Machine* priv = head;
            if (temp->machine_id==add_new->machine_id)
            {
                cout << "\nError: SEH exception thrown. The Machine already exists.\nNo DUPLICATES allowed.\n";
                return;
            }
            while (temp->next != head && temp->machine_id < add_new->machine_id)
            {
                if (temp->machine_id==add_new->machine_id || priv->machine_id==add_new->machine_id)
                {
                    cout << "\nError: SEH exception thrown. The Machine already exists.\nNo DUPLICATES alowed.\n";
                    return;
                }
                priv = temp;
                temp = temp->next;
            }

            if (createFolder(add_new->path))
            {
                cout << "Folder created successfully.\n";
            }
            else
            {
                cout << "Error creating folder.\n";
            }
            machine_count++;
            // shift btree functionality in below code
            if (temp == head)
            {
                if (temp->machine_id > add_new->machine_id)
                {
                    add_new->next = head;
                    do
                    {
                        temp = temp->next;
                    } while (temp->next != head);
                    //adjust_files_to_next(head, temp, add_new);
                    //all the values at the end as well special case
                    unsigned long long int val = temp->machine_id+1;
                    while (val > temp->machine_id && temp->tree.root != NULL && val < identifier_space)
                    {
                        TreeNode* ret_node = temp->tree.search(val);
                        if (ret_node != NULL)
                        {
                            string str = ret_node->keys->second;
                            string str2 = add_new->path + "\\" + fs::path(str).filename().string();
                            add_new->tree.insert(val, str2);
                            copy_in_folder(str, str2);//str= source & str2=dest
                            add_new->tree.insert(val, ret_node->keys->second);
                            temp->tree.deletion(val);
                        }
                        val++;
                    }
                    val = add_new->machine_id;
                    while (val < temp->machine_id && temp->tree.root != NULL && val >0)
                    {
                        TreeNode* ret_node = temp->tree.search(val);
                        if (ret_node != NULL)
                        {
                            string str = ret_node->keys->second;
                            string str2 = add_new->path + "\\" + fs::path(str).filename().string();
                            add_new->tree.insert(val, str2);
                            copy_in_folder(str, str2);//str= source & str2=dest
                            add_new->tree.insert(val, ret_node->keys->second);
                            temp->tree.deletion(val);
                        }
                        val++;
                    }
                    temp->next = add_new;
                    head = add_new;
                    add_new = NULL;
                    temp = NULL;
                }
                else
                {
                    if (createFolder(add_new->path))
                    {
                        cout << "Folder created successfully.\n";
                    }
                    else
                    {
                        cout << "Error creating folder.\n";
                    }
                    temp->next = add_new;
                    add_new->next = head;
                    //adjust_files_to_next(head, temp, add_new);
                    unsigned long long int val = temp->machine_id;
                    while (val < add_new->machine_id && temp->tree.root != NULL && val>0 && val<identifier_space)
                    {
                        TreeNode* ret_node = temp->tree.search(val);
                        if (ret_node != NULL)
                        {
                            string str = ret_node->keys->second;
                            string str2 = add_new->path + "\\" + fs::path(str).filename().string();
                            add_new->tree.insert(val, str2);
                            copy_in_folder(str, str2);//str= source & str2=dest
                            add_new->tree.insert(val, ret_node->keys->second);
                            temp->tree.deletion(val);
                        }
                        val++;
                    }
                    add_new = NULL;
                    temp = NULL;
                    machine_count++;
                    return;
                }
            }
            else
            {

                if (temp->machine_id > add_new->machine_id)
                {
                    add_new->next = temp;
                    priv->next = add_new;
                    adjust_files_to_next(temp, priv, add_new);

                }
                else
                {
                    add_new->next = temp->next;
                    temp->next = add_new;
                    adjust_files_to_next(temp->next, temp, add_new);

                }
                
            }
            machine_count++;
            return;
        }
        if (createFolder(head->path))
        {
            cout << "Folder created successfully.\n";
        }
        else
        {
            cout << "Error creating folder.\n";
        }
    }
    // IMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // shift btree functionality in code and integrate in below code
    // Adjust files
    void levelOrderTraversal_Btree_Copy_and_Delete2(Node_Machine* copy_to, Node_Machine* copy_from) {
        if (copy_from->tree.root == NULL) { // empty check
            cout << "Tree Empty. NO COPY" << endl;
            return;
        }

        queue<TreeNode*> temp;
        temp.push(copy_from->tree.root);   // enqueue the root node

        while (temp.empty() == NULL) {
            int num;
            num = temp.size(); // num of node
            for (int i = 0; i < num; ++i) { // process each node at current lvl
                TreeNode* node = temp.front(); // dequeue a node from the front
                temp.pop();


                for (int j = 0; j < node->numsKeys; ++j) {
                    string str = node->keys[j].second;
                    string str2 = copy_to->path + "\\" + fs::path(str).filename().string();
                    copy_in_folder(str, str2);//str= source & str2=dest
                    copy_to->tree.insert(node->keys[j].first, str2);
                }
                // enqueue child node if the current node is not a leaf
                if (node->leafFlag == NULL) {
                    for (int j = 0; j <= node->numsKeys; ++j) {
                        if (node->childArray[j]) {
                            temp.push(node->childArray[j]);
                        }
                    }
                }
                copy_from->tree.deletion(node->keys[node->numsKeys - 1].first);
            }
            cout << endl;
        }
    }
    void levelOrderTraversal_Btree_Copy_and_Delete1(Node_Machine* copy_to, Node_Machine* copy_from) {
        if (copy_from->tree.root == NULL) { // empty check
            cout << "Tree Empty. NO COPY" << endl;
            return;
        }

        queue<TreeNode*> temp;
        temp.push(copy_from->tree.root);   // enqueue the root node

        while (temp.empty() == NULL) {
            int num;
            num = temp.size(); // num of node
            for (int i = 0; i < num; ++i) { // process each node at current lvl
                TreeNode* node = temp.front(); // dequeue a node from the front
                temp.pop();


                for (int j = 0; j < node->numsKeys; ++j) {

                    string str = node->keys[j].second;
                    string str2 = copy_to->path + "\\" + fs::path(str).filename().string();
                    copy_in_folder(str, str2);//str= source & str2=dest
                    copy_to->tree.insert(node->keys[j].first, node->keys[j].second);

                    copy_from->tree.deletion(node->keys[j].first);
                }
                // enqueue child node if the current node is not a leaf
                if (node->leafFlag == NULL) {
                    for (int j = 0; j <= node->numsKeys; ++j) {
                        if (node->childArray[j]) {
                            temp.push(node->childArray[j]);
                        }
                    }
                }

            }
        }
    }
    void levelOrderTraversal_Btree_Copy_and_Delete3(Node_Machine* copy_to, Node_Machine* copy_from) {
        if (copy_from->tree.root == NULL) { // empty check
            cout << "Tree Empty. NO COPY" << endl;
            return;
        }

        queue<TreeNode*> temp;
        temp.push(copy_from->tree.root);   // enqueue the root node

        while (!temp.empty()) {
            int num;
            num = temp.size(); // num of node
            for (int i = 0; i < num; ++i) { // process each node at the current level
                TreeNode* node = temp.front(); // dequeue a node from the front
                temp.pop();

                for (int j = 0; j < node->numsKeys; ++j) {
                    string str = node->keys[j].second;
                    string str2 = copy_to->path + "\\" + fs::path(str).filename().string();
                    copy_in_folder(str, str2); // str=source & str2=dest
                    copy_to->tree.insert(node->keys[j].first, str2);
                }

                // enqueue child node if the current node is not a leaf
                if (node->leafFlag == NULL) {
                    for (int j = 0; j <= node->numsKeys; ++j) {
                        if (node->childArray[j]) {
                            temp.push(node->childArray[j]);
                        }
                    }
                }

                if (node->numsKeys > 0) {
                    // Assuming you want to delete the last key in the current node after copying
                    copy_from->tree.deletion(node->keys[node->numsKeys - 1].first);
                }
            }

        }
    }
    void levelOrderTraversal_Btree_Copy_and_Delete(Node_Machine* copy_to, Node_Machine* copy_from) {
        if (copy_from->tree.root == NULL) {
            cout << "Tree Empty. NO COPY" << endl;
            return;
        }

        queue<TreeNode*> temp;
        temp.push(copy_from->tree.root);
        queue<int> keysToDelete;  // Queue to store keys to delete

        while (!temp.empty()) {
            int num = temp.size();
            for (int i = 0; i < num; ++i) {
                TreeNode* node = temp.front();
                temp.pop();

                for (int j = 0; j < node->numsKeys; ++j) {
                    string str = node->keys[j].second;
                    string str2 = copy_to->path + "\\" + fs::path(str).filename().string();
                    copy_in_folder(str, str2); // str=source & str2=dest

                    copy_to->tree.insert(node->keys[j].first, node->keys[j].second);
                    keysToDelete.push(node->keys[j].first);  // Enqueue keys to delete
                }

                if (node->leafFlag == NULL) {
                    for (int j = 0; j <= node->numsKeys; ++j) {
                        if (node->childArray[j]) {
                            temp.push(node->childArray[j]);
                        }
                    }
                }
            }
        }

        // Delete collected keys from the original tree
        stack<int> keydel;
        while (!keysToDelete.empty()) {
            keydel.push(keysToDelete.front());
            keysToDelete.pop();
        }

        while (!keydel.empty()) {
            copy_from->tree.deletion(keydel.top());
            keydel.pop();
        }
    }
    void delete_machine(int id)
    {
        if (head == NULL)
        {
            cout << "No machines to delete\n";
            return;
        }
        Node_Machine* temp = head;
        Node_Machine* priv = NULL;
        Node_Machine* succesor = NULL;

        while (temp->next != head && temp->machine_id != id)
        {
            priv = temp;
            temp = temp->next;
        }
        succesor = temp->next;
        if (temp->machine_id == id)
        {
            string str = temp->path;
            if (temp == head)
            {
                // shift btree functionality in below code to successor
                if (temp->next == head)
                {
                    if (temp->Machine_Routing_Table != NULL)
                    {
                        temp->Machine_Routing_Table->Empty_Routing_Table();
                        delete temp->Machine_Routing_Table;
                        temp->Machine_Routing_Table = NULL;
                    }
                    delete temp;
                    head = NULL;
                    machine_count--;
                }
                else
                {
                    levelOrderTraversal_Btree_Copy_and_Delete(succesor, temp);
                    cout << "Successor:" << succesor->machine_id << endl;
                    succesor->tree.levelOrderTraversal();
                    priv = Return_last();
                    priv->next = succesor;
                    head = succesor;
                    if (temp->Machine_Routing_Table != NULL)
                    {
                        temp->Machine_Routing_Table->Empty_Routing_Table();
                        delete temp->Machine_Routing_Table;
                        temp->Machine_Routing_Table = NULL;
                    }
                    //priv->next = head;
                    delete temp;
                    temp = NULL;
                    priv = NULL;
                    machine_count--;
                }
                
            }
            else
            {
                levelOrderTraversal_Btree_Copy_and_Delete(succesor, temp);
                cout << "Successor:" << succesor->machine_id<<endl;
                succesor->tree.levelOrderTraversal();
                priv->next = temp->next;
                if (temp->Machine_Routing_Table != NULL)
                {
                    temp->Machine_Routing_Table->Empty_Routing_Table();
                    delete temp->Machine_Routing_Table;
                    temp->Machine_Routing_Table = NULL;
                }
                delete temp;
                temp = NULL;
                priv = NULL;
                machine_count--;
            }
            try {
                // Check if the folder exists
                if (fs::exists(str)) {
                    // Remove the folder and its contents
                    fs::remove_all(str);
                    std::cout << "Folder deleted successfully." << std::endl;
                }
                else {
                    std::cout << "Folder does not exist." << std::endl;
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
        else
        {
            cout << "Machine not found\n";
        }
    }
    //!!!!!!!!!!!!!!!!!!!IMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // MUST_______READ___________________________________________
    // creating folder and file copying code
    bool createFolder(const string& path)
    {
        // Create a folder using create_directory function
        try
        {
            fs::create_directory(path);
            return true;
        }
        catch (const fs::filesystem_error& e)
        {
            cerr << "Error creating folder: " << e.what() << endl;
            return false;
        }
    }

    bool copyFile(const string& source, const string& dest)
    {
        // Copy a file from sourcePath to destinationPath
        try
        {
            fs::copy_file(source, dest, fs::copy_options::overwrite_existing);
            return true;
        }
        catch (const fs::filesystem_error& e)
        {
            cerr << "Error copying file: " << e.what() << endl;
            return false;
        }
    }
    void Add_File()
    {
        string example = "D:/SEm 03/DATA_STRUCT/Data_struct/hello.txt or D:\\SEm 03\\DATA_STRUCT\\Data_struct\\hello.txt ";
        cout << "\nEnter path in this format:" << example << endl;
        cout << "Enter the path of the file to copy: ";
        string source;
        getline(cin >> ws, source);
        sha.update(source);
        string hash2 = sha.from_file(source);
        cout << "\n Hash in hex string:" << hash2;
        string decimal = Hex_Dec(hash2);                     //CONVERSION From HEXADECIMAL to DECIMAL
        cout << "\nHash in decimal string:" << decimal << endl;
        bigint a = bit_extractor(decimal, bit_space);             //USES THE MODULO OPERATION TO EXTRACT BITS
        cout << "\nFinal Hash Key:" << a << endl;
        string temp1 = a.str;
        int key = stoi(temp1);
        cout << "\nPress 0 if you want to satrt routing form head.\nPress 1 to start routing from a specific node.\n";
        int choice;
        Node_Machine* temp;
        do
        {
            cout << "\nEnter choice:";
            cin >> choice;
        } while (choice<0 || choice>1);
        if (choice==0)
        {

            temp = Routing(key, head->machine_id);
        }
        else
        {
            cout << "Enter hash key/ machine id of node you want to start search from:";
            unsigned long long int val;
            cin >> val;
            temp = Routing(key, val);
        }
        if (temp==NULL)
        {
            return;
        }
        cout << "\nData is stored on this Node.\n";
        
        // Check if the file exists
        if (fs::exists(source)) {
            std::cout << "File exists. Proceed with copying." << std::endl;
            // Add your file copy logic here
        }
        else {
            std::cerr << "Error: The specified file does not exist." << std::endl;
        }
        if (!fs::exists(source))
        {
            cerr << "Error: The specified file does not exist.\n";
            return;
        }

        // Specify folder path
        string filename = fs::path(source).filename().string();
        string folderPath = temp->path + "\\"+filename;
        if (copyFile(source, folderPath))
        {
            cout << "File copied successfully to the new folder.\n";
        }
        else
        {
            cout << "Error copying file to the new folder.\n";
        }
        
        temp->tree.insert(key,folderPath);
        // print updated btree
        temp->tree.levelOrderTraversal();
    }

    void Delete_File(unsigned long long int key)
    {
        cout << "\nPress 0 if you want to satrt routing form head.\nPress 1 to start routing from a specific node.\n";
        int choice;
        Node_Machine* temp;
        do
        {
            cout << "\nEnter choice:";
            cin >> choice;
        } while (choice < 0 || choice>1);
        if (choice == 0)
        {
            temp = Routing(key, head->machine_id);
        }
        else
        {
            cout << "Enter hash key/ machine id of node you want to start search from:";
            unsigned long long int val;
            cin >> val;
            temp = Routing(key, val);
        }
        if (temp == NULL)
        {
            return;
        }
        cout << "\nData is stored on this Node.\n";
        TreeNode* node = temp->tree.search(key);
        string filePath = node->keys->second;

        try {
            // Check if the file exists before attempting to delete
            if (fs::exists(filePath)) {
                // Delete the file
                fs::remove(filePath);
                std::cout << "File deleted successfully." << std::endl;
            }
            else {
                std::cerr << "Error: The specified file does not exist." << std::endl;
            }
        }
        catch (const std::filesystem::filesystem_error& ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
        }
        // temp->btree->delete(data);
        temp->tree.deletion(key);
        // print updated btree
        temp->tree.levelOrderTraversal();
    }
    Node_Machine* find_Sucessor(Node_Machine* temp, int factor)
    {
        Node_Machine* temp1 = temp;
        unsigned long long int var = temp->machine_id;
        unsigned long long int successor = temp->machine_id + pow(2, factor - 1); // id of successor
        while (temp1->next != temp)
        {
            temp1 = temp1->next;
            if (temp1->machine_id >= successor)
            {
                // temp1 is successor
                return temp1;
            }
        }
        if (successor <= identifier_space)
        {
            return head;
        }
        else
        {
            successor = successor - identifier_space;
            temp1 = head;
            while (temp1->next != head)
            {
                temp1 = temp1->next;
                if (temp1->machine_id >= successor)
                {
                    // temp1 is successor
                    return temp1;
                }
            }
        }
    }
    void Set_Routing_Table_Machine()
    {
        Node_Machine* temp = head;
        do
        {
            if (temp->Machine_Routing_Table!=NULL)
            {
                temp->Machine_Routing_Table->Empty_Routing_Table();
                delete temp->Machine_Routing_Table;
                temp->Machine_Routing_Table = NULL;
            }
            temp->Machine_Routing_Table = new Routing_Table;
            for (int i = 0; i < bit_space; ++i)
            {
                Node_Machine* succ = find_Sucessor(temp, i + 1);
                temp->Machine_Routing_Table->insert(succ);
            }
            temp = temp->next;
        } while (temp != head);
    }
    void print_machine()
    {
        Node_Machine* temp = head;
        do
        {
            cout << "Machine:" << temp->machine_id << "-->";
            temp = temp->next;
        } while (temp != head);
        cout << "Machine:" << temp->machine_id << "<--HEAD";
    }
    void print_B_Tree(unsigned long long int i)
    {
        Node_Machine* temp = leniar_search(i);
        if (temp==NULL)
        {
            return;
        }
        cout << "\nBtree of machine_" << temp->machine_id << endl;
        temp->tree.levelOrderTraversal();

    }
    void print_single_routing_table(unsigned long long int i)
    {
        Node_Machine* temp = leniar_search(i);
        if (temp == NULL)
        {
            return;
        }
        Routing_Table* temp1 = temp->Machine_Routing_Table;
        // temp->Machine_Routing_Table->print_routing();
        cout << "Routing_Table -> " << temp->machine_id << "\n";
        Node_Routing_Table* temp2 = temp1->head;
        for (int i = 0; i < bit_space; i++)
        {
            cout << "Machine : " << temp->machine_id << " <- MAPPED ON INDEX :" << i + 1 << " <- :" << temp2->machine->machine_id << endl;
            temp2 = temp2->next;
        }
        cout << endl;
    }
    Node_Machine* leniar_search(unsigned long long int i)
    {
        Node_Machine* temp = head;
        bool fl = false;
        do
        {
            if (temp->machine_id == i)
            {
                fl = true;
                break;
            }
            temp = temp->next;

        } while (temp!=head);
        if (fl==false)
        {
            cout << "\nMachine with " << i << " id does not exist.";
            return NULL;
        }
        return temp;
    }
    void print_complete_routing()
    {
        Node_Machine* temp = head;
        if (temp->Machine_Routing_Table==NULL)
        {
            return;
        }
        cout << endl;
        do
        {
            Routing_Table* temp1 = temp->Machine_Routing_Table;
            // temp->Machine_Routing_Table->print_routing();
            cout << "Routing_Table -> " << temp->machine_id << "\n";
            Node_Routing_Table* temp2 = temp1->head;
            for (int i = 0; i < bit_space; i++)
            {
                cout << "Machine : " << temp->machine_id << " <- MAPPED ON INDEX :" << i + 1 << " <- :" << temp2->machine->machine_id << endl;
                temp2 = temp2->next;
            }
            cout << endl;
            temp = temp->next;
        } while (temp != head);
    }
    void search_file(int key, int node_id)
    {
        Node_Machine* node_with_file = Routing(key, node_id);
        TreeNode* node=node_with_file->tree.search(key);
        cout << endl;
        std::filesystem::path currentPath = std::filesystem::current_path();

        cout << currentPath << node->keys->second << " <----Path of file.";

    }
    Node_Machine* Routing(int key, int node_id)
    {
        Node_Machine* temp = head;
        bool fl = false;
        do
        {
            if (temp->machine_id == node_id)
            {
                fl = true;
                break;
            }
            temp = temp->next;
        } while (temp != head);
        if (fl == false)
        {
            cout << "\nMachine Does Not Exist.\n";
            return NULL;
        }
        cout << "\nStarting Node ";
        if (temp == head && key < temp->machine_id)
        {
            cout << "---->(" << temp->machine_id << ")";

        }
        else if (special_case_head(temp, key) && temp==head )
        {
            cout << "---->(" << temp->machine_id << ")";
        }
        else
        {
            Node_Machine* machine_found = Search(temp, key);
            cout << "<---- Data is stored on this Node.\n";
            return machine_found;
        }
        
        cout << "<---- Data is stored on this Node.\n";
        return temp;
    }
    bool special_case_head(Node_Machine* temp, int key)
    {
        if (temp==head)
        {
            Node_Machine* temp1 = Return_last();
            if (key > temp1->machine_id)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    Node_Machine* Return_last()
    {
        Node_Machine* temp = head;
        Node_Machine* priv = NULL;
        do
        {
            priv = temp;
            temp = temp->next;

        } while (temp!= head);

        return priv;
    }
    Node_Machine* Search(Node_Machine* temp, int key)
    {

        if (temp == NULL)
        {
            return NULL;
            /* code */
        }
        
        cout << "---->(" << temp->machine_id << ")";
        if (key == temp->machine_id)
        {
            return temp;
            // base case Node found
        }
        else if (temp->machine_id < key) //&& key <= temp->Machine_Routing_Table->Return_At_Index(1)->machine_id)
        {
            Node_Machine* temp1 = temp->Machine_Routing_Table->Return_At_Index(1);
            
            // Search(temp, key);
            if (temp1->machine_id >= key) //|| find_Sucessor(temp1, 1) == head)
            {
                cout << "---->(" << temp1->machine_id << ")";
                return temp1;
            }
            else
            {
                Node_Machine* temp2 = findClosest(key, temp);
                if (temp2->machine_id==temp->machine_id)
                {
                    return temp;
                }
                else
                {
                    temp = temp2;
                }
                return Search(temp, key);
            }
        }
        else if (temp->machine_id > key)
        {
            Node_Machine* temp2 = findClosest(key, temp);
            if (temp2->machine_id == temp->machine_id)
            {
                return temp;
            }
            else
            {
                temp = temp2;
            }
            return Search(temp, key);
            
        }
    }

    Node_Machine* findClosest(int key, Node_Machine* temp1) // write code for this case to find the closest to key
    {
        unsigned long long int difference;
        Node_Routing_Table* closest = temp1->Machine_Routing_Table->head;
        Node_Routing_Table* temp = closest->next;
        while (temp != NULL)
        {
            if (closest->machine->machine_id < key && key <= temp->machine->machine_id)
            {
                break;
            }
            closest = temp;
            temp = temp->next;
        }
        return closest->machine;
    }
};

