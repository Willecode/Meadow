#include "csvfile.h"

void CSVFile::addEntry(unsigned int frame, std::map<std::string, float> entries)
{
    mEntries.insert({ frame, entries });
}

void CSVFile::setColumns(std::vector<std::string> columns)
{
    mColumns = columns;
}

void CSVFile::clearEntries()
{
    mEntries.clear();
}

void CSVFile::clearColumns()
{
    mColumns.clear();
}

void CSVFile::writeFile(std::string s)
{
	std::ofstream myfile;
    myfile.open(s);

    // Add columns
    myfile << "Frame, ";
    for (int i = 0; i < mColumns.size(); i++) {
        myfile << mColumns[i];
        if (i + 1 != mColumns.size()) {// not last element
            myfile << ", ";
        }
    }
    myfile << "\n";
    for (auto const& entry : mEntries) {
        myfile << std::to_string(entry.first) + ", "; // "Frame" column
        for (int i = 0; i < mColumns.size(); i++) {
            myfile << entry.second.at(mColumns[i]);
            if (i + 1 != mColumns.size()) {// not last element
                myfile << ", ";
            }
        }
        myfile << "\n";
    }
    myfile.close();
    return;
}
