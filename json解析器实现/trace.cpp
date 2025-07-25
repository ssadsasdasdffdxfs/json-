#include "trace.h"
#include <sstream>
using namespace trace;
json::json() :m_type(json_null){}
json::json(bool value):m_type(json_bool) {
	m_value.m_bool = value;
}
json::json(int value):m_type(json_int) {
	m_value.m_int = value;
}
json::json(double value):m_type(json_double) {
	m_value.m_double = value;
}
json::json(const std::string& value) :m_type(json_string){
	m_value.m_string = new std::string(value);
}
json::json(Type type) :m_type(type) {
	switch (type) {
	case json_null:
		break;
	case json_bool:
		m_value.m_bool = false;
		break;
	case json_int:
		m_value.m_int = 0;
		break;
	case json_double:
		m_value.m_double = 0.0;
		break;
	case json_string:
		m_value.m_string = new std::string();
		break;
	case json_array:
		m_value.m_array = new std::vector<json>();
		break;
	case json_object:
		m_value.m_object = new std::map<std::string, json>();
		break;
	default:
		break;
	}
}
json::json(const json& other)
{
	clear();
	json::copy(other);
}
json::~json()
{
	clear();
}
void json::clear()
{
	switch (m_type) {
	case json_null:
		break;
	case json_bool:
		m_value.m_bool = false;
		break;
	case json_int:
		m_value.m_int = 0;
		break;
	case json_double:
		m_value.m_double = 0.0;
		break;
	case json_string:
		delete m_value.m_string;
		break;
	case json_array:
		for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); it++)
		{
			it->clear();
		}
		delete m_value.m_array;
		break;
	case json_object:
		for (auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); it++)
		{
			(it->second).clear();
		}
		delete m_value.m_object;
		break;
	default:
		break;
	}
	m_type = json_null;
}
json::operator bool()
{
	if (m_type != json_bool)
	{
		throw  std::logic_error("type error");
	}
	return m_value.m_bool;
}
json::operator int()
{
	if (m_type != json_int)
	{
		throw  std::logic_error("type error");
	}
	return m_value.m_int;
}
json::operator double()
{
	if (m_type != json_double)
	{
		throw  std::logic_error("type error");
	}
	return m_value.m_double;
}
json::operator std::string() 
{
	if (m_type != json_string)
	{
		throw  std::logic_error("type error");
	}
	return *(m_value.m_string);
}

json& json::operator[](int index)
{
	if (m_type != json_array)
	{
		if (m_type == json_null) {
			m_type = json_array;
			m_value.m_array = new std::vector<json>();
		}
		else {
			json a(*this);
			clear();
			m_type = json_array;
			m_value.m_array = new std::vector<json>();
			m_value.m_array->push_back(std::move(a));
		}
	}	
	size_t size = m_value.m_array->size();
	if (index < 0)
	{
		throw  std::logic_error("index < 0 :error");
	}
	if (index > size)
	{
		for (size_t i = size; i <= index; i++)
		{
			m_value.m_array->push_back(json());
		}
	}
	return (m_value.m_array)->at(index);
}
json& json::operator[] (const std::string& key)
{
	if (m_type != json_object)
	{
		clear();
		m_type = json_object;
		m_value.m_object = new std::map<std::string, json>();
	}
	return (*m_value.m_object)[key];
}
json& json::operator=(const json& other)
{
	clear();
	copy(other);
	return *this;
}
bool json::operator==(const json& other)
{
	if (m_type != other.m_type)
	{
		return false;
	}
	switch (m_type)
	{
	case json_null:
		return true;
	case json_bool:
		return m_value.m_bool == other.m_value.m_bool;
	case json_int:
		return m_value.m_int == other.m_value.m_int;
	case json_double:
		return m_value.m_double == other.m_value.m_double;
	case json_string:
		return *(m_value.m_string) == *(other.m_value.m_string);
	case json_array:
		return m_value.m_array == other.m_value.m_array;
	case json_object:
		return m_value.m_object == other.m_value.m_object;
	default:
		break;
	}
}
bool json::operator!=(const json& other)
{
	return !((*this) == other);
}

bool json::asBool() const
{
	if (m_type != json_bool)
	{
		throw std::logic_error("type error");
	}
	return m_value.m_bool;
}
int json::asInt() const
{
	if (m_type != json_int)
	{
		throw std::logic_error("type error");
	}
	return m_value.m_int;
}
double json::asDouble() const
{
	if (m_type != json_double)
	{
		throw std::logic_error("type error");
	}
	return m_value.m_double;
}
std::string json::asString() const
{
	if (m_type != json_string)
	{
		throw std::logic_error("type error");
	}
	return *(m_value.m_string);
}

bool json::has(size_t index)
{
	if (m_type != json_array)
	{
		return false;
	}
	size_t size = (m_value.m_array)->size();
	return (index >= 0 && index < size);
}
bool json::has(const std::string& key)
{
	if (m_type != json_object)
	{
		return false;
	}
	return ((m_value.m_object)->find(key) != (m_value.m_object)->end());
}

void json::remove(size_t index)
{
	if (has(index))
	{
		(m_value.m_array)->erase((m_value.m_array)->begin() + index);
	}
}
void json::remove(const std::string& key)
{
	if (has(key))
	{
		(m_value.m_object)->erase(key);
	}
}

void json::append(const json& other)
{
	if (m_type != json_array)
	{
		if (m_type == json_null){
			clear();
			m_type = json_array;
			m_value.m_array = new std::vector<json>();		
		}
		else {	
			json a(*this);	
			clear();
			m_type = json_array;
			m_value.m_array = new std::vector<json>();	
			m_value.m_array->push_back(std::move(a));
		}
	}
	m_value.m_array->push_back(other);
}


std::string json::str() const
{
	std::stringstream ss;
	switch (m_type)
	{
	case json_null:
		ss << "null";
		break;
	case json_bool:
		m_value.m_bool ? ss << "true" : ss << "false";
		break;
	case json_int:
		ss << m_value.m_int;
		break;
	case json_double:	
		ss << m_value.m_double;
		break;
	case json_string:
		ss << '\"' << *(m_value.m_string) << '\"';
		break;
	case json_array:
		ss << '[';
		for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end();it++) {
			if (it != (m_value.m_array)->begin())
			{
				ss << ',';
			}
			ss << it->str();
		}
		ss << ']';
		break;
	case json_object:
		ss << '{';
		for (auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); it++) {
			if (it != (m_value.m_object)->begin())
			{
				ss << ',';
			}
			ss << '\"' << it->first << "\":" << it->second.str();
		}
		ss << '}';
		break;
	default:
		break;
	}
	return ss.str();
}
void json::copy(const json& other)
{
	m_type = other.m_type;
	switch (m_type) {
	case json_null:
		break;
	case json_bool:
		m_value.m_bool = other.m_value.m_bool;
		break;
	case json_int:
		m_value.m_int = other.m_value.m_int;
		break;
	case json_double:
		m_value.m_double = other.m_value.m_double;
		break;
	case json_string:
		m_value.m_string = new std::string(*other.m_value.m_string);
		break;
	case json_array:
		m_value.m_array = new std::vector<json>(*other.m_value.m_array);
		break;
	case json_object:
		m_value.m_object = new std::map<std::string ,json > (*other.m_value.m_object);
		break;
	default:
		break;
	}
}