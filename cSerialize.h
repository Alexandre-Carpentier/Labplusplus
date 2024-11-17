#pragma once
#include <memory>
#include <string>
#include <wx/fileconf.h>
#include "data_types.h"

class cSerialize
{
public:
	cSerialize() = delete;
	cSerialize(std::string filename) : filename_(filename) 
	{
		wxString wxfilename_ = filename_;
		cfg = std::make_unique<wxFileConfig>(wxEmptyString, wxEmptyString, wxfilename_, wxEmptyString, wxCONFIG_USE_LOCAL_FILE, wxConvAuto());
		cfg->SetPath(wxfilename_);
	}
	~cSerialize() {};
	void save(std::string key, std::string data)
	{
		wxString wxkey = key;
		wxString wxdata = data;
		cfg->Write(wxkey, wxdata);
	}

	void load(std::string key, std::string& data)
	{
		data = cfg->Read(key, data);
	}

private:
	std::string filename_;
	std::unique_ptr<wxFileConfig> cfg;
};

static void serialize_item(cSerialize& saver, std::string key, bool item)
{
	assert(key.size() > 0);
	saver.save(key, std::to_string(item));
}

static void serialize_item(cSerialize& saver, std::string key, std::vector<bool> item)
{
	assert(key.size() > 0);
	size_t size = 0;
	for (auto it : item)
	{
		std::string subkey = key;
		subkey.append("_"); subkey.append(std::to_string(size));
		saver.save(subkey, std::to_string(it));
		size++;
	}
}

static void serialize_item(cSerialize& saver, std::string key, int item)
{
	assert(key.size() > 0);
	saver.save(key, std::to_string(item));
}

static void serialize_item(cSerialize& saver, std::string key, std::vector<int> item)
{
	assert(key.size() > 0);
	size_t size = 0;
	for (auto it : item)
	{
		std::string subkey = key;
		subkey.append("_"); subkey.append(std::to_string(size));
		saver.save(subkey, std::to_string(it));
		size++;
	}
}

static void serialize_item(cSerialize& saver, std::string key, std::vector<std::string> item)
{
	assert(key.size() > 0);
	size_t size = 0;
	for (auto it : item)
	{
		std::string subkey = key;
		subkey.append("_"); subkey.append(std::to_string(size));
		saver.save(subkey, it);
		size++;
	}
}

static void serialize_item(cSerialize& saver, std::string key, std::string item[], size_t length)
{
	assert(length <= max_chan_number);
	assert(length >= 0);
	assert(key.size() > 0);
	assert(item != nullptr);

	for (size_t size = 0; size < length; size++)
	{
		std::string subkey = key;
		subkey.append("_");
		subkey.append(std::to_string(size));
		size_t subsize = 0;
		saver.save(subkey, item[size]);
	}
}

static void serialize_item(cSerialize& saver, std::string key, wxString item)
{
	assert(key.size() > 0);
	saver.save(key, item.ToStdString());
}

static void serialize_item(cSerialize& saver, std::string key, wxString item[], size_t length)
{
	assert(length <= max_chan_number);
	assert(length >= 0);
	assert(key.size() > 0);
	assert(item != nullptr);
	for (size_t size = 0; size < length; size++)
	{
		std::string subkey = key;
		subkey.append("_");
		subkey.append(std::to_string(size));
		saver.save(subkey, item[size].ToStdString());
	}
}

static void serialize_item(cSerialize& saver, std::string key, wxArrayString item)
{
	assert(key.size() > 0);
	size_t size = 0;
	for (auto it : item)
	{
		std::string subkey = key;
		subkey.append("_"); subkey.append(std::to_string(size));
		saver.save(subkey, it.ToStdString());
		size++;
	}
}

static void serialize_item(cSerialize& saver, std::string key, wxArrayString item[], size_t length)
{
	assert(length <= max_chan_number);
	assert(length >= 0);
	assert(key.size() > 0);
	assert(item != nullptr);

	for (size_t size = 0; size < length; size++)
	{
		std::string subkey = key;
		subkey.append("_");
		subkey.append(std::to_string(size));
		size_t subsize = 0;
		for (auto subitem : item[size])
		{
			std::string sub_subkey = subkey;
			sub_subkey.append("_");
			sub_subkey.append(std::to_string(subsize));
			saver.save(sub_subkey, subitem.ToStdString());
			subsize++;
		}
	}
}

