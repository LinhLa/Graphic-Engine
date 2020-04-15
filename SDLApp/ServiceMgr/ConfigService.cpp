#include "stdafx.h"
#include "ConfigService.h"
#include <fstream>
#include <inipp.h>
#include <INIReader.h>
#include <base_define.h>
#include <ServiceMgr.h>

ConfigService::ConfigService()
{
}


ConfigService::~ConfigService()
{
}

void ConfigService::onStartService()
{
	m_pServiceMgr->registerOnEvent(this, CONFIG_LOAD);

	//<Load and broadcast configuration from config.ini
	loadConfiguration();

	//<broadcast message config loaded
	m_pServiceMgr->sendMessage({ CONFIG_LOADED , 0ULL });
}

void ConfigService::onEndService()
{
	m_pServiceMgr->unregisterOnEvent(this, CONFIG_LOAD);
}
void ConfigService::loadConfiguration()
{
	//<Read config.ini
	inipp::Ini<char> ini;
	std::ifstream config_ifstream(CONFIG_FILE);
	_ASSERT(config_ifstream.is_open());
	ini.parse(config_ifstream);

	std::string str_return;

	inipp::extract(ini.sections["Log"]["level"], m_config.log_level);
	m_pServiceMgr->sendMessage({ CONFIG_UPDATE_LOG_LEVEL , uint64_t(m_config.log_level) });

	inipp::extract(ini.sections["Graphic"]["fullscreen"], m_config.full_screen);
	m_pServiceMgr->sendMessage({ CONFIG_UPDATE_FULL_SCREEN , m_config.full_screen });

	inipp::extract(ini.sections["Graphic"]["heigh"], m_config.height);
	m_pServiceMgr->sendMessage({ CONFIG_UPDATE_SCREEN_HEIGHT , int64_t(m_config.height) });

	inipp::extract(ini.sections["Graphic"]["width"], m_config.width);
	m_pServiceMgr->sendMessage({ CONFIG_UPDATE_SCREEN_WIDTH , int64_t(m_config.width) });

	inipp::extract(ini.sections["Memory"]["pool_size"], m_config.pool_size);
	inipp::extract(ini.sections["Memory"]["chunk_size"], m_config.chunk_size);

	inipp::extract(ini.sections["Texture"]["data_file"], str_return);
	memcpy_s(m_config.data_file, sizeof(m_config.data_file), str_return.data(), str_return.size());
	m_pServiceMgr->sendMessage({ CONFIG_UPDATE_DATA_FILE, message_data(m_config.data_file, strnlen(m_config.data_file, sizeof(m_config.data_file))) });

	inipp::extract(ini.sections["Texture"]["asset_path"], str_return);
	memcpy_s(m_config.asset_path, sizeof(m_config.asset_path), str_return.data(), str_return.size());
	m_pServiceMgr->sendMessage({ CONFIG_UPDATE_ASSET_PATH , message_data(m_config.asset_path, strnlen(m_config.asset_path, sizeof(m_config.asset_path))) });

	inipp::extract(ini.sections["General"]["version"], str_return);
	memcpy_s(m_config.version, sizeof(m_config.version), str_return.data(), str_return.size());
	m_pServiceMgr->sendMessage({ CONFIG_UPDATE_VERSION , message_data(m_config.version, strnlen(m_config.version, sizeof(m_config.version))) });

	inipp::extract(ini.sections["Graphic"]["fps"], m_config.fps);
	m_pServiceMgr->sendMessage({ CONFIG_UPDATE_FPS , uint64_t(m_config.fps) });
}

void ConfigService::onMessageReceive(const Message &msg)
{
	switch (msg.m_uValue)
	{
	case CONFIG_LOAD:
		//<Load and broadcast configuration from config.ini
		loadConfiguration();

		//<broadcast message config loaded
		m_pServiceMgr->sendMessage({ CONFIG_LOADED , 0ULL });

		break;
	default:
		break;
	}
}