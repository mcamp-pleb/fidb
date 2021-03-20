#include "IOData.hpp"

IOData::IOData()
:m_results(0)
{

}

IOData::~IOData()
{

}

bool IOData::createDB(const std::string &dir)
{
	std::ofstream writeFile;
	
	//Check if file already exists
	if(fileExists(dir))
	{
		std::cout << "File already exists" << std::endl;
		return false;
	}
	
	writeFile.open(dir, std::ios::binary);

	//Check if file can be saved
	if(!writeFile.is_open())
	{
		std::cout << "File could not be created" << std::endl;
		return false;
	}

	//Create file metadata
	setMetadata(m_header);
	m_header.stringSize = m_header.Title.size();

	//Save metadata to file
	writeFile.write(reinterpret_cast<char*>(&m_header.ItemCount), sizeof(MetaData::ItemCount));
	writeFile.write(reinterpret_cast<char*>(&m_header.OriginYear), sizeof(MetaData::OriginYear));
	writeFile.write(reinterpret_cast<char*>(&m_header.stringSize), sizeof(MetaData::stringSize));
	writeFile.write(m_header.Title.c_str(), m_header.stringSize);

	writeFile.close();

	return true;
}

bool IOData::saveEntry(const std::string &dir)
{	
	std::ofstream out;
	std::ifstream in;
	Entry newEntry;

	//Check if file already exists
	if(!fileExists(dir))
	{
		std::cout << "File does not exists" << std::endl;
		return false;
	}
	
	//Tip for future me: to prevent file truncation on writing to a file -- include ios::in
	//ios::in is not required with ios::app because it will only write at the end by default
	out.open(dir, std::ios::in | std::ios::binary | std::ios::ate);

	//Check if file can be saved
	if(!out.is_open())
	{
		std::cout << "File could not opened" << std::endl;
		return false;
	}
	
	//Load metadata
	in.open(dir, std::ios::binary);
	getMetadata(in);
	in.close();

	//Create new entry
	newEntry.addNewEntry(m_header.Title, m_header.ItemCount);

	//Save entry to file
	writeEntry(newEntry, out);

	//Append the item count for future indexing
	updateMetadataItemCount(out);
	out.close();

	return true;
}

void IOData::getMetadata(std::ifstream &readFile)
{
	readFile.read(reinterpret_cast<char*>(&m_header.ItemCount), sizeof(MetaData::ItemCount));
	readFile.read(reinterpret_cast<char*>(&m_header.OriginYear),sizeof(MetaData::OriginYear));
	readFile.read(reinterpret_cast<char*>(&m_header.stringSize),sizeof(MetaData::stringSize));
	m_header.Title.resize(m_header.stringSize);
	readFile.read(&m_header.Title.front(),m_header.stringSize);
}

void IOData::setMetadata(MetaData &data)
{
	const std::string MAX_INT = std::to_string(std::numeric_limits<unsigned int>::max());
	std::string tmpYear = "";
	
	//Get values from user
	std::cout << "fidb title: ";
	std::getline(std::cin, data.Title);
	std::cout << "Initial Year: ";
	std::getline(std::cin, tmpYear);

	if(data.Title.size() > MAX_STR_SIZE)
	{
		data.Title.resize(MAX_STR_SIZE, ' ');
	}

	//Its less of a hassle to use getline and convert rather than checking for streaming errors
	//subtracting the last character on the max size allows a 10 digit buffer on the max int size
	if(tmpYear.size() > MAX_INT.size() - 1)
	{
		tmpYear = MAX_INT;
	}
	data.OriginYear = std::stoul(tmpYear);

}

void IOData::updateMetadataItemCount(std::ofstream &writeFile)
{
	writeFile.seekp(0);
	writeFile.write(reinterpret_cast<char*>(&m_header.ItemCount), sizeof(MetaData::ItemCount));
}

//HERE -> check saving and formatting strings
void IOData::writeEntry(const Entry &entry, std::ofstream &writeFile)
{
	//Temp string to format strings as hardcoded 50 characters
	std::string strFormated = "";
	unsigned int id = entry.getID();
	unsigned int year = entry.getYear();
	unsigned int month = entry.getMonth();
	double price = entry.getPrice();
	size_t strSize = 0;
	
	//Store data as ID,Year,Month,Price,Vendor,Order#,Description
	writeFile.write(reinterpret_cast<char*>(&id), sizeof(id));
	writeFile.write(reinterpret_cast<char*>(&year), sizeof(year));
	writeFile.write(reinterpret_cast<char*>(&month), sizeof(month));
	writeFile.write(reinterpret_cast<char*>(&price), sizeof(price));
	
	//Store strings with fixed 50 char sizes
	strFormated = entry.getVendor();
	strFormated.resize(MAX_STR_SIZE, ' ');
	strSize = strFormated.size();
	writeFile.write(reinterpret_cast<char*>(&strSize), sizeof(strSize));
	writeFile.write(strFormated.c_str(), strSize);

	strFormated = entry.getOrderNum();
	strFormated.resize(MAX_STR_SIZE, ' ');
	strSize = strFormated.size();
	writeFile.write(reinterpret_cast<char*>(&strSize), sizeof(strSize));
	writeFile.write(strFormated.c_str(), strSize);

	strFormated = entry.getDescription();
	strFormated.resize(MAX_STR_SIZE, ' ');
	strSize = strFormated.size();
	writeFile.write(reinterpret_cast<char*>(&strSize), sizeof(strSize));
	writeFile.write(strFormated.c_str(), strSize);
}

