#pragma once



namespace ChessMob
{
	// allows for list to work for all types.
	template <class T>

	class List
	{
	private:

		// nodes of lists
		struct ListNode
		{
			// pointer to the next node in the list
			ListNode * Next;

			// the information being stored in the list
			T Info;

			// Default Constructor; sets Next to null
			ListNode()
			{
				Next = NULL;
				Info = NULL;
			}

			// Overloaded Constructor; sets Next to null and Info to passed value
			ListNode(T info)
			{
				Next = NULL;
				Info = info;
			}

			// Overloaded Constructor; sets Next and Info to the passed in value
			ListNode(ListNode * next, T info)
			{
				Next = next;
				Info = info;
			}

			~ListNode()
			{
				Next = NULL;
				//Info = NULL;
			}
		};

		// pointer to the front of the List
		ListNode * HeadNode;
		
		// keeps track of the number of elements in the List
		int Size;

	public:

		struct Iterator
		{
			// A pointer to the current node in a list
			ListNode * Current;
			ListNode * Previous;

			// Defualt Constructor; sets Current to null
			Iterator()
			{
				Current = NULL;
				Previous = NULL;
			}

			// Overloaded Constructor; sets Current to passed value
			Iterator(ListNode * current)
			{
				Current = &*current;
				Previous = NULL;
			}
			// override for assignment operator
			void operator = (ListNode * value)
			{
				Current = value;
			}

			// override for assignment operator
			void operator = (const Iterator other)
			{
				this->Current = other.Current;
				this->Previous = other.Previous;
			}
			
			// override for equal operator
			bool operator == (const Iterator other) const
			{
				return Current == other.Current;
			}

			// override for not equal operator
			bool operator != (const Iterator other) const
			{
				return Current != other.Current;
			}
			
			// override for the not equal operator
			bool operator != (const ListNode *other) const
			{
				return Current != other;
			}

			// override for increment operator
			void operator ++ ()
			{	
				Previous = Current;

				if(Current != NULL)
				{
					Current = Current->Next;
				}

			}

			// override for deference operator
			T & operator * ()
			{
				return Current->Info;
			}
		};

		// Constructor; sets HeadNode to null and Size to 0
		List()
		{
			HeadNode = NULL;
			Size = 0;
		}

		// Destructor
		~List()
		{
			ListNode * temp;

			while(HeadNode != NULL)
			{
				temp = HeadNode;
				HeadNode = HeadNode->Next;
				delete temp;
				temp = NULL;
			}
			HeadNode = NULL;
			Size = 0;
		}

		// Returns a pointer to the first node of the List
		Iterator Begin()
		{
			return Iterator(HeadNode);
		}

		// Returns null
		Iterator End()
		{
			return Iterator(NULL);
		}

		// creates a new element with Info = info to the fron of the list
		void PushFront(T info)
		{
			ListNode * temp = HeadNode;
			HeadNode = new ListNode(temp, info);
			temp = NULL;
			Size++;
		}

		// returns true if there are no elements in the list, otherwise it returns false
		bool Empty()
		{
			return Size == 0;
		}

		// removes all elements with Info equal to value
		void Remove(T value)
		{
			// indexes through List
			ListNode * index = HeadNode;
			ListNode * previous = NULL;

			// loop through list to remove items
			while(index != NULL)
			{
				// if item Info eqauls value, remove it
				if(index->Info == value)
				{
					// if item is not the first item, previous Next is item.Next
					if(previous != NULL)
					{
						previous->Next = index->Next;
						
						// remove next.
						delete index;
						
						//update index
						index = previous->Next;
					}

					// item is first node, so change HeadNode to item.Next
					else
					{
						HeadNode = index->Next;

						// remove next.
						delete index;

						// update index
						index = HeadNode;
					}
					Size--;
				}
				// not equal to the value.
				else
				{
					// update previous
					previous = index;

					// update index
					index = previous->Next;
				}						
			}
		}

