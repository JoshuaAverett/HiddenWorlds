#pragma once

template <class T>
class HWLinkedList
{
private:
	private:
	class HWLinkedListNode
	{
	public:
		HWLinkedListNode()
		{
		}

		const T* value;
		HWLinkedListNode* next;
	};

public:
	HWLinkedList()
	{
	}

	~HWLinkedList()
	{
	}

	void Add(const T& value)
	{
		HWLinkedListNode temp = HWLinkedListNode();
		temp.value = &value;
		temp.next = first;
		first = &temp;
	}

	HWLinkedListIterator GetIterator()
	{
		return HWLinkedListIterator(this);
	}

private:
	HWLinkedListNode* first;
};

template <class T>
class HWLinkedListIterator
{
public:
	frient class HWLi

	HWLinkedListIterator()
	{
	}

	HWLinkedListIterator(const HWLinkedList<T>& lst)
	{
		node = lst.first;
	}

	~HWLinkedListIterator()
	{
	}

	T GetFirst()
	{
		return node.value;
	}

	void GetNext()
	{
		node = node.next;
	}

	S32 HasNext()
	{
		return node.next != nullptr;
	}

private:
	HWLinkedList<T>::HWLinkedListNode* node;
};
