#ifndef ENTRY_HPP
#define ENTRY_HPP

#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <locale>
#include <string>

const size_t MAX_STR_SIZE = 50;

class Entry
{
	private:	
		std::string		m_vendor;
		std::string		m_orderNum;
		std::string 	m_description;
		unsigned int	m_id;
		unsigned int	m_year;
		unsigned int	m_month;
		double			m_price;

	public:
		Entry();
		Entry(unsigned int year, unsigned int month, double price, unsigned int id,
				std::string vendor,	std::string description, std::string orderNum);
		~Entry();

		void	setVendor(const std::string &vendor);
		void	setOrderNum(const std::string &orderNum);
		void	setDescription(const std::string &descr);
		void	setID(const unsigned int &id);
		void	setYear(const unsigned int &year);
		void	setPrice(const double &price);
		void	setMonth(const unsigned int &month);

		std::string 	getVendor()		const	{return m_vendor;};
		std::string 	getOrderNum()	const	{return m_orderNum;};
		std::string 	getDescription()const	{return m_description;};
		unsigned int 	getID()			const	{return m_id;};
		unsigned int 	getYear()		const	{return m_year;};
		unsigned int 	getMonth()		const	{return m_month;};
		double 			getPrice()		const	{return m_price;};

		bool isMatched(const unsigned int &id, const unsigned int year, const unsigned int &month,
			const double &price, const std::string &vendor, const std::string &order,
			const std::string &description);
		bool compare(const std::string &str1, const std::string &str2);
		void addNewEntry(const std::string &dbTitle, unsigned int &id);
		void appendEntry(const unsigned int &year, const unsigned int &month, const double &price,
			const unsigned int &id,	const std::string &vendor, const std::string &description,
			const std::string &orderNum);
		void clear();
		void print();
};

#endif //ENTRY_HPP
