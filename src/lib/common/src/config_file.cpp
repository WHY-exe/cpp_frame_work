#include "config_file.h"

#include "spdlog/fmt/fmt.h"
#include "util.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <regex>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace util {
Config::Section::Section(std::string &&name) noexcept { Init(std::move(name)); }

void Config::Section::Init(std::string &&name) noexcept {
  m_config.clear();
  m_name = std::move(name);
}

void Config::Section::AddConf(std::string &&key, std::string &&value) {
  m_config.emplace_back(Info{std::move(key), std::move(value)});
}

bool Config::Section::IsInit() const noexcept { return !m_name.empty(); }

void Config::Section::WriteConf(const std::string &key,
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

const std::string &Config::Section::ReadConf(const std::string &key) const {
  // 查找是否该key是否已经存在
  auto it =
      std::find_if(m_config.cbegin(), m_config.cend(),
                   [&key](const Info &cur) -> bool { return cur.key == key; });
  if (it == m_config.cend()) {
    return m_config.emplace_back(Info{key, ""}).value;
  }
  return it->value;
}

std::string &Config::Section::operator[](const std::string &key) {
  // 查找是否该key是否已经存在
  auto it =
      std::find_if(m_config.begin(), m_config.end(),
                   [&key](const Info &cur) -> bool { return cur.key == key; });
  if (it == m_config.end()) {
    return m_config.emplace_back(Info{key, ""}).value;
  }
  return it->value;
}

const std::string &Config::Section::operator[](const std::string &key) const {
  return ReadConf(key);
}

Config::Section &Config::operator[](std::string &&section_name) {
  return GetSection(std::move(section_name));
}

Config::Section &Config::GetSection(std::string &&section_name) {
  auto it = std::find_if(m_sections.begin(), m_sections.end(),
                         [&section_name](const Section &cur) -> bool {
                           return cur.m_name == section_name;
                         });
  if (it == m_sections.end()) {
    return m_sections.emplace_back(Section(std::move(section_name)));
  }
  return *it;
}

std::ostream &operator<<(std::ostream &os, const Config::Section &section) {
  os << "[" << section.m_name << "]" << std::endl;
  for (const auto &i : section.m_config) {
    os << i.key << "=" << i.value << std::endl;
  }
  return os;
}

Config::Config(const std::string &file_path) : Config() { Init(file_path); }

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

void Config::Clear() noexcept { m_sections.clear(); }

int Config::UpdateConfig() {
  Clear();
  return InitConfig();
}

int Config::InitConfig() {
  // use regex to tell wether it should be
  // a section
  std::regex sect_name_regex(R"(\[\w+?\])");
  std::smatch sm;
  // open the section file
  std::ifstream ifs(m_file_path);
  Section section;
  std::vector<std::string> file_content;
  // iterate through every line
  for (std::string line; std::getline(ifs, line);) {
    // ignore the comment and the newline character
    if (std::string_view("#\n;").find(line[0]) != std::string::npos) {
      continue;
    }
    // tell wether the line content match the
    // config section style
    if (std::regex_search(line, sm, sect_name_regex)) {
      section.Init(sm.str().substr(1, sm.str().length() - 2));
      if (!section.IsInit() && !m_sections.empty()) {
        m_sections.push_back(std::move(section));
        continue;
      }
    } else if (!section.IsInit()) {
      section.Init("");
    }
    // insert the value to the section
    auto tmp = util::Split(line, "=");
    if (tmp.size() != 2) {
      continue;
    }
    section.AddConf(std::move(tmp[0]), std::move(tmp[1]));
  }
  ifs.close();
  m_sections.push_back(std::move(section));
  return (int)m_sections.size();
}

void Config::UpdateFile() const {
  const auto bak_file_path = fmt::format("{}_bak", m_file_path);
  std::ofstream ofs(bak_file_path, std::ios::out | std::ios::trunc);
  if (!ofs.is_open()) {
    return;
  }
  for (const auto &it : m_sections) {
    ofs << it;
  }
  ofs.close();
  std::filesystem::rename(bak_file_path, m_file_path);
}

void Config::Close() {
  UpdateFile();
  Clear();
}

} // namespace util