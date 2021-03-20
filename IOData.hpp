#ifndef IODATA_HPP
#define IODATA_HPP

#include <filesystem>
#include <fstream>
#include <limits>
#include <string>
#include "Entry.hpp"

enum HandleType {EDIT, REMOVE};

struct MetaData
{
	size_t			stringSize	= 0;
	unsigned int	OriginYear	= 1900;
	unsigned int	ItemCount	= 0;
	std::string		Title		= "";
};

class IOData
{
	private:
		MetaData m_header;
		unsigned int m_results;

		bool fileExists(const std::string &dir) {return std::filesystem::exists(dir);};
	public:
		IOData();
		~IOData();
		bool createDB(const std::string &dir);
		bool saveEntry(const std::string &dir);
		void getMetadata(std::ifstream &readFile);
		void setMetadata(MetaData &data);
		void updateMetadataItemCount(std::ofstream &writeFile);
		void writeEntry(const Entry &entry, std::ofstream &writeFile);
		void readEntry(Entry &entry, std::ifstream &readFile);
		void editEntry(const HandleType &handle, const unsigned int &id, const std::string &dir,
			const std::string &vendor = "", const std::string &orderNumber = "",
			const std::string &description = "", const unsigned int &year = 0,
			const unsigned int &month = 0, const double &price = 0.0);
		void searchPrint(const unsigned int &id, const std::string &dir,
			const std::string &vendor = "", const std::string &orderNumber = "",
			const std::string &description = "", const unsigned int &year = 0,
			const unsigned int &month = 0, const double &price = 0.0);
};

#endif //IODATA_HPP
