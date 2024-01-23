#pragma once
#include <iostream>
#include <string>

using namespace std;

struct Node_Machine;

struct Node_Routing_Table
{
public:
    int index;
    Node_Routing_Table* next;
    Node_Routing_Table* privious;
    Node_Machine* machine;
    int size = 0;
    int Max_size;

    Node_Routing_Table()
    {
        index = -1;
        next = NULL;
        privious = NULL;
        Max_size = 0;
    }
    Node_Routing_Table(Node_Machine* temp)
    {
        index = size;
        machine = temp;
        size++;
        next = NULL;
        privious = NULL;
        Max_size = 0;
    }
};
class Routing_Table
{
public:
    Node_Routing_Table* head;
    Node_Routing_Table* tail;
    Routing_Table()
    {
        head = NULL;
        tail = NULL;
    }

    void insert(Node_Machine* temp)
    {
        if (head == NULL)
        {
            head = new Node_Routing_Table(temp);
            tail = head;
        }
        else
        {
            tail->next = new Node_Routing_Table(temp);
            tail->next->privious = tail;
            tail = tail->next;
        }
    }
    void delete_machine(int id)
    {
        Node_Routing_Table* temp = head;
        while (temp->next != NULL && temp->index != id)
        {
            temp = temp->next;
        }
        if (temp->index == id)
        {
            if (temp == head)
            {
                head = head->next;
                delete temp;
                temp = NULL;
            }
            else if (temp == tail)
            {
                tail = tail->privious;
                delete temp;
                temp = NULL;
            }
            else
            {
                temp->privious->next = temp->next;
                temp->next->privious = temp->privious;
                delete temp;
                temp = NULL;
            }
        }
        else
        {
            cout << "Machine not found\n";
        }
    }
    // function to delete all nodes of Doubly Linked List in case new node is added or a node is deleted
    void Empty_Routing_Table()
    {
        Node_Routing_Table* current = head;
        while (head != NULL)
        {
            head = head->next;
            delete current;
            current = head;
        }
        tail = head;
    }
    Node_Machine* Return_At_Index(int index1)
    {
        Node_Routing_Table* current = head;
        int control = 1;
        while (current != NULL && control < index1)
        {
            control++;
            current = current->next;
        }
        if (current == NULL)
        {
            return NULL;

        }
        else
            return current->machine;

    }

    
};

