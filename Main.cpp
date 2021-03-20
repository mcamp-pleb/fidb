#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include "IOData.hpp"
#include "Entry.hpp"

const float PROGRAM_VER = 1.02f;
const int ARGUMENT_SIZE = 50;

void default_info();
void help_page();
void print_version();
void createDB(const std::string &dir);
void addEntry(const std::string &dir);
void editEntry(const std::string &dir, std::vector<std::string>::iterator &vec,
	const std::vector<std::string>::iterator &end);
void removeEntry(const std::string &dir, std::vector<std::string>::iterator &vec,
	const std::vector<std::string>::iterator &end);
void printStats(const std::string &dir, std::vector<std::string>::iterator &vec,
		const std::vector<std::string>::iterator &end);
void formatEntrySearchParam(Entry &entry, std::vector<std::string>::iterator &vec,
	const std::vector<std::string>::iterator &end);
bool isNextParam(const std::vector<std::string>::iterator &param,
				const std::vector<std::string>::iterator &last);

int main(int argc, char *argv[])
{
	std::string filePath = "";
	
	if(argc == 1)
	{
		//When no arguments are used; print out how to get help
		default_info();
	}
	else
	{
		//Converting Cstring arguments to C++ string data types
		std::vector<std::string> argstr;

		for(int i = 1; i < argc; i++)
		{
			size_t cstrSize = strlen(argv[i]);
			cstrSize = (cstrSize < ARGUMENT_SIZE) ? cstrSize : ARGUMENT_SIZE;

			std::string temp(argv[i], cstrSize);

			//If the help or version argument is used; clear array to allow clean program exit
			if(temp == "-h" || temp == "--help" || temp == "-v" || temp == "--version")
			{
				argstr.clear();
				argstr.push_back(temp);
				break;
			}
			if(temp.front() != '-')
			{
				//Slightly confusing but much more simple to execute
				//Three possibilities x, x=, xx...
				//This checks for x= and then reverses the boolean
				//So if the input is x or xx... the input is treated as a file path
				//Then if the input is x=, the input is pushed into the argstr vector
				if(!(temp.size() > 1 && temp.at(1) == '='))
				{
					filePath = temp;
					continue;
				}
			}
			argstr.push_back(temp);
		}

		for(std::vector<std::string>::iterator i = argstr.begin(); i != argstr.end(); i++)
		{
			//Handle option arguments
			if(i->front() == '-')
			{
				if(*i == "-h" || *i == "--help")
				{
					help_page();
				}
				else if(*i == "-v" || *i == "--version")
				{
					print_version();
				}
				else if(*i == "-n" || *i == "--new")
				{
					createDB(filePath);
				}
				else if(*i == "-a" || *i == "--add")
				{
					addEntry(filePath);
				}
				else if(*i == "-e" || *i == "--edit")
				{
					editEntry(filePath, i, argstr.end());
				}
				else if(*i == "-r" || *i == "--remove")
				{
					removeEntry(filePath, i, argstr.end());
				}
				else if(*i == "-l" || *i == "--list")
				{
					printStats(filePath, i, argstr.end());
				}
			}
		}
	}

	return 0;
}

void default_info()
{
	std::cout << "This program must include a file or the argument -n to create a new database." 
	<< std::endl;
	std::cout << "Use -h or --help for more information." << std::endl;

}

