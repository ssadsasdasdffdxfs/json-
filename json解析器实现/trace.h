#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <map>
namespace trace {
	class json {
	public:
		enum Type {
			json_null,
			json_bool,
			json_int,
			json_double,
			json_string,
			json_array,
			json_object
		};
		json();
		json(bool value);
		json(int value);
		json(double value);
		json(const std::string& value);
		json(Type type);
		json(const json& other);
		
		operator bool();
		operator int();
		operator double();
		operator std::string();

		json& operator[](int index);
		json& operator[](const std::string& key);
		json& operator=(const json& other);
		bool operator==(const json& other);
		bool operator!=(const json& other);

		bool isNull() const { return m_type == json_null; }
		bool isBool() const { return m_type == json_bool; }
		bool isInt() const { return m_type == json_int; }
		bool isDouble() const { return m_type == json_double; }
		bool isString() const { return m_type == json_string; }
		bool isArray() const { return m_type == json_array; }
		bool isObject() const { return m_type == json_object; }

		bool asBool() const;
		int asInt() const;
		double asDouble() const;
		std::string asString() const;

		bool has(size_t index);
		bool has(const std::string& key);

		void remove(size_t index);
		void remove(const std::string& key);
		~json();
		void clear();

		void append(const json& other);
		std::string str() const;

		typedef std::vector<json>::iterator iterator;
		iterator begin() {
			if (m_type != json_array) {
				return iterator();
			}
			return m_value.m_array->begin();
		}
		iterator end() {
			if (m_type != json_array) {
				return iterator();
			}
			return m_value.m_array->end();
		}

	private :
		union Value {
			bool m_bool;
			int m_int;
			double m_double;
			std::string* m_string;
			std::vector<json>* m_array;
			std::map<std::string, json>* m_object;
		};
		Type m_type;
		Value m_value;

		void copy(const json& other);
	};

}