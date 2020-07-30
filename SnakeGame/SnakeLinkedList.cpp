#include "pch.h"
#include "SnakeLinkedList.h"

SnakeLinkedList::SnakeLinkedList() { // constructor of SnakeLinkedList
	head = new Node();
	tail = new Node();
	Node* body = new Node();
	head->Setlocation(12, 12); // set init location of head
	body->Setlocation(12, 13); // set init location of body
	tail->Setlocation(12, 14); // set init location of tail
	head->Setnext(body);
	head->Setprev(tail);
	body->Setnext(tail);
	body->Setprev(head);
	tail->Setprev(body);
	tail->Setnext(head);
}

void SnakeLinkedList::Append(int newrow, int newcol) {
	Node* cur = new Node;
	cur->Setlocation(newrow, newcol); // save new location in cur node data
	cur->Setnext(head);
	head->Setprev(cur);
	tail->Setnext(cur);
	cur->Setprev(tail);
	head = cur;
	size++; // increase size one by one
}

void SnakeLinkedList::Move(int newrow, int newcol) { // function to move
	Node* cur = head;

	tail = tail->Getprev();
	head = tail->Getnext();
	head->Setlocation(newrow, newcol); // set head location with new one
}

bool SnakeLinkedList::checkfail(int h_row, int h_col) { // if snake crush itself, return true
	Node* cur = head;
	do {
		if ((h_col == cur->Getcol()) && (h_row == cur->Getrow()))
			return true;
		cur = cur->Getnext();
	} while (cur != head);
	return false;
}