void help_page()
{
	std::cout << "Usage: fidb [OPTION] [SUB-OPTIONS] [FILE]" << std::endl;
	std::cout << "Digital checkbook for transaction records" << std::endl << std::endl;
	std::cout << "[OPTIONS]" << std::endl;
	std::cout << "  -a, --add\t" << "Adds a new entry" << std::endl;
	std::cout << "  -e, --edit\t" << "Edits an existing entry" << std::endl;
	std::cout << "  -h, --help\t" << "You are already here <--" << std::endl;
	std::cout << "  -l, --list\t" << "lists entries" << std::endl;
	std::cout << "  -n, --new\t" << "Creates a new fidb database file" << std::endl;
	std::cout << "  -r, --remove\t" << "removes an entry" << std::endl;
	std::cout << "  -v, --version\t" << "Prints application version" << std::endl << std::endl;
	std::cout << "[SUB-OPTIONS] -- Can only be used with edit, list, and remove(i= only)" 
		<< std::endl;
	std::cout << "  i=\t" << "Entry ID" << std::endl;
	std::cout << "  v=\t" << "Vendor" << std::endl;
	std::cout << "  o=\t" << "Transaction/Order #" << std::endl;
	std::cout << "  d=\t" << "Description" << std::endl;
	std::cout << "  y=\t" << "Year" << std::endl;
	std::cout << "  m=\t" << "Month" << std::endl;
	std::cout << "  p=\t" << "Price/Amount" << std::endl << std::endl;
	std::cout << "If entering non numeric data (Vendor/Description/Transaction) use "
		<< "quotations when entering punctuation. " << std::endl;
}

void print_version()
{
	std::cout << "fidb version " << PROGRAM_VER << std::endl;
}

void createDB(const std::string &dir)
{
	IOData db;

	if(db.createDB(dir))
	{
		std::cout << "New file created" << std::endl;
	}
}

void addEntry(const std::string &dir)
{
	IOData db;

	if(db.saveEntry(dir))
	{
		std::cout << "Entry added" << std::endl;
	}
}

void editEntry(const std::string &dir, std::vector<std::string>::iterator &vec,
		const std::vector<std::string>::iterator &end)
{
	IOData db;
	Entry search;

	formatEntrySearchParam(search,vec, end);
	db.editEntry(HandleType::EDIT, search.getID(), dir, search.getVendor(), search.getOrderNum(),
	search.getDescription(), search.getYear(), search.getMonth(), search.getPrice());
}

void removeEntry(const std::string &dir, std::vector<std::string>::iterator &vec,
		const std::vector<std::string>::iterator &end)
{
	if(isNextParam(vec, end))
	{
		IOData db;
		std::size_t	pos	= 0;
		unsigned int id = 0;

		vec++;
		pos = vec->find('=') + 1;
		id = std::stoul(vec->substr(pos));
		db.editEntry(HandleType::REMOVE, id, dir);
	}
}

void printStats(const std::string &dir, std::vector<std::string>::iterator &vec,
	const std::vector<std::string>::iterator &end)
{
	IOData db;
	Entry search;
	
	formatEntrySearchParam(search, vec, end);
	db.searchPrint(search.getID(), dir, search.getVendor(), search.getOrderNum(),
	search.getDescription(), search.getYear(), search.getMonth(), search.getPrice());
}

bool isNextParam(const std::vector<std::string>::iterator &param,
				const std::vector<std::string>::iterator &last)
{
	if(param == std::prev(last))
	{
		return false;
	}

	return std::next(param)->front() == '-' ? false : true;
}

void formatEntrySearchParam(Entry &entry, std::vector<std::string>::iterator &vec,
	const std::vector<std::string>::iterator &end)
{
	std::string		copy	= "";
	std::string		vend	= "";
	std::string		ord		= "";
	std::string 	des		= "";
	unsigned int	id		= 0;
	unsigned int	year	= 0;
	unsigned int	month	= 0;
	double			price	= 0.0;
	std::size_t		pos		= 0;

	while(isNextParam(vec, end))
	{
		vec++;
		pos = vec->find('=') + 1;
		copy = vec->substr(pos);
		switch(vec->front())
		{
			case 'i':
				id = std::stoul(copy);
				break;
			case 'y':
				year = std::stoul(copy);
				break;
			case 'm':
				month = std::stoul(copy);
				break;
			case 'p':
				price = std::stod(copy);
				break;
			case 'v':
				vend = copy;
				break;
			case 'o':
				ord = copy;
				break;
			case 'd':
				des = copy;
				break;
			default:
				break;
		}
	}
	entry.setID(id);
	entry.setYear(year);
	entry.setMonth(month);
	entry.setPrice(price);
	entry.setVendor(vend);
	entry.setOrderNum(ord);
	entry.setDescription(des);
}