static void deserialize_item(cSerialize& saver, std::string key, bool& item)
{
	assert(key.size() > 0);
	std::string result;
	saver.load(key, result);
	if (result.size() > 0)
	{
		assert(result == "0" or result == "1");
		item = std::stoi(result);
	}
}

static void deserialize_item(cSerialize& saver, std::string key, std::vector<bool>& item)
{
	assert(key.size() > 0);
	for (size_t size = 0; size < max_chan_number; size++)
	{
		std::string subkey = key;
		subkey.append("_"); subkey.append(std::to_string(size));
		std::string result;
		saver.load(subkey, result);
		if (result.size() > 0)
		{
			assert(item.size() <= max_chan_number);
			assert(result == "0" or result == "1");
			item.push_back(std::stoi(result));
		}
	}
}

static void deserialize_item(cSerialize& saver, std::string key, int& item)
{
	assert(key.size() > 0);
	std::string result;
	saver.load(key, result);
	item = std::stoi(result);
}

static void deserialize_item(cSerialize& saver, std::string key, std::vector<int>& item)
{
	assert(key.size() > 0);
	assert(item.size() <= max_chan_number);
	for (size_t size = 0; size < max_chan_number; size++)
	{
		std::string subkey = key;
		subkey.append("_"); subkey.append(std::to_string(size));
		std::string result;
		saver.load(subkey, result);
		if (result.size() > 0)
		{
			item.push_back(std::stoi(result));
		}
	}
}

static void deserialize_item(cSerialize& saver, std::string key, std::vector<std::string>& item)
{
	assert(key.size() > 0);
	for (size_t size = 0; size < max_chan_number; size++)
	{
		std::string subkey = key;
		subkey.append("_"); subkey.append(std::to_string(size));
		std::string result;
		saver.load(subkey, result);
		if (result.size() > 0)
		{
			assert(item.size() <= max_chan_number);
			item.push_back(result);
		}
	}
}

static void deserialize_item(cSerialize& saver, std::string key, std::string item[], size_t length)
{
	assert(key.size() > 0);
	for (size_t size = 0; size < max_chan_number; size++)
	{
		std::string subkey = key;
		subkey.append("_"); subkey.append(std::to_string(size));
		std::string result;
		saver.load(subkey, result);
		if (result.size() > 0)
		{
			item[size] = result;
		}
	}
}

static void deserialize_item(cSerialize& saver, std::string key, wxString& item)
{
	assert(key.size() > 0);
	std::string result;
	saver.load(key, result);
	if (result.size() > 0)
	{
		item = result;
	}
}

static void deserialize_item(cSerialize& saver, std::string key, wxString item[], size_t length)
{
	assert(key.size() > 0);
	for (size_t size = 0; size < max_chan_number; size++)
	{
		std::string subkey = key;
		subkey.append("_"); subkey.append(std::to_string(size));
		std::string result;
		saver.load(subkey, result);
		if (result.size() > 0)
		{
			item[size] = result;
		}
	}
}

static void deserialize_item(cSerialize& saver, std::string key, wxArrayString& item)
{
	assert(key.size() > 0);
	for (size_t size = 0; size < max_chan_number; size++)
	{
		std::string subkey = key;
		subkey.append("_"); subkey.append(std::to_string(size));
		std::string result;
		saver.load(subkey, result);
		if (result.size() > 0)
		{
			wxString wx_result(result);
			item[size].append(wx_result);
		}
	}
}

static void deserialize_item(cSerialize& saver, std::string key, wxArrayString item[], size_t length)
{
	assert(key.size() > 0);
	for (size_t size = 0; size < max_chan_number; size++)
	{
		for (size_t subsize = 0; subsize < 3; subsize++)
		{
			std::string subkey = key;
			subkey.append("_"); subkey.append(std::to_string(size)); subkey.append("_"); subkey.append(std::to_string(subsize));
			std::string result;
			saver.load(subkey, result);
			if (result.size() > 0)
			{
				wxString wx_result(result);
				item[size][subsize].append(wx_result);
			}
		}
	}
}