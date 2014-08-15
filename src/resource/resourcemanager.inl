#include <iostream>

namespace flat
{
namespace resource
{

template <class T> T* ResourceManager<T>::get(std::string name)
{
	T* resource;
	typename std::map<std::string, T*>::iterator it = m_resources.find(name);

	if (it != m_resources.end())
		resource = it->second;
		
	else
		resource = load(name);

	return resource;
}

template <class T> T* ResourceManager<T>::load(std::string name)
{
	T* resource = new T(name);
	m_resources[name] = resource;
	return resource;
}

} // resource
} // flat


