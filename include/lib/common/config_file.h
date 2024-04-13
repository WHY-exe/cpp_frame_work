#pragma once
#include <iostream>
#include <string>
#include <vector>

namespace util {
struct Info {
  std::string key;
  std::string value;
  Info() noexcept = default;
  ~Info() noexcept = default;
  explicit Info(const std::string &key, const std::string &value) noexcept
      : key(key), value(value) {}
};

class ConfigSection {
  friend class Config;

private:
  std::string m_name;
  mutable std::vector<Info> m_config;

public:
  ConfigSection() noexcept = default;
  ~ConfigSection() noexcept = default;

private:
  explicit ConfigSection(const std::string &name) noexcept;
  void Init(const std::string &name) noexcept;
  void AddConf(const std::string &key, const std::string &value);
  friend std::ostream &operator<<(std::ostream &os,
                                  const ConfigSection &section);

public:
  bool IsInit() const noexcept;
  // 写配置文件
  void WriteConf(const std::string &key, const std::string &value);
  // 读配置文件
  const std::string &ReadConf(const std::string &key) const;

  std::string &operator[](const std::string &key);
  const std::string &operator[](const std::string &key) const;
};

class Config {
private:
  std::string m_file_path;
  mutable std::vector<ConfigSection> m_sections;

private:
  int InitConfig();

public:
  Config() noexcept = default;
  ~Config() noexcept = default;
  explicit Config(const std::string &file_path);
  // 初始化
  bool Init(const std::string &file_path);
  ConfigSection &operator[](const std::string &section_name);
  const ConfigSection &operator[](const std::string &section_name) const;

  ConfigSection &GetSection(const std::string &section_name);
  const ConfigSection &GetSection(const std::string &section_name) const;
  void Close();
  // 将所有项读取到map
  // 返回读取到的项数
  int UpdateConfig();
  // 将map中的所有项写入文件
  // 返回写入到的项数
  void UpdateFile() const;
};

} // namespace util
