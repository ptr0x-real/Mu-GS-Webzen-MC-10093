#pragma once

class TokenizerRow
{
public:
	std::map<DWORD, std::string>	Columns;
	int	ColumnCount;

	std::string GetString(DWORD Column, std::string Default = "")
	{
		std::map<DWORD, std::string>::iterator it = this->Columns.find(Column);
		if (it == this->Columns.end())
		{
			return Default;
		}
		return it->second;
	}

	char* GetStringPtr(DWORD Column, std::string Default = "")
	{
		std::map<DWORD, std::string>::iterator it = this->Columns.find(Column);
		if (it == this->Columns.end())
		{
			return (char*)Default.c_str();
		}
		return (char*)it->second.c_str();
	}

	int GetInt(DWORD Column, DWORD Default = -1, BOOL Hex = FALSE)
	{
		std::map<DWORD, std::string>::iterator it = this->Columns.find(Column);

		if (it == this->Columns.end())
		{
			return Default;
		}

		if (Hex)
		{
			unsigned int pOffset = Default;
			sscanf_s(it->second.c_str(), "%x", &pOffset);
			return pOffset;
		}

		return atoi(it->second.c_str());

	}

	double GetFloat(DWORD Column, double Default = 0.0f)
	{
		std::map<DWORD, std::string>::iterator it = this->Columns.find(Column);
		if (it == this->Columns.end())
		{
			return Default;
		}
		return atof(it->second.c_str());
	}

};

class TokenizerSection
{
public:
	std::map<DWORD, TokenizerRow>	Rows;
	int							RowCount;
};

class TokenizerGroup
{
public:
	std::map<DWORD, TokenizerSection>	Sections;

	bool GetSection(DWORD Index, TokenizerSection & section)
	{
		std::map<DWORD, TokenizerSection>::iterator it = this->Sections.find(Index);
		if (it == this->Sections.end())
		{
			return false;
		}
		else
		{
			section = it->second;
			return true;
		}
	}

};

class Tokenizer
{
private:
	char*	m_pBuffer;
	DWORD	m_pBufferSize;
	DWORD	m_pBufferIndex;

public:

	bool ParseLine(std::string line, TokenizerRow & pRow)
	{

		std::string data = "";

		char* dump = (char*)line.c_str();

		bool openstring = false;
		bool clearingspace = true;

		int column = 0;

		for (unsigned int i = 0; i < line.length(); i++)
		{

			if (clearingspace)
			{
				if (dump[i] == ' ' || dump[i] == '\t')
				{
					continue;
				}
				clearingspace = false;
			}

			if (openstring)
			{
				if (dump[i] == '"')
				{
					openstring = false;
					continue;
				}
				data += dump[i];
			}
			else
			{
				if (dump[i] == '"')
				{
					if (data != "")
					{
						return false;
					}
					openstring = true;
					continue;
				}
				else
				{
					if (dump[i] == '\t' || dump[i] == ' ')
					{
						if (data != "")
						{
							pRow.Columns[column++] = data;
							data = "";
						}
						continue;
					}
					data += dump[i];
				}
			}

		}

		if (data != "")
		{
			pRow.Columns[column++] = data;
		}

		data = "";

		pRow.ColumnCount = column;

		return true;

	}

	bool ParseFile(std::string file, TokenizerGroup & tok)
	{

		std::fstream f(file.c_str(), std::ios::in);

		if (f.is_open())
		{

			if (f.good())
			{

				TokenizerSection sec = TokenizerSection();
				int current_sec = 0;
				int sec_index = 0;
				bool sec_open = false;

				while (!f.eof())
				{

					char temp[4096];
					char* dump = NULL;
					std::string line = "";

					ZeroMemory(&temp[0], 4096);

					f.getline(&temp[0], 4095);
					line.assign(&temp[0]);

					dump = (char*)line.c_str();

					int start = 0;
					int end = 0;

					for (DWORD i = 0; i < line.length(); i++)
					{
						if (dump[i] != ' ' && dump[i] != '\t')
						{
							break;
						}
						start++;
					}

					for (DWORD i = line.length() - 1; i > 0; i--)
					{
						if (dump[i] != ' ' && dump[i] != '\t')
						{
							break;
						}
						end++;
					}

					line = line.substr(start, line.length() - end - start);

					std::string::size_type k = 0;
					while ((k = line.find(13, k)) != line.npos)
					{
						line.erase(k, 1);
					}

					if (line.substr(0, 2) == "//") continue;
					if (line.substr(0, 1) == ";") continue;


					if (line.compare("end") == false)
					{
						if (sec_open == false)
						{
							return false;
						}

						sec_open = false;
						sec.RowCount = sec_index;
						tok.Sections[current_sec] = sec;
						continue;
					}

					if (line == "") continue;

					TokenizerRow row;
					if (!this->ParseLine(line, row))
						return false;

					if (row.ColumnCount == 1)
					{
						if (sec_open == false)
						{
							sec_index = 0;
							current_sec = row.GetInt(0, 0);
							sec = TokenizerSection();
							sec_open = true;
							continue;
						}
					}

					sec.Rows[sec_index++] = row;
				}

			}
			else
			{
				f.close();
				return false;
			}

			f.close();

			return true;

		}
		else
		{
			return false;
		}

	}

};