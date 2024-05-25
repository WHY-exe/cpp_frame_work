#include "config_file.h"
#include "exception.h"
#include "util.h"
#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace util {
Config::Section::Section(std::string &&name) noexcept { Init(std::move(name)); }

void Config::Section::Init(std::string &&name) noexcept {
  config_.clear();
  name_ = std::move(name);
}

void Config::Section::AddConf(std::string &&key, std::string &&value) {
  config_.emplace_back(Info{std::move(key), std::move(value)});
}

bool Config::Section::IsInit() const noexcept { return !name_.empty(); }

void Config::Section::WriteConf(const std::string &key,
                                const std::string &value) {
  auto it =
      std::find_if(config_.begin(), config_.end(),
                   [&key](const Info &cur) -> bool { return cur.key == key; });
  if (it != config_.end()) {
    it->value = value;
  } else {
    config_.emplace_back(Info{key, value});
  }
}

const std::string &Config::Section::ReadConf(const std::string &key) const {
  // 查找是否该key是否已经存在
  auto it =
      std::find_if(config_.cbegin(), config_.cend(),
                   [&key](const Info &cur) -> bool { return cur.key == key; });
  if (it == config_.cend()) {
    return config_.emplace_back(Info{key, ""}).value;
  }
  return it->value;
}

std::string &Config::Section::operator[](const std::string &key) {
  // 查找是否该key是否已经存在
  auto it =
      std::find_if(config_.begin(), config_.end(),
                   [&key](const Info &cur) -> bool { return cur.key == key; });
  if (it == config_.end()) {
    return config_.emplace_back(Info{key, ""}).value;
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
  auto it = std::find_if(sections_.begin(), sections_.end(),
                         [&section_name](const Section &cur) -> bool {
                           return cur.name_ == section_name;
                         });
  if (it == sections_.end()) {
    return sections_.emplace_back(Section(std::move(section_name)));
  }
  return *it;
}

std::ostream &operator<<(std::ostream &os, const Config::Section &section) {
  if (!section.name_.empty()) {
    os << "[" << section.name_ << "]" << std::endl;
  }
  for (const auto &i : section.config_) {
    os << i.key << "=" << i.value << std::endl;
  }
  return os;
}

Config::Config(const std::string &file_path) {
  if (file_path.empty()) {
    THROW_EXCEPTION("file_path is empty", "util");
  }
  if (file_path.length() >= MAX_PATH_LEN) {
    char err_buf[MAX_BUFFER_SIZE];
    snprintf(err_buf, MAX_BUFFER_SIZE, "file_path too long: %lld",
             file_path.length());
    THROW_EXCEPTION(err_buf, "util");
  }
  // create the file path if not exists
  std::filesystem::path path(file_path);
  if (!std::filesystem::exists(path)) {
    // create the parent dir first if the dir is no exists
    if (!std::filesystem::exists(path.parent_path())) {
      std::filesystem::create_directory(path.parent_path());
    }
    std::ofstream ofs(file_path);
  }

  file_.open(file_path, file_.in | file_.out);
  if (!file_.is_open()) {
    THROW_EXCEPTION("fail to open file", "util");
  }
}

int Config::ReadConfig() {
  sections_.clear();
  // use regex to tell wether it should be
  // a section
  std::regex sect_name_regex(R"(\[\w+?\])");
  std::smatch sm;
  // open the section file
  Section section;
  std::vector<std::string> file_content;
  // iterate through every line
  for (std::string line; std::getline(file_, line);) {
    // ignore the comment and the newline character
    if (std::string_view("#\n;").find(line[0]) != std::string::npos) {
      continue;
    }
    // tell wether the line content match the
    // config section style
    if (std::regex_search(line, sm, sect_name_regex)) {
      section.Init(sm.str().substr(1, sm.str().length() - 2));
      if (!section.IsInit() && !sections_.empty()) {
        sections_.push_back(std::move(section));
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
  file_.seekp(0);
  sections_.push_back(std::move(section));
  return (int)sections_.size();
}

void Config::WriteFile() {
  for (const auto &it : sections_) {
    file_ << it;
  }
  file_.seekp(0);
}

void Config::Close() {
  file_.close();
  sections_.clear();
}

} // namespace util