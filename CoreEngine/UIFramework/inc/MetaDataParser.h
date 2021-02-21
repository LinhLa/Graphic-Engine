#pragma once
#include "creator.h"
class MetaDataParser : public creator<MetaDataParser>
{
private:
	MetaDataParser();
public:
	~MetaDataParser();
	friend class creator<MetaDataParser>;

	void parse();

};

typedef std::shared_ptr<MetaDataParser> MetaDataParserPtr;