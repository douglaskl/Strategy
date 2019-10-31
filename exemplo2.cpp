#include <cassert>
#include <iostream>
#include <string>
using namespace std;

class Record
{
public:
    virtual void start_record() = 0;
    virtual void store_field(const string &name;, const string &value;) = 0;
    virtual void finish_record() = 0;
    virtual ~Record() { }
};

struct ContactData
{
    string first_name, last_name, phone, email;
};

class ContactRecorder
{
public:
    ContactRecorder(Record *a) : m_Record(a)
    {
        assert(a != 0);
    }

    void store(const ContactData &data;)
    {
        assert(m_Record != 0);

        m_Record->start_record();
        m_Record->store_field("first name", data.first_name);
        m_Record->store_field("last name", data.last_name);
        m_Record->store_field("phone", data.phone);
        m_Record->store_field("email", data.email);
        m_Record->finish_record();
    }

private:
    Record *m_Record;
};

class StreamRecord : public Record
{
public:
    StreamRecord(ostream &s;, const string &record;_name = string())
        : m_ostream(s), m_record_name(record_name)
    { }

    void start_record() { m_ostream << m_record_name << "( "; }

    void store_field(const string &name;, const string &value;)
    {
        m_ostream << name << ": " << value << "; ";
    }

    void finish_record() { m_ostream << ")" << endl; }

    void set_record_name(const string &name;) { m_record_name = name; }

private:
    ostream &m;_ostream;
    string m_record_name;
};

class MySql {};

class DatabaseRecord : public Record
{
public:
    DatabaseRecord() : m_dbConnection(new MySql) {}

    void start_record() { cout << "start transaction\n"; }

    void store_field(const string &name;, const string &value;)
    { cout << "insert into table\n"; }

    void finish_record() { cout << "finish transaction\n"; }

private:
    MySql *m_dbConnection;
};

int main()
{
	ContactData data = {"Black", "Smith", "415-675-1874", "adams@email.com"};
	StreamRecord sRecord(std::cout);
	ContactRecorder contact(&sRecord;);
	contact.store(data);

	DatabaseRecord dbRecord;
	ContactRecorder contact2(&dbRecord;);
	contact2.store(data);
	return 0;
}

/*
Output:

( first name: Black; last name: Smith; phone: 415-675-1874; email: smith@email.com; )
start transaction
insert into table
insert into table
insert into table
insert into table
finish transaction
*/
