#pragma once

class Node {
private:
	Node* prev; // save previous node address
	Node* next; // save next node address
	int col;
	int row;
public:
	Node* Getprev() { return prev; } // get previous node
	Node* Getnext() { return next; } // get next node
	void Setprev(Node* prev) { this->prev = prev; }
	void Setnext(Node* next) { this->next = next; }
	int Getcol() { return col; }
	int Getrow() { return row; }
	void Setcol(int col) { this->col = col; }
	void Setrow(int row) { this->row = row; }
	void Setlocation(int row, int col) { this->col = col; this->row = row; } // set location with new one
};

class SnakeLinkedList
{

public:
	Node* head;
	Node* tail;
	int size = 3; // inital size
	SnakeLinkedList(); // constructor
	void Append(int newrow, int newcol);
	void Move(int newrow, int newcol);
	bool checkfail(int h_row, int h_col);

};