void IOData::readEntry(Entry &entry, std::ifstream &readFile)
{
	//Temp string to format strings as hardcoded 50 characters
	std::string vendor = "";
	std::string order = "";
	std::string description = "";
	unsigned int id = 0;
	unsigned int year = 0;
	unsigned int month = 0;
	double price = 0.0;
	size_t strSize = 0;
	
	//Store data as ID,Year,Month,Price,Vendor,Order#,Description
	readFile.read(reinterpret_cast<char*>(&id), sizeof(id));
	readFile.read(reinterpret_cast<char*>(&year), sizeof(year));
	readFile.read(reinterpret_cast<char*>(&month), sizeof(month));
	readFile.read(reinterpret_cast<char*>(&price), sizeof(price));
	
	readFile.read(reinterpret_cast<char*>(&strSize),sizeof(strSize));
	vendor.resize(strSize);
	readFile.read(&vendor.front(),strSize);
	readFile.read(reinterpret_cast<char*>(&strSize),sizeof(strSize));
	order.resize(strSize);
	readFile.read(&order.front(),strSize);
	readFile.read(reinterpret_cast<char*>(&strSize),sizeof(strSize));
	description.resize(strSize);
	readFile.read(&description.front(),strSize);

	entry.appendEntry(year, month, price, id, vendor, description, order);
}

void IOData::editEntry(const HandleType &handle, const unsigned int &id, const std::string &dir,
		const std::string &vendor, const std::string &orderNumber, const std::string &description,
		const unsigned int &year, const unsigned int &month, const double &price)
{
	std::ifstream in(dir, std::ios::binary);
	std::ofstream out;
	std::streampos pos = 0;

	//Ignore editing if the ID is 0
	if(id > 0)
	{
		if(in.is_open())
		{
			//Start with pulling metadata
			getMetadata(in);
			if(id <= m_header.ItemCount)
			{
				//To prevent repeating the last print; only print after attempting to read the file
				//Causes unnecessary extra call
				while(in.good())
				{
					//Entry ID can only be edited on creation or once after - 
					//so entry is declared each loop
					Entry edEntry;
					
					pos = in.tellg();
					readEntry(edEntry, in);
					if(!in.eof())
					{
						if(edEntry.getID() == id)
						{
							//Append entry before overwriting; if removing overwrite with 0
							if(handle == HandleType::REMOVE)
							{
								edEntry.clear();
							}
							else
							{
								edEntry.appendEntry(year, month, price, id, vendor, description,
							orderNumber);
							}

							//Overwrite entry in database
							out.open(dir, std::ios::in | std::ios::binary);
							out.seekp(pos);
							writeEntry(edEntry, out);
							out.close();
							
							//Set pos to end of file to trigger in.eof()
							in.seekg(0, std::ios::end);
						}
					}
				}
			}
			else
			{
				std::cout << "No such ID exists" << std::endl;
			}
		}
		else
		{
			std::cout << "File does not exists" << std::endl;
		}
		
		in.close();
	}
}

void IOData::searchPrint(const unsigned int &id, const std::string &dir,
			const std::string &vendor, const std::string &orderNumber,
			const std::string &description, const unsigned int &year,
			const unsigned int &month, const double &price)
{
	std::ifstream in(dir, std::ios::binary);
	double total = 0;

	if(in.is_open())
	{
		//Start with pulling metadata
		getMetadata(in);

		std::cout << "fidb Transaction Book: " << m_header.Title << std::endl;
		std::cout << "Start Year: " << m_header.OriginYear	<< std::endl << std::endl;

		//To prevent repeating the last print; only print after attempting to read the file
		//Since its an extra call it could be handled to prevent processing
		while(in.good())
		{
			//Entry ID can only be edited on creation or once after - 
			//so entry is declared each loop
			Entry rdEntry;
			
			readEntry(rdEntry, in);
			if(!in.eof())
			{
				if(rdEntry.isMatched(id,year,month,price,vendor,orderNumber,description))
				{
					m_results++;
					total += rdEntry.getPrice();
					rdEntry.print();
				}
			}
		}

		//Display total hits of searched entries and list total price of transactions if found
		std::cout << m_results << " Result(s)" << std::endl;
		if(m_results)
		{
			std::cout << "Total $" << total << std::endl;
		}
	}
	else
	{
		std::cout << "File does not exists" << std::endl;
	}
	
	m_results = 0;
	in.close();
}