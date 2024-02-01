#pragma once

template <typename T>
class ObjectArray
{
public:
	//Returns a pointer to the object that was added
	T* addObject(T& object);
	void addObject(T* object);

	void removeObject(unsigned index);
	void removeObject(T* object);

	void clear();

	T& operator[](unsigned index);
	const T& operator[](unsigned index) const;

	unsigned getSize() const { return this->size;}

public:
	ObjectArray();
	ObjectArray(const ObjectArray<T>& other);
	ObjectArray& operator=(const ObjectArray<T>& other);
	~ObjectArray();

private:
	void copyFrom(const ObjectArray<T>& other);
	void free();
	void resize();

private:
	T** objects;
	unsigned size;
	unsigned capacity = 4;
};

template <typename T>
ObjectArray<T>::ObjectArray()
{
	objects = new T*[capacity];
	size = 0;
}

template <typename T>
ObjectArray<T>::ObjectArray(const ObjectArray& other)
{
	copyFrom(other);
}

template <typename T>
ObjectArray<T>& ObjectArray<T>::operator=(const ObjectArray<T>& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}

template <typename T>
ObjectArray<T>::~ObjectArray()
{
	free();
}

template <typename T>
void ObjectArray<T>::copyFrom(const ObjectArray<T>& other)
{
	objects = new T * [other.capacity];
	for (int i = 0; i < other.size; i++)
	{
		objects[i] = other.objects[i]->clone();
	}
	size = other.size;
	capacity = other.capacity;
}

template <typename T>
void ObjectArray<T>::free()
{
	for (size_t i = 0; i < size; i++)
	{
		delete objects[i];
	}
	delete[] objects;
}

template <typename T>
void ObjectArray<T>::resize()
{
	capacity *= 2;
	T** temp = new T* [capacity];
	for (int i = 0; i < size; i++)
	{
		temp[i] = objects[i];
	}
	delete[] objects;
	objects = temp;
}

template <typename T>
T* ObjectArray<T>::addObject(T& object)
{
	if (size == capacity)
	{
		resize();
	}
	objects[size++] = object.clone();
	return objects[size - 1];
}

template <typename T>
void ObjectArray<T>::addObject(T* object)
{
	if (size == capacity)
	{
		resize();
	}
	objects[size++] = object;
}

template <typename T>
T& ObjectArray<T>::operator[](unsigned index)
{
	return *objects[index];
}

template <typename T>
const T& ObjectArray<T>::operator[](unsigned index) const
{
	return *objects[index];
}

template <typename T>
void ObjectArray<T>::removeObject(unsigned index)
{
	if (index >= size)
	{
		return;
	}
	delete objects[index];
	objects[index] = nullptr;
	for (int i = index; i < size - 1; i++)
	{
		objects[i] = objects[i + 1];
	}
	size--;
}

template <typename T>
void ObjectArray<T>::removeObject(T* object)
{
for (int i = 0; i < size; i++)
	{
		if (objects[i] == object)
		{
			removeObject(i);
			return;
		}
	}
}

template <typename T>
void ObjectArray<T>::clear()
{
	free();
	size = 0;
}

