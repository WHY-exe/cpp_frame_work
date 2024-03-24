#include "config_file.h"

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <regex>
#include <sstream>

#include "util.h"

namespace util {
Config::Config(const std::string &file_path) : Config() { Init(file_path); }

// 初始化
bool Config::Init(const std::string &file_path) {
  if (!std::filesystem::exists(file_path)) {
    return false;
  }
  if (file_path.empty() || file_path.length() >= MAX_PATH_LEN) {
    return false;
  }
  m_file_path = file_path;
  m_sections.clear();
  InitConfig();
  return true;
}

ConfigSection &Config::operator[](const std::string &section_name) {
  return GetSection(section_name);
}

const ConfigSection &Config::operator[](const std::string &section_name) const {
  return GetSection(section_name);
}

ConfigSection &Config::GetSection(const std::string &section_name) {
  auto it = std::find_if(m_sections.begin(), m_sections.end(),
                         [&section_name](const ConfigSection &cur) -> bool {
                           return cur.m_name == section_name;
                         });
  if (it == m_sections.end()) {
    return m_sections.emplace_back(ConfigSection(section_name));
  }
  return *it;
}

const ConfigSection &Config::GetSection(const std::string &section_name) const {
  auto it = std::find_if(m_sections.cbegin(), m_sections.cend(),
                         [&section_name](const ConfigSection &cur) -> bool {
                           return cur.m_name == section_name;
                         });
  if (it == m_sections.cend()) {
    return m_sections.emplace_back(ConfigSection(section_name));
  }
  return *it;
}

void Config::Close() {
  m_file_path = "";
  m_sections.clear();
}

int Config::UpdateConfig() {
  Close();
  return InitConfig();
}

int Config::InitConfig() {
  std::regex sect_name_regex(R"(\[\w+?\])");
  std::smatch sm;
  std::ifstream ifs(m_file_path);
  ConfigSection section;
  int line_num = 0;
  for (std::string line; std::getline(ifs, line);) {
    line_num++;
    if (line[0] == '#' || line[0] == '\n') {
      continue;
    }
    if (std::regex_search(line, sm, sect_name_regex)) {
      section.Init(sm.str().substr(1, sm.str().length() - 2));
      if (line_num != 1) {
        m_sections.push_back(std::move(section));
        continue;
      }
    }
    const auto tmp = util::Split(line, "=");
    if (tmp.size() != 2) {
      continue;
    }

    section.AddConf(tmp[0], tmp[1]);
  }
  ifs.close();
  m_sections.push_back(std::move(section));
  return (int)m_sections.size();
}

void Config::UpdateFile() const {
  std::stringstream ss_bak_file;
  ss_bak_file << m_file_path << "_bak";
  std::ofstream ofs(ss_bak_file.str(), std::ios::out | std::ios::trunc);
  for (const auto &it : m_sections) {
    ofs << it;
  }
  ofs.close();
  std::filesystem::copy(ss_bak_file.str(), m_file_path);
  std::filesystem::remove(ss_bak_file.str());
}

ConfigSection::ConfigSection(const std::string &name) noexcept : m_name(name) {}

void ConfigSection::Init(const std::string &name) noexcept {
  m_config.clear();
  m_name = name;
}

bool ConfigSection::IsInit() const noexcept { return m_name.empty(); }

void ConfigSection::WriteConf(const std::string &key,
                              const std::string &value) {
  auto it =
      std::find_if(m_config.begin(), m_config.end(),
                   [&key](const Info &cur) -> bool { return cur.key == key; });
  if (it != m_config.end()) {
    it->value = value;
  } else {
    m_config.emplace_back(Info{key, value});
  }
}

void ConfigSection::AddConf(const std::string &key, const std::string &value) {
  m_config.emplace_back(Info{key, value});
}

const std::string &ConfigSection::ReadConf(const std::string &key) const {
  // 查找是否该key是否已经存在
  auto it =
      std::find_if(m_config.cbegin(), m_config.cend(),
                   [&key](const Info &cur) -> bool { return cur.key == key; });
  if (it == m_config.cend()) {
    return m_config.emplace_back(Info{key, ""}).value;
  }
  return it->value;
}

std::string &ConfigSection::operator[](const std::string &key) {
  // 查找是否该key是否已经存在
  auto it =
      std::find_if(m_config.begin(), m_config.end(),
                   [&key](const Info &cur) -> bool { return cur.key == key; });
  if (it == m_config.end()) {
    return m_config.emplace_back(Info{key, ""}).value;
  }
  return it->value;
}

const std::string &ConfigSection::operator[](const std::string &key) const {
  return ReadConf(key);
}

std::ostream &operator<<(std::ostream &os, const ConfigSection &section) {
  for (const auto &i : section.m_config) {
    os << i.key << "=" << i.value << std::endl;
  }
  return os;
}

}  // namespace util