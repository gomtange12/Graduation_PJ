#pragma once

class Todo {
	int RoomNumber;  
	int mod;
	int m_ids[PERSONNEL];
public:
	Todo(int RoomNum, int mod)
		: RoomNumber(RoomNum), mod(mod) {}

	bool operator==(const Todo& t) const {
		if (RoomNumber == t.RoomNumber && mod == t.mod) return true;
		return false;
	}

	friend struct std::hash<Todo>;
};

namespace std {
	template <>
	struct hash<Todo> {
		size_t operator()(const Todo& t) const {
			hash<int> hash_func;

			return t.RoomNumber ^ (hash_func(t.mod));
		}
	};
}  // namespace std


class Room
{
public:
	
	int mod;
	bool m_full;
public:
	Room();
	~Room();

private:

};

