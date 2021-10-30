#ifndef COLLADAINTERFACE_H
#define COLLADAINTERFACE_H

#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <iterator>

#include <gl\glew.h>
#include "tinyxml/tinyxml.h"

struct GeometryData
{
	GLenum type = 0U;
	unsigned int size = 0U;
	unsigned int stride = 0U;
	void* data = nullptr;
};

typedef std::map<std::string, GeometryData> geometryMap;

struct ColladaGeometry
{
	std::string name;
	geometryMap map;
	GLenum primitive = 0U;
	int index_count = 0;
	unsigned short* indices = nullptr;

	ColladaGeometry(){}
	~ColladaGeometry()
	{
		// Deallocate index data
		if (nullptr != indices)
		{
			free(indices);
		}

		// Deallocate array data in each map value
		for (auto map_it = map.begin(); map_it != map.end(); map_it++)
		{
			if(nullptr != (*map_it).second.data)
			{
				free((*map_it).second.data);
			}
		}
	}
};
typedef std::shared_ptr<ColladaGeometry> ColladaGeometryPtr;

GeometryData readSource(TiXmlElement*);

class ColladaInterface
{
private:
	static void loadInputElement(TiXmlElement*, ColladaGeometryPtr);
	static void loadPrimitiveType(TiXmlElement*, ColladaGeometryPtr);
public:
	ColladaInterface() {};
	static void readGeometries(std::vector<ColladaGeometryPtr>*, const char*);
	static void freeGeometries(std::vector<ColladaGeometryPtr>*);
};

#endif

