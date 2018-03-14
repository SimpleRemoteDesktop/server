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


#ifndef FIFO_H
#define FIFO_H

template <class T>
class Fifo
{
	struct Node
	{
		T* x;
		Node *next;
	};

	public:
	Fifo(){
		head = nullptr;
	}
	~Fifo() {
		Node *next = head;

		while(next) {
			Node *deleteMe = next;
			next = next->next;
			delete deleteMe;
		}

	}
	void push(T* val) {
		Node *n = new Node();
		n->x = val;


		if(head == nullptr)
		{
			head = n;
		} else {
			Node* current = head;
			while(current->next != nullptr) {
				current = current->next;
			}
			current->next = n;
		}
	}
	T* get() {
		Node *n = head;
		if(n != nullptr) {
			T* ret = n->x;
			head = head->next;
			delete n;
			return ret;


		}
	}

	private : 
	Node *head;
};

#endif // FIFO_H
