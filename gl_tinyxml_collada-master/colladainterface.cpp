#include "colladainterface.h"
#include "log.h"

char array_types[7][15] = { "float_array", "int_array", "bool_array", "Name_array",
						   "IDREF_array", "SIDREF_array", "token_array" };

char primitive_types[7][15] = { "lines", "linestrips", "polygons", "polylist",
							   "triangles", "trifans", "tristrips" };

void ColladaInterface::loadInputElement(TiXmlElement* mesh, ColladaGeometryPtr p_data)
{
	TiXmlElement* vertices = mesh->FirstChildElement("vertices");
	TiXmlElement* input = vertices->FirstChildElement("input");
	std::string geometry_name;
	// Iterate through input elements 
	while (input != NULL)
	{
		std::string geometry_name = std::string(input->Attribute("source"));
		geometry_name = geometry_name.erase(0, 1);
		TiXmlElement* source = mesh->FirstChildElement("source");

		// Iterate through source elements 
		while (source != NULL)
		{
			if (std::string(source->Attribute("id")) == geometry_name)
			{
				p_data->map[std::string(input->Attribute("semantic"))] = readSource(source);
			}

			source = source->NextSiblingElement("source");
		}

		input = input->NextSiblingElement("input");
	}
}

void ColladaInterface::loadPrimitiveType(TiXmlElement* mesh, ColladaGeometryPtr p_data)
{
	for (int i = 0; i < 7; i++)
	{
		TiXmlElement* primitive = mesh->FirstChildElement(primitive_types[i]);
		if (primitive != NULL)
		{
			int num_indices, prim_count;
			// Determine number of primitives
			primitive->QueryIntAttribute("count", &prim_count);

			// Determine primitive type and set count
			switch (i) {
			case 0:
				p_data->primitive = GL_LINES;
				num_indices = prim_count * 2;
				break;
			case 1:
				p_data->primitive = GL_LINE_STRIP;
				num_indices = prim_count + 1;
				break;
			case 4:
				p_data->primitive = GL_TRIANGLES;
				num_indices = prim_count * 3;
				break;
			case 5:
				p_data->primitive = GL_TRIANGLE_FAN;
				num_indices = prim_count + 2;
				break;
			case 6:
				p_data->primitive = GL_TRIANGLE_STRIP;
				num_indices = prim_count + 2;
				break;
			default: std::cout << "Primitive " << primitive_types[i] <<
				" not supported" << std::endl;
			}
			p_data->index_count = num_indices;

			// Allocate memory for indices
			p_data->indices = (unsigned short*)malloc(num_indices * sizeof(unsigned short));

			// Read the index values
			char* text = (char*)(primitive->FirstChildElement("p")->GetText());
			p_data->indices[0] = (unsigned short)atoi(strtok(text, " "));
			for (int index = 1; index < num_indices; index++)
			{
				p_data->indices[index] = (unsigned short)atoi(strtok(NULL, " "));
			}
		}
	}
}

void ColladaInterface::readGeometries(std::vector<ColladaGeometryPtr>* v, const char* filename)
{
	// Create document and load COLLADA file
	TiXmlDocument doc(filename);
	if (!doc.LoadFile())
	{
		LOG_DEBUG("ERROR XML LOAD FILE : %s", filename);
		_ASSERT(false);
	}
	TiXmlElement* geometry = doc.RootElement()->FirstChildElement("library_geometries")->FirstChildElement("geometry");

	// Iterate through geometry elements 
	while (geometry != NULL)
	{
		// Create new geometry
		ColladaGeometryPtr p_data(new ColladaGeometry());

		// Set the geometry name
		p_data->name = geometry->Attribute("id");

		// Iterate through mesh elements 
		TiXmlElement* mesh = geometry->FirstChildElement("mesh");
		while (mesh != NULL)
		{
			loadInputElement(mesh, p_data);

			// Determine primitive type
			loadPrimitiveType(mesh, p_data);

			mesh = mesh->NextSiblingElement("mesh");
		}
		v->push_back(p_data);

		geometry = geometry->NextSiblingElement("geometry");
	}
}

void ColladaInterface::freeGeometries(std::vector<ColladaGeometryPtr>* v)
{
	// for (auto geom_it = v->begin(); geom_it < v->end(); geom_it++)
	// {

	// 	// Deallocate index data
	// 	free(geom_it->indices);

	// 	// Deallocate array data in each map value
	// 	for (auto map_it = geom_it->map.begin(); map_it != geom_it->map.end(); map_it++)
	// 	{
	// 		free((*map_it).second.data);
	// 	}

	// 	// Erase the current ColladaGeometry from the vector
	// 	v->erase(geom_it);
	// }
	v->clear();
}

GeometryData readSource(TiXmlElement* source)
{
	GeometryData geometry_data;
	unsigned int num_vals, stride;

	for (int i = 0; i < 7; i++) {
		TiXmlElement* array = source->FirstChildElement(array_types[i]);
		if (array != NULL)
		{

			// Find number of values
			array->QueryUnsignedAttribute("count", &num_vals);
			geometry_data.size = num_vals;

			// Find stride
			int check = source->FirstChildElement("technique_common")->FirstChildElement("accessor")->QueryUnsignedAttribute("stride", &stride);
			if (check != TIXML_NO_ATTRIBUTE)
				geometry_data.stride = stride;
			else
				geometry_data.stride = 1;

			// Read array values
			char* text = (char*)(array->GetText());

			// Initialize mesh data according to data type
			switch (i)
			{
			case 0:// Array of floats
				geometry_data.type = GL_FLOAT;
				geometry_data.size *= sizeof(float);
				geometry_data.data = malloc(num_vals * sizeof(float));

				// Read the float values
				((float*)geometry_data.data)[0] = static_cast<float>(atof(strtok(text, " ")));
				for (unsigned int index = 1; index < num_vals; index++)
				{
					((float*)geometry_data.data)[index] = static_cast<float>(atof(strtok(NULL, " ")));
				}
				break;

			case 1:// Array of integers
				geometry_data.type = GL_INT;
				geometry_data.size *= sizeof(int);
				geometry_data.data = malloc(num_vals * sizeof(int));

				// Read the int values
				((int*)geometry_data.data)[0] = atoi(strtok(text, " "));
				for (unsigned int index = 1; index < num_vals; index++)
				{
					((int*)geometry_data.data)[index] = atoi(strtok(NULL, " "));
				}
				break;

				// Other
			default:
				std::cout << "Collada Reader doesn't support mesh data in this format" << std::endl;
				break;
			}
		}
	}
	return geometry_data;
}
