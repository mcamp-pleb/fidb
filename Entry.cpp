#include "Entry.hpp"

Entry::Entry()
:m_year(0), m_month(0), m_price(0.0), m_id(0), m_vendor(""), m_description(""), m_orderNum("")
{
}

Entry::Entry(unsigned int year, unsigned int month, double price, unsigned int id,
			std::string vendor, std::string description, std::string orderNum)
:m_year(year), m_month(month), m_vendor(vendor), m_description(description), m_price(price)
{
}

Entry::~Entry()
{
}

void Entry::addNewEntry(const std::string &dbTitle, unsigned int &id)
{
	const std::string MAX_DOUBLE = std::to_string(std::numeric_limits<double>::max());
	const std::string MAX_YEAR = std::to_string(std::numeric_limits<unsigned int>::max());
	const std::string MAX_MONTH = "12";
	std::string year = "";
	std::string month = "";
	std::string price = "";

	//Increment the header item total and apply to entry as ID
	//The ID must be unique for indexing; Databaseing 101
	id++;
	setID(id);

	std::cout << "Account: " << dbTitle << std::endl << "Add transaction:" << std::endl;
	std::cout << "Vendor: ";
	std::getline(std::cin, m_vendor);
	std::cout << "Order #: ";
	std::getline(std::cin, m_orderNum);
	std::cout << "Description of transaction: ";
	std::getline(std::cin, m_description);

	//Format the strings by setting a max string size
	m_vendor.resize(MAX_STR_SIZE, ' ');
	m_orderNum.resize(MAX_STR_SIZE, ' ');
	m_description.resize(MAX_STR_SIZE, ' ');

	//Next get numbers and translate from strings
	//Its less of a hassle to use getline and convert rather than checking for streaming errors
	//The ID is auto generated for indexing
	std::cout << "Year(YYYY): ";
	std::getline(std::cin, year);
	std::cout << "Month(MM): ";
	std::getline(std::cin, month);
	std::cout << "Price: ";
	std::getline(std::cin, price);

	if(year.size() > MAX_YEAR.size() - 1)
	{
		year = MAX_YEAR;
	}
	if(month.size() > MAX_MONTH.size())
	{
		month = MAX_MONTH;
	}
	if(price.size() > MAX_DOUBLE.size() - 1)
	{
		price = MAX_DOUBLE;
	}
	setYear(std::stoul(year));
	setMonth(std::stoul(month));
	setPrice(std::stod(price));
}

void Entry::appendEntry(const unsigned int &year, const unsigned int &month, const double &price,
	const unsigned int &id,	const std::string &vendor, const std::string &description,
	const std::string &orderNum)
{
	setYear(year);
	setMonth(month);
	setPrice(price);
	setID(id);
	setVendor(vendor);
	setDescription(description);
	setOrderNum(orderNum);
}

bool Entry::isMatched(const unsigned int &id, const unsigned int year, const unsigned int &month,
			const double &price, const std::string &vendor, const std::string &order,
			const std::string &description)
{
	//The entry ID must be 1 or more; if not just skip any matching
	if(getID() < 1)
	{
		return false;
	}
	if(id != 0 && getID() != id)
	{
		return false;
	}
	if(year != 0 && getYear() != year)
	{
		return false;
	}
	if(month != 0 && getMonth() != month)
	{
		return false;
	}
	if(price != 0.0 && getPrice() != price)
	{
		return false;
	}
	if(!vendor.empty() && !compare(vendor, getVendor()))
	{
		return false;
	}
	if(!order.empty() && !compare(order, getOrderNum()))
	{
		return false;
	}
	if(!description.empty() && !compare(description, getDescription()))
	{
		return false;
	}

	return true;
}

void Entry::print()
{
	if(m_id > 0)
	{
		std::cout << "ID: " << m_id << std::endl;
		std::cout << "Vendor: " << m_vendor << std::endl;
		std::cout << "Order# " << m_orderNum << std::endl;
		std::cout << "Desc: " << m_description << std::endl;
		std::cout << "Year: " << m_year << std::endl;
		std::cout << "Month: " << m_month << std::endl;
		std::cout << ":>" <<  std::fixed << std::setprecision(2) << m_price << std::endl 
		<< std::endl;
	}
}

void Entry::clear()
{
	m_vendor.clear();
	m_vendor.resize(MAX_STR_SIZE, ' ');
	m_orderNum.clear();
	m_orderNum.resize(MAX_STR_SIZE, ' ');
	m_description.clear();
	m_description.resize(MAX_STR_SIZE, ' ');
	m_id = 0;
	m_year = 0;
	m_month = 0;
	m_price = 0.0;
}

void Entry::setVendor(const std::string &vendor)
{
	if(!vendor.empty())
	{
		m_vendor = vendor;
		//m_vendor.resize(MAX_STR_SIZE, ' ');
	}
}

void Entry::setOrderNum(const std::string &orderNum)
{
	if(!orderNum.empty())
	{
		m_orderNum = orderNum;
		//m_orderNum.resize(MAX_STR_SIZE, ' ');
	}
}

void Entry::setDescription(const std::string &descr)
{
	if(!descr.empty())
	{
		m_description = descr;
		//m_description.resize(MAX_STR_SIZE, ' ');
	}
}

void Entry::setID(const unsigned int &id)
{
	if(m_id == 0 && id != 0)
	{
		m_id = id;
	}
}
void Entry::setYear(const unsigned int &year)
{
	if(year != 0)
	{
		m_year = year;
	}
}
void Entry::setPrice(const double &price)
{
	if(price != 0)
	{
		m_price = price;
	}
}

void Entry::setMonth(const unsigned int &month)
{
	if(month != 0)
	{
		m_month = month;
		
		if(m_month > 12)
		{
			m_month = 12;
		}
		else if(m_month < 1)
		{
			m_month = 1;
		}
	}
}

//NOTE: I believe this function will only work with utf8 characters
//Returns true if strings are similar; does not need to be exact same
bool Entry::compare(const std::string &str1, const std::string &str2)
{
	std::string copy1;
	std::string copy2;

	//Don't bother comparing if str1 is empty
	if(str1.empty())
	{
		return false;
	}

	for(unsigned int i = 0; i < str1.size(); i++)
	{
		copy1.push_back(std::tolower(str1.at(i)));
	}

	for(unsigned int i = 0; i < str2.size(); i++)
	{
		copy2.push_back(std::tolower(str2.at(i)));
	}
	
	if(copy2.find(copy1) == std::string::npos)
	{
		return false;
	}
	
	return true;
}