		// removes the first element from the list
		void PopFront()
		{
			if(HeadNode != NULL)
			{
				ListNode * temp = HeadNode;

				// update HeadNode
				HeadNode = temp->Next;

				// remove element
				delete temp;
				temp = NULL;

				// decrement size.
				Size--;
			}
		}

		// sorts the list by the passed in comparison function
		void Sort(bool (*compareFunction)(T first, T second))
		{
			// lists to merge
			ListNode * subListOne;
			ListNode * subListTwo;

			// next element on the merged list
			ListNode * nextMergeElement;

			// location to add next element on list
			ListNode * tail;
			
			int subListSize, subListOneSize, subListTwoSize, numMerges,stepsIntoList;

			// empty list, no need to sort
			if (HeadNode == NULL)
			{
				return;
			}

			// start with list size 1
			subListSize = 1;

			// go until down
			while(true)
			{
				subListOne = HeadNode;
				HeadNode = NULL;
				tail = NULL;

				numMerges = 0;

				while(subListOne != NULL)
				{
					// tells the number of merges per run to know when done
					numMerges++;

					// sets up second sub list
					subListTwo = subListOne;
					subListOneSize = 0;
					for(stepsIntoList = 0; stepsIntoList < subListSize; stepsIntoList++)
					{
						subListOneSize++;
						subListTwo = subListTwo->Next;

						// finishes if reaches end of list
						if(subListTwo == NULL)
						{
							break;
						}
					}

					// approximate size of subListTwo
					subListTwoSize = subListSize;

					// determine which element next
					while(subListOneSize > 0 || (subListTwoSize > 0 && subListTwo != NULL))
					{
						// subListOne is Empty, so pull from subListTwo
						if(subListOneSize == 0)
						{
							nextMergeElement = subListTwo;
							subListTwo = subListTwo->Next;
							subListTwoSize--;
						}

						// subList2 is Empty, so pull from subListOne
						else if(subListTwoSize == 0 || subListTwo == NULL)
						{
							nextMergeElement = subListOne;
							subListOne = subListOne->Next;
							subListOneSize--;
						}

						// actual sorting portion, based on input comparison function
						// comparison is true, so first element is stored first
						else if(compareFunction(subListOne->Info, subListTwo->Info))
						{
							nextMergeElement = subListOne;
							subListOne = subListOne->Next;
							subListOneSize--;
						}

						// comparison is false, so second element is stored first
						else
						{
							nextMergeElement = subListTwo;
							subListTwo = subListTwo->Next;
							subListTwoSize--;
						}

						// adds new element to the merged list
						// if elements on the list, add to the back
						if(tail != NULL)
						{
							tail->Next = nextMergeElement;
						}

						// no current elements, so put on front
						else
						{
							HeadNode = nextMergeElement;
						}

						// move tail to the back
						tail = nextMergeElement;
					}

					// start from the where left off to merge two new sublist
					subListOne = subListTwo;
				}

				tail->Next = NULL;

				// only merged once, the list is sorted
				if(numMerges <= 1)
				{
					return;
				}

				// increase the size of the sublists
				subListSize *= 2;
			}
		}

		// returns the number of elements in the list
		int GetSize()
		{
			return Size;
		}

		// removes the element pointed at by the Iterator removeLoc
		Iterator Erase(Iterator removeLoc)
		{
			if(removeLoc != NULL)
			{
				ListNode * temp = removeLoc.Current;

				// update iterator
				removeLoc = temp->Next;

				// updates the previous node to point to the new next node.
				if(removeLoc.Previous != NULL)
				{
					removeLoc.Previous->Next = temp->Next;
				}

				// updates HeadNode if erasing first element
				if(HeadNode == temp)
				{
					HeadNode = temp->Next;
				}

				// remove element
				delete temp;
				temp = NULL;
				Size--;
			}

			// return iterator to next element
			return removeLoc;			
		}

	};
}