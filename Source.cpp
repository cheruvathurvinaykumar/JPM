#include<iostream>
#include<unordered_map>
#include<string>
#include<list>
#include<sstream>
#include <fstream>
#include<memory>
#include<algorithm>

using namespace std;
class product
{
public:
	product() = default;
	~product() = default;
	product(string prod_name, unsigned long val, unsigned long quan) :name(prod_name), total_value(val), total_quantity(quan) {}
	unsigned long getvalue()
	{
		return total_value;
	}
	unsigned long getquantity()
	{
		return total_quantity;
	}
	string  getname()
	{
		return name;
	}
	void add(int inc)
	{
		total_value += total_quantity*inc;
	}
	void subtract(int inc)
	{
		total_value -= total_quantity*inc;
	}
	void multiply(int inc)
	{
		total_value *= inc;
	}
	void operator +=(const product &x)
	{
		total_quantity += x.total_quantity;
		total_value += (x.total_quantity)*(x.total_value);
	}

private:
	string name;
	unsigned long total_quantity;
	unsigned long total_value;

};

/*Assuming single threaded, using an unprotected hash to store RECORD the sales*/
unordered_map<string, product>unmap;

enum class operation {ADD, SUB, MUL, INVALID_OP};
enum message_type{TYPE1, TYPE2, TYPE3, TYPE_INVALID};

/*Abstract Base class*/
class message
{
public:
	virtual void record_sales() =0;
	virtual ~message() {}
	
};

/* Message class */
class message2 :public message
{
public:
	message2(string t_name, unsigned long t_value, unsigned long t_quantity=1) : prod_name(t_name,t_value, t_quantity) {}
	void record_sales() override 
	{ 
		unmap[prod_name.getname()] += prod_name; 
		//cout << endl << "Adding  " << prod_name.getquantity()<< " " << prod_name.getname() << " for " << prod_name.getvalue();
	}
	virtual ~message2() {}
private:
	product prod_name;
};
/* Message class with OPERATION */
class message3 :public message
{
public:
	message3(string t_name,  unsigned long t_value, unsigned long t_quantity, operation in_op, int in_operand) :
		prod_name(t_name, t_value, t_quantity), op(in_op), operand(in_operand) {}
	
	void record_sales() override 
	{ 
		unmap[prod_name.getname()] += prod_name; 
		switch(op)
		{
		case operation::ADD: 
			unmap[prod_name.getname()].add(operand);
			break;
		case operation::SUB:
			unmap[prod_name.getname()].subtract(operand);
			break;
		case operation::MUL:
			unmap[prod_name.getname()].multiply(operand);
			break;
		default:
			break;

		}

	}
	virtual ~message3() {}
private:
	product prod_name;
	operation op;
	int operand;

};

/* This function extracts the operation from the message string and returns an enum class value*/
operation get_operation(string op)
{
	operation return_op = operation::INVALID_OP;
	if ("ADD" == op)
	{
		return_op = operation::ADD;
	}
	else if ("MUL" == op)
	{
		return_op = operation::MUL;
	}
	else if ("SUB" == op)
	{
		return_op = operation::SUB;
	}
	else
	{
		return_op = operation::INVALID_OP;
	}
	return return_op;
}

/* This function creates and returns a message(abstract class) pointer from the message and the type of message */
unique_ptr<message> create_message(vector<string>words, message_type message_t)
{

	unique_ptr <message> message_ref;

	if (message_t != TYPE_INVALID)
	{
		string product_name = words[0];
		istringstream buffer(words[1]);
		unsigned long value = 0;
		buffer >> value;
		switch (message_t)
		{
			case TYPE1:
			{
				//cout << endl << "Creating message using " << product_name << " " << value << " ";
				message_ref = make_unique<message2>(product_name, value);
				break;
			}
			case TYPE2:
			{
				unsigned long quantity = 0;
				istringstream buffer_q(words[2]);
				buffer_q >> quantity;
				//cout << endl << "Creating message using " << product_name << " " << value << " " << quantity ;
				message_ref = make_unique<message2>(product_name, value, quantity);
				break;
			}
			case TYPE3:
			{
				unsigned long quantity = 0;
				operation op = operation::INVALID_OP;
				unsigned long operand = 0;
				istringstream buffer_q(words[2]);
				buffer_q >> quantity;
				op = get_operation(words[3]);
				if (op != operation::INVALID_OP)
				{
					istringstream buffer_op(words[4]);
					buffer_op >> operand;
					//cout << endl << "Creating message using " << product_name << " " << value << " " << quantity << " " << static_cast<int>(op) << " " << operand;
					message_ref = make_unique<message3>(product_name, value, quantity, op, operand);
				}
				else
				{
					cout << endl << "INVALID Operation";
				}
				break;
			}
			default:
				break;
		}
	}
	else
	{
		cout << endl << "MESSAGE ERROR" << endl;
		for (auto i : words)
		{
			cout << i << " ";
		}
	}

	return message_ref;

}

/* This function dynamically calls record_sales based on the derived class object*/
void process_message(unique_ptr<message> msg)
{
	msg->record_sales();
}

/* This function initially builds the message queue from the messages stored in the file. 
Change the test file name *.txt to run different test scenarios*/
void build_message_queue(list<unique_ptr<message>>& message_list)
{
	ifstream infile("test1.txt");

	std::string line;

	while (std::getline(infile, line))
	{
		unique_ptr<message> msg_ptr;
		//cout << endl << "Reading: " << line << endl;

		message_type message_t = message_type::TYPE_INVALID;
		istringstream split(line);
		const char delim = ',';
		int word_count = 0;
		vector<string> words;
		for (string each; getline(split, each, delim);remove(each.begin(), each.end(), ' '), words.push_back(each), word_count++);
		switch (word_count)
		{
		case 2:
			message_t = TYPE1;
			break;
		case 3:
			message_t = TYPE2;
			break;
		case 5:
			message_t = TYPE3;
			break;
		default:
			break;
		}

		msg_ptr = create_message(words, message_t);
		if (msg_ptr != nullptr)
		{
			message_list.push_back(move(msg_ptr));
		}
		else
		{
			//cout << endl << "MESSAGE UNIQUE POINTER IS NULL" << endl;
		}

		cout << endl;
	}

}
int main()
{
	list<unique_ptr<message>> message_list;
	build_message_queue(message_list);//This function reads messages in a CSV format and creates a message queue.
	
	int count = 0;

	while (!message_list.empty())
	{
		if ((0 == count % 50) && (count != 0))
		{
			cout << endl << "PROCESSED 50 MESSAGES. EXITING" << endl;
			break;
		}
		else
		{
			unique_ptr<message>  msg_ptr = std::move(message_list.front());
			message_list.pop_front();
			process_message(std::move(msg_ptr)); //This function rocesses each message.
			++count;
			if (0 == count % 10)
			{
				cout << endl << "ADDED 10 ITEMS ---------------------------------------" <<endl;
				for (auto i : unmap)
				{
					cout << i.first << "  " << i.second.getquantity() << " units for :  " << i.second.getvalue() << " pence" << endl;
				}
				cout << endl << "-----------------------------------------------------" << endl;
			}
		}
		

	}
	cout << endl << "FINAL STATUS -----------------------------------------" << endl;
	for (auto i : unmap)
	{
		cout << i.first << "  " << i.second.getquantity() << " units for :  " << i.second.getvalue() << " pence" << endl;
	}
	cout << endl << "------------------------------------------------------" << endl;


	system("pause");
	return 0;
}