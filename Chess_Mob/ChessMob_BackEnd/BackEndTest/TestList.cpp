#include "stdafx.h"
#include "CppUnitTest.h"
#include "../List.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ChessMob;

namespace BackEndTest
{
	TEST_CLASS(TestList)
	{
	public:

		TEST_METHOD(TestPushFront)
		{
			List<int> testList;

			int elementVal = 1;

			// adds an element to testList
			testList.PushFront(elementVal);
			Assert::AreEqual(*testList.Begin(), 1);
			Assert::AreEqual(1, testList.GetSize());

			// adds more elements to testList
			elementVal++;
			testList.PushFront(elementVal);
			elementVal++;
			testList.PushFront(elementVal);
			elementVal++;
			testList.PushFront(elementVal);
			Assert::AreEqual(*testList.Begin(), 4);
		}

		TEST_METHOD(TestPopFront)
		{
			List<int *> testList;
			int * elementVal = new int(1);

			// adds elements to testList
			testList.PushFront(elementVal);
			elementVal = new int(2);
			testList.PushFront(elementVal);
			elementVal = new int(3);
			testList.PushFront(elementVal);

			// remove an element
			testList.PopFront();
			Assert::AreEqual(2, *(*testList.Begin()));
			Assert::AreEqual(2, testList.GetSize());

			// test emptying the set
			testList.PopFront();
			testList.PopFront();
			Assert::AreEqual(true, testList.Begin()==testList.End());
			Assert::AreEqual(0, testList.GetSize());

			// test over emptying
			testList.PopFront();
			Assert::AreEqual(0, testList.GetSize());
		}

		TEST_METHOD(TestEmpty)
		{
			List<int> testList;

			// checks list intializes correctly
			Assert::AreEqual(true, testList.Empty());

			// checks not true for non empty
			testList.PushFront(1);
			testList.PushFront(2);
			Assert::AreEqual(false, testList.Empty());

			// checks reempties correctly
			testList.PopFront();
			testList.PopFront();
			Assert::AreEqual(true, testList.Empty());
		}

		TEST_METHOD(TestRemove)
		{
			List<int> testList;

			testList.PushFront(1);
			testList.PushFront(2);
			testList.PushFront(1);
			testList.PushFront(1);
			testList.PushFront(3);
			testList.PushFront(1);

			// checks removes multiples
			testList.Remove(1);
			Assert::AreEqual(2, testList.GetSize());

			// checks work when item not in the list
			testList.Remove(4);
			Assert::AreEqual(2, testList.GetSize());
		}

		TEST_METHOD(TestErase)
		{
			List<int> testList;

			List<int>::Iterator testListIterator;

			// generate list
			testList.PushFront(1);
			testList.PushFront(2);
			testList.PushFront(3);
			testList.PushFront(4);

			// test removing first item
			testListIterator = testList.Begin();
			testListIterator = testList.Erase(testListIterator);
			Assert::AreEqual(3, testList.GetSize());
			Assert::AreEqual(3, *testListIterator);

			// test removing from the middle
			++testListIterator;
			testListIterator = testList.Erase(testListIterator);
			Assert::AreEqual(2, testList.GetSize());
			Assert::AreEqual(1, *testListIterator);
			Assert::AreEqual(3, *(testList.Begin()));

			// test removing iterator to null
			testListIterator = testList.End();
			testList.Erase(testListIterator);
			Assert::AreEqual(2, testList.GetSize());
		}
		
		static bool compareInt(int int1, int int2)
		{ 
			return int1 > int2;
		}

		TEST_METHOD(TestSort)
		{
			List<int> testList;
			List<int>::Iterator element1Index, element2Index;

			int counter = 0;

			// create random list
			int currentNumElements;
			for(currentNumElements = 0; currentNumElements < 20; currentNumElements++)
			{
				testList.PushFront(rand()%5);
			}

			// sort
			testList.Sort(&compareInt);
			
			element1Index = testList.Begin();
			if(element1Index != NULL)
			{
				element2Index = testList.Begin();
				++element2Index;
			}
			for(; element2Index != testList.End(); ++element1Index, ++element2Index)
			{
				Assert::AreEqual(true, *element1Index >= *element2Index);
				counter++;
			}

			Assert::AreEqual(19, counter);
		}
	};
}