/*
 * Copyright 2018 <copyright holder> <email>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */

#include "fifo.h"


Fifo::Fifo() {
	head = NULL;
}

Fifo::~Fifo()  {
	Node *next = head;

	while(next) {
		Node *deleteMe = next;
		next = next->next;
		delete deleteMe;
	}
}

void Fifo::push(T* val) {
	Node *n = new Node();
	n->x = val;


	if(head == NULL)
	{
		head = n;
	} else {
		Node* current = header;
		while(current->next != NULL) {
			current = current->next;
		}
		current->next = n;
	}
}

T* Fifo::get() {
	Node *n = head;
	T* ret = n->x;
	head = head->next;
	delete n;
	return ret;
}



